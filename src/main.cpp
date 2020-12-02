// STD LIB
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <future>
// EXT LIB
#include <SFML/Graphics.hpp>
// LOCAL LIB
#include "Funcs.hpp"
#include "Globals.hpp"
#include "Clock.hpp"

enum ORDERS { RENDER = 0, PAUSE = 1, SEPPUKU = 2 };

double CurrentAspectRatio(sf::Vector2u size) {
	return (static_cast<double>(size.x) / size.y);
}

void killThread(std::atomic_bool& renderOrders) {
	renderOrders = ORDERS::SEPPUKU;
}

int main() {
	// Window Setup
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "FPS : []", sf::Style::Default);
	sf::View mainView(sf::Vector2f(faun::TRUE_RENDER_WIDTH / 2.0, faun::TRUE_RENDER_HEIGHT / 2.0f),
		sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT));
	sf::View editorView(sf::Vector2f(faun::TRUE_RENDER_WIDTH / 2.0, faun::TRUE_RENDER_HEIGHT / 2.0),
		sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT));
	faun::adaptView(CurrentAspectRatio(window.getSize()), mainView);
	faun::adaptView(editorView, mainView);
	window.setPosition(sf::Vector2i(1, 0));
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);
	window.setActive(false);

	// Time Tracking
	sf::Event evnt;
	faun::Clock mainClock;

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

	sf::RectangleShape* activeBox = nullptr;

	// Rendering Thread
	std::atomic<int> threadOrders = ORDERS::RENDER;
	std::mutex renderMutex;
	std::thread renderThread(
		[&programObjects, &editorObjects, &window, &mainView, &editorView, &renderMutex, &threadOrders] {
			window.setActive(true);
			while (window.isOpen()) {
				switch (threadOrders)
				{
				case ORDERS::RENDER: {
					renderMutex.lock();
					window.clear();
					window.setView(mainView);
					for (const auto& object : programObjects) {
						window.draw(object);
					}
					window.setView(editorView);
					if (!editorObjects.empty()) {
						for (const auto& object : editorObjects) {
							window.draw(object);
						}
					}
					window.display();
					renderMutex.unlock();
				}
				case ORDERS::PAUSE: {
					// do nothing
					break;
				}
				case ORDERS::SEPPUKU: {
					window.close();
					window.setActive(false);
					break;
				}
				default:
					break;
				}
			}
		}
	);

	while (window.isOpen()) {

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
				threadOrders = ORDERS::SEPPUKU;
				renderThread.join();
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
				case sf::Keyboard::Enter: {
					sf::RectangleShape newRect(sf::Vector2f(100, 100));
					newRect.setPosition(sf::Vector2f(faun::TRUE_RENDER_WIDTH / 2.0, faun::TRUE_RENDER_HEIGHT / 2.0));
					newRect.setFillColor(sf::Color::Cyan);
					editorObjects.emplace_back(newRect);
					break;
				}
				case sf::Keyboard::Delete: {
					if (!editorObjects.empty()) {
						editorObjects.pop_back();
					}
					break;
				}
				case sf::Keyboard::Escape: {
					threadOrders = ORDERS::SEPPUKU;
					renderThread.join();
					break;
				}
				default:
					break;
				}
			}
			case sf::Event::MouseButtonPressed: {
				sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
				// TODO : Fix math for calculating if mouse is in the editor
				bool inEditor =
				[&mainView, &editorView, &window, &mousePosI] {
					if (window.mapPixelToCoords(mousePosI, mainView).x > faun::TRUE_RENDER_WIDTH / 4.0 
						|| window.mapPixelToCoords(mousePosI, mainView).y < faun::TRUE_RENDER_HEIGHT / 4.0) {
						return true;
					} else {
						return false;
					}
				}();
				sf::Vector2f mousePos = window.mapPixelToCoords(mousePosI, (inEditor) ? editorView : mainView );
				if (inEditor) {
					if (evnt.mouseButton.button == sf::Mouse::Left) {
						if (activeBox != nullptr) {
							activeBox->setPosition(mousePos);
						}
					} else if (evnt.mouseButton.button == sf::Mouse::Right) {
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
				} else {
					if (activeBox != nullptr) {
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
		long double deltaTime = mainClock.Update();
		editorObjects[1].move(5 * deltaTime, 0);
		window.setTitle(std::to_string(mainClock.FPS()));


		// Drawing
		/*window.clear();
		window.setView(mainView);
		for (const auto& object : programObjects) {
			window.draw(object);
		}
		window.setView(editorView);
		if (!editorObjects.empty()) {
			for (const auto& object : editorObjects) {
				window.draw(object);
			}
		}
		window.display();*/
	}
}