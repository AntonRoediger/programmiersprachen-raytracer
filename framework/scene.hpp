#ifndef SCENE_HPP
#define SCENE_HPP

#include "material.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "camera.hpp"

#include <string>
#include <map>
#include <vector>

struct Scene
{
	const std::map<std::string, std::shared_ptr<Material>> materials;
	const std::shared_ptr<Shape> root;
	const std::vector<Light> lights;
	const Color ambient;
	Camera camera;

	//does this need a constructor definition here?
};

#endif