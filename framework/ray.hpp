#ifndef RAY_HPP
#define RAY_HPP

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction = { 0.0f, 0.0f, -1.0f };

	Ray(glm::vec3 const& origin_parameter, glm::vec3 const& direction_parameter) : origin(origin_parameter), direction(glm::normalize(direction_parameter))
	{}

};

Ray transform_ray(glm::mat4 const& mat, Ray const& ray);

#endif