/*
  ==============================================================================

    DeckGUI.h
    Created: 21 Feb 2022 5:59:18pm
    Author:  Hanna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
using namespace juce;

//==============================================================================
/*
*/
class DeckGUI  : public Component,
                 public Button::Listener, 
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager &  formatManagerToUse,
            AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint(Graphics&) override;
    void resized() override;

    /** implement Button::Listenser  */
    void buttonClicked (Button*) override;

    /** implement Slider::Listenser  */
    void sliderValueChanged (Slider* slider) override;

    /** implement FileDragAndDropTarget  */
    bool isInterestedInFileDrag(const StringArray& file) override;
    void filesDropped(const StringArray &file, int x, int y) override;

    /** implement Timer  */
    void timerCallback() override;

    /** load file: send file to DJAudioPlayer and WaveformDisplay */
    void loadFile(URL audioURL);

private:

    FileChooser chooser{ "Select a file..." };

    /** Buttons  */
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton restartButton{ "RESTART" };
    TextButton loadButton{ "LOAD A MUSIC FILE" };
    TextButton rewindButton{ "<<" };
    TextButton forwardButton{ ">>" };
    ToggleButton loopButton{ "LOOP" };

    /** Slider  */
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    /** Labels  */
    Label volLabel;
    Label speedLabel;
    Label posLabel;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
