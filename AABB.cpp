#include "AABB.h"

bool AABB::IsAABBHitjudge(const AABB* other) 
{
	return (max.x > other->min.x && 
			min.x < other->max.x && 
			max.y > other->min.y && 
			min.y < other->max.y && 
			max.z > other->min.z && 
			min.z < other->max.z); 
}