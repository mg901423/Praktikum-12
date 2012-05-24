#include "MenuState.h"
#include "PlayState.h"
#include "HelpState.h"
#include "StateManager.h"
#include "Image.h"


CMenuState::CMenuState(CStateManager* pManager) 
  : CGameState(pManager), m_pFont(NULL), m_iCurrentSelection(0), 
    m_pCurrentGame(NULL)
{
	m_pFont = new CGameFont;
	m_pFont->CreateFont("Verdana", 30, FW_NORMAL);

	// Create the different images
	m_pBackgroundImg = CImage::CreateImage("MainBackground.png",TRectanglei(0,600,0,800));
	m_pTitleImg = CImage::CreateImage("MenuTitlePortal.png",TRectanglei(0,600,0,800));
	m_pItemBckgndNormal = CImage::CreateImage("MenuItems.png",TRectanglei(0,57,0,382));
	m_pItemBckgndSelected = CImage::CreateImage("MenuItems.png",TRectanglei(58,114,0,382));

	// Create the text controls of the menu.
	m_pNewGameText = new CTextControl(m_pFont,TRectanglei(200,257,209,591));
	m_pNewGameText->SetAlignement(CTextControl::TACenter);
	m_pNewGameText->SetText("New game");

	m_pResumeGameText = new CTextControl(m_pFont,TRectanglei(300,357,209,591));
	m_pResumeGameText->SetAlignement(CTextControl::TACenter);
	m_pResumeGameText->SetText("Resume game");

	m_pHelpText = new CTextControl(m_pFont,TRectanglei(400,457,209,591));
	m_pHelpText->SetAlignement(CTextControl::TACenter);
	m_pHelpText->SetText("Help");

	m_pExitText = new CTextControl(m_pFont,TRectanglei(500,557,209,591));
	m_pExitText->SetAlignement(CTextControl::TACenter);
	m_pExitText->SetText("Exit");
}

CMenuState::~CMenuState()
{
}

CMenuState* CMenuState::GetInstance(CStateManager* pManager)
{
	static CMenuState Instance(pManager);
	return &Instance;
}

void CMenuState::OnKeyDown(WPARAM wKey)
{
	switch (wKey)
	{
	case VK_DOWN:
		SelectionDown();
		break;
	case VK_UP:
		SelectionUp();
		break;
	case VK_RETURN:
		SelectionChosen();
		break;
	}
}


void CMenuState::Draw()
{
	m_pBackgroundImg->BlitImage();
	m_pTitleImg->BlitImage();

	for (int i=0;i<4;i++)
	{
		if (i==m_iCurrentSelection)
			m_pItemBckgndSelected->BlitImage(209,200+i*100);
		else
			m_pItemBckgndNormal->BlitImage(209,200+i*100);
	}
	
	m_pNewGameText->Draw();
	m_pResumeGameText->Draw();
	m_pHelpText->Draw();
	m_pExitText->Draw();

}

void CMenuState::EnterState()
{
	// Select the projection stack and apply 
		// an orthographic projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0,800,600,0.0,-1.0,1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	// Checks whether there is a current game active
	m_iCurrentSelection = 0;
	if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
		m_pResumeGameText->SetTextColor(0.5,0.5,0.5);
	else
		m_pResumeGameText->SetTextColor(1.0,1.0,1.0);
}

void CMenuState::SelectionUp()
{
	m_iCurrentSelection--;
	if (m_iCurrentSelection==-1)
		m_iCurrentSelection = 3;

	// If there is no current game, we should skip
	// the "Resume game" item.
	if (m_iCurrentSelection==1) 
	{
		if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
			m_iCurrentSelection--;
	}
}

void CMenuState::SelectionDown()
{
	m_iCurrentSelection++;
	if (m_iCurrentSelection==4)
		m_iCurrentSelection = 0;

	// If there is no current game, we should skip
	// the "Resume game" item.
	if (m_iCurrentSelection==1) 
	{
		if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
			m_iCurrentSelection++;
	}
}

void CMenuState::SelectionChosen()
{
	switch (m_iCurrentSelection)
	{
	case 0:
		glMatrixMode(GL_PROJECTION);					
		glLoadIdentity();								

		gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);			
		glLoadIdentity();

		if (!m_pCurrentGame)
			m_pCurrentGame = CPlayState::GetInstance(m_pStateManager);
		m_pCurrentGame->Reset();
		//m_pCurrentGame = CPlayState::GetInstance(m_pStateManager);
		ChangeState(m_pCurrentGame,2);
		break;

	case 1:
		glMatrixMode(GL_PROJECTION);					
		glLoadIdentity();								

		gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);			
		glLoadIdentity();

		if (m_pCurrentGame && !m_pCurrentGame->IsGameOver())
			ChangeState(m_pCurrentGame,2);
		break;

	case 2:
		ChangeState(CHelpState::GetInstance(m_pStateManager),1);
		break;

	case 3:
		PostQuitMessage(0);
		break;
	}
}
