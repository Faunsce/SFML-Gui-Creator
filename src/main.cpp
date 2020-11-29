#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ur mom", sf::Style::Default);
	sf::RectangleShape testBox(sf::Vector2f(100, 100));
	sf::Event evnt;
	enum DIRECTION {LEFT, RIGHT};
	DIRECTION direction = LEFT;
	float speed = 10.0f;
	sf::Clock programClock;
	while (window.isOpen()) {
		// Input
		while (window.pollEvent(evnt)) {
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		// Logic
		if (testBox.getPosition().x > sf::VideoMode::getDesktopMode().width - 100.0f) {
			direction = DIRECTION::LEFT;
		} else if (testBox.getPosition().x < 100.0f) {
			direction = DIRECTION::RIGHT;
		}
		if (direction == DIRECTION::LEFT) {
			testBox.move(sf::Vector2f(-speed * programClock.restart().asSeconds(), 0.0f));
		} else if (direction == DIRECTION::RIGHT) {
			testBox.move(sf::Vector2f(speed * programClock.restart().asSeconds(), 0.0f));
		}
		// Draw
		window.clear();
		window.draw(testBox);
		window.display();
	}
}