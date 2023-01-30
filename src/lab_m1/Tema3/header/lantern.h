#pragma once
#include "object.h"

namespace tema3 {
	class Lantern : public Object {
	public:
		Lantern();
		Lantern(glm::vec3 pos, glm::vec3 local_up, float plane_angle);

		light_source getSpotlight1();
		light_source getSpotlight2();

	private:
		void calculateMatrixComp() override;

	private:
		glm::vec3 pillar_1_offset;
		glm::vec3 pillar_1_scale;
		glm::vec3 pillar_2_offset;
		glm::vec3 pillar_2_scale;
		float plane_angle;

		light_source spotlight2;
	};
}