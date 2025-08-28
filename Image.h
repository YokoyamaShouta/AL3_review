#pragma once
#include"KamataEngine.h"

class Image
{
public:
	void Init();
	void Update();
	void Draw();

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t textureHandle_ = 0;
};