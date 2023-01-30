#pragma once
#include "object.h"

namespace tema3 {
	class Rock : public Object {
	public:
		Rock();
		Rock(glm::vec3 pos, float plane_angle);

	private:
		void calculateMatrixComp() override;

	private:
		glm::vec3 rock1_offset;
		glm::vec3 rock2_offset;
		glm::vec3 rock3_offset;
		glm::vec3 rock4_offset;
		float plane_angle;
	};
}