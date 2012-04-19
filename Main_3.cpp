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

const int ESCAPE=27;
#define CAMERASPEED	0.005f	

#define EPSILON 1.0e-8
#define ZERO EPSILON

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		

bool	keys[256];			
bool	active=TRUE;		
bool	fullscreen=TRUE;	
const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;

GLfloat	yrot;			
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z=0.0f;			

GLuint	filter;			
GLuint	texture[3];	
GLuint	texture2[3];	

int main_window;
bool sekanje=false;

//struktura za 3D vektor in operatorji
/*typedef struct Vector3					
{			
	Vector3() {}

	Vector3 (float new_x, float new_y, float new_z) 
	{
		x = new_x; y = new_y; z = new_z;
	}
	
	Vector3 operator+(Vector3 vVector)
	{
		return Vector3(vVector.x+x, vVector.y+y, vVector.z+z);
	}
	
	Vector3 operator-(Vector3 vVector) 
	{
		return Vector3(x-vVector.x, y-vVector.y, z-vVector.z);
	}
	
	Vector3 operator*(float number)	
	{
		return Vector3(x*number, y*number, z*number);
	}
	
	Vector3 operator/(float number)
	{
		return Vector3(x/number, y/number, z/number);
	}
	
	Vector3 normalizacija(Vector3 a)
	{
		float d=((a.x*a.x) + (a.y*a.y) + (a.z*a.z));
		float dol = sqrt(d);
		return Vector3(a.x * (1 /dol) , a.y * (1/dol) , a.z * (1/dol));
	}
	Vector3 vektorski_produkt(Vector3 a, Vector3 b)
	{
		return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	}
	float x, y, z;		

}Vector3;

float skalarni_produkt(Vector3 a, Vector3 b)
{
		return a.x * b.x + a.y + b.y + a.z + b.z;
}
float dolzina_vektorja(Vector3 a)
{
		float d=((a.x*a.x) + (a.y*a.y) + (a.z*a.z));
		return sqrt(d);
}
*/


Vector3 mPos;	
Vector3 mView;		
Vector3 mUp;

Vector3 playerPreviousView;
Vector3 playerPreviousPosition;
Vector3 playerPosition;
Vector3 playerVector;

Vector3 intersectionPoint=Vector3(0,0,0);
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

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void InitVars()
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
void Position_Camera(float pos_x,  float pos_y,  float pos_z,
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


int Collision_Detection(Plane plane, Vector3 position, Vector3 &direction, double &lamda, Vector3 &pNormal)
{
	Vector3 s;
	
	float DotProduct=s.skalarni_produkt(position,plane._Normal);
	double l2;

    //determine if ray paralle to plane
    if ((DotProduct<ZERO)&&(DotProduct>-ZERO)) 
		return 0;

	Vector3 temp = plane._Position - position;
    l2=(s.skalarni_produkt(plane._Normal,temp))/DotProduct;//razdalja do collision
	
    if (l2<-ZERO) 
		return 0;

    pNormal=plane._Normal;
	lamda=l2;
    return 1;

}

void Move_Camera(float speed)	//za naprej in nazaj premikanje
{
	Vector3 vVector = mView - mPos;	
	
	mPos.x  = mPos.x  + vVector.x * speed;
	mPos.z  = mPos.z  + vVector.z * speed;
	mView.x = mView.x + vVector.x * speed;
	mView.z = mView.z + vVector.z * speed;
	uveloc = vVector * speed;

	double rt,lamda=10000;
	Vector3 norm;
	Vector3 normal;
	Vector3 IntersectionPoint;

	/*int test = Collision_Detection(pl1,mPos,vVector,rt,norm);
	if (test==1)
	{ 
		 mPos = playerPreviousPosition;
		 mView = playerPreviousView;
	}*/
	int test2 = Collision_Detection(pl2,mPos,uveloc,rt,norm);
	if (test2==1)
			  { 
				  mPos = playerPreviousPosition;
				mView = playerPreviousView;

				normal=norm;
							IntersectionPoint=mPos+uveloc*rt;
							intersectionPoint = IntersectionPoint;
				  //mPos =  playerPreviousPosition;//Vector3(0,0,1);
				  //playerPosition = Vector3(mPos.x,mPos.y+0.2,mPos.z+0.1);
			  }
	int test3 = Collision_Detection(pl3,mPos,vVector,rt,norm);
	if (test3==1)
			  { 
				  mPos = playerPreviousPosition;
				 mView = playerPreviousView;
				 normal=norm;
							IntersectionPoint=mPos+uveloc*rt;
				 //mPos = playerPreviousPosition;//Vector3(0,0,1);
				  //playerPosition = Vector3(mPos.x,mPos.y+0.2,mPos.z+0.1);
			  }
	int test4 = Collision_Detection(pl4,mPos,vVector,rt,norm);
	if (test4==1)
			  { 
				  mPos = playerPreviousPosition;
				mView = playerPreviousView;
				normal=norm;
							IntersectionPoint=mPos+uveloc*rt;
				 //mPos = playerPreviousPosition;//Vector3(0,0,1);
				  //playerPosition = Vector3(mPos.x,mPos.y+0.2,mPos.z+0.1);
			  }
	int test5 = Collision_Detection(pl5,mPos,vVector,rt,norm);
	if (test5==1)
			  { 
				  mPos = playerPreviousPosition;
				  mView = playerPreviousView;
				  normal=norm;
							IntersectionPoint=mPos+uveloc*rt;
				  //sekanje
				  //mPos = playerPreviousPosition;//Vector3(0,0,1);
				  //playerPosition = Vector3(mPos.x,mPos.y+0.2,mPos.z+0.1);
			  }
	

	playerPosition = Vector3(mPos.x,mPos.y+0.2,mPos.z+0.1);
	playerPreviousPosition = mPos;
	playerPreviousView = mView;
}
void Move_Camera_LR(float speed)	//za levo in desno premikanje
{
	Vector3 vVector = mView - mPos;	
	Vector3 T1 = vVector*2;
	Vector3 T2 = vVector*3;
	float k = (T2.y - T1.y) / (T2.x - T2.x);
	float k2 = -1 / k;
	vVector  = vVector * k2;

	mPos.x  = mPos.x  + vVector.x * speed;
	mPos.z  = mPos.z  + vVector.z * speed;
	mView.x = mView.x + vVector.x * speed;
	mView.z = mView.z + vVector.z * speed;

	playerPosition = Vector3(mPos.x,mPos.y+0.2,mPos.z+0.1);
}

void Rotate_View(float speed)	//rotiranje levo in desno
{
	Vector3 vVector = mView - mPos;	

	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}

GLvoid KillGLWindow(GLvoid)								
{
	if (fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);								
	}

	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,L"Release Of DC And RC Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))	
		{
			MessageBox(NULL,L"Release Rendering Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;					
	}

	if (hDC && !ReleaseDC(hWnd,hDC))
	{
		MessageBox(NULL,L"Release Device Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;					
	}

	if (hWnd && !DestroyWindow(hWnd))	
	{
		MessageBox(NULL,L"Could Not Release hWnd.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass(L"OpenGL",hInstance))			
	{
		MessageBox(NULL,L"Could Not Unregister Class.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
}

BOOL	done=FALSE;		

void Keyboard_Input()
{
	if((GetKeyState(VK_UP) & 0x80))
	{	
		Move_Camera( CAMERASPEED);
	}

	if((GetKeyState(VK_DOWN) & 0x80))
	{
		Move_Camera(-CAMERASPEED);
	}

	if((GetKeyState(VK_LEFT) & 0x80))
	{	
		Move_Camera_LR(-CAMERASPEED);
		//Rotate_View(-CAMERASPEED);
	}

	if((GetKeyState(VK_RIGHT) & 0x80))
	{
		Move_Camera_LR(CAMERASPEED);
		//Rotate_View( CAMERASPEED);
	}

	if((GetKeyState(ESCAPE) & 0x80))
	{
		done = TRUE;
	}
}

void Mouse_Move(int wndWidth, int wndHeight)
{
	POINT mousePos;	
	//dobimo center okna
	int mid_x = wndWidth  >> 1;	
	int mid_y = wndHeight >> 1;	

	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;							
	
	//dobimo pozicijo miške
	GetCursorPos(&mousePos);
	//doloèimo pozicijo miške na sredino okna
	SetCursorPos(mid_x, mid_y);	

	//iz smeri(vektor) do miškinega kuzorja doloèimo kot
	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	
	mView.y += angle_z * 2;

	//doloèimo omejitve rotacije kota kamere
	if((mView.y - mPos.y) > 8)  mView.y = mPos.y + 8;
	if((mView.y - mPos.y) <-8)  mView.y = mPos.y - 8;
	
	Rotate_View(-angle_y); 
}

void readstr(FILE *f,char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}

void SetupWorld()
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

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)	
{
	glViewport(0,0,width,height);					

	glMatrixMode(GL_PROJECTION);					
	glLoadIdentity();								

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();					
}

int DrawGLScene(GLvoid)									
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
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
	glBindTexture(GL_TEXTURE_2D, texture[filter]);
	
	numtriangles = sector1.numtriangles;
	/*
	// narisemo vse trikotnike
	for (int i = 0; i < numtriangles; i++)
	{
		if (i > 3)
		{
			glBindTexture(GL_TEXTURE_2D, texture2[filter]);
		}

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
	}
	*/
	//PLAYER - zaenkrat samo krogla
	//glBegin(GL_QUADS);                     
	//				glVertex3f(-0.5,0.5,0);		//1
	//				glVertex3f(0.5,0.5,0);		//2
	//				glVertex3f(0.5,1.5,0);		//3
	//				glVertex3f(-0.5,1.5,0);		//4
	//glEnd();

	
					
	//render walls(planes) with texture
	glBindTexture(GL_TEXTURE_2D, texture2[filter]); 

	
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3,3,3);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3,-3,3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3,-3,3);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-3,3,3);
        
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-3,3,-3);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-3,-3,-3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3,-3,-3);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(3,3,-3);
    
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3,3,-3);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3,-3,-3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3,-3,3);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(3,3,3);
	
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-3,3,3);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-3,-3,3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3,-3,-3);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-3,3,-3);
	glEnd();

	//render floor (plane) with colours
	glBindTexture(GL_TEXTURE_2D, texture[filter]); 
    glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-3,-3,3);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3,-3,3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3,-3,-3);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-3,-3,-3);
	glEnd();

	glTranslatef(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z);
	glutSolidSphere (0.08, 1000, 1000);

	return TRUE; 

}

//ponavljajoèa se zanka
void idle()
{
	
	return;
}

AUX_RGBImageRec *LoadBMP(char* Filename, LPWSTR Filename1) 
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
		return auxDIBImageLoad(Filename1);
	}

	return NULL; 
}

int LoadGLTextures() 
{
	 int Status=FALSE;  
	 int Status2 = FALSE;

        AUX_RGBImageRec *TextureImage[1];  
		AUX_RGBImageRec *TextureImage2[1];  

        memset(TextureImage,0,sizeof(void *)*1);
		memset(TextureImage2,0,sizeof(void *)*1);


        if (TextureImage[0]=LoadBMP("Data/bricks.bmp", L"Data/bricks.bmp"))
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
		 if (TextureImage2[0]=LoadBMP("Data/bricks2.bmp", L"Data/bricks2.bmp"))
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

        return Status;                                  
}

int InitGL(GLvoid)						
{
	InitVars();
	if (!LoadGLTextures()) 
	{
		return FALSE; 
	}

	glEnable(GL_TEXTURE_2D); 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	//glDepthFunc(GL_LEQUAL);	
	glDepthFunc(GL_LESS); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	
	SetupWorld();
	Position_Camera(0, 0, 1,0, 0, 0, 0, 1, 0);

	return TRUE;		
}


BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);	
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= L"OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
		
			if (MessageBox(NULL,L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",L"NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;
			}
			else
			{
				
				MessageBox(NULL,L"Program Will Now Close.",L"ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;							
			}
		}
	}

	if (fullscreen)										
	{
		dwExStyle=WS_EX_APPWINDOW;						
		dwStyle=WS_POPUP;								
		ShowCursor(FALSE);								
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
		dwStyle=WS_OVERLAPPEDWINDOW;					
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

	
	if (!(hWnd=CreateWindowEx(	dwExStyle,						
								L"OpenGL",						
								L"PORTAL",							
								dwStyle |						
								WS_CLIPSIBLINGS |				
								WS_CLIPCHILDREN,				
								0, 0,							
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								hInstance,							
								NULL)))								
	{
		KillGLWindow();			
		MessageBox(NULL,L"Window Creation Error.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;			
	}

	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,							
		PFD_DRAW_TO_WINDOW |		
		PFD_SUPPORT_OPENGL |		
		PFD_DOUBLEBUFFER,			
		PFD_TYPE_RGBA,				
		bits,						
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0										
	};
	
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();								
		MessageBox(NULL,L"Can't Create A GL Device Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillGLWindow();								
		MessageBox(NULL,L"Can't Find A Suitable PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,L"Can't Set The PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				
	{
		KillGLWindow();								
		MessageBox(NULL,L"Can't Create A GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillGLWindow();								
		MessageBox(NULL,L"Can't Activate The GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);									
	ReSizeGLScene(width, height);					

	if (!InitGL())									
	{
		KillGLWindow();								
		MessageBox(NULL,L"Initialization Failed.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;									
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			UINT	uMsg,			WPARAM	wParam,			LPARAM	lParam)			
{
	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if (!HIWORD(wParam))					
			{
				active=TRUE;						
			}
			else
			{
				active=FALSE;						
			}

			return 0;								
		}

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}

		case WM_KEYDOWN:							
		{
			keys[wParam] = TRUE;					
			return 0;								
		}

		case WM_KEYUP:								
		{
			keys[wParam] = FALSE;					
			return 0;								
		}

		case WM_SIZE:								
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam)); 
			return 0;								
		}
	}

	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			HINSTANCE	hPrevInstance,		LPSTR		lpCmdLine,			int			nCmdShow)			
{
	MSG		msg;									
							

	if (MessageBox(NULL,L"Would You Like To Run In Fullscreen Mode?", L"Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							
	}

	InitVars();
	if (!CreateGLWindow("OpenGl - Portal",640,480,16,fullscreen))
	{
		return 0;									
	}

	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{
			Keyboard_Input();    
			Mouse_Move(640,480);

			if (active)					
			{
				if (keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
				{
					done=TRUE;							// ESC or DrawGLScene Signalled A Quit
				}
				else									// Not Time To Quit, Update Screen
				{
					idle();                             // Advance Simulation
					DrawGLScene();                      // Draw Scene
					SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
				
					DrawGLScene();					
					SwapBuffers(hDC);	
				}
			}
		
		}
	}

	
	KillGLWindow();									
	return (msg.wParam);							
}
