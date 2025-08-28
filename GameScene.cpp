#include "GameScene.h"

using namespace KamataEngine;
using namespace MathUtility;

void GameScene::Init() 
{
	worldTransform_.Initialize();
	camera_.Initialize();

	//	fade
	fade_ = new Fade();
	fade_->Init();
	fade_->FadeStart(Fade::FadeStatus::FADEIN, fade_->fadeTimer_);

	//	skydome
	skydome_ = new Skydome();
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Init(skydomeModel_, &camera_);

	//	mapChip
	mapChipField_ = new MapChipField();
	mapChipField_->LoadmapChipCsv("mapChip/block.csv");

	blockModel_ = Model::CreateFromOBJ("block", true);
	trapModel_ = Model::CreateFromOBJ("TrapBlock", true);
	coinModel_ = Model::CreateFromOBJ("Coin", true);

	GenerateBlocks();

	//	player
	player_ = new Player();
	playerModel_ = Model::CreateFromOBJ("player",true);
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 17);
	player_->Initialize(playerModel_, &camera_, playerPosition);

	// プレイヤーのカメラ生成
	playerTargetCamera_ = new CameraContoroller();
	playerTargetCamera_->Initialize();
	playerTargetCamera_->SetTarget(player_);
	playerTargetCamera_->Reset();

	player_->SetMapChipField(mapChipField_);

	coin1_ = new Item();
	Vector3 coin1Pos = mapChipField_->GetMapChipPositionByIndex(1, 1);
	coin1_->Init(coinModel_,&camera_,coin1Pos);

	coin2_ = new Item();
	Vector3 coin2Pos = mapChipField_->GetMapChipPositionByIndex(37, 1);
	coin2_->Init(coinModel_, &camera_, coin2Pos);

	// coin sound
	coinSoundData_ = Audio::GetInstance()->LoadWave("coin.wav");
	gameBGMData_ = Audio::GetInstance()->LoadWave("game.wav");

	phase_ = Phase::KFADEIN;
}

void GameScene::Update()
{
	switch (phase_) 
	{
	case GameScene::Phase::KFADEIN:
		fade_->Update();

		if (!bgmCheck_)
		{
			gameBGM_ = Audio::GetInstance()->PlayWave(gameBGMData_,true);
			bgmCheck_ = true;
		}

		if (fade_->IsFadeFinishied())
		{
			phase_ = Phase::KPLAY;
		}
		break;
	case GameScene::Phase::KPLAY:
		if (coin1_->GetIsItemGet()&& coin2_->GetIsItemGet())
		{
			phase_ = Phase::KFEADEOUT;
			fade_->FadeStart(Fade::FadeStatus::FADEOUT, fade_->fadeTimer_);
		}

		break;
	case GameScene::Phase::KDEATH:
		break;
	case GameScene::Phase::KFEADEOUT:
		fade_->Update();

		if (fade_->IsFadeFinishied())
		{
			Audio::GetInstance()->StopWave(gameBGM_);
			finished_ = true;
		}
		break;
	}
	player_->Update();
	
	coin1_->Update();
	coin2_->Update();

	// block Update
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformBlock->scale_);
			Matrix4x4 rotateMatrixX = MakeRotateXMatrix(worldTransformBlock->rotation_.x);
			Matrix4x4 rotateMatrixY = MakeRotateYMatrix(worldTransformBlock->rotation_.y);
			Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(worldTransformBlock->rotation_.z);
			Matrix4x4 TransformMatrix = MakeTranslateMatrix(worldTransformBlock->translation_);

			worldTransformBlock->matWorld_ = scaleMatrix * (rotateMatrixX * rotateMatrixY * rotateMatrixZ) * TransformMatrix;
			worldTransformBlock->TransferMatrix();
		}
	}
	
	//	trap Update
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformTrap_) {
		for (WorldTransform* worldTransformTrap : worldTransformBlockLine) {
			if (!worldTransformTrap) {
				continue;
			}
			Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformTrap->scale_);
			Matrix4x4 rotateMatrixX = MakeRotateXMatrix(worldTransformTrap->rotation_.x);
			Matrix4x4 rotateMatrixY = MakeRotateYMatrix(worldTransformTrap->rotation_.y);
			Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(worldTransformTrap->rotation_.z);
			Matrix4x4 TransformMatrix = MakeTranslateMatrix(worldTransformTrap->translation_);

			worldTransformTrap->matWorld_ = scaleMatrix * (rotateMatrixX * rotateMatrixY * rotateMatrixZ) * TransformMatrix;
			worldTransformTrap->TransferMatrix();
		}
	}

	//	coin Update
	/*for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformCoin_)
	{
		for (WorldTransform* worldTransformCoin : worldTransformBlockLine) 
		{
			if (!worldTransformCoin) 
			{
				continue;
			}
			Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformCoin->scale_);
			Matrix4x4 rotateMatrixX = MakeRotateXMatrix(worldTransformCoin->rotation_.x);
			Matrix4x4 rotateMatrixY = MakeRotateYMatrix(worldTransformCoin->rotation_.y);
			Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(worldTransformCoin->rotation_.z);
			Matrix4x4 TransformMatrix = MakeTranslateMatrix(worldTransformCoin->translation_);

			worldTransformCoin->matWorld_ = scaleMatrix * (rotateMatrixX * rotateMatrixY * rotateMatrixZ) * TransformMatrix;
			worldTransformCoin->TransferMatrix();
		}
	}*/

	playerTargetCamera_->Update();
	camera_.matView = playerTargetCamera_->GetPlayerCamera().matView;
	camera_.matProjection = playerTargetCamera_->GetPlayerCamera().matProjection;
	camera_.TransferMatrix();

	CheckAllCollisions();
}

void GameScene::CheckAllCollisions() 
{
	AABB aabbPlayer,aabbCoin1,aabbCoin2;
	aabbPlayer = player_->GetAABB();
	aabbCoin1 = coin1_->GetAABB();
	aabbCoin2 = coin2_->GetAABB();

	if (aabbPlayer.IsAABBHitjudge(&aabbCoin1))
	{
		coin1_->OnCollision(player_);
		coinSound_ = Audio::GetInstance()->PlayWave(coinSoundData_, false);
	}
	
	if (aabbPlayer.IsAABBHitjudge(&aabbCoin2)) {
		coin2_->OnCollision(player_);
		coinSound_ = Audio::GetInstance()->PlayWave(coinSoundData_, false);
	} 
}

void GameScene::GenerateBlocks() 
{
	/*for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformTrap_) {
		for (WorldTransform* worldTransformTrap : worldTransformBlockLine) {
			delete worldTransformTrap;
		}
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformCoin_) {
		for (WorldTransform* worldTransformCoin : worldTransformBlockLine) {
			delete worldTransformCoin;
		}
	}

	worldTransformBlocks_.clear();*/

	// 縦の生成
	worldTransformBlocks_.resize(mapChipField_->GetNumBlockVirtical());
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVirtical(); ++i)
	{
		worldTransformBlocks_[i].resize(mapChipField_->GetNumBlockHorizontal());
	}

	//trap
	worldTransformTrap_.resize(mapChipField_->GetNumBlockVirtical());
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVirtical(); ++i) 
	{
		worldTransformTrap_[i].resize(mapChipField_->GetNumBlockHorizontal());
	}

	//	coin
	/*worldTransformCoin_.resize(mapChipField_->GetNumBlockVirtical());
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVirtical(); ++i) {
		worldTransformCoin_[i].resize(mapChipField_->GetNumBlockHorizontal());
	}*/

	//	block
	for (uint32_t y = 0; y < mapChipField_->GetNumBlockVirtical(); y++) {
		for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); x++) 
		{
			// 縦と横がブロックだったら
			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipField::MapChipType::BLOCK) 
			{
				// 描画する
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[y][x] = worldTransform;
				worldTransformBlocks_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
			}
		}
	}

	//	trap
	for (uint32_t y = 0; y < mapChipField_->GetNumBlockVirtical(); y++) {
		for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); x++) {
			// 縦と横がトラップだったら
			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipField::MapChipType::TRAP) {
				// 描画する
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformTrap_[y][x] = worldTransform;
				worldTransformTrap_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
			}
		}
	}

	//	coin
	//for (uint32_t y = 0; y < mapChipField_->GetNumBlockVirtical(); y++) {
	//	for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); x++) {
	//		// 縦と横がトラップだったら
	//		if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipField::MapChipType::COIN) {
	//			// 描画する
	//			WorldTransform* worldTransform = new WorldTransform();
	//			worldTransform->Initialize();
	//			worldTransformCoin_[y][x] = worldTransform;
	//			worldTransformCoin_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
	//		}
	//	}
	//}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3D model front
	Model::PreDraw(dxCommon->GetCommandList());
	
	skydome_->Draw();

	player_->Draw();

	coin1_->Draw();
	coin2_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			if (!worldTransformBlock) 
			{
				continue;
			}
			blockModel_->Draw(*worldTransformBlock, camera_);
		}
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformTrap_)
	{
		for (WorldTransform* worldTransformTrap : worldTransformBlockLine) 
		{
			if (!worldTransformTrap) 
			{
				continue;
			}
			trapModel_->Draw(*worldTransformTrap, camera_);
		}
	}

	//for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformCoin_) {
	//	for (WorldTransform* worldTransformCoin : worldTransformBlockLine) {
	//		if (!worldTransformCoin) {
	//			continue;
	//		}
	//		coinModel_->Draw(*worldTransformCoin, camera_);
	//	}
	//}

	fade_->Draw();
	
	Model::PostDraw();
}

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete skydomeModel_;
	delete skydome_;
	delete fade_;
	delete player_;
	delete playerModel_;
	delete playerTargetCamera_;
	delete blockModel_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}
