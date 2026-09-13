#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GoatLookAndFeel.h"
#include "MidiMappingModal.h"

class TheGoatAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    public juce::Timer,
                                    public juce::ComboBox::Listener,
                                    public juce::Button::Listener
{
public:
    explicit TheGoatAudioProcessorEditor(TheGoatAudioProcessor&);
    ~TheGoatAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button* button) override;

private:
    TheGoatAudioProcessor& audioProcessor;
    GoatLookAndFeel goatLookAndFeel;

    // Background image
    juce::Image backgroundImage;

    // --- Top Bar Preset Browser & Master Controls ---
    juce::TextButton prevPresetButton{"<"};
    juce::ComboBox categoryBox;
    juce::ComboBox presetBox;
    juce::TextButton nextPresetButton{">"};
    juce::TextButton savePresetButton{"SAVE"};
    juce::TextButton deletePresetButton{"DEL"};

    BrutalKnob inGainSlider{0.0};
    juce::Label inGainLabel;

    BrutalKnob blendSlider{1.0};
    juce::Label blendLabel;

    BrutalKnob masterVolSlider{0.0};
    juce::Label masterVolLabel;

    juce::ComboBox oversampleBox;
    juce::Label oversampleLabel;

    juce::TextButton midiMenuButton{"MIDI MAP"};
    BrutalPowerButton powerButton{"POWER ON"};

    // --- Sub-Banner / Ribbon ---
    juce::Label bannerLabel;

    // --- Main Chainsaw Stompbox Controls ---
    BrutalKnob distSlider{0.75};
    juce::Label distLabel;

    BrutalKnob lowSlider{18.0};
    juce::Label lowLabel;

    BrutalKnob highSlider{20.0};
    juce::Label highLabel;

    BrutalKnob volSlider{0.0};
    juce::Label volLabel;

    // 2-Way Diode Toggle Switch
    DiodeToggleSwitch diodeModeSwitch{"VINTAGE HM-2", "GOAT HOT-ROD"};
    juce::Label diodeModeLabel;

    // Heavy-Duty Footswitch Stomp
    GoatFootswitchComponent stompFootswitch{"STOMP BYPASS"};

    // Modal Overlays
    std::unique_ptr<MidiMappingModal> midiModal;

    // APVTS Attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<SliderAttachment> inGainAttachment;
    std::unique_ptr<SliderAttachment> blendAttachment;
    std::unique_ptr<SliderAttachment> masterVolAttachment;
    std::unique_ptr<SliderAttachment> distAttachment;
    std::unique_ptr<SliderAttachment> lowAttachment;
    std::unique_ptr<SliderAttachment> highAttachment;
    std::unique_ptr<SliderAttachment> volAttachment;
    std::unique_ptr<ComboBoxAttachment> oversampleAttachment;
    std::unique_ptr<ButtonAttachment> powerAttachment;

    std::vector<std::string> categories;
    bool isUpdatingPresetList = false;

    void updateCategoryList();
    void updatePresetListForCategory(const std::string& cat);
    void syncUiToPreset(int globalIndex);
    void showSavePresetDialog();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TheGoatAudioProcessorEditor)
};
