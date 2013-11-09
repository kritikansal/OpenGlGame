#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>
#include<math.h>
#include "imageloader.h"
#include<time.h>
#include <unistd.h>
#include "text3d.h"
#include "glm.h"

using namespace std;
#define p 250
#define q 250
#define PI 3.14159265
#define sp 0.5*0.1

GLMmodel * p1;
GLMmodel * p2;
GLfloat shadowMat[4][4];
float rottheta=60,rotthetavel=20;

int modper=0;
int modenemy1=0,modenemy2=0;
float coin=3;

float moveenemyj1=5,moveenemyj1vel=0.06;

//int movingtiles[10];
//int missing tiles[10];
//int block[10];
float _scale;
int oni=0,oni2=0,onj=0;

int tl[10][10]={0};
int co[10][10]={0};
int tilei=-5,tilej=0,tilek=10;

float moveblocki=-3,moveblocki2=3,moveblockj=6,moveblockiy=0,moveblockjy=0;
float moveblockjvel=0.06,moveblockivel=0.06,moveblocki2vel=-0.06,moveblockiyvel=0.1,moveblockjyvel=0.1;

float windowwidth=500,windowheight=500;
float passivex=0,passivey=0,passivez=0,addpassivez=0;
typedef float point[3];
float aa=0.3,bb=0.3,cc=0.3,fac=0.007;


int dragx1=0,dragy1=0,dragx2=0,dragy2=0,dragshiftx=0,dragshifty=0;
float dragx=0,dragz=10;
int level=0;
int tiley=-1.6;
int ran;
int a[5]={0};
float ang0=0,ang1=0,ang2=0,ang3=0,ang4=0, vel=0; /* initial joint angles */
int mouseflag=0;

float prx=0,pry=0;
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
GLUquadricObj *t, *h, *lua, *lla, *rua, *rla, *lll, *rll, *rul, *lul;

float current=1;
float constjump=0,constjump2=0;
int down=0,jump=0,jump2=0;
float trampolinevel=0.9;

float cam=0;
int sel_stop=0;
int sel_f=0,sel_n=0,sel_h=0,sel_t=0,sel_p=0,sel_s=0,sel_z=0,sel_R=0,sel_F=0;
double size=1.0;
float updown=1,updownvel=0.2,fall[100][100]={0};
float sel_fall[100][100]={0};

int var=0;
int sel=0;
float head=(3/1.1)*0.1;
float	bodyradius=(3/1.3)*0.1;
float	bodyheight=(3/1.3)*0.1;
float	armradius=(0.9/1.1)*0.08;
float	armheight=(3/1.3)*0.1;
float	legradius=(0.9/1.1)*0.1;
float	legheight=(5.0/1.3)*0.1;
float	legspace=(1/1.3)*0.1;
float	earradius=(1.125/1.1)*0.06;
float	earheight=(1/1.3)*0.3;
float	eyeradius=(1.25/1.3)*0.06;
float	posx=0.0;
float	posy=0.0;
float	posz=0.0;
float prevx=0.0,prevy=0.0,prevz=0.0;
GLfloat xRotated, yRotated, zRotated;
GLdouble radius=4;
GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0}; //Black Color
GLfloat qaGreen[] = {1.0, 0.0, 0.0, 1.0}; //Green Color
GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0}; //White Color
GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0}; //Red Color

    // Set lighting intensity and color
GLfloat qaAmbientLight[]    = {0.1, 0.1, 0.1, 1.0};
GLfloat qaDiffuseLight[]    = {1, 1, 1, 1.0};
GLfloat qaSpecularLight[]    = {1.0, 1.0, 1.0, 1.0};
GLfloat emitLight[] = {0.9, 0.9, 0.9, 0.01};
GLfloat Noemit[] = {0.0, 0.0, 0.0, 1.0};
    // Light source position
GLfloat qaLightPosition[]    = {0, 0, 2, 1};
GLfloat qaLightDirection[]    = {1, 1, 1, 0};
GLfloat dirVector0[]={ 1.0, 0.0, 0.0, 0.0};

GLfloat ambient[3];
GLfloat diffuse[3];
GLfloat specular[3];
GLfloat shiness[] = {50.0f};
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
			0,                            //0 for now
			GL_RGB,                       //Format OpenGL uses for image
			image->width, image->height,  //Width and height
			0,                            //The border of the image
			GL_RGB, //GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
			//as unsigned numbers
			image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureId;//back image
GLuint _textureId2;//right image
GLuint _textureId3;//left image
GLuint _textureId4;//down image //ocean
GLuint _textureId5;//brick
GLuint _textureId6;//MOVING LEFT RIGHT TILE
GLuint _textureId7;//TRAMPOLINE
GLuint _textureId8;//TELEPORTER
GLuint _textureId9;//COIN

void shadowMatrix(GLfloat groundplane[4],GLfloat lightpos[4])
{
	GLfloat dot;

	/* Find dot product between light position vector and ground plane normal. */
	int X=0,Y=1,Z=2,W=3;
	dot = groundplane[X] * lightpos[X] +
		groundplane[Y] * lightpos[Y] +
		groundplane[Z] * lightpos[Z] +
		groundplane[W] * lightpos[W];

	shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
	shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
	shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
	shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

	shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
	shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
	shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
	shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

	shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
	shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
	shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
	shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

	shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
	shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
	shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
	shadowMat[3][3] = dot - lightpos[W] * groundplane[W];

}

//Computes a scaling value so that the strings
float computeScale(const char* strs[4]) {
	float maxWidth = 0;
	for(int i = 0; i < 4; i++) {
		float width = t3dDrawWidth(strs[i]);
		if (width > maxWidth) {
			maxWidth = width;
		}
	}

	return 2.6f / maxWidth;
}
//The four strings that are drawn
const char* STRS[4] = {"END", "START", "Rock", ".com"};

void cleanup() {
	t3dCleanup();
}


void initLighting()
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_LIGHT2); //Enable light #1
	glEnable(GL_LIGHT3); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals

}
void update2(int v)
{
	if(aa > 3.9)
	{
		fac=fac*-1;
	}
	if(aa >= 0 && aa <= 0.1 )
	{
		fac=fac*-1;
	}
	aa=aa+fac;
	bb=bb+fac;
	cc=cc+fac;
	glutTimerFunc(25,update2,0);
}

void update(int v)
{
	rottheta+=rotthetavel;
	if(rottheta>360)
		rotthetavel*=-1;
	if(rottheta<0)
		rotthetavel*=-1;
	if(down==0)
		var++;

	prevx=posx;
	prevy=posy;
	prevz=posz;
	if(oni==1)
	{
		posx=moveblocki;
	}
	if(oni2==1)
	{
		posx=moveblocki2;
	}
		
	if(onj==1)
	{
		posz=moveblockj;
	}
	else
	{}


	if(posx>4 || posx<-5.5 || posz>9.5 || posz<-1)
	{
		if (sel==0)
			posz+=0.1;
		else if (sel==1)
			posx+=0.1;
		else if (sel==2)
			posz-=0.1;
		else if (sel==3)
			posx-=0.1;
		
		posy-=0.2;
		if(posy<-2)
			exit(0);	
	}
	else
	{
		if (sel==0 && down==0)
			posz+=vel;
		else if (sel==1 && down==0)
			posx+=vel;
		else if (sel==2 && down==0)
			posz-=vel;
		else if (sel==3 && down==0)
			posx-=vel;
		if (vel<=0.0){ vel=0.0;}
		else
			vel-=0.02;
		if (vel>0.000001)
		{
			modper=(modper+1)%2;
			//printf("in motion\n");
			if(var%2==1)
			{
				ang3=60;
				ang0=0;
				ang4=60;
				ang1=0;
			}
			if(var%2==0)
			{
				ang0=60;
				ang3=0;
				ang1=60;
				ang4=0;
			}
		}
		else
		{
			ang0=0;
			ang3=0;
			ang1=0;
			ang4=0;
		}
	}
	if(posy>0.0001 && jump==1)
	{
		if(constjump>0)
			constjump-=0.3;
		posy-=0.3;
		if (sel==0 && down==0)
			posz+=0.25;
		else if (sel==1 && down==0)
			posx+=0.25;
		else if (sel==2 && down==0)
			posz-=0.25;
		else if (sel==3 && down==0)
			posx-=0.25;
		
		if(posy<0)
		{
			jump=0;
			posy=0;
		}
	}
	if(posy>0.0001 && jump2==1)
	{
		if(constjump2>0)
			constjump2-=0.3;
		posy-=0.3;
		if(posy<0)
		{
			jump2=0;
			posy=0;
		}
	}

	if(updown>6)
		updownvel=-0.2;
	if(updown<0.2)
		updownvel=0.2;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			if(sel_fall[i][j]==1)
			{
				fall[i][j]-=0.2;
				//printf("i %d\n",i);
			}
		}
	}
	updown+=updownvel;
	
	moveblockj+=moveblockjvel;
	if(moveblockj>9 || moveblockj<5)
		moveblockjvel*=-1;
	moveblocki+=moveblockivel;
	if(moveblocki>-2 || moveblocki<-5)
		moveblockivel*=-1;
	
	moveblocki2+=moveblocki2vel;
	if(moveblocki2>4 || moveblocki2<1)
		moveblocki2vel*=-1;


	moveenemyj1+=moveenemyj1vel;
	if(moveenemyj1>7 || moveenemyj1<5)
		moveenemyj1vel*=-1;
	
	coin-=0.005;

	glutPostRedisplay();
	glutTimerFunc(100,update, 0);
}
void handleMouseclick(int button, int state, int x, int y) {

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			//printf("GLUT_DOWN LEFT_BUTTON x,y %d,%d\n",x,y);
			dragx1=x;
			dragy1=y;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{}
		//printf("GLUT_DOWN RIGHT_BUTTON x,y %d,%d\n",x,y);
	}
	if (state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			//printf("GLUT_UP LEFT_BUTTON x,y %d,%d\n",x,y);

			/*dragshiftx=dragx1-x;
			  dragshifty=dragy1-y;
			  dragshiftx=(int)(dragshiftx)%8;
			  dragshifty=(int)(dragshifty)%4;*/

			dragx2=x;
			dragy2=y;
			dragshiftx=(dragx1-dragx2)/10;
			dragshifty=(dragy1-dragy2)/10;
			dragx=(dragx*cos(dragshiftx)) + (dragz*sin(dragshiftx));
			dragz=-(dragx*sin(dragshiftx)) + (dragz*cos(dragshiftx));

			float ddx=x-dragx1;
			float tanglex=-((ddx/40.0)*20.0)/180;
			//printf("%f\n",tanglex);
			float tempx=passivex;
			//printf("%f %f\n",camx,camz);

			passivex=passivex*cos(tanglex)+passivez*sin(tanglex);
			passivez=-tempx*sin(tanglex)+passivez*cos(tanglex);
			float ddy=y-dragy1;
			float tangley=-((ddy/20.0)*20.0)/180;
			float tempy=passivey;
			passivey=passivey*cos(tangley)-passivez*sin(tangley);
			passivez=tempy*sin(tangley)+passivez*cos(tangley);




			//dragx=(float)x;
			//dragz=(float)y;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{}
		//printf("GLUT_UP RIGHT_BUTTON x,y %d,%d\n",x,y);
	}
	if(button == 3)
	{
		addpassivez-=0.2;
		//printf("ZOOMED IN %f\n",passivey);
		// Zoom in
	}
	if(button == 4)
	{
		addpassivez+=0.2;
		//printf("ZOOMED OUT %f\n",passivey);
		// Zoom out
	}


}

void myMouseMove( int x, int y)
{
	//passivex=(x-(windowwidth/2))*(xscale);
	//passivey=((y-(windowheight/2))*(yscale))*-1;
	//passivex=x*0.01;//*0.02;//(x-(windowwidth/2);
	//passivey=y*0.01;//*0.02;
	if(mouseflag==0){
		mouseflag=1;
		prx=x;
		pry=y;
	}
	float delx,dely;
	delx=-(prx-x);
	dely=+(pry-y);
	//passivex+=delx/40;
	//passivey+=dely/40;
	passivex=(x-800)*0.01;
	passivez=(y-800)*0.01;
	prx=x;
	pry=y;
	//printf("x=%d,y=%d,delx=%f,dely=%f,inix=%f,iniy=%f\n",x,y,delx,dely,inix,iniy);
	/*printf("%f %f\n",x/20.0,y/20.0);
	  inix=x/20.0;
	  iniy=y/20.0;*/
	passivez=0;//-(windowheight/2);
	//printf("MY MOUSE MOVE  %d,%d\n",x,y);
	//passivex=x/((800*2*w)+-w);
	//passivey=1-(y/((600*2*h)+-h));

	glutPostRedisplay();
}
void drawhemisphere(float R,float x,float y,float z)
{
	glPushMatrix();
		glTranslatef(x,y,z);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for(int j = 0; j < q; j++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0; i <= p; i++)
			{
				glVertex3f( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
						R * sin( (float)(j+1)/q * PI/2.0 ),
						R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
				glVertex3f( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
						R * sin( (float)j/q * PI/2.0 ),
						R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );         
			}
			glEnd();
		}
		glPopMatrix();
		glColor3f(0.0, 1.0, 0.0);

	}
	void drawBall(float r, float h,float x, float y , float z)
	{
		glPushMatrix();
		glTranslatef(x,y,z);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	GLUquadricObj *obj;
	obj=gluNewQuadric();
	gluCylinder(obj,r,r,h,1000,1000);
	glPopMatrix();
}
void drawsphere(float r,float x,float y , float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	GLUquadricObj *obj;
	obj=gluNewQuadric();
	gluSphere(obj,r,10,10);
	glPopMatrix();
}
void drawHead()
{
	glColor3f(1.0,0.0,1.0);
	drawhemisphere(head,0,bodyheight+sp,0);
}
void drawEar(float a,float b,float c,float earang)
{
	glPushMatrix();
	glTranslatef(a,b,c);
	glRotatef(earang,0.0,0.0,1.0);
	glColor3f(0.0,1.0,0.0);
	drawBall(earradius,earheight,0,0,0);
	glPopMatrix();
}
void drawEye(float a,float b,float c)
{
	glColor3f(1.0,1.0,1.0);
	glColor3f(0.0,0.0,0.0);
	drawsphere(eyeradius,a,b,c);
}
void drawBody()
{
	glPushMatrix();
	glColor3f(0.0,1.0,0.0);
	glRotatef(ang2,0.0,1.0,0.0);
	drawBall(bodyradius,bodyheight,0,0,0);
	glPopMatrix();
}
void drawArm(float a,float b,float c,float armang)
{
	glPushMatrix();
	glTranslatef(a,b,c);
	glRotatef(-armang,1.0,0.0,0.0);
	glColor3f(1.0,1.0,0.0);
	drawBall(armradius,armheight,0,-bodyheight/2-armradius,0);
	glColor3f(0.0,1.0,0.0);
	drawhemisphere(armradius,0,-armradius-sp,0);
	drawhemisphere(-armradius,0,-3*bodyheight/4+armradius+sp,0);
	glPopMatrix();
}
void drawLeg(float a,float b,float c,float legang,float d)
{
	glPushMatrix();
	glRotatef(legang,1.0,0.0,0.0);
	glColor3f(1.0,1.0,0.0);
	drawBall(legradius,legheight,d,-legheight,0);
	glColor3f(0.0,1.0,0.0);
	drawhemisphere(a,b,c,0);
	glPopMatrix();
}

void drawrobo()
{
	glPushMatrix();
	glTranslatef(posx,posy,posz);

	//if(posx,posz)
	//glScalef(0.1,0.1,0.2);
	if (sel==1)
		glRotatef(90,0.0,1.0,0.0);
	else if (sel==2)
		glRotatef(180,0.0,1.0,0.0);
	else if (sel==3)
		glRotatef(270,0.0,1.0,0.0);
	glPushMatrix();
	
	drawHead();

	drawEar(bodyradius/4+2*sp,bodyheight+head,0,-45);
	drawEar(-bodyradius/4-2*sp,bodyheight+head,0,45);

		
	drawEye(bodyradius/2.5,bodyheight+bodyradius/1.5,head-eyeradius/1.5);
	drawEye(-bodyradius/2.5,bodyheight+bodyradius/1.5,head-eyeradius/1.5);
	
	glPopMatrix();
	drawBody();
	
	drawArm(bodyradius+sp+armradius,bodyheight,0,ang1);
	drawArm(-bodyradius-sp-armradius,bodyheight,0,ang4);
	
	drawLeg(-legradius,legspace,-legheight-legradius/2+sp,ang3,legspace);
	drawLeg(-legradius,-legspace,-legheight-legradius/2+sp,ang0,-legspace);
	
	glPopMatrix();
}
void drawCube(float i, float j,float r,float g,float b)
{
	glPushMatrix();
	glTranslatef(i,0,j);
r=1;g=1;b=1;	

	if(posx>i-0.5 && posx<i+0.5 && posz>j-0.5 && posz<j+0.5)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);

		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
		r=1;g=1;b=0.23;
	}
	r=1;g=1;b=1;  
	glColor3f(0,0,0);
	//glutWireCube(1);
	glColor3f(1,0,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);

	glColor3f( r, g, b ); 
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P1 is red
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P2 is green
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P3 is blue
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P4 is purple

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b);
	glColor3f( r, g, b);
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawCube1(float i, float j,float r,float g,float b)
{
	glPushMatrix();
	glTranslatef(i,0,j);
	r=1;g=1;b=1;  

	if(posx>i-0.5 && posx<i+0.5 && posz>j-0.5 && posz<j+0.5)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);

		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
		r=1;g=1;b=0.23;
	}
	r=1;g=1;b=1;  
	glColor3f(0,0,0);
	//glutWireCube(1);
	glColor3f(1,0,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);

	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 ); // P1 is red
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P2 is green
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P3 is blue
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 ); // P4 is purple

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( 0.5, -0.5, 0.5 );
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b);
	glColor3f( r, g, b);
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawCube2(float i, float j,float r,float g,float b)//PERIODIC
{
	glPushMatrix();
	glTranslatef(i,0,j);
r=1;g=1;b=1;	

	if(posx>i-0.5 && posx<i+0.5 && posz>j-0.5 && posz<j+0.5)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);

		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
		r=1;g=1;b=0.23;
	}
	r=1;g=1;b=1;  
	glColor3f(0,0,0);
	//glutWireCube(1);
	glColor3f(1,0,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);

	glColor3f( r, g, b ); 
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 ); // P1 is red
	glTexCoord2f(1,0);
	glVertex3f( 0.5, -0.5, -0.5 ); // P2 is green
	glTexCoord2f(0,1);
	glVertex3f( -0.5, -0.5, -0.5 ); // P3 is blue
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 ); // P4 is purple

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( 0.5, -0.5, 0.5 );
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b);
	glColor3f( r, g, b);
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawCube3(float i, float j,float r,float g,float b,int type)//TRAMPOLINE
{
	glPushMatrix();
	glTranslatef(i,0,j);
	r=1;g=1;b=1;  

	if(posx>i-0.5 && posx<i+0.5 && posz>j-0.5 && posz<j+0.5)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);

		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
		r=1;g=1;b=0.23;
	}
	r=1;g=1;b=1;  
	glColor3f(0,0,0);
	//glutWireCube(1);
	glColor3f(1,0,1);
	glEnable(GL_TEXTURE_2D);
	if(type==0)
	glBindTexture(GL_TEXTURE_2D, _textureId7);

	if(type==1)
	glBindTexture(GL_TEXTURE_2D, _textureId8);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);

	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 ); // P1 is red
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P2 is green
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P3 is blue
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 ); // P4 is purple

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( 0.5, -0.5, 0.5 );
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b);
	glColor3f( r, g, b);
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, -0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, -0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, -0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawCube5(float i, float j,float r,float g,float b)//PERIODIC
{
	glPushMatrix();
	glTranslatef(i,0,j);
r=1;g=1;b=1;	

	if(posx>i-0.5 && posx<i+0.5 && posz>j-0.5 && posz<j+0.5)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);

		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
		r=1;g=1;b=0.23;
	}
	r=1;g=1;b=1;  
	glColor3f(0,0,0);
	//glutWireCube(1);
	glColor3f(1,0,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);

	glColor3f( r, g, b ); 
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P1 is red
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P2 is green
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P3 is blue
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P4 is purple

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b);
	glColor3f( r, g, b);
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawCube4(float i, float j,float r,float g,float b)//PERIODIC
{
	glPushMatrix();
	glTranslatef(i,0,j);
r=1;g=1;b=1;	

	if(posx>i-0.5 && posx<i+0.5 && posz>j-0.5 && posz<j+0.5)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaRed);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);

		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
		r=1;g=1;b=0.23;
	}
	r=1;g=1;b=1;  
	glColor3f(0,0,0);
	//glutWireCube(1);
	glColor3f(1,0,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId8);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);

	glColor3f( r, g, b ); 
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P1 is red
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 ); // P2 is green
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P3 is blue
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 ); // P4 is purple

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( 0.5, 0.5, 0.5 );
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b);
	glColor3f( r, g, b);
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	//glColor3f( r, g, b );
	glTexCoord2f(0,0);
	glVertex3f( 0.5, 0.5, -0.5 );
	glTexCoord2f(1,0);
	glVertex3f( 0.5, 0.5, 0.5 );
	glTexCoord2f(0,1);
	glVertex3f( -0.5, 0.5, 0.5 );
	glTexCoord2f(1,1);
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawTile()
{
	glPushMatrix();	
	glTranslatef(0,tiley,0);
	//glScalef(0.9,0.9,0.9);
	int i,j;
	//int k=0;
	for(j=0;j<10;j++)
	{
		//k=0;
		//k=(k+1)%3;
		for(i=-5;i<5;i++)
		{
				if(tl[i+5][j]==0)//((i%2==0 && j%2==0 && i!=0)||(tl[i+5][j]==1))//((i*313 + j + ran)%(ran+1)==0)   //MISSING TILE
				{
					//printf("HRFERGI");
					glPushMatrix();
					//drawCube(i,j,0,0,0);
					glPopMatrix();
					
					if(posx>i-0.4 && posx<i+0.4)
					{
						if(posz>j-0.4 && posz<j+0.4 && posy<0.001 && oni==0 && oni2==0 && onj==0)
						{
							//posx=prevx;
							//posz=prevz;
							down=1;
							posy-=0.07;
							//printf("%f posy jump\n",posy);
							if(posy<-2)
								exit(0);
						}
					}
				}
				else if(tl[i+5][j]==2)//((j + i*20 +ran)%(ran+2)==0)//(i%3==0 && j%7==0 && i!=0)//if((j + i*20 +ran)%(ran+2)==0)  //UPDOWN
				{
					glPushMatrix();
					glScalef(1,updown,1);
					drawCube(i,j,0.92,0.64,0.51);
					glPopMatrix();
					//printf("updown %f\n",updown);	
					if(posx>=i-0.6 && posx<=i+0.6)
					{
						if(posz>=j-0.6 && posz<=j+0.6)
						{
							if(updown*0.5>current)
							{
								if(sel==0)
								{
									posx=prevx;
									posz=prevz-0.3;
								}
								else if(sel==1)
								{
									posx=prevx-0.3;
									posz=prevz;
								}
								else if(sel==2)
								{
									posx=prevx;
									posz=prevz+0.3;
								}
								else if(sel==3)
								{
									posx=prevx+0.3;
									posz=prevz;
								}
								posy=0;
							}
							else
							{
								posy=(0.5*(updown))+tiley+bodyheight+legheight+constjump;
								current=0.5*(updown+0.2);
							}
						}
					}
				}
				else if(tl[i+5][j]==3)//(i%2==0 && level==1)//((i+j+ran)%(ran+3)==0 && level==1) //FALLING
				{
					//falling
					glPushMatrix();
					if(fall[i+5][j]<-2)
					{	
						tl[i+5][j]=0;
						continue;
					}
					glTranslatef(0,fall[i+5][j],0);
					drawCube5(i,j,0,1,0);
					glPopMatrix();
					if(posx>i-0.5 && posx<i+0.5)
					{
						if(posz>=j-0.5 && posz<=j+0.5)
						{
							sel_fall[i+5][j]=1;
							posy=fall[i+5][j]+tiley+bodyheight+legheight+0.2;
							if(posy<-2)
							{
								exit(0);
								posy=0;
							}
						}
					}
				}
				else if(tl[i+5][j]==4)//TRAMPOLINE
				{
					glPushMatrix();
					drawCube3(i,j,1,0.69,0.19,0);
					glPopMatrix();
					
					if(posx>i-0.5 && posx<i+0.5)
					{
						if(posz>=j-0.5 && posz<=j+0.5)
						{
							posy+=trampolinevel;
							if(posy==0)
								trampolinevel=0.9;
							if(posy>1)
								trampolinevel*=-1;
							if(posy<0.32)
								trampolinevel*=-1;
							jump2=1;	
							/*posy+=trampolinevel;
							if(posy-0.25>1)
								trampolinevel*=-1;
							if(posy-0.25<0.045)
								trampolinevel*=-1;
							jump2=1;*/	
							/*if(sel==0)
							{
								posz=prevz+0.1;
							}
							else if(sel==1)
							{
								posx=prevx+0.1;
							}
							else if(sel==2)
							{
								posz=prevz-0.1;
							}
							else if(sel==3)
							{
								posx=prevx-0.1;
								posz=prevz;
							}*/
						}
					}

				}
				else if(tl[i+5][j]==5)//TELEPORTER
				{
					drawCube4(i,j,1,0.69,0.19);
					if(posx>=i-0.5 && posx<=i+0.5)
					{
						if(posz>=j-0.5 && posz<=j+0.5)
						{
							posx+=6;
							for(int hh=0;hh<1000000000;hh++)
							{}
						}
					}


				}
				else if(tl[i+5][j]==6)//STATIC BLOCK
				{
					glPushMatrix();
					glTranslatef(0,1,0);
					drawCube3(i,j,1,1,0,1);
					glPopMatrix();
					if(posx>=i-0.6 && posx<=i+0.6)
					{    
						if(posz>=j-0.6 && posz<=j+0.6)
						{    
							if(posy<0.5*4)
							{    
								if(sel==0)
								{    
									posx=prevx;
									posz=prevz-0.3;
								}    
								else if(sel==1)
								{    
									posx=prevx-0.3;
									posz=prevz;
								}
								else if(sel==2)
								{
									posx=prevx;
									posz=prevz+0.3;
								}
								else if(sel==3)
								{
									posx=prevx+0.3;
									posz=prevz;
								}
							}
							else
							{
								posy=(0.5*4) -0.3;
							}
						}
					}

				}
				else if(tl[i+5][j]==1)  //NORMAL TILE
				{
					if(posx>=i-0.5 && posx<=i+0.5)
					{
						if(posz>=j-0.5 && posz<=j+0.5)
						{
							if(jump!=1)
								posy=0;
						}
					}
					drawCube(i,j,0.49,0.79,0.79);
					drawCube(i,j,1,0.69,0.19);
				}
				if(co[i+5][j]==1)
				{
					glPushMatrix();
					//glColor3f(0.255,0.215,0.0);


					glTranslatef(i,1,j);

					GLUquadricObj *Cylinder;
					Cylinder = gluNewQuadric();

					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, _textureId9);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
					glEnable(GL_TEXTURE_GEN_T);
					glBindTexture(GL_TEXTURE_2D, _textureId9);
					glRotatef(rottheta,0,1,0);

					gluCylinder( Cylinder,.2, .2, .1,20,20 );
					glPushMatrix();
					glTranslatef(0,0,.1);
					gluDisk( Cylinder,0,.2, 20,20);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(0,0,0);
					gluDisk( Cylinder,0,.2, 20,20);
					if(posx>=i-0.6 && posx<=i+0.6)
					{    
						if(posz>=j-0.6 && posz<=j+0.6)
						{
							co[i+5][j]=0;
							coin++;
						}
					}

					glPopMatrix();

					//gluSphere(h,jr,20,20);
					glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
					glDisable(GL_TEXTURE_GEN_T);
					glDisable(GL_TEXTURE_2D);


					glPopMatrix();
					/*glPushMatrix();
					glColor3f(0.255,0.215,0.0);
					drawsphere(0.2,i,1.0,j);
					glPopMatrix();
					if(posx>=i-0.6 && posx<=i+0.6)
					{    
						if(posz>=j-0.6 && posz<=j+0.6)
						{
							co[i+5][j]=0;
							coin++;
						}
					}*/
				}
		}
	}
		glPushMatrix();
		glTranslatef(0,1,0);
		drawCube2(moveblocki,6,1,0.42,0.42);
		glPopMatrix();
		if(posx>=moveblocki-0.5 && posx<=moveblocki+0.5)
		{
			if(posz>=6-0.5 && posz<=6+0.5 && posy>-0.001)
			{
				oni=1;
			}
			else
			{
				oni=0;
			}
		}
		else
		{
			oni=0;
		}
		glPushMatrix();
		glTranslatef(0,1,0);
		drawCube2(moveblocki2,3,1,0.42,0.42);
		glPopMatrix();
		if(posx>=moveblocki2-0.5 && posx<=moveblocki2+0.5 && posy>-0.001)
		{
			if(posz>=3-0.5 && posz<=3+0.5)
			{
				//if(posy+1.3<=moveblockjy && moveblockjy>=0)
				//{
				oni2=1;
			}
			else
			{
				oni2=0;
			}
		}
		else
		{
			oni2=0;
		}
	
		glPushMatrix();
		glTranslatef(0,1,0);
		//glTranslatef(0,moveblockjy,0);
		//drawCube(moveblocki,6,0.92,0.64,0.52);
		drawCube2(3,moveblockj,1,0.42,0.42);
		glPopMatrix();
		if(posx>=3-0.5 && posx<=3+0.5)
		{
			if(posz>=moveblockj-0.5 && posz<=moveblockj+0.5 && posy>-0.001)
			{
				//if(posy+1.3<=moveblockjy && moveblockjy>=0)
				//{
				//}
				onj=1;
			}
			else
			{
				onj=0;
			}
		}
		else
		{
			onj=0;
		}


		//ENEMY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		glPushMatrix();
		modenemy1=(modenemy1+1)%2;
		//glTranslatef(0,1,0);
		//drawCube2(4,moveenemyj1,1,0.42,0.42);
		glColor3f(0.255,0.215,0.0);
		//drawsphere(0.2,3,0,moveblockj);
		glPushMatrix();
		glTranslatef(-1,1,moveenemyj1);
		if(moveenemyj1>7 ||  moveenemyj1<5)
			modenemy2=(modenemy2+1)%2;
		glRotatef(modenemy2*180,0,1,0);
		glScalef(0.6,0.6,0.6);
		glColor3f(0.55,0.47,0.14);
		glColor3f(0.72,0.45,0.20);
		if(modenemy1%2==0)
			glmDraw(p1, GLM_COLOR | GLM_SMOOTH | GLM_MATERIAL);
		else
			glmDraw(p2, GLM_COLOR | GLM_SMOOTH | GLM_MATERIAL);
		glPopMatrix();



		glPopMatrix();
		if(posx>=-1-0.5 && posx<=-1+0.5)
		{
			if(posz>=moveenemyj1-0.5 && posz<=moveenemyj1+0.5 && posy>-0.001)
			{

				//if(posy+1.3<=moveblockjy && moveblockjy>=0)
				//{
				//}
				exit(0);
			}
		}
		////////////////////////////

	//}
	glPopMatrix();	
}
void drawScene(void)
{
	glLoadIdentity();

	
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	if(sel_f==1)//FIRST PERSIN VIEW
	{
		float y=posy+bodyheight+bodyradius/1.5;
		float z=posz+head;
		//float z=posz+head-eyeradius/1.5;
		if(sel==0)
		gluLookAt(posx, y,posz -5, posx, y-0.3, z+1.1-5,0,1,0);
		if(sel==1)
		gluLookAt(posx, y, posz-5, posx+2.1, y-0.3,posz-5,0,1,0);
		if(sel==2)
		gluLookAt(posx, y, posz-5, posx, y-0.3, posz-1.1-5,0,1,0);
		if(sel==3)
		gluLookAt(posx, y, posz-5, posx-2.1, y-0.3, posz-5,0,1,0);
	}
	else if(sel_t==1)//THIRD PERSON VIEW
	{	float y=posy+bodyheight+bodyradius/1.5;
		float z=posz+head;
		//float z=posz+head-eyeradius/1.5;
		if(sel==0)
		gluLookAt(posx, y+1,posz-3-5 , posx, y-3, z+1.1-5,0,1,0);
		if(sel==1)
		gluLookAt(posx-3, y+1, posz-0-5, posx+2.1, y-2,posz-5,0,1,0);
		if(sel==2)
		gluLookAt(posx, y+1, posz+3-5, posx, y-1.5, posz-1.1-5,0,1,0);
		if(sel==3)
		gluLookAt(posx+3, y+1, posz-0-5, posx-2.1, y-2, posz-5,0,1,0);
	}
	else if(sel_h==1) //HELICOPTER VIEW
	{
		gluLookAt(0, 8-addpassivez, 3, passivex, passivey, 2-passivez, 0.0, 1.0, 0.0);
		//gluLookAt(0, 10-addpassivez, 10, passivex, passivey, passivez, 0.0, 1.0, 0.0);
		//gluLookAt(passivex, passivey,-4,0,0,0, 0.0, 1.0, 0.0);
	}
	else if(sel_z==1)//DRAG VIEW
	{
		gluLookAt(passivex,passivey-addpassivez,passivez,0,0,0,0,1,0);
		//gluLookAt(dragx, 10, -11+dragz, 0, 0, 0, 0.0, 1.0, 0.0);
		//gluLookAt(dragx, 10, dragz, 0, 0, 0, 0.0, 1.0, 0.0);
		//gluLookAt(dragshiftx, 3+addpassivez, 3, 0, 0, 4-passivez, 0.0, 1.0, 0.0);
	}
	else if(sel_n==1) //NORMAL FROM FRONT
	{
		gluLookAt(0.1, 3.3, 14.9, 0.0, 1.25, 0.0, 0.0, 1.0, 0.0);
	}
	else if(sel_p==1)//TOWER BETTER VIEW
	{
		gluLookAt(0,4.3, -7, posx, posy, posz,0,1,0);
	}
	else if(sel_s==1)
	{
		if(tilei%3==0 && tilej%9==0)//((tilej + tilei*20 +ran)%(ran+2)==0)
			gluLookAt(tilei,updown*0.5 + 1,tilej-5,tilei,0,tilek-5,0,1,0);
		else if((tilei*7 + tilej + ran)%(ran+5)==0)
			gluLookAt(tilei,4*0.5 + 1,tilej-5,tilei,0,tilek-5,0,1,0);
		else
			gluLookAt(tilei,1,tilej-5,tilei,0,tilek-5,0,1,0);
		
	}
	else if(sel_R==1)
	{
	    	gluLookAt(0+cam, 2.3-cam, 6.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
	}
	else // NORMAL TOWER VIEW
	{
	    	gluLookAt(0+cam, 5.3-cam, 5.0, 0.0, 0.25, -2.0, 0.0, 1.0, 0.0);
	    	//gluLookAt(0+cam, 2.3-cam, 6.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
	}
	if(sel_F==1)
	{
		glEnable(GL_FOG);
		GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1};
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 5.0f);
		glFogf(GL_FOG_END, 10.0f);
	}
	if(sel_F==0)
	{
	
		glDisable(GL_FOG);
	}
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
/*	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-42, -7, -20);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-12, -7, -20);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-12, 10, -20);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-42, 10, -20);
	glEnd();

	
	glDisable(GL_TEXTURE_2D);
*/
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-22, -22, -20);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(22, -22, -20);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(22, 20, -30);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-22, 20, -30);
	glEnd();

	
	glDisable(GL_TEXTURE_2D);


	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(22, -22, -20);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(22, -22, 20);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(22, 20, 5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(22, 20, -30);
	
	

	glEnd();

	
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-22, -22, -20);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-22, -22, 20);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-22, 20, 5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-22, 20, -30);
	
	

	glEnd();

	
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-32, -12, 20);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(32, -12, 20);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(22, -12, -20);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-22, -12, -20);
	glEnd();

	
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-42, -22, 20);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(42, -22, 20);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(42, 20, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-42, 20, 0);
	glEnd();

	
	glDisable(GL_TEXTURE_2D);







	//glScalef(0.3,0.3,0.3);
	//printf("%f\n",vel);
	glTranslatef(0,0,-5);
	if (vel>0.000001)
	{
		//printf("in motion\n");
		if(var%2==1)
		{
			ang3=60;
			ang0=0;
			ang4=60;
			ang1=0;
		}
		if(var%2==0)
		{
			ang0=60;
			ang3=0;
			ang1=60;
			ang4=0;
		}
	}
	else
	{
	//printf("?ELSE\n");
		ang0=0;
		ang3=0;
		ang1=0;
		ang4=0;

	}
	//DrawCube();
	drawTile();
	
	
	//drawrobo();
		
	glPushMatrix();
	//glTranslatef(posx,posy-0.2,posz);
	glTranslatef(posx,posy,posz);
	//glScalef(1.5,1.5,1.5);
	glScalef(0.6,0.6,0.6);
	if (sel==1)
		glRotatef(90,0.0,1.0,0.0);
	else if (sel==2)
		glRotatef(180,0.0,1.0,0.0);
	else if (sel==3)
		glRotatef(270,0.0,1.0,0.0);
	
	glColor3f(1.0,1.0,1.0);
	//GLfloat groundplane[]={0,1,0,0};
	//GLfloat lightpos[] = {posx,5,posz-5,1.0};
	//shadowMatrix(groundplane,lightpos);
	//glMultMatrixf(shadowMat[3]);
	if(modper%2==0)
		glmDraw(p1, GLM_COLOR );
	else
		glmDraw(p2, GLM_COLOR );
	//printf("modper is %d vel=%f\n",modper,vel);
	//glmDraw(p1, GLM_COLOR);
	glPopMatrix();

	//glTranslatef(0,-1,0);
	
	glPushMatrix();
	glTranslatef(-15,6,-15);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	
	glBegin(GL_QUADS);
	glColor3f(1,1,1);	
	glTexCoord2f(0,0);
	glVertex3f(0,0,0);
	
	glTexCoord2f(1,0);
	glVertex3f(coin/1.2,0,0);
	
	glTexCoord2f(1,1);
	glVertex3f(coin/1.2,0.6,0);
	
	glTexCoord2f(0,1);
	glVertex3f(0,0.6,0);
	
	glEnd();

	glPopMatrix();
	
	glPushMatrix();
	glColor3f(1,1,0);
	glTranslatef(0,1,0);
	glScalef(0.4,0.4,1);
	glColor3f(1,1,1);
	t3dDraw3D(STRS[1], 0, 0, 0.2f);
	glColor3f(1,1,1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0,1,0);
	glTranslatef(-5,1,9);
	glRotatef(180,0,1,0);
	glScalef(0.4,0.4,1);
	t3dDraw3D(STRS[0], 0, 0, 0.2f);
	glColor3f(1,1,1);
	glPopMatrix();

	glPopMatrix();

	GLfloat ambientColor[] = {aa,bb , cc, 1.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	GLfloat lightColor0[] = {aa, bb, cc, 1.0f}; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {0.0f, 0.0f, 5.0f, 1.0f}; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = {aa, bb, bb, 1.0f}; //Color (0.5, 0.2, 0.2)
	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	GLfloat ambientLigh[] = {1, 1, 1, 1.0f};
	GLfloat position[4] = {posx,5,posz-5,1.0};
	float LightDir[3] = {0,-1,0}; // towards the viewer
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 7);
	glLightfv(GL_LIGHT2, GL_POSITION, position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, LightDir);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLigh);
//	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,100.0f); // spot

	
	glFlush();
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h); 

	//glOrtho(-100,100,-100,100,0.5,100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat) w / (GLfloat) h, 0.5, 100.0);
	//gluPerspective(90.0, 1, 0.5, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	p1 = glmReadOBJ("shaolin_left.obj");
	p2 = glmReadOBJ("shaolin_right.obj");
	glmUnitize(p1);
	glmFacetNormals(p1);
	glmVertexNormals(p1, 90.0);
	
	glmUnitize(p2);
	glmFacetNormals(p2);
	glmVertexNormals(p2, 90.0);

		
	glClearColor(0.0, 0.0, 0.0, 0.0);


	Image* image = loadBMP("vtr.bmp");
	_textureId = loadTexture(image);
	delete image;
	Image* image2 = loadBMP("images.bmp");
	_textureId2 = loadTexture(image2);
	delete image2;
	Image* image3 = loadBMP("images.bmp");
	_textureId3 = loadTexture(image3);
	delete image3;
	Image* image4 = loadBMP("ocean.bmp");
	_textureId4 = loadTexture(image4);
	delete image4;
	Image* image5 = loadBMP("brick.bmp");
	_textureId5 = loadTexture(image5);
	delete image5;
	Image* image6 = loadBMP("SKY.bmp");
	_textureId6 = loadTexture(image6);
	delete image6;
	Image* image7 = loadBMP("trampoline.bmp");
	_textureId7 = loadTexture(image7);
	delete image7;
	Image* image8 = loadBMP("teleporter.bmp");
	_textureId8 = loadTexture(image8);
	delete image8;
	Image* image9 = loadBMP("coin.bmp");
	_textureId9 = loadTexture(image9);
	delete image9;
	
	t3dInit();
}

int  main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowwidth, windowheight);
	glutCreateWindow("robot");


	//movingtiles[10];
	//movingtilesi[0]=
	//missing tiles[10];
	//block[10];
	//int i,x;
//1=static
//2=updown
//3=falling

	/*tl[-5+5][6]=1;tl[-4+5][3]=2;tl[2][3]=2;
	tl[-3+5][1]=1;tl[-4+5][8]=2;tl[3][9]=2;
	tl[-2+5][5]=1;tl[-1+5][1]=2;
	tl[3+5][6]=1;tl[0+5][6]=2;	
	tl[-5+5][4]=1;
	tl[-5+5][2]=1;
	tl[-5+5][5]=1;
	tl[-5+5][3]=1;
*/
	tl[-4+5][2]=1;
	tl[-1+5][0]=1;
	tl[0+5][0]=1;
	tl[1+5][0]=1;
	tl[2+5][0]=1;

	tl[-4+5][4]=2;
	tl[-2+5][1]=2;
	tl[-2+5][7]=2;
	tl[0+5][3]=1;
	tl[1+5][0]=1;
	tl[1+5][5]=2;
	tl[1+5][8]=1;
	tl[3+5][1]=2;
	tl[4+5][8]=2;
	tl[-4+5][8]=3;
	tl[-2+5][3]=3;
	tl[0+5][6]=3;
	tl[4+5][5]=3;
	for(int i=1;i<5;i++)
	tl[i][0]=1;
	
	for(int i=0;i<2;i++)
		tl[i][1]=1;

	tl[7][1]=1;
	tl[9][1]=1;
	tl[8][2]=1;
	tl[9][2]=1;
	tl[4][2]=1;

	for(int i=6;i<9;i++)
		tl[i][4]=1;

	for(int i=0;i<2;i++)
		tl[i][5]=1;
	
	for(int i=5;i<8;i++)
		tl[4][i]=1;
	tl[9][7]=1;

	for(int i=0;i<2;i++)
		tl[i][7]=1;

	for(int i=7;i<9;i++)
		tl[7][i]=1;
	tl[1][3]=1;
	tl[7][2]=1;
	for(int i=0;i<8;i++)
		tl[i][9]=1;
	tl[9][9]=1;
	for(int i=3;i<6;i++)
		tl[2][i]=1;
	tl[3][5]=4;
	tl[5][1]=4;

	tl[2][2]=5;
	tl[2][4]=5;

	tl[6][7]=6;
	
	co[7][1]=1;
	co[9][1]=1;
	co[8][2]=1;
	co[9][2]=1;
	co[4][2]=1;

	

	srand(time(NULL));
	ran=rand()%12;
	ang0=60;
	ang3=0;
	ang4=0;
	init();
	_scale = computeScale(STRS);
	
	initLighting();
		
	glutReshapeFunc(Reshape);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
  	glutMouseFunc(handleMouseclick);
	glutPassiveMotionFunc(myMouseMove);
	glutTimerFunc(25 , update , 0);
	glutTimerFunc(25, update2 , 0);
	glutMainLoop();
	return 0;
}
void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 27) {
		exit(0);     // escape key is pressed
	}
	if(key==106)
	{
		//printf("j pressed\n");
		posy+=1.9;
		jump=1;
		constjump=0.9;
	}
	if(key==102)  //First Person
	{
		sel_f=(sel_f+1)%2;
	}
	if(key==110)//Normal View In Front
	{
		sel_n=(sel_n+1)%2;
	}
	/*if(key==102)//Forward Camera
	{
		cam+=0.5;
	}
	if(key==98)//Back Camera
	{
		cam-=0.5;
	}*/
	if(key==104)//Helicoopter View
	{
		sel_h=(sel_h+1)%2;
	}
	if(key==116)//Third Person
	{
		sel_t=(sel_t+1)%2;
	}
	if(key==112)//Howering View
	{
		sel_p=(sel_p+1)%2;
	}
	if(key==115)//Tile View
	{
		sel_s=(sel_s+1)%2;
	}
	if(key==108)//left tile
	{
		tilei+=1;
		if(tilei>=5)
			tilei=-5;
	}
	if(key==114)//right tile
	{
		tilei-=1;
		if(tilei<=-6)
		{
			tilei=5;
		}
	}
	if(key==117)//up tile
	{
		tilej+=1;
		if(tilej>=10)
		{
			tilej=0;
			tilek=10;
		}
	}
	if(key==100)//down tile
	{
		tilej-=1;
		if(tilej<=-1)
		{
			tilej=10;
			tilek=-10;
		}
	}
	if(key==122) // DRAG MODE
	{
		sel_z=(sel_z+1)%2;
	}
	if(key==82) //OK MODE
	{
		sel_R=(sel_R+1)%2;
	}
	if(key==70)
	{
		sel_F=(sel_F+1)%2;
	}
}


void handleKeypress2(int key, int x, int y)
{
	if (key== GLUT_KEY_UP)
	{
		if (vel==0.0)
			vel=0.01;
		else if(vel!=1)
			vel+=0.01;

		/*if (sel==0 && down==0)
			posz+=0.05;
		else if (sel==1 && down==0)
			posx+=0.05;
		else if (sel==2 && down==0)
			posz-=0.05;
		else if (sel==3 && down==0)
			posx-=0.05;
		*/
	}
	else if (key== GLUT_KEY_DOWN)
	{
		if (vel>=0.6)
			vel-=0.01;
	}
	else if (key== GLUT_KEY_RIGHT)
	{
		if (sel==0)
			sel=3;
		else
			sel--;
	}
	else if (key== GLUT_KEY_LEFT)
	{
		if (sel==3)
			sel=0;
		else
			sel++;
	}
}
