#include "../header/light.h"

using namespace tema3;

glm::vec3 tema3::RandomRGB(int min, int max) {
	float r, g, b;

	if (max > 256) max = 256;
	if (min < 0) min = 0;

	int value = max - min;

	r = (float)(rand() % value) / 255;
	g = (float)(rand() % value) / 255;
	b = (float)(rand() % value) / 255;

	return glm::vec3(r, g, b);
}

glm::vec3 tema3::RandomRGB() {
	return RandomRGB(0, 256);
}