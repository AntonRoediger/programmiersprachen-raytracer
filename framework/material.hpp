#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"
#include <string>
#include <glm/vec3.hpp>

struct Material
{
	std::string name{ "placeholder" };
	Color ka{ 0, 0, 0 }; //0 <= ka <= 1.0
	Color kd{ 0, 0, 0 }; //0 <= kd <= 1.0
	Color ks{ 0, 0, 0 }; //0 <= ks <= 1.0
	float exponent{ 0 }; //10 <= m <= 810
	float reflectivity{ 1 }; //0 <= reflectivity <= 1.0, represents how much light is reflected
};

inline std::ostream& operator<<(std::ostream& os, Material const& material) // inline makes it so that the function can be defined within the header file
{
	os << "material name: " << material.name << " ka: " << "(" << material.ka.r << ", " << material.ka.g << ", " << material.ka.b << ") "
		<< "kd: " << "(" << material.kd.r << ", " << material.kd.g << ", " << material.kd.b << ") "
		<< "ks: " << "(" << material.ks.r << ", " << material.kd.g << ", " << material.kd.b << ")"
		<< " exponent: " << material.exponent;
	return os;
}
#endif