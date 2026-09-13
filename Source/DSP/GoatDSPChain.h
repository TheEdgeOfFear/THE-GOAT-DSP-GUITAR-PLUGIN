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

enum class InputRoutingMode
{
    AutoDetect = 0,  // Automatically detect active channel(s) and route to both if mono
    LeftOnly = 1,    // Force Left (Input 1) to BOTH Left and Right outputs
    RightOnly = 2,   // Force Right (Input 2) to BOTH Left and Right outputs
    Stereo = 3,      // True Stereo (L->L, R->R)
    MonoSum = 4      // Sum (L+R)*0.5 to BOTH Left and Right outputs
};

struct BiquadCoeffs
{
    float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f;
    float a1 = 0.0f, a2 = 0.0f;
};

struct BiquadChannel
{
    float z1 = 0.0f, z2 = 0.0f;

    void reset() { z1 = 0.0f; z2 = 0.0f; }

    inline float processSample(float in, const BiquadCoeffs& c) noexcept
    {
        // Direct Form II Transposed (numerically stable, minimal state, zero-alloc)
        float out = in * c.b0 + z1;
        z1 = in * c.b1 - out * c.a1 + z2;
        z2 = in * c.b2 - out * c.a2;
        return out;
    }
};

class StereoBiquad
{
public:
    StereoBiquad() = default;

    void reset() noexcept
    {
        ch[0].reset();
        ch[1].reset();
    }

    void setHighPass(double sampleRate, float cutoffHz, float q = 0.7071f) noexcept
    {
        const float w0 = static_cast<float>(2.0 * juce::MathConstants<double>::pi * cutoffHz / sampleRate);
        const float cosw0 = std::cos(w0);
        const float alpha = std::sin(w0) / (2.0f * q);

        const float a0 = 1.0f + alpha;
        const float invA0 = 1.0f / a0;

        coeffs.b0 = ((1.0f + cosw0) * 0.5f) * invA0;
        coeffs.b1 = (-(1.0f + cosw0)) * invA0;
        coeffs.b2 = ((1.0f + cosw0) * 0.5f) * invA0;
        coeffs.a1 = (-2.0f * cosw0) * invA0;
        coeffs.a2 = (1.0f - alpha) * invA0;
    }

    void setLowPass(double sampleRate, float cutoffHz, float q = 0.7071f) noexcept
    {
        const float w0 = static_cast<float>(2.0 * juce::MathConstants<double>::pi * cutoffHz / sampleRate);
        const float cosw0 = std::cos(w0);
        const float alpha = std::sin(w0) / (2.0f * q);

        const float a0 = 1.0f + alpha;
        const float invA0 = 1.0f / a0;

        coeffs.b0 = ((1.0f - cosw0) * 0.5f) * invA0;
        coeffs.b1 = (1.0f - cosw0) * invA0;
        coeffs.b2 = ((1.0f - cosw0) * 0.5f) * invA0;
        coeffs.a1 = (-2.0f * cosw0) * invA0;
        coeffs.a2 = (1.0f - alpha) * invA0;
    }

    void setPeak(double sampleRate, float freqHz, float q, float gainDb) noexcept
    {
        const float w0 = static_cast<float>(2.0 * juce::MathConstants<double>::pi * freqHz / sampleRate);
        const float cosw0 = std::cos(w0);
        const float alpha = std::sin(w0) / (2.0f * q);
        const float A = std::pow(10.0f, gainDb / 40.0f); // sqrt(gainLinear)

        const float a0 = 1.0f + alpha / A;
        const float invA0 = 1.0f / a0;

        coeffs.b0 = (1.0f + alpha * A) * invA0;
        coeffs.b1 = (-2.0f * cosw0) * invA0;
        coeffs.b2 = (1.0f - alpha * A) * invA0;
        coeffs.a1 = (-2.0f * cosw0) * invA0;
        coeffs.a2 = (1.0f - alpha / A) * invA0;
    }

    void setHighShelf(double sampleRate, float freqHz, float q, float gainDb) noexcept
    {
        const float w0 = static_cast<float>(2.0 * juce::MathConstants<double>::pi * freqHz / sampleRate);
        const float cosw0 = std::cos(w0);
        const float alpha = std::sin(w0) / (2.0f * q);
        const float A = std::pow(10.0f, gainDb / 40.0f);
        const float twoSqrtAAlpha = 2.0f * std::sqrt(A) * alpha;

        const float a0 = (A + 1.0f) - (A - 1.0f) * cosw0 + twoSqrtAAlpha;
        const float invA0 = 1.0f / a0;

        coeffs.b0 = (A * ((A + 1.0f) + (A - 1.0f) * cosw0 + twoSqrtAAlpha)) * invA0;
        coeffs.b1 = (-2.0f * A * ((A - 1.0f) + (A + 1.0f) * cosw0)) * invA0;
        coeffs.b2 = (A * ((A + 1.0f) + (A - 1.0f) * cosw0 - twoSqrtAAlpha)) * invA0;
        coeffs.a1 = (2.0f * ((A - 1.0f) - (A + 1.0f) * cosw0)) * invA0;
        coeffs.a2 = ((A + 1.0f) - (A - 1.0f) * cosw0 - twoSqrtAAlpha) * invA0;
    }

    inline void process(float* left, float* right, int numSamples) noexcept
    {
        for (int i = 0; i < numSamples; ++i)
        {
            left[i] = ch[0].processSample(left[i], coeffs);
            right[i] = ch[1].processSample(right[i], coeffs);
        }
    }

private:
    BiquadCoeffs coeffs;
    BiquadChannel ch[2];
};

class GoatDSPChain
{
public:
    GoatDSPChain() = default;

    void prepare(double sampleRate, int samplesPerBlock, int numChannels = 2)
    {
        juce::ignoreUnused(numChannels);
        currentSampleRate = (sampleRate > 8000.0) ? sampleRate : 48000.0;
        maxBlockSize = std::max(16384, samplesPerBlock * 8);

        // Pre-allocate scratch buffers generously to prevent heap allocations during audio processing
        dryScratchBuffer.setSize(2, maxBlockSize, false, true, true);
        workBuffer.setSize(2, maxBlockSize, false, true, true);

        // Reset Filters
        dcBlocker.reset();
        dcBlocker.setHighPass(currentSampleRate, 20.0f, 0.7071f);

        preHpFilter.reset();
        preHpFilter.setHighPass(currentSampleRate, 72.3f, 0.7071f);

        preTrebleShelf.reset();
        preTrebleShelf.setHighShelf(currentSampleRate, 1800.0f, 0.7071f, 2.0f);

        midScoopFilter.reset();
        midScoopFilter.setPeak(currentSampleRate, 530.0f, 0.8f, -6.5f);

        lowGyrator.reset();
        lastLowDb = 18.0f;
        lowGyrator.setPeak(currentSampleRate, 100.0f, 2.1f, 18.0f);

        highGyrator.reset();
        lastHighDb = 20.0f;
        highGyrator.setPeak(currentSampleRate, 1300.0f, 3.5f, 20.0f);

        postCabLpf.reset();
        postCabLpf.setLowPass(currentSampleRate, 6800.0f, 0.7071f);

        // Parameter smoothing
        smoothedInGain.reset(currentSampleRate, 0.02);
        smoothedDistGain.reset(currentSampleRate, 0.02);
        smoothedLowDb.reset(currentSampleRate, 0.02);
        smoothedHighDb.reset(currentSampleRate, 0.02);
        smoothedOutGain.reset(currentSampleRate, 0.02);
        smoothedBlend.reset(currentSampleRate, 0.02);
        smoothedBypass.reset(currentSampleRate, 0.02);

        // Initialize oversamplers for 2x, 4x, 8x with ample capacity
        prepareOversamplers();
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

        if (oversampling2x) oversampling2x->reset();
        if (oversampling4x) oversampling4x->reset();
        if (oversampling8x) oversampling8x->reset();

        autoDetectedMode = 1; // Default to Left Mono -> Both
    }

    void setOversamplingFactor(int factor)
    {
        if (factor != 1 && factor != 2 && factor != 4 && factor != 8)
            factor = 4;

        currentOversamplingFactor = factor;
    }

    int getOversamplingFactor() const noexcept { return currentOversamplingFactor; }

    void process(juce::AudioBuffer<float>& buffer,
                 float inputGainLinear,
                 float distAmount,
                 float lowDb,
                 float highDb,
                 float outputGainLinear,
                 float blendAmount,
                 ClippingMode mode,
                 InputRoutingMode routingMode,
                 bool isPowerOn)
    {
        const int numSamples = buffer.getNumSamples();
        const int inChannels = buffer.getNumChannels();

        if (numSamples <= 0 || inChannels <= 0)
            return;

        // Safety check on work buffer capacity
        if (workBuffer.getNumSamples() < numSamples)
            workBuffer.setSize(2, std::max(maxBlockSize, numSamples * 2), false, true, true);

        if (dryScratchBuffer.getNumSamples() < numSamples)
            dryScratchBuffer.setSize(2, std::max(maxBlockSize, numSamples * 2), false, true, true);

        // 1. Channel Routing Strategy:
        // Handle Left Only, Right Only, Stereo, Auto-Detect, and Mono-Sum.
        // If Left or Right is chosen, ALWAYS copy that input to BOTH Left and Right of workBuffer!
        const float* inL = buffer.getReadPointer(0);
        const float* inR = (inChannels >= 2) ? buffer.getReadPointer(1) : inL;

        float* workL = workBuffer.getWritePointer(0);
        float* workR = workBuffer.getWritePointer(1);

        switch (routingMode)
        {
            case InputRoutingMode::LeftOnly:
            {
                // Force Input 1 (Left) to BOTH channels
                for (int i = 0; i < numSamples; ++i)
                {
                    const float s = inL[i];
                    workL[i] = s;
                    workR[i] = s;
                }
                break;
            }
            case InputRoutingMode::RightOnly:
            {
                // Force Input 2 (Right) to BOTH channels (falls back to inL if only 1 channel exists)
                const float* src = (inChannels >= 2) ? inR : inL;
                for (int i = 0; i < numSamples; ++i)
                {
                    const float s = src[i];
                    workL[i] = s;
                    workR[i] = s;
                }
                break;
            }
            case InputRoutingMode::Stereo:
            {
                // True Stereo
                for (int i = 0; i < numSamples; ++i)
                {
                    workL[i] = inL[i];
                    workR[i] = (inChannels >= 2) ? inR[i] : inL[i];
                }
                break;
            }
            case InputRoutingMode::MonoSum:
            {
                // Sum (L+R)*0.5 to both channels
                for (int i = 0; i < numSamples; ++i)
                {
                    const float s = (inChannels >= 2) ? (0.5f * (inL[i] + inR[i])) : inL[i];
                    workL[i] = s;
                    workR[i] = s;
                }
                break;
            }
            case InputRoutingMode::AutoDetect:
            default:
            {
                if (inChannels < 2)
                {
                    for (int i = 0; i < numSamples; ++i)
                    {
                        const float s = inL[i];
                        workL[i] = s;
                        workR[i] = s;
                    }
                }
                else
                {
                    const float magL = buffer.getMagnitude(0, 0, numSamples);
                    const float magR = buffer.getMagnitude(1, 0, numSamples);

                    // Dynamic threshold detection (handles guitar into In 1 or In 2 with real interface noise floors)
                    if (magL > 0.0005f && (magL > 4.0f * magR || magR < 0.001f))
                    {
                        autoDetectedMode = 1; // Left Mono -> Both
                    }
                    else if (magR > 0.0005f && (magR > 4.0f * magL || magL < 0.001f))
                    {
                        autoDetectedMode = 2; // Right Mono -> Both
                    }
                    else if (magL > 0.003f && magR > 0.003f)
                    {
                        autoDetectedMode = 0; // True Stereo
                    }

                    if (autoDetectedMode == 1)
                    {
                        for (int i = 0; i < numSamples; ++i)
                        {
                            const float s = inL[i];
                            workL[i] = s;
                            workR[i] = s;
                        }
                    }
                    else if (autoDetectedMode == 2)
                    {
                        for (int i = 0; i < numSamples; ++i)
                        {
                            const float s = inR[i];
                            workL[i] = s;
                            workR[i] = s;
                        }
                    }
                    else
                    {
                        for (int i = 0; i < numSamples; ++i)
                        {
                            workL[i] = inL[i];
                            workR[i] = inR[i];
                        }
                    }
                }
                break;
            }
        }

        // Store dry signal for parallel blend & smooth bypass crossfade
        dryScratchBuffer.copyFrom(0, 0, workL, numSamples);
        dryScratchBuffer.copyFrom(1, 0, workR, numSamples);

        // Update smoothed parameter targets
        smoothedInGain.setTargetValue(inputGainLinear);
        smoothedDistGain.setTargetValue(distAmount);
        smoothedLowDb.setTargetValue(lowDb);
        smoothedHighDb.setTargetValue(highDb);
        smoothedOutGain.setTargetValue(outputGainLinear);
        smoothedBlend.setTargetValue(blendAmount);
        smoothedBypass.setTargetValue(isPowerOn ? 1.0f : 0.0f);

        // Update Gyrator filter coefficients if parameters moved
        const float curLow = smoothedLowDb.getNextValue();
        if (std::abs(lastLowDb - curLow) > 0.05f)
        {
            lastLowDb = curLow;
            lowGyrator.setPeak(currentSampleRate, 100.0f, 2.1f, curLow);
        }

        const float curHigh = smoothedHighDb.getNextValue();
        if (std::abs(lastHighDb - curHigh) > 0.05f)
        {
            lastHighDb = curHigh;
            highGyrator.setPeak(currentSampleRate, 1300.0f, 3.5f, curHigh);
        }

        // 1. DC Blocker
        dcBlocker.process(workL, workR, numSamples);

        // 2. Input Gain Staging
        const float inG = smoothedInGain.getNextValue();
        for (int i = 0; i < numSamples; ++i)
        {
            workL[i] *= inG;
            workR[i] *= inG;
        }

        // 3. Stage 1: Pre-Emphasis Filters (72.3 Hz HPF + 1.8 kHz Treble Shelf)
        preHpFilter.process(workL, workR, numSamples);
        preTrebleShelf.process(workL, workR, numSamples);

        // 4. Stage 2: Non-Linear Distortion Engine with Oversampling
        const float targetDriveNorm = smoothedDistGain.getNextValue();
        const float driveGain = std::pow(10.0f, (targetDriveNorm * 48.0f) / 20.0f);

        juce::dsp::Oversampling<float>* activeOversampler = nullptr;
        if (currentOversamplingFactor == 2) activeOversampler = oversampling2x.get();
        else if (currentOversamplingFactor == 4) activeOversampler = oversampling4x.get();
        else if (currentOversamplingFactor == 8) activeOversampler = oversampling8x.get();

        if (activeOversampler != nullptr)
        {
            juce::dsp::AudioBlock<float> block(workBuffer.getArrayOfWritePointers(), 2, static_cast<size_t>(numSamples));
            juce::dsp::AudioBlock<float> oversampledBlock = activeOversampler->processSamplesUp(block);
            const size_t oversampledSamples = oversampledBlock.getNumSamples();

            for (size_t ch = 0; ch < 2; ++ch)
            {
                auto* samples = oversampledBlock.getChannelPointer(ch);
                processDistortionBlock(samples, oversampledSamples, driveGain, mode);
            }

            activeOversampler->processSamplesDown(block);
        }
        else
        {
            processDistortionBlock(workL, static_cast<size_t>(numSamples), driveGain, mode);
            processDistortionBlock(workR, static_cast<size_t>(numSamples), driveGain, mode);
        }

        // 5. Stage 3: Passive Mid-Scoop Filter (530 Hz Notch)
        midScoopFilter.process(workL, workR, numSamples);

        // 6. Stage 4: Active Dual-Gyrator Tone Stack (Low 100 Hz + High 1.3 kHz Buzzsaw)
        lowGyrator.process(workL, workR, numSamples);
        highGyrator.process(workL, workR, numSamples);

        // 7. Stage 5: Post-EQ Cab Filtering (6.8 kHz Low-Pass)
        postCabLpf.process(workL, workR, numSamples);

        // 8. Stage 6: Output Master Level, Parallel Blend & Smooth True Bypass Crossfade
        const float outG = smoothedOutGain.getNextValue();
        const float blend = smoothedBlend.getNextValue();
        const float bypassState = smoothedBypass.getNextValue();

        const float* dryL = dryScratchBuffer.getReadPointer(0);
        const float* dryR = dryScratchBuffer.getReadPointer(1);

        // Write to destination buffer (guaranteeing both Left and Right output channels receive audio)
        float* destL = buffer.getWritePointer(0);
        float* destR = (buffer.getNumChannels() >= 2) ? buffer.getWritePointer(1) : destL;

        for (int i = 0; i < numSamples; ++i)
        {
            // Left output
            const float wetL = workL[i] * outG;
            const float blendedL = (1.0f - blend) * dryL[i] + blend * wetL;
            destL[i] = (1.0f - bypassState) * dryL[i] + bypassState * blendedL;

            // Right output (Always rendered so guitar plays in both ears)
            const float wetR = workR[i] * outG;
            const float blendedR = (1.0f - blend) * dryR[i] + blend * wetR;
            destR[i] = (1.0f - bypassState) * dryR[i] + bypassState * blendedR;
        }
    }

private:
    void prepareOversamplers()
    {
        oversampling2x = std::make_unique<juce::dsp::Oversampling<float>>(
            2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true);
        oversampling2x->initProcessing(static_cast<size_t>(maxBlockSize));

        oversampling4x = std::make_unique<juce::dsp::Oversampling<float>>(
            2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true);
        oversampling4x->initProcessing(static_cast<size_t>(maxBlockSize));

        oversampling8x = std::make_unique<juce::dsp::Oversampling<float>>(
            2, 3, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true);
        oversampling8x->initProcessing(static_cast<size_t>(maxBlockSize));
    }

    inline void processDistortionBlock(float* samples, size_t numSamples, float driveGain, ClippingMode mode) noexcept
    {
        if (mode == ClippingMode::VintageHM2)
        {
            // Vintage HM-2: Silicon + Germanium Asymmetric Pairs
            const float alphaPos = 2.4f;
            const float betaPos  = 0.5f;
            const float gammaPos = 1.6f;

            const float alphaNeg = 3.6f;
            const float betaNeg  = 0.8f;
            const float gammaNeg = 2.2f;

            for (size_t i = 0; i < numSamples; ++i)
            {
                float x = samples[i] * driveGain;
                float y = 0.0f;

                if (x >= 0.0f)
                {
                    y = (1.0f - std::exp(-alphaPos * x)) + betaPos * std::tanh(gammaPos * x);
                    y = std::min(y, 1.4f);
                }
                else
                {
                    const float absX = -x;
                    y = -((1.0f - std::exp(-alphaNeg * absX)) + betaNeg * std::tanh(gammaNeg * absX));
                    y = std::max(y, -1.2f);
                }

                samples[i] = y * 0.72f;
            }
        }
        else // GoatHotRod
        {
            // Goat Hot-Rod: Silicon + High-Headroom Red LED Asymmetry
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
                    y = (1.0f - std::exp(-alphaPos * x)) + betaPos * std::tanh(gammaPos * x);
                    y = std::min(y, 1.75f);
                }
                else
                {
                    const float absX = -x;
                    y = -((1.0f - std::exp(-alphaNeg * absX)) + betaNeg * std::tanh(gammaNeg * absX));
                    y = std::max(y, -1.35f);
                }

                samples[i] = y * 0.65f;
            }
        }
    }

    double currentSampleRate = 48000.0;
    int maxBlockSize = 16384;
    int currentOversamplingFactor = 4;

    float lastLowDb = 18.0f;
    float lastHighDb = 20.0f;
    int autoDetectedMode = 1; // 0 = Stereo, 1 = Left Only -> Both, 2 = Right Only -> Both

    // Filters (100% Zero-Heap-Allocation Biquads)
    StereoBiquad dcBlocker;
    StereoBiquad preHpFilter;
    StereoBiquad preTrebleShelf;
    StereoBiquad midScoopFilter;
    StereoBiquad lowGyrator;
    StereoBiquad highGyrator;
    StereoBiquad postCabLpf;

    // Pre-allocated Oversamplers
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling2x;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling4x;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling8x;

    // Pre-allocated Scratch Buffers
    juce::AudioBuffer<float> dryScratchBuffer;
    juce::AudioBuffer<float> workBuffer;

    // Smoothed parameter values
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedInGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDistGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedLowDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedHighDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedOutGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedBlend;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedBypass;
};

} // namespace GoatDSP
