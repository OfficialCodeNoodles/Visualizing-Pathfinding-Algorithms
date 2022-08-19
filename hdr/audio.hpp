#pragma once

//Dependencies
#include "resources.hpp"

namespace engine {
	namespace audio {
		extern sf::SoundBuffer soundBuffer;
		extern sf::Sound sounds[mNumOfSoundObjects];
		extern int lastPlayed; 

		void loadAssets();

		void changeVolume(float newVolume); 
		void playSound(float pitch); 
	}
}