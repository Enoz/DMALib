#include "CommanderAdapter.h"

void DMARender::CommanderAdapter::setOverlay(std::shared_ptr<IOverlay> ptr)
{
	this->overlayPtr = ptr;
}

std::shared_ptr<DMARender::IOverlay> DMARender::CommanderAdapter::getOverlay()
{
	return this->overlayPtr;
}
