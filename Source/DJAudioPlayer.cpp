/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 21 Feb 2022 4:22:07pm
    Author:  Hanna

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
               :formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 10.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 10" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}
void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}
void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}
//restart the music 
void DJAudioPlayer::restart()
{
    /** When restart buttom was clicked, reset to the starting point*/
    transportSource.setPosition(0);
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}

//Moving the position backwards of the transportSource
void DJAudioPlayer::rewind()
{
    //Check if the new position value is more than 0
    if (transportSource.getCurrentPosition() - 1 > 0)
    {
        //set the new position by reducing 1.8 position from the current position 
        transportSource.setPosition(transportSource.getCurrentPosition() - 1.8);
    }
}
//Moving the playback forward
void DJAudioPlayer::forward()
{
    //the last position of transportSource
    double lastPos{ transportSource.getLengthInSeconds() };

    //check if the new position is not equal or greater than the last position 
    if (transportSource.getCurrentPosition() + 0.5 != lastPos || transportSource.getCurrentPosition() + 0.5 > lastPos)
    {
        //update the new position by adding 1.5 position
        transportSource.setPosition(transportSource.getCurrentPosition() + 1.5);
    }
}
void DJAudioPlayer::setLoop()
{
    if (readerSource != nullptr)
    {
        readerSource->setLooping(true);
    }
}
void DJAudioPlayer::unsetLoop()
{
    if (readerSource != nullptr)
    {
        readerSource->setLooping(false);
    }
}
bool DJAudioPlayer::isLooping() const
{
    return false;
}

