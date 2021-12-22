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

double delta_time = 0.009;
double t_max = delta_time;

//��������� ���������
GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
GLfloat sh = 0.1f * 256;

double formula_Beze_3(double p0, double p1, double p2, double p3, double t)
{
	return p0 * pow((1 - t), 3) + 3 * t * p1 * pow((1 - t), 2) + 3 * t * t * p2 * (1 - t) + pow(t, 3) * p3;
}

double formula_Ermit_3(double p1, double p4, double r1, double r4, double t)
{
	return p1 * (2 * pow(t, 3) - 3 * t * t + 1) + p4 * (-2 * pow(t, 3) + 3 * t * t) + r1 * (pow(t, 3) - 2 * t * t + t) + r4 * (pow(t, 3) - t * t);
}


// ��������� ����� ����� 3 �������
void renderBeze3Line(double* P1, double* P2, double* P3, double* P4) {
	double P[3];

	// �������� ������� P1P2 � P2P3 � �3�4
	glBegin(GL_LINES);
	glColor3d(0, 0, 0);
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P3);
	glVertex3dv(P4);
	glEnd();

	glLineWidth(4);
	glColor3d(0, 1, 0);

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += 0.01)
	{
		P[0] = formula_Beze_3(P1[0], P2[0], P3[0], P4[0], t);
		P[1] = formula_Beze_3(P1[1], P2[1], P3[1], P4[1], t);
		P[2] = formula_Beze_3(P1[2], P2[2], P3[2], P4[2], t);
		glVertex3dv(P);
	}
	glEnd();

	glColor3d(1, 0, 1);
	glLineWidth(1);
	for (double t = 0; t <= t_max; t += 0.01)
	{
		P[0] = formula_Beze_3(P1[0], P2[0], P3[0], P4[0], t);
		P[1] = formula_Beze_3(P1[1], P2[1], P3[1], P4[1], t);
		P[2] = formula_Beze_3(P1[2], P2[2], P3[2], P4[2], t);
	}

	double t = t_max;
	glColor3d(1, 0, 0);

	glBegin(GL_POINTS);
	glVertex3dv(P1);
	glVertex3dv(P4);
	glEnd();
}
void renderErmit3Line(double* P1, double* P4, double* R1, double* R4) {
	double P[3];

	// �������� ������� R1 � R4
	glBegin(GL_LINES);
	glColor3d(0, 0, 0);
	glVertex3dv(P1);
	glVertex3dv(R1);
	glVertex3dv(P4);
	glVertex3dv(R4);
	glEnd();

	glLineWidth(5);
	glColor3d(0, 0.4, 0);

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += 0.01)
	{
		P[0] = formula_Ermit_3(P1[0], P4[0], R1[0], R4[0], t);
		P[1] = formula_Ermit_3(P1[1], P4[1], R1[1], R4[1], t);
		P[2] = formula_Ermit_3(P1[2], P4[2], R1[2], R4[2], t);
		glVertex3dv(P);
	}
	glEnd();

	glColor3d(1, 0, 1);
	glLineWidth(1);
	for (double t = 0; t <= t_max; t += 0.01)
	{
		P[0] = formula_Ermit_3(P1[0], P4[0], R1[0], R4[0], t);
		P[1] = formula_Ermit_3(P1[1], P4[1], R1[1], R4[1], t);
		P[2] = formula_Ermit_3(P1[2], P4[2], R1[2], R4[2], t);
	}

	double t = t_max;
	glColor3d(1, 0, 0);

	glBegin(GL_POINTS);
	glVertex3dv(P1);
	glVertex3dv(P4);
	glEnd();
}


// ����� ��� ���� ������ �����
double P0_1[] = { 0,0,-4 };
double P1_1[] = { 0,10,39	 };
double P2_1[] = { 0,7,12 };
double P3_1[] = { 0,2,-13 };

double P0_2[] = { 0,0,-4 };
double P1_2[] = { 0,-17,20 };
double P2_2[] = { 0,-7,-12 };
double P3_2[] = { 0,2,-13 };

// ����� ��� ���� ������ ������
double P11[] = { 0,-20,2 };
double R11[] = { 0,-26,7 };
double P41[] = { 0,-24,-2.5 };
double R41[] = { 0,-34,-5 };

double P12[] = { 0,-20,2 };
double R12[] = { 0,-12,7 };
double P42[] = { 0,-10,-2.5 };
double R42[] = { 0,-6,-5 };


bool textureMode = true;
bool lightMode = true;

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

		pos.setCoords(camDist * cos(fi2) * cos(fi1),
			camDist * cos(fi2) * sin(fi1),
			camDist * sin(fi2));

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
		s.scale = s.scale * 0.08;
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
			c.scale = c.scale * 1.5;
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

void mouseEvent(OpenGL* ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01 * dx;
		camera.fi2 += -0.01 * dy;
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

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02 * dy);
	}


}

void mouseWheelEvent(OpenGL* ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01 * delta;

}

void keyDownEvent(OpenGL* ogl, int key)
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
}

void keyUpEvent(OpenGL* ogl, int key)
{

}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL* ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);


	//������ ����������� ���������  (R G B)
	RGBTRIPLE* texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
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


void Render(OpenGL* ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

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

	renderBeze3Line(P0_1, P1_1, P2_1, P3_1);
	renderBeze3Line(P0_2, P1_2, P2_2, P3_2);
	renderErmit3Line(P11, P41, R11, R41);
	renderErmit3Line(P12, P42, R12, R42);

	//����� ���������


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
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;

	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////