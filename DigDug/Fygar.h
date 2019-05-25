#pragma once
#include "BaseComponent.h"

namespace flgin
{
	class MovementGrid;
}

namespace DigDug
{
	class Player;

	class Fygar final : public flgin::BaseComponent
	{
	public:
		Fygar(flgin::GameObject* pOwnerObject, flgin::GameObject* pFire);
		~Fygar() = default;
		void Update() override {}

		UINT GetScoreWorth();

		void SetFiring(bool firing) { m_IsFiring = firing; }
		bool IsFiring() const { return m_IsFiring; }

		void SetBloating(bool bloating) { m_IsBloating = bloating; }
		bool IsBloating() const { return m_IsBloating; }

		void SetHitBy(Player* player) { m_pHitBy = player; }
		Player* GetHitBy() const { return m_pHitBy; }

		flgin::GameObject* GetFire() const { return m_pFire; }

		static void SetGrid(flgin::MovementGrid* grid) { m_pGrid = grid; }

	private:
		bool m_IsFiring;
		bool m_IsBloating;
		flgin::GameObject* m_pFire;
		Player* m_pHitBy;
		static flgin::MovementGrid* m_pGrid;
	};
}

