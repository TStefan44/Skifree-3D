#include "../header/plane.h"

using namespace tema3;

Plane::Plane(glm::vec3 pos, float plane_angle) : Object(pos), plane_angle(plane_angle) {
	plane_offset = glm::vec3(0.f, -0.6f, 0.f);
	plane_size = glm::vec3(2, 2, 2);

	calculateMatrixComp();

	material_ka = 0.30f;
}

Plane::Plane() : Plane(glm::vec3(0, 0, 0), 0) {}

void Plane::calculateMatrixComp() {
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, plane_offset);
		modelMatrix = glm::scale(modelMatrix, plane_size);
		matrixComp["plane"] = modelMatrix;
	}
}

void Plane::update(glm::vec3 pos, float deltaTimeSeconds) {
	this->pos = pos;
	calculateMatrixComp();
}