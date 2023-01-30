#include "../header/object.h"

using namespace tema3;

Object::Object(glm::vec3 pos) : pos(pos) {
	material_ka = 0.75f;
	color_ke = glm::vec3(0);
}

Object::Object() : Object(glm::vec3(0, 0, 0)) {}

void Object::update(float deltaTimeSeconds) {}

void Object::update(glm::vec3 pos, float deltaTimeSeconds) {}

void Object::calculateMatrixComp() {}

glm::vec3 Object::RotateVectorOX(float angle, glm::vec3 vec) {
	glm::mat4 rotateWorld = glm::rotate(glm::mat4(1.0f), angle, glm::vec3_right);
	glm::vec4 rotateVec = rotateWorld * glm::vec4(vec, 1.0f);

	return glm::vec3(rotateVec);
}

bool Object::ColisionAABB(Object* obj) {
	glm::vec3 obj_minAABB = obj->getMinAABB() + obj->getPos();
	glm::vec3 obj_maxAABB = obj->getMaxAABB() + obj->getPos();

	glm::vec3 minAABB = this->minAABB + pos;
	glm::vec3 maxAABB = this->maxAABB + pos;

	return (
		minAABB.x <= obj_maxAABB.x &&
		maxAABB.x >= obj_minAABB.x &&
		minAABB.y <= obj_maxAABB.y &&
		maxAABB.y >= obj_minAABB.y &&
		minAABB.z <= obj_maxAABB.z &&
		maxAABB.z >= obj_minAABB.z
		);
}

void Object::setPos(glm::vec3 pos) {
	this->pos = pos;
}

void Object::setLight(light_source light) {
	this->light = light;
}

float Object::getMaterialKa() {
	return material_ka;
}

glm::vec3 Object::getColorKe() {
	return color_ke;
}

glm::vec3 Object::getPos() {
	return pos;
}

std::unordered_map<std::string, glm::mat4> Object::getMatrixComp() {
	return matrixComp;
}

glm::vec3 Object::getMaxAABB() {
	return maxAABB;
}

glm::vec3 Object::getMinAABB() {
	return minAABB;
}

light_source Object::getLight() {
	return light;
}