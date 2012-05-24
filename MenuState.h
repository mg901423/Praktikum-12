#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "GameState.h"
#include "GameFont.h"
#include "PlayState.h"

class CMenuState : public CGameState
{
public:
	~CMenuState();

	void OnKeyDown(WPARAM wKey);
	void Draw();
	void EnterState();

	static CMenuState* GetInstance(CStateManager* pManager);

protected:
	CMenuState(CStateManager* pManager);

private:
	
	void SelectionUp();
	void SelectionDown();
	void SelectionChosen();

	CGameFont* m_pFont;

	int m_iCurrentSelection;


	CPlayState* m_pCurrentGame;

	
	TImagePtr m_pBackgroundImg;
	TImagePtr m_pTitleImg;


	TImagePtr m_pItemBckgndNormal;
	TImagePtr m_pItemBckgndSelected;

	CTextControl* m_pNewGameText;
	CTextControl* m_pResumeGameText;
	CTextControl* m_pHelpText;
	CTextControl* m_pExitText;
};

#endif  // _MENUSTATE_H_