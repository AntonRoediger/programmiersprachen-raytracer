/*#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"
#include "hitpoint.hpp"
#include "ray.hpp"
#include "color.hpp"
#include <glm/vec3.hpp>

class Triangle : public Shape {
public:
    Cylinder(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, glm::vec3 const& v0, glm::vec3 const& v1, glm::vec3 const& v2);
	HitPoint intersect(Ray const& ray_) const override;
	glm::vec3 get_surface_normal(HitPoint const& hitpoint) const override;
    glm::vec3 get_center() const override;
    double area() const override;
    double volume() const override;
private:
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
};

#endif
*/