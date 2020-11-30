// STD LIB
#include <iostream>
#include <queue>
// EXT LIB
#include <SFML/Graphics.hpp>
// LOCAL LIB
#include "funcs.hpp"

int main() {
	// Window Setup
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ur mom", sf::Style::Default);
	sf::View view(sf::Vector2f(1920.0f / 2.0f, 1080.0f / 2.0f), sf::Vector2f(1920.0f, 1080.0f));
	faun::adaptView(window, view);
	window.setPosition(sf::Vector2i(1, 0));
	window.setKeyRepeatEnabled(false);

	// Time Tracking
	sf::Event evnt;
	sf::Clock programClock;
	std::queue<long double> frames;
	frames.push(0.0L);
	long double totalElapsedTime = 0.0L;

	std::vector<sf::RectangleShape> boxes{
		sf::RectangleShape(sf::Vector2f(200, 100)),
		sf::RectangleShape(sf::Vector2f(100, 200))
	};
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
				faun::adaptView(window, view);
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
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
				if (evnt.mouseButton.button == sf::Mouse::Left) {
					if (activeBox != nullptr) {
						activeBox->setPosition(mousePos);
					}
				}
				else if (evnt.mouseButton.button == sf::Mouse::Right) {
					bool boxHit = false;
					for (int i = boxes.size() - 1; i >= 0; --i) {
						if (boxes[i].getGlobalBounds().contains(mousePos)) {
							if (activeBox != nullptr) {
								activeBox->setFillColor(sf::Color::White);
							}
							activeBox = &boxes[i];
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

		// Draw
		window.clear();
		for (const auto& box : boxes) {
			window.draw(box);
		}
		window.display();

	}

}