#include "MusicLibrary.h"
#include "../JuceLibraryCode/JuceHeader.h"

MusicLibrary::MusicLibrary()
{
    std::cout << "MusicLibrary initialized!" << std::endl;
    // Set library file path
    libraryFilePath = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("music_library.txt").getFullPathName();

    // Configure table
    addAndMakeVisible(table);
    table.setModel(this);
    table.setHeaderHeight(30);

    // Add columns without fixed width (we'll set them in resized())
    table.getHeader().addColumn("Track Name", 1, 100); // Placeholder width
    table.getHeader().addColumn("Delete", 2, 80);      // Fixed width

    // Add search box
    addAndMakeVisible(searchBox);
    searchBox.setTextToShowWhenEmpty("Search for a track...", Colours::grey);
    searchBox.addListener(this);

    // Add button
    addAndMakeVisible(addButton);
    addButton.addListener(this);

    // Load saved library
    loadLibrary();
}

MusicLibrary::~MusicLibrary() {}

void MusicLibrary::paint(Graphics& g)
{
    // Apply a gradient background to the music library
    ColourGradient gradient(Colours::midnightblue, 0.0f, 0.0f, Colours::darkslateblue, getWidth(), getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();

    // Apply a soft shadow effect for the search box and button
    g.setColour(Colours::black.withAlpha(0.2f));
    g.fillRoundedRectangle(searchBox.getBounds().toFloat(), 8.0f);
    g.fillRoundedRectangle(addButton.getBounds().toFloat(), 8.0f);

  
    //// Add smooth fade-in effect for text (e.g., "Add Music" button)
    g.setColour(Colours::green.withAlpha(0.8f));
    g.setFont(18.0f);
    g.drawText("Add Music", addButton.getBounds(), Justification::centred, true);

    //// Optional: Add some subtle hover effect for the button (optional)
    if (addButton.isMouseOver())
    {
        g.setColour(Colours::green.withAlpha(0.6f));
        g.setFont(18.0f);
        g.drawText("Add Music", addButton.getBounds(), Justification::centred, true);
    }
}


void MusicLibrary::resized()
{
    // Animations for resizing with smooth transitions
    const int margin = 10;
    const int buttonWidth = 80;
    const int buttonHeight = 30;

    int tableWidth = getWidth();  // Get available width
    int deleteColumnWidth = 80;   // Fixed width for delete button
    int trackColumnWidth = tableWidth - deleteColumnWidth - 20; // Remaining width

    table.getHeader().setColumnWidth(1, trackColumnWidth);
    table.getHeader().setColumnWidth(2, deleteColumnWidth);

    // Search box should be responsive
    searchBox.setBounds(margin, margin, getWidth() - buttonWidth - 2 * margin, buttonHeight);

    // Button with smooth shadow effect
    addButton.setBounds(getWidth() - buttonWidth - margin, margin, buttonWidth, buttonHeight);

    // Table should take the remaining height and width
    table.setBounds(margin, margin + buttonHeight + 10, getWidth() - 2 * margin, getHeight() - margin - buttonHeight - 20);
    //table.getViewport()->getViewedComponent()->removeAllChildren();

    table.updateContent();
    table.repaint();       // Redraws the table

}


// Table methods
int MusicLibrary::getNumRows()
{
    return tracks.size();
}

void MusicLibrary::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.setColour(Colours::red.withAlpha(0.5f)); // Better contrast for selected row
    }
    else
    {
        g.setColour(rowNumber % 2 == 0 ? Colours::green.withAlpha(0.4f) : Colours::blue.withAlpha(0.4f));
    }

    g.fillRect(0, 0, width, height);
}




void MusicLibrary::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowSelected)
{
    if (rowNumber < displayedTracks.size())
    {
        g.setColour(Colours::white);
        g.drawText(displayedTracks[rowNumber].getFileName(), 2, 0, width - 4, height, Justification::centredLeft);
    }
    else
    {
        g.setColour(Colours::grey);
        g.drawText("No Tracks Available", 2, 0, width - 4, height, Justification::centredLeft);
    }
}


Component* MusicLibrary::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId == 2) // Delete button column
    {
        auto* deleteButton = dynamic_cast<TextButton*>(existingComponentToUpdate);
        if (!deleteButton)
        {
            deleteButton = new TextButton("X");
            deleteButton->setColour(TextButton::buttonColourId, Colours::red);
            deleteButton->onClick = [this, rowNumber]() { deleteTrack(rowNumber); };
        }
        return deleteButton;
    }
    return nullptr;
}



void MusicLibrary::deleteTrack(int rowNumber)
{
    if (rowNumber >= 0 && rowNumber < displayedTracks.size())
    {
        File trackToDelete = displayedTracks[rowNumber];
        tracks.erase(std::remove(tracks.begin(), tracks.end(), trackToDelete), tracks.end());
        displayedTracks.erase(displayedTracks.begin() + rowNumber);

        saveLibrary();
        table.updateContent();
    }
}


// Load & Save Library
void MusicLibrary::loadLibrary()
{
    tracks.clear();
    displayedTracks.clear();

    File libraryFile(libraryFilePath);
    if (libraryFile.existsAsFile())
    {
        StringArray lines;
        libraryFile.readLines(lines);

        for (const auto& line : lines)
        {
            File track(line.trim());
            if (track.existsAsFile())
            {
                tracks.push_back(track);
            }
            else
            {
                DBG("❌ Skipping invalid file: " + track.getFullPathName());
            }
        }
    }

    displayedTracks = tracks;
    saveLibrary();
    table.updateContent();
}


void MusicLibrary::saveLibrary()
{
    File libraryFile(libraryFilePath);
    String data;

    for (const auto& track : tracks)
        data += track.getFullPathName() + "\n";

    libraryFile.replaceWithText(data);  // ✅ More reliable than deleteFile() + create()
}




void MusicLibrary::buttonClicked(Button* button)
{
    if (button == &addButton)
    {
        // Apply a subtle animation or highlight when the button is clicked
        addButton.setColour(TextButton::buttonOnColourId, Colours::lightgreen);

        DBG("Add Music Button Clicked!");

        MessageManager::callAsync([this]()
            {
                DBG("Launching FileChooser...");
                fileChooser = std::make_unique<FileChooser>("Select Audio Files", File(), "*.mp3;*.wav");

                fileChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectMultipleItems,
                    [this](const FileChooser& fc)
                    {
                        DBG("Inside FileChooser callback!");
                        auto results = fc.getResults();

                        if (results.isEmpty())
                        {
                            DBG("No files selected.");
                            return;
                        }

                        DBG("Files selected!");
                        for (const auto& file : results)
                        {
                            // ✅ Check if the file is already in the list before adding
                            bool exists = std::any_of(tracks.begin(), tracks.end(),
                                [&file](const File& existingFile)
                                {
                                    return existingFile.getFullPathName() == file.getFullPathName();
                                });

                            if (!exists)
                            {
                                DBG("Adding file: " + file.getFullPathName());
                                tracks.push_back(file);
                            }
                            else
                            {
                                DBG("File already exists, skipping: " + file.getFullPathName());
                            }
                        }

                        displayedTracks = tracks;
                        saveLibrary();
                        table.updateContent();
                        DBG("Library updated!");
                    });
            });
    }
}


// Search Functionality
void MusicLibrary::textEditorTextChanged(TextEditor& textEditor)
{
    String searchText = searchBox.getText().trim();

    if (searchText.isEmpty())
    {
        displayedTracks = tracks;
    }
    else
    {
        displayedTracks.clear();
        for (const auto& track : tracks)
        {
            if (track.getFileNameWithoutExtension().containsIgnoreCase(searchText))
                displayedTracks.push_back(track);
        }

        if (displayedTracks.empty())
        {
            DBG("No matching tracks found!");
        }
    }

    table.updateContent();
}




// Load Music into Deck
void MusicLibrary::cellClicked(int rowNumber, int columnId, const MouseEvent& e)
{
    if (rowNumber >= 0 && rowNumber < displayedTracks.size())
    {
        File selectedFile = displayedTracks[rowNumber];

        DBG("Track selected: " + selectedFile.getFileName());

        if (onTrackSelected) // If DeckGUI is connected, notify it
        {
            onTrackSelected(selectedFile);
        }
    }
}

void MusicLibrary::fileSelected(const File& file)
{
    DBG("✅ MusicLibrary - Selected track: " + file.getFullPathName());

    if (file.getFullPathName().isEmpty())
    {
        DBG("❌ MusicLibrary - Error: File path is empty!");
        return;
    }

    if (!file.existsAsFile())
    {
        DBG("❌ MusicLibrary - Error: File does not exist! " + file.getFullPathName());
        return;
    }

    if (onTrackSelected)
    {
        onTrackSelected(file);
    }
}
