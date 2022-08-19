#pragma once

//Dependencies
#include <vector>
#include <queue>
#include <unordered_map>

#include <Glass/glass.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

//Macros

#define mGetStyle(fullscreen) fullscreen ? sf::Style::Fullscreen :\
	sf::Style::Default
#define mNumOfSoundObjects 20 

//Globals

extern int ticks;