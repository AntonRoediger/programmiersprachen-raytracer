#ifndef BOX_HPP
#define BOX_HPP

#include "shape.hpp"
#include "color.hpp"
#include "hitpoint.hpp"
#include "material.hpp"
//#include <memory>
#include <glm/vec3.hpp>

class Box : public Shape {
public:
	Box(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, glm::vec3 const& maximum_parameter, glm::vec3 const& minimum_parameter);
	~Box();
	double area() const override;
	double volume() const override;
	std::ostream& print(std::ostream& os) const override;
	HitPoint intersect(Ray const& ray_) const override;
	glm::vec3 get_surface_normal(HitPoint const& hitpoint) const override;
	glm::vec3 get_center() const override;
private:
	glm::vec3 maximum_;
	glm::vec3 minimum_;
};

std::ostream& operator<<(std::ostream& os, Box const& b);

#endif