#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

#include <gl\gl.h>           // ������������ ���� ��� ���������� OpenGL32

#include <gl\glu.h>          // ������������ ���� ��� ���������� GLu32

//#include <gl\glaux.h>    

bool textureMode = true;
bool lightMode = true;
bool colour = true;
bool drive = true;
bool A = false;
double a, b, c;
double a1, b1, c1;
bool B = true;
bool ooooops = true;
bool line = true;
bool ground = true;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

	if (key == 'S')
	{
		colour = !colour;
	}

	if (key == 'Z')
	{
		drive = !drive;
		
	}

	if (key == 'A')
	{
		A = !A;
	}

	if (key == 'X')
	{
		line = !line;
	}

	if (key == 'C')
	{
		ooooops = !ooooops;
	}

	if (key == 'D') {
		ground = !ground;


	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture_2.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//���������� �� ��� ��������
	glGenTextures(1, &texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}


inline double factual(int n) {
	int r = 1;
	for (int i = 2; i <= n; ++i) {
		r *= i;
	}
	return r;
}

inline double Bern(int i, int n, double m) {
	//return 1.0 * factual(n) * 1.7* pow(m, i) * pow(1 - m, n - i) /(3* factual(i)) / 6* factual(n - i);
	if (A) return (0.5 * factual(n) / (0.7 * factual(i) * 0.245 * factual(1.593 * n - i))) * pow(2.13 * m, 1.2 * i) * pow(1 - m, 0.8 * n - 0.2 * i);
	else return  (-0.5 * factual(0.347 * n) / (0.7 * factual(1.104 * i) * factual(n - i))) * pow(2.13 * m, 1.2 * i) * pow(1 - m, n - i);
	//return abs(sin(n)) /*+ sin(n - i)*/;
	//return sqrt(pow(n, i) - pow(n, i));
}

void eye(double a1, double b1, double c1, double d1)
{
	double h1 = 6, h2 = 8;

	double F[] = { a1, d1, h1 };
	double G[] = { a1, c1, h1 };
	double E[] = { b1, c1, h1 };
	double S[] = { b1, d1, h1 };

	double F2[] = { a1, d1, h2 };
	double G2[] = { a1, c1, h2 };
	double E2[] = { b1, c1, h2 };
	double S2[] = { b1, d1, h2 };

	double F1[] = { a1, (d1 + c1) / 2, 7 };
	double G1[] = { a1, (d1 + c1) / 2, h1 };
	double F_1[] = { a1, d1, 7 };
	double G_1[] = { a1, c1, 7 };


	glBegin(GL_QUADS);
	glNormal3d(-2, 0, 0); //���������
	//glColor3d(0.89, 0.9, 0.85); //�����
	//glColor3d(0.1, 0.1, 0.1);
	glColor3d(0.9, 0.7, 0.9);
	glVertex3dv(F);
	glVertex3dv(F_1);
	glVertex3dv(F1);
	glVertex3dv(G1);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3d(-2, 0, 0); //���������
	glColor3d(0.1, 0.1, 0.1);
	glVertex3dv(G1);
	glVertex3dv(F1);
	glVertex3dv(G_1);
	glVertex3dv(G);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS); //����� ���� ��� ��������
	glNormal3d(-2, 0, 0); //���������
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(F_1);
	glTexCoord2d(0, 1);
	glVertex3dv(F2);
	glTexCoord2d(1, 1);
	glVertex3dv(G2);
	glTexCoord2d(1, 0);
	glVertex3dv(G_1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS); //������ ���� �������� ����������
	glNormal3d(-2, 0, 0);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(G);
	glTexCoord2d(0, 1);
	glVertex3dv(G2);
	glTexCoord2d(1, 1);
	glVertex3dv(E2);
	glTexCoord2d(1, 0);
	glVertex3dv(E);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 3, 0);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(E);
	glTexCoord2d(0, 1);
	glVertex3dv(E2);
	glTexCoord2d(1, 1);
	glVertex3dv(S2);
	glTexCoord2d(1, 0);
	glVertex3dv(S);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(-2, 0, 0);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(S);
	glTexCoord2d(0, 1);
	glVertex3dv(S2);
	glTexCoord2d(1, 1);
	glVertex3dv(F2);
	glTexCoord2d(1, 0);
	glVertex3dv(F);
	glEnd();

	for (int i = 0; i < 2; i++) {

		double F1[] = { a1, d1, h1 }; //eye1
		double G1[] = { a1, c1, h1 };
		double E1[] = { b1, c1, h1 };
		double S1[] = { b1, d1, h1 };

		
		glBindTexture(GL_TEXTURE_2D, texId);
		glBegin(GL_TRIANGLES);
		glNormal3d(0, 0, 1);
		glColor3d(a, b, c);
		glTexCoord2d(0, 0);
		glVertex3dv(E1);
		glTexCoord2d(0, 1);
		glVertex3dv(G1);
		glTexCoord2d(1, 1);
		glVertex3dv(F1);
		glTexCoord2d(1, 0);
		glEnd();

		
		glBindTexture(GL_TEXTURE_2D, texId);
		glBegin(GL_TRIANGLES);
		glNormal3d(0, 0, 1);
		glColor3d(a, b, c);
		glTexCoord2d(1, 0);
		glVertex3dv(E1);
		glTexCoord2d(0, 1);
		glVertex3dv(F1);
		glTexCoord2d(0, 0);
		glVertex3dv(S1);
		//glTexCoord2d(1, 1);
		glEnd();
		h1 = 8;
	}
}

void side(double y)
{
	double z = 6;
	double A1[] = { 0, y, z };
	double B1[] = { 9, y, z };

	double A[] = { 0, y, 1 };
	double _1[] = { 0.5, y, 1 };
	double _2[] = { 0.5, y, 3 };
	double _3[] = { 3.5, y, 3 };
	double _4[] = { 3.5, y, 1 };
	double _5[] = { 6, y, 1 };
	double _6[] = { 6, y, 3 };
	double _7[] = { 9, y, 3 };

	glBegin(GL_TRIANGLES);

	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(_2);
	glVertex3dv(_1);
	glVertex3dv(A);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(A);
	glVertex3dv(_2);
	glVertex3dv(A1);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(A1);
	glVertex3dv(_2);
	glVertex3dv(_3);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(_3);
	glVertex3dv(A1);
	glVertex3dv(_6);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(_6);
	glVertex3dv(_3);
	glVertex3dv(_4);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(_4);
	glVertex3dv(_5);
	glVertex3dv(_6);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(_6);
	glVertex3dv(A1);
	glVertex3dv(_7);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (y == -7) glNormal3d(0, -1, 0);
	if (y == 0) glNormal3d(0, 1, 0);
	glColor3d(a, b, c);
	glVertex3dv(_7);
	glVertex3dv(A1);
	glVertex3dv(B1);
	glEnd();
}

void buttocks()
{
	double h1 = 6;
	double B1[] = { 9, 0, h1 };
	double C1[] = { 9, -7, h1 };

	double _1[] = { 9, -7, 3 };
	double _2[] = { 9, -6, 3 };
	double _3[] = { 9, -6, 1 };
	double _4[] = { 9, -1, 1 };
	double _5[] = { 9, -1, 3 };
	double _6[] = { 9, 0, 3 };

	glBegin(GL_TRIANGLES);
	//glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(C1);
	glVertex3dv(_1);
	glVertex3dv(_2);
	glEnd();

	glBegin(GL_TRIANGLES);
	//glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(_2);
	;	glVertex3dv(_3);
	glVertex3dv(_4);
	glEnd();

	glBegin(GL_TRIANGLES);
	//glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(_4);
	glVertex3dv(_2);
	glVertex3dv(_5);
	glEnd();

	glBegin(GL_TRIANGLES);
	//glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(_5);
	glVertex3dv(_2);
	glVertex3dv(C1);
	glEnd();

	glBegin(GL_TRIANGLES);
	//glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(_5);
	glVertex3dv(C1);
	glVertex3dv(_6);
	glEnd();

	glBegin(GL_TRIANGLES);
	//glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(_6);
	glVertex3dv(C1);
	glVertex3dv(B1);
	glEnd();
}

void belly()
{
	double A[] = { 0, 0, 1 };
	double D[] = { 0, -7, 1 };

	double _1[] = { 0.5, 0, 1 };
	double _2[] = { 0.5, -0.5, 1 };
	double _3[] = { 3.5, -0.5, 1 };
	double _4[] = { 3.5, 0, 1 };
	double _5[] = { 6, 0, 1 };
	double _6[] = { 6, -1, 1 };
	double _7[] = { 9, -1, 1 };

	double _11[] = { 0.5, -7, 1 };
	double _21[] = { 0.5, -6.5, 1 };
	double _31[] = { 3.5, -6.5, 1 };
	double _41[] = { 3.5, -7, 1 };
	double _51[] = { 6, -7, 1 };
	double _61[] = { 6, -6, 1 };
	double _71[] = { 9, -6, 1 };

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	glVertex3dv(A);
	glColor3d(a, b, c);
	glVertex3dv(_2);
	glColor3d(a, b, c);
	glVertex3dv(_1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(A);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(D);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_21);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(D);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_11);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_21);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_21);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(A);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_21);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_2);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_31);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_31);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_2);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_3);
	glEnd();

	glBegin(GL_TRIANGLES); //???
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_31);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_3);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_61);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_31);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_41);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_51);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_51);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_31);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_61);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_61);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_71);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_7);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_61);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_7);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_6);
	glEnd();

	glBegin(GL_TRIANGLES); //???
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_61);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_6);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_3);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_6);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_3);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_4);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, -1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_6);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(_5);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(_4);
	glEnd();
}

void foot(double a1, double b1, double c1, double d1, int n)
{
	double z = 0;
	double z1 = 3;

	double _1[] = { d1, b1, z };
	double _2[] = { a1, b1, z };
	double _3[] = { a1, c1, z };
	double _4[] = { d1, c1, z };
	double _11[] = { d1, b1, z1 };
	double _21[] = { a1, b1, z1 };
	double _31[] = { a1, c1, z1 };
	double _41[] = { d1, c1, z1 };

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_11);
	glTexCoord2d(0, 1);
	glVertex3dv(_21);
	glTexCoord2d(1, 1);
	glVertex3dv(_31);
	glTexCoord2d(1, 0);
	glVertex3dv(_41);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	if(n == 1) glNormal3d(0, 1, 0);
	if (n == 2) glNormal3d(0, -1, 0);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_11);
	glTexCoord2d(0, 1);
	glVertex3dv(_1);
	glTexCoord2d(1, 1);
	glVertex3dv(_2);
	glTexCoord2d(1, 0);
	glVertex3dv(_21);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	if (n == 1) glNormal3d(1, 0, 0);
	if (n == 2) glNormal3d(-1, 0, 0);
	glNormal3d(1, 0, 0);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_21);
	glTexCoord2d(0, 1);
	glVertex3dv(_2);
	glTexCoord2d(1, 1);
	glVertex3dv(_3);
	glTexCoord2d(1, 0);
	glVertex3dv(_31);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId); //???
	glBegin(GL_QUADS);
	if (n == 1) glNormal3d(-3, 1, 0);
	if (n == 2)glNormal3d(1, -3, 0);
	glNormal3d(0, -1, 0);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_31);
	glTexCoord2d(0, 1);
	glVertex3dv(_3);
	glTexCoord2d(1, 1);
	glVertex3dv(_4);
	glTexCoord2d(1, 0);
	glVertex3dv(_41);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(-1, 0, 0); //1 0 0
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_41);
	glTexCoord2d(0, 1);
	glVertex3dv(_4);
	glTexCoord2d(1, 1);
	glVertex3dv(_1);
	glTexCoord2d(1, 0);
	glVertex3dv(_11);
	glEnd();
}

void paw(double x1, double x2, double x3, double x4, double x5) {

	double _1[] = { -1, x1, 0 };
	double _2[] = { 0, x1, 0 };
	double _3[] = { 0, x2, 0 };
	double _4[] = { 1, x2, 0 };
	double _5[] = { 1, x3, 0 };
	double _6[] = { 3.5, x3, 0 };
	double _7[] = { 3.5, x4, 0 };
	double _8[] = { -1, x4, 0 };
	double _9[] = { -1, x5, 0 };
	double _10[] = { -2, x5, 0 };
	double _11[] = { -2, x2, 0 };
	double _12[] = { -1, x2, 0 };
	double _0[] = { -1, x3, 0 };

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_1);
	glTexCoord2d(0, 1);
	glVertex3dv(_2);
	glTexCoord2d(1, 1);
	glVertex3dv(_3);
	glTexCoord2d(1, 0);
	glVertex3dv(_12);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_12);
	glTexCoord2d(0, 1);
	glVertex3dv(_4);
	glTexCoord2d(1, 1);
	glVertex3dv(_5);
	glTexCoord2d(1, 0);
	glVertex3dv(_0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_0);
	glTexCoord2d(0, 1);
	glVertex3dv(_6);
	glTexCoord2d(1, 1);
	glVertex3dv(_7);
	glTexCoord2d(1, 0);
	glVertex3dv(_8);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_11);
	glTexCoord2d(0, 1);
	glVertex3dv(_12);
	glTexCoord2d(1, 1);
	glVertex3dv(_9);
	glTexCoord2d(1, 0);
	glVertex3dv(_10);
	glEnd();
}

void paw2(double x1, double x2, double x3, double x4, double x5) {

	double _1[] = { 5, x1, 0 };
	double _2[] = { 6, x1, 0 };
	double _3[] = { 6, x2, 0 };
	double _4[] = { 7, x2, 0 };
	double _5[] = { 7, x1, 0 };
	double _6[] = { 8, x1, 0 };
	double _7[] = { 8, x2, 0 };
	double _8[] = { 9, x2, 0 };
	double _9[] = { 9, x3, 0 };
	double _10[] = { 10, x3, 0 };
	double _11[] = { 10, x4, 0 };
	double _12[] = { 6, x4, 0 };
	double _13[] = { 6, x5, 0 };
	double _14[] = { 5, x5, 0 };
	double _0[] = { 6, x3, 0 };

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);;
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_1);
	glTexCoord2d(0, 1);
	glVertex3dv(_2);
	glTexCoord2d(1, 1);
	glVertex3dv(_13);
	glTexCoord2d(1, 0);
	glVertex3dv(_14);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	//glNormal3d(0, 0, -1);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_12);
	glTexCoord2d(0, 1);
	glVertex3dv(_11);
	glTexCoord2d(1, 1);
	glVertex3dv(_10);
	glTexCoord2d(1, 0);
	glVertex3dv(_0);
	glEnd();

	
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_0);
	glTexCoord2d(0, 1);
	glVertex3dv(_9);
	glTexCoord2d(1, 1);
	glVertex3dv(_8);
	glTexCoord2d(1, 0);
	glVertex3dv(_3);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glTexCoord2d(0, 0);
	glVertex3dv(_4);
	glTexCoord2d(0, 1);
	glVertex3dv(_5);
	glTexCoord2d(1, 1);
	glVertex3dv(_6);
	glTexCoord2d(1, 0);
	glVertex3dv(_7);
	glEnd();
}

void frog()
{
	//glColor3d(0.423, 0.613, 0.187);

	double h1 = 6;
	double A[] = { 0, 0, 1 }; //belly
	double B[] = { 9, 0, 1 };
	double C[] = { 9, -7, 1 };
	double D[] = { 0, -7, 1 };

	double A1[] = { 0, 0, h1 }; //back
	double B1[] = { 9, 0, h1 };
	double C1[] = { 9, -7, h1 };
	double D1[] = { 0, -7, h1 };

	double F1[] = { 1, 0, h1 }; //eye1
	double G1[] = { 1, -3, h1 };
	double E1[] = { 3, -3, h1 };
	double S1[] = { 3, 0, h1 };

	double M1[] = { 1, -7, h1 }; //eye2
	double H1[] = { 1, -4, h1 };
	double K1[] = { 3, -4, h1 };
	double N1[] = { 3, -7, h1 };

	//eye2(1, 3, -4, -7); //eye2
	belly();

	//face
	glBegin(GL_QUADS);
	glNormal3d(-1, 0, 0);
	glColor3d(a, b, c);
	//glTexCoord2d(0, 0);
	glVertex3dv(A);
	//glTexCoord2d(1, 0);
	glVertex3dv(A1);
	//glTexCoord2d(1, 1);
	glVertex3dv(D1);
	//glTexCoord2d(0, 1);
	glVertex3dv(D);
	glEnd();

	//back
	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(D1);
	glVertex3dv(A1);
	glVertex3dv(F1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(F1);
	//glColor3d(0.5, 0.5, 0.2);
	glVertex3dv(M1);
	//glColor3d(0.1, 0.4, 0.4);
	glVertex3dv(D1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(H1);
	glVertex3dv(G1); ;
	glVertex3dv(K1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(K1);
	glVertex3dv(G1);
	glVertex3dv(E1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(N1);
	glVertex3dv(C1);
	glVertex3dv(S1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3d(0, 0, 1);
	glColor3d(a, b, c);
	glVertex3dv(C1);
	glVertex3dv(S1);
	glVertex3dv(B1);
	glEnd();

	//glPushMatrix();

	//glPopMatrix();
	//eye(1, 3, -4, -7); //eye2
	//eye(1, 3, -3, 0); //eye1

	side(-7);
	side(0);

	buttocks();

	//leg
	foot(10, -6, -9, 6, 1);
	foot(10, -1, 2, 6, 2);
	foot(3.5, -6.5, -8.5, 0.5, 1);
	foot(3.5, -0.5, 1.5, 0.5, 2);

	//��������
	paw(-4.5, -5.5, -6.5, -8.5, -7.5);
	paw(-2.5, -1.5, -0.5, 1.5, 0.5);

	//������
	paw2(-11, -10, -9, -6, -8);
	paw2(4, 3, 2, -1, 1);

	eye(1, 3, -3, 0); //eye1
	eye(1, 3, -4, -7); //eye2
}

float anim_h = 0.01;
double anim_t = anim_h;

GLUquadricObj* quadratic;    // ����� ��� �������� ������� Quadratic ( ����� )

Vector3 BCurve(Vector3* points, int n, double t)		//������ �������
{
	Vector3 res(0, 0, 0);
	for (int i = 0; i < n;i++)
		res = res + points[i] * Bern(i, n - 1, t) * 0.437;
	return res;
}

void Render(OpenGL *ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	if (colour)
	{
		a = 0.423;
		b = 0.613;
		c = 0.187;
	}
	else 
	{
		a = 0.823;
		b = 0.549;
		c = 0.587;
	}

	if (!ground && drive) {
		double A[2] = { -10, -10 };
		double B1[2] = { 10, -10 };
		double C[2] = { 10, 10 };
		double D[2] = { -10, 10 };
		
		glPushMatrix();
		glTranslated(2.0, -4.5, -0.1);
		glColor3d(0.6, 0.6, 0.6);
		glBegin(GL_QUADS);
		glNormal3d(0, 0, 1);
		glTexCoord2d(0, 0);
		glVertex2dv(A);
		glTexCoord2d(1, 0);
		glVertex2dv(B1);
		glTexCoord2d(1, 1);
		glVertex2dv(C);
		glTexCoord2d(0, 1);
		glVertex2dv(D);
        glEnd();
		
		glPopMatrix();
	}

	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  

	if (!ooooops && drive)
	{
	
		if (B) {
			glPushMatrix();
			glTranslated(-.0, -.0, 3);
			frog();
			glEnd();
			//lPopMatrix();
		}
		B = !B;
		ooooops = !ooooops;
	}
	if (!B)	ooooops = !ooooops;

	if (!drive)
	{
		
		Vector3 p[4];
		p[0] = Vector3(0, 0, 0);
		p[1] = Vector3(-15, 20, 40);
		p[2] = Vector3(25, 50, -10);
		p[3] = Vector3(-25, -6, 10);

		glPushMatrix();
		Vector3 pos = BCurve(p, 4, anim_t);
		Vector3 pre_pos = BCurve(p, 4, anim_t - anim_h);
		Vector3 dir = (pos - pre_pos).normolize();

		Vector3 orig(1, 0, 0);
		Vector3 rotX(dir.X(), dir.Y(), 0);
		rotX = rotX.normolize();
		double cosU = (orig.X() * rotX.X()) + (orig.Y() * rotX.Y()) + (orig.Z() * rotX.Z());
		Vector3 vecpr = orig.vectProisvedenie(rotX);

		double sinSign = vecpr.Z() / abs(vecpr.Z());
		double U = acos(cosU) * 180 / 3.141592 * sinSign;

		double cosZU = (0 * dir.X()) + (0 * dir.Y()) + (1 * dir.Z());
		double ZU = acos(dir.Z()) * 180.0 / M_PI - 90;

		glTranslated(pos.X(), pos.Y(), pos.Z());
		glRotated(U, 0, 0, 1);
		glRotated(ZU, 0, 1, 0);

		frog();
		glPopMatrix();

		glDisable(GL_LIGHTING);
		glColor3d(1, 0.3, 0);

		glBegin(GL_LINE);
		glVertex3dv(pos.toArray());
		glVertex3dv((pos + dir.normolize() * 3).toArray());


		glEnd();
		anim_t += anim_h;
		if (anim_t > 1) anim_h = -anim_h;
		if (anim_t < 0) anim_h = -anim_h;

		if (line) {
			glBegin(GL_LINE_STRIP);
			glDisable(GL_LIGHTING);
			glColor3d(1, 0, 1);
			for (double t = 0; t <= 1; t += 0.01) {
				glVertex3dv(BCurve(p, 4, t).toArray());
			}
			glEnd();
		}
	}
	else 
	{
		//glTranslatef(2.5f, -3.5f, 6.0f);  // ����� ������
		//// ����� � ������ �������� .5 � ������� 2
		////gluCylinder(quadratic, 1.0f, 0.0f, 3.0f, 32, 32);
		//gluCylinder(quadratic, 1.5f, 0,.0f, 4.0f, 32);
		//
		frog();
	}

	////������ ��������� ���������� ��������
	//double A[2] = { -4, -4 };
	//double B[2] = { 4, -4 };
	//double C[2] = { 4, 4 };
	//double D[2] = { -4, 4 };

	//glBindTexture(GL_TEXTURE_2D, texId);

	//glColor3d(0.6, 0.6, 0.6);
	//glBegin(GL_QUADS);

	//glNormal3d(0, 0, 1);
	//glTexCoord2d(0, 0);
	//glVertex2dv(A);
	//glTexCoord2d(1, 0);
	//glVertex2dv(B);
	//glTexCoord2d(1, 1);
	//glVertex2dv(C);
	//glTexCoord2d(0, 1);
	//glVertex2dv(D);

	//glEnd();
	//����� ��������� ���������� ��������


   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(500, 200);
	rec.setPosition(10, ogl->getHeight() - 200 - 10);


	std::stringstream ss;
	ss << "S - ����� �����" << std::endl;
	ss << "X - ���/���� ��� ��������" << std::endl;
	ss << "Z - ���/���� ��������" << std::endl;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}