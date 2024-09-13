#include "sdf_writer.hpp"

void write_to_sdf_file(std::string const& filepath, int current_frame, std::vector<glm::vec3> & positions)
{
	int current_frame_ = current_frame;

	if (current_frame_ > 59)
	{
		current_frame_ -= 60;
	}

	std::ofstream sdf_file(filepath);
	if (!sdf_file.is_open())
	{
		std::cout << "could not find or open: " << filepath << "\n";
		return;
	}
	sdf_file.clear();
	sdf_file << "define material white 1 1 1 0.1 0.1 0.1 0.1 0.1 0.1 50 0 \n"
		<< "define material mirror 0 0 0 0 0 0 0 0 0 0 1\n"
		<< "define material reflective_white 1 1 1 0.1 0.1 0.1 0.1 0.1 0.1 50 0.9\n"
		<< "define material blue_grey 0.4117647058823529 0.4509803921568627 0.5568627450980392 0.04117647058823529 0.04509803921568627 0.05568627450980392 0.04117647058823529 0.04509803921568627 0.05568627450980392 50 0.2\n"
		<< "define material wine_red 0.4117647058823529 0.0627450980392157 0.2901960784313725 0.04117647058823529 0.00627450980392157 0.02901960784313725 0.04117647058823529 0.00627450980392157 0.02901960784313725 50 0.2\n"
		<< "define material lighter_wine_red 0.5843137254901961 0.0196078431372549 0.2588235294117647 0.05843137254901961 0.00196078431372549 0.02588235294117647 0.05843137254901961 0.00196078431372549 0.02588235294117647 50 0.2\n"
		<< "define material orange 0.6941176470588235 0.203921568627451 0.1254901960784314 0.06941176470588235 0.0203921568627451 0.01254901960784314 0.06941176470588235 0.0203921568627451 0.01254901960784314 50 0.2\n"
		<< "define material yellow 0.9764705882352941 0.7058823529411765 0.003921568627451 0.09764705882352941 0.07058823529411765 0.0003921568627451 0.09764705882352941 0.07058823529411765 0.0003921568627451 50 0.2\n"
		<< "define material purple 0.5 0.45 1 0.05 0.045 0.1 0.05 0.045 0.1 50 0.2\n"
		<< "define material sky_blue 0.2 0.8392156862745098 1 0.02 0.08392156862745098 0.1 0.02 0.08392156862745098 0.1 50 0.1\n"
		<< "define material reflective_black 0.0005 0.0005 0.05 0.00005 0.00005 0.005 0.00005 0.00005 0.005 50 0.2"
		<< "\n";

	//camera has to move 3° every frame

	float starting_height = 1.88484; //so that a fall lasts a little less than 1/8th of the animation
	glm::vec3 starting_position = { starting_height, starting_height, 0 };
	float distance_fallen = (current_frame_ * current_frame_) / (24.0 * 24.0) * 0.5 * 9.81;
	float x_position = std::sqrt( starting_height * starting_height - distance_fallen * distance_fallen);

	glm::vec3 current_position = { std::max(0.0f, x_position), std::max(0.0f, starting_height - distance_fallen), 0.0f };
	if (current_frame_ < 15)
	{
		current_position = positions[current_frame_ % 15];
		sdf_file << "define shape sphere swinging_sphere " << std::to_string(current_position[0]) << " " << std::to_string(current_position[1]) << " -0.5 0.5 yellow\n";
		sdf_file << "define shape sphere sphere_0 0 0 -0.5 0.5 blue_grey\n";
	}

	else if (current_frame_ / 15 == 1)
	{
		current_position = positions[15 - current_frame_ % 15];
		current_position[0] = -1 * current_position[0];
		sdf_file << "define shape sphere swinging_sphere 0 0 -0.5 0.5 yellow\n";
		sdf_file << "define shape sphere sphere_0 " << std::to_string(current_position[0]) << " " << std::to_string(current_position[1]) << " -0.5 0.5 blue_grey\n";
	}

	else if (current_frame_ / 15 == 2)
	{
		current_position = positions[current_frame_ % 15];
		current_position[0] = -1 * current_position[0];
		sdf_file << "define shape sphere swinging_sphere 0 0 -0.5 0.5 yellow\n";
		sdf_file << "define shape sphere sphere_0 " << std::to_string(current_position[0]) << " " << std::to_string(current_position[1]) << " -0.5 0.5 blue_grey\n";
	}

	else if (current_frame_ / 15 == 3)
	{
		current_position = positions[15 - current_frame_ % 15];
		sdf_file << "define shape sphere swinging_sphere " << std::to_string(current_position[0]) << " " << std::to_string(current_position[1]) << " -0.5 0.5 yellow\n";
		sdf_file << "define shape sphere sphere_0 0 0 -0.5 0.5 blue_grey\n";
	}

	float angle = current_frame * 3 * std::numbers::pi / 180;

	sdf_file << "define shape box x_axis 300 0.01 0.01 -300 -0.01 -0.01 white\n"
		<< "define shape box y_axis 0.01 300 0.01 -0.01 -300 -0.01 white\n"
		<< "define shape box z_axis 0.01 0.01 300 -0.01 -0.01 -300 white\n"
		<< "define shape box cradle_top 6 0.001 0 0 -0.001 -1 reflective_white\n"
		<< "define shape box floor 20 0 20 -20 -0.2 -20 reflective_black\n"
		<< "define shape box behind_mirror 20 20 20.5 -20 0 20 mirror\n"
		<< "define shape box right_mirror 20.5 20 20 20 0 -20 mirror\n"
		<< "define shape box front_mirror 20 20 -20 -20 0 -20.5 mirror\n"
		<< "define shape box left_mirror -20 20 20 -20.5 0 -20 mirror\n"
		<< "\n"
		<< "define shape sphere sphere_1 2 1 -0.5 0.5 wine_red\n"
		<< "define shape sphere sphere_2 3 1 -0.5 0.5 lighter_wine_red\n"
		<< "define shape sphere sphere_3 4 1 -0.5 0.5 orange\n"
		<< "\n"
		<< "define shape composite newton's_cradle sphere_0 sphere_1 sphere_2 sphere_3 swinging_sphere cradle_top\n"
		<< "define shape composite all newton's_cradle floor\n"
		<< "\n"
		<< "define camera eye 90\n"
		<< "\n"
		<< "define light orange 100 100 33 0.6941176470588235 0.203921568627451 0.1254901960784314 100\n"
		<< "ambient 0.7 0.2 0.125\n"
		<< "\n"
		<< "transform eye rotate " << angle * 180 / std::numbers::pi << " 0 1 0\n"
		<< "transform eye translate " << 5 * sin(angle) << " " << 4.5 << " " << 5 * cos(angle) << "\n"
		<< "transform swinging_sphere translate 5 1 0\n"
		<< "transform sphere_0 translate 1 1 0\n"
		<< "transform newton's_cradle translate -3 2 0.5\n";
	sdf_file.close();
}