#include "matrixstack.h"

MatrixStack::MatrixStack()
	:m_currMat(1.0f)
{

}

MatrixStack::~MatrixStack()
{

}

float MatrixStack::DegToRad(float fAngDeg)
{
	const float fDegToRad = 3.14159f * 2.0f / 360.0f;
	return fAngDeg * fDegToRad;
}

float MatrixStack::Clamp(float fValue, float fMinValue, float fMaxValue)
{
	if(fValue < fMinValue)
		return fMinValue;

	if(fValue > fMaxValue)
		return fMaxValue;

	return fValue;
}

glm::mat3 MatrixStack::GLRotateX(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[1].y = fCos; theMat[2].y = -fSin;
	theMat[1].z = fSin; theMat[2].z = fCos;

	return theMat;
}

glm::mat3 MatrixStack::GLRotateY(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[2].x = fSin;
	theMat[0].z = -fSin; theMat[2].z = fCos;

	return theMat;
}

glm::mat3 MatrixStack::GLRotateZ(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[1].x = -fSin;
	theMat[0].y = fSin; theMat[1].y = fCos;

	return theMat;
}

void MatrixStack::RotateX(float fAngDeg)
{
	m_currMat = m_currMat * glm::mat4(GLRotateX(fAngDeg));
}

void MatrixStack::RotateY(float fAngDeg)
{
	m_currMat = m_currMat * glm::mat4(GLRotateY(fAngDeg));
}

void MatrixStack::RotateZ(float fAngDeg)
{
	m_currMat = m_currMat * glm::mat4(GLRotateZ(fAngDeg));
}


void MatrixStack::Scale(const glm::vec3 &scaleVec)
{
	glm::mat4 scaleMat(1.0f);
	scaleMat[0].x = scaleVec.x;
	scaleMat[1].y = scaleVec.y;
	scaleMat[2].z = scaleVec.z;

	m_currMat = m_currMat * scaleMat;
}

void MatrixStack::Translate(const glm::vec3 &offsetVec)
{
	glm::mat4 translateMat(1.0f);
	translateMat[3] = glm::vec4(offsetVec, 1.0f);

	m_currMat = m_currMat * translateMat;
}

void MatrixStack::Push()
{
	m_matrices.push(m_currMat);
}

void MatrixStack::Pop()
{
	m_currMat = m_matrices.top();
	m_matrices.pop();
}
