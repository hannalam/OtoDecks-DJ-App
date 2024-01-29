/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 21 Feb 2022 4:22:07pm
    Author:  Hanna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

class DJAudioPlayer : public AudioSource

{
public:
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();
    void restart();

    /** functions for buttoms in DeckGUI*/
    void rewind();
    void forward();
    void setLoop();
    void unsetLoop();
    bool isLooping() const;

    /** get the relative position of the playhead */
    double getPositionRelative();
    double getLengthInSeconds();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
