#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "DSP/GoatDSPChain.h"
#include "PresetManager.h"
#include "MidiManager.h"

class TheGoatAudioProcessor : public juce::AudioProcessor
{
public:
    TheGoatAudioProcessor();
    ~TheGoatAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameter layout
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    PresetManager& getPresetManager() { return presetManager; }
    MidiManager& getMidiManager() { return midiManager; }

    void loadPreset(int index);

private:
    juce::AudioProcessorValueTreeState apvts;
    GoatDSP::GoatDSPChain dspChain;

    PresetManager presetManager;
    MidiManager midiManager;
    int currentPresetIndex = 0;

    // Cached parameter atomic pointers for real-time thread safety
    std::atomic<float>* inGainParam = nullptr;
    std::atomic<float>* distParam = nullptr;
    std::atomic<float>* lowParam = nullptr;
    std::atomic<float>* highParam = nullptr;
    std::atomic<float>* volParam = nullptr;
    std::atomic<float>* blendParam = nullptr;
    std::atomic<float>* powerParam = nullptr;

    juce::AudioParameterChoice* modeChoice = nullptr;
    juce::AudioParameterChoice* oversampleChoice = nullptr;
    juce::AudioParameterChoice* inputRoutingChoice = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TheGoatAudioProcessor)
};
