#pragma once
#include <Xinput.h>
#include <vector>
#include "Singleton.h"

namespace flgin
{
	// TODO: REMOVE TEST CODE
	class PathfinderComponent;

	class Command;
	class InputComponent;
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();
		bool ProcessInput();
		void AddCommand(Command* const command);
		void AddPlayer(InputComponent* player);
		void RemovePlayer(InputComponent* player);
		
		template <class T>
		void RemoveCommand()
		{
			const type_info& typeInfo{ typeid(T) };
			for (size_t i{}, commandAmount{ m_pCommands.size() }; i < commandAmount; ++i)
			{
				if (m_pCommands[i])
				{
					if (typeid(*m_pCommands[i]) == typeInfo)
					{
						delete m_pCommands[i];
						m_pCommands[i] = nullptr;
					}
				}
			}
		}

		template <class T>
		void GetCommand()
		{
			const type_info& typeInfo{ typeid(T) };
			for (size_t i{}, commandAmount{ m_pCommands.size() }; i < commandAmount; ++i)
			{
				if (m_pCommands[i])
				{
					if (typeid(*m_pCommands[i]) == typeInfo)
					{
						delete m_pCommands[i];
						m_pCommands[i] = nullptr;
					}
				}
			}
		}

		void AddPathfinder(PathfinderComponent* pathfinder);

	private:
		std::vector<Command*> m_pCommands;
		std::vector<InputComponent*> m_pPlayers;

		//TODO: REMOVE TEST CODE
		PathfinderComponent* m_pPathFinder;
	};
}