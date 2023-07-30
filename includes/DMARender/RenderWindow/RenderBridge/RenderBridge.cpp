#include "RenderBridge.h"

void DMARender::RenderBridge::createFonts()
{
	if (this->getOverlay()) {
		this->getOverlay()->createFonts();
	}
}

void DMARender::RenderBridge::setOverlay(std::shared_ptr<IOverlay> ptr)
{
	this->overlayPtr = ptr;
}

std::shared_ptr<DMARender::IOverlay> DMARender::RenderBridge::getOverlay()
{
	return this->overlayPtr;
}
