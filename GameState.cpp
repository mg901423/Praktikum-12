#include "GameState.h"
#include "StateManager.h"

CGameState::CGameState(CStateManager* pManager)
  : m_pStateManager(pManager)
{
}
CGameState::CGameState()
{
}

CGameState::~CGameState()
{
}

void CGameState::ChangeState(CGameState* pNewState, int state)
{
	m_pStateManager->ChangeState(pNewState,state);
}