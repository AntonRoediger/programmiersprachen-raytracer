#include <renderer.hpp>
#include <window.hpp>
#include "material.hpp"
#include "scene.hpp"
#include "sdf_reader.hpp"
#include "sdf_writer.hpp"
#include "ray.hpp"

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

#include <glm/vec3.hpp>
#include <vector>
#include <algorithm>
#include <limits>

//now single threaded again
int main(int argc, char* argv[])
{
	unsigned const image_width = 2560; //(1920) //formerly 800 //don't set to 0!
	unsigned const image_height = 1600; //(1080) //formerly 600 //don't set to 0!
	int max = 120;

	std::vector<glm::vec3> positions(15, glm::vec3{ 0, 0, 0 });

	for (int current_frame = 0; current_frame < max; ++current_frame)
	{
		float starting_height = 1.88484; //so that a fall lasts a little less than 1/8th of the animation
		glm::vec3 starting_position = { starting_height, starting_height, 0 };
		float distance_fallen = (current_frame * current_frame) / (24.0 * 24.0) * 0.5 * 9.81;
		float x_position = std::sqrt(starting_height * starting_height - distance_fallen * distance_fallen);

		glm::vec3 current_position = { std::max(0.0f, x_position), std::max(0.0f, starting_height - distance_fallen), 0.0f };

		if (current_frame < 15)
		{
			positions[current_frame] = current_position;
		}

		std::string filename = "C:\\university\\computer_science\\SoSe2024\\SE_I\\programmiersprachen-raytracer\\rendered_images\\rendered_image_";
		filename += std::to_string(current_frame);
		filename += ".ppm";

		std::string sdf_filename = "C:\\university\\computer_science\\SoSe2024\\SE_I\\programmiersprachen-raytracer\\sdf_files\\sdf_file_";
		sdf_filename += std::to_string(current_frame);
		sdf_filename += ".sdf";
		write_to_sdf_file(sdf_filename, current_frame, positions);

		Renderer renderer{ image_width, image_height, filename };

		renderer.render(sdf_filename);

		/*Window window{ {image_width, image_height} };

		while (!window.should_close()) {
			if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				window.close();
			}
			window.show(renderer.color_buffer());
		}*/
		std::cout << current_frame << "\n";
	}

	return 0;
}
