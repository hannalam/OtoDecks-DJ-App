/*
  ==============================================================================

    TrackData.h
    Created: 8 Mar 2022 12:14:51am
    Author:  Hanna

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

class TrackData
{
public:
    TrackData(juce::File _file);
    File file;
    URL URL;
    String fileName;
    String fileType;
    String length;
    String fileLocation;

    bool operator==(const juce::String& other) const;
};