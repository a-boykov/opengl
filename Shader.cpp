/*
*	File: Shader.cpp
*	Created on: 6/10/2014
*	Author: kalin
*/

#include "Shader.h"
#include <stdio.h>
#include <algorithm>
#include <string.h>

//#include <stdlib.h>
//#include <exception>
//#include <stdexcept>

Shader::Shader():fFrustumScale(0),cameraToClipMatrix(0.0f)
{
}

Shader::~Shader()
{
	Use(false);
	glDeleteProgram(program);
}

void Shader::CalcFrustumScale(float fFovDeg)
{
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	float fFovRad = fFovDeg * degToRad;

	fFrustumScale = 1.0f/tan(fFovRad/2.0f);
}

//const float fFrustumScale = CalcFrustumScale(45.0f);

void Shader::InitializeProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	program = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	positionAttrib = glGetAttribLocation(program, "position");
	colorAttrib = glGetAttribLocation(program, "color");

	modelToCameraMatrixUnif = glGetUniformLocation(program, "modelToCameraMatrix");
	cameraToClipMatrixUnif = glGetUniformLocation(program, "cameraToClipMatrix");

	float fzNear = 1.0f;
	float fzFar = 100.0f;

	cameraToClipMatrix[0].x = fFrustumScale;
	cameraToClipMatrix[1].y = fFrustumScale;
	cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
	cameraToClipMatrix[2].w = -1.0f;
	cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

	Use(true);
	glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix) );
	Use(false);
}

GLuint Shader::CreateShader(GLenum eShaderType, const std::string &strShaderFile){
	GLuint shader = glCreateShader(eShaderType);
	const char* fileData = ReadFile(strShaderFile.c_str());

	glShaderSource(shader, 1, &fileData, NULL);

	delete fileData;

	try
	{
		glCompileShader(shader);
	}
	catch(std::exception &e)
	{
		fprintf(stderr, "%s\n", e.what());
	}

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

			const char *strShaderType = NULL;
			switch (eShaderType)
				{
				case GL_VERTEX_SHADER:
					strShaderType = "vertex";
					break;
				case GL_GEOMETRY_SHADER:
					strShaderType = "geometry";
					break;
				case GL_FRAGMENT_SHADER:
					strShaderType = "fragment";
					break;
				}

			fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
			delete[] strInfoLog;
		}
	return shader;
}

GLuint Shader::CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		{
			glAttachShader(program, shaderList[iLoop]);
		}

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if ( status == GL_FALSE )
		{
			GLint infoLogLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
			fprintf(stderr, "Linker failure: %s\n", strInfoLog);
			delete[] strInfoLog;
		}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		{
			glDetachShader(program, shaderList[iLoop]);
		}

	return program;
	/*
	try
	{
		GLuint prog = glLinkProgram(shaderList);
		std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
		return prog;
	}
	catch(std::exception &e)
	{
		std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
		fprintf(stderr, "%s\n", e.what());
		throw;
	}
	*/
}

void Shader::Use(bool var)
{
	if(var && !used)
		glUseProgram(program);

	if(!var && used)
		glUseProgram(0);

	used = var;
}

void Shader::SetTime(float var)
{
	glUniform1f(time, var);
}

void Shader::SetWidth(float var)
{
	glUniform1f(maskWidth, var);
}

void Shader::SetHeight(float var)
{
	glUniform1f(maskHeight, var);
}
GLenum Shader::GetPositionAttrib()
{
	return positionAttrib;
}

GLuint Shader::GetColorAttrib()
{
	return colorAttrib;
}

GLuint Shader::GetZFar()
{
	return cameraToClipMatrixUnif;
}

GLuint Shader::GetModelToCameraMatrixUnif()
{
	return modelToCameraMatrixUnif;
}

glm::mat4 Shader::GetCameraToClipMx()
{
	return cameraToClipMatrix;
}

const char* Shader::ReadFile(const char *file)
{
	FILE *fp;
	char *content = NULL;
	int count=0;

	if (file != NULL)
		{
			fp = fopen(file,"rt");

			if (fp != NULL)
				{

					fseek(fp, 0, SEEK_END);
					count = ftell(fp);
					rewind(fp);

					if (count > 0)
						{
							content = new char[count+1];
							count = fread(content,sizeof(char),count,fp);
							content[count] = '\0';
						}
					fclose(fp);
				}
		}
	return content;
}
