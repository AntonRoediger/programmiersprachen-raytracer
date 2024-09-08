#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include "shape.hpp"

#include <map>

class Composite : public Shape
{
public:
	Composite(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, std::map<std::string, std::shared_ptr<Shape>> const& children_parameter);
	~Composite();
	virtual double area() const override;
	virtual double volume() const override;
	std::ostream& print(std::ostream& os) const override;
	HitPoint intersect(Ray const& ray_) const override;
private:
	std::map<std::string, std::shared_ptr<Shape>> children_; //maybe a vector is better for this, depends if you ever search for a shape by name (maybe to prevent intersecting with itself, where surface normal doesn't help)
	glm::vec3 get_surface_normal(glm::vec3 const& hit_position_) const override;
};

std::ostream& operator<<(std::ostream& os, Composite const& c);

#endif
