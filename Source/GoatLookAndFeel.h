#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <algorithm>
#include <cmath>

class GoatLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GoatLookAndFeel()
    {
        // Theme: Blood Red, Industrial Dark Charcoal, Deep Obsidian, White / Silver
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff08090b));
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff121419));
        setColour(juce::ComboBox::textColourId, juce::Colours::white);
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff3a1014));
        setColour(juce::ComboBox::arrowColourId, juce::Colour(0xffff2233)); // Blood Red
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff101216));
        setColour(juce::PopupMenu::textColourId, juce::Colours::white);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff800a14)); // Dark Blood Red
        setColour(juce::TextButton::buttonColourId, juce::Colour(0xff1a1d24));
        setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff2233));
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xee0a0c10));
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x55ff1e2e));
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override
    {
        juce::ignoreUnused(slider);

        const float radius = static_cast<float>(std::min(width, height)) * 0.5f - 4.0f;
        const float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
        const float centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // 1. Outer chassis shadow and recessed well
        g.setColour(juce::Colour(0xff030406));
        g.fillEllipse(centreX - radius - 3.0f, centreY - radius - 1.5f, (radius + 3.0f) * 2.0f, (radius + 3.0f) * 2.0f);

        // 2. Track background groove
        juce::Path trackPath;
        trackPath.addCentredArc(centreX, centreY, radius - 2.0f, radius - 2.0f, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(juce::Colour(0xff14171e));
        g.strokePath(trackPath, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 3. Active Glowing Blood Red Arc
        if (sliderPosProportional > 0.001f)
        {
            juce::Path activeArc;
            activeArc.addCentredArc(centreX, centreY, radius - 2.0f, radius - 2.0f, 0.0f, rotaryStartAngle, angle, true);
            // Outer soft glow
            g.setColour(juce::Colour(0x55ff1e2e));
            g.strokePath(activeArc, juce::PathStrokeType(7.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            // Inner crisp laser arc
            g.setColour(juce::Colour(0xffff2233));
            g.strokePath(activeArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // 4. Inner Machined Knurled Boss/Davies Knob Body
        const float knobRadius = radius - 8.5f;
        juce::ColourGradient knobGrad(juce::Colour(0xff343844), centreX, centreY - knobRadius,
                                      juce::Colour(0xff0d0f14), centreX, centreY + knobRadius, false);
        g.setGradientFill(knobGrad);
        g.fillEllipse(centreX - knobRadius, centreY - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);

        // Machined bezel rim
        g.setColour(juce::Colour(0xff525b6c));
        g.drawEllipse(centreX - knobRadius, centreY - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 1.2f);

        // Perimeter grip knurls
        const int numGrips = 16;
        g.setColour(juce::Colour(0xff08090d));
        for (int i = 0; i < numGrips; ++i)
        {
            const float a = static_cast<float>(i) * (juce::MathConstants<float>::twoPi / static_cast<float>(numGrips));
            const float gx = centreX + (knobRadius - 2.5f) * std::sin(a);
            const float gy = centreY - (knobRadius - 2.5f) * std::cos(a);
            g.fillEllipse(gx - 1.2f, gy - 1.2f, 2.4f, 2.4f);
        }

        // 5. Center Cap (Matte Gunmetal / Obsidian)
        const float capRadius = knobRadius * 0.62f;
        juce::ColourGradient capGrad(juce::Colour(0xff242730), centreX, centreY - capRadius,
                                     juce::Colour(0xff101216), centreX, centreY + capRadius, false);
        g.setGradientFill(capGrad);
        g.fillEllipse(centreX - capRadius, centreY - capRadius, capRadius * 2.0f, capRadius * 2.0f);
        g.setColour(juce::Colour(0xff444c5a));
        g.drawEllipse(centreX - capRadius, centreY - capRadius, capRadius * 2.0f, capRadius * 2.0f, 0.8f);

        // 6. Pointer Line (Glowing blood-red laser needle)
        juce::Path pointer;
        const float pointerLength = knobRadius - 2.5f;
        pointer.startNewSubPath(centreX + (capRadius * 0.35f) * std::sin(angle),
                                centreY - (capRadius * 0.35f) * std::cos(angle));
        pointer.lineTo(centreX + pointerLength * std::sin(angle),
                       centreY - pointerLength * std::cos(angle));

        g.setColour(juce::Colour(0x88ff1e2e));
        g.strokePath(pointer, juce::PathStrokeType(4.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
        g.setColour(juce::Colour(0xffff2233));
        g.strokePath(pointer, juce::PathStrokeType(2.2f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
        g.setColour(juce::Colours::white);
        g.strokePath(pointer, juce::PathStrokeType(1.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);

        juce::Colour base = backgroundColour;
        if (shouldDrawButtonAsDown)
            base = base.darker(0.35f);
        else if (shouldDrawButtonAsHighlighted)
            base = base.brighter(0.2f);

        juce::ColourGradient grad(base.brighter(0.12f), 0, bounds.getY(),
                                  base.darker(0.25f), 0, bounds.getBottom(), false);
        g.setGradientFill(grad);
        g.fillRoundedRectangle(bounds, 4.0f);

        // Blood Red highlight on hover
        g.setColour(shouldDrawButtonAsHighlighted ? juce::Colour(0xffff1e2e) : juce::Colour(0xff343a47));
        g.drawRoundedRectangle(bounds, 4.0f, 1.2f);
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override
    {
        juce::ignoreUnused(isButtonDown, buttonX, buttonY, buttonW, buttonH, box);

        auto bounds = juce::Rectangle<float>(0, 0, static_cast<float>(width), static_cast<float>(height)).reduced(1.0f);

        juce::ColourGradient grad(juce::Colour(0xff181a22), 0, 0,
                                  juce::Colour(0xff0c0d11), 0, static_cast<float>(height), false);
        g.setGradientFill(grad);
        g.fillRoundedRectangle(bounds, 4.0f);

        g.setColour(juce::Colour(0xff441217));
        g.drawRoundedRectangle(bounds, 4.0f, 1.2f);

        // Blood Red Arrow
        const float arrowX = static_cast<float>(width) - 16.0f;
        const float arrowY = static_cast<float>(height) * 0.5f;

        juce::Path p;
        p.startNewSubPath(arrowX - 4.0f, arrowY - 2.5f);
        p.lineTo(arrowX + 4.0f, arrowY - 2.5f);
        p.lineTo(arrowX, arrowY + 3.0f);
        p.closeSubPath();

        g.setColour(juce::Colour(0xffff2233));
        g.fillPath(p);
    }
};

// Brutal Rotary Knob Component with double-click reset
class BrutalKnob : public juce::Slider
{
public:
    explicit BrutalKnob(double defaultVal = 0.0)
        : defaultValue(defaultVal)
    {
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 18);
        setDoubleClickReturnValue(true, defaultValue);
    }

    std::function<void(const juce::MouseEvent&)> onRightClick;

    void mouseDown(const juce::MouseEvent& e) override
    {
        if (e.mods.isPopupMenu() && onRightClick)
        {
            onRightClick(e);
            return;
        }
        juce::Slider::mouseDown(e);
    }

private:
    double defaultValue;
};

// 2-Way Mini Metal Toggle Switch for Diode Clipping Mode
class DiodeToggleSwitch : public juce::Component
{
public:
    DiodeToggleSwitch(const juce::String& option1, const juce::String& option2)
        : opt1(option1), opt2(option2)
    {
    }

    int getSelectedIndex() const { return selectedIndex; }

    void setSelectedIndex(int idx, juce::NotificationType notify = juce::sendNotification)
    {
        idx = juce::jlimit(0, 1, idx);
        if (selectedIndex != idx)
        {
            selectedIndex = idx;
            repaint();
            if (notify == juce::sendNotification && onChange)
                onChange(selectedIndex);
        }
    }

    std::function<void(int newIndex)> onChange;
    std::function<void(const juce::MouseEvent&)> onRightClick;

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        const float switchH = 26.0f;
        const float switchY = bounds.getCentreY() - switchH * 0.5f;
        const float switchW = bounds.getWidth() - 4.0f;
        const float switchX = bounds.getX() + 2.0f;

        // Slot plate
        auto slotRect = juce::Rectangle<float>(switchX, switchY, switchW, switchH);
        g.setColour(juce::Colour(0xff090a0d));
        g.fillRoundedRectangle(slotRect, 4.0f);
        g.setColour(juce::Colour(0xff4a1218));
        g.drawRoundedRectangle(slotRect, 4.0f, 1.2f);

        // 2 Segments
        const float segW = switchW / 2.0f;
        const juce::String labels[2] = { opt1, opt2 };

        for (int i = 0; i < 2; ++i)
        {
            auto segRect = juce::Rectangle<float>(switchX + static_cast<float>(i) * segW, switchY, segW, switchH);
            const bool isSelected = (i == selectedIndex);

            if (isSelected)
            {
                // Glowing blood-red active segment
                juce::ColourGradient selGrad(juce::Colour(0xffff2233), 0, segRect.getY(),
                                             juce::Colour(0xff8a0a14), 0, segRect.getBottom(), false);
                g.setGradientFill(selGrad);
                g.fillRoundedRectangle(segRect.reduced(1.5f), 3.0f);
                g.setColour(juce::Colour(0xffffbbbb));
                g.drawRoundedRectangle(segRect.reduced(1.5f), 3.0f, 1.0f);
            }

            g.setFont(juce::FontOptions(11.0f, juce::Font::bold));
            g.setColour(isSelected ? juce::Colours::white : juce::Colour(0xff8a909d));
            g.drawText(labels[i], segRect, juce::Justification::centred);
        }
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        if (e.mods.isPopupMenu() && onRightClick)
        {
            onRightClick(e);
            return;
        }

        const float switchW = getWidth() - 4.0f;
        const float segW = switchW / 2.0f;
        const float clickX = static_cast<float>(e.x) - 2.0f;
        int clickedIdx = juce::jlimit(0, 1, static_cast<int>(clickX / segW));
        setSelectedIndex(clickedIdx);
    }

private:
    juce::String opt1, opt2;
    int selectedIndex = 0;
};

// Heavy-Duty 3D Stainless Steel Footswitch Stomp Component with dynamic LED indicator
class GoatFootswitchComponent : public juce::Component
{
public:
    explicit GoatFootswitchComponent(const juce::String& labelText)
        : name(labelText)
    {
    }

    void setLedActive(bool active)
    {
        if (isLedOn != active)
        {
            isLedOn = active;
            repaint();
        }
    }

    std::function<void(bool isDown)> onPointerState;
    std::function<void()> onClick;
    std::function<void(const juce::MouseEvent&)> onRightClick;

    void paint(juce::Graphics& g) override
    {
        const float cx = getWidth() * 0.5f;
        const float cy = getHeight() * 0.5f + 8.0f;
        const float r = 36.0f;

        // 1. Outer chassis drop shadow
        g.setColour(juce::Colour(0xff030406));
        g.fillEllipse(cx - r - 6.0f, cy - r - 3.0f, (r + 6.0f) * 2.0f, (r + 6.0f) * 2.0f);

        // 2. Heavy-duty knurled stainless steel hex nut / bezel
        const int numSides = 6;
        juce::Path hexPath;
        const float hexRadius = r + 3.0f;
        for (int i = 0; i < numSides; ++i)
        {
            const float angle = static_cast<float>(i) * (juce::MathConstants<float>::twoPi / 6.0f);
            const float px = cx + hexRadius * std::cos(angle);
            const float py = cy + hexRadius * std::sin(angle);
            if (i == 0) hexPath.startNewSubPath(px, py);
            else        hexPath.lineTo(px, py);
        }
        hexPath.closeSubPath();

        juce::ColourGradient nutGrad(juce::Colour(0xff747d8f), cx - hexRadius, cy - hexRadius,
                                     juce::Colour(0xff22262e), cx + hexRadius, cy + hexRadius, false);
        g.setGradientFill(nutGrad);
        g.fillPath(hexPath);
        g.setColour(juce::Colour(0xff8f99ac));
        g.strokePath(hexPath, juce::PathStrokeType(1.5f));

        // 3. Circular actuator plunger
        const float plungerR = isPressed ? (r - 7.0f) : (r - 5.0f);
        juce::ColourGradient plungerGrad(isPressed ? juce::Colour(0xff22252c) : juce::Colour(0xff4a5260), cx, cy - plungerR,
                                         isPressed ? juce::Colour(0xff121418) : juce::Colour(0xff282d36), cx, cy + plungerR, false);
        g.setGradientFill(plungerGrad);
        g.fillEllipse(cx - plungerR, cy - plungerR, plungerR * 2.0f, plungerR * 2.0f);

        g.setColour(juce::Colour(0xff758092));
        g.drawEllipse(cx - plungerR, cy - plungerR, plungerR * 2.0f, plungerR * 2.0f, 1.5f);

        // Center machined dimple
        g.setColour(juce::Colour(0xff14161b));
        g.fillEllipse(cx - 9.0f, cy - 9.0f, 18.0f, 18.0f);

        // 4. LED Jewel Indicator above switch
        const float ledY = cy - r - 20.0f;
        const float ledR = 8.0f;

        // LED Bezel
        g.setColour(juce::Colour(0xff1a1d24));
        g.fillEllipse(cx - ledR - 2.5f, ledY - ledR - 2.5f, (ledR + 2.5f) * 2.0f, (ledR + 2.5f) * 2.0f);
        g.setColour(juce::Colour(0xff4f5666));
        g.drawEllipse(cx - ledR - 2.5f, ledY - ledR - 2.5f, (ledR + 2.5f) * 2.0f, (ledR + 2.5f) * 2.0f, 1.0f);

        if (isLedOn)
        {
            // Blood-Red / Amber Laser Glow
            g.setColour(juce::Colour(0x66ff1e2e));
            g.fillEllipse(cx - ledR * 2.4f, ledY - ledR * 2.4f, ledR * 4.8f, ledR * 4.8f);

            juce::ColourGradient ledGrad(juce::Colour(0xffff9999), cx - 2.0f, ledY - 2.0f,
                                         juce::Colour(0xffff1122), cx + ledR, ledY + ledR, true);
            g.setGradientFill(ledGrad);
            g.fillEllipse(cx - ledR, ledY - ledR, ledR * 2.0f, ledR * 2.0f);

            // Hot Center Specular
            g.setColour(juce::Colours::white.withAlpha(0.9f));
            g.fillEllipse(cx - 2.5f, ledY - 3.5f, 5.0f, 4.0f);
        }
        else
        {
            // Dark unlit glass jewel
            juce::ColourGradient darkLed(juce::Colour(0xff3a1418), cx, ledY - ledR,
                                         juce::Colour(0xff120608), cx, ledY + ledR, false);
            g.setGradientFill(darkLed);
            g.fillEllipse(cx - ledR, ledY - ledR, ledR * 2.0f, ledR * 2.0f);
        }

        // Label below
        g.setFont(juce::FontOptions(13.0f, juce::Font::bold));
        g.setColour(isLedOn ? juce::Colour(0xffff2233) : juce::Colour(0xff8a909d));
        g.drawText(name, 0, static_cast<int>(cy + r + 6.0f), getWidth(), 20, juce::Justification::centred);
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        if (e.mods.isPopupMenu() && onRightClick)
        {
            onRightClick(e);
            return;
        }

        isPressed = true;
        repaint();
        if (onPointerState) onPointerState(true);
        if (onClick) onClick();
    }

    void mouseUp(const juce::MouseEvent& e) override
    {
        juce::ignoreUnused(e);
        isPressed = false;
        repaint();
        if (onPointerState) onPointerState(false);
    }

private:
    juce::String name;
    bool isLedOn = true;
    bool isPressed = false;
};

// Power Button with glowing status
class BrutalPowerButton : public juce::Button
{
public:
    explicit BrutalPowerButton(const juce::String& name)
        : juce::Button(name)
    {
    }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);

        const bool isPowerActive = getToggleState();

        juce::ColourGradient grad(isPowerActive ? juce::Colour(0xff8a0e16) : juce::Colour(0xff1c2028), 0, bounds.getY(),
                                  isPowerActive ? juce::Colour(0xff3a060a) : juce::Colour(0xff0c0e12), 0, bounds.getBottom(), false);
        g.setGradientFill(grad);
        g.fillRoundedRectangle(bounds, 4.0f);

        g.setColour(isPowerActive ? juce::Colour(0xffff2233) : juce::Colour(0xff383e4c));
        g.drawRoundedRectangle(bounds, 4.0f, 1.2f);

        g.setFont(juce::FontOptions(11.0f, juce::Font::bold));
        g.setColour(isPowerActive ? juce::Colours::white : juce::Colour(0xff8a909d));
        g.drawText(isPowerActive ? "POWER ON" : "BYPASS", bounds, juce::Justification::centred);
    }
};
