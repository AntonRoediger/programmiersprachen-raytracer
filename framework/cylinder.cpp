#include "cylinder.hpp"
#include "hitpoint.hpp"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <numbers>
#include <cmath>
#include <string>

Cylinder::Cylinder(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, glm::vec3 const& top_parameter, glm::vec3 const& bottom_parameter, float radius_parameter) : Shape::Shape{ name_parameter, material_parameter }, top(top_parameter), bottom(bottom_parameter), radius(radius_parameter) { }

HitPoint Cylinder::intersect(Ray const& ray_) const
{
    glm::vec3 center = { (top.x + bottom.x) / 2, (top.y + bottom.y) / 2, (top.z + bottom.z) / 2 };
    float a = (ray_.direction.x * ray_.direction.x) + (ray_.direction.z * ray_.direction.z);
    float b = 2 * (ray_.direction.x * (ray_.origin.x - center.x) + ray_.direction.z * (ray_.origin.z - center.z));
    float c = (ray_.origin.x - center.x) * (ray_.origin.x - center.x) + (ray_.origin.z - center.z) * (ray_.origin.z - center.z) - (radius * radius);

    float discriminant = b * b - 4 * (a * c);
    if (fabs(discriminant) < 0.001) {
        return HitPoint{ false, 0, "", std::shared_ptr<Material>{}, glm::vec3{}, glm::vec3{} };
    }
    else if (discriminant < 0.0) {
        return HitPoint{ false, 0, "", std::shared_ptr<Material>{}, glm::vec3{}, glm::vec3{} };
    }

    float t1 = (-b - sqrt(discriminant)) / (2 * a);
    float t2 = (-b + sqrt(discriminant)) / (2 * a);
    float t;

    if (t1 > t2) {
        t = t2;
    }
    else {
        t = t1;
    }

    float r = ray_.origin.z + t * ray_.direction.z;
    if ((r > top.z) && (r < bottom.z) ) {
        return HitPoint{ true, t, Shape::name_, Shape::material_, (ray_.origin + t * ray_.direction), glm::normalize(ray_.direction) };
    }

    float top_plane = glm::dot(ray_.direction, glm::vec3{ 0, 0, top.z });
    if (abs(top_plane) > 0.0001f) // positive
    {
        float t1 = glm::dot(glm::vec3{ 0, 0, top.z }, top - ray_.origin) / top_plane;
        if (t1 >= 0) {
            if ((top.x + radius) > (ray_.origin.x + t1 * ray_.direction.x) > (top.x - radius) && (top.y + radius) > (ray_.origin.y + t1 * ray_.direction.y) > (top.y - radius)) {
                return HitPoint{ true, t, Shape::name_, Shape::material_, (ray_.origin + t1 * ray_.direction), glm::normalize(glm::normalize(glm::vec3{ top.x, top.y, top.z - radius })) };

            }
        }
    }

    float bottom_plane = glm::dot(ray_.direction, glm::vec3{ 0, 0, bottom.z });
    if (abs(bottom_plane) > 0.0001f) // positive
    {
        float t2 = glm::dot(glm::vec3{ 0, 0, bottom.z }, bottom - ray_.origin) / bottom_plane;
        if (t2 >= 0) {
            if ((bottom.x + radius) > (ray_.origin.x + t1 * ray_.direction.x) > (bottom.x - radius) && (bottom.y + radius) > (ray_.origin.y + t1 * ray_.direction.y) > (bottom.y - radius)) {
                return HitPoint{ true, t, Shape::name_, Shape::material_, (ray_.origin + t1 * ray_.direction), glm::normalize(glm::normalize(glm::vec3{ bottom.x, bottom.y, bottom.z - radius })) };
            }
        }
    }

    else {
        return HitPoint{ false, 0, "", std::shared_ptr<Material>{}, glm::vec3{}, glm::vec3{} };
    }
}

glm::vec3 Cylinder::get_surface_normal(HitPoint const& hitpoint) const
{
    if ( hitpoint.position_.z < top.z && hitpoint.position_.z > bottom.z ) {
        glm::vec3 center = { (top.x + bottom.x) / 2, (top.y + bottom.y) / 2, (top.z + bottom.z) / 2 };
        glm::vec3 n = { hitpoint.position_.x - center.x, hitpoint.position_.y - center.y, hitpoint.position_.z };
        glm::normalize(n);
        return n;
    }
    else if (hitpoint.position_.z == top.z) {
        return glm::normalize(glm::vec3{ hitpoint.position_.x, hitpoint.position_.y, hitpoint.position_.z + radius });
    }
    else {
        return glm::normalize(glm::vec3{ hitpoint.position_.x, hitpoint.position_.y, hitpoint.position_.z - radius });
    }
}

glm::vec3 Cylinder::get_center() const
{
    return glm::vec3();
}

double Cylinder::area() const
{
    return 0.0;
}

double Cylinder::volume() const
{
    return 0.0;
}


