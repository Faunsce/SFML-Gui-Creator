#include "Application.hpp"
#include <SFML/Graphics.hpp>
#include "Globals.hpp"
#include "Funcs.hpp"

Application::Application() 
	: window { sf::VideoMode::getDesktopMode(), "FPS : []", sf::Style::Default} // Initialize Window
	, mainView { sf::Vector2f(faun::TRUE_RENDER_WIDTH / 2.0, faun::TRUE_RENDER_HEIGHT / 2.0f), // Create view for whole program
		sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT) }
	, editorView{ sf::Vector2f(faun::TRUE_RENDER_WIDTH / 2.0, faun::TRUE_RENDER_HEIGHT / 2.0), // Create view for gui editor space
			sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT) }
	, programObjects {
		sf::RectangleShape(sf::Vector2f(faun::TRUE_RENDER_WIDTH, faun::TRUE_RENDER_HEIGHT)) }
	, editorObjects {	
		sf::RectangleShape(sf::Vector2f(1920, 1080)),
		sf::RectangleShape(sf::Vector2f(200, 100)),
		sf::RectangleShape(sf::Vector2f(100, 200)) } 
	, activeBox{ nullptr } {
	programObjects[0].setFillColor(sf::Color::Green); // Set distinct colors
	editorObjects[0].setFillColor(sf::Color::White); 
	editorObjects[1].setFillColor(sf::Color::Red);
	editorObjects[2].setFillColor(sf::Color::Blue);
	faun::adaptView(faun::CurrentAspectRatio(window.getSize()), mainView);
	faun::adaptView(editorView, mainView);
	//window.setPosition(sf::Vector2i(1, 0));
	window.setKeyRepeatEnabled(false);
}

Application::~Application() {
	while (!programObjects.empty()) {
		programObjects.pop_back();
	}
	while (!editorObjects.empty()) {
		editorObjects.pop_back();
	}
}

void Application::run() {
	while (window.isOpen()) {
		this->processEvents();
		this->processLogic();
		this->processRendering();
	}
}

void Application::processEvents() {
	while (window.pollEvent(evnt)) {
		switch (evnt.type)
		{
		case sf::Event::Resized: {
			faun::adaptView(faun::CurrentAspectRatio(window.getSize()), mainView);
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
				window.close();
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
				[this, &mousePosI] {
				if (window.mapPixelToCoords(mousePosI, mainView).x > faun::TRUE_RENDER_WIDTH / 4.0
					|| window.mapPixelToCoords(mousePosI, mainView).y < faun::TRUE_RENDER_HEIGHT / 4.0) {
					return true;
				}
				else {
					return false;
				}
			}();
			sf::Vector2f mousePos = window.mapPixelToCoords(mousePosI, (inEditor) ? editorView : mainView);
			if (inEditor) {
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
			}
			else {
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
}

void Application::processLogic() {
	long double deltaTime = mainClock.Update();
	editorObjects[1].move(5 * deltaTime, 0);
	window.setTitle(std::to_string(mainClock.FPS()));
}

void Application::processRendering() {
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
}