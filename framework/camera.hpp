#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <iostream>
#include <numbers>
#include <cmath>

//cameras live in right handed coordinate space
//(thumb upwards) = y, (index finger away from you) = z, (middle finger) = x
//default camera points towards negative z direction
class Camera
{
public:
	Camera();
	Camera(std::string const& name_parameter, float fov_x_parameter, glm::vec3 const& eye_parameter, glm::vec3 const& dir_parameter, glm::vec3 const& up_parameter);
	~Camera();
	std::vector<Ray> generate_rays(unsigned int width_, unsigned int height_) const;
	std::ostream& print(std::ostream& os) const;
	glm::mat4 world_transformation_ = glm::mat4(1.0);
private:
	std::string name_;
	float fov_x; //in radians please
	glm::vec3 eye_;
	glm::vec3 dir_ = { 0, 0, -1 };
	glm::vec3 up_ = { 0, 1, 0 };
	float distance_;
};

std::ostream& operator<<(std::ostream& os, Camera const& camera);

#endif