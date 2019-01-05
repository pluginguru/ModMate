#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "UnidirectionalSlider.h"

class CCLabel : public Label
{
public:
    void mouseDown(const MouseEvent&) override;
};

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
    void onWheel2LabelTextChange();
    void onWheel4LabelTextChange();
    void onWheel67LabelTextChange();
    void onCC1LabelTextChange();
    void onCC2LabelTextChange();
    void onCC4LabelTextChange();
    void onCC67LabelTextChange();

private:
    ModMateAudioProcessor& processor;

    // Controls
    DrawableButton aboutButton;
    Label pbUpLabel, pbDownLabel;
    CCLabel modWheelLabel, wheel2Label, wheel4Label, wheel67Label;
    UnidirectionalSlider pbUpSlider, pbDownSlider, modWheelSlider, wheel2Slider, wheel4Slider, wheel67Slider;
    CCLabel cc1Label, cc2Label, cc4Label, cc67Label;
    UnidirectionalSlider cc1Slider, cc2Slider, cc4Slider, cc67Slider;
    ToggleButton pbUp_cc1Btn, pbUp_cc2Btn, pbUp_cc4Btn, pbUp_cc67Btn;
    ToggleButton pbDn_cc1Btn, pbDn_cc2Btn, pbDn_cc4Btn, pbDn_cc67Btn;
    ToggleButton modW_cc1Btn, modW_cc2Btn, modW_cc4Btn, modW_cc67Btn;
    ToggleButton wh2_cc1Btn, wh2_cc2Btn, wh2_cc4Btn, wh2_cc67Btn;
    ToggleButton wh4_cc1Btn, wh4_cc2Btn, wh4_cc4Btn, wh4_cc67Btn;
    ToggleButton wh67_cc1Btn, wh67_cc2Btn, wh67_cc4Btn, wh67_cc67Btn;

    // Data model: see corresponding variables in PluginProcessor class
    float pitchBendUp, pitchBendDown, modWheel, wheel2, wheel4, wheel67;
    float cc1, cc2, cc4, cc67;
    ControlBitmap pbUp, pbDown, wheel, ctrl2, ctrl4, ctrl67;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModMateAudioProcessorEditor)
};
