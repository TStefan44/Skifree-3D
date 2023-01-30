#include "../header/player.h"

using namespace tema3;

Player::Player(glm::vec3 pos, float plane_angle) : Object(pos), plane_angle(plane_angle) {
	// Basic variables set
	head_offset = glm::vec3(0, 0.1f, 0);
	sky_offset = glm::vec3(0.35f, -0.5f, 0);
	sky_size = glm::vec3(0.3f, 0.2f, 3.f);
	player_speed = 8.f;
	rotSpeed = 2.5f;

	calculateMatrixComp();

	// Bonding Box varaibles set
	minAABB = glm::vec3(-0.35f, 0.f, -0.35f);
	maxAABB = glm::vec3(0.35f, 1.1f, 0.35f);

	minAABB = RotateVectorOX(plane_angle, minAABB);
	maxAABB = RotateVectorOX(plane_angle, maxAABB);

	// Set player light
	light = light_source(PUNCTIFORM, pos, glm::vec3(1), glm::vec3(1, 0, 0), 0, 0.90f, 0.90f, 0.90f);
	material_ka = 0.75f;
	color_ke = glm::vec3(0.2f, 0, 0);
}

Player::Player() : Player(glm::vec3(0, 0, 0), 0.f) {}

void Player::calculateMatrixComp() { 

	{
		// Head
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix *= glm::toMat4(quatRot);
		//modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, head_offset);
		matrixComp["player_head"] = modelMatrix;
	}

	{
		// Ski Left
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix *= glm::toMat4(quatRot);
		//modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, sky_offset * glm::vec3(-1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, sky_size);
		matrixComp["player_ski_left"] = modelMatrix;
	}

	{
		// Ski Right
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix *= glm::toMat4(quatRot);
		//modelMatrix = glm::rotate(modelMatrix, plane_angle, glm::vec3_right);
		modelMatrix = glm::translate(modelMatrix, sky_offset);
		modelMatrix = glm::scale(modelMatrix, sky_size);
		matrixComp["player_ski_right"] = modelMatrix;
	}
}

void Player::update(glm::vec3 dir, float deltaTimeSeconds) {
	pos += dir * player_speed * deltaTimeSeconds;
	quatRot = quadRotation(dir, rotSpeed * deltaTimeSeconds);

	light.light_position = pos;

	calculateMatrixComp();
	
}

glm::quat Player::quadRotation(glm::vec3 dir, float angularSpeed) {
	glm::quat targetRot = glm::quatLookAt(dir, local_up);
	float rotSlerpFactor = glm::clamp(angularSpeed, 0.f, 1.f);
	return glm::slerp(quatRot, targetRot, rotSlerpFactor);
}

void Player::setQuatRotation(glm::vec3 forward, glm::vec3 up) {
	quatRot = glm::quat(up, forward);
}

void Player::setLocalUp(glm::vec3 local_up) {
	this->local_up = local_up;
}

float Player::getPlayerSpeed() {
	return player_speed;
}