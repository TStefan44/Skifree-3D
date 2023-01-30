#pragma once
#include "object.h"

namespace tema3 {
	class Fir : public Object{
	public:
		Fir();
		Fir(glm::vec3 pos, float plane_angle);

	private:
		void calculateMatrixComp() override;

	protected:
		glm::vec3 body_m_offset;
		glm::vec3 body_u_offset;
		glm::vec3 trunk_offset;
		glm::vec3 trunk_size;
		glm::vec3 body_middle_size;
		glm::vec3 body_upper_size;
		float plane_angle;
	};
}