/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ModMateAudioProcessor::ModMateAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    pitchBendUp = pitchBendDown = modWheel = 0.0f;
    cc1 = cc2 = cc4 = cc67 = 0.0f;

    pbUpBits = { true, false, true, false };
    pbDownBits = { false, true, false, true };
    wheelBits = { false, true, true, true };
}

ModMateAudioProcessor::~ModMateAudioProcessor()
{
}

//==============================================================================
const String ModMateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ModMateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ModMateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ModMateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ModMateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ModMateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ModMateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ModMateAudioProcessor::setCurrentProgram (int index)
{
}

const String ModMateAudioProcessor::getProgramName (int index)
{
    return {};
}

void ModMateAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ModMateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ModMateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ModMateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

static MidiBuffer midiOut;

void ModMateAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // the audio buffer in a midi effect will have zero channels!
    jassert(buffer.getNumChannels() == 0);

    midiOut.clear();

    MidiMessage msg;
    int samplePos;
    for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(msg, samplePos);)
    {
        if (msg.isPitchWheel())
        {
            int pwv = msg.getPitchWheelValue();
            if (pwv >= 8192)
            {
                pitchBendUp = (pwv - 8192) / 8191.0f;
                int cval = int(pitchBendUp * 127 + 0.5f);
                if (pbUpBits.cc1)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 1, cval), samplePos);
                if (pbUpBits.cc2)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 2, cval), samplePos);
                if (pbUpBits.cc4)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 4, cval), samplePos);
                if (pbUpBits.cc67)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 67, cval), samplePos);
            }
            else
            {
                pitchBendDown = (8192 - pwv) / 8192.0f;
                int cval = int(pitchBendDown * 127 + 0.5f);
                if (pbDownBits.cc1)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 1, cval), samplePos);
                if (pbDownBits.cc2)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 2, cval), samplePos);
                if (pbDownBits.cc4)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 4, cval), samplePos);
                if (pbDownBits.cc67)
                    midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 67, cval), samplePos);
            }
        }
        else if (msg.isControllerOfType(1))
        {
            modWheel = msg.getControllerValue() / 127.0f;

            int cval = msg.getControllerValue();
            if (wheelBits.cc1)
                midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 1, cval), samplePos);
            if (wheelBits.cc2)
                midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 2, cval), samplePos);
            if (wheelBits.cc4)
                midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 4, cval), samplePos);
            if (wheelBits.cc67)
                midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 67, cval), samplePos);
        }
        else
        {
            // all other messages are passed through
            midiOut.addEvent(msg, samplePos);
        }
    }

    midiMessages.swapWith(midiOut);
}

//==============================================================================
bool ModMateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ModMateAudioProcessor::createEditor()
{
    return new ModMateAudioProcessorEditor (*this);
}

//==============================================================================
void ModMateAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    destData.setSize(1);
}

void ModMateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModMateAudioProcessor();
}
