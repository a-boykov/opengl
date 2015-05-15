#include "hierarchy.h"

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))
#define STANDARD_ANGLE_INCREMENT 11.25f
#define SMALL_ANGLE_INCREMENT 9.0f

Hierarchy::Hierarchy(const float &pVertexData, const GLshort &pIndexData, GLuint &pVAO )
	:posBase(glm::vec3(3.0f, -5.0f, -40.0f))
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
	, m_pVertexData(pVertexData)
	, m_pIndexData(pIndexData)
	, m_pVAO(pVAO)

{
}

Hierarchy::~Hierarchy()
{

}

void Hierarchy::Draw()
{
	MatrixStack modelToCameraStack;

	shader1.Use(true);
	glBindVertexArray(*m_pVAO);

	modelToCameraStack.Translate(posBase);
	modelToCameraStack.RotateY(angBase);

	//Draw left base.
	{
		modelToCameraStack.Push();
		modelToCameraStack.Translate(posBaseLeft);
		modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();
	}

	//Draw right base.
	{
		modelToCameraStack.Push();
		modelToCameraStack.Translate(posBaseRight);
		modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();
	}

	//Draw main arm.
	DrawUpperArm(modelToCameraStack);

	glBindVertexArray(0);
	shader1.Use(false);
}

void Hierarchy::AdjBase(bool bIncrement)
{
	angBase += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
	angBase = fmodf(angBase, 360.0f);
}

void Hierarchy::AdjUpperArm(bool bIncrement)
{
	angUpperArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
	angUpperArm = Clamp(angUpperArm, -90.0f, 0.0f);
}

void Hierarchy::AdjLowerArm(bool bIncrement)
{
	angLowerArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
	angLowerArm = Clamp(angLowerArm, 0.0f, 146.25f);
}

void Hierarchy::AdjWristPitch(bool bIncrement)
{
	angWristPitch += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
	angWristPitch = Clamp(angWristPitch, 0.0f, 90.0f);
}

void Hierarchy::AdjWristRoll(bool bIncrement)
{
	angWristRoll += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
	angWristRoll = fmodf(angWristRoll, 360.0f);
}

void Hierarchy::AdjFingerOpen(bool bIncrement)
{
	angFingerOpen += bIncrement ? SMALL_ANGLE_INCREMENT : -SMALL_ANGLE_INCREMENT;
	angFingerOpen = Clamp(angFingerOpen, 9.0f, 180.0f);
}

void Hierarchy::WritePose()
{
	printf("angBase:\t%f\n", angBase);
	printf("angUpperArm:\t%f\n", angUpperArm);
	printf("angLowerArm:\t%f\n", angLowerArm);
	printf("angWristPitch:\t%f\n", angWristPitch);
	printf("angWristRoll:\t%f\n", angWristRoll);
	printf("angFingerOpen:\t%f\n", angFingerOpen);
	printf("\n");
}

void Hierarchy::SetShader(Shader &pShader)
{
	shader1 = pShader;
}

void Hierarchy::DrawFingers(MatrixStack &modelToCameraStack)
{
	//Draw left finger
	modelToCameraStack.Push();
	modelToCameraStack.Translate(posLeftFinger);
	modelToCameraStack.RotateY(angFingerOpen);

	modelToCameraStack.Push();
	modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
	modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
	glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));

	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
	modelToCameraStack.Pop();

	{
		//Draw left lower finger
		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
		modelToCameraStack.RotateY(-angLowerFinger);

		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
		modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));

		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();

		modelToCameraStack.Pop();
	}

	modelToCameraStack.Pop();

	//Draw right finger
	modelToCameraStack.Push();
	modelToCameraStack.Translate(posRightFinger);
	modelToCameraStack.RotateY(-angFingerOpen);

	modelToCameraStack.Pop();
	modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
	modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
	glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));

	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
	modelToCameraStack.Pop();

	{
		//Draw right lower finger
		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
		modelToCameraStack.RotateY(angLowerFinger);

		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
		modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));

		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();

		modelToCameraStack.Pop();
	}

	modelToCameraStack.Pop();
}

void Hierarchy::DrawWrist(MatrixStack &modelToCameraStack)
{
	modelToCameraStack.Push();
	modelToCameraStack.Translate(posWrist);
	modelToCameraStack.RotateZ(angWristRoll);
	modelToCameraStack.RotateX(angWristPitch);

	modelToCameraStack.Push();
	modelToCameraStack.Scale(glm::vec3(widthWrist / 2.0f, widthWrist / 2.0f, lenWrist / 2.0f));
	glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));

	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
	modelToCameraStack.Pop();

	DrawFingers(modelToCameraStack);

	modelToCameraStack.Pop();
}

void Hierarchy::DrawLowerArm(MatrixStack &modelToCameraStack)
{
	modelToCameraStack.Push();
	modelToCameraStack.Translate(posLowerArm);
	modelToCameraStack.RotateX(angLowerArm);

	modelToCameraStack.Push();
	modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenLowerArm / 2.0f));
	modelToCameraStack.Scale(glm::vec3(widthLowerArm / 2.0f, widthLowerArm / 2.0f, lenLowerArm / 2.0f));
	glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));

	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
	modelToCameraStack.Pop();

	DrawWrist(modelToCameraStack);

	modelToCameraStack.Pop();
}

void Hierarchy::DrawUpperArm(MatrixStack &modelToCameraStack)
{
	modelToCameraStack.Push();
	modelToCameraStack.RotateX(angUpperArm);

	{
		modelToCameraStack.Push();
		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, (sizeUpperArm / 2.0f) - 1.0f));
		modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, sizeUpperArm / 2.0f));
		glUniformMatrix4fv(shader1.GetModelToCameraMatrixUnif(), 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));

		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(*m_pIndexData), GL_UNSIGNED_SHORT, 0);
		modelToCameraStack.Pop();
	}

	DrawLowerArm(modelToCameraStack);

	modelToCameraStack.Pop();
}


