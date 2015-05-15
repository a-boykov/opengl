/*
*	File: Shader.h
*	Created on: 6/10/2014
*	Author: kalin
*/

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef unsigned int GLenum;

class Shader
{
public:
	Shader();
	~Shader();

	void InitializeProgram(const std::string &strVertexShader, const std::string &strFragmentShader);

	GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
	GLuint CreateProgram(const std::vector<GLuint> &shaderList);

	void Use(bool var);
	void SetTime(float var);
	void SetWidth(float var);
	void SetHeight(float var);
	void SetPosition(int x, int y);


	GLenum GetPositionAttrib();
	GLuint GetColorAttrib();
	GLuint GetModelToCameraMatrixUnif();
	GLuint GetZFar();

	glm::mat4 GetCameraToClipMx();

	void CalcFrustumScale(float fFovDeg);
	float fFrustumScale;

private:
	const char* ReadFile(const char *file);

	glm::mat4 cameraToClipMatrix;
	GLuint positionAttrib;
	GLuint colorAttrib;
	GLuint modelToCameraMatrixUnif;
	GLuint cameraToClipMatrixUnif;

	GLenum program;
	GLenum time;
	GLenum maskWidth;
	GLenum maskHeight;
	GLenum position;
	bool used;
};

#endif // SHADER_H
