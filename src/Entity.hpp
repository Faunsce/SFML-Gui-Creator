#pragma once
#include <type_traits>
#include <SFML/Graphics.hpp>
#include "SceneNode.hpp"

namespace faun
{
	class Entity : public faun::SceneNode
	{
	public:
		Entity(const sf::Texture&);
		Entity(const sf::Texture&, sf::Vector2f);
		~Entity();
		void setVelocity(sf::Vector2f);
		sf::Vector2f getVelocity() const;
		void updateSelf(double long) override;
		void drawSelf(sf::RenderTarget&, sf::RenderStates) const override;
	private:
		sf::Vector2f velocity;
		sf::Sprite sprite;
	};
}