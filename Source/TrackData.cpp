/*
  ==============================================================================

    TrackData.cpp
    Created: 8 Mar 2022 12:14:51am
    Author:  Hanna

  ==============================================================================
*/

#include "TrackData.h"
#include <filesystem>

TrackData::TrackData(File _file) : file(_file),
fileName(_file.getFileNameWithoutExtension()),
fileType(_file.getFileExtension()),
fileLocation(_file.getFullPathName()),
URL(juce::URL{ _file })
{
    DBG("Created new file with the name: " << fileName);
}

bool TrackData::operator==(const String& other) const
{
    return fileName == other;
}
