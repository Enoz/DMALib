#pragma once
#include <memory>
#include "../../IOverlay/IOverlay.h"
namespace DMARender {
	class CommanderAdapter {
		std::shared_ptr<IOverlay> overlayPtr = nullptr;

	public:
		virtual void miscDraw() {};

		void setOverlay(std::shared_ptr<IOverlay> ptr);
		std::shared_ptr<IOverlay> getOverlay();
	};
}