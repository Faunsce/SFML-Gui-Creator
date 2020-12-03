#pragma once
#include <SFML/Graphics.hpp>

namespace faun
{
	double CurrentAspectRatio(sf::Vector2u);
	void adaptView(double, sf::View&);
	void adaptView(sf::View&, sf::View&);
}