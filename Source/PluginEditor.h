#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "UnidirectionalSlider.h"

class ModMateAudioProcessorEditor   : public AudioProcessorEditor
                                    , public ChangeListener
{
public:
    ModMateAudioProcessorEditor (ModMateAudioProcessor&);
    ~ModMateAudioProcessorEditor();

    // Component
    void paint(Graphics&) override;
    void resized() override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;

private:
    ModMateAudioProcessor& processor;

    // Controls
    Label pbUpLabel, pbDownLabel, modWheelLabel;
    UnidirectionalSlider pbUpSlider, pbDownSlider, modWheelSlider;
    UnidirectionalSlider cc1Slider, cc2Slider, cc4Slider, cc67Slider;
    ToggleButton pbUp_cc1Btn, pbUp_cc2Btn, pbUp_cc4Btn, pbUp_cc67Btn;
    ToggleButton pbDn_cc1Btn, pbDn_cc2Btn, pbDn_cc4Btn, pbDn_cc67Btn;
    ToggleButton modW_cc1Btn, modW_cc2Btn, modW_cc4Btn, modW_cc67Btn;

    // Data model: see corresponding variables in PluginProcessor class
    float pitchBendUp, pitchBendDown, modWheel;
    float cc1, cc2, cc4, cc67;
    ControlBitmap pbUp, pbDown, wheel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModMateAudioProcessorEditor)
};
