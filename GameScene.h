#pragma once
#include"KamataEngine.h"
#include"Fade.h"
#include"Skydome.h"
#include"MapChipField.h"
#include"Player.h"
#include"CameraController.h"
#include"item.h"

class GameScene
{
public:
	enum class Phase
	{
		KFADEIN,
		KPLAY,
		KDEATH,
		KFEADEOUT,
	};

	Phase phase_;

	void Init();
	
	void Update();	
	//	あたり判定
	void CheckAllCollisions();
	
	bool IsFinished() const { return finished_; }

	void GenerateBlocks();

	const std::vector<std::vector<KamataEngine::WorldTransform*>>& GetWorldTransformCoin() const { return worldTransformCoin_; }

	void Draw();
	
	GameScene();
	~GameScene();

private:
	
	//	fade
	Fade* fade_ = nullptr;
	bool finished_ = false;

	//	worldTransform
	KamataEngine::WorldTransform worldTransform_;

	//	camera
	KamataEngine::Camera camera_;
	CameraContoroller* playerTargetCamera_ = nullptr;

	//	skydome
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* skydomeModel_ = nullptr;

	//	mapChip
	MapChipField* mapChipField_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformTrap_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformCoin_;
	KamataEngine::Model* blockModel_ = nullptr;
	KamataEngine::Model* trapModel_ = nullptr;
	KamataEngine::Model* coinModel_ = nullptr;

	//	player
	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;

	//	item
	Item* coin1_ = nullptr;
	Item* coin2_ = nullptr;

	//	sound data
	uint32_t coinSoundData_ = 0;
	uint32_t gameBGMData_ = 0;

	uint32_t coinSound_ = 0;
	uint32_t gameBGM_ = 0;

	bool bgmCheck_ = false;
};