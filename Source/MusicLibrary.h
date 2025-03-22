#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MusicLibrary : public Component,
    public TableListBoxModel,
    public Button::Listener,
    public TextEditor::Listener
{
public:
    MusicLibrary();
    ~MusicLibrary();

    void paint(Graphics&) override;
    void resized() override;

    // Table methods
    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    void cellClicked(int rowNumber, int columnId, const MouseEvent&) override; // Load Music into Deck
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void fileSelected(const File& file);
    std::function<void(const File&)> onTrackSelected; // Callback for DeckGUI

    // Load & Save Library
    void loadLibrary();
    void saveLibrary();

    // Button & TextEditor events
    void buttonClicked(Button* button) override;
    void textEditorTextChanged(TextEditor& textEditor) override;
    std::unique_ptr<FileChooser> fileChooser;
     

private:
    TableListBox table;
    TextButton addButton{ "Add Music" };
    TextEditor searchBox;
    std::vector<File> tracks;
    String libraryFilePath;
    std::vector<File> displayedTracks; // Stores filtered tracks for display
    std::map<int, std::unique_ptr<TextButton>> deleteButtons; // Delete buttons for each track

    void deleteTrack(int rowNumber);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MusicLibrary)
};
