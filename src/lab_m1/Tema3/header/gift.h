#pragma once
#include "object.h"

namespace tema3 {
	class Gift : public Object {
	public:
		Gift();
		Gift(glm::vec3 pos, float plane_angle);

	private:
		void calculateMatrixComp() override;

	private:
		glm::vec3 gift_offset;
		glm::float32 plane_angle;
	};
}