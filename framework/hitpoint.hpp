#ifndef HITPOINT_HPP
#define HITPOINT_HPP

#include "color.hpp"
#include "material.hpp"
#include <memory>
#include <string>
#include <glm/vec3.hpp>

struct HitPoint
{
	bool did_intersect_{ false };
	float distance_{ 0 };
	std::string object_name_{"placeholder"};
	std::shared_ptr<Material> object_material_{};
	glm::vec3 position_{ 0, 0, 0 };
	glm::vec3 direction_{ 0, 0, 0 };
	glm::vec3 surface_normal{ 0, 0, 0 };
};

#endif