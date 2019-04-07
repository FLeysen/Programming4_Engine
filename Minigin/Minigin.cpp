#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "Logger.h"
#include "Time.h"
#include "Invoker.h"

void flgin::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, std::string("SDL_Init Error: ") + SDL_GetError(), this });
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (window == nullptr) 
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, std::string("SDL_CreateWindow Error: ") + SDL_GetError(), this });
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(window);
}

void flgin::Minigin::LoadGame() const
{
	Scene& scene = SceneManager::GetInstance().CreateScene("Demo");

	GameObject* go{ new flgin::GameObject{} };
	flgin::RenderComponent* renderComponent{ new flgin::RenderComponent{ go } };
	renderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("background.jpg"));
	go->AddComponent(renderComponent);
	scene.AddGameObject(go);
	scene.AddRenderComponent(renderComponent);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go };
	renderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("logo.png"));
	go->AddComponent( renderComponent );
	go->SetPosition(216, 180);
	scene.AddGameObject(go);
	scene.AddRenderComponent(renderComponent);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, "../Data/Lingua.otf", 36, {255, 0,0 }, "Programming 4 Assignment" });
	go->SetPosition(80.f, 20.f);
	scene.AddGameObject(go);
	scene.AddRenderComponent(renderComponent);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, "../Data/Lingua.otf", 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene.AddGameObject(go);
	scene.AddRenderComponent(renderComponent);
}

void flgin::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

void flgin::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	flgin::ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		float lag{ 1.0f };
		auto lastTime{ std::chrono::high_resolution_clock::now() };
		Renderer& renderer{ Renderer::GetInstance() };
		flgin::SceneManager& sceneManager{ flgin::SceneManager::GetInstance() };
		flgin::InputManager& input{ flgin::InputManager::GetInstance() };
		flgin::Invoker& invoker{ flgin::Invoker::GetInstance() };
		Time& time{ Time::GetInstance() };
		bool doContinue{ true };
		float frameTime{ msPerFrame / 1000.0f };
		time.SetFixedTime(frameTime);
		time.SetTimeScale(1.0f);

		while (doContinue)
		{
			auto currTime{ std::chrono::high_resolution_clock::now() };
			float deltaTime{ std::chrono::duration<float>(currTime - lastTime).count() };
			lastTime = currTime;
			lag += deltaTime;
			time.SetDeltaTime(deltaTime);

			doContinue = input.ProcessInput();

			invoker.Update();
			sceneManager.Update();

			while (lag >= frameTime)
			{
				//FixedUpdate functions are called here
				lag -= frameTime;
			}

			renderer.Render();
		}
	}

	Cleanup();
}
