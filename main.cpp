#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "Title.h"
#include "Clear.h"

using namespace KamataEngine;

GameScene* gameScene = new GameScene();
Title* titleScene = new Title();
Clear* clearScene = new Clear();

enum class Scene 
{
	KUNKNOWN,
	TITLE,
	GAME,
	CLEAR,
};

Scene scene;

void ChangeScene()
{
	switch (scene) 
	{
	case Scene::KUNKNOWN:
		break;
	case Scene::TITLE:
		if (titleScene->IsFinished())
		{
			scene = Scene::GAME;
			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene();
			gameScene->Init();
		}
		break;
	case Scene::GAME:
		if (gameScene->IsFinished())
		{
			scene = Scene::CLEAR;

			delete gameScene;
			gameScene = nullptr;

			clearScene = new Clear();
			clearScene->Init();
		}
		break;
	case Scene::CLEAR:
		if (clearScene->IsFinished())
		{
			scene = Scene::TITLE;
			
			delete clearScene;
			clearScene = nullptr;

			titleScene = new Title();
			titleScene->Init();
		}
		break;
	}
}

void UpdateScene() 
{
	switch (scene) 
	{
	case Scene::KUNKNOWN:
		break;
	case Scene::TITLE:
		titleScene->Update();
		break;
	case Scene::GAME:
		gameScene->Update();
		break;
	case Scene::CLEAR:
		clearScene->Update();
		break;
	}
}

void DrawScene()
{
	switch (scene) 
	{
	case Scene::KUNKNOWN:
		break;
	case Scene::TITLE:
		titleScene->Draw();
		break;
	case Scene::GAME:
		gameScene->Draw();
		break;
	case Scene::CLEAR:
		clearScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) 
{
	Initialize(L"Get the Money");

	//	DX初期化
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	gameScene->Init();

	titleScene->Init();

	clearScene->Init();
	
	scene = Scene::TITLE;

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	while (true)
	{
		if (Update())
		{
			break;
		}

		imguiManager->Begin();

		ChangeScene();

		UpdateScene();

		imguiManager->End();

		dxCommon->PreDraw();

		DrawScene();

		AxisIndicator::GetInstance()->Draw();

		imguiManager->Draw();

		dxCommon->PostDraw();
	}

	delete gameScene;
	gameScene = nullptr;

	delete titleScene;
	titleScene = nullptr;

	Finalize();

	return 0;
}
