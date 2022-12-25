/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "blinkinLed.h"
#include "MyButton.h"


struct AtomicLabel :juce::Component, juce::Timer
{
    AtomicLabel(std::atomic<double>& valueToUse) :value(valueToUse)
    {
        startTimerHz(60);
        addAndMakeVisible(label);
    }
    void resized() override { label.setBounds(getLocalBounds()); }

    void timerCallback() override
    {
        label.setText(juce::String(value.load()), juce::dontSendNotification);


    }
    juce::Label label;
    std::atomic<double>& value;
};

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MyLookAndFeel()
    {
        
    }
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        g.setColour(juce::Colour(205, 235, 236));
        //g.fillEllipse(x, y, width, height);
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        // fill
        //g.setColour(juce::Colours::orange);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        //g.setColour(juce::Colours::red);
        //g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.66f;
        auto pointerThickness = 10.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        // pointer
        g.setColour(juce::Colour(60,90,118));
        g.fillPath(p);
        
    }/*
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)override
    {

        if (shouldDrawButtonAsDown)
        {
            color = juce::Colour(juce::Colours::green);
        }
        else
        {
            color = juce::Colour(156, 218, 222);
        }
        juce::Rectangle<int> buttonArea = g.getClipBounds();
        //g.drawRect(buttonArea, 1.f);
        
        //g.fillRect(buttonArea);
        juce::Path p;
        p.startNewSubPath(0, buttonArea.getHeight());
        p.quadraticTo(0, 0, buttonArea.getCentreX(), 0);
        p.lineTo(buttonArea.getWidth(),0);
        p.quadraticTo(buttonArea.getWidth(),buttonArea.getHeight(), buttonArea.getCentreX(), button.getHeight());
        p.lineTo(0, buttonArea.getHeight());
        shadower.drawForPath(g, p);
        g.setColour(juce::Colour(173, 209, 210));
        g.fillPath(p);
        g.setColour(juce::Colour(60, 90, 118));
        g.strokePath(p, juce::PathStrokeType(1.0f));
        
        juce::Rectangle<float> ledArea(buttonArea.getCentreX() * 1.25, 5, buttonArea.getCentreX() * 0.45, buttonArea.getCentreX() * 0.45);
        g.setColour(color);
        g.fillEllipse(ledArea);
        g.setColour(juce::Colour(60, 90, 118));
        g.drawEllipse(ledArea, 1.f);
        g.setFont(15.0f);
        g.drawFittedText("1/   .", buttonArea, juce::Justification::centred, 1);
        
    }*/
    void drawTickBox(juce::Graphics& g, juce::Component& box, float x, float	y, float	w, float h,
        bool ticked, bool 	isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {

        //auto* child = dynamic_cast<juce::Component> box.getChildComponent(0);
        if (ticked)
        {

            color = juce::Colour(juce::Colour(205, 235, 236));
 
        }
        else
        {
            color = juce::Colour(60, 90, 118);
        }

        juce::Rectangle<int> buttonArea = box.getLocalBounds();
        juce::Path p;
        p.startNewSubPath(0, buttonArea.getHeight());
        p.quadraticTo(0, 0, buttonArea.getCentreX(), 0);
        p.lineTo(buttonArea.getWidth(), 0);
        p.quadraticTo(buttonArea.getWidth(), buttonArea.getHeight(), buttonArea.getCentreX(), buttonArea.getHeight());
        p.lineTo(0, buttonArea.getHeight());
        if (!ticked) { shadower.drawForPath(g, p); }
        g.setColour(juce::Colour(173, 209, 210));
        g.fillPath(p);
        g.setColour(juce::Colour(60, 90, 118));
        g.strokePath(p, juce::PathStrokeType(1.0f));

        juce::Path p2;
        p2.startNewSubPath(borderPadding, buttonArea.getHeight()- borderPadding);
        p2.quadraticTo(borderPadding, borderPadding, buttonArea.getCentreX(), borderPadding);
        p2.lineTo(buttonArea.getWidth()-2, 2);
        p2.quadraticTo(buttonArea.getWidth()- borderPadding, buttonArea.getHeight()- borderPadding, buttonArea.getCentreX(), buttonArea.getHeight()- borderPadding);
        p2.lineTo(borderPadding, buttonArea.getHeight()- borderPadding);
        g.setColour(color);
        if (ticked) { g.strokePath(p2, juce::PathStrokeType(1.0f)); }

        



        juce::Rectangle<float> ledArea(buttonArea.getCentreX() * 1.25, 5, buttonArea.getCentreX() * 0.45, buttonArea.getCentreX() * 0.45);
        g.setColour(juce::Colour(156, 218, 222));
        g.fillEllipse(ledArea);
        g.setColour(juce::Colour(60, 90, 118));
        g.drawEllipse(ledArea, 1.f);
        g.setFont(15.0f);
        g.drawFittedText("1/   .", buttonArea, juce::Justification::centred, 1);
    }

    
private:
    juce::DropShadow shadower;
    int var{ 0 };
    juce::Colour color;

    float borderPadding{ 1.5 };
};
//==============================================================================
/**
*/
class AmbientChopperAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AmbientChopperAudioProcessorEditor (AmbientChopperAudioProcessor&);
    ~AmbientChopperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AmbientChopperAudioProcessor& audioProcessor;

    MyLookAndFeel myLookAndFeel;

    int width{960};
    int height{540};

    juce::Slider sizeSlider;
    juce::Slider dryWetSlider;
    juce::Slider attackSlider;
    juce::Slider decaySlider;

    int bigSliderWidth{ 180 };
    int bigSliderPadding{ 88 };
    int smallSliderWidth{ 50 };

    int buttonWidth{ 85 };
    int buttonHeight{ 30};

    MyButton seqOne;

    
    MyButton seqTwo ;
    MyButton seqThree;
    MyButton seqFour;
    
    
    AtomicLabel positionLabel;
    //blinkinLed led;

    enum RadioButtonIds
    {
        sequencerMode = 1001
    };
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sizeAttachment;
    std::unique_ptr<SliderAttachment> dryWetAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmbientChopperAudioProcessorEditor)
};
