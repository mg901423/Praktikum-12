#include "PlayState.h"
#include "StateManager.h"
#include "MenuState.h"
#include <sstream>
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

#define EPSILON 1.0e-8
#define ZERO EPSILON	



CPlayState::CPlayState(CStateManager* pManager) 
  : CGameState(pManager), CAMERASPEED(0.05f)
{
	ESCAPE = 27;
	
		piover180=0.0174532925f;
   m_bGameOver=false;
	AddFontResource("01 Digitall.ttf");
	
	m_pFont = new CGameFont;
	m_pFont->CreateFont("01 Digitall", 20, FW_NORMAL);

	InitVars();
	LoadGLTextures();

//	Mouse_Move(640,480);
	SetupWorld();
	Position_Camera(0, 0, 1,0, 0, 0, 0, 1, 0);

}

GLvoid CPlayState::ReSizeGLScene(GLsizei width, GLsizei height)	
{
	glViewport(0,0,width,height);					

	glMatrixMode(GL_PROJECTION);					
	glLoadIdentity();								

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();					
}

void readstr(FILE *f,char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}
/*
//http://www.gamedev.net/topic/275238-gluax-replacement-code/
AUX_RGBImageRec *LoadBMP(char* Filename) 
{

	FILE *File=NULL;
	
	if (!Filename) 
	{
		return NULL; 
	}

	File=fopen(Filename,"r"); 

	if (File) 
	{
		
		fclose(File); 
		
		return auxDIBImageLoad(Filename);
	}

	return NULL; 
}*/

void load_texture( char *file_name, int width, int height, int depth, GLenum colour_type, GLenum filter_type)
{
	GLubyte *raw_bitmap;
	FILE    *file;
    if ((file = fopen(file_name, "rb"))==NULL)
    {
		printf("File Not Found : %s\n",file_name);
        exit (1);
    } 
    raw_bitmap = (GLubyte *) malloc (width * height * depth * (sizeof(GLubyte)));
    if (raw_bitmap == NULL)
    {
        printf("Cannot allocate memory for texture\n");
        fclose(file);
        exit(1);
    } 
	fread(raw_bitmap, width * height * depth, 1, file);
	fclose(file); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_type);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    gluBuild2DMipmaps(GL_TEXTURE_2D, colour_type, width, height, colour_type, GL_UNSIGNED_BYTE, raw_bitmap); 
	free(raw_bitmap);
}

GLuint loadtexture (const char *filename, float width, float height) 
{
  GLuint texture;

  unsigned char *data;
  FILE *file;

  file = fopen( filename, "rb" );
  if ( file == NULL ) return 0;

  data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );

  fclose( file );

  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 
GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
 GL_LINEAR_MIPMAP_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
 GL_LINEAR );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, 
GL_RGB, GL_UNSIGNED_BYTE, data );
    
  data = NULL;

  return texture;
}

void CPlayState::LoadGLTextures() 
{
		
			texture[0] = loadtexture("Data/floor.bmp", 512, 512);
		
			
			texture[1] = loadtexture("Data/TileFloorTexture.bmp", 512, 512);
		
		
			texture[2] = loadtexture("Data/bricks2.bmp", 512, 512);
			texture[3] = loadtexture("Data/portal2.bmp", 512, 512);
			
	/*
	 int Status=FALSE;  
	 int Status2 = FALSE;
	 int Status3 = FALSE;

        AUX_RGBImageRec *TextureImage[1];  
		AUX_RGBImageRec *TextureImage2[1];  
		AUX_RGBImageRec *TextureImage3[1];  

        memset(TextureImage,0,sizeof(void *)*1);
		memset(TextureImage2,0,sizeof(void *)*1);
		memset(TextureImage3,0,sizeof(void *)*1);


		FILE *File=NULL;
		File=fopen("Data/floor.bmp","r"); 
		if (File) 
		{
			fclose(File); 
		
			TextureImage[0] = auxDIBImageLoad("Data/floor.bmp");
		}

		FILE *File2=NULL;
		File2=fopen("Data/bricks2.bmp","r"); 
		if (File2) 
		{
			fclose(File2); 
		
			TextureImage3[0] = auxDIBImageLoad("Data/bricks2.bmp");
		}

		FILE *File3=NULL;
		File3=fopen("Data/TileFloorTexture.bmp","r"); 
		if (File3) 
		{
			fclose(File3); 
		
			TextureImage2[0] = auxDIBImageLoad("Data/TileFloorTexture.bmp");
		}

        if (TextureImage[0]!=NULL)
        {
                Status=TRUE;                            

                glGenTextures(3, &texture[0]);          

				glBindTexture(GL_TEXTURE_2D, texture[0]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
                
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

				glBindTexture(GL_TEXTURE_2D, texture[2]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        }
		 if (TextureImage2[0]!=NULL)//LoadBMP("Data/TileFloorTexture.bmp"))
        {
                Status2=TRUE;                            

                glGenTextures(3, &texture2[0]);          

				glBindTexture(GL_TEXTURE_2D, texture2[0]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage2[0]->sizeX, TextureImage2[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage2[0]->data);
                
                glBindTexture(GL_TEXTURE_2D, texture2[1]);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage2[0]->sizeX, TextureImage2[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage2[0]->data);

				
				glBindTexture(GL_TEXTURE_2D, texture2[2]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage2[0]->sizeX, TextureImage2[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage2[0]->data);
        }
		  if (TextureImage3[0]!=NULL)//LoadBMP("Data/bricks2.bmp"))
        {
                Status3=TRUE;                            

                glGenTextures(3, &texture3[0]);          

				glBindTexture(GL_TEXTURE_2D, texture3[0]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage3[0]->sizeX, TextureImage3[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage3[0]->data);
                
                glBindTexture(GL_TEXTURE_2D, texture3[1]);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage3[0]->sizeX, TextureImage3[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage3[0]->data);

				
				glBindTexture(GL_TEXTURE_2D, texture3[2]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage3[0]->sizeX, TextureImage3[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage3[0]->data);
        }
		  if (TextureImage3[0]) 
        {
                if (TextureImage3[0]->data)              
                {
                        free(TextureImage3[0]->data);    
                }

                free(TextureImage3[0]);                  
        }
		 if (TextureImage2[0]) 
        {
                if (TextureImage2[0]->data)              
                {
                        free(TextureImage2[0]->data);    
                }

                free(TextureImage2[0]);                  
        }
        if (TextureImage[0]) 
        {
                if (TextureImage[0]->data)              
                {
                        free(TextureImage[0]->data);    
                }

                free(TextureImage[0]);                  
        }

        */                               
}

void CPlayState::SetupWorld()
{
	float x, y, z, u, v;
	int numtriangles;
	FILE *filein;
	char oneline[255];
	filein = fopen("data/track.txt", "rt");				

	readstr(filein,oneline);
	sscanf(oneline, "TRIANGLES %d\n", &numtriangles);

	sector1.triangle = new TRIANGLE[numtriangles];
	sector1.numtriangles = numtriangles;
	for (int loop = 0; loop < numtriangles; loop++)
	{
		for (int vert = 0; vert < 3; vert++)
		{
			readstr(filein,oneline);
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			sector1.triangle[loop].vertex[vert].x = x;
			sector1.triangle[loop].vertex[vert].y = y;
			sector1.triangle[loop].vertex[vert].z = z;
			sector1.triangle[loop].vertex[vert].u = u;
			sector1.triangle[loop].vertex[vert].v = v;
		}
	}
	fclose(filein);
	return;
}
void CPlayState::Rotate_View(float speed)	//rotiranje levo in desno
{
	Vector3 vVector = mView - mPos;	

	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}
void CPlayState::Move_Camera(float speed)	//za naprej in nazaj premikanje
{
	Vector3 vVector = mView - mPos;	
	vVector = vVector.normalizacija(vVector);//Vector3 vVector2 = Vector3.normalizacija(vVector);
	
	mPos.x  = mPos.x  + vVector.x * speed;
	mPos.z  = mPos.z  + vVector.z * speed;
	mView.x = mView.x + vVector.x * speed;
	mView.z = mView.z + vVector.z * speed;

}
void CPlayState::InitVars()
{
	 //create palnes
	pl1._Position=Vector3(0,-300,0);
	pl1._Normal=Vector3(0,1,0);
	pl2._Position=Vector3(300,0,0);
	pl2._Normal=Vector3(-1,0,0);
	pl3._Position=Vector3(-300,0,0);
	pl3._Normal=Vector3(1,0,0);
	
	pl4._Position=Vector3(0,0,300);
	pl4._Normal=Vector3(0,0,-1);
	
	pl5._Position=Vector3(0,0,-300);
	pl5._Normal=Vector3(0,0,1);
}
//doloèimo pozicijo kamere in igralca
void CPlayState::Position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
	mPos	= Vector3(pos_x,  pos_y,  pos_z ); // set position
	mView	= Vector3(view_x, view_y, view_z); // set view
	mUp		= Vector3(up_x,   up_y,   up_z  ); // set the up vector	

	playerPosition = Vector3(mPos.x,mPos.y+0.2,mPos.z+0.1);
	playerPreviousPosition = mPos;

	Vector3 vVector = mView - mPos;	
	uveloc=vVector;
}

CPlayState::~CPlayState()
{

	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	//RemoveFontResource(L"01 Digitall.ttf");
}

CPlayState* CPlayState::GetInstance(CStateManager* pManager)
{
	static CPlayState Instance(pManager);
	return &Instance;
}


void CPlayState::OnKeyDown(WPARAM wKey)
{
	switch (wKey)
	{
	case VK_UP:
		Move_Camera( CAMERASPEED);
		break;
	case VK_DOWN:
		Move_Camera( -CAMERASPEED);
		break;
	case VK_LEFT:
		Rotate_View(-CAMERASPEED);
		break;
	case VK_RIGHT:
		Rotate_View(CAMERASPEED);
		break;

	case VK_ESCAPE:
		ChangeState(CMenuState::GetInstance(m_pStateManager),1);
		break;
	case VK_RETURN:
		break;
	}
}

//void CPlayState::Mouse_Move(int wndWidth, int wndHeight)
//{
//	POINT mousePos;	
//	//dobimo center okna
//	int mid_x = wndWidth  >> 1;	
//	int mid_y = wndHeight >> 1;	
//
//	float angle_y  = 0.0f;				
//	float angle_z  = 0.0f;							
//	
//	//dobimo pozicijo miške
//	GetCursorPos(&mousePos);
//	//doloèimo pozicijo miške na sredino okna
//	SetCursorPos(mid_x, mid_y);	
//
//	//iz smeri(vektor) do miškinega kuzorja doloèimo kot
//	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
//	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;
//
//	
//	mView.y += angle_z * 2;
//
//	//doloèimo omejitve rotacije kota kamere
//	if((mView.y - mPos.y) > 8)  mView.y = mPos.y + 8;
//	if((mView.y - mPos.y) <-8)  mView.y = mPos.y - 8;
//	
//	Rotate_View(-angle_y); 
//}

void CPlayState::Update(DWORD dwCurrentTime)
{
	if (mPos.z >0 && mPos.z < 1 && mPos.x >2.77)
	{
		//rotiraj levo za 90 stopinj
		Vector3 vVector = mView - mPos;	

		mView.z = (float)(mPos.z + sin(-90.0)*vVector.x + cos(-90.0)*vVector.z);
		mView.x = (float)(mPos.x + cos(-90.0)*vVector.x - sin(-90.0)*vVector.z);

		mPos.x = 0.5;
		mPos.z = 2.5;
		
	}
	else if (mPos.x>0 && mPos.x <1 && mPos.z >2.77)
	{
		//rotiraj desno za 90 stopinj
		Vector3 vVector = mView - mPos;	

		mView.z = (float)(mPos.z + sin(90.0)*vVector.x + cos(90.0)*vVector.z);
		mView.x = (float)(mPos.x + cos(90.0)*vVector.x - sin(90.0)*vVector.z);

		mPos.x = 2.5;
		mPos.z = 0.5;
		
	}
	else
	{
		//preveri collision
		if (mPos.x > 2.77 || mPos.x < -2.77 || mPos.z > 2.77 || mPos.z < -2.77)
		{
			mPos = playerPreviousPosition;
		}
		else
		{
			playerPreviousPosition = mPos;
		}
	}
}



void CPlayState::Draw()  
{ 
	 glClearStencil(0); //clear the stencil buffer
    glClearDepth(1.0f);
	 glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | 
GL_STENCIL_BUFFER_BIT); //clear the buffers
	
	glLoadIdentity();									

	GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias-0.25f;

	int numtriangles;
	
			gluLookAt(mPos.x,  mPos.y,  mPos.z,	
			  mView.x, mView.y, mView.z,	
			  mUp.x,   mUp.y,   mUp.z);
			glTranslatef(xtrans, ytrans, ztrans);
	
		//start
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //disable the color mask
		glDepthMask(GL_FALSE); //disable the depth mask

		glEnable(GL_STENCIL_TEST); //enable the stencil testing

		glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); //setthe stencil buffer to replace our next lot of data

		glPushMatrix();
		//glColor4f(1,1,1,0.7);
		glBindTexture(GL_TEXTURE_2D,texture[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(1,0);
		glVertex3f(3,0,1);
			glTexCoord2f(1,1);
		glVertex3f(3,0,0);
			glTexCoord2f(0,1);
		glVertex3f(3,1,0);  
		    glTexCoord2f(0,0);
		glVertex3f(3,1,1);
		glEnd();
		glPopMatrix();

	    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //enablethe color mask
		glDepthMask(GL_TRUE); //enable the depth mask

		glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //set the stencilbuffer to keep our next lot of data

		glDisable(GL_DEPTH_TEST); //disable depth testing of thereflection
		glPushMatrix();
		glScalef(-1.0f, 1.0f, 1.0f); //flip the reflection 

		glTranslatef(0,0,0); //translate the reflection onto thedrawing plane
		//glRotatef(40,-1,0,0); //rotate the reflection
		//draw the square as our reflection
		

		numtriangles = sector1.numtriangles;
	// narisemo vse trikotnike
	for (int i = 0; i < numtriangles; i++)
	{
		if (i == 0 || i == 1)
		{
			glBindTexture(GL_TEXTURE_2D, texture[0]);
		}
		else if (i == 2 || i== 3)
		{
			glBindTexture(GL_TEXTURE_2D, texture[1]);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[2]);
		}
		glPushMatrix();
		glBegin(GL_TRIANGLES);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			x_m = sector1.triangle[i].vertex[0].x;
			y_m = sector1.triangle[i].vertex[0].y;
			z_m = sector1.triangle[i].vertex[0].z;
			u_m = sector1.triangle[i].vertex[0].u;
			v_m = sector1.triangle[i].vertex[0].v;
			glTexCoord2f(u_m,v_m); 
			glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[i].vertex[1].x;
			y_m = sector1.triangle[i].vertex[1].y;
			z_m = sector1.triangle[i].vertex[1].z;
			u_m = sector1.triangle[i].vertex[1].u;
			v_m = sector1.triangle[i].vertex[1].v;
			glTexCoord2f(u_m,v_m); 
			glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[i].vertex[2].x;
			y_m = sector1.triangle[i].vertex[2].y;
			z_m = sector1.triangle[i].vertex[2].z;
			u_m = sector1.triangle[i].vertex[2].u;
			v_m = sector1.triangle[i].vertex[2].v;
			glTexCoord2f(u_m,v_m); 
			glVertex3f(x_m,y_m,z_m);
		glEnd();
		glPopMatrix();
	}

		glPopMatrix();
		glEnable(GL_DEPTH_TEST); //enable the depth testing

		glDisable(GL_STENCIL_TEST); //disable the stencil testing

		//end

		glEnable(GL_BLEND); //enable alpha blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //set the blending function
    glPushMatrix();
		glBindTexture(GL_TEXTURE_2D,texture[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(1,0);
		glVertex3f(1,0,3);
			glTexCoord2f(1,1);
		glVertex3f(0,0,3);
			glTexCoord2f(0,1);
		glVertex3f(0,1,3);
			glTexCoord2f(0,0);
		glVertex3f(1,1,3);
		glEnd();
		glPopMatrix();
	
		
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D,texture[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(1,0);
		glVertex3f(3,0,1);
			glTexCoord2f(1,1);
		glVertex3f(3,0,0);
			glTexCoord2f(0,1);
		glVertex3f(3,1,0);  
		    glTexCoord2f(0,0);
		glVertex3f(3,1,1);
		glEnd();
		glPopMatrix();
		
    glDisable(GL_BLEND); //disable alpha blending
	
	
		

	//***********************************************


    numtriangles = sector1.numtriangles;
	// narisemo vse trikotnike
	for (int i = 0; i < numtriangles; i++)
	{
		if (i == 0 || i == 1)
		{
			glBindTexture(GL_TEXTURE_2D, texture[0]);
		}
		else if (i == 2 || i== 3)
		{
			glBindTexture(GL_TEXTURE_2D, texture[1]);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[2]);
		}
		glPushMatrix();
		glBegin(GL_TRIANGLES);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			x_m = sector1.triangle[i].vertex[0].x;
			y_m = sector1.triangle[i].vertex[0].y;
			z_m = sector1.triangle[i].vertex[0].z;
			u_m = sector1.triangle[i].vertex[0].u;
			v_m = sector1.triangle[i].vertex[0].v;
			glTexCoord2f(u_m,v_m); 
			glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[i].vertex[1].x;
			y_m = sector1.triangle[i].vertex[1].y;
			z_m = sector1.triangle[i].vertex[1].z;
			u_m = sector1.triangle[i].vertex[1].u;
			v_m = sector1.triangle[i].vertex[1].v;
			glTexCoord2f(u_m,v_m); 
			glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[i].vertex[2].x;
			y_m = sector1.triangle[i].vertex[2].y;
			z_m = sector1.triangle[i].vertex[2].z;
			u_m = sector1.triangle[i].vertex[2].u;
			v_m = sector1.triangle[i].vertex[2].v;
			glTexCoord2f(u_m,v_m); 
			glVertex3f(x_m,y_m,z_m);
		glEnd();
		glPopMatrix();
	}

	

}


