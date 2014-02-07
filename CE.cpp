#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <stdlib.h>
#include <math.h>   //to use sin and cos
//#include <math3d.h>
#include "vec3f.cpp"
#include "imageloader.cpp"

		

#define GL_PI 3.1415f

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

using namespace std;

GLuint textureID;
unsigned int texWidth , texHeight ;
unsigned char* texData;

GLfloat xRot = 0.0f , yRot = -0.0f ,  zoom_out = -400.0f;
bool* keySpecialStates = new bool[256];

GLboolean pan =0 , rot = 1;

GLint modifier;

bool* keyNormalStates = new bool[256];

GLfloat xInitial=0 , yInitial=0 , xFinal=0 , yFinal=0 , x_trans=0.0f , y_trans=0.0f , light_y = 100.0f , angle = 0.0f , light_x = 100.0f , light_z = 400.0f;

void keyOperations(void)
{
	if(keySpecialStates[GLUT_KEY_LEFT] == 1)
	{
		printf("left \n");
		//yRot = yRot+1;
		light_x = light_x - 10;
	}
	if(keySpecialStates[GLUT_KEY_RIGHT] ==1)
	{
		printf("right \n");
		//yRot = yRot-1;
		light_x = light_x +10;
	}
	if(keySpecialStates[GLUT_KEY_UP] ==1)
	{
		printf("up \n");
		//xRot = xRot-1;
		light_z = light_z-10;
	}
	if(keySpecialStates[GLUT_KEY_DOWN] ==1)
	{
		printf("down \n");
		//xRot = xRot+1;
		light_z = light_z+10;
	}
	if(keyNormalStates['z'] == 1)
	{
		printf("z \n");
		zoom_out = zoom_out + 10;
	}

	if(keyNormalStates['x'] == 1)
	{
		printf("x \n");
		zoom_out = zoom_out - 10;
	}

	if(keyNormalStates['o'] == 1)
	{
		printf("o \n");
		light_y = light_y + 10;
	}

	if(keyNormalStates['l'] == 1)
	{
		printf("l \n");
		light_y = light_y - 10;
	}
}

GLuint loadTexture(Image *image)
{
	GLuint textureId;
	glGenTextures(1 , &textureId);

	glBindTexture(GL_TEXTURE_2D , textureId);
	glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB , image->width , image->height , 0 , GL_RGB , GL_UNSIGNED_BYTE , image->pixels);

	return textureId;
}

	
void SetupRC(void)
{
	glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	//glDisable(GL_LIGHTING);
	//glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	

}

void computeNormals( GLfloat normal1[3] , GLfloat normal2[3] , GLfloat normal3[3] )
{
	GLfloat v1[] = {normal2[0] , normal2[1] , normal2[2]};
	GLfloat v2[] = {normal1[0] , normal1[1] , normal1[2]};
	GLfloat v4[] = {normal3[0] , normal3[1] , normal3[2]};
	
	GLfloat a1 = v2[0] - v1[0];
	GLfloat a2 = v2[1] - v1[1];
	GLfloat a3 = v2[2] - v1[2];

	GLfloat b1 = v4[0] - v1[0];
	GLfloat b2 = v4[1] - v1[1];
	GLfloat b3 = v4[2] - v1[2];

	GLfloat nx = a2*b3 - b2*a3;
	GLfloat ny = b2*a3 - a1*b3;
	GLfloat nz = a1*b2 - b1*a2;

	glNormal3f(nx , ny , nz);

}

void RenderScene( void )
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	glPushMatrix();	

	glTranslatef(x_trans, y_trans , zoom_out);	
	glRotatef(xRot , 1.0f , 0.0f , 0.0f);
	glRotatef(yRot , 0.0f , 1.0f , 0.0f);

	glColor3f(1.0f , 1.0f , 1.0f);
	
	glBegin(GL_LINES);

	glVertex2f(-1000.0f , 0.0f);
	glVertex2f(1000.0f , 0.0f );

	glVertex2f(0.0f , 1000.0f);
	glVertex2f(0.0f , -1000.0f );

	glVertex3f(0.0f , 0.0f , -1000.0f);
	glVertex3f(0.0f , 0.0f , 1000.0f);

	glEnd();

	

	GLfloat light0_ambient[] = {0.0f, 0.0f , 0.0f , 1.0f};
	GLfloat light0_diffuse[] = {0.3f , 0.5f , 0.0f , 1.0};
	GLfloat light0_specular[] = {1.0f , 1.0f , 1.0f , 1.0};

	GLfloat light0_position[] = { light_x , 160 , 285 , 1.0f};


	glLightfv(GL_LIGHT0 , GL_AMBIENT , light0_ambient);
	glLightfv(GL_LIGHT0 , GL_DIFFUSE ,light0_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0 , GL_POSITION , light0_position);

	GLfloat light1_ambient[] = {0.0f, 0.0f , 0.0f , 1.0f};
	GLfloat light1_diffuse[] = {0.0f , 0.3f , 0.5f , 1.0};
	GLfloat light1_specular[] = {1.0f , 1.0f , 1.0f , 1.0};

	GLfloat light1_position[] = { 0 , 160 , light_z , 1.0f};


	glLightfv(GL_LIGHT1 , GL_AMBIENT , light1_ambient);
	glLightfv(GL_LIGHT1 , GL_DIFFUSE ,light1_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT1 , GL_POSITION , light1_position);
	
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex2f(-50.0f , 160+100*sin(410*GL_PI/180));
		glVertex2f(50.0f , 160+100*sin(410*GL_PI/180));
		glVertex2f(50.0f , 270.0f);
		glVertex2f(-50.0f , 270.0f);

		glVertex2f(-50.0f , 50.0f);
		glVertex2f(50.0f , 50.0f);
		glVertex2f(50.0f, 160+100*sin(310*GL_PI/180));
		glVertex2f(-50.0f , 160+100*sin(310*GL_PI/180));

		glVertex2f(-50.0f , 160+100*sin(350*GL_PI/180));
		glVertex2f(50.0f , 160+100*sin(350*GL_PI/180));
		glVertex2f(50.0f, 160+100*sin(370*GL_PI/180));
		glVertex2f(-50.0f ,160+100*sin(370*GL_PI/180));

		glVertex2f(-50.0f , 50.0f );
		glVertex2f(-25.0f ,	50.0f);
		glVertex2f(-25.0f,	270.0f);
		glVertex2f(-50.0f ,	270.0f);

		glVertex3f(-200.0f , 260.0f , 200.0f );
		glVertex3f(-200.0f , 260.0f , 100.0f);
		glVertex3f(-200.0f,	 270.0f, 100.0f  );
		glVertex3f(-200.0f , 270.0f , 200.0f);
		
		glVertex3f(-200.0f , 50.0f , 200.0f );
		glVertex3f(-200.0f , 50.0f , 100.0f );
		glVertex3f(-200.0f,	 60.0f , 100.0f );
		glVertex3f(-200.0f , 60.0f , 200.0f );


	glEnd();

	glPushMatrix();
		glTranslatef(-200.0f , 160.0f , 200.0f);
			glBegin(GL_QUADS);
				for(int theta = 270 ; theta <= 440 ; theta=theta+10)
				{
				glVertex3f(0.0f , 100.0f*sin((theta+10)*GL_PI/180) , 100.0f*cos((theta+10)*GL_PI/180));
				glVertex3f(0.0f , 100.0f*sin((theta)*GL_PI/180) , 100.0f*cos((theta)*GL_PI/180));
				glVertex3f(0.0f , 110.0f*sin((theta)*GL_PI/180) , 110.0f*cos((theta)*GL_PI/180));
				glVertex3f(0.0f , 110.0f*sin((theta+10)*GL_PI/180) , 110.0f*cos((theta+10)*GL_PI/180));
				}

			glEnd();
	glPopMatrix();


	
	GLfloat material_diffuse[] = { 1.0f , 1.0f , 1.0f , 0.3f };
	glMaterialfv(GL_FRONT , GL_DIFFUSE , material_diffuse);
	
	glBegin(GL_QUADS);
		glNormal3f(0.0f , 0.0f , 1.0f);
		
		glVertex2f(-200.0f , -200.0f);
		glVertex2f(200.0f , -200.0f);
		glVertex2f(200.0f , 400.0f);
		glVertex2f(-200.0f , 400.0f);

		glNormal3f(1.0f , 0.0f , 0.0f);
		
		glVertex3f(-200.0f , -200.0f , 0.0f);
		glVertex3f(-200.0f , 400.0f , 0.0f);
		glVertex3f(-200.0f , 400.0f , 400.0f);
		glVertex3f(-200.0f , -200.0f , 400.0f);

	glEnd();

	
	glPushMatrix();
		glTranslatef(0.0f , 55.0f , 150.0f);
		glScalef(10.0f, 1.0f , 10.0f);
		
		glNormal3f(0.0f , 1.0f, 0.0f);
		glutSolidCube(10.0f);

		glTranslatef(0.0f , 210.0f , 0.0f);
		glNormal3f(0.0f , -1.0f, 0.0f);
		glutSolidCube(10.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f , 160.0f , 200.0f);
	GLfloat X1circle , Y1circle , Z1circle , X2circle , Y2circle , Z2circle ;
	GLfloat X1circle2 , Y1circle2 , Z1circle2 , X2circle2 , Y2circle2 , Z2circle2 ;
	
		glBegin(GL_QUADS);
			for(int theta=270 ; theta <=300 ; theta = theta+10)
			{
				X1circle = -50;
				Y1circle = 100*sin(theta*GL_PI/180);
				Z1circle = 100*cos(theta*GL_PI/180);

				X2circle = 50;
				Y2circle = 100*sin((theta+10)*GL_PI/180);
				Z2circle = 100*cos((theta+10)*GL_PI/180);

				X1circle2 = -50;
				Y1circle2 = 110*sin(theta*GL_PI/180);
				Z1circle2 = 110*cos(theta*GL_PI/180);

				X2circle2 = 50;
				Y2circle2 = 110*sin((theta+10)*GL_PI/180);
				Z2circle2 = 110*cos((theta+10)*GL_PI/180);

				//inner face of C

				GLfloat normal1[] = {50 , Y1circle2 , Z1circle2};
				GLfloat normal2[] = {50 , Y2circle2 , Z2circle2};
				GLfloat normal3[] = {-50 , Y2circle2 , Z2circle2};
				
				

				glVertex3f(X1circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y2circle , Z2circle);
				glVertex3f(X1circle , Y2circle , Z2circle);

				computeNormals(normal1 , normal2 , normal3);

				//outer face of C
				glVertex3f(X1circle2 , Y1circle2 , Z1circle2);
				glVertex3f(X2circle2, Y1circle2 , Z1circle2);
				glVertex3f(X2circle2 , Y2circle2 , Z2circle2);
				glVertex3f(X1circle2 , Y2circle2 , Z2circle2);

				// right Side face of C

				glNormal3f( 1.0f , 0.0f , 0.0f);
				glVertex3f(50 , Y1circle , Z1circle);
				glVertex3f(50, Y1circle2 , Z1circle2);
				glVertex3f(50 , Y2circle2 , Z2circle2);
				glVertex3f(50 , Y2circle , Z2circle);

				//left side face of C

				glNormal3f( -1.0f , 0.0f , 0.0f);
				
				glVertex3f(-50 , Y1circle , Z1circle);
				glVertex3f(-50, Y1circle2 , Z1circle2);
				glVertex3f(-50 , Y2circle2 , Z2circle2);
				glVertex3f(-50 , Y2circle , Z2circle);			

			}


			for(int theta=350 ; theta <=360 ; theta = theta+10)
			{
				X1circle = -50;
				Y1circle = 100*sin(theta*GL_PI/180);
				Z1circle = 100*cos(theta*GL_PI/180);

				X2circle = 50;
				Y2circle = 100*sin((theta+10)*GL_PI/180);
				Z2circle = 100*cos((theta+10)*GL_PI/180);

				X1circle2 = -50;
				Y1circle2 = 110*sin(theta*GL_PI/180);
				Z1circle2 = 110*cos(theta*GL_PI/180);

				X2circle2 = 50;
				Y2circle2 = 110*sin((theta+10)*GL_PI/180);
				Z2circle2 = 110*cos((theta+10)*GL_PI/180);

				//inner face of C

				GLfloat normal1[] = {50 , Y1circle2 , Z1circle2};
				GLfloat normal2[] = {50 , Y2circle2 , Z2circle2};
				GLfloat normal3[] = {-50 , Y2circle2 , Z2circle2};
				
				

				glVertex3f(X1circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y2circle , Z2circle);
				glVertex3f(X1circle , Y2circle , Z2circle);

				computeNormals(normal1 , normal2 , normal3);

				//outer face of C
				glVertex3f(X1circle2 , Y1circle2 , Z1circle2);
				glVertex3f(X2circle2, Y1circle2 , Z1circle2);
				glVertex3f(X2circle2 , Y2circle2 , Z2circle2);
				glVertex3f(X1circle2 , Y2circle2 , Z2circle2);

				// right Side face of C

				glNormal3f( 1.0f , 0.0f , 0.0f);
				glVertex3f(50 , Y1circle , Z1circle);
				glVertex3f(50, Y1circle2 , Z1circle2);
				glVertex3f(50 , Y2circle2 , Z2circle2);
				glVertex3f(50 , Y2circle , Z2circle);

				//left side face of C

				glNormal3f( -1.0f , 0.0f , 0.0f);
				
				glVertex3f(-50 , Y1circle , Z1circle);
				glVertex3f(-50, Y1circle2 , Z1circle2);
				glVertex3f(-50 , Y2circle2 , Z2circle2);
				glVertex3f(-50 , Y2circle , Z2circle);			

			}


			for(int theta=410 ; theta <=440 ; theta = theta+10)
			{
				X1circle = -50;
				Y1circle = 100*sin(theta*GL_PI/180);
				Z1circle = 100*cos(theta*GL_PI/180);

				X2circle = 50;
				Y2circle = 100*sin((theta+10)*GL_PI/180);
				Z2circle = 100*cos((theta+10)*GL_PI/180);

				X1circle2 = -50;
				Y1circle2 = 110*sin(theta*GL_PI/180);
				Z1circle2 = 110*cos(theta*GL_PI/180);

				X2circle2 = 50;
				Y2circle2 = 110*sin((theta+10)*GL_PI/180);
				Z2circle2 = 110*cos((theta+10)*GL_PI/180);

				//inner face of C

				GLfloat normal1[] = {50 , Y1circle2 , Z1circle2};
				GLfloat normal2[] = {50 , Y2circle2 , Z2circle2};
				GLfloat normal3[] = {-50 , Y2circle2 , Z2circle2};
				
				

				glVertex3f(X1circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y2circle , Z2circle);
				glVertex3f(X1circle , Y2circle , Z2circle);

				computeNormals(normal1 , normal2 , normal3);

				//outer face of C
				glVertex3f(X1circle2 , Y1circle2 , Z1circle2);
				glVertex3f(X2circle2, Y1circle2 , Z1circle2);
				glVertex3f(X2circle2 , Y2circle2 , Z2circle2);
				glVertex3f(X1circle2 , Y2circle2 , Z2circle2);

				// right Side face of C

				glNormal3f( 1.0f , 0.0f , 0.0f);
				glVertex3f(50 , Y1circle , Z1circle);
				glVertex3f(50, Y1circle2 , Z1circle2);
				glVertex3f(50 , Y2circle2 , Z2circle2);
				glVertex3f(50 , Y2circle , Z2circle);

				//left side face of C

				glNormal3f( -1.0f , 0.0f , 0.0f);
				
				glVertex3f(-50 , Y1circle , Z1circle);
				glVertex3f(-50, Y1circle2 , Z1circle2);
				glVertex3f(-50 , Y2circle2 , Z2circle2);
				glVertex3f(-50 , Y2circle , Z2circle);			

			}

			for(int theta=310 ; theta <=340 ; theta = theta+10)
			{
				X1circle = -50;
				Y1circle = 100*sin(theta*GL_PI/180);
				Z1circle = 100*cos(theta*GL_PI/180);

				X2circle = -25;
				Y2circle = 100*sin((theta+10)*GL_PI/180);
				Z2circle = 100*cos((theta+10)*GL_PI/180);

				X1circle2 = -50;
				Y1circle2 = 110*sin(theta*GL_PI/180);
				Z1circle2 = 110*cos(theta*GL_PI/180);

				X2circle2 = -25;
				Y2circle2 = 110*sin((theta+10)*GL_PI/180);
				Z2circle2 = 110*cos((theta+10)*GL_PI/180);

				//inner face of C

				GLfloat normal1[] = {50 , Y1circle2 , Z1circle2};
				GLfloat normal2[] = {50 , Y2circle2 , Z2circle2};
				GLfloat normal3[] = {-50 , Y2circle2 , Z2circle2};
				
				

				glVertex3f(X1circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y2circle , Z2circle);
				glVertex3f(X1circle , Y2circle , Z2circle);

				computeNormals(normal1 , normal2 , normal3);

				//outer face of C
				glVertex3f(X1circle2 , Y1circle2 , Z1circle2);
				glVertex3f(X2circle2, Y1circle2 , Z1circle2);
				glVertex3f(X2circle2 , Y2circle2 , Z2circle2);
				glVertex3f(X1circle2 , Y2circle2 , Z2circle2);

				// right Side face of C

				glNormal3f( 1.0f , 0.0f , 0.0f);
				glVertex3f(-25 , Y1circle , Z1circle);
				glVertex3f(-25, Y1circle2 , Z1circle2);
				glVertex3f(-25 , Y2circle2 , Z2circle2);
				glVertex3f(-25 , Y2circle , Z2circle);

				//left side face of C

				glNormal3f( -1.0f , 0.0f , 0.0f);
				
				glVertex3f(-50 , Y1circle , Z1circle);
				glVertex3f(-50, Y1circle2 , Z1circle2);
				glVertex3f(-50 , Y2circle2 , Z2circle2);
				glVertex3f(-50 , Y2circle , Z2circle);			

			}

			for(int theta=370 ; theta <=400 ; theta = theta+10)
			{
				X1circle = -50;
				Y1circle = 100*sin(theta*GL_PI/180);
				Z1circle = 100*cos(theta*GL_PI/180);

				X2circle = -25;
				Y2circle = 100*sin((theta+10)*GL_PI/180);
				Z2circle = 100*cos((theta+10)*GL_PI/180);

				X1circle2 = -50;
				Y1circle2 = 110*sin(theta*GL_PI/180);
				Z1circle2 = 110*cos(theta*GL_PI/180);

				X2circle2 = -25;
				Y2circle2 = 110*sin((theta+10)*GL_PI/180);
				Z2circle2 = 110*cos((theta+10)*GL_PI/180);

				//inner face of C

				GLfloat normal1[] = {50 , Y1circle2 , Z1circle2};
				GLfloat normal2[] = {50 , Y2circle2 , Z2circle2};
				GLfloat normal3[] = {-50 , Y2circle2 , Z2circle2};
				
				

				glVertex3f(X1circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y1circle , Z1circle);
				glVertex3f(X2circle , Y2circle , Z2circle);
				glVertex3f(X1circle , Y2circle , Z2circle);

				computeNormals(normal1 , normal2 , normal3);

				//outer face of C
				glVertex3f(X1circle2 , Y1circle2 , Z1circle2);
				glVertex3f(X2circle2, Y1circle2 , Z1circle2);
				glVertex3f(X2circle2 , Y2circle2 , Z2circle2);
				glVertex3f(X1circle2 , Y2circle2 , Z2circle2);

				// right Side face of C

				glNormal3f( 1.0f , 0.0f , 0.0f);
				glVertex3f(-25 , Y1circle , Z1circle);
				glVertex3f(-25, Y1circle2 , Z1circle2);
				glVertex3f(-25 , Y2circle2 , Z2circle2);
				glVertex3f(-25 , Y2circle , Z2circle);

				//left side face of C

				glNormal3f( -1.0f , 0.0f , 0.0f);
				
				glVertex3f(-50 , Y1circle , Z1circle);
				glVertex3f(-50, Y1circle2 , Z1circle2);
				glVertex3f(-50 , Y2circle2 , Z2circle2);
				glVertex3f(-50 , Y2circle , Z2circle);			

			}

		glEnd();

		
	
	glPopMatrix();

	
	
	printf("Render scene working \n");
	
	glPopMatrix();
	glutSwapBuffers();

}


void ChangeSize(GLsizei w , GLsizei h)
{
	GLfloat aspectRatio;

	if(h==0)
	{
		h=1;
	}

	glViewport(0,0,w,h);

	//Resetting coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspectRatio = (GLfloat) w / (GLfloat) h ;

	gluPerspective(45.0f , aspectRatio , 1.0f , 2000.0f );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void processSpecialKeys( int key , int x , int y)
{
	keySpecialStates[key] = true;
	printf("Special Keys DOWN registered \n");	
	printf("%d \t %d \n",x,y);
	keyOperations();
	glutPostRedisplay();
}

void processSpecialKeysUp( int key , int x , int y)
{
	keySpecialStates[key] = false;
	printf("Special Keys UP registered \n");	
	printf("%d \t %d \n",x,y);
	//glutPostRedisplay();
}

void processKeyboardKeys( unsigned char key , int x , int y)
{
	keyNormalStates[key] = true;
	printf("Normal Keys DOWN registered \n");	
	printf(" %f \t %f \t %f \n" , xRot , yRot , zoom_out);
	keyOperations();
	glutPostRedisplay();
}

void processKeyboardKeysUP( unsigned char key , int x , int y)
{
	keyNormalStates[key] = false;
	printf("Normal Keys UP registered \n");	
	printf(" %f \t %f \t %f \n" , xRot , yRot , zoom_out);
	keyOperations();
	glutPostRedisplay();
}

void processMouse(int button , int state , int x , int y)
{
	if ( state == GLUT_DOWN)
	{
		printf("mouse DOWN captured \n");
		xInitial = x;
		yInitial = y;
	}

	if(keyNormalStates['c'] == 1 && state == GLUT_DOWN)
	{
		pan = 1;
		rot = 0;
	}
	
	if( state == GLUT_UP )
	{
		pan = 0;
		rot = 1;
	}
	
}

void processActiveMotion( int x , int y)
{
	xFinal=x;
	yFinal=y;

	if(rot == 1)
	{
	xRot = xRot + (yFinal-yInitial)/50;
	yRot = yRot + (xFinal-xInitial)/50;
	}

	printf(" xRot = %f and yRot = %f \n " , xRot , yRot);

	if(pan == 1 )
	{
		x_trans = x_trans+(xFinal - xInitial)/50;
		y_trans = y_trans-(yFinal - yInitial)/50;

	}


	glutPostRedisplay();
}

int main( int argc , char* argv[])
{
	glutInit(&argc , argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutCreateWindow("CE");
	
	glutKeyboardFunc(processKeyboardKeys);
	glutKeyboardUpFunc(processKeyboardKeysUP);
	
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processSpecialKeysUp);
	
	glutMouseFunc(processMouse);
	glutMotionFunc(processActiveMotion);

	glutDisplayFunc(RenderScene);
	//glutIdleFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	//glutTimerFunc(66 , Timer , 1 );
	

	SetupRC();
	
	glutMainLoop();
	return 0;

}