/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>  // Required for storing beat positions

//==============================================================================
/*
*/
class WaveformDisplay    : public Component, 
                           public ChangeListener,
                           public ChangeBroadcaster,
                           public Timer
{
public:
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    double getPosition() const { return position; } // Getter function
    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

    void updatePlayhead(float newPosition);

    // New: Mouse interaction for playhead dragging
    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    void timerCallback() override;  // ✅ Timer function for smooth updates


private:
    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    float colorHue = 0.0f; // Starts at red


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
