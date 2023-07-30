#include "IRadar.h"
#include <vector>
ImVec2 DMARender::IRadar::WorldToRadar(IGameMap* curMap, const MapTransform& mTransform, Vector3 gamePos)
{
    //Get point on image
    ImVec2 imagePos = curMap->translatePoint(gamePos);

    //Apply zoom + offset transformations
    imagePos.x *= mTransform.mapZoom;
    imagePos.y *= mTransform.mapZoom;

    imagePos.x += mTransform.dragOffsetX;
    imagePos.y += mTransform.dragOffsetY;

    return imagePos;
}

void DMARender::IRadar::drawBlip(const ImVec2& screenPos, const float& size, const ImU32& color, const std::vector<std::string>& text)
{
    ImDrawList* bgDraw = ImGui::GetBackgroundDrawList();
    bgDraw->AddCircleFilled(screenPos, size, IM_COL32_BLACK, 16);
    bgDraw->AddCircleFilled(screenPos, size - 1, color, 16);
}
