#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include <FreeImage.h>
#include "Shader.h"
#include <string.h>
//#include <GL/glu.h>
//#include <GL/gl.h>
//#include <iostream>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;
const int INSCRIPTION_WIDTH = 400;
const int INSCRIPTION_HEIGHT = 70;
const int INSCRIPTION_X1 = 320;
const int INSCRIPTION_Y1 = 5;
const int INSCRIPTION_X2 = 740;
const int INSCRIPTION_Y2 = 5;

GLenum image1;
GLenum image2;
GLenum image3;
GLenum background;

Shader shader1;
Shader shader2;
Shader shader3;
Shader shColorTopBottom;
Shader shMove;

const float vertexPositions[] = {
	0.25f, 0.25f, 0.0f, 1.0f,
	0.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;

void TimeEvent(int te);
void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void InitializeVertexBuffer();
void ComputePositionOffsets(float &fXOffset, float &fYOffset);
void AdjustVertexData(float fXOffset, float fYOffset);

GLenum loadTexture(const char *textureFile);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (1400, 900);
	glutInitWindowPosition (10, 10);
	glutCreateWindow (argv[0]);
	glutSetWindowTitle("Shader test");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutTimerFunc( 10, TimeEvent, 1);
	glutMainLoop();

	return 0;
}

void TimeEvent(int )
{
	static float time1 = 0;
	static float time2 = 0;
	static float delta1 = 0.005;
	static float delta2 = 0.02;

	glutPostRedisplay();
	shader1.Use(true);
	shader1.SetTime(time1);
	shader2.Use(true);
	shader2.SetTime(time2);
	//shader3.Use(true);
	//shader3.SetTime(time2);
	glutTimerFunc( 10, TimeEvent, 1);

	time1 += delta1;
	time2 += delta2;

	if(time1 > 1.0f)
		time1 = 0.0f;


	if(time2 > 1.0f || time2 < 0)
		delta2 = -delta2;
}

void InitializeVertexBuffer()
{
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init(void)
{
	image1 = loadTexture("data/pick_01.png");
	image2 = loadTexture("data/pick_02.png");
	image3 = loadTexture("data/mask005.png");
	background = loadTexture("data/screen.png");
	shader1.InitializeProgram("glsl/glow.vert","glsl/glow.frag", true);
	shader1.Use(true);
	shader1.SetWidth(2);
	shader1.SetHeight(1.0);

	shader2.InitializeProgram("glsl/color.vert","glsl/color.frag", false);

	//shader3.InitializeProgram("glsl/grayscale.vert","glsl/grayscale.frag");

	//shColorTopBottom.InitializeProgram("glsl/colortopbottom.vert", "glsl/colortopbottom.frag");

	// Smooth Color GLSL BEGIN
	//shSmoothColor.InitializeProgram("glsl/smoothcolor.vert", "glsl/smoothcolor.frag");
	// Smooth Color GLSL END

	// GLSL Moving triangle BEGIN
	shMove.InitializeProgram("glsl/standard.vert", "glsl/standard.frag");
	InitializeVertexBuffer();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// GLSL Moving triangle END

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, -SCREEN_HEIGHT/2, 1.0, 30.0);
}

void ComputePositionOffsets(float &fXOffset, float &fYOffset)
{
	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.5f;
	fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.5f;
}

void AdjustVertexData(float fXOffset, float fYOffset)
{
	std::vector<float> fNewData(ARRAY_COUNT(vertexPositions));
	memcpy(&fNewData[0], vertexPositions, sizeof(vertexPositions));

	for(int iVertex = 0; iVertex < ARRAY_COUNT(vertexPositions); iVertex += 4)
	{
		fNewData[iVertex] += fXOffset;
		fNewData[iVertex + 1] += fYOffset;
	}

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), &fNewData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display(void)
{
	float fXOffset = 0.0f, fYOffset = 0.0f;
	ComputePositionOffsets(fXOffset, fYOffset);
	AdjustVertexData(fXOffset, fYOffset);

	glEnable(GL_BLEND);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, -SCREEN_HEIGHT/2, 1.0, 30.0);
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 1.0, 30.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Put view state on screen
	glColor3f( 1.0, 1.0, 1.0);

	glPushMatrix();
	glTranslatef(0,0,-10);

	shMove.Use(true);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	shMove.Use(false);

//	glEnable(GL_TEXTURE_2D);

//	shader1.Use(false);
//	shader2.Use(false);
//	//shader3.Use(false);
//	//shColorTopBottom.Use(false);


//	//shader3.Use(true);
//	//shColorTopBottom.Use(true);

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, background);
//	glBegin(GL_QUADS);

//	glMultiTexCoord2f(GL_TEXTURE0,0,0);
//	glVertex3f(0,0,0);

//	glMultiTexCoord2f(GL_TEXTURE0,1,0);
//	glVertex3f( SCREEN_WIDTH,0,0);

//	glMultiTexCoord2f(GL_TEXTURE0,1,1);
//	glVertex3f( SCREEN_WIDTH, SCREEN_HEIGHT,0);

//	glMultiTexCoord2f(GL_TEXTURE0,0,1);
//	glVertex3f(0, SCREEN_HEIGHT, 0);

//	glEnd();
//	//shader3.Use(false);
//	//shColorTopBottom.Use(false);



//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, 0);

//	shader1.Use(true);

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, image1);

//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, image2);

//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, image3);

//	glBegin(GL_QUADS);
//	glMultiTexCoord2f(GL_TEXTURE0,0,0);
//	glMultiTexCoord2f(GL_TEXTURE1,0,0);
//	glMultiTexCoord2f(GL_TEXTURE2,0,0);
//	glVertex3f(INSCRIPTION_X1,INSCRIPTION_Y1,1);

//	glMultiTexCoord2f(GL_TEXTURE0,1,0);
//	glMultiTexCoord2f(GL_TEXTURE1,1,0);
//	glMultiTexCoord2f(GL_TEXTURE2,1,0);
//	glVertex3f(INSCRIPTION_X1+INSCRIPTION_WIDTH,INSCRIPTION_Y1,1);

//	glMultiTexCoord2f(GL_TEXTURE0,1,1);
//	glMultiTexCoord2f(GL_TEXTURE1,1,1);
//	glMultiTexCoord2f(GL_TEXTURE2,1,1);
//	glVertex3f(INSCRIPTION_X1+INSCRIPTION_WIDTH,INSCRIPTION_Y1 + INSCRIPTION_HEIGHT,1);

//	glMultiTexCoord2f(GL_TEXTURE0,0,1);
//	glMultiTexCoord2f(GL_TEXTURE1,0,1);
//	glMultiTexCoord2f(GL_TEXTURE2,0,1);
//	glVertex3f(INSCRIPTION_X1,INSCRIPTION_Y1 + INSCRIPTION_HEIGHT,1);

//	glEnd();

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, 0);

//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, 0);

//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, 0);

//	shader1.Use(false);
//	shader2.Use(true);

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, image1);

//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, image2);

//	glBegin(GL_QUADS);
//	glMultiTexCoord2f(GL_TEXTURE0,0,0);
//	glMultiTexCoord2f(GL_TEXTURE1,0,0);
//	glVertex3f(INSCRIPTION_X2,INSCRIPTION_Y2,1);

//	glMultiTexCoord2f(GL_TEXTURE0,1,0);
//	glMultiTexCoord2f(GL_TEXTURE1,1,0);
//	glVertex3f(INSCRIPTION_X2+INSCRIPTION_WIDTH,INSCRIPTION_Y2,1);

//	glMultiTexCoord2f(GL_TEXTURE0,1,1);
//	glMultiTexCoord2f(GL_TEXTURE1,1,1);
//	glVertex3f(INSCRIPTION_X2+INSCRIPTION_WIDTH,INSCRIPTION_Y2 + INSCRIPTION_HEIGHT,1);

//	glMultiTexCoord2f(GL_TEXTURE0,0,1);
//	glMultiTexCoord2f(GL_TEXTURE1,0,1);
//	glVertex3f(INSCRIPTION_X2,INSCRIPTION_Y2 + INSCRIPTION_HEIGHT,1);

//	glEnd();

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, 0);

//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, 0);

//	shader2.Use(false);

	glPopMatrix();

	glDisable(GL_BLEND);


	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
}

void keyboard (unsigned char key,int x,int y)
{
	switch (key)
		{
		case 27:
			glutLeaveMainLoop();
			return;
		}

}

GLenum loadTexture(const char * textureFile)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(textureFile,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, textureFile);

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	GLubyte* textura = new GLubyte[4*w*h];
	char* pixeles = (char*)FreeImage_GetBits(imagen);
	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

	for(int j= 0; j<w*h; j++)
		{
			textura[j*4+0]= pixeles[j*4+2];
			textura[j*4+1]= pixeles[j*4+1];
			textura[j*4+2]= pixeles[j*4+0];
			textura[j*4+3]= pixeles[j*4+3];
			//cout<<j<<": "<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<endl;
		}

	//Now generate the OpenGL texture object

	GLenum texturaID;

	glGenTextures(1,&texturaID);
	glBindTexture(GL_TEXTURE_2D, texturaID);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)textura );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLenum huboError = glGetError();
	if(huboError)
		return 0;

	return texturaID;
}
