#pragma once
#include "../IGameMap/IGameMap.h"
#include "../RenderWindow/MapTransform.h"
#include "imgui.h"
#include <vector>
namespace DMARender {
	class IRadar {
	protected:
		ImVec2 WorldToRadar(IGameMap* curMap, const MapTransform& mTransform, Vector3 gamePos);
		void drawBlip(const ImVec2& screenPos, const float& size, const ImU32& color, const std::vector<std::string>& text = {});
	public:
		virtual void DrawOverlay(IGameMap* curMap, const MapTransform& mTransform) = 0;
		virtual void createFonts() {};

	};
}