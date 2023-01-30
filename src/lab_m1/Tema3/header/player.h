#pragma once
#include "object.h"

namespace tema3 {
	class Player : public Object {
	public:
		Player();
		Player(glm::vec3 pos, float plane_angle);

		void update(glm::vec3 dir, float deltaTimeSeconds) override;
		void setQuatRotation(glm::vec3 forward, glm::vec3 up);
		void setLocalUp(glm::vec3 local_up);
		float getPlayerSpeed();

	private:
		void calculateMatrixComp() override;
		glm::quat quadRotation(glm::vec3 dir, float angularSpeed);

	private:
		glm::vec3 head_offset;
		glm::vec3 sky_offset;
		glm::vec3 sky_size;
		glm::vec3 local_up;
		glm::quat quatRot;
		float plane_angle;
		float player_speed;
		float rotSpeed;
	};
}