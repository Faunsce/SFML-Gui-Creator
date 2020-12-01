#pragma once
#include <SFML/Graphics.hpp>

namespace faun
{
	void adaptView(double, sf::View&);
	void adaptView(sf::View&, sf::View&);
}