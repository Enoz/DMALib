#pragma once
#include "../IGameMap/IGameMap.h"
#include "../RenderWindow/MapTransform.h"
#include "imgui.h"
namespace DMARender {
	class IRadar {
	protected:
		ImVec2 WorldToRadar(IGameMap* curMap, const MapTransform& mTransform, Vector3 gamePos);
	public:
		virtual void DrawOverlay(IGameMap* curMap, const MapTransform& mTransform) = 0;
		virtual void createFonts() {};

	};
}