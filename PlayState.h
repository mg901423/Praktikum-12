#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"
#include "GameFont.h"
#include "TextControl.h"
#include "ComboControl.h"
#include <atlstr.h>
#include <iostream>
#include <windows.h>	
#include <stdio.h>
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <gl\glaux.h>		
#include <GL\glui.h>
#include <string.h>
#include <string>
#include <math.h>	
#include "Vector3.h"
using namespace std;

class CPlayState : public CGameState
				 
{
public:
	~CPlayState();
	CPlayState::GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	// Implementation of specific events
	void OnKeyDown(WPARAM wKey);
	void Update(DWORD dwCurrentTime);
	void Draw();
	void InitVars();
	void Position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z);
	void SetupWorld();
	void Rotate_View(float speed);
	//void Mouse_Move(int wndWidth, int wndHeight);
	void LoadGLTextures();

	void Move_Camera(float speed);

	void Reset()
	{
		m_bGameOver=false;
		Position_Camera(0, 0, 1,0, 0, 0, 0, 1, 0);
	}
	

	bool IsGameOver(){ return m_bGameOver; }

	// Returns the single instance
	static CPlayState* GetInstance(CStateManager* pManager);
	

protected:
	CPlayState(CStateManager* pManager);

private:
	
	 int ESCAPE;
	 const float CAMERASPEED;		 float piover180;


	double mouse_x;
	double mouse_y;
		
	
	float heading;
	float xpos;
	float zpos;

	GLfloat	yrot;			
	GLfloat walkbias;
	GLfloat walkbiasangle;
	GLfloat lookupdown;
	GLfloat	z;			

	GLint stencilBits;
	GLuint texture[40];

	int main_window;

	Vector3 mPos;	
	Vector3 mView;		
	Vector3 mUp;

	Vector3 playerPreviousView;
	Vector3 playerPreviousPosition;
	Vector3 playerPosition;
	Vector3 playerVector;

	Vector3 intersectionPoint;
	Vector3 uveloc;

struct Plane{
	        Vector3 _Position;
			Vector3 _Normal;
};

Plane pl1,pl2,pl3,pl4,pl5,pl6;

typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagSECTOR
{
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;

SECTOR sector1;	

	// The font used to draw text
	CGameFont* m_pFont;
	// The control in charge of the decreasing 
	// time for the combo score.
	CComboControl* m_pComboControl;


	bool m_bGameOver;


};

#endif   //_PLAYSTATE_H_