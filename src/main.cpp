// STD LIB
#include <iostream>
#include <queue>
// EXT LIB
#include <SFML/Graphics.hpp>
// LOCAL LIB
#include "funcs.hpp"
#include "globals.hpp"

double CurrentAspectRatio(sf::Vector2u size) {
	return (static_cast<double>(size.x) / size.y);
}

int main() {
	// Window Setup
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ur mom", sf::Style::Default);
	sf::View mainView(sf::Vector2f(faun::TRUE_RENDER_WIDTH / 2.0, faun::TRUE_RENDER_HEIGHT / 2.0f), 
		sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT));
	sf::View editorView(sf::Vector2f((faun::TRUE_RENDER_WIDTH / 2.0) + faun::TRUE_RENDER_WIDTH, 
		(faun::TRUE_RENDER_HEIGHT / 2.0)), sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT));
	faun::adaptView(CurrentAspectRatio(window.getSize()), mainView);
	faun::adaptView(editorView, mainView);
	window.setPosition(sf::Vector2i(1, 0));
	window.setKeyRepeatEnabled(false);

	// Time Tracking
	sf::Event evnt;
	sf::Clock programClock;
	std::queue<long double> frames;
	frames.push(0.0L);
	long double totalElapsedTime = 0.0L;

	// Scene Setup
	std::vector<sf::RectangleShape> programObjects{ // Program
		sf::RectangleShape(sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT))
	};
	programObjects[0].setFillColor(sf::Color::Green);
	std::vector<sf::RectangleShape> editorObjects{ // Stuff in editor
		sf::RectangleShape(sf::Vector2f(1920, 1080)),
		sf::RectangleShape(sf::Vector2f(200, 100)),
		sf::RectangleShape(sf::Vector2f(100, 200))
	};
	editorObjects[0].setFillColor(sf::Color::White); // Set distinct colors
	editorObjects[1].setFillColor(sf::Color::Red); 
	editorObjects[2].setFillColor(sf::Color::Blue);
	editorObjects[0].move(1920, 0); // adjust to "plane"
	editorObjects[1].move(1920, 0);
	editorObjects[2].move(1920, 0);
	sf::RectangleShape* activeBox = nullptr;

	while (window.isOpen()) {

		// Time && FPS Calculations
		long double elapsedTime = programClock.restart().asSeconds();
		totalElapsedTime += elapsedTime;
		while (!frames.empty() && frames.front() < totalElapsedTime - 1.0L ) {
			frames.pop();
		}
		frames.push(totalElapsedTime);

		// Input
		while (window.pollEvent(evnt)) {
			switch (evnt.type)
			{
			case sf::Event::Resized: {
				faun::adaptView(CurrentAspectRatio(window.getSize()), mainView);
				faun::adaptView(editorView, mainView);
				break;
			}
			case sf::Event::Closed: {
				window.close();
				break;
			}
			case sf::Event::KeyPressed: {
				switch (evnt.key.code)
				{
				case sf::Keyboard::BackSpace: {
					if (activeBox != nullptr) {
						activeBox->setFillColor(sf::Color::White);
						activeBox = nullptr;
					}
					break;
				}
				case sf::Keyboard::Escape: {
					window.close();
					break;
				}
				default:
					break;
				}
			}
			case sf::Event::MouseButtonPressed: {
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), 
					[&mainView, &editorView]{
						return mainView;
					}()
				);
				if (evnt.mouseButton.button == sf::Mouse::Left) {
					if (activeBox != nullptr) {
						activeBox->setPosition(mousePos);
					}
				}
				else if (evnt.mouseButton.button == sf::Mouse::Right) {
					bool boxHit = false;
					for (int i = editorObjects.size() - 1; i >= 0; --i) {
						if (editorObjects[i].getGlobalBounds().contains(mousePos)) {
							if (activeBox != nullptr) {
								activeBox->setFillColor(sf::Color::White);
							}
							activeBox = &editorObjects[i];
							activeBox->setFillColor(sf::Color::Red);
							boxHit = true;
							break;
						}
					}
					if (!boxHit && activeBox != nullptr) {
						activeBox->setFillColor(sf::Color::White);
						activeBox = nullptr;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		// Logic
		window.setTitle("FPS : [" + std::to_string(frames.size()) + "]");
		editorObjects[1].move(10.0f * elapsedTime, 0.0f);
		// Draw
		window.clear();
		window.setView(mainView);
		for (const auto& object : programObjects) {
			window.draw(object);
		}
		window.setView(editorView);
		for (const auto& object : editorObjects) {
			window.draw(object);
		}
		window.display();

	}

}