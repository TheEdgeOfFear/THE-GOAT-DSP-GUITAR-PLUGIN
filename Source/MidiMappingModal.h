#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "MidiManager.h"

class MidiMappingModal : public juce::Component
{
public:
    MidiMappingModal(MidiManager& manager, std::function<void()> onCloseCallback)
        : midiManager(manager), onClose(onCloseCallback)
    {
        closeButton.setButtonText("X");
        closeButton.onClick = [this]() {
            if (onClose) onClose();
        };
        addAndMakeVisible(closeButton);

        addButton.setButtonText("+ ADD MAPPING");
        addButton.onClick = [this]() {
            MidiMapping newMap;
            newMap.type = MidiMappingType::CC_Absolute;
            newMap.parameterId = "dist";
            newMap.displayName = "Gain / Horns";
            newMap.channel = 0; // Omni
            newMap.controlNumber = 1;
            midiManager.addOrUpdateMapping(newMap);
            rebuildRows();
        };
        addAndMakeVisible(addButton);

        rebuildRows();
    }

    void rebuildRows()
    {
        rows.clear();
        const auto& list = midiManager.getMappings();

        for (int i = 0; i < static_cast<int>(list.size()); ++i)
        {
            auto* row = rows.add(new RowComponent(midiManager, i, [this]() {
                rebuildRows();
            }));
            addAndMakeVisible(row);
        }

        resized();
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        // Dark translucent overlay with blood red border
        g.setColour(juce::Colour(0xf2101216));
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
        g.setColour(juce::Colour(0xff800a14));
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 8.0f, 1.5f);

        // Header Title
        g.setFont(juce::FontOptions(15.0f, juce::Font::bold));
        g.setColour(juce::Colours::white);
        g.drawText("THE GOAT - MIDI MAPPINGS", 20, 12, getWidth() - 40, 24, juce::Justification::centred);

        // Table Column Labels
        g.setFont(juce::FontOptions(11.0f, juce::Font::bold));
        g.setColour(juce::Colour(0xff8c92a0));
        g.drawText("Type", 28, 42, 140, 20, juce::Justification::centredLeft);
        g.drawText("Target Parameter", 176, 42, 160, 20, juce::Justification::centredLeft);
        g.drawText("Channel", 344, 42, 65, 20, juce::Justification::centredLeft);
        g.drawText("Control / Note #", 418, 42, 110, 20, juce::Justification::centredLeft);
    }

    void resized() override
    {
        closeButton.setBounds(getWidth() - 32, 10, 22, 22);

        int y = 68;
        const int rowH = 34;

        for (auto* row : rows)
        {
            row->setBounds(12, y, getWidth() - 24, rowH);
            y += rowH + 4;
        }

        addButton.setBounds(20, y + 8, 140, 28);
    }

private:
    class RowComponent : public juce::Component
    {
    public:
        RowComponent(MidiManager& mgr, int index, std::function<void()> onUpdate)
            : manager(mgr), rowIndex(index), notifyUpdate(onUpdate)
        {
            const auto& m = manager.getMappings()[static_cast<size_t>(index)];

            // Type ComboBox
            typeBox.addItem("CC Absolute", 1);
            typeBox.addItem("CC Relative", 2);
            typeBox.addItem("CC Toggle", 3);
            typeBox.addItem("CC Gate (Momentary)", 4);
            typeBox.addItem("Note Toggle", 5);
            typeBox.addItem("Note Gate", 6);
            typeBox.addItem("Program Change", 7);

            switch (m.type)
            {
                case MidiMappingType::CC_Absolute:    typeBox.setSelectedId(1, juce::dontSendNotification); break;
                case MidiMappingType::CC_Relative:    typeBox.setSelectedId(2, juce::dontSendNotification); break;
                case MidiMappingType::CC_Toggle:      typeBox.setSelectedId(3, juce::dontSendNotification); break;
                case MidiMappingType::CC_Gate:        typeBox.setSelectedId(4, juce::dontSendNotification); break;
                case MidiMappingType::Note_Toggle:    typeBox.setSelectedId(5, juce::dontSendNotification); break;
                case MidiMappingType::Note_Gate:      typeBox.setSelectedId(6, juce::dontSendNotification); break;
                case MidiMappingType::Program_Change: typeBox.setSelectedId(7, juce::dontSendNotification); break;
                default:                              typeBox.setSelectedId(1, juce::dontSendNotification); break;
            }

            typeBox.onChange = [this]() {
                auto& item = manager.getMappings()[static_cast<size_t>(rowIndex)];
                switch (typeBox.getSelectedId())
                {
                    case 1: item.type = MidiMappingType::CC_Absolute; break;
                    case 2: item.type = MidiMappingType::CC_Relative; break;
                    case 3: item.type = MidiMappingType::CC_Toggle; break;
                    case 4: item.type = MidiMappingType::CC_Gate; break;
                    case 5: item.type = MidiMappingType::Note_Toggle; break;
                    case 6: item.type = MidiMappingType::Note_Gate; break;
                    case 7: item.type = MidiMappingType::Program_Change; break;
                }
            };
            addAndMakeVisible(typeBox);

            // Parameter Target ComboBox
            paramBox.addItem("Gain / Horns", 1);
            paramBox.addItem("Low 100Hz Gyrator", 2);
            paramBox.addItem("High 1.3kHz Buzzsaw", 3);
            paramBox.addItem("Master Volume", 4);
            paramBox.addItem("Dry / Wet Blend", 5);
            paramBox.addItem("Diode Mode Toggle", 6);
            paramBox.addItem("Bypass Footswitch", 7);
            paramBox.addItem("Input Gain", 8);
            paramBox.addItem("Oversampling", 9);
            paramBox.addItem("Input Channel Routing", 10);

            if (m.parameterId == "dist")              paramBox.setSelectedId(1, juce::dontSendNotification);
            else if (m.parameterId == "low")          paramBox.setSelectedId(2, juce::dontSendNotification);
            else if (m.parameterId == "high")         paramBox.setSelectedId(3, juce::dontSendNotification);
            else if (m.parameterId == "vol")          paramBox.setSelectedId(4, juce::dontSendNotification);
            else if (m.parameterId == "blend")        paramBox.setSelectedId(5, juce::dontSendNotification);
            else if (m.parameterId == "mode")         paramBox.setSelectedId(6, juce::dontSendNotification);
            else if (m.parameterId == "power")        paramBox.setSelectedId(7, juce::dontSendNotification);
            else if (m.parameterId == "inGain")       paramBox.setSelectedId(8, juce::dontSendNotification);
            else if (m.parameterId == "oversample")   paramBox.setSelectedId(9, juce::dontSendNotification);
            else if (m.parameterId == "inputRouting") paramBox.setSelectedId(10, juce::dontSendNotification);
            else paramBox.setSelectedId(1, juce::dontSendNotification);

            paramBox.onChange = [this]() {
                auto& item = manager.getMappings()[static_cast<size_t>(rowIndex)];
                switch (paramBox.getSelectedId())
                {
                    case 1:  item.parameterId = "dist";         item.displayName = "Gain / Horns";          break;
                    case 2:  item.parameterId = "low";          item.displayName = "Low 100Hz Gyrator";     break;
                    case 3:  item.parameterId = "high";         item.displayName = "High 1.3kHz Buzzsaw";   break;
                    case 4:  item.parameterId = "vol";          item.displayName = "Master Volume";         break;
                    case 5:  item.parameterId = "blend";        item.displayName = "Dry / Wet Blend";       break;
                    case 6:  item.parameterId = "mode";         item.displayName = "Diode Mode Toggle";     break;
                    case 7:  item.parameterId = "power";        item.displayName = "Bypass Footswitch";     break;
                    case 8:  item.parameterId = "inGain";       item.displayName = "Input Gain";            break;
                    case 9:  item.parameterId = "oversample";   item.displayName = "Oversampling";          break;
                    case 10: item.parameterId = "inputRouting"; item.displayName = "Input Channel Routing"; break;
                }
            };
            addAndMakeVisible(paramBox);

            // Channel ComboBox (Omni = 0, 1-16)
            channelBox.addItem("Omni", 1);
            for (int c = 1; c <= 16; ++c)
                channelBox.addItem(juce::String(c), c + 1);

            channelBox.setSelectedId(m.channel == 0 ? 1 : m.channel + 1, juce::dontSendNotification);
            channelBox.onChange = [this]() {
                auto& item = manager.getMappings()[static_cast<size_t>(rowIndex)];
                item.channel = channelBox.getSelectedId() - 1;
            };
            addAndMakeVisible(channelBox);

            // Control number ComboBox (CC #0 to CC #127)
            for (int cc = 0; cc <= 127; ++cc)
                ccBox.addItem("CC #" + juce::String(cc), cc + 1);

            ccBox.setSelectedId(m.controlNumber + 1, juce::dontSendNotification);
            ccBox.onChange = [this]() {
                auto& item = manager.getMappings()[static_cast<size_t>(rowIndex)];
                item.controlNumber = ccBox.getSelectedId() - 1;
            };
            addAndMakeVisible(ccBox);

            // Delete Button
            deleteButton.setButtonText("X");
            deleteButton.onClick = [this]() {
                manager.removeMapping(static_cast<size_t>(rowIndex));
                if (notifyUpdate) notifyUpdate();
            };
            addAndMakeVisible(deleteButton);
        }

        void paint(juce::Graphics& g) override
        {
            // Indicator dot
            g.setColour(juce::Colours::white);
            g.fillEllipse(10.0f, static_cast<float>(getHeight()) * 0.5f - 3.5f, 7.0f, 7.0f);
        }

        void resized() override
        {
            const int h = getHeight();
            typeBox.setBounds(28, 3, 140, h - 6);
            paramBox.setBounds(176, 3, 160, h - 6);
            channelBox.setBounds(344, 3, 65, h - 6);
            ccBox.setBounds(418, 3, 110, h - 6);
            deleteButton.setBounds(getWidth() - 32, 4, 24, h - 8);
        }

    private:
        MidiManager& manager;
        int rowIndex;
        std::function<void()> notifyUpdate;

        juce::ComboBox typeBox;
        juce::ComboBox paramBox;
        juce::ComboBox channelBox;
        juce::ComboBox ccBox;
        juce::TextButton deleteButton;
    };

    MidiManager& midiManager;
    std::function<void()> onClose;
    juce::TextButton closeButton;
    juce::TextButton addButton;
    juce::OwnedArray<RowComponent> rows;
};
