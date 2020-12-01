#include "clock.hpp"

namespace faun
{
	Clock::Clock() {
		clock = sf::Clock();
		totalTime = 0.0L;
		recentFrames.push(0.0L);
	}

	long double Clock::Update() {
		long double deltaTime = clock.restart().asSeconds();
		totalTime += deltaTime;
		while (!recentFrames.empty() && recentFrames.front() < totalTime - 1.0L) {
			recentFrames.pop();
		}
		allFrames.push(totalTime);
		recentFrames.push(totalTime);
		return deltaTime;
	}

	int Clock::FPS() {
		return recentFrames.size();
	}
}
