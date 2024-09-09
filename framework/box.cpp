#include "box.hpp"
#include "color.hpp"
#include "hitpoint.hpp"
#include "material.hpp"
#include <glm/vec3.hpp>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <tuple>

Box::Box(std::string const& name_parameter, std::shared_ptr<Material> const& material_parameter, glm::vec3 const& maximum_parameter, glm::vec3 const& minimum_parameter) : Shape::Shape{ name_parameter, material_parameter }, maximum_(maximum_parameter), minimum_(minimum_parameter)
{
	std::cout << "Box constructor called\n";
}

Box::~Box()
{
	std::cout << "Box destructor called\n";
}

double Box::area() const {
	glm::vec3 diagonal_{ maximum_ - minimum_ };
	double a = std::abs(diagonal_.x);
	double b = std::abs(diagonal_.y);
	double c = std::abs(diagonal_.z);
	return 2 * (a * (b + c) + b * c);
}

double Box::volume() const {
	glm::vec3 diagonal_{ maximum_ - minimum_ };
	return std::abs(diagonal_.x * diagonal_.y * diagonal_.z);
}

std::ostream& Box::print(std::ostream& os) const
{
	Shape::print(os);
	return os << "maximum: (" << maximum_.x << ", " << maximum_.y << ", " << maximum_.z << ')' << " minimum: (" << minimum_.x << ", " << minimum_.y << ", " << minimum_.z << ')' << '\n';
}

std::ostream& operator<<(std::ostream& os, Box const& b)
{
	return b.print(os);
}

HitPoint Box::intersect(Ray const& ray_) const
{
	//std::cout << "ray before transformation = " << ray_.origin[0] << ", " << ray_.origin[1] << ", " << ray_.origin[2] 
	//	<< ", " << ray_.origin[0] << ", " << ray_.origin[1] << ", " << ray_.origin[2] << "\n";
	Ray transformed_ray_ = transform_ray(world_transformation_inv, ray_);

	glm::vec3 ray_direction{ glm::normalize(transformed_ray_.direction) }; // normalize direction (redundant?)

	std::vector<std::tuple<float, int>> distances; // t, (x|y|z)

	distances.push_back(std::make_tuple((minimum_.x - transformed_ray_.origin.x) / ray_direction.x, 0));
	distances.push_back(std::make_tuple((maximum_.x - transformed_ray_.origin.x) / ray_direction.x, 0));
	distances.push_back(std::make_tuple((minimum_.y - transformed_ray_.origin.y) / ray_direction.y, 1));
	distances.push_back(std::make_tuple((maximum_.y - transformed_ray_.origin.y) / ray_direction.y, 1));
	distances.push_back(std::make_tuple((minimum_.z - transformed_ray_.origin.z) / ray_direction.z, 2));
	distances.push_back(std::make_tuple((maximum_.z - transformed_ray_.origin.z) / ray_direction.z, 2));

	// sort the vector by distances from low to high
	std::sort(distances.begin(), distances.end(), [](std::tuple<float, int> & t_0, std::tuple<float, int> & t_1) -> bool {
		return std::get<0>(t_0) < std::get<0>(t_1);
		});

	// erase all the negative distances
	std::erase_if(distances, [](std::tuple<float, int> & t) -> bool { return std::get<0>(t) < 0; });

	// this seems highly illegal: assume the shortest 3 distances are the only valid ones, works with my tests, will probably fail at edge cases
	/*if (distances.size() > 3)
	{
		distances.erase(distances.begin() + 3, distances.end());
	}*/
	
	// boolean later used to remember whether an intersection was in bounds of y, for example while checking z
	bool is_in_bounds = true;

	// go through distances, check if one intersects and if yes, return
	for (std::tuple<float, int> tuple : distances)
	{
		// check if in bounds
		float t = std::get<0>(tuple); // the distance t
		int x_y_z = std::get<1>(tuple); // the value that stored whether we intersected with an x-, y- or z-plane
		glm::vec3 intersection{ transformed_ray_.origin + t * ray_direction }; // calculate the intersection point with said plane
		for (int i = x_y_z + 1; i < x_y_z + 3; ++i) // have to compare with other variables (if x-plane, compare to y and z)
		{
			int j = i % 3; // this makes sure that we get all the indices different from the one we intersected with
			is_in_bounds = true; // in case this was set to false by an earlier intersection test, reset
			if (!((minimum_[j] <= intersection[j]) && (intersection[j] <= maximum_[j]))) // if the intersection is not in bounds...
			{
				is_in_bounds = false; // ...remember that...
				break; // ...and exit the checks for this intersection
			}
		}
		// check if all was in bounds before
		if (is_in_bounds) 
		{ 
			glm::vec3 surface_normal = get_surface_normal(intersection);
			Ray retransformed_ray_{ transform_ray(world_transformation_, Ray{ intersection + 0.0001f * surface_normal, ray_direction }) };
			Ray transformed_surface_normal{ transform_ray(glm::transpose(world_transformation_), Ray{{0, 0, 0}, surface_normal}) };
			//std::cout << "ray after final transformation = " << retransformed_ray_.origin[0] << ", " << retransformed_ray_.origin[1] << ", " << retransformed_ray_.origin[2]
			//	<< ", " << retransformed_ray_.origin[0] << ", " << retransformed_ray_.origin[1] << ", " << retransformed_ray_.origin[2] << "\n";
			return { true, t, Shape::name_, Shape::material_, retransformed_ray_.origin, retransformed_ray_.direction, transformed_surface_normal.direction };
		}
	}

	// if we didn't find an intersection so far, return false and some default values
	return HitPoint{ false, 0, "", std::shared_ptr<Material>{}, glm::vec3{}, glm::vec3{}, glm::vec3{} };
}

glm::vec3 Box::get_center() const
{
	return glm::vec3{ (maximum_.x + minimum_.x) / 2.0f, (maximum_.y + minimum_.y) / 2.0f, (maximum_.z + minimum_.z) / 2.0f };
}

glm::vec3 Box::get_surface_normal(glm::vec3 const& hit_position_) const
{
	float margin = 0.0001;
	glm::vec3 surface_normal{ 0, 0, 0 };
	for (int i = 0; i < 3; ++i)
	{
		if (std::fabs(minimum_[i] - hit_position_[i]) < margin) //-margin < minimum_[i] - hitpoint.direction_[i] < margin
		{
			surface_normal[i] = -1.0f;
			return surface_normal;
		}
		else if (std::fabs(maximum_[i] - hit_position_[i]) < margin) //-margin < minimum_[i] - hitpoint.direction_[i] < margin
		{
			surface_normal[i] = 1.0f;/*hitpoint.position_[i];*/
			return surface_normal;/*glm::normalize(surface_normal);*/
		}
	}
	glm::vec3 debug{ hit_position_ };
	std::cout << "couldn't find intersecting plane\n";
	return surface_normal;
}