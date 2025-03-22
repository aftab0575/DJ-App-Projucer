#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================

DeckGUI::DeckGUI(int trackNum,
    DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse)
    : trackNumber(trackNum),
      player(_player),
    formatManager(formatManagerToUse),
    waveformDisplay(formatManagerToUse, cacheToUse)
{
    waveformDisplay.addChangeListener(this); //listener For draging song position using mouse!

    // Set label properties
    trackLabel.setFont(Font(18.0f, Font::bold));
    trackLabel.setJustificationType(Justification::centred);
    trackLabel.setColour(Label::textColourId, Colours::white);

    // Assign Track Name based on track number
    trackLabel.setText("Track: " + String(trackNumber), dontSendNotification);

    // Add label to the component
    addAndMakeVisible(trackLabel);

    // Set button texts with symbols for better UX
     // Initialize buttons with default styles
    customizeButton(playButton, "Play");
    customizeButton(stopButton, "Stop");
    customizeButton(loadButton, "Load Track");

    // Add components to the UI
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);

    // Assign event listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);

    // Slider appearance customization
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 4.0); // Updated speed range for better control

    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    volSlider.setColour(Slider::thumbColourId, Colours::cyan);
    speedSlider.setColour(Slider::thumbColourId, Colours::orange);

    volSlider.setColour(Slider::trackColourId, Colours::darkcyan);
    speedSlider.setColour(Slider::trackColourId, Colours::darkorange);

    startTimer(30); // Updates every 30ms for smoother animation

    DBG("DeckGUI initialized.");
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics& g)
{
    // Create a gradient background from dark to light
    ColourGradient backgroundGradient(Colours::black, 0, 0, Colours::darkblue, getWidth(), getHeight(), false);
    g.setGradientFill(backgroundGradient);
    g.fillAll();

    // Draw a subtle border
    g.setColour(Colours::darkgrey);
    g.drawRect(getLocalBounds(), 2);
}

void DeckGUI::resized()
{
    auto bounds = getLocalBounds().reduced(10); // Add padding around elements

    // Label - Reserve a small portion at the top
    auto labelArea = bounds.removeFromTop(30);
    trackLabel.setBounds(labelArea);

    // Increase Waveform Display height to use extra space (55% of total height)
    auto waveformArea = bounds.removeFromTop(bounds.getHeight() * 0.55);
    waveformDisplay.setBounds(waveformArea);

    // Sliders - Placed below waveform with spacing
    auto sliderHeight = 40;
    volSlider.setBounds(bounds.removeFromTop(sliderHeight).reduced(5));
    speedSlider.setBounds(bounds.removeFromTop(sliderHeight).reduced(5));

    // Play & Stop Buttons - Side by side with padding
    auto buttonArea = bounds.removeFromTop(50);
    playButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(8));
    stopButton.setBounds(buttonArea.reduced(8));

    // Load Track Directly Button - Below Play/Stop
    loadButton.setBounds(bounds.removeFromTop(50).reduced(8));
}

// Function to style buttons
void DeckGUI::customizeButton(TextButton& button, String buttonText)
{
    button.setButtonText(buttonText);
    button.setColour(TextButton::buttonColourId, Colour::fromRGB(90, 10, 70));  // Dark grey base color
    button.setColour(TextButton::textColourOffId, Colours::white);
    button.setColour(TextButton::textColourOnId, Colours::white);
    button.setColour(TextButton::buttonOnColourId, Colours::orange);  // Highlight on press

    // Adding Hover Effect
    //button.onMouseEnter = [&]() { button.setColour(TextButton::buttonColourId, Colour::fromRGB(70, 70, 70)); };
    //button.onMouseExit = [&]() { button.setColour(TextButton::buttonColourId, Colour::fromRGB(50, 50, 50)); };
}







void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        DBG("▶️ Play button clicked!");
        player->start();

        // 🔹 Check if player is actually playing
        DBG("🎵 Is player playing? " + String(player->isPlaying() ? "YES" : "NO"));
    }

    if (button == &stopButton)
    {
        DBG("⏹ Stop button clicked!");
        player->stop();
    }

    if (button == &loadButton)
    {
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                File chosenFile = chooser.getResult();
                if (chosenFile.exists()) {
                    player->loadURL(URL{ chooser.getResult() });
                    waveformDisplay.loadURL(URL{ chooser.getResult() });
                }
            });
    }
}


void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        player->loadURL(URL{ File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{
    if (!waveformDisplay.isMouseButtonDown())  // Prevents override during user interaction
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }

}

void DeckGUI::loadTrack(const File& file)
{
    DBG("✅ DeckGUI - Loading track: " + file.getFullPathName());

    if (!file.existsAsFile())
    {
        DBG("❌ DeckGUI - Error: File does not exist! " + file.getFullPathName());
        return;
    }

    player->stop();  // Stop any existing playback
    player->loadURL(URL(file));  // Load the new track
    waveformDisplay.loadURL(URL(file));  // Update waveform display

    DBG("▶️ Auto-playing track...");
    player->start();  // ✅ Automatically start playing

    DBG("✅ DeckGUI - Track successfully loaded and playing: " + file.getFileName());
}


void DeckGUI::changeListenerCallback(ChangeBroadcaster* source)
{
    DBG("Seeking to changeListenerCallback: ");

    if (source == &waveformDisplay)  // Ensure it's coming from waveform
    {
        double newPos = waveformDisplay.getPosition();
        DBG("Seeking to: " + String(newPos));

        player->setPositionRelative(newPos); // ✅ Actually seek in audio
    }

}

