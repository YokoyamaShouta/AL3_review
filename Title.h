#pragma once
#include"KamataEngine.h"
#include"Fade.h"
#include"Image.h"

class Title
{
public:
	enum class Phase
	{
		FADEIN,
		MAIN,
		FADEOUT,
	};

	void Init();

	void Update();
	bool IsFinished() const { return finished_; }

	void Draw();

	Title();
	~Title();

private:
	KamataEngine::WorldTransform worldTransform_ = {};
	KamataEngine::WorldTransform worldTransformSpace_ = {};

	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::Model* spaceModel_ = nullptr;

	KamataEngine::Camera* titleCamera_ = nullptr;
	
	//	sound Data
	uint32_t titleBGMData_ = 0;
	uint32_t jumpSoundData_ = 0;

	// sound
	uint32_t titleBGM_ = 0;
	uint32_t jumpSound_ = 0;

	Fade* fade_ = nullptr;
	Image* image_ = nullptr;

	Phase phase_ = Phase::FADEIN;	

	bool finished_ = false;
};