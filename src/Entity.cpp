#include "Entity.hpp"
#include <SFML/Graphics.hpp>

namespace faun
{
	Entity::Entity(const sf::Texture& texture) {
		this->sprite.setTexture(texture);
		this->velocity = sf::Vector2f(0.0f, 0.0f);
	}

	Entity::Entity(const sf::Texture& texture, sf::Vector2f velocity) {
		this->sprite.setTexture(texture);
		this->velocity = velocity;
	}

	Entity::~Entity() {

	}

	void Entity::setVelocity(sf::Vector2f velocity) {
		this->velocity = velocity;
	}

	sf::Vector2f Entity::getVelocity() const {
		return this->velocity;
	}

	void Entity::updateSelf(double long deltaTime) {
		this->move(velocity.x * deltaTime, velocity.y * deltaTime);
	}
	
	void Entity::drawSelf(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(this->sprite, states);
	}
}