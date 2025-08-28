#include "item.h"
#include <numbers>
#include <algorithm>
#include<assert.h>

using namespace KamataEngine;
using namespace MathUtility;
using namespace std;


void Item::Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos) {
	assert(model);
	coinModel_ = model;
	coinCamera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
}

void Item::Update() {
	worldTransform_.MakeAffinMatrix();
	worldTransform_.TransferMatrix();
}

void Item::OnCollision(const Player* player) {
	player;
	IsGet_ = true;
}

const Vector3 Item::GetWorldPosition()
{
	Vector3 worldPos = worldTransform_.translation_;
	return worldPos;
}

void Item::Draw() 
{
	if (!IsGet_)
	{
		coinModel_->Draw(worldTransform_, *coinCamera_);	
	}
}

AABB Item::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb1;
	aabb1.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb1.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb1;
}
