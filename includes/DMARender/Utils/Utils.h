#pragma once
#include <d3dtypes.h>
#include "../Vectors/Vector3.h"
#include "../Vectors/Vector2.h"
namespace DMARender {
	bool WorldToScreen(const _D3DMATRIX &view_matrix, const DMARender::Vector3& position, DMARender::Vector2& outScreenPos, const float& renderWidth, const float& renderHeight);
}