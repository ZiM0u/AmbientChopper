/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "paramsID.h"

//==============================================================================
AmbientChopperAudioProcessor::AmbientChopperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),apvts(*this,nullptr,"parameters",initParameters())
#endif
{
}

AmbientChopperAudioProcessor::~AmbientChopperAudioProcessor()
{
}

//==============================================================================
const juce::String AmbientChopperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmbientChopperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmbientChopperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AmbientChopperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AmbientChopperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmbientChopperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AmbientChopperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmbientChopperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AmbientChopperAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmbientChopperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AmbientChopperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    reverb.prepare(spec);
}

void AmbientChopperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmbientChopperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif
void AmbientChopperAudioProcessor::updateReverbSettings()
{
    reverbParams.roomSize = apvts.getParameter("SIZE")->getValue();
    reverbParams.damping = 0.5f;
    reverbParams.width = 0.3f;
    reverbParams.wetLevel = apvts.getParameter("DRYWET")->getValue();
    reverbParams.dryLevel = 1.0f - apvts.getParameter("DRYWET")->getValue();
    reverbParams.freezeMode = 0.f;

    reverb.setParameters(reverbParams);
}

void AmbientChopperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo info;
        playHead->getCurrentPosition(info);
        currentPosition.store(info.ppqPosition);

    }
    updateReverbSettings();
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);
    reverb.process(ctx);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool AmbientChopperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AmbientChopperAudioProcessor::createEditor()
{
    return new AmbientChopperAudioProcessorEditor (*this);
}

//==============================================================================
void AmbientChopperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AmbientChopperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbientChopperAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AmbientChopperAudioProcessor::initParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    juce::NormalisableRange<float> range{ juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f )};
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SIZE", "Size", range, 50.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET", "DryWet", range, 50.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", range, 50.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", range, 50.f));

    return { params.begin(),params.end() };
}