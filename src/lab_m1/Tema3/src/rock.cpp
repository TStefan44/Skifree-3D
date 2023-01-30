#include "../header/rock.h"

using namespace tema3;

Rock::Rock(glm::vec3 pos, float plane_angle) : Object(pos), plane_angle(plane_angle) {
	// Basic variables
	rock1_offset = glm::vec3(0.f, 0.f, 0.f);
	rock2_offset = glm::vec3(-0.35f, 0.f, 0.f);
	rock3_offset = glm::vec3(0.35f, 0.f, 0.25f);
	rock4_offset = glm::vec3(0.f, 0.30f, 0.f);

	calculateMatrixComp();

	// Bonding Box varaibles set
	minAABB = glm::vec3(-0.35f, -0.5f, -0.35f);
	maxAABB = glm::vec3(0.35f, 0.5f, 0.35f);

	minAABB = RotateVectorOX(plane_angle, minAABB);
	maxAABB = RotateVectorOX(plane_angle, maxAABB);

	material_ka = 0.5f;
}

void Rock::calculateMatrixComp() {
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, rock1_offset);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.8f));
		matrixComp["rock_1"] = modelMatrix;
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, rock2_offset);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.f), glm::vec3_up);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.4f, 0.3f));
		matrixComp["rock_2"] = modelMatrix;
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, rock3_offset);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.3f, 0.5f));
		modelMatrix = glm::rotate(modelMatrix, (float)glm::radians(60.f), glm::vec3_up);
		matrixComp["rock_3"] = modelMatrix;
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, rock4_offset);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.2f, 0.4f));
		matrixComp["rock_4"] = modelMatrix;
	}
}

Rock::Rock() : Rock(glm::vec3(0, 0, 0), 0) {}