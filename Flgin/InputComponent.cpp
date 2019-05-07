#include "FlginPCH.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Command.h"

flgin::InputComponent::InputComponent(GameObject* const ownerObject)
	: BaseComponent(ownerObject)
	, m_ControllerMappings{}
{
	FInputManager.AddPlayer(this);
}

flgin::InputComponent::~InputComponent()
{
	FInputManager.RemovePlayer(this);
}

void flgin::InputComponent::AddControllerMapping(UINT8 key, Command* command)
{
	if (!m_ControllerMappings.emplace(key, command).second)
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add keycode " + std::to_string(key) + " as mapping, but it was already in use!" });
}

void flgin::InputComponent::AddKeyboardMapping(int key, Command* command)
{
	if (!m_KeyboardMappings.emplace(key, command).second)
		FLogger.Log(StatusCode{ StatusCode::Status::FAIL, "Attempted to add keycode " + std::to_string(key) + " as mapping, but it was already in use!" });
}

void flgin::InputComponent::Update()
{}

bool flgin::InputComponent::ProcessKeyboardKey(int key, bool isKeyUp)
{
	for (const std::pair<int, Command*>& mapping : m_KeyboardMappings)
	{
		if (key == mapping.first)
		{
			if (!mapping.second->Execute(*m_pOwnerObject, isKeyUp))
				return false;
			return true;
		}
	}
	return true;
}

bool flgin::InputComponent::ProcessControllerKey(UINT8 key, bool isKeyUp)
{
	for (const std::pair<UINT8, Command*>& mapping : m_ControllerMappings)
	{
		if (key == mapping.first)
		{
			if (!mapping.second->Execute(*m_pOwnerObject, isKeyUp))
				return false;
			return true;
		}
	}
	return true;
}