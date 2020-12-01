#pragma once
#include <queue>
#include <SFML/Graphics.hpp>

namespace faun
{
	class Clock
	{
	public:
		Clock();
		long double Update();
		int FPS();
	private:
		sf::Clock clock;
		std::queue<long double> allFrames;
		std::queue<long double> recentFrames;
		long double totalTime;
	};
}