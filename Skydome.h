#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Skydome
{
public:
	void Init(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

private:
	KamataEngine::WorldTransform worldTrasform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
};