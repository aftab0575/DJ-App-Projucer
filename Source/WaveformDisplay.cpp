/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);
  startTimerHz(60); // ✅ Smoothly update 60 times per second
}

WaveformDisplay::~WaveformDisplay()
{
}
void WaveformDisplay::paint(Graphics& g)
{
    g.fillAll(Colours::black);

    // Create a light color that changes gradually over time
    Colour waveformColor = Colour::fromHSV(colorHue, 0.5f, 1.0f, 1.0f); // Light colors (light saturation and full brightness)

    if (fileLoaded)
    {
        g.setColour(waveformColor.withAlpha(0.7f)); // Apply the changing color with transparency
        float playheadX = position * getWidth();

        // Draw waveform up to the playhead
        audioThumb.drawChannel(g, Rectangle<int>(0, 0, playheadX, getHeight()),
            0, position * audioThumb.getTotalLength(), 0, 1.0f);

        // Draw smooth glowing playhead
        g.setColour(Colours::red);
        g.setOpacity(0.8f); // Smooth opacity for glowing effect
        g.drawLine(playheadX, 0, playheadX, getHeight(), 3.0f);

        // Add a glow effect to the playhead
        g.setColour(Colours::red.withAlpha(0.4f)); // Slightly transparent glow
        g.drawLine(playheadX - 2, 0, playheadX - 2, getHeight(), 6.0f);
    }
    else
    {
        g.setColour(Colours::grey);
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(), Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

    if (fileLoaded)
    {
        std::cout << "wfd: loaded!" << std::endl;
        position = 0;
        repaint();
    }
    else
    {
        std::cout << "wfd: not loaded!" << std::endl;
    }
}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }

  
}

void WaveformDisplay::mouseDown(const MouseEvent& event)
{
    double clickPos = event.position.x / getWidth();
    setPositionRelative(clickPos);
    sendChangeMessage();  // Notify DeckGUI that position changed
}

void WaveformDisplay::mouseDrag(const MouseEvent& event)
{
    double dragPos = event.position.x / getWidth();
    setPositionRelative(dragPos);
    sendChangeMessage();  // Notify DeckGUI
}

void WaveformDisplay::mouseUp(const MouseEvent& event)
{
    double newPosition = static_cast<double>(event.getPosition().getX()) / getWidth();
    setPositionRelative(newPosition);
    sendChangeMessage(); // ✅ Ensure final position is sent
}

void WaveformDisplay::updatePlayhead(float newPosition)
{
    position = newPosition;
    repaint(); // Refresh UI
}



// ✅ Timer function for smooth updates
void WaveformDisplay::timerCallback()
{
    // Gradually change the hue over time
    colorHue += 0.001f;  // Increase the hue slightly on each timer tick
    if (colorHue > 1.0f)
        colorHue = 0.0f; // Reset the hue when it exceeds 1

    repaint();
}
