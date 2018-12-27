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
    pbUpChanged = pbDownChanged = modWheelChanged = false;
    cc1Changed = cc2Changed = cc4Changed = cc67Changed = false;
    presetLoaded = false;

    cc1In = 1;
    cc1Out = 1;
    cc2Out = 2;
    cc4Out = 4;
    cc67Out = 67;

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

void ModMateAudioProcessor::pbUpChange(float ccVal)
{
    if (ccVal != pitchBendUp)
    {
        pitchBendUp = ccVal;
        pbUpChanged = true;
    }
}

void ModMateAudioProcessor::pbDownChange(float ccVal)
{
    if (ccVal != pitchBendDown)
    {
        pitchBendDown = ccVal;
        pbDownChanged = true;
    }
}

void ModMateAudioProcessor::modWheelChange(float ccVal)
{
    if (ccVal != modWheel)
    {
        modWheel = ccVal;
        modWheelChanged = true;
    }
}

void ModMateAudioProcessor::cc1Change(float ccVal)
{
    if (ccVal != cc1)
    {
        cc1 = ccVal;
        cc1Changed = true;
    }
}

void ModMateAudioProcessor::cc2Change(float ccVal)
{
    if (ccVal != cc2)
    {
        cc2 = ccVal;
        cc2Changed = true;
    }
}

void ModMateAudioProcessor::cc4Change(float ccVal)
{
    if (ccVal != cc4)
    {
        cc4 = ccVal;
        cc4Changed = true;
    }
}

void ModMateAudioProcessor::cc67Change(float ccVal)
{
    if (ccVal != cc67)
    {
        cc67 = ccVal;
        cc67Changed = true;
    }
}

void ModMateAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // the audio buffer in a midi effect will have zero channels!
    jassert(buffer.getNumChannels() == 0);

    MidiBuffer midiOut;
    
    float CC1 = cc1;
    float CC2 = cc2;
    float CC4 = cc4;
    float CC67 = cc67;

    if (pbUpChanged)
    {
        pbUpChanged = false;
        if (pbUp.byteValue != 0)
        {
            int cval = int(pitchBendUp * 127 + 0.5f);
            if (pbUp.bits.cc1) midiOut.addEvent(MidiMessage::controllerEvent(1, cc1Out, cval), 0);
            if (pbUp.bits.cc2) midiOut.addEvent(MidiMessage::controllerEvent(1, cc2Out, cval), 0);
            if (pbUp.bits.cc4) midiOut.addEvent(MidiMessage::controllerEvent(1, cc4Out, cval), 0);
            if (pbUp.bits.cc67) midiOut.addEvent(MidiMessage::controllerEvent(1, cc67Out, cval), 0);
        }
        else midiOut.addEvent(MidiMessage::pitchWheel(1, 8192 + int(pitchBendUp * 8191)), 0);
    }
    if (pbDownChanged)
    {
        pbDownChanged = false;
        if (pbDown.byteValue != 0)
        {
            int cval = int(pitchBendDown * 127 + 0.5f);
            if (pbDown.bits.cc1) midiOut.addEvent(MidiMessage::controllerEvent(1, cc1Out, cval), 0);
            if (pbDown.bits.cc2) midiOut.addEvent(MidiMessage::controllerEvent(1, cc2Out, cval), 0);
            if (pbDown.bits.cc4) midiOut.addEvent(MidiMessage::controllerEvent(1, cc4Out, cval), 0);
            if (pbDown.bits.cc67) midiOut.addEvent(MidiMessage::controllerEvent(1, cc67Out, cval), 0);
        }
        else midiOut.addEvent(MidiMessage::pitchWheel(1, 8192 - int(pitchBendDown * 8192)), 0);
    }
    if (modWheelChanged)
    {
        modWheelChanged = false;
        int cval = int(modWheel * 127 + 0.5f);
        if (wheel.byteValue != 0)
        {
            if (wheel.bits.cc1) midiOut.addEvent(MidiMessage::controllerEvent(1, cc1Out, cval), 0);
            if (wheel.bits.cc2) midiOut.addEvent(MidiMessage::controllerEvent(1, cc2Out, cval), 0);
            if (wheel.bits.cc4) midiOut.addEvent(MidiMessage::controllerEvent(1, cc4Out, cval), 0);
            if (wheel.bits.cc67) midiOut.addEvent(MidiMessage::controllerEvent(1, cc67Out, cval), 0);
        }
        else midiOut.addEvent(MidiMessage::controllerEvent(1, cc1In, cval), 0);
    }

    if (cc1Changed)
    {
        cc1Changed = false;
        midiOut.addEvent(MidiMessage::controllerEvent(1, cc1Out, int(cc1 * 127)), 0);
    }
    if (cc2Changed)
    {
        cc2Changed = false;
        midiOut.addEvent(MidiMessage::controllerEvent(1, cc2Out, int(cc2 * 127)), 0);
    }
    if (cc4Changed)
    {
        cc4Changed = false;
        midiOut.addEvent(MidiMessage::controllerEvent(1, cc4Out, int(cc4 * 127)), 0);
    }
    if (cc67Changed)
    {
        cc67Changed = false;
        midiOut.addEvent(MidiMessage::controllerEvent(1, cc67Out, int(cc67 * 127)), 0);
    }

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
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc1Out, 0), samplePos);
                        CC1 = 0.0f;
                        somethingChanged = true;
                    }
                    if (pbDown.bits.cc2)
                    {
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc2Out, 0), samplePos);
                        CC2 = 0.0f;
                        somethingChanged = true;
                    }
                    if (pbDown.bits.cc4)
                    {
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc4Out, 0), samplePos);
                        CC4 = 0.0f;
                        somethingChanged = true;
                    }
                    if (pbDown.bits.cc67)
                    {
                        midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc67Out, 0), samplePos);
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
        else if (msg.isPitchWheel())
        {
            // pitchWheel used but not allocated to any output: ensure GUI updates
            int pwv = msg.getPitchWheelValue();
            if (pwv >= 8192)
            {
                pitchBendUp = (pwv - 8192) / 8191.0f;
                pitchBendDown = 0.0f;
            }
            else
            {
                pitchBendUp = 0.0f;
                pitchBendDown = (8192 - pwv) / 8191.0f;
            }
            somethingChanged = true;
            midiOut.addEvent(msg, samplePos);
        }
        else if (msg.isControllerOfType(cc1In))
        {
            modWheel = msg.getControllerValue() / 127.0f;
            somethingChanged = true;
            if (wheel.byteValue)
            {
                if (wheel.bits.cc1) CC1 = modWheel;
                if (wheel.bits.cc2) CC2 = modWheel;
                if (wheel.bits.cc4) CC4 = modWheel;
                if (wheel.bits.cc67) CC67 = modWheel;
            }
            else midiOut.addEvent(msg, samplePos);
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
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc1Out, cval), samplePos);
            somethingChanged = true;
        }
        if (CC2 != cc2)
        {
            cc2 = CC2;
            int cval = int(cc2 * 127 + 0.5f);
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc2Out, cval), samplePos);
            somethingChanged = true;
        }
        if (CC4 != cc4)
        {
            cc4 = CC4;
            int cval = int(cc4 * 127 + 0.5f);
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc4Out, cval), samplePos);
            somethingChanged = true;
        }
        if (CC67 != cc67)
        {
            cc67 = CC67;
            int cval = int(cc67 * 127 + 0.5f);
            midiOut.addEvent(MidiMessage::controllerEvent(msg.getChannel(), cc67Out, cval), samplePos);
            somethingChanged = true;
        }
        if (somethingChanged) sendChangeMessage();
    }

    midiMessages.swapWith(midiOut);
}

bool ModMateAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* ModMateAudioProcessor::createEditor()
{
    ModMateAudioProcessorEditor* editor = new ModMateAudioProcessorEditor (*this);
    //presetLoaded = true;
    sendChangeMessage();
    return editor;
}

void ModMateAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml = XmlElement("modBits");
    xml.setAttribute("pbUpBits", pbUp.byteValue);
    xml.setAttribute("pbDownBits", pbDown.byteValue);
    xml.setAttribute("wheelBits", wheel.byteValue);
    xml.setAttribute("cc1In", cc1In);
    xml.setAttribute("cc1Out", cc1Out);
    xml.setAttribute("cc2Out", cc2Out);
    xml.setAttribute("cc3Out", cc4Out);
    xml.setAttribute("cc4Out", cc67Out);
    copyXmlToBinary(xml, destData);
}

void ModMateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xml = getXmlFromBinary(data, sizeInBytes);
    pbUp.byteValue = xml->getIntAttribute("pbUpBits", 0);
    pbDown.byteValue = xml->getIntAttribute("pbDownBits", 0);
    wheel.byteValue = xml->getIntAttribute("wheelBits", 0);
    cc1In = xml->getIntAttribute("cc1In", 1);
    cc1Out = xml->getIntAttribute("cc1Out", 1);
    cc2Out = xml->getIntAttribute("cc2Out", 2);
    cc4Out = xml->getIntAttribute("cc3Out", 4);
    cc67Out = xml->getIntAttribute("cc4Out", 67);
    presetLoaded = true;
    sendChangeMessage();
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModMateAudioProcessor();
}
