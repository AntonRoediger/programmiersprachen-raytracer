#ifndef PPM_WRITER_HPP
#define PPM_WRITER_HPP

#include <fstream>
#include <iostream>
#include <glm/vec3.hpp>
#include <string>
#include <numbers>
#include <vector>

void write_to_sdf_file(std::string const& filepath, int current_frame, std::vector<glm::vec3> & positions);

#endif