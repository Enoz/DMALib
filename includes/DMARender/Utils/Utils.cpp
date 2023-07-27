#include "Utils.h"

bool DMARender::WorldToScreen(const _D3DMATRIX& view_matrix, const DMARender::Vector3& position, DMARender::Vector2& outScreenPos, const float& renderWidth, const float& renderHeight)
{
	DMARender::Vector3 transform = { view_matrix._14, view_matrix._24, view_matrix._34 };
	DMARender::Vector3 right = { view_matrix._11, view_matrix._21, view_matrix._31 };
	DMARender::Vector3 up = { view_matrix._12, view_matrix._22, view_matrix._32 };

	float w = transform.Dot(position) + view_matrix._44;

	if (w < 0.099f)
		return false;

	float x = right.Dot(position) + view_matrix._41;
	float y = up.Dot(position) + view_matrix._42;

	outScreenPos.x = (renderWidth / 2) * (1.f + x / w);
	outScreenPos.y = (renderHeight / 2) * (1.f - y / w);

	return true;
}