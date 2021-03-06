//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.


#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <Shader.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))
const int FPS_CAP = 30;
int frame=0,fps_time,fps_timebase=0, fps;
char buffer[3];
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

Shader shader1;
GLuint vao;
GLuint vertexBufferObject;
GLuint indexBufferObject;

const int numberOfVertices = 24;

#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f

#define YELLOW_COLOR 1.0f, 1.0f, 0.0f, 1.0f
#define CYAN_COLOR 0.0f, 1.0f, 1.0f, 1.0f
#define MAGENTA_COLOR 	1.0f, 0.0f, 1.0f, 1.0f

const float vertexData[] =
{
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
	MAGENTA_COLOR,
};

static const GLshort indexData[] =
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
	22, 23, 20,
};

void TimeEvent(int te);
void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void InitializeVertexBuffer();

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

inline float DegToRad(float fAngDeg)
{
	const float fDegToRad = 3.14159f * 2.0f / 360.0f;
	return fAngDeg * fDegToRad;
}

inline float Clamp(float fValue, float fMinValue, float fMaxValue)
{
	if(fValue < fMinValue)
		return fMinValue;

	if(fValue > fMaxValue)
		return fMaxValue;

	return fValue;
}

glm::mat3 RotateX(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[1].y = fCos; theMat[2].y = -fSin;
	theMat[1].z = fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateY(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[2].x = fSin;
	theMat[0].z = -fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateZ(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[1].x = -fSin;
	theMat[0].y = fSin; theMat[1].y = fCos;
	return theMat;
}

class MatrixStack
{
public:
	MatrixStack()
		: m_currMat(1.0f)
	{
	}

	const glm::mat4 &Top()
	{
		return m_currMat;
	}

	void RotateX(float fAngDeg)
	{
		m_currMat = m_currMat * glm::mat4(::RotateX(fAngDeg));
	}

	void RotateY(float fAngDeg)
	{
		m_currMat = m_currMat * glm::mat4(::RotateY(fAngDeg));
	}

	void RotateZ(float fAngDeg)
	{
		m_currMat = m_currMat * glm::mat4(::RotateZ(fAngDeg));
	}

	void Scale(const glm::vec3 &scaleVec)
	{
		glm::mat4 scaleMat(1.0f);
		scaleMat[0].x = scaleVec.x;
		scaleMat[1].y = scaleVec.y;
		scaleMat[2].z = scaleVec.z;

		m_currMat = m_currMat * scaleMat;
	}

	void Translate(const glm::vec3 &offsetVec)
	{
		glm::mat4 translateMat(1.0f);
		translateMat[3] = glm::vec4(offsetVec, 1.0f);

		m_currMat = m_currMat * translateMat;
	}

	void Push()
	{
		m_matrices.push(m_currMat);
	}

	void Pop()
	{
		m_currMat = m_matrices.top();
		m_matrices.pop();
	}

private:
	glm::mat4 m_currMat;
	std::stack<glm::mat4> m_matrices;
};

class Hierarchy
{
public:
	Hierarchy()
		: posBase(glm::vec3(3.0f, -5.0f, -40.0f))
		, angBase(-45.0f)
		, posBaseLeft(glm::vec3(2.0f, 0.0f, 0.0f))
		, posBaseRight(glm::vec3(-2.0f, 0.0f, 0.0f))
		, scaleBaseZ(3.0f)
		, angUpperArm(-33.75f)
		, sizeUpperArm(9.0f)
		, posLowerArm(glm::vec3(0.0f, 0.0f, 8.0f))
		, angLowerArm(146.25f)
		, lenLowerArm(5.0f)
		, widthLowerArm(1.5f)
		, posWrist(glm::vec3(0.0f, 0.0f, 5.0f))
		, angWristRoll(0.0f)
		, angWristPitch(67.5f)
		, lenWrist(2.0f)
		, widthWrist(2.0f)
		, posLeftFinger(glm::vec3(1.0f, 0.0f, 1.0f))
		, posRightFinger(glm::vec3(-1.0f, 0.0f, 1.0f))
		, angFingerOpen(180.0f)
		, lenFinger(2.0f)
		, widthFinger(0.5f)
		, angLowerFinger(45.0f)
	{}

	virtual ~Hierarchy(){}

	void Draw()
	{
		MatrixStack modelToCameraStack;

		shader1.Use(true);
		glBindVertexArray(vao);

		modelToCameraStack.Translate(posBase);
		modelToCameraStack.RotateY(angBase);

		//Draw left base.
		{
			modelToCameraStack.Push();
			modelToCameraStack.Translate(posBaseLeft);
			modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
			glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}

		//Draw right base.
		{
			modelToCameraStack.Push();
			modelToCameraStack.Translate(posBaseRight);
			modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
			glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}

		//Draw main arm.
		DrawUpperArm(modelToCameraStack);

		glBindVertexArray(0);
		shader1.Use(false);
	}

#define STANDARD_ANGLE_INCREMENT 11.25f
#define SMALL_ANGLE_INCREMENT 9.0f

	void AdjBase(bool bIncrement)
	{
		angBase += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angBase = fmodf(angBase, 360.0f);
	}

	void AdjUpperArm(bool bIncrement)
	{
		angUpperArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angUpperArm = Clamp(angUpperArm, -90.0f, 0.0f);
	}

	void AdjLowerArm(bool bIncrement)
	{
		angLowerArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angLowerArm = Clamp(angLowerArm, 0.0f, 146.25f);
	}

	void AdjWristPitch(bool bIncrement)
	{
		angWristPitch += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angWristPitch = Clamp(angWristPitch, 0.0f, 90.0f);
	}

	void AdjWristRoll(bool bIncrement)
	{
		angWristRoll += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angWristRoll = fmodf(angWristRoll, 360.0f);
	}

	void AdjFingerOpen(bool bIncrement)
	{
		angFingerOpen += bIncrement ? SMALL_ANGLE_INCREMENT : -SMALL_ANGLE_INCREMENT;
		angFingerOpen = Clamp(angFingerOpen, 9.0f, 180.0f);
	}

	void WritePose()
	{
		printf("angBase:\t%f\n", angBase);
		printf("angUpperArm:\t%f\n", angUpperArm);
		printf("angLowerArm:\t%f\n", angLowerArm);
		printf("angWristPitch:\t%f\n", angWristPitch);
		printf("angWristRoll:\t%f\n", angWristRoll);
		printf("angFingerOpen:\t%f\n", angFingerOpen);
		printf("\n");
	}

private:
	void DrawFingers(MatrixStack &modelToCameraStack)
	{
		//Draw left finger
		modelToCameraStack.Push();
		modelToCameraStack.Translate(posLeftFinger);
		modelToCameraStack.RotateY(angFingerOpen);

		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
		modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();

		{
			//Draw left lower finger
			modelToCameraStack.Push();
			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
			modelToCameraStack.RotateY(-angLowerFinger);

			modelToCameraStack.Push();
			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
			modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
			glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();

			modelToCameraStack.Pop();
		}

		modelToCameraStack.Pop();

		//Draw right finger
		modelToCameraStack.Push();
		modelToCameraStack.Translate(posRightFinger);
		modelToCameraStack.RotateY(-angFingerOpen);

		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
		modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();

		{
			//Draw right lower finger
			modelToCameraStack.Push();
			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
			modelToCameraStack.RotateY(angLowerFinger);

			modelToCameraStack.Push();
			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
			modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
			glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();

			modelToCameraStack.Pop();
		}

		modelToCameraStack.Pop();
	}

	void DrawWrist(MatrixStack &modelToCameraStack)
	{
		modelToCameraStack.Push();
		modelToCameraStack.Translate(posWrist);
		modelToCameraStack.RotateZ(angWristRoll);
		modelToCameraStack.RotateX(angWristPitch);

		modelToCameraStack.Push();
		modelToCameraStack.Scale(glm::vec3(widthWrist / 2.0f, widthWrist/ 2.0f, lenWrist / 2.0f));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();

		DrawFingers(modelToCameraStack);

		modelToCameraStack.Pop();
	}

	void DrawLowerArm(MatrixStack &modelToCameraStack)
	{
		modelToCameraStack.Push();
		modelToCameraStack.Translate(posLowerArm);
		modelToCameraStack.RotateX(angLowerArm);

		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenLowerArm / 2.0f));
		modelToCameraStack.Scale(glm::vec3(widthLowerArm / 2.0f, widthLowerArm / 2.0f, lenLowerArm / 2.0f));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();

		DrawWrist(modelToCameraStack);

		modelToCameraStack.Pop();
	}

	void DrawUpperArm(MatrixStack &modelToCameraStack)
	{
		modelToCameraStack.Push();
		modelToCameraStack.RotateX(angUpperArm);

		{
			modelToCameraStack.Push();
			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, (sizeUpperArm / 2.0f) - 1.0f));
			modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, sizeUpperArm / 2.0f));
			glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
			modelToCameraStack.Pop();
		}

		DrawLowerArm(modelToCameraStack);

		modelToCameraStack.Pop();
	}

	glm::vec3		posBase;
	float			angBase;

	glm::vec3		posBaseLeft, posBaseRight;
	float			scaleBaseZ;

	float			angUpperArm;
	float			sizeUpperArm;

	glm::vec3		posLowerArm;
	float			angLowerArm;
	float			lenLowerArm;
	float			widthLowerArm;

	glm::vec3		posWrist;
	float			angWristRoll;
	float			angWristPitch;
	float			lenWrist;
	float			widthWrist;

	glm::vec3		posLeftFinger, posRightFinger;
	float			angFingerOpen;
	float			lenFinger;
	float			widthFinger;
	float			angLowerFinger;
};
Hierarchy g_armature;

void init(void)
{
	shader1.CalcFrustumScale(40.0f);
	shader1.InitializeProgram("glsl/poscolorlocaltransform.vert","glsl/smoothcolor.frag");

	InitializeVertexBuffer();
	// GLSL Moving triangle END

//	g_armature.Reinit();

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
	(shader1.GetCameraToClipMx())[0].x = shader1.fFrustumScale * (h / (float)w);
	(shader1.GetCameraToClipMx())[1].y = shader1.fFrustumScale;

	shader1.Use(true);
	glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(shader1.GetCameraToClipMx()));
	shader1.Use(false);

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to
//exit the program.
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
