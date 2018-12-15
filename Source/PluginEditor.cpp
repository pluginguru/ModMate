#include "PluginProcessor.h"
#include "PluginEditor.h"

ModMateAudioProcessorEditor::ModMateAudioProcessorEditor (ModMateAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
    , pbUpSlider(Colours::lime)
    , pbDownSlider(Colours::red)
    , modWheelSlider(Colours::darkorange)
    , cc1Slider(Colours::darkorange)
    , cc2Slider(Colours::hotpink)
    , cc4Slider(Colours::cyan)
    , cc67Slider(Colours::greenyellow)
{
    pitchBendUp = processor.pitchBendUp;
    pitchBendDown = processor.pitchBendDown;
    modWheel = processor.modWheel;
    cc1 = processor.cc1;
    cc2 = processor.cc2;
    cc4 = processor.cc4;
    cc67 = processor.cc67;
    pbUp = processor.pbUp;
    pbDown = processor.pbDown;
    wheel = processor.wheel;

    addAndMakeVisible(pbUpLabel);
    pbUpLabel.setJustificationType(Justification::left);
    pbUpLabel.setText("pbUp", NotificationType::dontSendNotification);
    addAndMakeVisible(pbDownLabel);
    pbDownLabel.setJustificationType(Justification::left);
    pbDownLabel.setText("pbDn", NotificationType::dontSendNotification);
    addAndMakeVisible(modWheelLabel);
    modWheelLabel.setJustificationType(Justification::left);
    modWheelLabel.setText("modW", NotificationType::dontSendNotification);

    addAndMakeVisible(pbUpSlider);
    addAndMakeVisible(pbDownSlider);
    addAndMakeVisible(modWheelSlider);

    addAndMakeVisible(cc1Slider);
    addAndMakeVisible(cc2Slider);
    addAndMakeVisible(cc4Slider);
    addAndMakeVisible(cc67Slider);

    addAndMakeVisible(pbUp_cc1Btn);
    addAndMakeVisible(pbUp_cc2Btn);
    addAndMakeVisible(pbUp_cc4Btn);
    addAndMakeVisible(pbUp_cc67Btn);

    addAndMakeVisible(pbDn_cc1Btn);
    addAndMakeVisible(pbDn_cc2Btn);
    addAndMakeVisible(pbDn_cc4Btn);
    addAndMakeVisible(pbDn_cc67Btn);

    addAndMakeVisible(modW_cc1Btn);
    addAndMakeVisible(modW_cc2Btn);
    addAndMakeVisible(modW_cc4Btn);
    addAndMakeVisible(modW_cc67Btn);

    processor.addChangeListener(this);

    setSize (400, 300);
}

ModMateAudioProcessorEditor::~ModMateAudioProcessorEditor()
{
    processor.removeChangeListener(this);
}

void ModMateAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ModMateAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);

    auto column = bounds.removeFromLeft(50);
    pbUpLabel.setBounds(column.removeFromTop(20));
    pbUpSlider.setBounds(column.removeFromLeft(20));
    int colHeight = column.getHeight();
    pbUp_cc1Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbUp_cc2Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbUp_cc4Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbUp_cc67Btn.setBounds(column);

    column = bounds.removeFromLeft(50);
    pbDownLabel.setBounds(column.removeFromTop(20));
    pbDownSlider.setBounds(column.removeFromLeft(20));
    pbDn_cc1Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbDn_cc2Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbDn_cc4Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbDn_cc67Btn.setBounds(column);

    column = bounds.removeFromLeft(50);
    modWheelLabel.setBounds(column.removeFromTop(20));
    modWheelSlider.setBounds(column.removeFromLeft(20));
    modW_cc1Btn.setBounds(column.removeFromTop(colHeight / 4));
    modW_cc2Btn.setBounds(column.removeFromTop(colHeight / 4));
    modW_cc4Btn.setBounds(column.removeFromTop(colHeight / 4));
    modW_cc67Btn.setBounds(column);

    bounds.removeFromTop(20);
    auto row = bounds.removeFromTop(colHeight / 4); row.reduce(0, 8);
    cc1Slider.setBounds(row);
    row = bounds.removeFromTop(colHeight / 4); row.reduce(0, 8);
    cc2Slider.setBounds(row);
    row = bounds.removeFromTop(colHeight / 4); row.reduce(0, 8);
    cc4Slider.setBounds(row);
    row = bounds; row.reduce(0, 8);
    cc67Slider.setBounds(row);
}

void ModMateAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster*)
{
    if (pitchBendUp != processor.pitchBendUp)
    {

        pitchBendUp = processor.pitchBendUp;
    }
    if (pitchBendDown != processor.pitchBendDown)
    {

        pitchBendDown = processor.pitchBendDown;
    }
    if (modWheel != processor.modWheel)
    {

        modWheel = processor.modWheel;
    }
    if (cc1 != processor.cc1)
    {

        cc1 = processor.cc1;
    }
    if (cc2 != processor.cc2)
    {

        cc2= processor.cc2;
    }
    if (cc4 != processor.cc4)
    {

        cc4 = processor.cc4;
    }
    if (cc67 != processor.cc67)
    {

        cc67 = processor.cc67;
    }
    if (pbUp.byteValue != processor.pbUp.byteValue)
    {

        pbUp.byteValue = processor.pbUp.byteValue;
    }
    if (pbDown.byteValue != processor.pbDown.byteValue)
    {

        pbDown.byteValue = processor.pbDown.byteValue;
    }
    if (wheel.byteValue != processor.wheel.byteValue)
    {

        wheel.byteValue = processor.wheel.byteValue;
    }
}
