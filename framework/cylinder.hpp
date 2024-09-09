#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "shape.hpp"
#include "hitpoint.hpp"
#include "ray.hpp"
#include "color.hpp"
#include <glm/vec3.hpp>

class Cylinder : public Shape {
public:
    Cylinder(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, glm::vec3 const& top, glm::vec3 const& bottom, float radius);
    HitPoint intersect(Ray const& ray_) const override;
    glm::vec3 get_surface_normal(HitPoint const& hitpoint) const override;
    glm::vec3 get_center() const override;
    double area() const override;
    double volume() const override;
private:
    glm::vec3 top;
    glm::vec3 bottom;
    float radius;

};
#endif
