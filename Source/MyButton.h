/*
  ==============================================================================

    MyButton.h
    Created: 17 Dec 2022 8:21:29pm
    Author:  aurel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "blinkinLed.h"

//==============================================================================
/*
*/
class MyButton  : public juce::ToggleButton,public juce::Timer
{
public:
    MyButton(std::atomic<double>& valueToUse) : value(valueToUse)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        startTimerHz(60);
        
        
        

    }

    ~MyButton() override
    {
    }

    void paint (juce::Graphics& g) override
    {

        buttonArea = getLocalBounds();

        juce::Path p;
        p.startNewSubPath(0, buttonArea.getHeight());
        p.quadraticTo(0, 0, buttonArea.getCentreX(), 0);
        p.lineTo(buttonArea.getWidth(), 0);
        p.quadraticTo(buttonArea.getWidth(), buttonArea.getHeight(), buttonArea.getCentreX(), buttonArea.getHeight());
        p.lineTo(0, buttonArea.getHeight());
        if (!getToggleState()) { shadower.drawForPath(g, p); }
        g.setColour(juce::Colour(173, 209, 210));
        g.fillPath(p);
        g.setColour(juce::Colour(60, 90, 118));
        g.strokePath(p, juce::PathStrokeType(1.0f));

        juce::Path p2;
        p2.startNewSubPath(borderPadding, buttonArea.getHeight() - borderPadding);
        p2.quadraticTo(borderPadding, borderPadding, buttonArea.getCentreX(), borderPadding);
        p2.lineTo(buttonArea.getWidth() - 2, 2);
        p2.quadraticTo(buttonArea.getWidth() - borderPadding, buttonArea.getHeight() - borderPadding, buttonArea.getCentreX(), buttonArea.getHeight() - borderPadding);
        p2.lineTo(borderPadding, buttonArea.getHeight() - borderPadding);
        g.setColour(color);
        if (!getToggleState()) { g.strokePath(p2, juce::PathStrokeType(1.0f)); }





        ledArea = juce::Rectangle<float>(buttonArea.getCentreX() * 1.25, 5, buttonArea.getCentreX() * 0.45, buttonArea.getCentreX() * 0.45);
        g.setColour(juce::Colour(156, 218, 222));
        g.fillEllipse(ledArea);
        g.setColour(juce::Colour(60, 90, 118));
        g.drawEllipse(ledArea, 1.f);
        g.setFont(15.0f);
        g.drawFittedText("1/"+getName(), buttonArea, juce::Justification::centred, 1);
        ledGradient = juce::ColourGradient(ledColor, ledArea.getCentreX(), ledArea.getCentreY(), gradientColor2, ledArea.getCentreX(), ledArea.getCentreY()/2, true);
        if (isActive && lightTiming<30)
        {
            g.setGradientFill(ledGradient);
            
        }
        else
        {
            //g.setGradientFill(juce::ColourGradient());
            g.setColour(ledColor);
        }
        //juce::Rectangle<float> ledArea(0, 0, getWidth(), getHeight());
        
        //g.setColour(color);
        g.fillEllipse(ledArea);

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }
    void buttonStateChanged() override
    {
        isActive = getToggleState();
        ledColor = juce::Colour(156, 218, 222);
    }

    void timerCallback() override
    {
        if (isActive)
        {
            fixedvalue = (int)value.load();
            if (tmp != fixedvalue)
            {
                tmp = fixedvalue;
                ledColor = juce::Colour(juce::Colours::antiquewhite);
                lightTiming = 0;

            }
            else
            {
                if (lightTiming == 5)
                {
                    ledColor = juce::Colour(156, 218, 222);
                    //repaint();
                }
                lightTiming += 1;

            }
        }

    }

private:
    juce::Rectangle<int> buttonArea;
    juce::Rectangle<float> ledArea;
    juce::ColourGradient ledGradient;
    juce::Colour gradientColor1{ 230,230,255 };
    juce::Colour gradientColor2{ 156,218,222 };
    
    juce::DropShadow shadower;
    int var{ 0 };
    int fixedvalue;
    int divisor{ 1 };
    int tmp{ 0 };
    int lightTiming{ 0 };
    juce::Colour color;
    juce::Colour ledColor = juce::Colour(156, 218, 222);
    bool isActive{ false }; //MUST SET IN PARAMETERS TO KEEP IT SAVED
    float borderPadding{ 1.5 };
    std::atomic<double>& value;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyButton)
};
