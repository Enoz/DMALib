#include "RenderBridge.h"

DMARender::RenderBridge::RenderBridge(ID3D11Device** g_pd3dDevice)
{
	this->g_pd3dDevicePtr = g_pd3dDevice;
	this->mapManager = std::shared_ptr<MapManager>(new MapManager(g_pd3dDevice));
}

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

void DMARender::RenderBridge::addMap(std::shared_ptr<IGameMap> map)
{
	this->mapManager->addMap(map);
}

std::shared_ptr<DMARender::MapManager> DMARender::RenderBridge::getMapManager()
{
	return mapManager;
}
