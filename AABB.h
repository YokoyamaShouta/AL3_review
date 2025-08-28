#pragma once
#pragma once
#include "KamataEngine.h"

class AABB 
{
public:
	bool IsAABBHitjudge(const AABB* other);

	KamataEngine::Vector3 max, min;
};
