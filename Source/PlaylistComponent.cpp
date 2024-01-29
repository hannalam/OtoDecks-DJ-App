/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 23 Feb 2022 11:08:05am
    Author:  Hanna

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <fstream>
#include "DeckGUI.h"
//#include <algorithm>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _djAudioPlayer
                                    ) :deckGUI1(_deckGUI1),
                                       deckGUI2(_deckGUI2),
                                       djAudioPlayer(_djAudioPlayer)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    /** Playlist Header */
    playlist.getHeader().addColumn("Song Name", 1, 200);
    playlist.getHeader().addColumn("Type", 2, 60);
    playlist.getHeader().addColumn("Length", 3, 60);
    playlist.getHeader().addColumn("File Location", 4, 350);
    playlist.getHeader().addColumn("Action", 5, 100);
    playlist.setModel(this);

    /** Buttons */
    addAndMakeVisible(playlist);
    addAndMakeVisible(loadMusic);
    addAndMakeVisible(searchBar);
    addAndMakeVisible(addToDeckGUI1);
    addAndMakeVisible(addToDeckGUI2);

    /** Buttons: Listener */
    loadMusic.addListener(this);
    addToDeckGUI1.addListener(this);
    addToDeckGUI2.addListener(this);
    searchBar.addListener(this);

    /** Text in the box to remind people to search a song */
    searchBar.setTextToShowWhenEmpty("Type and search the music", juce::Colours::cadetblue);
    /** change the color when the search input is much with the keyword */
    searchBar.onTextChange = [this] { searchInPlaylist(searchBar.getText()); };

    loadPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
    saveInPlaylist();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::deeppink);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::hotpink);
    g.setFont (18.0f);
    g.drawText ("Playlist", getLocalBounds(),
                juce::Justification::centredTop, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 10;
    double colW = getWidth() / 4;
    searchBar.setBounds(0, rowH, getWidth() / 2, rowH);
    addToDeckGUI1.setBounds(colW*2, rowH, colW, rowH);
    addToDeckGUI2.setBounds(colW*3 , rowH, colW, rowH);
    playlist.setBounds(0, rowH * 2, getWidth(), rowH * 8);
    loadMusic.setBounds(0, rowH * 8, getWidth(), rowH * 2);
}

int PlaylistComponent::getNumRows()
{
    //return the number of the rows from TrackData.h
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::hotpink);
    }
    else
    {
        g.fillAll(Colours::cadetblue);
    }
}

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        //the first column in the track data is the file name
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].fileName,
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true
            );
        }
        //the second column in the track data is the file type
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].fileType,
                2,
                0,
                width - 4,
                height,
                Justification::centred,
                true
            );
        }
        //the third column in the track data is the length of the music
        if (columnId == 3)
        {
            g.drawText(tracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                Justification::centred,
                true
            );
        }
        //the fourth column in the track data is the file location in the computer directory
        if (columnId == 4)
        {
            g.drawText(tracks[rowNumber].fileLocation,
                2,
                0,
                width - 4,
                height,
                Justification::centred,
                true
            );
        }
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                int columnId,
                bool isRowSelected,
                Component* existingComponentToUpdate)
{
    //the fifth column in the track data is the delete button
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"Delete"};
            
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}
//Function to import tracks to playlist
void PlaylistComponent::importToPlaylist()
{
    DBG("PlaylistComponent::importToPlaylist called");

    //initialize file chooser
    auto dlgFlags =
        FileBrowserComponent::openMode |
        FileBrowserComponent::canSelectFiles;

    this->chooser.launchAsync(dlgFlags,
        [this](const FileChooser& chooser)
        {
            for (const File& file : chooser.getResults())
            {
                String fileName{ file.getFileNameWithoutExtension() };
                if (!isInPlaylist(fileName)) // if there is not repeated file 
                {
                    //load file and get the file meta data to the playlist
                    TrackData importedFile{ file };
                    URL audioURL{ file };
                    importedFile.length = getLength(audioURL);
                    tracks.push_back(importedFile);
                }
                playlist.updateContent();
            }
        });
}

String PlaylistComponent::getLength(URL audioURL)
{
    djAudioPlayer->loadURL(audioURL);
    double seconds{ djAudioPlayer->getLengthInSeconds() };
    String minutes{ secondToMinute(seconds) };
    return minutes;
}

String PlaylistComponent::secondToMinute(int seconds)
{
    //Minutes in string
    String minStr{ std::to_string(seconds / 60) };
    //Seconds in string
    String secStr{ std::to_string(seconds % 60) };
    //return minutes and seconds in a single string
    return String{ minStr + ":" + secStr };
}
bool PlaylistComponent::isInPlaylist(String fileName)
{
    return (std::find(tracks.begin(), tracks.end(), fileName) != tracks.end());
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadMusic)
    {
        //Load files when button was clicked
        importToPlaylist();
        playlist.updateContent();
    }
    else if (button == &addToDeckGUI1)
    {
        //Load selected music file to deckGUI 1 when button was clicked
        loadToDeckGUI(deckGUI1);
    }
    else if (button == &addToDeckGUI2)
    {
        //Load selected music file to deckGUI 2 when button was clicked
        loadToDeckGUI(deckGUI2);
    }
    else
    {
        //tracks[id].fileName will be removed from the playlist
        int id = std::stoi(button->getComponentID().toStdString());
        deleteTrack(id);
        playlist.updateContent();
    }
}

void PlaylistComponent::searchInPlaylist(String keyword)
{
    //Search the track in the playlist
    if (keyword != "")
    {
        int rowIdNumber = findTrackName(keyword);
        //change the track color when there are matching keyword
        playlist.selectRow(rowIdNumber);
    }
    else
    {
        //not highlight anythig
        playlist.deselectAllRows();
    }
}
int PlaylistComponent::findTrackName(String keyword)
{
    // finds the index id where the input search contains the keyword
    auto music = find_if(tracks.begin(), tracks.end(),
        [&keyword](const TrackData& obj) {return obj.fileName.contains(keyword); });
    int i = -1;

    if (music != tracks.end())
    {
        //track is found if it is not the end of the search
        i = std::distance(tracks.begin(), music);
    }
    return i;
}
void PlaylistComponent::deleteTrack(int id)
{
    tracks.erase(tracks.begin() + id);
}

//Function to load tracks to deck
void PlaylistComponent::loadToDeckGUI(DeckGUI* deckGUI)
{
    int selectedRow{ playlist.getSelectedRow() };
    if (selectedRow != -1)
    {
        //Add the selected music to the DeckGUI
        deckGUI->loadFile(tracks[selectedRow].URL);
    }
}
bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files) {
    return true;
}
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y) {
    if (files.size() == 1)
    {
        for (const File& files : chooser.getResults())
            {
                String fileName{ files.getFileNameWithoutExtension() };
                if (!isInPlaylist(fileName)) // if there is not repeated file 
                {
                    //load file and get the file meta data to the playlist
                    TrackData importedFile{ files };
                    URL audioURL{ files };
                    importedFile.length = getLength(audioURL);
                    tracks.push_back(importedFile);
                }
             playlist.updateContent();
         }
    }
}
void PlaylistComponent::saveInPlaylist()
{
    // create a text file and save in the playlist
    std::ofstream playlist("playlist.txt");

    for (TrackData& t : tracks)
    {
        playlist << t.file.getFullPathName() << "," << t.length << "\n";
    }
}
void PlaylistComponent::loadPlaylist()
{
    //Read from the saved playlist.txt
    std::ifstream playlist("playlist.txt");
    std::string path;
    std::string length;

    // Read data from playlist.txt line by line
    if (playlist.is_open())
    {
        while (getline(playlist, path, ',')) {
            File file{ path };
            TrackData loadPlaylist{ file };

            getline(playlist, length);
            loadPlaylist.length = length;
            tracks.push_back(loadPlaylist);
        }
    }
    playlist.close();
}