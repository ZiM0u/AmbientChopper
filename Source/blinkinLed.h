/*
  ==============================================================================

    blinkinLed.h
    Created: 17 Dec 2022 8:24:22am
    Author:  aurel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class blinkinLed  : public juce::Component, public juce::Timer
{
public:
    blinkinLed(std::atomic<double>& valueToUse) :value(valueToUse) 
    {
        startTimerHz(60);
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
    }

    ~blinkinLed() override
    {
    }

    void paint(juce::Graphics& g)
    {
        if (activated)
        {
            juce::Rectangle<float> ledArea(0, 0, getWidth(), getHeight());
            //g.setColour(juce::Colour(156, 218, 222));
            g.setColour(color);
            g.fillEllipse(ledArea);
        }
 

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        //setBounds(getLocalBounds());
    }
    void timerCallback() override
    {
        auto handler = this->getParentComponent()->getProperties();
        
        if (handler.getValueAt(0)>down)
        {
            activated = true;
            fixedvalue = (int)value.load();
            if (fixedvalue % divisor == 1)
            {
                color = juce::Colour(juce::Colours::green);
            }
            if (fixedvalue % divisor == 3)
            {
                color = juce::Colour(juce::Colours::black);
            }
        }
        else
        {
            color = juce::Colour(juce::Colours::blue);
        }
        
        //value += 1;
    }

private:
    juce::var down {0};
    bool activated{ true };
    juce::Colour color = juce::Colours::black;
    int fixedvalue;
    int divisor{ 4 };
    std::atomic<double>& value;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (blinkinLed)
};
