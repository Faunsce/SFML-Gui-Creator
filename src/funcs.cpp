#include "funcs.hpp"
#include "globals.hpp"

namespace faun
{
	double CurrentAspectRatio(sf::Vector2u size) {
		return (static_cast<double>(size.x) / size.y);
	}

	void adaptView(double aspectRatio, sf::View& view) {
		// Check if program and window aspect ratios match
		if (aspectRatio != (faun::TRUE_RENDER_ASPECT_RATIO)) {
			// Check which part of aspect ratio is being voided
			if (aspectRatio > (faun::TRUE_RENDER_ASPECT_RATIO)) { // Add bars left and right
				view.setViewport(sf::FloatRect(
					(1.0f - ((faun::TRUE_RENDER_ASPECT_RATIO) / aspectRatio)) / 2.0f,
					0.0f,
					(faun::TRUE_RENDER_ASPECT_RATIO) / aspectRatio,
					1.0f
				));
			} else { // Add bars above and below
				view.setViewport(sf::FloatRect(
					0.0f,
					(1.0f - (aspectRatio / (faun::TRUE_RENDER_ASPECT_RATIO))) / 2.0f,
					1.0f,
					aspectRatio / (faun::TRUE_RENDER_ASPECT_RATIO)
				));
			}
		} else {
			view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	void adaptView(sf::View& view, sf::View& parentView) {
		view.setViewport(sf::FloatRect(
			(parentView.getViewport().left + (parentView.getViewport().width / 4.0f)),
			parentView.getViewport().top,
			(parentView.getViewport().width / 4.0f) * 3.0f,
			(parentView.getViewport().height / 4.0f) * 3.0f
		));
	}
}