#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <memory>
#include <cmath>
#include <algorithm>

namespace GoatDSP
{

enum class ClippingMode
{
    VintageHM2 = 0, // Silicon + Germanium asymmetric diode clipping & heavy compression
    GoatHotRod = 1  // Silicon + LED asymmetric clipping with higher headroom and punch
};

class GoatDSPChain
{
public:
    GoatDSPChain() = default;

    void prepare(double sampleRate, int samplesPerBlock, int numChannels)
    {
        currentSampleRate = sampleRate;
        channels = numChannels;
        maxBlockSize = samplesPerBlock;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
        spec.numChannels = static_cast<juce::uint32>(numChannels);

        // 1. DC Blocker (~20 Hz HPF)
        dcBlocker.prepare(spec);
        dcBlocker.reset();
        *dcBlocker.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 20.0f);

        // 2. Pre-Emphasis High-Pass Filter (72.3 Hz 2nd-order Butterworth HPF)
        preHpFilter.prepare(spec);
        preHpFilter.reset();
        *preHpFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 72.3f, 0.7071f);

        // 3. Pre-Emphasis Treble Shelf / Input Buffer Simulation (1.8 kHz, +2.0 dB, Q = 0.707)
        preTrebleShelf.prepare(spec);
        preTrebleShelf.reset();
        *preTrebleShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 1800.0f, 0.7071f, juce::Decibels::decibelsToGain(2.0f));

        // 4. Passive Mid-Scoop Notch (530 Hz, Q = 0.8, -6.5 dB)
        midScoopFilter.prepare(spec);
        midScoopFilter.reset();
        *midScoopFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 530.0f, 0.8f, juce::Decibels::decibelsToGain(-6.5f));

        // 5. Active Dual-Gyrators
        // Low Gyrator: 100 Hz, Q = 2.1, +/- 18 dB
        lowGyrator.prepare(spec);
        lowGyrator.reset();
        updateLowGyrator(18.0f, true);

        // High Gyrator: 1300 Hz, Q = 3.5, +/- 20 dB
        highGyrator.prepare(spec);
        highGyrator.reset();
        updateHighGyrator(20.0f, true);

        // 6. Post-EQ Output Low-Pass Filter (6.8 kHz 2-pole Low-Pass)
        postCabLpf.prepare(spec);
        postCabLpf.reset();
        *postCabLpf.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 6800.0f, 0.7071f);

        // Scratch dry buffer
        dryScratchBuffer.setSize(numChannels, samplesPerBlock);

        // Initialize smoothed parameters
        smoothedInGain.reset(sampleRate, 0.02);
        smoothedDistGain.reset(sampleRate, 0.02);
        smoothedLowDb.reset(sampleRate, 0.02);
        smoothedHighDb.reset(sampleRate, 0.02);
        smoothedOutGain.reset(sampleRate, 0.02);
        smoothedBlend.reset(sampleRate, 0.02);
        smoothedBypass.reset(sampleRate, 0.02);

        // Initialize 4x polyphase oversampling
        setOversamplingFactor(4);
    }

    void reset()
    {
        dcBlocker.reset();
        preHpFilter.reset();
        preTrebleShelf.reset();
        midScoopFilter.reset();
        lowGyrator.reset();
        highGyrator.reset();
        postCabLpf.reset();
        if (oversampling)
            oversampling->reset();
    }

    void setOversamplingFactor(int factor)
    {
        if (factor != 1 && factor != 2 && factor != 4 && factor != 8)
            factor = 4;

        if (currentOversamplingFactor == factor && oversampling != nullptr && factor != 1)
            return;

        currentOversamplingFactor = factor;

        if (factor == 1)
        {
            oversampling.reset();
            return;
        }

        const size_t order = (factor == 2) ? 1 : ((factor == 4) ? 2 : 3);
        oversampling = std::make_unique<juce::dsp::Oversampling<float>>(
            static_cast<size_t>(channels),
            order,
            juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
            true
        );

        oversampling->initProcessing(static_cast<size_t>(maxBlockSize));
    }

    int getOversamplingFactor() const { return currentOversamplingFactor; }

    void process(juce::AudioBuffer<float>& buffer,
                 float inputGainLinear,
                 float distAmount,
                 float lowDb,
                 float highDb,
                 float outputGainLinear,
                 float blendAmount,
                 ClippingMode mode,
                 bool isPowerOn)
    {
        const int numSamples = buffer.getNumSamples();
        const int numChans = std::min(buffer.getNumChannels(), channels);

        if (numSamples == 0 || numChans == 0)
            return;

        // Set smoothed targets
        smoothedInGain.setTargetValue(inputGainLinear);
        smoothedDistGain.setTargetValue(distAmount);
        smoothedLowDb.setTargetValue(lowDb);
        smoothedHighDb.setTargetValue(highDb);
        smoothedOutGain.setTargetValue(outputGainLinear);
        smoothedBlend.setTargetValue(blendAmount);
        smoothedBypass.setTargetValue(isPowerOn ? 1.0f : 0.0f);

        // Store dry signal for blend and click-free bypass crossfade
        if (dryScratchBuffer.getNumSamples() < numSamples || dryScratchBuffer.getNumChannels() < numChans)
            dryScratchBuffer.setSize(numChans, numSamples, false, false, true);

        for (int ch = 0; ch < numChans; ++ch)
            dryScratchBuffer.copyFrom(ch, 0, buffer.getReadPointer(ch), numSamples);

        // Update Gyrator filter coefficients
        updateLowGyrator(smoothedLowDb.getNextValue(), false);
        updateHighGyrator(smoothedHighDb.getNextValue(), false);

        // Process audio context
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        // 1. Apply DC Blocker
        dcBlocker.process(context);

        // 2. Apply Input Gain (Smoothly)
        for (int ch = 0; ch < numChans; ++ch)
        {
            auto* channelData = buffer.getWritePointer(ch);
            for (int i = 0; i < numSamples; ++i)
                channelData[i] *= smoothedInGain.getNextValue();
        }

        // 3. Stage 1: Pre-Emphasis Filters
        // 72.3 Hz High-Pass + 1.8 kHz Treble Shelf
        preHpFilter.process(context);
        preTrebleShelf.process(context);

        // 4. Stage 2: Non-Linear Distortion Engine (with Oversampling)
        // Map dist (0..1) to drive gain: 0 dB to +48 dB
        const float targetDriveNorm = smoothedDistGain.getNextValue();
        // Exponential curve for pleasant knob taper: 1.0 (0dB) to 251.18 (+48dB)
        const float driveGain = std::pow(10.0f, (targetDriveNorm * 48.0f) / 20.0f);

        if (oversampling && currentOversamplingFactor > 1)
        {
            juce::dsp::AudioBlock<float> oversampledBlock = oversampling->processSamplesUp(block);
            const size_t oversampledSamples = oversampledBlock.getNumSamples();

            for (size_t ch = 0; ch < static_cast<size_t>(numChans); ++ch)
            {
                auto* samples = oversampledBlock.getChannelPointer(ch);
                processDistortionBlock(samples, oversampledSamples, driveGain, mode);
            }

            oversampling->processSamplesDown(block);
        }
        else
        {
            for (int ch = 0; ch < numChans; ++ch)
            {
                auto* samples = buffer.getWritePointer(ch);
                processDistortionBlock(samples, static_cast<size_t>(numSamples), driveGain, mode);
            }
        }

        // 5. Stage 3: Passive Mid-Scoop Filter (~530 Hz, Q = 0.8, -6.5 dB)
        midScoopFilter.process(context);

        // 6. Stage 4: Active Dual-Gyrator Tone Stack ("The Chainsaw Engine")
        // Low Band (100 Hz, Q=2.1) + High Band (1.3 kHz, Q=3.5)
        lowGyrator.process(context);
        highGyrator.process(context);

        // 7. Stage 5: Post-EQ Cab Filtering (6.8 kHz Low-Pass Roll-off)
        postCabLpf.process(context);

        // 8. Stage 6: Output Master Gain, Parallel Dry/Wet Blend & Smooth True Bypass
        for (int ch = 0; ch < numChans; ++ch)
        {
            auto* wet = buffer.getWritePointer(ch);
            const auto* dry = dryScratchBuffer.getReadPointer(ch);

            for (int i = 0; i < numSamples; ++i)
            {
                const float outGain = smoothedOutGain.getNextValue();
                const float blend = smoothedBlend.getNextValue();
                const float bypassState = smoothedBypass.getNextValue();

                // Wet with Master Gain
                const float wetProcessed = wet[i] * outGain;

                // Dry / Wet Blend: (1 - blend)*dry + blend*wet
                const float blended = (1.0f - blend) * dry[i] + blend * wetProcessed;

                // Click-free True Bypass crossfade
                wet[i] = (1.0f - bypassState) * dry[i] + bypassState * blended;
            }
        }
    }

private:
    void updateLowGyrator(float gainDb, bool force)
    {
        if (!force && std::abs(lastLowDb - gainDb) < 0.05f)
            return;

        lastLowDb = gainDb;
        const float gainLinear = juce::Decibels::decibelsToGain(gainDb);
        *lowGyrator.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            currentSampleRate, 100.0f, 2.1f, gainLinear);
    }

    void updateHighGyrator(float gainDb, bool force)
    {
        if (!force && std::abs(lastHighDb - gainDb) < 0.05f)
            return;

        lastHighDb = gainDb;
        const float gainLinear = juce::Decibels::decibelsToGain(gainDb);
        *highGyrator.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            currentSampleRate, 1300.0f, 3.5f, gainLinear);
    }

    // Hybrid Diode Transfer Function:
    // f(x) = sign(x) * (1.0 - exp(-alpha * |x|)) + beta * tanh(gamma * x)
    inline void processDistortionBlock(float* samples, size_t numSamples, float driveGain, ClippingMode mode)
    {
        if (mode == ClippingMode::VintageHM2)
        {
            // Vintage HM-2: Silicon + Germanium Asymmetric Pairs
            // Germanium clips early on positive half, silicon clips harder on negative half
            const float alphaPos = 2.4f;
            const float betaPos  = 0.5f;
            const float gammaPos = 1.6f;

            const float alphaNeg = 3.6f;
            const float betaNeg  = 0.8f;
            const float gammaNeg = 2.2f;

            for (size_t i = 0; i < numSamples; ++i)
            {
                float x = samples[i] * driveGain;

                // Asymmetric non-linear transfer
                float y = 0.0f;
                if (x >= 0.0f)
                {
                    // Positive half-wave (Germanium soft knee + compression)
                    y = (1.0f - std::exp(-alphaPos * x)) + betaPos * std::tanh(gammaPos * x);
                    // Soft rail limiting
                    y = std::min(y, 1.4f);
                }
                else
                {
                    // Negative half-wave (Silicon hard clamp)
                    const float absX = -x;
                    y = -((1.0f - std::exp(-alphaNeg * absX)) + betaNeg * std::tanh(gammaNeg * absX));
                    // Hard rail clamping
                    y = std::max(y, -1.2f);
                }

                // Normalization compensation
                samples[i] = y * 0.72f;
            }
        }
        else // ClippingMode::GoatHotRod
        {
            // Goat Hot-Rod: Silicon + High-Headroom Red LED Asymmetry
            // Extended headroom, explosive dynamic punch, and harmonically saturated bite
            const float alphaPos = 1.3f;
            const float betaPos  = 0.7f;
            const float gammaPos = 1.1f;

            const float alphaNeg = 2.8f;
            const float betaNeg  = 0.6f;
            const float gammaNeg = 1.8f;

            for (size_t i = 0; i < numSamples; ++i)
            {
                float x = samples[i] * driveGain * 1.15f;

                float y = 0.0f;
                if (x >= 0.0f)
                {
                    // Positive LED clipping: wider dynamic window before hard limit
                    y = (1.0f - std::exp(-alphaPos * x)) + betaPos * std::tanh(gammaPos * x);
                    y = std::min(y, 1.75f);
                }
                else
                {
                    // Negative Silicon clipping: tight clamped base
                    const float absX = -x;
                    y = -((1.0f - std::exp(-alphaNeg * absX)) + betaNeg * std::tanh(gammaNeg * absX));
                    y = std::max(y, -1.35f);
                }

                // Normalization compensation
                samples[i] = y * 0.65f;
            }
        }
    }

    double currentSampleRate = 48000.0;
    int channels = 2;
    int maxBlockSize = 512;
    int currentOversamplingFactor = 4;

    float lastLowDb = 18.0f;
    float lastHighDb = 20.0f;

    // Filters
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> dcBlocker;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> preHpFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> preTrebleShelf;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> midScoopFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowGyrator;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highGyrator;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> postCabLpf;

    // Oversampling
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling;

    // Scratch buffer for Dry/Wet and Bypass
    juce::AudioBuffer<float> dryScratchBuffer;

    // Smoothed values
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedInGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDistGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedLowDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedHighDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedOutGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedBlend;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedBypass;
};

} // namespace GoatDSP
