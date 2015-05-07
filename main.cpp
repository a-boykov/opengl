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
Shader shMoveBetter;

const int numberOfVertices = 8;

#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertexData[] = {
	//Object 1 positions
	+1.0f, +1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	+1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,

	//Object 1 colors
	GREEN_COLOR,
	BLUE_COLOR,
	RED_COLOR,
	BROWN_COLOR,

	GREEN_COLOR,
	BLUE_COLOR,
	RED_COLOR,
	BROWN_COLOR
};

const GLshort indexData[] =
{
	//obj 1
	0, 1, 2,
	1, 0, 3,
	2, 3, 0,
	3, 2, 1,

	5, 4, 6,
	4, 5, 7,
	7, 6, 4,
	6, 7, 5
};

GLuint	vao;
GLuint vertexBufferObject;
GLuint indexBufferObject;


void TimeEvent(int te);
void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void InitializeVertexBuffer();
void InitializeVertexArrayObj();
void ComputePositionOffsets(float &fXOffset, float &fYOffset);
void AdjustVertexData(float fXOffset, float fYOffset);

glm::vec3 StationaryOffset(float fElapsedTime);
glm::vec3 OvalOffset(float fElapsedTime);
glm::vec3 BottomCircleOffset(float fElapsedTime);
glm::mat4 ConstructMatrix(float fElapsedTime);

GLenum loadTexture(const char *textureFile);

struct Instance
{
	typedef glm::vec3(*OffsetFunc)(float);

	OffsetFunc CalcOffset;

	glm::mat4 ConstructMatrix(float fElapsedTime)
	{
		glm::mat4 theMat(1.0f);

		theMat[3] = glm::vec4(CalcOffset(fElapsedTime), 1.0f);

		return theMat;
	}
};

Instance g_instaceList[] =
{
	{StationaryOffset},
	{OvalOffset},
	{BottomCircleOffset}
};


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (800, 600);
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
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

glm::vec3 StationaryOffset(float fElapsedTime)
{
	return glm::vec3(0.0f, 0.0f, -20.0f);
}

glm::vec3 OvalOffset(float fElapsedTime)
{
	const float fLoopDuration = 3.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	return glm::vec3(cosf(fCurrTimeThroughLoop * fScale) * 4.0f,
					 sinf(fCurrTimeThroughLoop * fScale) * 6.0f,
					 -20.0f);
}

glm::vec3 BottomCircleOffset(float fElapsedTime)
{
	const float fLoopDuration = 12.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	return glm::vec3(cosf(fCurrTimeThroughLoop * fScale) * 5.0f,
					 -3.5f,
					 sinf(fCurrTimeThroughLoop * fScale) * 5.0f - 20.0f);
}

void InitializeVertexArrayObj()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

	glBindVertexArray(0);
}

void init(void)
{
	shader1.CalcFrustumScale(80.0f);
	shader1.InitializeProgram("glsl/poscolorlocaltransform.vert","glsl/smoothcolor.frag");

	InitializeVertexBuffer();
	InitializeVertexArrayObj();
	// GLSL Moving triangle END

//	glClearColor (0.0, 0.0, 0.0, 0.0);
//	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, -SCREEN_HEIGHT/2, 1.0, 30.0);
}

void display(void)
{

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	glMatrixMode (GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 1.0, 30.0);

//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();

//	// Put view state on screen
//	glColor3f( 1.0, 1.0, 1.0);

//	glPushMatrix();
//	glTranslatef(0,0,-10);

	shader1.Use(true);
	glBindVertexArray(vao);

	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	for(int iLoop = 0; iLoop < ARRAY_COUNT(g_instaceList); ++iLoop)
	{
		Instance &currInst = g_instaceList[iLoop];
		const glm::mat4 &transformMatrix = currInst.ConstructMatrix(fElapsedTime);

		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(transformMatrix));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
	}

	glBindVertexArray(0);
	shader1.Use(false);

//	glPopMatrix();
//	glDisable(GL_BLEND);


	glutSwapBuffers();

}

void reshape (int w, int h)
{
	if ( w < h )
		glViewport (0, 0, (GLsizei) w, (GLsizei) w);
	else
		glViewport (0, 0, (GLsizei) h, (GLsizei) h);
//	glMatrixMode (GL_PROJECTION);
//	glLoadIdentity ();
}

void keyboard (unsigned char key,int x,int y)
{
	static bool bDepthClampingActive = false;
	switch (key)
		{
		case 27:
			glutLeaveMainLoop();
			return;
			break;
		case 32:
			if(bDepthClampingActive)
				glDisable(GL_DEPTH_CLAMP);
			else
				glEnable(GL_DEPTH_CLAMP);

			bDepthClampingActive = !bDepthClampingActive;
			break;
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

