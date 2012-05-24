#include "HelpState.h"
#include "MenuState.h"
#include "StateManager.h"
#include "Image.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;


CHelpState::CHelpState(CStateManager* pManager) 
  : CGameState(pManager), m_pFont(NULL)
{
	m_pFont = new CGameFont;
	m_pFont->CreateFont("Verdana", 30, FW_NORMAL);
	

	m_pBackgroundImg = CImage::CreateImage("MainBackground.png",TRectanglei(0,600,0,800));
	m_pTitleImg = CImage::CreateImage("HelpTitle.png",TRectanglei(0,600,0,800));

	// Create the text controls of the menu.
	help = new CTextControl(m_pFont,TRectanglei(200,257,209,591));
	help->SetAlignement(CTextControl::TACenter);
	help->SetText("Premikanje: smerne tipke...");

}

CHelpState::~CHelpState()
{
}

CHelpState* CHelpState::GetInstance(CStateManager* pManager)
{
	static CHelpState Instance(pManager);
	return &Instance;
}

void CHelpState::OnKeyDown(WPARAM wKey) 
{ 

		switch(wKey)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			ChangeState(CMenuState::GetInstance(m_pStateManager),1);
			break;
		}
	
}

void CHelpState::OnChar(WPARAM wChar) 
{ 
	
}

void CHelpState::Draw()  
{
	m_pBackgroundImg->BlitImage();
	m_pTitleImg->BlitImage();

	help->Draw();

}

void CHelpState::EnterState()
{

}

