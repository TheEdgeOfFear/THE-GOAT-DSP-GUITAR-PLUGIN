#include "PluginEditor.h"
#include <BinaryData.h>

TheGoatAudioProcessorEditor::TheGoatAudioProcessorEditor(TheGoatAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setLookAndFeel(&goatLookAndFeel);

    // Load background image from BinaryData
    if (BinaryData::THE_GOAT_BG_jpg != nullptr && BinaryData::THE_GOAT_BG_jpgSize > 0)
    {
        backgroundImage = juce::ImageFileFormat::loadFrom(
            BinaryData::THE_GOAT_BG_jpg,
            static_cast<size_t>(BinaryData::THE_GOAT_BG_jpgSize));
    }

    // --- Top Bar Preset Browser ---
    prevPresetButton.addListener(this);
    addAndMakeVisible(prevPresetButton);

    categoryBox.addListener(this);
    addAndMakeVisible(categoryBox);

    presetBox.addListener(this);
    addAndMakeVisible(presetBox);

    nextPresetButton.addListener(this);
    addAndMakeVisible(nextPresetButton);

    savePresetButton.addListener(this);
    addAndMakeVisible(savePresetButton);

    deletePresetButton.addListener(this);
    addAndMakeVisible(deletePresetButton);

    // --- Header Controls ---
    // Input Routing (Auto, Left Only, Right Only, Stereo, Mono Sum)
    inputRoutingBox.addItem("AUTO (Detect)", 1);
    inputRoutingBox.addItem("L (In 1 -> Both)", 2);
    inputRoutingBox.addItem("R (In 2 -> Both)", 3);
    inputRoutingBox.addItem("STEREO (L+R)", 4);
    inputRoutingBox.addItem("MONO SUM", 5);
    inputRoutingBox.setSelectedId(1, juce::dontSendNotification);
    addAndMakeVisible(inputRoutingBox);
    inputRoutingLabel.setText("INPUT CH", juce::dontSendNotification);
    inputRoutingLabel.setJustificationType(juce::Justification::centred);
    inputRoutingLabel.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    inputRoutingLabel.setColour(juce::Label::textColourId, juce::Colour(0xff8a909d));
    addAndMakeVisible(inputRoutingLabel);

    // Input Gain Trim
    inGainSlider.setRange(-24.0, 12.0, 0.1);
    inGainSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(inGainSlider);
    inGainLabel.setText("INPUT", juce::dontSendNotification);
    inGainLabel.setJustificationType(juce::Justification::centred);
    inGainLabel.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    inGainLabel.setColour(juce::Label::textColourId, juce::Colour(0xff8a909d));
    addAndMakeVisible(inGainLabel);

    // Blend
    blendSlider.setRange(0.0, 1.0, 0.01);
    blendSlider.setTextValueSuffix(" WET");
    addAndMakeVisible(blendSlider);
    blendLabel.setText("BLEND", juce::dontSendNotification);
    blendLabel.setJustificationType(juce::Justification::centred);
    blendLabel.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    blendLabel.setColour(juce::Label::textColourId, juce::Colour(0xff8a909d));
    addAndMakeVisible(blendLabel);

    // Master Output Volume
    masterVolSlider.setRange(-30.0, 12.0, 0.1);
    masterVolSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(masterVolSlider);
    masterVolLabel.setText("OUTPUT", juce::dontSendNotification);
    masterVolLabel.setJustificationType(juce::Justification::centred);
    masterVolLabel.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    masterVolLabel.setColour(juce::Label::textColourId, juce::Colour(0xff8a909d));
    addAndMakeVisible(masterVolLabel);

    // Oversampling ComboBox
    oversampleBox.addItem("1x (Off)", 1);
    oversampleBox.addItem("2x Poly", 2);
    oversampleBox.addItem("4x Poly", 3);
    oversampleBox.addItem("8x Poly", 4);
    oversampleBox.setSelectedId(3, juce::dontSendNotification);
    addAndMakeVisible(oversampleBox);
    oversampleLabel.setText("OVERSAMPLE", juce::dontSendNotification);
    oversampleLabel.setJustificationType(juce::Justification::centred);
    oversampleLabel.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    oversampleLabel.setColour(juce::Label::textColourId, juce::Colour(0xff8a909d));
    addAndMakeVisible(oversampleLabel);

    // MIDI Map & Power Buttons
    midiMenuButton.addListener(this);
    addAndMakeVisible(midiMenuButton);

    powerButton.setClickingTogglesState(true);
    powerButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(powerButton);

    // --- Sub-Banner / Ribbon ---
    bannerLabel.setText("THE EDGE OF FEAR  //  HM-2 CULT CHAINSAW & GYRATOR TONE STACK", juce::dontSendNotification);
    bannerLabel.setFont(juce::FontOptions(12.0f, juce::Font::bold));
    bannerLabel.setJustificationType(juce::Justification::centred);
    bannerLabel.setColour(juce::Label::textColourId, juce::Colour(0xffff2233));
    addAndMakeVisible(bannerLabel);

    // --- 4 Main Massive Knobs ---
    // 1. GAIN / HORNS
    distSlider.setRange(0.0, 1.0, 0.01);
    distSlider.setTextValueSuffix("");
    addAndMakeVisible(distSlider);
    distLabel.setText("HORNS", juce::dontSendNotification);
    distLabel.setJustificationType(juce::Justification::centred);
    distLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    distLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(distLabel);

    // 2. LOW (100 Hz Gyrator)
    lowSlider.setRange(-18.0, 18.0, 0.1);
    lowSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(lowSlider);
    lowLabel.setText("LOW (100Hz)", juce::dontSendNotification);
    lowLabel.setJustificationType(juce::Justification::centred);
    lowLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    lowLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(lowLabel);

    // 3. HIGH (1.3 kHz Gyrator)
    highSlider.setRange(-20.0, 20.0, 0.1);
    highSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(highSlider);
    highLabel.setText("HIGH (1.3kHz)", juce::dontSendNotification);
    highLabel.setJustificationType(juce::Justification::centred);
    highLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    highLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(highLabel);

    // 4. LEVEL (Master)
    volSlider.setRange(-30.0, 12.0, 0.1);
    volSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(volSlider);
    volLabel.setText("LEVEL", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centred);
    volLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    volLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(volLabel);

    // 2-Way Diode Mode Toggle Switch
    diodeModeSwitch.onChange = [this](int newIdx) {
        if (auto* param = audioProcessor.getAPVTS().getParameter("mode"))
            param->setValueNotifyingHost(param->convertTo0to1(static_cast<float>(newIdx)));
    };
    addAndMakeVisible(diodeModeSwitch);
    diodeModeLabel.setText("CLIPPING ENGINE", juce::dontSendNotification);
    diodeModeLabel.setJustificationType(juce::Justification::centred);
    diodeModeLabel.setFont(juce::FontOptions(11.0f, juce::Font::bold));
    diodeModeLabel.setColour(juce::Label::textColourId, juce::Colour(0xff8a909d));
    addAndMakeVisible(diodeModeLabel);

    // Footswitch
    stompFootswitch.onClick = [this]() {
        const bool newState = !powerButton.getToggleState();
        powerButton.setToggleState(newState, juce::sendNotification);
        stompFootswitch.setLedActive(newState);
    };
    addAndMakeVisible(stompFootswitch);

    // APVTS Attachments
    auto& apvts = audioProcessor.getAPVTS();
    inGainAttachment       = std::make_unique<SliderAttachment>(apvts, "inGain", inGainSlider);
    blendAttachment        = std::make_unique<SliderAttachment>(apvts, "blend", blendSlider);
    masterVolAttachment    = std::make_unique<SliderAttachment>(apvts, "vol", masterVolSlider);
    distAttachment         = std::make_unique<SliderAttachment>(apvts, "dist", distSlider);
    lowAttachment          = std::make_unique<SliderAttachment>(apvts, "low", lowSlider);
    highAttachment         = std::make_unique<SliderAttachment>(apvts, "high", highSlider);
    volAttachment          = std::make_unique<SliderAttachment>(apvts, "vol", volSlider);
    oversampleAttachment   = std::make_unique<ComboBoxAttachment>(apvts, "oversample", oversampleBox);
    inputRoutingAttachment = std::make_unique<ComboBoxAttachment>(apvts, "inputRouting", inputRoutingBox);
    powerAttachment        = std::make_unique<ButtonAttachment>(apvts, "power", powerButton);

    // Synchronize mode switch from APVTS initial state
    if (auto* param = apvts.getRawParameterValue("mode"))
        diodeModeSwitch.setSelectedIndex(static_cast<int>(param->load()), juce::dontSendNotification);

    // Populate Presets
    updateCategoryList();

    // Default Window Dimensions (16:9 proportion, 960 x 540)
    setSize(960, 540);
    setResizable(true, true);
    setResizeLimits(720, 405, 1440, 810);
    getConstrainer()->setFixedAspectRatio(16.0 / 9.0);

    startTimerHz(20);
}

TheGoatAudioProcessorEditor::~TheGoatAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void TheGoatAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Draw Background
    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat(),
                    juce::RectanglePlacement::stretchToFit);
    }
    else
    {
        // Dark metallic gradient fallback
        juce::ColourGradient bgGrad(juce::Colour(0xff181a20), 0, 0,
                                    juce::Colour(0xff060709), 0, static_cast<float>(getHeight()), false);
        g.setGradientFill(bgGrad);
        g.fillAll();
    }

    // Top Bar Ribbon Glass Overlay
    auto topBar = juce::Rectangle<float>(0, 0, static_cast<float>(getWidth()), 52.0f);
    g.setColour(juce::Colour(0xdd080a0e));
    g.fillRect(topBar);
    g.setColour(juce::Colour(0x66ff1e2e));
    g.drawLine(0, 52.0f, static_cast<float>(getWidth()), 52.0f, 1.5f);

    // Sub-Banner Background
    auto subBanner = juce::Rectangle<float>(0, 53.0f, static_cast<float>(getWidth()), 24.0f);
    g.setColour(juce::Colour(0xcc100406));
    g.fillRect(subBanner);
    g.setColour(juce::Colour(0x33ff1e2e));
    g.drawLine(0, 77.0f, static_cast<float>(getWidth()), 77.0f, 1.0f);

    // Stompbox Control Chassis Well (Translucent Dark Obsidian with glowing border)
    const float wellMarginX = static_cast<float>(getWidth()) * 0.05f;
    const float wellY = 88.0f;
    const float wellW = static_cast<float>(getWidth()) - (wellMarginX * 2.0f);
    const float wellH = static_cast<float>(getHeight()) - wellY - 14.0f;

    auto wellRect = juce::Rectangle<float>(wellMarginX, wellY, wellW, wellH);
    g.setColour(juce::Colour(0x80040508));
    g.fillRoundedRectangle(wellRect, 8.0f);
    g.setColour(juce::Colour(0x55ff1e2e));
    g.drawRoundedRectangle(wellRect, 8.0f, 1.5f);

    // Swedish Death Metal / Deathcore Stencil Titles
    g.setFont(juce::FontOptions(22.0f, juce::Font::bold));
    g.setColour(juce::Colour(0xccffffff));
    g.drawText("THE GOAT", static_cast<int>(wellMarginX + 20.0f), static_cast<int>(wellY + 12.0f), 300, 26, juce::Justification::centredLeft);

    g.setFont(juce::FontOptions(11.0f, juce::Font::bold));
    g.setColour(juce::Colour(0xffff2233));
    g.drawText("SWEDISH CHAINSAW TONE STACK", static_cast<int>(wellMarginX + 20.0f), static_cast<int>(wellY + 36.0f), 300, 16, juce::Justification::centredLeft);
}

void TheGoatAudioProcessorEditor::resized()
{
    const int w = getWidth();
    const int h = getHeight();

    // Top Bar (y = 0..52)
    const int topMargin = 10;
    const int btnH = 26;

    prevPresetButton.setBounds(10, topMargin, 24, btnH);
    categoryBox.setBounds(36, topMargin, 120, btnH);
    presetBox.setBounds(158, topMargin, 140, btnH);
    nextPresetButton.setBounds(300, topMargin, 24, btnH);
    savePresetButton.setBounds(326, topMargin, 42, btnH);
    deletePresetButton.setBounds(370, topMargin, 38, btnH);

    // Header Sliders & Controls (Right side of top bar)
    const int headerKnobSize = 34;
    int curX = w - 460;

    inputRoutingBox.setBounds(curX, topMargin, 105, btnH);
    inputRoutingLabel.setBounds(curX, topMargin + 26, 105, 14);
    curX += 112;

    inGainSlider.setBounds(curX, topMargin - 4, headerKnobSize, headerKnobSize);
    inGainLabel.setBounds(curX - 10, topMargin + 26, headerKnobSize + 20, 14);
    curX += 52;

    blendSlider.setBounds(curX, topMargin - 4, headerKnobSize, headerKnobSize);
    blendLabel.setBounds(curX - 10, topMargin + 26, headerKnobSize + 20, 14);
    curX += 52;

    masterVolSlider.setBounds(curX, topMargin - 4, headerKnobSize, headerKnobSize);
    masterVolLabel.setBounds(curX - 10, topMargin + 26, headerKnobSize + 20, 14);
    curX += 56;

    oversampleBox.setBounds(curX, topMargin, 85, btnH);
    oversampleLabel.setBounds(curX, topMargin + 26, 85, 14);
    curX += 92;

    midiMenuButton.setBounds(curX, topMargin, 66, btnH);
    curX += 70;

    powerButton.setBounds(w - 78, topMargin, 68, btnH);

    // Sub-Banner
    bannerLabel.setBounds(0, 53, w, 24);

    // Main Stompbox Faceplate Layout
    const float wellMarginX = static_cast<float>(w) * 0.05f;
    const float wellY = 88.0f;
    const float wellW = static_cast<float>(w) - (wellMarginX * 2.0f);
    const float wellH = static_cast<float>(h) - wellY - 14.0f;

    // 4 Massive Knobs across the middle row
    const int knobDiameter = static_cast<int>(std::min(wellW * 0.18f, wellH * 0.38f));
    const int knobY = static_cast<int>(wellY + wellH * 0.16f);
    const int spacing = static_cast<int>(wellW / 4.0f);

    // 1. HORNS (Gain)
    int kx = static_cast<int>(wellMarginX + (spacing * 0.5f) - (knobDiameter * 0.5f));
    distSlider.setBounds(kx, knobY, knobDiameter, knobDiameter);
    distLabel.setBounds(kx - 10, knobY + knobDiameter + 2, knobDiameter + 20, 20);

    // 2. LOW (100 Hz)
    kx = static_cast<int>(wellMarginX + (spacing * 1.5f) - (knobDiameter * 0.5f));
    lowSlider.setBounds(kx, knobY, knobDiameter, knobDiameter);
    lowLabel.setBounds(kx - 10, knobY + knobDiameter + 2, knobDiameter + 20, 20);

    // 3. HIGH (1.3 kHz)
    kx = static_cast<int>(wellMarginX + (spacing * 2.5f) - (knobDiameter * 0.5f));
    highSlider.setBounds(kx, knobY, knobDiameter, knobDiameter);
    highLabel.setBounds(kx - 10, knobY + knobDiameter + 2, knobDiameter + 20, 20);

    // 4. LEVEL (Master)
    kx = static_cast<int>(wellMarginX + (spacing * 3.5f) - (knobDiameter * 0.5f));
    volSlider.setBounds(kx, knobY, knobDiameter, knobDiameter);
    volLabel.setBounds(kx - 10, knobY + knobDiameter + 2, knobDiameter + 20, 20);

    // Lower Section: Diode Mode Toggle on left & Center Stomp Footswitch
    const int toggleW = 220;
    const int toggleH = 32;
    const int toggleX = static_cast<int>(wellMarginX + 40.0f);
    const int toggleY = static_cast<int>(wellY + wellH * 0.72f);
    diodeModeSwitch.setBounds(toggleX, toggleY, toggleW, toggleH);
    diodeModeLabel.setBounds(toggleX, toggleY - 18, toggleW, 16);

    // Center Stomp Footswitch
    const int footW = 160;
    const int footH = static_cast<int>(wellH * 0.38f);
    const int footX = static_cast<int>(w * 0.5f - footW * 0.5f);
    const int footY = static_cast<int>(wellY + wellH * 0.58f);
    stompFootswitch.setBounds(footX, footY, footW, footH);

    // MIDI Modal
    if (midiModal != nullptr)
        midiModal->setBounds(getLocalBounds().reduced(40, 30));
}

void TheGoatAudioProcessorEditor::timerCallback()
{
    // Sync power LED with APVTS power parameter
    const bool isPowerOn = powerButton.getToggleState();
    stompFootswitch.setLedActive(isPowerOn);

    // Sync diode clipping switch if changed via host or automation
    if (auto* param = audioProcessor.getAPVTS().getRawParameterValue("mode"))
    {
        const int modeIdx = static_cast<int>(param->load());
        if (diodeModeSwitch.getSelectedIndex() != modeIdx)
            diodeModeSwitch.setSelectedIndex(modeIdx, juce::dontSendNotification);
    }
}

void TheGoatAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    if (isUpdatingPresetList)
        return;

    if (box == &categoryBox)
    {
        const int selId = categoryBox.getSelectedId();
        if (selId > 0 && selId <= static_cast<int>(categories.size()))
            updatePresetListForCategory(categories[static_cast<size_t>(selId - 1)]);
    }
    else if (box == &presetBox)
    {
        const int globalIdx = presetBox.getSelectedId() - 1;
        if (globalIdx >= 0)
        {
            audioProcessor.loadPreset(globalIdx);
            syncUiToPreset(globalIdx);
        }
    }
}

void TheGoatAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    auto& presetMgr = audioProcessor.getPresetManager();
    const auto& allPresets = presetMgr.getAllPresets();
    int curIdx = audioProcessor.getCurrentProgram();

    if (button == &prevPresetButton)
    {
        if (curIdx > 0)
        {
            audioProcessor.loadPreset(curIdx - 1);
            syncUiToPreset(curIdx - 1);
        }
    }
    else if (button == &nextPresetButton)
    {
        if (curIdx < static_cast<int>(allPresets.size()) - 1)
        {
            audioProcessor.loadPreset(curIdx + 1);
            syncUiToPreset(curIdx + 1);
        }
    }
    else if (button == &savePresetButton)
    {
        showSavePresetDialog();
    }
    else if (button == &deletePresetButton)
    {
        if (!presetMgr.isCurrentPresetFactory(curIdx))
        {
            presetMgr.deleteUserPreset(curIdx);
            updateCategoryList();
            audioProcessor.loadPreset(0);
            syncUiToPreset(0);
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Cannot Delete",
                "Factory presets are permanent and cannot be deleted.");
        }
    }
    else if (button == &midiMenuButton)
    {
        if (midiModal == nullptr)
        {
            midiModal = std::make_unique<MidiMappingModal>(audioProcessor.getMidiManager(), [this]() {
                midiModal.reset();
                repaint();
            });
            addAndMakeVisible(midiModal.get());
            midiModal->setBounds(getLocalBounds().reduced(40, 30));
        }
        else
        {
            midiModal.reset();
            repaint();
        }
    }
}

void TheGoatAudioProcessorEditor::updateCategoryList()
{
    isUpdatingPresetList = true;
    categoryBox.clear(juce::dontSendNotification);

    auto& presetMgr = audioProcessor.getPresetManager();
    categories = presetMgr.getCategories();

    for (size_t i = 0; i < categories.size(); ++i)
        categoryBox.addItem(categories[i], static_cast<int>(i + 1));

    categoryBox.setSelectedId(1, juce::dontSendNotification);
    isUpdatingPresetList = false;

    if (!categories.empty())
        updatePresetListForCategory(categories[0]);
}

void TheGoatAudioProcessorEditor::updatePresetListForCategory(const std::string& cat)
{
    isUpdatingPresetList = true;
    presetBox.clear(juce::dontSendNotification);

    auto& presetMgr = audioProcessor.getPresetManager();
    const auto& allPresets = presetMgr.getAllPresets();

    int firstMatchGlobalIdx = -1;

    for (size_t i = 0; i < allPresets.size(); ++i)
    {
        if (allPresets[i].category == cat)
        {
            presetBox.addItem(allPresets[i].name, static_cast<int>(i + 1));
            if (firstMatchGlobalIdx == -1)
                firstMatchGlobalIdx = static_cast<int>(i);
        }
    }

    if (firstMatchGlobalIdx != -1)
    {
        presetBox.setSelectedId(firstMatchGlobalIdx + 1, juce::dontSendNotification);
        audioProcessor.loadPreset(firstMatchGlobalIdx);
        syncUiToPreset(firstMatchGlobalIdx);
    }

    isUpdatingPresetList = false;
}

void TheGoatAudioProcessorEditor::syncUiToPreset(int globalIndex)
{
    auto& presetMgr = audioProcessor.getPresetManager();
    const auto& allPresets = presetMgr.getAllPresets();

    if (globalIndex < 0 || globalIndex >= static_cast<int>(allPresets.size()))
        return;

    const auto& p = allPresets[static_cast<size_t>(globalIndex)];

    isUpdatingPresetList = true;

    // Select category
    for (size_t i = 0; i < categories.size(); ++i)
    {
        if (categories[i] == p.category)
        {
            categoryBox.setSelectedId(static_cast<int>(i + 1), juce::dontSendNotification);
            break;
        }
    }

    // Refresh presetBox items for this category if needed
    presetBox.clear(juce::dontSendNotification);
    for (size_t i = 0; i < allPresets.size(); ++i)
    {
        if (allPresets[i].category == p.category)
            presetBox.addItem(allPresets[i].name, static_cast<int>(i + 1));
    }
    presetBox.setSelectedId(globalIndex + 1, juce::dontSendNotification);

    diodeModeSwitch.setSelectedIndex(p.clippingMode, juce::dontSendNotification);

    isUpdatingPresetList = false;
}

void TheGoatAudioProcessorEditor::showSavePresetDialog()
{
    auto* aw = new juce::AlertWindow("SAVE USER PRESET", "Enter a name for your custom Goat preset:", juce::AlertWindow::QuestionIcon);
    aw->addTextEditor("PresetName", "My Custom Chainsaw", "Preset Name:");
    aw->addButton("SAVE", 1, juce::KeyPress(juce::KeyPress::returnKey));
    aw->addButton("CANCEL", 0, juce::KeyPress(juce::KeyPress::escapeKey));

    aw->enterModalState(true, juce::ModalCallbackFunction::create([this, aw](int result) {
        if (result == 1)
        {
            auto name = aw->getTextEditorContents("PresetName").trim().toStdString();
            if (!name.empty())
            {
                auto& apvts = audioProcessor.getAPVTS();
                GoatPresets::Preset newP;
                newP.name = name;
                newP.category = "User Presets";
                newP.description = "User created Chainsaw tone";
                newP.dist = apvts.getRawParameterValue("dist")->load();
                newP.low = apvts.getRawParameterValue("low")->load();
                newP.high = apvts.getRawParameterValue("high")->load();
                newP.volume = apvts.getRawParameterValue("vol")->load();
                newP.blend = apvts.getRawParameterValue("blend")->load();
                newP.clippingMode = static_cast<int>(apvts.getRawParameterValue("mode")->load());
                newP.inputGain = apvts.getRawParameterValue("inGain")->load();
                newP.oversample = static_cast<int>(apvts.getRawParameterValue("oversample")->load());
                newP.isFactory = false;

                int newIdx = 0;
                if (audioProcessor.getPresetManager().saveUserPreset(name, newP, &newIdx))
                {
                    updateCategoryList();
                    syncUiToPreset(newIdx);
                }
            }
        }
        delete aw;
    }));
}
