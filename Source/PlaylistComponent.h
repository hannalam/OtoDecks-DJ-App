/*
  ==============================================================================

    PlaylistComponent.h
    Created: 23 Feb 2022 11:08:05am
    Author:  Hanna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "TrackData.h"
#include "DJAudioPlayer.h"
using namespace juce;

//==============================================================================
/*
*/
class PlaylistComponent  : public Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener,
                           public FileDragAndDropTarget
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2,
                      DJAudioPlayer* _trackMetaData);
    ~PlaylistComponent() ;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&, 
                            int rowNumber, 
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(Graphics&,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
                     int columnId,
                     bool isRowSelected,
                     Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

private:
    FileChooser chooser{ "Select a file..." };

    /** Playlist box */
    TableListBox playlist;
    /** Track File list, rows number according to the import file number */
    std::vector<TrackData> tracks;

    /** Buttons */
    TextButton addToDeckGUI1{ "Add to Deck 1" };
    TextButton addToDeckGUI2{ "Add to Deck 2" };
    TextButton loadMusic{ "Load Music to Playlist and add music to Deck" };
    TextButton deleteButton{ "Delete" };
    /** Search Bar */
    TextEditor searchBar;

    /** Link to DeckGUI & DJAudioPlayer */
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* djAudioPlayer;

    /** Get the length from the audio file and convert from second to minute */
    String getLength(URL audioURL);
    String secondToMinute(int seconds);

    /** load a file and import to the playlist */
    void importToPlaylist();
    
    /** search the music name from the playlist, 
    if the file is found, add to the DeckGUI or delete from the playlist */
    void searchInPlaylist(String searchText);
    int findTrackName(String searchText);
    void deleteTrack(int id);
    bool isInPlaylist(String fileName);
    void loadToDeckGUI(DeckGUI* deckGUI);

    /** save the imported files in the playlist,
    load the saved files for the next open */
    void saveInPlaylist();
    void loadPlaylist();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
