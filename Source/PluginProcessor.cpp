#include "PluginProcessor.h"
#include "PluginEditor.h"

TheGoatAudioProcessor::TheGoatAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    inGainParam        = apvts.getRawParameterValue("inGain");
    distParam          = apvts.getRawParameterValue("dist");
    lowParam           = apvts.getRawParameterValue("low");
    highParam          = apvts.getRawParameterValue("high");
    volParam           = apvts.getRawParameterValue("vol");
    blendParam         = apvts.getRawParameterValue("blend");
    powerParam         = apvts.getRawParameterValue("power");
    modeChoice         = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("mode"));
    oversampleChoice   = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("oversample"));
    inputRoutingChoice = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("inputRouting"));
}

TheGoatAudioProcessor::~TheGoatAudioProcessor() = default;

juce::AudioProcessorValueTreeState::ParameterLayout TheGoatAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // inGain: -24 dB to +12 dB
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"inGain", 1},
        "Input Gain",
        juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    // dist: 0.0 to 1.0 (Gain / Horns)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"dist", 1},
        "Gain / Horns",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.75f));

    // low: -18 dB to +18 dB (Low Gyrator 100 Hz, default +18 dB Dime)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"low", 1},
        "Low (100 Hz)",
        juce::NormalisableRange<float>(-18.0f, 18.0f, 0.1f),
        18.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    // high: -20 dB to +20 dB (High Gyrator 1.3 kHz, default +20 dB Dime)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"high", 1},
        "High (1.3 kHz)",
        juce::NormalisableRange<float>(-20.0f, 20.0f, 0.1f),
        20.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    // vol: -30 dB to +12 dB (Master Volume)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"vol", 1},
        "Master Volume",
        juce::NormalisableRange<float>(-30.0f, 12.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    // blend: 0.0 to 1.0 (Dry / Wet Blend)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"blend", 1},
        "Dry / Wet Blend",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        1.0f));

    // mode: 0 = "Vintage HM-2", 1 = "Goat Hot-Rod"
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"mode", 1},
        "Diode Clipping Mode",
        juce::StringArray{"Vintage HM-2", "Goat Hot-Rod"},
        0));

    // power: bypass stomp switch
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{"power", 1},
        "Power / Bypass",
        true));

    // oversample: 1x, 2x, 4x, 8x
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"oversample", 1},
        "Oversampling",
        juce::StringArray{"1x (Off)", "2x Polyphase", "4x Polyphase", "8x Polyphase"},
        2)); // default 4x

    // inputRouting: Auto Detect, Input 1 (L -> Both), Input 2 (R -> Both), Stereo (L+R), Mono Sum (1+2)
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"inputRouting", 1},
        "Input Channel",
        juce::StringArray{"Auto Detect", "Input 1 (L -> Both)", "Input 2 (R -> Both)", "Stereo (L+R)", "Mono Sum (1+2)"},
        1)); // default to Input 1 (L -> Both)

    return { params.begin(), params.end() };
}

const juce::String TheGoatAudioProcessor::getName() const
{
    return "THE GOAT";
}

bool TheGoatAudioProcessor::acceptsMidi() const
{
    return true;
}

bool TheGoatAudioProcessor::producesMidi() const
{
    return false;
}

bool TheGoatAudioProcessor::isMidiEffect() const
{
    return false;
}

double TheGoatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheGoatAudioProcessor::getNumPrograms()
{
    return static_cast<int>(presetManager.getAllPresets().size());
}

int TheGoatAudioProcessor::getCurrentProgram()
{
    return currentPresetIndex;
}

void TheGoatAudioProcessor::setCurrentProgram(int index)
{
    loadPreset(index);
}

const juce::String TheGoatAudioProcessor::getProgramName(int index)
{
    const auto& presets = presetManager.getAllPresets();
    if (index >= 0 && index < static_cast<int>(presets.size()))
        return presets[static_cast<size_t>(index)].name;
    return {};
}

void TheGoatAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void TheGoatAudioProcessor::loadPreset(int index)
{
    const auto& presets = presetManager.getAllPresets();
    if (index < 0 || index >= static_cast<int>(presets.size()))
        return;

    currentPresetIndex = index;
    const auto& p = presets[static_cast<size_t>(index)];

    if (auto* param = apvts.getParameter("dist"))
        param->setValueNotifyingHost(param->convertTo0to1(p.dist));
    if (auto* param = apvts.getParameter("low"))
        param->setValueNotifyingHost(param->convertTo0to1(p.low));
    if (auto* param = apvts.getParameter("high"))
        param->setValueNotifyingHost(param->convertTo0to1(p.high));
    if (auto* param = apvts.getParameter("vol"))
        param->setValueNotifyingHost(param->convertTo0to1(p.volume));
    if (auto* param = apvts.getParameter("blend"))
        param->setValueNotifyingHost(param->convertTo0to1(p.blend));
    if (auto* param = apvts.getParameter("mode"))
        param->setValueNotifyingHost(param->convertTo0to1(static_cast<float>(p.clippingMode)));
    if (auto* param = apvts.getParameter("inGain"))
        param->setValueNotifyingHost(param->convertTo0to1(p.inputGain));
    if (auto* param = apvts.getParameter("oversample"))
        param->setValueNotifyingHost(param->convertTo0to1(static_cast<float>(p.oversample)));
}

void TheGoatAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    const int numChannels = std::max(2, std::max(getTotalNumInputChannels(), getTotalNumOutputChannels()));
    dspChain.prepare(sampleRate, samplesPerBlock, numChannels);
}

void TheGoatAudioProcessor::releaseResources()
{
    dspChain.reset();
}

bool TheGoatAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainOutput = layouts.getMainOutputChannelSet();
    const auto& mainInput  = layouts.getMainInputChannelSet();

    if (mainOutput != juce::AudioChannelSet::mono() && mainOutput != juce::AudioChannelSet::stereo())
        return false;

    if (mainInput != juce::AudioChannelSet::mono() && mainInput != juce::AudioChannelSet::stereo()
        && mainInput != juce::AudioChannelSet::disabled())
        return false;

    return true;
}

void TheGoatAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int numSamples = buffer.getNumSamples();
    const int bufChannels = buffer.getNumChannels();

    if (numSamples <= 0 || bufChannels <= 0)
        return;

    for (int i = totalNumInputChannels; i < bufChannels; ++i)
        buffer.clear(i, 0, numSamples);

    // Process incoming MIDI for CC Learn / Presets / Footswitch
    midiManager.processMidiBuffer(midiMessages, apvts, [this](int pIdx) {
        loadPreset(pIdx);
    });

    // Extract real-time parameter values
    const float inGainDb     = inGainParam ? inGainParam->load() : 0.0f;
    const float distVal      = distParam ? distParam->load() : 0.75f;
    const float lowDb        = lowParam ? lowParam->load() : 18.0f;
    const float highDb       = highParam ? highParam->load() : 20.0f;
    const float volDb        = volParam ? volParam->load() : 0.0f;
    const float blendVal     = blendParam ? blendParam->load() : 1.0f;
    const bool isPowerOn     = powerParam ? (powerParam->load() > 0.5f) : true;
    const int modeVal        = modeChoice ? modeChoice->getIndex() : 0;
    const int overIdx        = oversampleChoice ? oversampleChoice->getIndex() : 2;
    const int routingIdx     = inputRoutingChoice ? inputRoutingChoice->getIndex() : 1;

    const float inputGainLinear  = juce::Decibels::decibelsToGain(inGainDb);
    const float outputGainLinear = juce::Decibels::decibelsToGain(volDb);

    // Update oversampling factor if changed
    int factor = 4;
    if (overIdx == 0) factor = 1;
    else if (overIdx == 1) factor = 2;
    else if (overIdx == 2) factor = 4;
    else if (overIdx == 3) factor = 8;

    if (dspChain.getOversamplingFactor() != factor)
        dspChain.setOversamplingFactor(factor);

    GoatDSP::ClippingMode clipMode = (modeVal == 1) ? GoatDSP::ClippingMode::GoatHotRod : GoatDSP::ClippingMode::VintageHM2;

    GoatDSP::InputRoutingMode routingMode = GoatDSP::InputRoutingMode::LeftOnly;
    if (routingIdx == 0) routingMode = GoatDSP::InputRoutingMode::AutoDetect;
    else if (routingIdx == 1) routingMode = GoatDSP::InputRoutingMode::LeftOnly;
    else if (routingIdx == 2) routingMode = GoatDSP::InputRoutingMode::RightOnly;
    else if (routingIdx == 3) routingMode = GoatDSP::InputRoutingMode::Stereo;
    else if (routingIdx == 4) routingMode = GoatDSP::InputRoutingMode::MonoSum;

    // Route audio through DSP Engine
    dspChain.process(buffer,
                     inputGainLinear,
                     distVal,
                     lowDb,
                     highDb,
                     outputGainLinear,
                     blendVal,
                     clipMode,
                     routingMode,
                     isPowerOn);
}

bool TheGoatAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TheGoatAudioProcessor::createEditor()
{
    return new TheGoatAudioProcessorEditor(*this);
}

void TheGoatAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->setAttribute("currentPresetIndex", currentPresetIndex);
    copyXmlToBinary(*xml, destData);
}

void TheGoatAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
            currentPresetIndex = xmlState->getIntAttribute("currentPresetIndex", 0);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheGoatAudioProcessor();
}
