#include "funcs.hpp"

namespace faun
{
	void adaptView(float aspectRatio, sf::View& view) {
		// Check if program and window aspect ratios match
		if (aspectRatio != (16.0f / 9.0f)) {
			// Check which part of aspect ratio is being voided
			if (aspectRatio > (16.0 / 9.0)) { // Add bars left and right
				view.setViewport(sf::FloatRect(
					(1.0f - ((16.0f / 9.0f) / aspectRatio)) / 2.0f,
					0.0f,
					(16.0f / 9.0f) / aspectRatio,
					1.0f
				));
			} else { // Add bars above and below
				view.setViewport(sf::FloatRect(
					0.0f,
					(1.0f - (aspectRatio / (16.0f / 9.0f))) / 2.0f,
					1.0f,
					aspectRatio / (16.0f / 9.0f)
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