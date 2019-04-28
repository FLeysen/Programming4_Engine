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
#include "InputComponent.h"
#include "PlayerCommands.h"
#include "MovementGrid.h"
#include "GridRenderer.h"
#include "GridMovementComponent.h"
#include "MovementCommands.h"
#include "SpriteComponent.h"
#include "PathfinderComponent.h"

void flgin::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, std::string("SDL_Init Error: ") + SDL_GetError(), this });
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	SDL_GameController* controller{ };

	for (int i{}, amt{ SDL_NumJoysticks() }; i < amt; ++i)
	{
		if (SDL_IsGameController(i))
			controller = SDL_GameControllerOpen(i);
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
	RenderComponent* renderComponent{ new flgin::RenderComponent{ go, scene } };
	renderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("background.jpg"));
	go->AddComponent(renderComponent);
	scene.AddGameObject(go);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go, scene };
	renderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("logo.png"));
	go->AddComponent(renderComponent);
	go->SetPosition(216, 180);
	scene.AddGameObject(go);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go, scene };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, "../Data/Lingua.otf", 36, {255, 0,0 }, "Programming 4 Assignment" });
	go->SetPosition(80.f, 20.f);
	scene.AddGameObject(go);
	
	go = new GameObject{};
	renderComponent = new RenderComponent{ go, scene };
	go->AddComponent(renderComponent);
	go->AddComponent(new TextComponent{ go, "../Data/Lingua.otf", 20, {255, 255, 0} });
	go->AddComponent(new FPSComponent{ go, .5f });
	scene.AddGameObject(go);

	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	MovementGrid* grid{ new MovementGrid{ go, 16, 21, 30.0f } };
	go->AddComponent(grid);
	go->AddComponent(new GridRenderer{ go, scene, grid });
	scene.AddGameObject(go);

	go = new GameObject{};
	go->SetPosition(15.0f, 15.0f);
	InputComponent* inputComponent{ new InputComponent{ go } };
	GridMovementComponent* gridMover{ new GridMovementComponent{ go, 100.0f, grid} };
	QuitCommand* quitCommand{ new QuitCommand{} };
	DirectionalGridMove* gridMoveRight{ new DirectionalGridMove{ gridMover, true, true } };
	DirectionalGridMove* gridMoveLeft{ new DirectionalGridMove{ gridMover, true, false } };
	DirectionalGridMove* gridMoveDown{ new DirectionalGridMove{ gridMover, false, true } };
	DirectionalGridMove* gridMoveUp{ new DirectionalGridMove{ gridMover, false, false } };

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_START, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 0 });

	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, gridMoveRight);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_LEFT, gridMoveLeft);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_DOWN, gridMoveDown);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_DPAD_UP, gridMoveUp);

	inputComponent->AddKeyboardMapping(SDLK_RIGHT, gridMoveRight);
	inputComponent->AddKeyboardMapping(SDLK_LEFT, gridMoveLeft);
	inputComponent->AddKeyboardMapping(SDLK_DOWN, gridMoveDown);
	inputComponent->AddKeyboardMapping(SDLK_UP, gridMoveUp);
	inputComponent->AddKeyboardMapping(SDLK_ESCAPE, quitCommand);

	SpriteComponent* spriteComponent{ new SpriteComponent{ go, scene } };
	spriteComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("ActualSprite.png"));
	spriteComponent->SetPositionOffset(-15.f, -15.f);
	spriteComponent->SetSpriteInfo(4, 1, 30.0f, 30.0f, 1.0f);
	spriteComponent->SetDimensions(30.0f, 30.0f);
	go->AddComponent(spriteComponent);
	go->AddComponent(gridMover);
	go->AddComponent(inputComponent);
	scene.AddGameObject(go);

	go = new GameObject{};
	inputComponent = new InputComponent{ go };
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_B, quitCommand);
	inputComponent->AddControllerMapping(SDL_CONTROLLER_BUTTON_Y, new RumbleCommand{ 1 });
	go->AddComponent(inputComponent);
	scene.AddGameObject(go);

	//go = new GameObject{};
	//PathfinderComponent* pathfinderComponent{ new PathfinderComponent{ go, 100.0f, grid} };
	//spriteComponent = new SpriteComponent{ go, scene };
	//spriteComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("ActualSprite.png"));
	//spriteComponent->SetPositionOffset(-7.5f, -7.5f);
	//spriteComponent->SetSpriteInfo(4, 1, 15.0f, 15.0f, 1.0f);
	//spriteComponent->SetDimensions(25.0f, 25.0f);
	//go->AddComponent(spriteComponent);
	//go->AddComponent(pathfinderComponent);
	//go->SetPosition(15.0f, 15.0f);
	//scene.AddGameObject(go);
	//InputManager::GetInstance().AddPathfinder(pathfinderComponent);
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
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();
	{
		float lag{ 1.0f };
		auto lastTime{ std::chrono::high_resolution_clock::now() };
		Renderer& renderer{ Renderer::GetInstance() };
		SceneManager& sceneManager{ SceneManager::GetInstance() };
		InputManager& input{ InputManager::GetInstance() };
		Invoker& invoker{ Invoker::GetInstance() };
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
