#pragma once
#include "object.h"

namespace tema3 {
	class Plane : public Object {
	public:
		Plane();
		Plane(glm::vec3 pos, float plane_angle);

		void update(glm::vec3 pos, float deltaTimeSeconds) override;

	private:
		void calculateMatrixComp() override;

	private:
		float plane_angle;
		glm::vec3 plane_offset;
		glm::vec3 plane_size;
	};
}