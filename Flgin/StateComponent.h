#pragma once
#include "BaseComponent.h"

namespace flgin
{
	class GameObject;
	class State
	{
	public:
		State() : m_pTargetState{ nullptr } {}
		virtual ~State() = default;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual bool Update() = 0;
		State* GetTargetState() const { return m_pTargetState; }

	protected:
		State* m_pTargetState;
	};

	class StateComponent final : public BaseComponent
	{
	public:
		StateComponent(GameObject* pOwnerObject);
		void Update();
		void SetCurrentState(State* pNewState);

		~StateComponent() = default;
		StateComponent(StateComponent&&) = delete;
		StateComponent(const StateComponent&) = delete;
		StateComponent& operator=(StateComponent&&) = delete;
		StateComponent& operator=(const StateComponent&) = delete;

	private:
		State* m_pCurrentState;
	};
}
