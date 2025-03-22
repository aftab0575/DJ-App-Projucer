# Otodecks - Advanced DJ Application

## Project Overview
Otodecks is a DJ application developed as part of a course assignment. This project extends the basic functionality by introducing a custom deck control component and a music library component. The new features enhance the usability of the application, allowing users to manipulate audio tracks creatively and manage a music library efficiently.

## Features

### Basic Features (Inherited from Course Work)
- Load audio files into audio players.
- Play two or more tracks simultaneously.
- Mix tracks by adjusting their volume levels.
- Control track speed (speed up or slow down).

### Custom Features Added
#### 1. **Custom Deck Control Component**
- Implemented a visually unique deck control with custom graphics using the `paint` function.
- Added interactive controls that allow dynamic manipulation of track playback.

#### 2. **Music Library Component**
- Users can manage a music library within the application.
- Search functionality to find specific tracks.
- Load tracks from the library into decks.
- Persistent storage: The library state is saved in a data file and remains available across application sessions.

#### 3. **User Interface Customization**
- Redesigned GUI layout with an improved user experience.
- Introduced color themes and a modernized control panel.
- Implemented additional event listeners for enhanced interactivity.

#### 4. **New DJ-Inspired Feature**
- Implemented a **Beat Sync Feature** inspired by professional DJ software.
- This feature allows seamless synchronization of beats between tracks to maintain a smooth mix.
- Engineered using object-oriented programming principles with dedicated classes for beat analysis and synchronization.

## Installation and Setup

### Prerequisites
- JUCE framework installed.
- Visual Studio 2022 (or any compatible IDE with JUCE support).
- Audio files for testing.

### Steps to Run the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/YOUR_GITHUB_USERNAME/Otodecks.git
   cd Otodecks
   ```
2. Open the project in Projucer.
3. Configure the project settings and export it to your IDE (e.g., Visual Studio 2022).
4. Build and run the application.

## Usage Guide
1. Load audio tracks into the decks.
2. Use the volume and speed controls to adjust the playback.
3. Explore the custom deck control for advanced track manipulation.
4. Manage your music library by adding, searching, and loading tracks.
5. Use the beat sync feature to align tracks for a seamless DJ mix.

## Project Structure
```
Otodecks/
│-- Source/
│   │-- MainComponent.cpp
│   │-- DeckGUI.cpp
│   │-- CustomDeckControl.cpp
│   │-- MusicLibrary.cpp
│   │-- BeatSyncFeature.cpp
│-- Resources/
│   │-- Icons/
│   │-- Skins/
│-- Data/
│   │-- musicLibraryData.json
│-- README.md
│-- LICENSE
```

## Future Improvements
- Implement waveform visualization.
- Add effects like reverb and echo.
- Enhance UI with draggable track panels.

## Contributions
Feel free to fork the project and submit pull requests with enhancements!

## License
This project is licensed under the MIT License.

## Author
**Aftab Bashir**
- GitHub: [GITHUB_PROFILE](https://github.com/aftab0575)
