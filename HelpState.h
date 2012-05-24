#ifndef _HELPSTATE_H_
#define _HELPSTATE_H_

#include "GameState.h"
#include "GameFont.h"
#include "TextControl.h"
#include "Image.h"
#include <string>
#include <vector>


class CHelpState : public CGameState
{
public:
	~CHelpState();

	void OnKeyDown(WPARAM wKey);
	void OnChar(WPARAM wChar);
	void Draw();
	void EnterState();

	static CHelpState* GetInstance(CStateManager* pManager);

protected:
	CHelpState(CStateManager* pManager);

private:
	


	CGameFont* m_pFont;

	CTextControl* help;

	TImagePtr m_pBackgroundImg; 
	TImagePtr m_pTitleImg; 

};

#endif  // _HIGHSCORESTATE_H_