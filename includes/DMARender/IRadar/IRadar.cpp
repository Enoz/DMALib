#include "IRadar.h"

bool DMARender::IRadar::WorldToRadar(IGameMap* curMap, const MapTransform& mTransform, Vector3 gamePos, ImVec2& WindowPos)
{
    //Get point on image
    ImVec2 imagePos = curMap->translatePoint(gamePos);

    //Apply zoom + offset transformations
    imagePos.x *= mTransform.mapZoom;
    imagePos.y *= mTransform.mapZoom;

    imagePos.x += mTransform.dragOffsetX;
    imagePos.y += mTransform.dragOffsetY;

    WindowPos.x = imagePos.x;
    WindowPos.y = imagePos.y;
    return true;
}
