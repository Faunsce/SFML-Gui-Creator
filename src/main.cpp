#include <iostream>
#include <queue>
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ur mom", sf::Style::Default);
	sf::Event evnt;
	sf::Clock programClock;
	std::queue<long double> frames;
	frames.push(0.0L);
	long double totalElapsedTime = 0.0L;

	std::vector<sf::RectangleShape> boxes{
		sf::RectangleShape(sf::Vector2f(100, 100)),
		sf::RectangleShape(sf::Vector2f(100, 100))
	};
	sf::RectangleShape* activeBox = nullptr;

	while (window.isOpen()) {

		// Time && FPS Calculations
		long double elapsedTime = programClock.restart().asSeconds();
		totalElapsedTime += elapsedTime;
		while (frames.front() < totalElapsedTime - 1.0L) {
			frames.pop();
		}
		frames.push(totalElapsedTime);

		// Input
		while (window.pollEvent(evnt)) {
			switch (evnt.type)
			{
			case sf::Event::Closed: {
				window.close();
				break;
			}
			case sf::Event::KeyPressed: {
				switch (evnt.key.code)
				{
				case sf::Keyboard::Escape: {
					window.close();
				}
				default:
					break;
				}
			}
			case sf::Event::MouseButtonPressed: {
				sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
				if (evnt.mouseButton.button == sf::Mouse::Left) {
					if (activeBox != nullptr) {
						activeBox->setPosition(mousePos);
					}
				}
				else if (evnt.mouseButton.button == sf::Mouse::Right) {
					bool boxHit = false;
					for (auto& box : boxes) {
						if (box.getGlobalBounds().contains(mousePos)) {
							if (activeBox != nullptr) {
								activeBox->setFillColor(sf::Color::White);
							}
							activeBox = &box;
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