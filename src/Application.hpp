#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Clock.hpp"

namespace faun 
{
	class Application
	{
	public:
		Application();
		~Application();
		void run();
		void processEvents();
		void processLogic();
		void processRendering();
	private:
		sf::RenderWindow window;
		sf::View mainView;
		sf::View editorView;
		sf::Event evnt;
		faun::Clock mainClock;
		sf::RectangleShape* activeBox;
		std::vector<sf::RectangleShape> programObjects; // Main Program
		std::vector<sf::RectangleShape> editorObjects; // Editor
	};
}