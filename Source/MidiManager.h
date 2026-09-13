#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <vector>
#include <string>
#include <functional>
#include <atomic>
#include <memory>

enum class MidiMappingType
{
    CC_Absolute,
    CC_Relative,
    CC_Toggle,
    CC_Gate,      // Momentary (>=64 is down, <64 is up)
    CC_Preset,
    Note_Toggle,
    Note_Gate,
    Note_Preset,
    Program_Change
};

inline std::string getMidiMappingTypeName(MidiMappingType t)
{
    switch (t)
    {
        case MidiMappingType::CC_Absolute:    return "CC Absolute";
        case MidiMappingType::CC_Relative:    return "CC Relative";
        case MidiMappingType::CC_Toggle:      return "CC Toggle";
        case MidiMappingType::CC_Gate:        return "CC Gate (Momentary)";
        case MidiMappingType::CC_Preset:      return "CC Preset";
        case MidiMappingType::Note_Toggle:    return "Note Toggle";
        case MidiMappingType::Note_Gate:      return "Note Gate";
        case MidiMappingType::Note_Preset:    return "Note Preset";
        case MidiMappingType::Program_Change: return "Program Change";
        default: return "CC Absolute";
    }
}

struct MidiMapping
{
    MidiMappingType type{MidiMappingType::CC_Absolute};
    std::string parameterId;
    std::string displayName;
    int channel{0};            // 0 = Omni, 1-16
    int controlNumber{11};     // CC number, Note number, or Program number
    float minValue{0.0f};
    float maxValue{1.0f};
};

class MidiManager
{
public:
    MidiManager()
    {
        // Factory default MIDI mappings for THE GOAT
        mappings.push_back({MidiMappingType::CC_Absolute, "dist",   "Gain / Horns",       0, 1,  0.0f, 1.0f}); // Mod Wheel CC #1
        mappings.push_back({MidiMappingType::CC_Absolute, "vol",    "Master Volume",      0, 7,  -30.0f, 12.0f}); // Volume CC #7
        mappings.push_back({MidiMappingType::CC_Absolute, "blend",  "Dry / Wet Blend",    0, 11, 0.0f, 1.0f}); // Expression CC #11
        mappings.push_back({MidiMappingType::CC_Absolute, "low",    "Low 100Hz Gyrator",  0, 14, -18.0f, 18.0f});
        mappings.push_back({MidiMappingType::CC_Absolute, "high",   "High 1.3kHz Buzzsaw",0, 15, -20.0f, 20.0f});
        mappings.push_back({MidiMappingType::CC_Toggle,   "mode",   "Diode Clipping Mode",0, 16, 0.0f, 1.0f});
        mappings.push_back({MidiMappingType::CC_Gate,     "power",  "Bypass Footswitch",  0, 64, 0.0f, 1.0f}); // Sustain Pedal CC #64
    }

    void setLearnTarget(const std::string& paramId, const std::string& displayName)
    {
        isLearning = true;
        learnParamId = paramId;
        learnParamName = displayName;
    }

    void cancelLearn()
    {
        isLearning = false;
        learnParamId.clear();
        learnParamName.clear();
    }

    bool getIsLearning() const { return isLearning; }
    std::string getLearnParamId() const { return learnParamId; }
    std::string getLearnParamName() const { return learnParamName; }

    std::vector<MidiMapping>& getMappings() { return mappings; }
    const std::vector<MidiMapping>& getMappings() const { return mappings; }

    void addOrUpdateMapping(const MidiMapping& newMapping)
    {
        for (auto& m : mappings)
        {
            if (m.parameterId == newMapping.parameterId)
            {
                m = newMapping;
                return;
            }
        }
        mappings.push_back(newMapping);
    }

    void removeMapping(size_t index)
    {
        if (index < mappings.size())
            mappings.erase(mappings.begin() + static_cast<ptrdiff_t>(index));
    }

    void processMidiBuffer(const juce::MidiBuffer& midiMessages,
                           juce::AudioProcessorValueTreeState& apvts,
                           std::function<void(int presetIndex)> onPresetChange = nullptr)
    {
        for (const auto metadata : midiMessages)
        {
            const auto msg = metadata.getMessage();
            const int channel = msg.getChannel();

            // MIDI Learn capture
            if (isLearning)
            {
                if (msg.isController())
                {
                    MidiMapping map;
                    map.type = MidiMappingType::CC_Absolute;
                    map.parameterId = learnParamId;
                    map.displayName = learnParamName;
                    map.channel = channel;
                    map.controlNumber = msg.getControllerNumber();

                    if (auto* param = apvts.getParameter(learnParamId))
                    {
                        auto range = param->getNormalisableRange();
                        map.minValue = range.start;
                        map.maxValue = range.end;
                    }

                    addOrUpdateMapping(map);
                    isLearning = false;
                    learnParamId.clear();
                    learnParamName.clear();
                    continue;
                }
                else if (msg.isNoteOn())
                {
                    MidiMapping map;
                    map.type = MidiMappingType::Note_Toggle;
                    map.parameterId = learnParamId;
                    map.displayName = learnParamName;
                    map.channel = channel;
                    map.controlNumber = msg.getNoteNumber();

                    if (auto* param = apvts.getParameter(learnParamId))
                    {
                        auto range = param->getNormalisableRange();
                        map.minValue = range.start;
                        map.maxValue = range.end;
                    }

                    addOrUpdateMapping(map);
                    isLearning = false;
                    learnParamId.clear();
                    learnParamName.clear();
                    continue;
                }
            }

            // Normal processing
            for (const auto& map : mappings)
            {
                if (map.channel != 0 && map.channel != channel)
                    continue;

                if (msg.isController() && msg.getControllerNumber() == map.controlNumber)
                {
                    const float normalizedVal = static_cast<float>(msg.getControllerValue()) / 127.0f;
                    handleMappingValue(map, normalizedVal, msg.getControllerValue() >= 64, apvts, onPresetChange);
                }
                else if (msg.isNoteOn() && msg.getNoteNumber() == map.controlNumber)
                {
                    const float normalizedVal = static_cast<float>(msg.getVelocity()) / 127.0f;
                    handleMappingValue(map, normalizedVal, true, apvts, onPresetChange);
                }
                else if (msg.isNoteOff() && msg.getNoteNumber() == map.controlNumber)
                {
                    handleMappingValue(map, 0.0f, false, apvts, onPresetChange);
                }
                else if (msg.isProgramChange() && map.type == MidiMappingType::Program_Change)
                {
                    if (onPresetChange)
                        onPresetChange(msg.getProgramChangeNumber());
                }
            }
        }
    }

private:
    void handleMappingValue(const MidiMapping& map,
                            float normalizedVal,
                            bool isDown,
                            juce::AudioProcessorValueTreeState& apvts,
                            std::function<void(int presetIndex)>& onPresetChange)
    {
        auto* param = apvts.getParameter(map.parameterId);

        switch (map.type)
        {
            case MidiMappingType::CC_Absolute:
            {
                if (param != nullptr)
                {
                    const float mappedVal = map.minValue + normalizedVal * (map.maxValue - map.minValue);
                    param->setValueNotifyingHost(param->convertTo0to1(mappedVal));
                }
                break;
            }
            case MidiMappingType::CC_Toggle:
            {
                if (isDown && param != nullptr)
                {
                    const float currentVal = param->getValue();
                    param->setValueNotifyingHost(currentVal > 0.5f ? 0.0f : 1.0f);
                }
                break;
            }
            case MidiMappingType::CC_Gate:
            case MidiMappingType::Note_Gate:
            {
                if (param != nullptr)
                    param->setValueNotifyingHost(isDown ? 1.0f : 0.0f);
                break;
            }
            case MidiMappingType::Note_Toggle:
            {
                if (isDown && param != nullptr)
                {
                    const float currentVal = param->getValue();
                    param->setValueNotifyingHost(currentVal > 0.5f ? 0.0f : 1.0f);
                }
                break;
            }
            case MidiMappingType::CC_Preset:
            case MidiMappingType::Note_Preset:
            {
                if (isDown && onPresetChange)
                    onPresetChange(static_cast<int>(map.minValue));
                break;
            }
            default:
                break;
        }
    }

    std::vector<MidiMapping> mappings;
    std::atomic<bool> isLearning{false};
    std::string learnParamId;
    std::string learnParamName;
};
