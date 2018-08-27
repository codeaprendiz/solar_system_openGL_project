#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<cstdlib>
#include<vector>
#include<string>
#include<algorithm>
#include<fstream>
#include<cstdio>
#include<iostream>


#define AXIS_ON 1
#define AXIS_OFF 0
#define LIGHT_OFF 0
#define LIGHT_ON 1
#define THREE_D_MODE_OFF 0
#define THREE_D_MODE_ON 1
#define STARS_ON 1
#define STARS_OFF 0


enum view{ parallel,
	perspective
};
enum view userView=parallel;

float SCALE_FACTOR=2;
float angle=0.0;
GLubyte x[] = "X axis";
GLubyte y[] = "Y axis";
GLubyte z[] = "Z axis";
GLfloat mat_shininess[] = { 50.0 };
GLfloat light_position[] = { 3.0, 3.0, 3, 0.0 };
GLfloat light_position1[] = { 0.0, -3.0, 0, 0.0 };
GLfloat light_position2[] = { 0.0, 0.0, -3, 0.0 };
GLfloat light_position3[] = { 0.0, 0.0, 3, 0.0 };
GLfloat light_position4[] = { 3.0, 0.0, 0, 0.0 };
GLfloat light_position5[] = { -3.0, 0.0, 0, 0.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat mat_diffuse[]={1,1,1,0.0};
double viewer[]={0,0,-100};
GLfloat rotX, rotY, zoom, offsetY = 1.5, speed;
int earthSpeed=0,earthMoonSpeed=0,mercurySpeed=0,mercurySpeed1=0,venusSpeed=0,marsSpeed=0,marsMoonSpeed=0,jupiterSpeed=0,jupiterMoonSpeed=0,saturnSpeed=0,
    saturnMoonSpeed=0,uranusSpeed=0,uranusMoonSpeed=0,neptuneSpeed=0,neptuneMoonSpeed=0,starSpeed=0;
int axisFlag=0,lightFlag=1;
int viewMode=THREE_D_MODE_OFF;
int monkey;
int stars=STARS_OFF;
struct coordinate {
	float x,y,z;
	coordinate(float a,float b,float c) : x(a),y(b),z(c) {};
};

struct face{
	int facenum;
	bool four;
	int faces[4];
	face(int facen,int f1,int f2,int f3,int f4): facenum(facen) {
		faces[0]=f1;
		faces[1]=f2;
		faces[2]=f3;
		faces[3]=f4;
		four=true;
	}
};

int loadObject(const char* filename)
{
	std::vector<std::string*> coord;
	std::vector<coordinate*> vertex;
	std::vector<face*> faces;
	std::vector<coordinate*> normals;
	std::ifstream in(filename);
	if(!in.is_open())
	{
		std::cout<<"\nNot opened ";
		return -1;
	}
	
	char buf[255];
	while(!in.eof())
	{
		in.getline(buf,256);
		coord.push_back(new std::string(buf));
	}
	
	for(unsigned int i=0;i<coord.size();i++)
	{
		if((*coord[i])[0]=='#')
			continue;
		else if((*coord[i])[0]=='v' && (*coord[i])[1]==' ')
		{
				float tmpx,tmpy,tmpz;
				sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);
				vertex.push_back(new coordinate(tmpx,tmpy,tmpz));
		}
		else if((*coord[i])[0]=='v' && (*coord[i])[1]=='n')
		{
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
			normals.push_back(new coordinate(tmpx,tmpy,tmpz));
		}
		else if((*coord[i])[0]=='f')
		{
			int a,b,c,d,e;

			if(count(coord[i]->begin(),coord[i]->end(),' ')==4)
			{
					sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);
					faces.push_back(new face(b,a,c,d,e));
					
			}
			else
			{
					sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
					faces.push_back(new face(b,a,c,d,e));
			}
		}
		else
		{
				
		}
	}
	//draw
	int num;
	num=glGenLists(1);
	glNewList(num,GL_COMPILE);
	for(unsigned int i=0;i<faces.size();i++)
	{
		if(faces[i]->four)
		{		glColor3f(1,0,0);
				glBegin(GL_QUADS);
					glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
					glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
					glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
					glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
					glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
				glEnd();
		}
		else 
		{
				glBegin(GL_TRIANGLES);
					glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
					glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
					glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
					glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
				glEnd();
		}
	}
	
	glEndList();
	
	
	
	for(unsigned int i=0;i<coord.size();i++)
		delete coord[i];
		
	for(unsigned int i=0;i<faces.size();i++)
		delete faces[i];
		
	for(unsigned int i=0;i<normals.size();i++)
		delete normals[i];
	
	for(unsigned int i=0;i<vertex.size();i++)
		delete vertex[i];
		
	return num;
}

void getAxis()
{
	glBegin(GL_LINES);
	glColor3f(0,0,1);
		glVertex3f(0,0,-50);
		glVertex3f(0,0,50);
	glColor3f(0,1,0);
		glVertex3f(0,-30,0);
		glVertex3f(0,30,0);
	glColor3f(1,0,0);
		glVertex3f(-50,0,0);
		glVertex3f(50,0,0);
	glEnd();
	
	glColor3f(1,0,0);
	glRasterPos3f(25,0,0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,x);
	glColor3f(0,1,0);
	glRasterPos3f(0,35,0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,y);
	glColor3f(0,0,1);
	glRasterPos3f(0,0,45);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,z);
}

void getLight()
{
	if(lightFlag && viewMode==THREE_D_MODE_OFF)
	{
		float 	ambient[]={5.7,5.7,5.7},
	diffuse[]={15.5,15.5,15.5},
	specular[]={1,1,1,1},
	shiness[]={50},
	lightPos[]={6,6,6,6},
	lightDif[]={1.7,1.7,1.7,2},
	lightPos1[]={11,11,11,1},
	lightDif1[]={1.7,1.7,1.7,2},
	lightPos2[]={-11,-11,-11,1},
	lightDif2[]={3.7,3.7,3.7,2},
	lightPos3[]={-11,-11,11,1},
	lightDif3[]={1.7,1.7,1.7,2},
	lightPos4[]={11,-11,-11,1},
	lightDif4[]={1.7,1.7,1.7,2},
	lightPos5[]={-11,11,-11,1},
	lightDif5[]={1.7,1.7,1.7,2},
	lightPos6[]={-11,11,11,1},
	lightDif6[]={1.7,1.7,1.7,2},
	lightPos7[]={11,-11,11,1},
	lightDif7[]={1.7,1.7,1.7,2},
	lightPos8[]={11,11,-11,1},
	lightDif8[]={1.7,1.7,1.7,2}
	;
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);
	glEnable(GL_LIGHT7);
	//glEnable(GL_LIGHT8);
	//glEnable(GL_LIGHT9);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,shiness);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDif);
	/*
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDif1);
	glLightfv(GL_LIGHT1,GL_POSITION,lightPos2);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDif2);
	glLightfv(GL_LIGHT2,GL_POSITION,lightPos3);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,lightDif3);
	glLightfv(GL_LIGHT3,GL_POSITION,lightPos4);
	glLightfv(GL_LIGHT3,GL_DIFFUSE,lightDif4);
	glLightfv(GL_LIGHT4,GL_POSITION,lightPos5);
	glLightfv(GL_LIGHT4,GL_DIFFUSE,lightDif5);
	glLightfv(GL_LIGHT5,GL_POSITION,lightPos6);
	glLightfv(GL_LIGHT5,GL_DIFFUSE,lightDif6);
	glLightfv(GL_LIGHT6,GL_POSITION,lightPos7);
	glLightfv(GL_LIGHT6,GL_DIFFUSE,lightDif7);
	glLightfv(GL_LIGHT7,GL_POSITION,lightPos8);
	glLightfv(GL_LIGHT7,GL_DIFFUSE,lightDif8);
	 */
	}
	else if(viewMode==THREE_D_MODE_ON &&lightFlag==LIGHT_OFF )
	{
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glDisable(GL_COLOR_MATERIAL);
		float col[]={1,1,1};
		glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
	}
	else if(viewMode==THREE_D_MODE_ON && lightFlag==LIGHT_ON)
	{
		glColor3f(1,1,1);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float pos[]={-2,2,-4,2};
		glLightfv(GL_LIGHT0,GL_POSITION,pos);    
		float col[]={2,2,2};
		glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
		
		glDisable(GL_COLOR_MATERIAL);
	}
	else if(viewMode==THREE_D_MODE_OFF && lightFlag==LIGHT_OFF)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_COLOR_MATERIAL);
	}
}

void init()
{
	//glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if((viewMode==THREE_D_MODE_ON || viewMode==THREE_D_MODE_OFF) && userView==parallel)
	{
		glClearColor(0.0,0.0,0.0,1.0);
		gluPerspective(45,640/480,1,500);
	}
	else if(userView==perspective)
		glFrustum(-10,10,-10,10,1,500);
	glMatrixMode(GL_MODELVIEW);
	monkey=loadObject("Monkey.obj");
	getLight();
}

void SpecialKey(int key, int x, int y)
{
	switch(key) 
	{
		case GLUT_KEY_UP:
			rotX -= 5;
			break;
		case GLUT_KEY_DOWN:
			rotX += 5;
			break;
		case GLUT_KEY_LEFT:
			rotY -= 5;
			break;
		case GLUT_KEY_RIGHT:
			rotY += 5;
			break;
		case GLUT_KEY_PAGE_UP:
			offsetY -= 0.1;
			break;
		case GLUT_KEY_PAGE_DOWN:
			offsetY += 0.1;
			break;
	}
	glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 500.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void disablelight12()
{
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHT5);
}

void enablelight12()
{
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
}

void drawSolarSystem()
{

	glColor3f(1.0,1.0,0.0);
	glPushMatrix();
	//glDisable(GL_LIGHT0);

	glRotatef(12,0,0,0);
	glutSolidSphere(10,200,200);
	glEnable(GL_LIGHT0);
	glPopMatrix();
	
	//disablelight12();
	glColor3f(0.5,1,0);
	glPushMatrix();
	glRotatef((mercurySpeed+=3)%360,1,0,0);
	glTranslatef(1,11.5,0);
	glutSolidSphere(.38,200,200);
	glPopMatrix();
	
	glColor3f(0.5,0.5,0);
	glPushMatrix();
	glRotatef((venusSpeed+=2)%360,1,0,0);
	glTranslatef(2,13.5,0);
	glutSolidSphere(.95,200,200);
	glPopMatrix();
	
	glColor3f(0,0,1);
	glPushMatrix();
	glRotatef((earthSpeed+=2)%360,1,0,0);
	glTranslatef(0,25,0);
	glutSolidSphere(1,200,200);
		glPushMatrix();
		glColor3f(0,1,0);
		glRotatef((earthMoonSpeed+=5)%360,0,1,0);
		glTranslatef(2,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
	glPopMatrix();
	
	glColor3f(0,1,1);
	glPushMatrix();
	glRotatef((marsSpeed+=3)%360,1,0,0);
	glTranslatef(-2,16.5,0);
	glutSolidSphere(.53,200,200);
		glPushMatrix();
		glColor3f(0,1,0);
		glRotatef((marsMoonSpeed+=5)%360,0,1,0);
		glTranslatef(2,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
		glPushMatrix();
		glColor3f(0,1,0);
		glRotatef((marsMoonSpeed+=5)%360,0,0,1);
		glTranslatef(3,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
	glPopMatrix();
	
	glColor3f(0,1,1);
	glPushMatrix();
	glRotatef((jupiterSpeed+=3)%360,1,0,0);
	glTranslatef(-5,24,0);
	glutSolidSphere(2,200,200);
		glPushMatrix();
		glColor3f(0.64,0.16,0.16);
		glRotatef((jupiterMoonSpeed+=2)%360,0,1,0);
		glTranslatef(4,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
		glPushMatrix();
		glColor3f(0,1,0);
		glRotatef((jupiterMoonSpeed+=5)%360,0,0,1);
		glTranslatef(3,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
	glPopMatrix();
	
	glColor3f(0.34,0.16,0.16);
	glPushMatrix();
	glRotatef((saturnSpeed+=4)%360,1,0,0);
	glTranslatef(4,29,0);
	glutSolidSphere(1.5,200,200);
		glPushMatrix();
		glColor3f(0.64,0.16,0.16);
		glRotatef((saturnMoonSpeed+=2)%360,0,1,0);
		glTranslatef(2,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
		glPushMatrix();
		glColor3f(0,1,0);
		glRotatef((saturnMoonSpeed+=5)%360,0,0,1);
		glTranslatef(3,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
	glPopMatrix();
	
	glColor3f(0.67,0.45,0.12);
	glPushMatrix();
	glRotatef((uranusSpeed+=1)%360,1,0,0);
	glTranslatef(0,34,0);
	glutSolidSphere(0.7,200,200);
		glPushMatrix();
		glColor3f(0.64,0.16,0.16);
		glRotatef((uranusMoonSpeed+=2)%360,0,1,0);
		glTranslatef(2,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
		glPushMatrix();
		glColor3f(0,1,0);
		glRotatef((uranusMoonSpeed+=5)%360,0,0,1);
		glTranslatef(3,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
	glPopMatrix();
	
	glColor3f(0.0,0.0,1);
	glPushMatrix();
	glRotatef((neptuneSpeed+=2)%360,1,0,0);
	glTranslatef(2,39,0);
	glutSolidSphere(0.5,200,200);
		glPushMatrix();
		glColor3f(0.64,0.16,0.16);
		glRotatef((neptuneMoonSpeed+=2)%360,0,1,0);
		glTranslatef(2,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
		glPushMatrix();
		glColor3f(0,1,0);
		glRotatef((neptuneMoonSpeed+=5)%360,0,0,1);
		glTranslatef(3,0,0);
		glutSolidSphere(.2,200,200);
		glPopMatrix();
	glPopMatrix();
}
void getScaled()
{	
	glScalef(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);	
}

float randomz()
{
	float z = 0;
	while(z <70.0)
	z = (float)(rand() % 1000)/ 10.0;
	return z;
}
float randomy()
{
	float y = 0;
	while(y <70.0)
	y = (float)(rand() % 1000)/ 10.0;
	return y;
}
float randomx()
{
	float x = 0;
	while(x <70.0)
	x = (float)(rand() % 1000)/ 10.0;
	return x;
}
float randomy1()
{
	return ((float)(rand() % 1000)/ 10.0);
}
float randomx1()
{
	return ((float)(rand() % 1000)/ 10.0);
}
float randomz1()
{
	return ((float)(rand() % 1000)/ 10.0);
}
void putStar(int x,int y,int z)
{
	glColor3f(1,1,1);
	/*glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,y,z);
	glutSolidSphere(.01,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,y,z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
		
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,-y,z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,-y,z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,-y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,-y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();*/
	glPointSize(1.0);
	glPushMatrix();
	glTranslatef(-x,-y,-z);
	glRotatef((starSpeed+=3)%360,1,0,0);
	glBegin(GL_POINTS);
		glVertex3f(randomx1(),randomy1(),randomz());
		glVertex3f(-randomx1(),randomy1(),randomz());
		glVertex3f(randomx1(),-randomy1(),randomz());
		glVertex3f(-randomx1(),-randomy1(),randomz());
		glVertex3f(randomx1(),randomy1(),-randomz());
		glVertex3f(-randomx1(),randomy1(),-randomz());
		glVertex3f(randomx1(),-randomy1(),-randomz());
		glVertex3f(-randomx1(),-randomy1(),-randomz());
		
		glVertex3f(randomx(),randomy1(),randomz1());
		glVertex3f(-randomx(),randomy1(),randomz1());
		glVertex3f(randomx(),-randomy1(),randomz1());
		glVertex3f(-randomx(),-randomy1(),randomz1());
		glVertex3f(randomx(),randomy1(),-randomz1());
		glVertex3f(-randomx(),randomy1(),-randomz1());
		glVertex3f(randomx(),-randomy1(),-randomz1());
		glVertex3f(-randomx(),-randomy1(),-randomz1());
		
		glVertex3f(randomx1(),randomy(),randomz1());
		glVertex3f(-randomx1(),randomy(),randomz1());
		glVertex3f(randomx1(),-randomy(),randomz1());
		glVertex3f(-randomx1(),-randomy(),randomz1());
		glVertex3f(randomx1(),randomy(),-randomz1());
		glVertex3f(-randomx1(),randomy(),-randomz1());
		glVertex3f(randomx1(),-randomy(),-randomz1());
		glVertex3f(-randomx1(),-randomy(),-randomz1());
	glEnd();
	glPopMatrix();
	glPointSize(1.0);
	
	
}		
/*
void putStar(int x,int y,int z)
{
	glColor3f(1,1,1);
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,y,z);
	glutSolidSphere(.01,10,10);
	//glVertex3f(x,y,z);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,y,z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
		
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,-y,z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,-y,z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(x,-y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	glPushMatrix();
	glRotatef((mercurySpeed1+=3)%360,1,0,0);
	glTranslatef(-x,-y,-z);
	glutSolidSphere(.01 ,10,10);
	glPopMatrix();
	
}
*/ 		
void putStars()
{
	int i;
	for(i = 0; i < 500; i++)
  putStar(rand()%50, rand()%50,rand()%50);
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(viewMode==THREE_D_MODE_ON)
	{
		float pos[]={-2,2,-4,2};
		glLightfv(GL_LIGHT0,GL_POSITION,pos);    
		gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0, 0.0, 0, 1.0, 0.0);
		glTranslatef(0,0,-5);
		glRotatef(angle,0,1,0);
		angle=(angle+0.1);
	}
	if(viewMode==THREE_D_MODE_OFF)
		gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0, 0.0, 0, 1.0, 0.0);
	getScaled();
	glRotatef(rotY, 0.0, 1.0, 0.0);	 /* rotate Y axis */
	glRotatef(rotX, 1.0, 0.0, 0.0);	 /* rotate X axis */
	if(viewMode==THREE_D_MODE_ON)
		glCallList(monkey);
	else
	{
		drawSolarSystem();
		if(stars)
			putStars();
	}
	if(axisFlag==AXIS_ON)
		getAxis();
	getLight();
	glutSwapBuffers();

}


void keyboard(unsigned char key,int x,int y)
{
	if(key=='X') viewer[0]-=1.0;
	if(key=='x') viewer[0]+=1.0;
	if(key=='Y') viewer[1]-=1.0;
	if(key=='y') viewer[1]+=1.0;
	if(key=='Z') viewer[2]-=1.0;
	if(key=='z') viewer[2]+=1.0;
	if(key=='s') stars=STARS_ON;
	if(key=='S') stars=STARS_OFF;
	printf("\nviewer[z]= %lf ",viewer[2]);
	glutPostRedisplay();
	//display();
}

void lightMenu(int id)
{
	switch(id)
	{
	case 1: lightFlag=LIGHT_ON; init();glutFullScreen()	 ;reshape(1300,800);break;
	case 2: lightFlag=LIGHT_OFF; init();glutFullScreen()	   ;reshape(1300,800);break;
	}
	display();
}

void scalingMenu(int id)
{
	switch(id)
	{
	case 1: SCALE_FACTOR=3; init();glutFullScreen()	 ;reshape(1300,800);break;
	case 2: SCALE_FACTOR=4;init();glutFullScreen()	 ;reshape(1300,800);break;
	}
	display();
}

void axisMenu(int id)
{
	switch(id)
	{
	case 1: axisFlag=AXIS_ON; init();glutFullScreen()	 ;reshape(1300,800);break;
	case 2: axisFlag=AXIS_OFF; init();glutFullScreen()	 ;reshape(1300,800);break;
	}
	display();
}
void viewMenu(int id)
{
	switch(id)
	{
	case 1: userView=parallel; init(); glutFullScreen()	 ;reshape(1300,800);break;
	case 2: userView=perspective; init(); init();glutFullScreen();	 break;
	}
	display();
}
void mainMenu(int id)
{
}
void switchMenu(int id)
{
	switch(id)
	{
	case 1: viewMode=THREE_D_MODE_ON;init();glutFullScreen()	   ;reshape(1300,800);break;
	case 2: viewMode=THREE_D_MODE_OFF; init(); glutFullScreen()	   ;reshape(1300,800);break;
	}
	display();
}
void mainMenu()
{
}
void createMainMenu()
{   
	int lightMenuSelection=glutCreateMenu(lightMenu);
	glutAddMenuEntry("ON",1);
	glutAddMenuEntry("OFF",2);
	int scalingMenuSelection=glutCreateMenu(scalingMenu);
	glutAddMenuEntry("x3",1);
	glutAddMenuEntry("x4",2);
	int axisMenuSelection=glutCreateMenu(axisMenu);
	glutAddMenuEntry("Yes",1);
	glutAddMenuEntry("No",2);
	int viewMenuSelection=glutCreateMenu(viewMenu);
	glutAddMenuEntry("Perspective",1);
	glutAddMenuEntry("Frustum",2);
	int switchMenuSelection=glutCreateMenu(switchMenu);
	glutAddMenuEntry("Yes",1);
	glutAddMenuEntry("Back to Solar System",2);
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Lighting",lightMenuSelection);
	glutAddSubMenu("Scaling",scalingMenuSelection);
	glutAddSubMenu("Axis",axisMenuSelection);
	glutAddSubMenu("View",viewMenuSelection);
	glutAddSubMenu("Switch to Viewing 3D objects",switchMenuSelection);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow("1BI13CS017");
	glutFullScreen()	   ;
	glutInitWindowSize(800,800);
	glutInitWindowPosition(270,0);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKey);
	//glutIdleFunc(putStars);
	createMainMenu();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}	
	
