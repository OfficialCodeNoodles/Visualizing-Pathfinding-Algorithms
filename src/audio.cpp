#include "../hdr/audio.hpp"

namespace engine {
	namespace audio {
		sf::SoundBuffer soundBuffer;
		sf::Sound sounds[mNumOfSoundObjects]; 
		int lastPlayed = 0; 

		void loadAssets() {
			soundBuffer.loadFromFile("beep.wav"); 

			for (sf::Sound& sound : sounds) {
				sound.setBuffer(soundBuffer);
				sound.setVolume(25.0f); 
			}
		}

		void changeVolume(float newVolume) {
			for (sf::Sound& sound : sounds) 
				sound.setVolume(newVolume); 
		}
		void playSound(float pitch) {
			if (lastPlayed == ticks)
				return; 

			lastPlayed = ticks; 

			for (sf::Sound& sound : sounds) {
				if (sound.getStatus() == sf::Sound::Status::Stopped) {
					sound.setPitch(pitch); 
					sound.play();
					break; 
				}
			}
		}
	}
}