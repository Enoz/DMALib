#include "IRadar.h"

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
