/*
  ==============================================================================

    DeckGUI.cpp
    Created: 21 Feb 2022 5:59:18pm
    Author:  Hanna

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
                ) : player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse)
{
    /** Buttons: addAndMakeVisible  */
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(forwardButton);
    addAndMakeVisible(loopButton);

    /** Slider: addAndMakeVisible  */
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    /** Display Audio Wave form: addAndMakeVisible */
    addAndMakeVisible(waveformDisplay);

    /** Buttons: addListener  */
    playButton.addListener(this);
    stopButton.addListener(this);
    restartButton.addListener(this);
    loadButton.addListener(this);
    rewindButton.addListener(this);
    forwardButton.addListener(this);
    loopButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    /** volSlider: set the volume style */
    volSlider.setRange(0.0, 1.0);
    volSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 40, volSlider.getTextBoxHeight());
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setValue(0.5);
    volSlider.setSkewFactorFromMidPoint(0.5);
    /** volSlider: rotary slider */
    volSlider.setSliderStyle(Slider::Rotary);
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
    /** volLabel: create a label next to Volume slider */
    volLabel.setText("Volume", dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    

    /**  speedSlider: set the speed style */
    speedSlider.setRange(0.0, 10.0);
    speedSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 40, speedSlider.getTextBoxHeight());
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setValue(1);
    speedSlider.setSkewFactorFromMidPoint(1);
    /** speedSlider: rotary slider */
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
    /**  speedLabel: create a label next to Speed slider */
    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    

    /** posSlider: set the position style */
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 40, posSlider.getTextBoxHeight());
    posSlider.setNumDecimalPlacesToDisplay(2);
    /** posLabel: create a label next to Position slider */
    posLabel.setText("Position", dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);

    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::hotpink);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::cadetblue);
    g.setFont (14.0f);
    g.drawText ("DeckGUI   ", getLocalBounds(),
                juce::Justification::centredRight, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 9;
    double colW = getWidth() / 6;

    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    posSlider.setBounds(colW, rowH * 2, colW * 5, rowH);
    rewindButton.setBounds(0, rowH * 3, colW, rowH);
    playButton.setBounds(colW, rowH * 3, colW, rowH);
    stopButton.setBounds(colW * 2, rowH * 3, colW, rowH);
    forwardButton.setBounds(colW * 3, rowH * 3, colW, rowH);
    restartButton.setBounds(colW * 4, rowH * 3, colW, rowH);
    loopButton.setBounds(colW * 5, rowH * 3, colW, rowH);
    volSlider.setBounds(0, rowH * 5, getWidth() / 2, rowH * 3);
    speedSlider.setBounds(getWidth() / 2, rowH * 5, getWidth() / 2, rowH * 3);
    loadButton.setBounds(0, rowH * 8, getWidth(), rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    
    if (button == &playButton)
    {
        /** Play button was clicked */
        player->start();
    }
    if (button == &stopButton)
    {
        /** Stop button was clicked */
        player->stop();
    }
    if (button == &restartButton)
    {
        /** Restart button was clicked */
        player->restart();
    }
    if (button == &loadButton)
    {
        /** new style of code for loading a file, shared from peers */
        auto dlgFlags =
            FileBrowserComponent::openMode |
            FileBrowserComponent::canSelectFiles;

        this->chooser.launchAsync(dlgFlags,
            [this](const FileChooser& chooser)
            {
                player->loadURL(chooser.getURLResult());
                waveformDisplay.loadURL(chooser.getURLResult());
            }); 

        /** This was the old version from the lecture videos, no longer working for VScode 2022
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
        } */
    } 
    if (button == &rewindButton)
    {
        /** Rewind button was clicked */
        player->rewind();
    }
    if (button == &forwardButton)
    {
        /** Forward button was clicked */
        player->forward();
    }

    if (button == &loopButton)
    {
        if (loopButton.getToggleState() == true)
        {
            /** Loop button was ticked */
            player->setLoop();
        }
        else
        {
            /** Loop button unticked */
            player->unsetLoop();
        }
    }
}


void DeckGUI::sliderValueChanged(Slider* slider)
{

    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    } 
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& file)
{
    /** DeckGUI::isInterestedInFileDrag */
    return true; 
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    /** DeckGUI::filesDropped */
    if (files.size() == 1)
    {
        player->loadURL(URL{ File{files[0]}});
        waveformDisplay.loadURL(juce::URL{File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}