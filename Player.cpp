#define NOMINMAX
#include "Player.h"
#include "GameScene.h"
#include "MapChipField.h"
#include <algorithm>
#include <numbers>
#include<assert.h>

using namespace KamataEngine;
using namespace MathUtility;
using namespace std;

void Player::Initialize(Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	assert(camera);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = numbers::pi_v<float> / 2.0f;
	jumpSoundData_ = Audio::GetInstance()->LoadWave("se_jump_001 (1).wav");
}

void Player::Update()
{
	Player::Move(); //	1

	// 衝突情報の初期化
	CollisionMapInfo collisionMapChipInfo;

	// 移動量に速度の値をコピー
	collisionMapChipInfo.moveMent = velocity_;

	// 衝突判定
	HitJudgeMapChip(collisionMapChipInfo); //	2

	// 判定結果を反映
	HitJudgeMove(collisionMapChipInfo); //	3

	// 壁に接触している場合の処理
	HitTop(collisionMapChipInfo); //	4
	HitGround(collisionMapChipInfo);
	HitWall(collisionMapChipInfo);

	// 旋回制御 プレイヤーの振り向き
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {numbers::pi_v<float> / 2.0f, numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = lerp(turnFirstRotationY_, destinationRotationY, 1 - turnTimer_ / kTimeTurn);
	}

	if (hp_ == 0)
	{
		isDead_ = true;
	}

	//	行列更新
	worldTransform_.MakeAffinMatrix();
	worldTransform_.TransferMatrix();
}

void Player::Move() {
	bool landing = false;

	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 1.5f) {
			landing = true;
		}
	}

	//	地面にいる
	if (onGround_) {
		//	move
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {
			//	左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {
				if (lrDirection_ != LRDirection::kRIGHT) {
					lrDirection_ = LRDirection::kRIGHT;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

				// 左キーを押していた時
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {
				if (lrDirection_ != LRDirection::kLEFT) {
					lrDirection_ = LRDirection::kLEFT;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

				// 右キーを押していた時
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;
			}

			// 加速　減速
			velocity_.x += acceleration.x;
			// プレイヤーの速さ制限
			velocity_.x = clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else // 地面にいないとき
		{
			velocity_.x *= (1.0f - kAttenuation);
		}

		//	jump
		if (Input::GetInstance()->PushKey(DIK_SPACE) || Input::GetInstance()->PushKey(DIK_UP)) 
		{						
			jumpSound_ = Audio::GetInstance()->PlayWave(jumpSoundData_,false);
			velocity_.y += kJumpAcceleration;
			onGround_ = false;
		}

	} else // 着地していないとき
	{
		velocity_.y += -kGravityAcceleration;

	}
}

void Player::HitJudgeMapChip(CollisionMapInfo& info) {
	Player::HitJudgeTop(info);    //	天井との当たり
	Player::HitJudgeGround(info); //	地面との当たり
	Player::HitJudgeRight(info);  //	右壁との当たり
	Player::HitJudgeLeft(info);   //	左壁との当たり
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //	rightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //	leftBotoom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //	rightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //	leftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

//	天井との当たり判定
void Player::HitJudgeTop(CollisionMapInfo& info) {
	if (info.moveMent.y <= 0) {
		return;
	}

	array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	MapChipField::MapChipType mapChipType;
	MapChipField::MapChipType mapChipTypeNext;

	bool isHit = false;

	MapChipField::IndexSet indexSet;

	// 左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY - 1);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) {
		isHit = true;
	} 
	
	/*if (mapChipType == MapChipField::MapChipType::COIN) 
	{
		mapChipField_->SetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY, MapChipField::MapChipType::BLANK);
		gameScene_->GenerateBlocks();
	}*/
	
	// 右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY - 1);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) {
		isHit = true;
	}

	//if (mapChipType == MapChipField::MapChipType::COIN) 
	//{
	//	mapChipField_->SetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY, MapChipField::MapChipType::BLANK);
	//	gameScene_->GenerateBlocks();
	//}

	if (isHit) {
		// めりこみ削除
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.moveMent) + Vector3(0, kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, kHeight / 2.0f, 0));
		if (indexSetNow.indexY != indexSet.indexY) {
			// めり込み先ブロックの範囲
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.indexX, indexSet.indexY);
			info.moveMent.y = max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight / 2.0f - kBlank));
			info.isHitTop = true;
		}
	}
}

//	地面との当たり判定
void Player::HitJudgeGround(CollisionMapInfo& info) {
	if (info.moveMent.y >= 0) {
		return;
	}

	array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	MapChipField::MapChipType mapChipType;
	MapChipField::MapChipType mapChipTypeNext;

	bool isHit = false;

	MapChipField::IndexSet indexSet;

	//	左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY - 1);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) {
		isHit = true;
	}

	//	右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY - 1);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) {
		isHit = true;
	}

	if (isHit) {
		// めりこみ削除
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.moveMent) - Vector3(0, kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ - Vector3(0, kHeight / 2.0f, 0));
		if (indexSetNow.indexY != indexSet.indexY) {
			// めり込み先ブロックの範囲
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.indexX, indexSet.indexY);
			info.moveMent.y = min(0.0f, (rect.top - worldTransform_.translation_.y) + (kHeight / 2.0f + kBlank));
			info.isHitGround = true;
		}
	}
}

//	右の壁との当たり判定
void Player::HitJudgeRight(CollisionMapInfo& info) {
	if (info.moveMent.x <= 0) {
		return;
	}

	array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	MapChipField::MapChipType mapChipType;
	MapChipField::MapChipType mapChipTypeNext;

	bool isHit = false;

	MapChipField::IndexSet indexSet;

	//	右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX - 1, indexSet.indexY);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) {
		isHit = true;
	}
	
	/*if (mapChipType == MapChipField::MapChipType::COIN) 
	{
		IsCoinGet_ = true;
		mapChipField_->SetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY, MapChipField::MapChipType::BLANK);
	}*/
	
	//	右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX - 1, indexSet.indexY);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) {
		isHit = true;
	}

	/*if (mapChipType == MapChipField::MapChipType::COIN) 
	{
		IsCoinGet_ = true;
		mapChipField_->SetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY, MapChipField::MapChipType::BLANK);
	}*/

	if (isHit) {
		// めりこみ削除
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.moveMent) + Vector3(kWidth / 2.0f, 0, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(kWidth / 2.0f, 0, 0));
		if (indexSetNow.indexX != indexSet.indexX) {
			// めり込み先ブロックの範囲
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.indexX, indexSet.indexY);
			info.moveMent.x = min(0.0f, (rect.left - worldTransform_.translation_.x) + (kWidth / 2.0f + kBlank));
			info.isHitWall = true;
		}
	}
}

//	左の壁
void Player::HitJudgeLeft(CollisionMapInfo& info) {
	if (info.moveMent.x >= 0) {
		return;
	}

	array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	MapChipField::MapChipType mapChipType;
	MapChipField::MapChipType mapChipTypeNext;

	bool isHit = false;

	MapChipField::IndexSet indexSet;

	//	左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX + 1, indexSet.indexY);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) 
	{
		isHit = true;
	}
	
	/*if (mapChipType == MapChipField::MapChipType::COIN) 
	{
		IsCoinGet_ = true;
		mapChipField_->SetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY, MapChipField::MapChipType::BLANK);
	}*/
	
	//	左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX + 1, indexSet.indexY);
	if (mapChipType != MapChipField::MapChipType::BLANK && mapChipTypeNext == MapChipField::MapChipType::BLANK) {
		isHit = true;
	}

	/*if (mapChipType == MapChipField::MapChipType::COIN) 
	{
		IsCoinGet_ = true;
		mapChipField_->SetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY, MapChipField::MapChipType::BLANK);
	}*/

	if (isHit) {
		// めりこみ削除
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.moveMent) - Vector3(kWidth / 2.0f, 0, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));

		if (indexSetNow.indexX != indexSet.indexX) {
			// めり込み先ブロックの範囲
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.indexX, indexSet.indexY);
			info.moveMent.x = min(0.0f, (rect.right - worldTransform_.translation_.x) + (kWidth / 2.0f + kBlank));
			info.isHitWall = true;
		}
	}
}

void Player::HitJudgeMove(const CollisionMapInfo& info) { worldTransform_.translation_ += info.moveMent; }

//	地面にあったていたら
void Player::HitGround(const CollisionMapInfo& info) {
	if (onGround_) {
		//	接地状態
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}

		array<Vector3, kNumCorner> positionsNew;
		for (uint32_t i = 0; i < positionsNew.size(); i++) {
			positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
		}

		// MapChipType mapChipType;

		bool isHit = false;

		MapChipField::IndexSet indexSet;
		MapChipField::MapChipType mapChipType;

		//	左下
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
		if (mapChipType == MapChipField::MapChipType::BLOCK) 
		{
			isHit = true;
		}

		//	右下
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.indexX, indexSet.indexY);
		if (mapChipType == MapChipField::MapChipType::BLOCK) 
		{
			isHit = true;
		}

		if (!isHit) {
			onGround_ = false;
		}
	} else {
		//	空中状態
		if (info.isHitGround) {
			onGround_ = true;

			//	着地時にx速度の減衰
			velocity_.x *= (1.0f - kAttenuationLanding);

			//	y速度の変更
			velocity_.y = 0.0f;
		}
	}
}

void Player::HitTop(const CollisionMapInfo& info) {
	if (info.isHitTop) {
		velocity_.y = 0;
	}
}

void Player::HitWall(const CollisionMapInfo& info) {
	if (info.isHitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

const KamataEngine::Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb1;
	aabb1.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb1.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb1;
}

void Player::OnCollision(const Item* item) 
{
	item;
}

void Player::Draw() {
	if (!isDead_) {
		model_->Draw(worldTransform_, *camera_);
	}
}

Player::Player() {}

Player::~Player()
{ }