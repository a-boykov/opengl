#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H
#include <glm/glm.hpp>
#include <stack>

class MatrixStack
{
public:
	MatrixStack();

	virtual ~MatrixStack();

	const glm::mat4 &Top()
	{
		return m_currMat;
	}
	float DegToRad(float fAngDeg);
	float Clamp(float fValue, float fMinValue, float fMaxValue);

	glm::mat3 GLRotateX(float fAngDeg);
	glm::mat3 GLRotateY(float fAngDeg);
	glm::mat3 GLRotateZ(float fAngDeg);

	void RotateX(float fAngDeg);
	void RotateY(float fAngDeg);
	void RotateZ(float fAngDeg);
	void Scale(const glm::vec3 &scaleVec);
	void Translate(const glm::vec3 &offsetVec);
	void Push();
	void Pop();

private:
	glm::mat4 m_currMat;
	std::stack<glm::mat4> m_matrices;
};

#endif // MATRIXSTACK_H
