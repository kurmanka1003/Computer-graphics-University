

#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <cmath>


double ermit(double p1, double p4, double r1, double r4, double t) {
	return p1 * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p4 * (-2 * pow(t, 3) + 3 * pow(t, 2)) + r1 * (pow(t, 3) - 2 * pow(t, 2) + t) + r4 * (pow(t, 3) - pow(t, 2));
}

double bezier(double p0, double p1, double p2, double p3, double t) {
	return pow(1 - t, 3) * p0 + 3 * t * pow(1 - t, 2) * p1 + 3 * pow(t, 2) * (1 - t) * p2 + pow(t, 3) * p3;
}

void draw_vector(double r1[3], double r2[3]) {
	glColor3d(1, 0.5, 0.2);

	glBegin(GL_LINES);
	glVertex3dv(r1);
	glVertex3dv(r2);
	glEnd();
}

void draw_ermit_kurva(double P1[3], double P4[3], double R1[3], double R4[3]) {
	glColor3d(0.3, 0.7, 0);

	glBegin(GL_LINE_STRIP);

	for (double t = 0; t <= 1.0001; t += 0.01)
	{
		double P[3];

		//Ermit
		P[0] = ermit(P1[0], P4[0], R1[0] - P1[0], R4[0] - P4[0], t);
		P[1] = ermit(P1[1], P4[1], R1[1] - P1[1], R4[1] - P4[1], t);
		P[2] = ermit(P1[2], P4[2], R1[2] - P1[2], R4[2] - P4[2], t);

		glVertex3dv(P);
	}
	glEnd();
	draw_vector(P1, R1);
	draw_vector(P4, R4);

	glColor3d(0.5, 0.5, 1);
	glLineWidth(3);
	glBegin(GL_POINTS);
	glVertex3dv(P1);
	glVertex3dv(P4);
	glVertex3dv(R1);
	glVertex3dv(R4);
	glEnd();
}

void draw_bezier_curve(double p0[3], double p1[3], double p2[3], double p3[3]) {
	glColor3d(1, 0, 1);

	glBegin(GL_LINE_STRIP);

	for (double t = 0; t <= 1.0001; t += 0.01)
	{
		double P[3];

		//Bezier
		P[0] = bezier(p0[0], p1[0], p2[0], p3[0], t);
		P[1] = bezier(p0[1], p1[1], p2[1], p3[1], t);
		P[2] = bezier(p0[2], p1[2], p2[2], p3[2], t);

		glVertex3dv(P);
	}
	glEnd();

	glColor3d(0.2, 0.2, 0.8);
	glBegin(GL_LINE_STRIP);
	glVertex3dv(p0);
	glVertex3dv(p1);
	glVertex3dv(p2);
	glVertex3dv(p3);
	glEnd();

	glColor3d(0.5, 0.5, 1);
	glLineWidth(3);
	glBegin(GL_POINTS);
	glVertex3dv(p0);
	glVertex3dv(p1);
	glVertex3dv(p2);
	glVertex3dv(p3);
	glEnd();
}

void Render(double delta_time) {    
	
	//Ermit points
	double P1_ermit[] = { 0,0,0 }; 
	double P4_ermit[] = { -4,6,7 };
	
	double R1[] = {-1, 0, 1};
	double R4[] = {1, 6, 2};

	double P1_ermit2[] = { 1,1,1 };
	double P4_ermit2[] = { 2,2,2 };

	double R1_2[] = { 2, 2, 8 };
	double R4_2[] = { 3, 2, 2 };

	//Bezier points
	double P0[] = {0, 0, 0};
	double P1[] = { 2, 2, 3 };
	double P2[] = { 4, 5, 2 };
	double P3[] = { 7, 8, 0 };

	double P4[] = { -1, 5, 1 };
	double P5[] = { -2, 4, 3 };
	double P6[] = { -4, 2, 3 };
	double P7[] = { -6, 7, 1 };


	glLineWidth(3);
	draw_ermit_kurva(P1_ermit2, P4_ermit2, R1_2, R4_2);
	draw_ermit_kurva(P1_ermit, P4_ermit, R1, R4);

	draw_bezier_curve(P0, P1, P2, P3);
	draw_bezier_curve(P4, P5, P6, P7);

	glEnd;
}   

