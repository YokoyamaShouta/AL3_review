#pragma once

#pragma once
#include "KamataEngine.h"
#include"AABB.h"

// 前方宣言
class MapChipField;
class Item;

enum class LRDirection {
	kRIGHT,
	kLEFT,
};

struct CollisionMapInfo {
	bool isHitTop = false;    // 天井との当たり
	bool isHitGround = false; // 着地
	bool isHitWall = false;   // 壁との当たり判定

	// 移動量
	KamataEngine::Vector3 moveMent;
};

enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();
	void Move(); // プレイヤーの動き

	// マップ衝突判定
	void HitJudgeMapChip(CollisionMapInfo& info);

	void HitJudgeTop(CollisionMapInfo& info);
	void HitJudgeGround(CollisionMapInfo& info);
	void HitJudgeRight(CollisionMapInfo& info);
	void HitJudgeLeft(CollisionMapInfo& info);

	//	もしあたっていたら
	void HitTop(const CollisionMapInfo& info);
	// 接地状態の切り替え
	void HitGround(const CollisionMapInfo& info);
	//	もし壁にあったたら
	void HitWall(const CollisionMapInfo& info);

	// 判定結果で移動
	void HitJudgeMove(const CollisionMapInfo& info);

	void OnCollision(const Item* item);

	//	デスフラグのゲッター
	bool IsDead() const { return isDead_; }

	bool IsGetCoin() const { return IsCoinGet_; }

	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	//	AABBの取得
	AABB GetAABB();

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	Player();
	~Player();

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	//	プレイヤーのポジションを受け取る
	const KamataEngine::Vector3 GetWorldPosition();

	static inline const float kAcceleration = 0.02f;
	static inline const float kAttenuation = 0.07f;
	static inline const float kLimitRunSpeed = 0.1f;

	static inline const float kTimeTurn = 0.3f;
	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 5.0f;
	static inline const float kJumpAcceleration = 0.7f;

	//	プレイヤーの当たり判定
	static inline const float kWidth = 0.8f;  // 横
	static inline const float kHeight = 0.8f; // 縦
	static inline const float kBlank = 0.1f;  // 隙間

	// 着地時の速度減速率
	static inline const float kAttenuationLanding = 0.01f;

	static inline const float kAttenuationWall = 0.1f;

private:
	KamataEngine::WorldTransform worldTransform_ = {};

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	// 接地状態フラグ
	bool onGround_ = true;

	//	デスフラグ
	bool isDead_ = false;

	bool IsCoinGet_ = false;
	
	int hp_ = 5;

	LRDirection lrDirection_ = LRDirection::kRIGHT;

	// マップチップのフィールド
	MapChipField* mapChipField_ = nullptr;

	//	sound  Data
	uint32_t jumpSoundData_ = 0;

	uint32_t jumpSound_ = 0;
};
