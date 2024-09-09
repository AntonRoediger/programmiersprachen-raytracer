#include "composite.hpp"

Composite::Composite(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, std::map<std::string, std::shared_ptr<Shape>> const& children_parameter) : Shape::Shape{name_parameter, material_parameter}, children_(children_parameter)
{
	std::cout << "composite constructor called\n";
}

Composite::~Composite()
{
	std::cout << "composite destructor called\n";
}

double Composite::area() const
{
	double area = 0;
	for (auto const& [name, shape] : children_)
	{
		area += shape->area();
	}
	return area;
}

double Composite::volume() const
{
	double volume = 0;
	for (auto const& [name, shape] : children_)
	{
		volume += shape->volume();
	}
	return volume;
}

HitPoint Composite::intersect(Ray const& ray_) const
{
	HitPoint min_distance_hitpoint{ false, std::numeric_limits<float>::max() };

	for (auto const& [name, shape] : children_)
	{
		HitPoint hitpoint{shape->intersect(ray_)};
		if (hitpoint.did_intersect_ && hitpoint.distance_ < min_distance_hitpoint.distance_)
		{
			min_distance_hitpoint = hitpoint;
		}
	}
	return min_distance_hitpoint;
}

std::ostream& Composite::print(std::ostream& os) const
{
	Shape::print(os);
	for (auto const& [name, shape] : children_)
	{
		shape->print(os);
	}
	return os;
}

glm::vec3 Composite::get_center() const
{
	return { 0, 0, 0 };
}

glm::vec3 Composite::get_surface_normal(glm::vec3 const& hit_position_) const
{
	//I don't like this, but this is a placeholder and shouldn't be called
	//this just ensures that you can create instances of composites
	return { 0, 0, 0 };
}

std::ostream& operator<<(std::ostream& os, Composite const& c)
{
	return c.print(os);
}