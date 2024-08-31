#include "sdf_reader.hpp"


Scene read_sdf_file(std::string const& sdf_file_path)
{
	{
		std::ifstream sdf_file(sdf_file_path); // an input stream that reads data from a file, ofstream would be used to create files

		if (!sdf_file.is_open())
		{
			std::cout << "Could not find or open: " << sdf_file_path << '\n';
			return Scene{}; // needs consideration
		}

		std::map<std::string, std::shared_ptr<Material>> materials;
		std::map<std::string, std::shared_ptr<Shape>> shapes;
		std::vector<Light> lights;

		std::string line_buffer; // individual lines are stored here


		while (std::getline(sdf_file, line_buffer)) // gets the next line from sdf_file and stores it in line_buffer
		{
			std::istringstream line_as_stream(line_buffer); // creates an input stream so that >> can be used to read from line_buffer

			std::string token; // a string to store words in (from whitespace to whitespace)

			line_as_stream >> token; // store the next word in token

			if (token == "define") // if it's define, we continue
			{
				line_as_stream >> token; // see what we're defining

				if (token == "material")
				{
					Material parsed_material; // create a material object

					// start filling the member variables
					line_as_stream >> parsed_material.name;

					// had to write operator[] for Color
					for (int ka_index = 0; ka_index < 3; ++ka_index)
					{
						line_as_stream >> parsed_material.ka[ka_index];
					}

					for (int kd_index = 0; kd_index < 3; ++kd_index)
					{
						line_as_stream >> parsed_material.kd[kd_index];
					}

					for (int ks_index = 0; ks_index < 3; ++ks_index)
					{
						line_as_stream >> parsed_material.ks[ks_index];
					}

					line_as_stream >> parsed_material.exponent;

					//std::cout << '\n' << parsed_material << '\n';

					materials.insert({ parsed_material.name, std::make_shared<Material>(parsed_material) }); // the materials still exist after the function is done, because of the shared pointers
				}

				else if (token == "shape")
				{
					line_as_stream >> token;

					if (token == "sphere")
					{
						std::string parsed_sphere_name_;
						glm::vec3 parsed_sphere_center_;
						float parsed_sphere_radius_;
						std::string parsed_sphere_material_; //use this string to look up the material

						line_as_stream >> parsed_sphere_name_;
						line_as_stream >> parsed_sphere_center_[0];
						line_as_stream >> parsed_sphere_center_[1];
						line_as_stream >> parsed_sphere_center_[2];
						line_as_stream >> parsed_sphere_radius_;
						line_as_stream >> parsed_sphere_material_;

						auto material = materials.find(parsed_sphere_material_);
						if (material == materials.end())
						{
							std::cout << "the Material " << parsed_sphere_material_ << " could not be found.\n";
						}

						else
						{
							Sphere parsed_sphere{ parsed_sphere_name_, (*material).second, parsed_sphere_center_, parsed_sphere_radius_ };
							shapes.insert({ parsed_sphere_name_, std::make_shared<Sphere>(parsed_sphere) }); //don't do std::make_shared<Shape>, EVER!
						}
					}

					else if (token == "box")
					{
						std::string parsed_box_name_;
						std::string parsed_box_material_;
						glm::vec3 parsed_box_max_;
						glm::vec3 parsed_box_min_;

						line_as_stream >> parsed_box_name_;
						for (int x_y_z = 0; x_y_z < 3; ++x_y_z)
						{
							line_as_stream >> parsed_box_max_[x_y_z];
						}
						for (int x_y_z = 0; x_y_z < 3; ++x_y_z)
						{
							line_as_stream >> parsed_box_min_[x_y_z];
						}

						line_as_stream >> parsed_box_material_;

						auto material = materials.find(parsed_box_material_);
						if (material == materials.end())
						{
							std::cout << "the Material " << parsed_box_material_ << "could not be found.\n";
						}
						else
						{
							Box parsed_box{ parsed_box_name_, (*material).second, parsed_box_max_, parsed_box_min_ };
							shapes.insert({ parsed_box_name_, std::make_shared<Box>(parsed_box) });
						}
					}

					else
					{
						std::cout << "unexpected keyword: " << token << '\n';
					}

				}

				else if (token == "light")
				{
					Light parsed_light; //if weird error, probably change this
					line_as_stream >> parsed_light.name_;

					for (int i = 0; i < 3; ++i)
					{
						line_as_stream >> parsed_light.position_[i];
					}
					for (int i = 0; i < 3; ++i)
					{
						line_as_stream >> parsed_light.color_[i];
					}

					line_as_stream >> parsed_light.brightness_;
					lights.push_back(parsed_light);
				}

				else
				{
					std::cout << "unexpected keyword: " << token << '\n';
				}
			}

			else
			{
				std::cout << "unexpected keyword: " << token << '\n';
			}
		}

		sdf_file.close();
		return Scene{ materials, shapes, lights };
	}
}