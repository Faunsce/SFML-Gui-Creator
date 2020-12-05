#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

namespace faun
{
	class SceneNode : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		SceneNode();
		~SceneNode();
		void addChild(std::unique_ptr<SceneNode>);
		std::unique_ptr<SceneNode> removeChild(const SceneNode&);
	private:
		void update(double long);
		virtual void updateSelf(double long) = 0;
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const final;
		virtual void drawSelf(sf::RenderTarget&, sf::RenderStates) const = 0;
		sf::Transform getWorldTransform() const;
		sf::Vector2f getWorldPosition() const;
		std::vector<std::unique_ptr<SceneNode>> children;
		SceneNode* parent;
	};
}