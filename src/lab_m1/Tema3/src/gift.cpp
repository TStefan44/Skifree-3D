#include "../header/gift.h"

using namespace tema3;

Gift::Gift(glm::vec3 pos, float plane_angle) : Object(pos), plane_angle(plane_angle) {
	// Basic variables
	gift_offset = glm::vec3(0, 0.5f, 0);

	calculateMatrixComp();

	// Bonding Box varaibles set
	minAABB = glm::vec3(-0.5f, 0.0f, -0.5f);
	maxAABB = glm::vec3(0.5f, 1.f, 0.5f);

	minAABB = RotateVectorOX(plane_angle, minAABB);
	maxAABB = RotateVectorOX(plane_angle, maxAABB);

	// Set gift light
	glm::vec3 rand_color_light = RandomRGB(100, 256);
	light = light_source(PUNCTIFORM, pos, glm::vec3(1), rand_color_light, 0, 0.90f, 0.90f, 0.90f);
	material_ka = 0.5f;
	color_ke = rand_color_light * 0.8f;
}

void Gift::calculateMatrixComp() {
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, gift_offset);
		matrixComp["gift"] = modelMatrix;
	}
}

Gift::Gift() : Gift(glm::vec3(0, 0, 0), 0) {}