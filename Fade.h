#pragma once
#include"KamataEngine.h"

class Fade
{
public:
	//	fadeの状態
	enum class FadeStatus
	{
		NONE,
		FADEIN,
		FADEOUT,
	};

	void Init();
	
	void Update();
	void FadeStart(FadeStatus status, float duration);
	void FadeStop();
	static inline const float fadeTimer_ = 0.5f;

	void Draw();

	bool IsFadeFinishied() const;

private:
	KamataEngine::Sprite* sprite_ = nullptr;
	
	uint32_t textureHandle_ = 0;

	FadeStatus fadeStatus_ = FadeStatus::NONE;

	bool finished_ = false;
	float duration_ = 0.0f;
	float fadeCounter_ = 0.0f;
};