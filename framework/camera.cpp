#include "camera.hpp"

Camera::Camera() :
	name_("default_camera"),
	fov_x(std::numbers::pi / 2.0),
	eye_({ 0, 0, 0 }),
	dir_({ 0, 0, -1 }),
	up_({ 0, 1, 0 }),
	distance_((std::sin(std::numbers::pi / 2.0 - fov_x / 2.0) / (2 * std::sin(fov_x / 2.0))))
{
	std::cout << "Camera constructor called\n";
}

Camera::Camera(std::string const& name_parameter, float fov_x_parameter, glm::vec3 const& eye_parameter, glm::vec3 const& dir_parameter, glm::vec3 const& up_parameter) :
	name_(name_parameter), 
	fov_x(fov_x_parameter * std::numbers::pi / 180),
	eye_(eye_parameter), 
	dir_(dir_parameter), 
	up_(up_parameter), 
	distance_((std::sin(std::numbers::pi/2.0 - fov_x/2.0)/(2 * std::sin(fov_x/2.0))))
{
	std::cout << "Camera constructor called\n";
}

Camera::~Camera()
{
	std::cout << "Camera destructor called\n";
}

std::vector<Ray> Camera::generate_rays(unsigned int width_, unsigned int height_) const
{
	std::vector<Ray> rays; //stores all rays in order of their pixels (top left to bottom right)

	glm::vec3 origin_shift_1{ (width_ - 1) / 2.0, (height_ - 1) / 2.0, distance_ * (height_ - 1)/*if distance_==1, you get 45°*/};

	for (int current_row = 0; current_row < height_; ++current_row)
	{
		for (int current_column = 0; current_column < width_; ++current_column)
		{
			rays.push_back(Ray{ eye_, glm::vec3{ current_column, current_row, 0 } - origin_shift_1 });
		}
	}

	//so far we should have a default camera at origin{0, 0, 0} pointing towards {0, 0, -1}, up is {0, 1, 0} (no matter what up_ and dir_ are)
	//now apply a rotation matrix to the camera conform to up_ and dir_
	
	return rays;
}

std::ostream& Camera::print(std::ostream& os) const
{
	return os << "name: " << name_ << ", fov-x: " << fov_x << ", eye: (" << eye_[0] << ", " << eye_[1] << ", " << eye_[2] << ")" << ", dir: (" << dir_[0] << ", " << dir_[1] << ", " << dir_[2] << "), up: (" << up_[0] << ", " << up_[1] << ", " << up_[2] << "), distance: " << distance_ << "\n";
}

std::ostream& operator<<(std::ostream& os, Camera const& camera)
{
	return camera.print(os);
}