#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"
#include "hitpoint.hpp"
#include "ray.hpp"
#include "color.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape {
public:
	Sphere(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, glm::vec3 const& center_parameter, double radius_parameter);
	~Sphere();
	double area() const override;
	double volume() const override;
	std::ostream& print(std::ostream& os) const override;
	HitPoint intersect(Ray const& ray_) const override;
private:
	glm::vec3 center_;
	double radius_;
	glm::vec3 get_surface_normal(glm::vec3 const& hit_position_) const override;
};

std::ostream& operator<<(std::ostream& os, Sphere const& s);

#endif