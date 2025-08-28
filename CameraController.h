#pragma once
#include"KamataEngine.h"

class Player;

class CameraContoroller {
public:
	struct Rect 
	{
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void Initialize();

	void Update();

	// get point
	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	void SetMoveableArea(Rect& area) { moveableArea_ = area; }

	const KamataEngine::Camera& GetPlayerCamera() const { return playerCamera_; }

	static inline const float kInterpolationRate = 0.2f;

	static inline const float kVelocityBias = 2.0f;

	// 目標座標
	KamataEngine::Vector3 targetCoordinates_;

private:
	KamataEngine::Camera playerCamera_;

	// 上で宣言したもの
	Player* target_ = nullptr;
	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};

	//	カメラの移動制限
	Rect moveableArea_ = {9.1f, 30.0f, 4.4f, 30};

	KamataEngine::Vector3 targetVelocity_ = {1.5f, 1.5f, 1.5f};
};