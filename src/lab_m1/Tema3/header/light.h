#pragma once
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace tema3 {

	enum light_type {DIRECTIONAL = 0, PUNCTIFORM = 1, SPOTLIGHT = 2};

	struct light_source {
		int  type;
		glm::vec3 light_position;
		glm::vec3 light_direction;
		glm::vec3 light_color;

		// Spotlight component
		float cut_off;

		// Difuze component
		float material_kd;

		// Specular component
		float material_ks;
		float material_shininess;

		light_source(light_type type, glm::vec3 light_position, glm::vec3 light_direction, glm::vec3 light_color,
					 float cut_off,
					 float material_kd, float material_ks, float material_shininess) :

			light_position(light_position), light_direction(light_direction), light_color(light_color),
			cut_off(cut_off), 
			material_kd(material_kd), material_ks(material_ks), material_shininess(material_shininess){
			
			switch (type) {
				case DIRECTIONAL: this->type = 0; break;
				case PUNCTIFORM: this->type = 1; break;
				case SPOTLIGHT: this->type = 2;
			}
		}

		light_source() {}
	};

	 glm::vec3 RandomRGB(int min, int max);
	 glm::vec3 RandomRGB();
}