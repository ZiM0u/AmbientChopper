/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AmbientChopperAudioProcessorEditor::AmbientChopperAudioProcessorEditor (AmbientChopperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), positionLabel(p.currentPosition),seqOne(p.currentPosition), seqTwo(p.currentPosition),
                                                                                      seqThree(p.currentPosition), seqFour(p.currentPosition)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (width, height);
    addAndMakeVisible(positionLabel);
    addAndMakeVisible(sizeSlider);
    sizeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);   
    sizeSlider.setLookAndFeel(&myLookAndFeel);
    sizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    

    addAndMakeVisible(dryWetSlider);
    dryWetSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dryWetSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    dryWetSlider.setLookAndFeel(&myLookAndFeel);

    addAndMakeVisible(seqOne);
    seqOne.setRadioGroupId(RadioButtonIds::sequencerMode);
    //seqOne.setLookAndFeel(&myLookAndFeel);
    seqOne.setName("1");
    addAndMakeVisible(seqTwo);
    seqTwo.setRadioGroupId(RadioButtonIds::sequencerMode);
    //seqTwo.setLookAndFeel(&myLookAndFeel);
    seqTwo.setName("2");
    addAndMakeVisible(seqThree);
    seqThree.setRadioGroupId(RadioButtonIds::sequencerMode);
    //seqThree.setLookAndFeel(&myLookAndFeel);
    seqThree.setName("3");
    addAndMakeVisible(seqFour);
    seqFour.setRadioGroupId(RadioButtonIds::sequencerMode);
    //seqFour.setLookAndFeel(&myLookAndFeel);
    seqFour.setName("4");
   

    addAndMakeVisible(attackSlider);
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    attackSlider.setLookAndFeel(&myLookAndFeel);

    addAndMakeVisible(decaySlider);
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    decaySlider.setLookAndFeel(&myLookAndFeel);



    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    attackAttachment = std::make_unique<SliderAttachment>(p.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(p.apvts, "DECAY", decaySlider);
    sizeAttachment = std::make_unique<SliderAttachment>(p.apvts, "SIZE", sizeSlider);
    dryWetAttachment = std::make_unique<SliderAttachment>(p.apvts, "DRYWET", dryWetSlider);

    

  



}

AmbientChopperAudioProcessorEditor::~AmbientChopperAudioProcessorEditor()
{
}

//==============================================================================
void AmbientChopperAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //16, 106, 141, 33 bg  color
    //g.fillAll (juce::Colour(16, 106, 141));
    g.drawImage(background, juce::Rectangle<float>(0.f, 0.f, 960.f, 540.f));

    //g.setColour (juce::Colours::white);
    /*g.setFont(15.0f);
    g.drawFittedText ("2", seqOne.getBounds(), juce::Justification::centred, 1);
    g.drawFittedText("4", seqTwo.getBounds(), juce::Justification::centred, 1);
    g.drawFittedText("8", seqThree.getBounds(), juce::Justification::centred, 1);
    g.drawFittedText("16", seqFour.getBounds(), juce::Justification::centred, 1);
    //g.drawVerticalLine(480, 0, 540);*/
}

void AmbientChopperAudioProcessorEditor::resized()
{

    sizeSlider.setBounds(bigSliderPadding, 150, bigSliderWidth, bigSliderWidth);
    dryWetSlider.setBounds(22 + width / 2 + bigSliderPadding * 2, 150, bigSliderWidth, bigSliderWidth);

    seqOne.setBounds(getLocalBounds().getCentreX() - buttonWidth * 1.5, 163, buttonWidth, buttonHeight);
    seqTwo.setBounds(getLocalBounds().getCentreX() + buttonWidth * 0.5, 163, buttonWidth, buttonHeight);
    seqThree.setBounds(getLocalBounds().getCentreX() + buttonWidth * 0.5, 251, buttonWidth, buttonHeight);
    seqFour.setBounds(getLocalBounds().getCentreX() - buttonWidth * 1.5, 251, buttonWidth, buttonHeight);

    attackSlider.setBounds(42, 420, smallSliderWidth, smallSliderWidth);
    decaySlider.setBounds(smallSliderWidth + 90, 420, smallSliderWidth, smallSliderWidth);

    positionLabel.setBounds(getLocalBounds());


    

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}