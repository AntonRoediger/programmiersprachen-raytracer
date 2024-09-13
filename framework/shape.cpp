#include "shape.hpp"
#include "color.hpp"
#include <string>
#include <glm/vec3.hpp>

Shape::Shape(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter) : name_(name_parameter), material_(material_parameter)
{
	//std::cout << "Shape constructor called\n";
}

Shape::~Shape()
{
	//std::cout << "Shape destructor called\n";
}

std::ostream& Shape::print(std::ostream& os) const
{
	return os << "name: " << name_ << " material: " << *material_ << '\n';
}

std::ostream& operator<<(std::ostream& os, Shape const& s)
{
	return s.print(os);
}