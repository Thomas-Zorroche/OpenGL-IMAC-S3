#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>


namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

	ShapeVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 texpos)
		: position(pos), normal(normal), texCoords(texpos) {}
	ShapeVertex(){}
};

}
