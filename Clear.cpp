#include "Clear.h"

using namespace KamataEngine;

void Clear::Init() {
	clearModel_ = Model::CreateFromOBJ("clear", true);
	spaceModel_ = Model::CreateFromOBJ("space", true);
	
	clearCamera_ = new Camera;
	clearCamera_->Initialize();
	
	worldTransform_.Initialize();
	worldTransformSpace_.Initialize();

	fade_ = new Fade;
	fade_->Init();
	fade_->FadeStart(Fade::FadeStatus::FADEIN, fade_->fadeTimer_);

	clearBGMData_ = Audio::GetInstance()->LoadWave("clear.wav");

	image_ = new Image();
	image_->Init();
}

void Clear::Update() 
{
	switch (phase_) {
	case Clear::Phase::FADEIN:
		fade_->Update();
		
		if (!soundCheck_)
		{
			clearBGM_ = Audio::GetInstance()->PlayWave(clearBGMData_, true);
			soundCheck_ = true;
		}

		if (fade_->IsFadeFinishied())
		{
			phase_ = Phase::MAIN;
		}

		break;
	case Clear::Phase::MAIN:
		if (Input::GetInstance()->PushKey(DIK_SPACE))
		{
			Audio::GetInstance()->StopWave(clearBGM_);
			phase_ = Phase::FADEOUT;
			fade_->FadeStart(Fade::FadeStatus::FADEOUT, fade_->fadeTimer_);
		}

		break;
	case Clear::Phase::FADEOUT:
		fade_->Update();
		if (fade_->IsFadeFinishied())
		{
			finished_ = true;
		}

		break;
	}
	worldTransform_.translation_ = {0, 5, 0};
	worldTransform_.scale_ = {5, 5, 5};
	worldTransform_.rotation_ = {0, 0, 0};

	worldTransformSpace_.translation_ = {0, -5, 0};
	worldTransformSpace_.scale_ = {5, 5, 5};
	worldTransformSpace_.rotation_ = {0, 0, 0};

	worldTransform_.MakeAffinMatrix();
	worldTransform_.TransferMatrix();

	worldTransformSpace_.MakeAffinMatrix();
	worldTransformSpace_.TransferMatrix();
}

void Clear::Draw()
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());

	clearModel_->Draw(worldTransform_, *clearCamera_);
	spaceModel_->Draw(worldTransformSpace_, *clearCamera_);

	image_->Draw();
	fade_->Draw();

	Model::PostDraw();
}

Clear::Clear() {}

Clear::~Clear() 
{
	delete clearCamera_;
	delete clearModel_;
	delete fade_;
}
