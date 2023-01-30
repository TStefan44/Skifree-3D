#include "../header/lantern.h"

using namespace tema3;

Lantern::Lantern(glm::vec3 pos, glm::vec3 local_up, float plane_angle) : Object(pos), plane_angle(plane_angle) {
	// Basic varaibles
	pillar_1_scale = glm::vec3(0.2f, 3.5f, 0.2f);
	pillar_1_offset = glm::vec3(0.f, 1.75f, 0.f);

	pillar_2_scale = glm::vec3(2.f, 0.2f, 0.2f);
	pillar_2_offset = glm::vec3(0.f, 3.6f, 0.f);

	calculateMatrixComp();

	// Bonding Box varaibles set
	minAABB = glm::vec3(-0.1f, -1.75f, -0.1f);
	maxAABB = glm::vec3(0.1f, 1.75f, 0.1f);

	minAABB = RotateVectorOX(plane_angle, minAABB);
	maxAABB = RotateVectorOX(plane_angle, maxAABB);

	// Set lantern light

	glm::vec3 rand_color_light = RandomRGB(100, 256);
	light = light_source(SPOTLIGHT, pos + local_up * 3.f + glm::vec3(-2.5f, 0, 0), -local_up, rand_color_light, 60, 0.95f, 0.95f, 0.5f);
	spotlight2 = light_source(SPOTLIGHT, pos + local_up * 3.f + glm::vec3(+2.5f, 0, 0), -local_up, rand_color_light, 60, 0.95f, 0.95f, 0.5f);
	material_ka = 0.7f;
	color_ke = rand_color_light * 0.25f;
}

Lantern::Lantern() : Lantern(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0) {}

void Lantern::calculateMatrixComp() {
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, pillar_1_offset);
		modelMatrix = glm::scale(modelMatrix, pillar_1_scale);
		matrixComp["pillar_1"] = modelMatrix;
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, pillar_2_offset);
		modelMatrix = glm::scale(modelMatrix, pillar_2_scale);
		matrixComp["pillar_2"] = modelMatrix;
	}
}

light_source Lantern::getSpotlight1() {
	return getLight();
}

light_source Lantern::getSpotlight2() {
	return spotlight2;
}
