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
#include <hierarchy.h>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

const int FPS_CAP = 30;
int frame=0,fps_time,fps_timebase=0, fps;
char buffer[3];


const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;


const int numberOfVertices = 24;

#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f

#define YELLOW_COLOR 1.0f, 1.0f, 0.0f, 1.0f
#define CYAN_COLOR 0.0f, 1.0f, 1.0f, 1.0f
#define MAGENTA_COLOR 1.0f, 0.0f, 1.0f, 1.0f

const float vertexData[] = {
	//Front
	+1.0f, +1.0f, +1.0f,
	+1.0f, -1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,

	//Top
	+1.0f, +1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,
	-1.0f, +1.0f, -1.0f,
	+1.0f, +1.0f, -1.0f,

	//Left
	+1.0f, +1.0f, +1.0f,
	+1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, +1.0f,

	//Back
	+1.0f, +1.0f, -1.0f,
	-1.0f, +1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,

	//Bottom
	+1.0f, -1.0f, +1.0f,
	+1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, +1.0f,

	//Right
	-1.0f, +1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, +1.0f, -1.0f,


	//Object colors
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,
	GREEN_COLOR,

	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,

	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,

	YELLOW_COLOR,
	YELLOW_COLOR,
	YELLOW_COLOR,
	YELLOW_COLOR,

	CYAN_COLOR,
	CYAN_COLOR,
	CYAN_COLOR,
	CYAN_COLOR,

	MAGENTA_COLOR,
	MAGENTA_COLOR,
	MAGENTA_COLOR,
	MAGENTA_COLOR
};

const GLshort indexData[] =
{
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,
	18, 19, 16,

	20, 21, 22,
	22, 23, 20
};

GLuint vao;
GLuint vertexBufferObject;
GLuint indexBufferObject;
Shader shader1;
Hierarchy g_armature(*vertexData, *indexData, *vao);

void TimeEvent(int te);
void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void InitializeVertexBuffer();

GLenum loadTexture(const char *textureFile);


void TimeEvent(int )
{
	glutPostRedisplay();

	//get current FPS
	frame++;
	fps_time=glutGet(GLUT_ELAPSED_TIME);

	if (fps_time - fps_timebase > 1000)
	{
		fps = frame*1000.0/(fps_time-fps_timebase);
		fps_timebase = fps_time;
		frame = 0;
	}
	//end calculation

	sprintf(buffer,"FRAME RATE: %d", fps);
	glutSetWindowTitle(buffer);

	glutTimerFunc( 1000/FPS_CAP, TimeEvent, 1);

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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	glEnableVertexAttribArray(shader1.GetPositionAttrib());
	glEnableVertexAttribArray(shader1.GetColorAttrib());

	glVertexAttribPointer(shader1.GetPositionAttrib(), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(shader1.GetColorAttrib(), 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

	glBindVertexArray(0);
}


void init(void)
{
	shader1.CalcFrustumScale(40.0f);
	shader1.InitializeProgram("glsl/poscolorlocaltransform.vert","glsl/smoothcolor.frag");

	InitializeVertexBuffer();
	// GLSL Moving triangle END
	g_armature.SetShader(*shader1);

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
	g_armature.Draw();
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

void keyboard (unsigned char key,int /*x*/,int /*y*/)
{
//	static bool bDepthClampingActive = false;
	switch (key)
		{
		case 27:
			glutLeaveMainLoop();
			return;
			break;
//		case 32:
//			if(bDepthClampingActive)
//				glDisable(GL_DEPTH_CLAMP);
//			else
//				glEnable(GL_DEPTH_CLAMP);

//			bDepthClampingActive = !bDepthClampingActive;
//			break;
		case 'a': g_armature.AdjBase(true); break;
		case 'd': g_armature.AdjBase(false); break;
		case 'w': g_armature.AdjUpperArm(false); break;
		case 's': g_armature.AdjUpperArm(true); break;
		case 'r': g_armature.AdjLowerArm(false); break;
		case 'f': g_armature.AdjLowerArm(true); break;
		case 't': g_armature.AdjWristPitch(false); break;
		case 'g': g_armature.AdjWristPitch(true); break;
		case 'z': g_armature.AdjWristRoll(true); break;
		case 'c': g_armature.AdjWristRoll(false); break;
		case 'q': g_armature.AdjFingerOpen(true); break;
		case 'e': g_armature.AdjFingerOpen(false); break;
		case 32: g_armature.WritePose(); break;
		}

}

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
	glutTimerFunc( 1000/FPS_CAP, TimeEvent, 0);
	glutMainLoop();

	return 0;
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

