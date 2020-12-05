#include "SceneNode.hpp"
#include <memory> // std::unique_ptr
#include <algorithm> // std::find
#include <cassert> // assert
#include <utility> // std::move
#include <SFML/Graphics.hpp>

namespace faun
{
	SceneNode::SceneNode() {
		parent = nullptr;
	}

	void SceneNode::addChild(std::unique_ptr<SceneNode> child) {
		child->parent = this;
		children.push_back(std::move(child));
	}

	std::unique_ptr<SceneNode> SceneNode::removeChild(const SceneNode& child) {
		auto found = std::find_if(children.begin(), children.end(),
			[&](std::unique_ptr<SceneNode>& ptr) -> bool { return ptr.get() == &child; });
		assert(found != children.end());
		std::unique_ptr<SceneNode> result = std::move(*found);
		result->parent = nullptr;
		children.erase(found);
		return result;
	}

	SceneNode::~SceneNode() {

	}

	void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		drawSelf(target, states);
		for (const auto& child : children) {
			child->draw(target, states);
		}
	}

	void SceneNode::update(double long timeDelta) {
		updateSelf(timeDelta);
		for (const auto& child : children) {
			child->update(timeDelta);
		}
	}

	sf::Transform SceneNode::getWorldTransform() const {
		sf::Transform transform = sf::Transform::Identity;
		for (const SceneNode* node = this; node != nullptr; node = node->parent) {
			transform *= node->getTransform();
		}
		return transform;
	}

	sf::Vector2f SceneNode::getWorldPosition() const {
		return this->getWorldTransform() * sf::Vector2f();
	}
}