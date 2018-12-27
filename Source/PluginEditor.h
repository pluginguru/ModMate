#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "UnidirectionalSlider.h"

class ModMateAudioProcessorEditor   : public AudioProcessorEditor
                                    , public ChangeListener
                                    , public Button::Listener
{
public:
    ModMateAudioProcessorEditor (ModMateAudioProcessor&);
    ~ModMateAudioProcessorEditor();

    // Component
    void paint(Graphics&) override;
    void resized() override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;
    
    // Button::Listener
    void buttonClicked(Button*) override;

protected:
    void onModWheelLabelTextChange();
    void onCC1LabelTextChange();
    void onCC2LabelTextChange();
    void onCC4LabelTextChange();
    void onCC67LabelTextChange();

private:
    ModMateAudioProcessor& processor;

    // Controls
    DrawableButton aboutButton;
    Label pbUpLabel, pbDownLabel, modWheelLabel;
    UnidirectionalSlider pbUpSlider, pbDownSlider, modWheelSlider;
    Label cc1Label, cc2Label, cc4Label, cc67Label;
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
