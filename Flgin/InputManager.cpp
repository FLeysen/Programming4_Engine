#include "FlginPCH.h"
#include "InputManager.h"
#include "Command.h"
#include "SDL.h"
#include "InputComponent.h"

//TODO: REMOVE TEST CODE:
#include "PathfinderComponent.h"
#include "SceneManager.h"

flgin::InputManager::InputManager()
	: m_pCommands{}
	, m_pPathFinder{}
{}

void flgin::InputManager::AddPathfinder(PathfinderComponent* pathfinder)
{
	m_pPathFinder = pathfinder;
}

flgin::InputManager::~InputManager()
{
	Logger& logger{ FLogger };
	for (Command* command : m_pCommands)
		logger.SafeDelete(command);
}

bool flgin::InputManager::ProcessInput()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) return false;
		else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			for (InputComponent* player : m_pPlayers)
			{
				if (player)
				{
					if (!player->ProcessKeyboardKey(e.key.keysym.sym, e.type == SDL_KEYUP))
						return false;
				}
			}
		}
		else if (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP)
		{
			SDL_JoystickID playerID{ e.cbutton.which };
			if (playerID >= static_cast<SDL_JoystickID>(m_pPlayers.size())) continue;
			if (!m_pPlayers[playerID]) continue;
			if (!m_pPlayers[playerID]->ProcessControllerKey(e.cbutton.button, e.type == SDL_CONTROLLERBUTTONUP)) return false;
		}
		//TODO: REMOVE TEST CODE
		//else if (e.type == SDL_MOUSEBUTTONUP)
		//{
		//	m_pPathFinder->SetTarget(float(e.button.x), float(e.button.y));
		//}
	}
	return true;	
}

void flgin::InputManager::AddCommand(Command* const command)
{
	m_pCommands.push_back(command);
}

void flgin::InputManager::AddPlayer(InputComponent* player)
{
	m_pPlayers.push_back(player);
}

void flgin::InputManager::RemovePlayer(InputComponent* player)
{
	for (InputComponent* pl : m_pPlayers)
	{
		if (pl == player)
			FLogger.SafeDelete(pl);
	}
}
