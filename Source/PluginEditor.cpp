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
    modWheelLabel.setColour(Label::textColourId, Colours::darkorange);
    modWheelLabel.setJustificationType(Justification::left);
    String lblText(processor.cc1In == 1 ? "modW" : "cc" + String(processor.cc1In));
    modWheelLabel.setText(lblText, NotificationType::dontSendNotification);
    modWheelLabel.setEditable(false, true, true);
    modWheelLabel.onTextChange = [this] { onModWheelLabelTextChange(); };

    addAndMakeVisible(pbUpSlider);
    addAndMakeVisible(pbDownSlider);
    addAndMakeVisible(modWheelSlider);

    addAndMakeVisible(cc1Label);
    cc1Label.setJustificationType(Justification::centredRight);
    cc1Label.setColour(Label::textColourId, Colours::darkorange);
    lblText = processor.cc1Out == 1 ? "modW" : "cc" + String(processor.cc1Out);
    cc1Label.setText(lblText, NotificationType::dontSendNotification);
    cc1Label.setEditable(false, true, true);
    cc1Label.onTextChange = [this] { onCC1LabelTextChange(); };

    addAndMakeVisible(cc2Label);
    cc2Label.setJustificationType(Justification::centredRight);
    cc2Label.setColour(Label::textColourId, Colours::hotpink);
    lblText = processor.cc2Out == 1 ? "modW" : "cc" + String(processor.cc2Out);
    cc2Label.setText(lblText, NotificationType::dontSendNotification);
    cc2Label.setEditable(false, true, true);
    cc2Label.onTextChange = [this] { onCC2LabelTextChange(); };

    addAndMakeVisible(cc4Label);
    cc4Label.setJustificationType(Justification::centredRight);
    cc4Label.setColour(Label::textColourId, Colours::cyan);
    lblText = processor.cc4Out == 1 ? "modW" : "cc" + String(processor.cc4Out);
    cc4Label.setText(lblText, NotificationType::dontSendNotification);
    cc4Label.setEditable(false, true, true);
    cc4Label.onTextChange = [this] { onCC4LabelTextChange(); };

    addAndMakeVisible(cc67Label);
    cc67Label.setJustificationType(Justification::centredRight);
    cc67Label.setColour(Label::textColourId, Colours::greenyellow);
    lblText = processor.cc67Out == 1 ? "modW" : "cc" + String(processor.cc67Out);
    cc67Label.setText(lblText, NotificationType::dontSendNotification);
    cc67Label.setEditable(false, true, true);
    cc67Label.onTextChange = [this] { onCC67LabelTextChange(); };

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

    pbUpSlider.addChangeListener(this);
    pbDownSlider.addChangeListener(this);
    modWheelSlider.addChangeListener(this);
    cc1Slider.addChangeListener(this);
    cc2Slider.addChangeListener(this);
    cc4Slider.addChangeListener(this);
    cc67Slider.addChangeListener(this);
    
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
    
    setSize (500, 300);
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
    pbUpLabel.setBounds(column.removeFromTop(20).withX(column.getX() - 8));
    pbUpSlider.setBounds(column.removeFromLeft(20));
    int colHeight = column.getHeight();
    pbUp_cc1Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbUp_cc2Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbUp_cc4Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbUp_cc67Btn.setBounds(column);

    column = bounds.removeFromLeft(50);
    pbDownLabel.setBounds(column.removeFromTop(20).withX(column.getX() - 8));
    pbDownSlider.setBounds(column.removeFromLeft(20));
    pbDn_cc1Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbDn_cc2Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbDn_cc4Btn.setBounds(column.removeFromTop(colHeight / 4));
    pbDn_cc67Btn.setBounds(column);

    column = bounds.removeFromLeft(50);
    modWheelLabel.setBounds(column.removeFromTop(20).withX(column.getX() - 8));
    modWheelSlider.setBounds(column.removeFromLeft(20));
    modW_cc1Btn.setBounds(column.removeFromTop(colHeight / 4));
    modW_cc2Btn.setBounds(column.removeFromTop(colHeight / 4));
    modW_cc4Btn.setBounds(column.removeFromTop(colHeight / 4));
    modW_cc67Btn.setBounds(column);

    column = bounds.removeFromLeft(50);
    column.removeFromTop(20);
    cc1Label.setBounds(column.removeFromTop(colHeight / 4));
    cc2Label.setBounds(column.removeFromTop(colHeight / 4));
    cc4Label.setBounds(column.removeFromTop(colHeight / 4));
    cc67Label.setBounds(column);

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

void ModMateAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
    if (sender == (ChangeBroadcaster*)&pbUpSlider)
    {
        pitchBendUp = pbUpSlider.getValue();
        processor.pbUpChange(pitchBendUp);
    }
    else if (sender == (ChangeBroadcaster*)&pbDownSlider)
    {
        pitchBendDown = pbDownSlider.getValue();
        processor.pbDownChange(pitchBendDown);
    }
    else if (sender == (ChangeBroadcaster*)&modWheelSlider)
    {
        modWheel = modWheelSlider.getValue();
        processor.modWheelChange(modWheel);
    }
    else if (sender == (ChangeBroadcaster*)&cc1Slider)
    {
        cc1 = cc1Slider.getValue();
        processor.cc1Change(cc1);
    }
    else if (sender == (ChangeBroadcaster*)&cc2Slider)
    {
        cc2 = cc2Slider.getValue();
        processor.cc2Change(cc2);
    }
    else if (sender == (ChangeBroadcaster*)&cc4Slider)
    {
        cc4 = cc4Slider.getValue();
        processor.cc4Change(cc4);
    }
    else if (sender == (ChangeBroadcaster*)&cc67Slider)
    {
        cc67 = cc67Slider.getValue();
        processor.cc67Change(cc67);
    }
    else if (sender == (ChangeBroadcaster*)&processor)
    {
        if (processor.presetLoaded)
        {
            processor.presetLoaded = false;
            String lblText(processor.cc1In == 1 ? "modW" : "cc" + String(processor.cc1In));
            modWheelLabel.setText(lblText, NotificationType::dontSendNotification);
            lblText = processor.cc1Out == 1 ? "modW" : "cc" + String(processor.cc1Out);
            cc1Label.setText(lblText, NotificationType::dontSendNotification);
            lblText = processor.cc2Out == 1 ? "modW" : "cc" + String(processor.cc2Out);
            cc2Label.setText(lblText, NotificationType::dontSendNotification);
            lblText = processor.cc4Out == 1 ? "modW" : "cc" + String(processor.cc4Out);
            cc4Label.setText(lblText, NotificationType::dontSendNotification);
            lblText = processor.cc67Out == 1 ? "modW" : "cc" + String(processor.cc67Out);
            cc67Label.setText(lblText, NotificationType::dontSendNotification);
        }
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
            cc2 = processor.cc2;
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
        if (processor.presetLoaded || pbUp.byteValue != processor.pbUp.byteValue)
        {
            processor.presetLoaded = false;
            pbUp.byteValue = processor.pbUp.byteValue;
            pbUp_cc1Btn.setToggleState(pbUp.bits.cc1, NotificationType::dontSendNotification);
            pbUp_cc2Btn.setToggleState(pbUp.bits.cc2, NotificationType::dontSendNotification);
            pbUp_cc4Btn.setToggleState(pbUp.bits.cc4, NotificationType::dontSendNotification);
            pbUp_cc67Btn.setToggleState(pbUp.bits.cc67, NotificationType::dontSendNotification);
        }
        if (processor.presetLoaded || pbDown.byteValue != processor.pbDown.byteValue)
        {
            processor.presetLoaded = false;
            pbDown.byteValue = processor.pbDown.byteValue;
            pbDn_cc1Btn.setToggleState(pbDown.bits.cc1, NotificationType::dontSendNotification);
            pbDn_cc2Btn.setToggleState(pbDown.bits.cc2, NotificationType::dontSendNotification);
            pbDn_cc4Btn.setToggleState(pbDown.bits.cc4, NotificationType::dontSendNotification);
            pbDn_cc67Btn.setToggleState(pbDown.bits.cc67, NotificationType::dontSendNotification);
        }
        if (processor.presetLoaded || wheel.byteValue != processor.wheel.byteValue)
        {
            processor.presetLoaded = false;
            wheel.byteValue = processor.wheel.byteValue;
            modW_cc1Btn.setToggleState(wheel.bits.cc1, NotificationType::dontSendNotification);
            modW_cc2Btn.setToggleState(wheel.bits.cc2, NotificationType::dontSendNotification);
            modW_cc4Btn.setToggleState(wheel.bits.cc4, NotificationType::dontSendNotification);
            modW_cc67Btn.setToggleState(wheel.bits.cc67, NotificationType::dontSendNotification);
        }
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

void ModMateAudioProcessorEditor::onModWheelLabelTextChange()
{
    String newValueStr = modWheelLabel.getText();
    int newCC = newValueStr.trimCharactersAtStart("cC").getIntValue();
    if (newValueStr.toUpperCase().startsWith("M")) newCC = 1;
    if (newCC > 0 && newCC < 128) processor.cc1In = newCC;
    else newCC = processor.cc1In;
    String lblText(newCC == 1 ? "modW" : "cc" + String(newCC));
    modWheelLabel.setText(lblText, NotificationType::dontSendNotification);
}

void ModMateAudioProcessorEditor::onCC1LabelTextChange()
{
    String newValueStr = cc1Label.getText();
    int newCC = newValueStr.trimCharactersAtStart("cC").getIntValue();
    if (newValueStr.toUpperCase().startsWith("M")) newCC = 1;
    if (newCC > 0 && newCC < 128) processor.cc1Out = newCC;
    else newCC = processor.cc1Out;
    String lblText(newCC == 1 ? "modW" : "cc" + String(newCC));
    cc1Label.setText(lblText, NotificationType::dontSendNotification);
}

void ModMateAudioProcessorEditor::onCC2LabelTextChange()
{
    String newValueStr = cc2Label.getText();
    int newCC = newValueStr.trimCharactersAtStart("cC").getIntValue();
    if (newValueStr.toUpperCase().startsWith("M")) newCC = 1;
    if (newCC > 0 && newCC < 128) processor.cc2Out = newCC;
    else newCC = processor.cc2Out;
    String lblText(newCC == 1 ? "modW" : "cc" + String(newCC));
    cc2Label.setText(lblText, NotificationType::dontSendNotification);
}

void ModMateAudioProcessorEditor::onCC4LabelTextChange()
{
    String newValueStr = cc4Label.getText();
    int newCC = newValueStr.trimCharactersAtStart("cC").getIntValue();
    if (newValueStr.toUpperCase().startsWith("M")) newCC = 1;
    if (newCC > 0 && newCC < 128) processor.cc4Out = newCC;
    else newCC = processor.cc4Out;
    String lblText(newCC == 1 ? "modW" : "cc" + String(newCC));
    cc4Label.setText(lblText, NotificationType::dontSendNotification);
}

void ModMateAudioProcessorEditor::onCC67LabelTextChange()
{
    String newValueStr = cc67Label.getText();
    int newCC = newValueStr.trimCharactersAtStart("cC").getIntValue();
    if (newValueStr.toUpperCase().startsWith("M")) newCC = 1;
    if (newCC > 0 && newCC < 128) processor.cc67Out = newCC;
    else newCC = processor.cc67Out;
    String lblText(newCC == 1 ? "modW" : "cc" + String(newCC));
    cc67Label.setText(lblText, NotificationType::dontSendNotification);
}
