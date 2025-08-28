#include "Skydome.h"
#include <assert.h>

void Skydome::Init(KamataEngine::Model* model, KamataEngine::Camera* camera) 
{
	assert(model);
	model_ = model;
	camera_ = camera;

	worldTrasform_.Initialize();
}

void Skydome::Update() { worldTrasform_.TransferMatrix(); }

void Skydome::Draw() { model_->Draw(worldTrasform_, *camera_); }
