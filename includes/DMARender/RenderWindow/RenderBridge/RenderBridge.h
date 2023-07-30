#pragma once
#include <memory>
#include "../../IOverlay/IOverlay.h"
#include "MapManager/MapManager.h"
#include <d3d11.h>
namespace DMARender {
	class RenderBridge {
		std::shared_ptr<IOverlay> overlayPtr = nullptr;
		std::shared_ptr<MapManager> mapManager;
		ID3D11Device** g_pd3dDevicePtr;
	public:
		RenderBridge(ID3D11Device** g_pd3dDevice);
		void createFonts();
		void setOverlay(std::shared_ptr<IOverlay> ptr);
		std::shared_ptr<IOverlay> getOverlay();
		void addMap(std::shared_ptr<IGameMap> map);
		std::shared_ptr<MapManager> getMapManager();
	};
}