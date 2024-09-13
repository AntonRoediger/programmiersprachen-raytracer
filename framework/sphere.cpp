#include "sphere.hpp"
#include "hitpoint.hpp"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <numbers>
#include <cmath>
#include <string>

Sphere::Sphere(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, glm::vec3 const& center_parameter, double radius_parameter) : Shape::Shape{ name_parameter, material_parameter }, center_{center_parameter}, radius_(radius_parameter)
{
	//std::cout << "Sphere constructor called\n";
}

Sphere::~Sphere()
{
	//std::cout << "Sphere destructor called\n";
}

double Sphere::area() const {
	return 4 * std::numbers::pi * radius_ * radius_;
}

double Sphere::volume() const {
	return 4.0 / 3.0 * std::numbers::pi * radius_ * radius_ * radius_;
}

std::ostream& Sphere::print(std::ostream& os) const
{
	Shape::print(os);
	return os << "center: (" << center_.x << ", " << center_.y << ", " << center_.z << ')' << " radius: " << radius_ << '\n';
}

HitPoint Sphere::intersect(Ray const& ray_) const
{
	Ray transformed_ray_ = transform_ray(world_transformation_inv, ray_);

	float intersection_distance_parameter = 0;
	glm::vec3 ray_direction = glm::normalize(transformed_ray_.direction);
	bool did_intersect_parameter = glm::intersectRaySphere(transformed_ray_.origin, ray_direction, center_, radius_ * radius_, intersection_distance_parameter); // outside so intersection_distance_parameter is updated
	glm::vec3 position{ transformed_ray_.origin + intersection_distance_parameter * ray_direction };

	glm::vec3 surface_normal = get_surface_normal(position);
	Ray retransformed_ray_{ transform_ray(world_transformation_, Ray{ position + 0.0001f * surface_normal, ray_direction }) };
	Ray transformed_surface_normal{ transform_ray(glm::transpose(world_transformation_inv), Ray{{0, 0, 0}, surface_normal}) };
	
	return HitPoint{ did_intersect_parameter, intersection_distance_parameter, Shape::name_, Shape::material_, retransformed_ray_.origin, retransformed_ray_.direction, transformed_surface_normal.direction };
}

glm::vec3 Sphere::get_center() const
{
	return center_;
}

glm::vec3 Sphere::get_surface_normal(glm::vec3 const& hit_position_) const
{
	//if shadow acne on small spheres, look here
	return glm::normalize(hit_position_ - center_);
}

std::ostream& operator<<(std::ostream& os, Sphere const& s)
{
	return s.print(os);
}