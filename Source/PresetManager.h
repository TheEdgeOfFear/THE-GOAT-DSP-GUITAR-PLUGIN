#pragma once

#include "GoatPresets.h"
#include <juce_core/juce_core.h>
#include <vector>
#include <algorithm>

class PresetManager
{
public:
    PresetManager()
    {
        loadFactoryPresets();
        loadUserPresets();
    }

    const std::vector<GoatPresets::Preset>& getAllPresets() const
    {
        return allPresets;
    }

    std::vector<std::string> getCategories() const
    {
        std::vector<std::string> categories;
        for (const auto& p : allPresets)
        {
            if (std::find(categories.begin(), categories.end(), p.category) == categories.end())
                categories.push_back(p.category);
        }
        return categories;
    }

    bool isCurrentPresetFactory(int globalIndex) const
    {
        if (globalIndex >= 0 && globalIndex < static_cast<int>(allPresets.size()))
            return allPresets[static_cast<size_t>(globalIndex)].isFactory;
        return true;
    }

    bool saveUserPreset(const std::string& name,
                        const GoatPresets::Preset& presetData,
                        int* outNewIndex = nullptr)
    {
        for (const auto& p : allPresets)
        {
            if (p.name == name && p.isFactory)
                return false; // Prevent overwriting factory presets
        }

        for (size_t i = 0; i < allPresets.size(); ++i)
        {
            if (allPresets[i].name == name && !allPresets[i].isFactory)
            {
                allPresets[i] = presetData;
                allPresets[i].name = name;
                allPresets[i].category = "User Presets";
                allPresets[i].isFactory = false;
                if (outNewIndex) *outNewIndex = static_cast<int>(i);
                saveUserPresetsToDisk();
                return true;
            }
        }

        GoatPresets::Preset newPreset = presetData;
        newPreset.name = name;
        newPreset.category = "User Presets";
        newPreset.isFactory = false;

        allPresets.push_back(newPreset);
        if (outNewIndex) *outNewIndex = static_cast<int>(allPresets.size() - 1);
        saveUserPresetsToDisk();
        return true;
    }

    bool deleteUserPreset(int globalIndex)
    {
        if (globalIndex < 0 || globalIndex >= static_cast<int>(allPresets.size()))
            return false;

        if (allPresets[static_cast<size_t>(globalIndex)].isFactory)
            return false;

        allPresets.erase(allPresets.begin() + globalIndex);
        saveUserPresetsToDisk();
        return true;
    }

private:
    std::vector<GoatPresets::Preset> allPresets;

    void loadFactoryPresets()
    {
        allPresets = GoatPresets::getBuiltInPresets();
    }

    juce::File getUserPresetFile() const
    {
        auto dir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                   .getChildFile("TheEdgeOfFear")
                   .getChildFile("TheGoat");
        if (!dir.exists())
            dir.createDirectory();
        return dir.getChildFile("UserPresets.xml");
    }

    void loadUserPresets()
    {
        auto file = getUserPresetFile();
        if (!file.existsAsFile())
            return;

        auto xml = juce::XmlDocument::parse(file);
        if (xml == nullptr || !xml->hasTagName("GoatUserPresets"))
            return;

        for (auto* child : xml->getChildIterator())
        {
            if (child->hasTagName("Preset"))
            {
                GoatPresets::Preset p;
                p.category = "User Presets";
                p.name = child->getStringAttribute("name", "Custom User").toStdString();
                p.description = child->getStringAttribute("description", "").toStdString();
                p.dist = static_cast<float>(child->getDoubleAttribute("dist", 0.75));
                p.low = static_cast<float>(child->getDoubleAttribute("low", 18.0));
                p.high = static_cast<float>(child->getDoubleAttribute("high", 20.0));
                p.volume = static_cast<float>(child->getDoubleAttribute("volume", 0.0));
                p.blend = static_cast<float>(child->getDoubleAttribute("blend", 1.0));
                p.clippingMode = child->getIntAttribute("clippingMode", 0);
                p.inputGain = static_cast<float>(child->getDoubleAttribute("inputGain", 0.0));
                p.oversample = child->getIntAttribute("oversample", 2);
                p.isFactory = false;

                allPresets.push_back(p);
            }
        }
    }

    void saveUserPresetsToDisk()
    {
        auto file = getUserPresetFile();
        juce::XmlElement root("GoatUserPresets");

        for (const auto& p : allPresets)
        {
            if (!p.isFactory)
            {
                auto* child = root.createNewChildElement("Preset");
                child->setAttribute("name", juce::String(p.name));
                child->setAttribute("description", juce::String(p.description));
                child->setAttribute("dist", p.dist);
                child->setAttribute("low", p.low);
                child->setAttribute("high", p.high);
                child->setAttribute("volume", p.volume);
                child->setAttribute("blend", p.blend);
                child->setAttribute("clippingMode", p.clippingMode);
                child->setAttribute("inputGain", p.inputGain);
                child->setAttribute("oversample", p.oversample);
            }
        }

        root.writeTo(file);
    }
};
