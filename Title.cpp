#include "Title.h"

using namespace KamataEngine;

void Title::Init() 
{
	titleModel_ = Model::CreateFromOBJ("title", true);
	spaceModel_ = Model::CreateFromOBJ("space", true);
	titleCamera_ = new Camera;
	titleCamera_->Initialize();

	worldTransform_.Initialize();
	worldTransformSpace_.Initialize();

	fade_ = new Fade();
	fade_->Init();
	fade_->FadeStart(Fade::FadeStatus::FADEIN, fade_->fadeTimer_);

	image_ = new Image();
	image_->Init();

	titleBGMData_ = Audio::GetInstance()->LoadWave("title.wav");
	titleBGM_ = Audio::GetInstance()->PlayWave(titleBGMData_, true);

	jumpSoundData_ = Audio::GetInstance()->LoadWave("se_jump_001 (1).wav");	
}

void Title::Update() 
{
	switch (phase_) {
	case Title::Phase::FADEIN:
		
		fade_->Update();
		
		if (fade_->IsFadeFinishied())
		{
			phase_ = Phase::MAIN;
		}

		break;
	case Title::Phase::MAIN:
	
		if (Input::GetInstance()->PushKey(DIK_SPACE))
		{
			Audio::GetInstance()->StopWave(titleBGM_);
			jumpSound_ = Audio::GetInstance()->PlayWave(jumpSoundData_, false);
			phase_ = Phase::FADEOUT;
			fade_->FadeStart(Fade::FadeStatus::FADEOUT, fade_->fadeTimer_);
		}

		break;
	case Title::Phase::FADEOUT:
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

void Title::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());

	titleModel_->Draw(worldTransform_, *titleCamera_);
	spaceModel_->Draw(worldTransformSpace_, *titleCamera_);

	image_->Draw();

	fade_->Draw();

	Model::PostDraw();
}

Title::Title() {}

Title::~Title() 
{
	delete titleCamera_;
	delete titleModel_;
	delete fade_;
}
