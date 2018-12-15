#include "PluginProcessor.h"
#include "PluginEditor.h"

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

    //pbUp.bits = { true, false, true, false };
    //pbDown.bits = { false, true, false, true };
    //wheel.bits = { false, true, true, true };
    pbUp.byteValue = 0;
    pbDown.byteValue = 0;
    wheel.byteValue = 0;
}

ModMateAudioProcessor::~ModMateAudioProcessor()
{
}

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
    
    float CC1 = cc1;
    float CC2 = cc2;
    float CC4 = cc4;
    float CC67 = cc67;

    MidiMessage msg;
    int samplePos;
    for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(msg, samplePos);)
    {
        bool somethingChanged = false;
        if (msg.isPitchWheel() && (pbDown.byteValue || pbUp.byteValue))
        {
            int pwv = msg.getPitchWheelValue();
            if (pwv >= 8192)
            {
                pitchBendUp = (pwv - 8192) / 8191.0f;
                bool pbdWas0 = (pitchBendDown == 0.0f);
                pitchBendDown = 0.0f;
                if (!pbdWas0)
                {
                    if (pbDown.bits.cc1)
                    {
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 1, 0), samplePos);
                        CC1 = 0.0f;
                        somethingChanged = true;
                    }
                    if (pbDown.bits.cc2)
                    {
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 2, 0), samplePos);
                        CC2 = 0.0f;
                        somethingChanged = true;
                    }
                    if (pbDown.bits.cc4)
                    {
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 4, 0), samplePos);
                        CC4 = 0.0f;
                        somethingChanged = true;
                    }
                    if (pbDown.bits.cc67)
                    {
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 67, 0), samplePos);
                        CC67 = 0.0f;
                        somethingChanged = true;
                    }
                }
                if (pbUp.bits.cc1) CC1 = pitchBendUp;
                if (pbUp.bits.cc2) CC2 = pitchBendUp;
                if (pbUp.bits.cc4) CC4 = pitchBendUp;
                if (pbUp.bits.cc67) CC67 = pitchBendUp;
            }
            else if (pwv < 8192)
            {
                pitchBendDown = (8192 - pwv) / 8191.0f;
                pitchBendUp = 0.0f;
                if (pbDown.bits.cc1) CC1 = pitchBendDown;
                if (pbDown.bits.cc2) CC2 = pitchBendDown;
                if (pbDown.bits.cc4) CC4 = pitchBendDown;
                if (pbDown.bits.cc67) CC67 = pitchBendDown;
            }
            somethingChanged = true;
        }
        else if (msg.isControllerOfType(1) && wheel.byteValue)
        {
            modWheel = msg.getControllerValue() / 127.0f;
            if (wheel.bits.cc1) CC1 = modWheel;
            if (wheel.bits.cc2) CC2 = modWheel;
            if (wheel.bits.cc4) CC4 = modWheel;
            if (wheel.bits.cc67) CC67 = modWheel;
            somethingChanged = true;
        }
        else
        {
            // all other messages are passed through
            midiOut.addEvent(msg, samplePos);
        }
        
        if (CC1 != cc1)
        {
            cc1 = CC1;
            int cval = int(cc1 * 127 + 0.5f);
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 1, cval), samplePos);
            somethingChanged = true;
        }
        if (CC2 != cc2)
        {
            cc2 = CC2;
            int cval = int(cc2 * 127 + 0.5f);
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 2, cval), samplePos);
            somethingChanged = true;
        }
        if (CC4 != cc4)
        {
            cc4 = CC4;
            int cval = int(cc4 * 127 + 0.5f);
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 4, cval), samplePos);
            somethingChanged = true;
        }
        if (CC67 != cc67)
        {
            cc67 = CC67;
            int cval = int(cc67 * 127 + 0.5f);
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), 67, cval), samplePos);
            somethingChanged = true;
        }
        if (somethingChanged) sendChangeMessage();
    }

    midiMessages.swapWith(midiOut);
}

bool ModMateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ModMateAudioProcessor::createEditor()
{
    return new ModMateAudioProcessorEditor (*this);
}

void ModMateAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml = XmlElement("modBits");
    xml.setAttribute("pbUpBits", pbUp.byteValue);
    xml.setAttribute("pbDownBits", pbDown.byteValue);
    xml.setAttribute("wheelBits", wheel.byteValue);
    copyXmlToBinary(xml, destData);
}

void ModMateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xml = getXmlFromBinary(data, sizeInBytes);
    pbUp.byteValue = xml->getIntAttribute("pbUpBits", 0);
    pbDown.byteValue = xml->getIntAttribute("pbDownBits", 0);
    wheel.byteValue = xml->getIntAttribute("wheelBits", 0);
    sendChangeMessage();
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModMateAudioProcessor();
}
