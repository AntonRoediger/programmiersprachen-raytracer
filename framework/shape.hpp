#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "color.hpp"
#include "ray.hpp"
#include "hitpoint.hpp"
#include "material.hpp"
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

class Shape {
public:
	Shape(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter);
	virtual ~Shape();
	virtual double area() const = 0;
	virtual double volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
	virtual HitPoint intersect(Ray const& ray_) const = 0;
	virtual glm::vec3 get_surface_normal(HitPoint const& hitpoint) const = 0;
	virtual glm::vec3 get_center() const = 0;
	glm::mat4 world_transformation_ = glm::mat4(1.0); // Dont work with 'protected' in sdf_reader
	glm::mat4 world_transformation_inv = glm::mat4(1.0);
protected:
	std::string name_;
	std::shared_ptr<Material> material_;

};

std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif