#ifndef HIERARCHY_H
#define HIERARCHY_H
#include <stdio.h>

#include <matrixstack.h>
#include <Shader.h>

class Hierarchy : public MatrixStack
{
public:
	Hierarchy(const float &pVertexData, const GLshort &pIndexData, GLuint &pVAO );
	virtual ~Hierarchy();

	void Draw();
	void AdjBase(bool bIncrement);
	void AdjUpperArm(bool bIncrement);
	void AdjLowerArm(bool bIncrement);
	void AdjWristPitch(bool bIncrement);
	void AdjWristRoll(bool bIncrement);
	void AdjFingerOpen(bool bIncrement);
	void WritePose();

	void SetShader(Shader &pShader);

private:
	void DrawFingers(MatrixStack &modelToCameraStack);
	void DrawWrist(MatrixStack &modelToCameraStack);
	void DrawLowerArm(MatrixStack &modelToCameraStack);
	void DrawUpperArm(MatrixStack &modelToCameraStack);

	glm::vec3	posBase;
	float		angBase;

	glm::vec3	posBaseLeft, posBaseRight;
	float		scaleBaseZ;

	float		angUpperArm;
	float		sizeUpperArm;

	glm::vec3	posLowerArm;
	float		angLowerArm;
	float		lenLowerArm;
	float		widthLowerArm;

	glm::vec3	posWrist;
	float		angWristRoll;
	float		angWristPitch;
	float		lenWrist;
	float		widthWrist;

	glm::vec3	posLeftFinger, posRightFinger;
	float		angFingerOpen;
	float		lenFinger;
	float		widthFinger;
	float		angLowerFinger;

	Shader *shader1;
	const float *m_pVertexData;
	const GLshort *m_pIndexData;
	const GLuint *m_pVAO;
};

#endif // HIERARCHY_H
