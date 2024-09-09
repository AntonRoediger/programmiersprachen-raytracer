#include "ray.hpp"


Ray transform_ray(glm::mat4 const& mat, Ray const& ray)
{
	//point transformation
	glm::vec4 point_trans{ ray.origin, 1 };
	point_trans = mat * point_trans;
	//direction transformation
	glm::vec4 direct_trans{ ray.direction, 0 };
	direct_trans = mat * direct_trans;
	Ray ray_transed{ glm::vec3{point_trans[0], point_trans[1], point_trans[2]}, glm::vec3{direct_trans[0], direct_trans[1], direct_trans[2]} };

	return ray_transed;
}