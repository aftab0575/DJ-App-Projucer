/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer,
                   public ChangeListener // New: Listen for waveform changes
{
public:
    DeckGUI(int trackNum,
           DJAudioPlayer* player,
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override; 

    void loadTrack(const File& file);

    void customizeButton(TextButton& button, String buttonText);

private:


    Label trackLabel; // Label for track name
    int trackNumber;
    juce::FileChooser fChooser{"Select a file..."};


    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
  
    Slider volSlider; 
    Slider speedSlider;
    //Slider posSlider;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player; 

    AudioTransportSource transportSource; // Added transport source
    std::unique_ptr<AudioFormatReaderSource> audioFormatReaderSource; // Smart pointer for audio source
    AudioFormatManager& formatManager; // Reference to format manager


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
