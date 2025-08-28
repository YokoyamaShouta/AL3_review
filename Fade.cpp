#include "Fade.h"

using namespace KamataEngine;
using namespace std;

void Fade::Init() {
	textureHandle_ = TextureManager::Load("fade/fade.png");
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
	sprite_->SetSize(Vector2(1280,720));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() 
{
	switch (fadeStatus_) 
	{
	case Fade::FadeStatus::NONE:
		break;
	case Fade::FadeStatus::FADEIN:
		fadeCounter_ += 1.0f / 60.0f;

		if (fadeCounter_ >= duration_)	
		{
			fadeCounter_ = duration_;
		}
		sprite_->SetColor(Vector4(0, 0, 0, max(1.0f - fadeCounter_ / duration_, 0.0f)));
		
		break;
	case Fade::FadeStatus::FADEOUT:
		fadeCounter_ += 1.0f / 60.0f;

		if (fadeCounter_ >= duration_)
		{
			fadeCounter_ = duration_;
		}

		sprite_->SetColor(Vector4(0, 0, 0, max(fadeCounter_ / duration_, 0.0f)));

		break;
	}
}

void Fade::FadeStart(FadeStatus status, float duration) 
{
	fadeStatus_ = status;
	duration_ = duration;
	fadeCounter_ = 0.0f;
}

void Fade::FadeStop() 
{
	fadeStatus_ = FadeStatus::NONE; 
}

bool Fade::IsFadeFinishied() const 
{
	switch (fadeStatus_) 
	{
	case Fade::FadeStatus::NONE:
		break;
	case Fade::FadeStatus::FADEIN:
		if (fadeCounter_ >= duration_) 
		{
			return true;
		} 
		else
		{
			return false;
		}

		break;
	case Fade::FadeStatus::FADEOUT:
		if (fadeCounter_ >= duration_) 
		{
			return true;
		}
		else 
		{
			return false;
		}

		break;
	}

	return true;
}


void Fade::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Sprite::PreDraw(dxCommon->GetCommandList());

	if (fadeStatus_ == FadeStatus::NONE)
	{
		return;
	}

	sprite_->Draw();
	Sprite::PostDraw();
}
