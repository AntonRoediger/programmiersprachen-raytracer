#include "sdf_reader.hpp"


Scene read_sdf_file(std::string const& sdf_file_path)
{
	{
		std::ifstream sdf_file(sdf_file_path); // an input stream that reads data from a file, ofstream would be used to create files

		if (!sdf_file.is_open())
		{
			std::cout << "Could not find or open: " << sdf_file_path << '\n';
			return Scene{ std::map<std::string, std::shared_ptr<Material>>{}, std::shared_ptr<Shape>{}, std::vector<Light>{}, { 0, 0, 0 } }; // needs consideration
		}

		std::map<std::string, std::shared_ptr<Material>> materials;
		std::map<std::string, std::shared_ptr<Shape>> shapes;
		std::shared_ptr<Shape> root;
		std::vector<Light> lights;
		Color ambient{ 0.1, 0.1, 0.1 };
		Camera camera{ "default_camera", 90, { 0, 0, 0 }, { 0, 0, -1 }, { 0, 1, 0 } };
		std::string parsed_camera_name_;

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

					line_as_stream >> parsed_material.reflectivity;

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

					else if (token == "composite")
					{
						std::string parsed_composite_name_;
						std::map<std::string, std::shared_ptr<Shape>> parsed_composite_children_;
						std::string child_name_;

						line_as_stream >> parsed_composite_name_;
						while (line_as_stream)
						{
							line_as_stream >> child_name_;
							auto child = shapes.find(child_name_);
							if (child == shapes.end())
							{
								std::cout << "couldn't find child:" << child_name_ << " make sure the objects are parsed in the correct order\n";
							}
							else
							{
								parsed_composite_children_.insert({ child_name_, (*child).second });
							}
						}
						Composite parsed_composite{ parsed_composite_name_, std::make_shared<Material>(Material{}), parsed_composite_children_ };
						shapes.insert({ parsed_composite_name_, std::make_shared<Composite>(parsed_composite) });
						root = std::make_shared<Composite>(parsed_composite);
					}

					else
					{
						std::cout << "unexpected keyword: " << token << '\n';
					}

				}

				else if (token == "light")
				{
					Light parsed_light; //if weird error, probably change this to not involve default construction that is undefined
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

				else if (token == "camera")
				{
					float parsed_camera_fov_x_;
					glm::vec3 parsed_camera_eye_ = { 0, 0, 0 };
					glm::vec3 parsed_camera_dir_ = { 0, 0, -1 };
					glm::vec3 parsed_camera_up_ = { 0, 1, 0 };

					line_as_stream >> parsed_camera_name_;
					line_as_stream >> parsed_camera_fov_x_;

					/*for (int i = 0; i < 3; ++i)
					{
						line_as_stream >> parsed_camera_eye_[i];
					}

					if (line_as_stream)
					{
						for (int i = 0; i < 3; ++i)
						{
							line_as_stream >> parsed_camera_dir_[i];
						}
						glm::normalize(parsed_camera_dir_);
					}
					else
					{
						parsed_camera_dir_ = { 0, 0, -1 };
					}

					if (line_as_stream)
					{
						for (int i = 0; i < 3; ++i)
						{
							line_as_stream >> parsed_camera_up_[i];
						}
						glm::normalize(parsed_camera_up_);
					}
					else
					{
						parsed_camera_up_ = { 0, 1, 0 };
					}*/

					camera = { parsed_camera_name_, parsed_camera_fov_x_, parsed_camera_eye_, parsed_camera_dir_, parsed_camera_up_ };
					glm::vec3 u = glm::normalize(glm::cross(parsed_camera_dir_, parsed_camera_up_));
					glm::vec3 v = glm::normalize(glm::cross(u, parsed_camera_dir_));
					camera.world_transformation_ = { glm::vec4{u, 0}, glm::vec4{v, 0}, glm::vec4{-parsed_camera_dir_, 0} , glm::vec4{parsed_camera_eye_, 1} };
				}

				else
				{
					std::cout << "unexpected keyword: " << token << '\n';
				}
			}

			else if (token == "")
			{
				continue;
			}

			else if (token == "transform")
			{
				line_as_stream >> token;

				glm::mat4 world_transformation_;
				glm::mat4 world_transformation_inv;

				std::shared_ptr<Shape> shape_;

				glm::mat4 transf_mat = glm::mat4(1.0);

				int marker = -1; //0 means camera, 1 means shape

				if (token == parsed_camera_name_)
				{
					world_transformation_ = camera.world_transformation_;
					marker = 0;
				}

				for (auto const& [name, shape] : shapes)
				{
					if (token == name)
					{
						shape_ = shape;
						world_transformation_ = shape->world_transformation_;
						world_transformation_inv = shape->world_transformation_inv;
						marker = 1;
					}
				}

				line_as_stream >> token;

				if (token == "scale")
				{
					line_as_stream >> transf_mat[0][0];
					line_as_stream >> transf_mat[1][1];
					line_as_stream >> transf_mat[2][2];

					world_transformation_ = transf_mat * world_transformation_;
					world_transformation_inv = glm::inverse(world_transformation_);
				}
				else if (token == "translate")
				{
					line_as_stream >> transf_mat[3][0];
					line_as_stream >> transf_mat[3][1];
					line_as_stream >> transf_mat[3][2];

					world_transformation_ = transf_mat * world_transformation_;
					world_transformation_inv = glm::inverse(world_transformation_);
				}
				else if (token == "rotate")
				{
					float angle;
					float coef0;
					float coef1;
					float coef2;
					line_as_stream >> angle;
					line_as_stream >> coef0;
					line_as_stream >> coef1;
					line_as_stream >> coef2;

					if (coef0 == 1)
					{
						transf_mat[1][1] = cos(angle * std::numbers::pi / 180);
						transf_mat[1][2] = -sin(angle * std::numbers::pi / 180);
						transf_mat[2][1] = sin(angle * std::numbers::pi / 180);
						transf_mat[2][2] = cos(angle * std::numbers::pi / 180);

						world_transformation_ = transf_mat * world_transformation_;
						world_transformation_inv = glm::inverse(world_transformation_);
					}

					else if (coef1 == 1)
					{
						transf_mat[0][0] = cos(angle * std::numbers::pi / 180);
						transf_mat[2][0] = sin(angle * std::numbers::pi / 180);
						transf_mat[0][2] = -sin(angle * std::numbers::pi / 180);
						transf_mat[2][2] = cos(angle * std::numbers::pi / 180);

						world_transformation_ = transf_mat * world_transformation_;
						world_transformation_inv = glm::inverse(world_transformation_);
					}

					else if (coef2 == 1)
					{
						transf_mat[0][0] = cos(angle * std::numbers::pi / 180);
						transf_mat[1][0] = -sin(angle * std::numbers::pi / 180);
						transf_mat[0][1] = sin(angle * std::numbers::pi / 180);
						transf_mat[1][1] = cos(angle * std::numbers::pi / 180);

						world_transformation_ = transf_mat * world_transformation_;
						world_transformation_inv = glm::inverse(world_transformation_);
					}
				}
				else
				{
					std::cout << "unexpected keyword: " << token << "\n";
				}
				switch (marker)
				{
				case 0: 
					camera.world_transformation_ = world_transformation_;
					break;
				case 1: 
					shape_->world_transformation_ = world_transformation_; 
					shape_->world_transformation_inv = world_transformation_inv;
					break;
				default: 
					std::cout << "couldn't find the object to transform\n";
					break;
				}
			}

			else if (token == "ambient")
			{
				for (int i = 0; i < 3; ++i)
				{
					line_as_stream >> ambient[i];
				}
			}

			else
			{
				std::cout << "unexpected keyword: " << token << '\n';
			}
		}

		sdf_file.close();
		return Scene{ materials, root, lights, ambient, camera };
	}
}