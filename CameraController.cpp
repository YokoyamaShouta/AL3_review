#define NOMINMAX

#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <cmath>

using namespace KamataEngine;
using namespace MathUtility;
using namespace std;

void CameraContoroller::Initialize() { playerCamera_.Initialize(); }

void CameraContoroller::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 追従対象とオフセットからのカメラの目標座標の計算
	targetCoordinates_ = targetWorldTransform.translation_ + targetOffset_;

	// 座標保管の追従
	playerCamera_.translation_.x = Lerp(playerCamera_.translation_.x, targetCoordinates_.x, kInterpolationRate);
	playerCamera_.translation_.y = Lerp(playerCamera_.translation_.y, targetCoordinates_.y, kInterpolationRate);
	playerCamera_.translation_.z = Lerp(playerCamera_.translation_.z, targetCoordinates_.z, kInterpolationRate);

	// 移動制限
	playerCamera_.translation_.x = min(playerCamera_.translation_.x, moveableArea_.right);
	playerCamera_.translation_.x = max(playerCamera_.translation_.x, moveableArea_.left);
	playerCamera_.translation_.y = min(playerCamera_.translation_.y, moveableArea_.top);
	playerCamera_.translation_.y = max(playerCamera_.translation_.y, moveableArea_.bottom);

	// 行列の更新
	playerCamera_.UpdateMatrix();
}

void CameraContoroller::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	playerCamera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}
