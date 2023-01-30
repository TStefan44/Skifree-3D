#include "../header/fir.h"

using namespace tema3;

Fir::Fir(glm::vec3 pos, float plane_angle) : Object(pos), plane_angle(plane_angle) {
	// Basic variables
	body_m_offset = glm::vec3(0, 3.f, 0);
	body_u_offset = glm::vec3(0, 4.f, 0);
	trunk_offset = glm::vec3(0, 1.5f, 0);
	trunk_size = glm::vec3(0.5f, 3, 0.5f);
	body_middle_size = glm::vec3(1.2f, 1.2f, 1.2f);
	body_upper_size = glm::vec3(0.85f, 0.85f, 0.85f);

	calculateMatrixComp();

	// Bonding Box varaibles set
	minAABB = glm::vec3(-0.20f, 0.f, -0.20f);
	maxAABB = glm::vec3(0.20f, 3.f, 0.20f);

	minAABB = RotateVectorOX(plane_angle, minAABB);
	maxAABB = RotateVectorOX(plane_angle, maxAABB);

	// Set fir light
	glm::vec3 rand_color_light = RandomRGB(100, 256);
	light = light_source(PUNCTIFORM, pos + glm::vec3(0, 0.5f, 0), glm::vec3(1), rand_color_light, 0, 1.0f, 0.90f, 0.3f);
	material_ka = 0.6f;
	color_ke = rand_color_light * 0.5f;
}

void Fir::calculateMatrixComp() {
	{
		// Trunk
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, trunk_offset);
		modelMatrix = glm::scale(modelMatrix, trunk_size);
		matrixComp["fir_trunk"] = modelMatrix;
	}

	{
		// Body middle
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, body_m_offset);
		modelMatrix = glm::scale(modelMatrix, body_middle_size);
		matrixComp["fir_body_middle"] = modelMatrix;
	}

	{
		// Body upper
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, body_u_offset);
		modelMatrix = glm::scale(modelMatrix, body_upper_size);
		matrixComp["fir_body_upper"] = modelMatrix;
	}
}

Fir::Fir() : Fir(glm::vec3(0, 0, 0), 0) {}