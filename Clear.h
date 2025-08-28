#pragma once
#include"KamataEngine.h"
#include"Fade.h"
#include"Image.h"

class Clear
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
	void Draw();

	Clear();
	~Clear();

	bool IsFinished() const { return finished_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformSpace_;
	KamataEngine::Model* clearModel_ = nullptr;
	KamataEngine::Model* spaceModel_ = nullptr;
	KamataEngine::Camera* clearCamera_ = nullptr;

	Fade* fade_ = nullptr;

	Image* image_ = nullptr;

	bool finished_ = false;

	Phase phase_ = Phase::FADEIN;

	uint32_t clearBGMData_ = 0;

	uint32_t clearBGM_ = 0;

	bool soundCheck_ = false;
};