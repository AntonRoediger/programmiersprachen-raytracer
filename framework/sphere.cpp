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
	std::cout << "Sphere constructor called\n";
}

Sphere::~Sphere()
{
	std::cout << "Sphere destructor called\n";
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
	float intersection_distance_parameter = 0;
	glm::vec3 ray_direction = glm::normalize(ray_.direction);
	bool did_intersect_parameter = glm::intersectRaySphere(ray_.origin, ray_direction, center_, radius_ * radius_, intersection_distance_parameter); // outside so intersection_distance_parameter is updated
	glm::vec3 position{ ray_.origin + intersection_distance_parameter * ray_direction };
	return HitPoint{ did_intersect_parameter, intersection_distance_parameter, Shape::name_, Shape::material_, position, ray_direction, get_surface_normal(position)};
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