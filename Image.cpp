#include "Image.h"

using namespace KamataEngine;
using namespace std;

void Image::Init()  {
	textureHandle_ = TextureManager::Load("title.png");
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
	sprite_->SetSize(Vector2(1280, 720));
	sprite_->SetColor(Vector4(1, 1, 1, 0.2f));
}

void Image::Update() {}



void Image::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Sprite::PreDraw(dxCommon->GetCommandList());

	sprite_->Draw();
	Sprite::PostDraw();
}
