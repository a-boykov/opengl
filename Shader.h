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

typedef unsigned int GLenum;

class Shader
{
public:
    Shader();
    ~Shader();

    void InitializeProgram(const std::string &strVertexShader, const std::string &strFragmentShader);
    void InitializeProgramWithArray(const std::string &strVertexShader, const std::string &strFragmentShader);
    void InitializeProgram(const std::string &strVertexShader, const std::string &strFragmentShader, bool special);

    GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
    GLuint CreateProgram(const std::vector<GLuint> &shaderList);

    void Use(bool var);
    void SetTime(float var);
    void SetWidth(float var);
    void SetHeight(float var);
    void SetPosition(int x, int y);
    const char* ReadFile(const char *file);

private:
    GLenum program;
    GLenum time;
    GLenum maskWidth;
    GLenum maskHeight;
    GLenum position;
    bool used;
};

#endif // SHADER_H
