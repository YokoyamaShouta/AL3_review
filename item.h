#pragma once
#include"KamataEngine.h"
#include"AABB.h"

class Player;
class MapChipField;

class Item
{
public:
	void Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);
	void Update();
	void OnCollision(const Player* player);
	bool GetIsItemGet() const { return IsGet_; }
	void Draw();

	const KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();
	static inline const float kWidth = 0.8f;  // 横
	static inline const float kHeight = 0.8f; // 縦

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* coinModel_ = nullptr;
	KamataEngine::Camera* coinCamera_ = nullptr;
	MapChipField* mapChipFiled_ = nullptr;

	bool IsGet_ = false;
};