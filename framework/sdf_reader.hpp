#ifndef SDF_READER_HPP
#define SDF_READER_HPP

#include "material.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include "light.hpp"
#include "composite.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <utility>

Scene read_sdf_file(std::string const& sdf_file_path);

#endif