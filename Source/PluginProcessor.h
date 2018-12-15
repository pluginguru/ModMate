#pragma once
#include "JuceHeader.h"

struct ControlBitmap
{
    union {
        struct {
            bool cc1 : 1;
            bool cc2 : 1;
            bool cc4 : 1;
            bool cc67 : 1;
            int padding : 4;
        } bits;
        int byteValue : 8;
    };
};

class ModMateAudioProcessor : public AudioProcessor
                            , public ChangeBroadcaster
{
public:
    ModMateAudioProcessor();
    ~ModMateAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // MIDI inputs
    float pitchBendUp, pitchBendDown, modWheel;

    // MIDI outputs
    float cc1, cc2, cc4, cc67;

    // control matrix
    ControlBitmap pbUp, pbDown, wheel;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModMateAudioProcessor)
};
