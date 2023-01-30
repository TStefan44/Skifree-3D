#pragma once
#include <string>
#include <unordered_map>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "light.h"

namespace tema3 {
	class Object {
	public:
		Object();
		Object(glm::vec3 pos);

		virtual void update(float deltaTimeSeconds);
		virtual void update(glm::vec3 pos, float deltaTimeSeconds);

		bool ColisionAABB(Object* obj);

		// Setters and Getters
		std::unordered_map<std::string, glm::mat4> getMatrixComp();
		void setPos(glm::vec3 pos);
		void setLight(light_source light);

		float getMaterialKa();
		light_source getLight();
		glm::vec3 getColorKe();
		glm::vec3 getPos();
		glm::vec3 getMinAABB();
		glm::vec3 getMaxAABB();

	protected:
		virtual void calculateMatrixComp();

		glm::vec3 RotateVectorOX(float angle, glm::vec3 vec);

	protected:
		glm::vec3 pos;
		std::unordered_map<std::string, glm::mat4> matrixComp;
		glm::vec3 minAABB;
		glm::vec3 maxAABB;

		light_source light;
		glm::vec3 color_ke;
		float material_ka;
	};
}