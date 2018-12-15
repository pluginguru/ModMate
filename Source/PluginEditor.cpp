#include "PluginProcessor.h"
#include "PluginEditor.h"

ModMateAudioProcessorEditor::ModMateAudioProcessorEditor(ModMateAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
    , aboutButton(String("aboutBtn"), DrawableButton::ButtonStyle::ImageFitted)
    , pbUpSlider(Colours::lime)
    , pbDownSlider(Colours::red)
    , modWheelSlider(Colours::darkorange)
    , cc1Slider(Colours::darkorange)
    , cc2Slider(Colours::hotpink)
    , cc4Slider(Colours::cyan)
    , cc67Slider(Colours::greenyellow)
{
    std::unique_ptr<Drawable> icon(Drawable::createFromImageData(BinaryData::help_svg, BinaryData::help_svgSize));
    icon->replaceColour(Colours::black, Colours::aliceblue);
    aboutButton.setImages(icon.get());
    addAndMakeVisible(aboutButton);
    aboutButton.onClick = [this] {
        URL url("https://github.com/getdunne/modmate");
        url.launchInDefaultBrowser();
    };

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
    
    pitchBendUp = pitchBendDown = modWheel = -1.0f;
    cc1 = cc2 = cc4 = cc67 = -1.0f;
    pbUp.byteValue = pbDown.byteValue = wheel.byteValue = -1;
    changeListenerCallback(nullptr);

    processor.addChangeListener(this);
    
    pbUp_cc1Btn.addListener(this);
    pbUp_cc2Btn.addListener(this);
    pbUp_cc4Btn.addListener(this);
    pbUp_cc67Btn.addListener(this);

    pbDn_cc1Btn.addListener(this);
    pbDn_cc2Btn.addListener(this);
    pbDn_cc4Btn.addListener(this);
    pbDn_cc67Btn.addListener(this);
    
    modW_cc1Btn.addListener(this);
    modW_cc2Btn.addListener(this);
    modW_cc4Btn.addListener(this);
    modW_cc67Btn.addListener(this);
    
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
    auto aboutBox = getLocalBounds().reduced(6).removeFromTop(24).removeFromRight(24);
    aboutButton.setBounds(aboutBox);

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
        pbUpSlider.setValue(pitchBendUp);
    }
    if (pitchBendDown != processor.pitchBendDown)
    {
        pitchBendDown = processor.pitchBendDown;
        pbDownSlider.setValue(pitchBendDown);
    }
    if (modWheel != processor.modWheel)
    {
        modWheel = processor.modWheel;
        modWheelSlider.setValue(modWheel);
    }
    if (cc1 != processor.cc1)
    {
        cc1 = processor.cc1;
        cc1Slider.setValue(cc1);
    }
    if (cc2 != processor.cc2)
    {
        cc2= processor.cc2;
        cc2Slider.setValue(cc2);
    }
    if (cc4 != processor.cc4)
    {
        cc4 = processor.cc4;
        cc4Slider.setValue(cc4);
    }
    if (cc67 != processor.cc67)
    {
        cc67 = processor.cc67;
        cc67Slider.setValue(cc67);
    }
    if (pbUp.byteValue != processor.pbUp.byteValue)
    {
        pbUp.byteValue = processor.pbUp.byteValue;
        pbUp_cc1Btn.setToggleState(pbUp.bits.cc1, NotificationType::dontSendNotification);
        pbUp_cc2Btn.setToggleState(pbUp.bits.cc2, NotificationType::dontSendNotification);
        pbUp_cc4Btn.setToggleState(pbUp.bits.cc4, NotificationType::dontSendNotification);
        pbUp_cc67Btn.setToggleState(pbUp.bits.cc67, NotificationType::dontSendNotification);
    }
    if (pbDown.byteValue != processor.pbDown.byteValue)
    {
        pbDown.byteValue = processor.pbDown.byteValue;
        pbDn_cc1Btn.setToggleState(pbDown.bits.cc1, NotificationType::dontSendNotification);
        pbDn_cc2Btn.setToggleState(pbDown.bits.cc2, NotificationType::dontSendNotification);
        pbDn_cc4Btn.setToggleState(pbDown.bits.cc4, NotificationType::dontSendNotification);
        pbDn_cc67Btn.setToggleState(pbDown.bits.cc67, NotificationType::dontSendNotification);
    }
    if (wheel.byteValue != processor.wheel.byteValue)
    {
        wheel.byteValue = processor.wheel.byteValue;
        modW_cc1Btn.setToggleState(wheel.bits.cc1, NotificationType::dontSendNotification);
        modW_cc2Btn.setToggleState(wheel.bits.cc2, NotificationType::dontSendNotification);
        modW_cc4Btn.setToggleState(wheel.bits.cc4, NotificationType::dontSendNotification);
        modW_cc67Btn.setToggleState(wheel.bits.cc67, NotificationType::dontSendNotification);
    }
}

void ModMateAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &pbUp_cc1Btn) pbUp.bits.cc1 = processor.pbUp.bits.cc1 = button->getToggleState();
    if (button == &pbUp_cc2Btn) pbUp.bits.cc2 = processor.pbUp.bits.cc2 = button->getToggleState();
    if (button == &pbUp_cc4Btn) pbUp.bits.cc4 = processor.pbUp.bits.cc4 = button->getToggleState();
    if (button == &pbUp_cc67Btn) pbUp.bits.cc67 = processor.pbUp.bits.cc67 = button->getToggleState();

    if (button == &pbDn_cc1Btn) pbDown.bits.cc1 = processor.pbDown.bits.cc1 = button->getToggleState();
    if (button == &pbDn_cc2Btn) pbDown.bits.cc2 = processor.pbDown.bits.cc2 = button->getToggleState();
    if (button == &pbDn_cc4Btn) pbDown.bits.cc4 = processor.pbDown.bits.cc4 = button->getToggleState();
    if (button == &pbDn_cc67Btn) pbDown.bits.cc67 = processor.pbDown.bits.cc67 = button->getToggleState();

    if (button == &modW_cc1Btn) wheel.bits.cc1 = processor.wheel.bits.cc1 = button->getToggleState();
    if (button == &modW_cc2Btn) wheel.bits.cc2 = processor.wheel.bits.cc2 = button->getToggleState();
    if (button == &modW_cc4Btn) wheel.bits.cc4 = processor.wheel.bits.cc4 = button->getToggleState();
    if (button == &modW_cc67Btn) wheel.bits.cc67 = processor.wheel.bits.cc67 = button->getToggleState();
}
