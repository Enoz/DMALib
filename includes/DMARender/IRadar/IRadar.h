#pragma once
#include "../IGameMap/IGameMap.h"
#include "../RenderWindow/MapTransform.h"
#include "imgui.h"
namespace DMARender {
	class IRadar {
	protected:
		bool WorldToRadar(IGameMap* curMap, const MapTransform& mTransform, Vector3 gamePos, ImVec2& WindowPos);
	public:
		virtual void DrawOverlay(IGameMap* curMap, const MapTransform& mTransform) = 0;
		virtual void createFonts() {};

	};
}