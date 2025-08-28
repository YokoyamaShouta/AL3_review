#include "3d/WorldTransform.h"
#include "math/MathUtility.h"

using namespace KamataEngine;
using namespace MathUtility;

void WorldTransform::MakeAffinMatrix() 
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale_);
	Matrix4x4 rotateMatrixX = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 rotateMatrixY = MakeRotateYMatrix(rotation_.y);
	Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(rotation_.z);
	Matrix4x4 TransformMatrix = MakeTranslateMatrix(translation_);

	matWorld_ = scaleMatrix * (rotateMatrixX * rotateMatrixY * rotateMatrixZ) * TransformMatrix;
}