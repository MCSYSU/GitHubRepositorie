#include <gl/glut.h>
#include "math.h"
#include <stdio.h>
#include "tea.h"
#include "Room.h"
#include "sofa.h"

int flag=1;
double front_Point_x,front_Point_y,front_Point_z;
double pos_Up[3],pos_Down[3],pos_Motion[3];
double viewmatrix[16],modelviewmatrix[16];
#define GLUT_MIDDLE_UP_BUTTON 0x0003
#define GLUT_MIDDLE_DOWN_BUTTON 0x0004
float PI = 3.141592654;
int listnum;//�������
GLfloat xangle = 0.0;
GLfloat yangle = 0.0;
GLfloat oDistance = 5.0;
int cacheX = 0;
int cacheY = 0;
int xSpeed = 1;
int ySpeed = 1;
bool light=true; // ��Դ�Ŀ�/��
bool lp=true; // L��������ô?
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; //���������
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // ��������
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f }; // ��Դλ��
							
void motion(int x, int y) //�����Ӧ�¼�
{
	if (x > cacheX)
	{
		yangle = yangle - xSpeed;
	}
	if (x < cacheX) 
	{
		yangle = yangle + xSpeed;
	}
	if (y > cacheY)
	{
		xangle = xangle + ySpeed;
	}
	if (y < cacheY)
	{
		xangle = xangle - ySpeed;
	}
	glutPostRedisplay();
	cacheX = x;
	cacheY = y;
}

void setView() //���ù۲�Ƕ�
{
	glRotatef(10,1.0,0.0,0.0);
	glRotatef(22,0.0,1.0,0.0);
	glTranslatef(-20.5,-10.0,-55.0);
	glGetDoublev(GL_MODELVIEW_MATRIX,viewmatrix);
}

void SetRC() //������Ⱦ
{
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH,GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK,GL_LINE);
	glEnable( GL_COLOR_MATERIAL);
}

void renderWorld() //ģ�ⳡ��
{
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	float oXhudu = PI / (180 / xangle); //����X����ת�ĽǶ�
	float oYhudu = PI / (180 / yangle); //����Y����ת�ĽǶ�
	//���ó�������
	GLfloat btm = oDistance * cos(oXhudu);
	GLfloat vpY = oDistance * sin(oXhudu);
	GLfloat vpX = btm * sin(oYhudu);
	GLfloat vpZ = btm * cos(oYhudu);
	if (fabs(xangle) < 90.0) 
	{
		gluLookAt(vpX, vpY, vpZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	} 
	else 
	{
		if (fabs(xangle) >= 270.0) 
		{
			if (fabs(xangle) >= 360.0) 
			{
				xangle = 0.0;
			}
			gluLookAt(vpX, vpY, vpZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		} 
		else 
		{
			gluLookAt(vpX, vpY, vpZ, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0);
		}
	}
	if (fabs(yangle) >= 360.0)
	{
		yangle = 0;
	}
	//���Ƴ���������
 	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(200,0,0);
	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,200,0.0);
	glColor3f(0.0,0.0,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,200.0);
	glEnd();
	

	glGetDoublev(GL_MODELVIEW_MATRIX,modelviewmatrix);
	glCallList(listnum);// �������뺯��
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.1,0.1,0.1,0.1);
	glPushMatrix();
	setView();
	renderWorld();
	glPopMatrix();
	glutSwapBuffers();
}

void myReshape(int w,int h) //���»��Ƴ���
{	
	GLfloat nRange = 100.0f;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 	if(w<=h)	//�����ü������������Ͻ�Զ��
	{
		glOrtho(-nRange,nRange,-nRange*h/w,nRange*h/w,-100,100);
	}
	else
	{
		glOrtho(-nRange*w/h,nRange*w/h,-nRange,nRange,-100,100);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_RIGHT_BUTTON ) 
	{
		if(state == GLUT_DOWN)
		{
			if(light)
			{
				glDisable(GL_LIGHT1); // ���ù�Դ
				light=!light;
			}
			else
			{
				glEnable(GL_LIGHT1);
				light=!light;
			}
		}
	}
	glutPostRedisplay();//���¼���

}

void GL_myInitial()
{
 GLfloat light_position[]={1.0,1.0,1.0,0.0};//���λ��
  GLfloat light_ambient []={0.0,0.0,0.0,1.0};//������
  GLfloat light_diffuse []={1.0,1.0,1.0,1.0};//�����
  GLfloat light_specular[]={1.0,1.0,1.0,1.0};//�����
  
 glLightfv(GL_LIGHT0,GL_POSITION,light_position);
 glLightfv(GL_LIGHT0,GL_AMBIENT ,light_ambient );
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,light_diffuse );
   glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);


 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
   glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glColorMaterial(GL_FRONT, GL_AMBIENT); 
 glEnable(GL_COLOR_MATERIAL);
}
int main(int argc, char *argv[]) //������
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800, 800);
	glutCreateWindow("VR-2");
	/*
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // ���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // ���������
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // ��Դλ��
	glEnable(GL_LIGHT1); // ���ù�Դ
	*/
	GL_myInitial();
	//CModernho cmodernho;
	listnum = GL3DS_initialize_tea(); //����audi
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(&myReshape);
	SetRC();
	glutMouseFunc(mouse);    
	glutMotionFunc(&motion);
	glutMainLoop();
	return 0;
}