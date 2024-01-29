/*
  ==============================================================================

    WaveformDisplay.h
    Created: 22 Feb 2022 6:12:46pm
    Author:  Hanna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead */
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    String trackName;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
