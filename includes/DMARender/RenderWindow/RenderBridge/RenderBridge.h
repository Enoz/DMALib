#pragma once
#include <memory>
#include "../../IOverlay/IOverlay.h"
namespace DMARender {
	class RenderBridge {
		std::shared_ptr<IOverlay> overlayPtr = nullptr;

	public:
		void createFonts();

		void setOverlay(std::shared_ptr<IOverlay> ptr);
		std::shared_ptr<IOverlay> getOverlay();
	};
}