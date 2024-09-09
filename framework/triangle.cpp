/*#include "triangle.hpp"
#include "hitpoint.hpp"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <numbers>
#include <cmath>
#include <string>

HitPoint Triangle::intersect(Ray const& ray_) const
{
	float intersection_distance_parameter = 0;
	glm::vec3 ray_direction = glm::normalize(ray_.direction);
	bool did_intersect_parameter = glm::intersectRayTriangle(ray_.origin, ray_direction, v0, v1, v2, intersection_distance_parameter); // outside so intersection_distance_parameter is updated
	return HitPoint{ did_intersect_parameter, intersection_distance_parameter, Shape::name_, Shape::material_, (ray_.origin + intersection_distance_parameter * ray_direction), ray_direction };
}

glm::vec3 Triangle::get_surface_normal(HitPoint const& hitpoint) const
{
	return glm::normalize(hitpoint.position_.x * v0.x + hitpoint.position_.y * v0.y + hitpoint.position_.z * v0.z);
}*/