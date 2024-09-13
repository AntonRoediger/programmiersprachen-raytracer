// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

void Renderer::render()
{
    render("./example.sdf");
}

void Renderer::render(std::string const& sdf_file_path)
{
  //read sdf file
  Scene scene_{ read_sdf_file(sdf_file_path) };
  Camera test_camera = scene_.camera;
  //std::cout << test_camera;

  std::vector<Ray> rays{ test_camera.generate_rays(width_, height_) }; //returns all rays in order of their pixels (top left to bottom right)

  //intersect all rays with all objects...
  for (int position = 0; position < rays.size(); ++position)
  {
      //make the following into a function

      Ray ray{ rays[position] };

      //write the calculated color as a pixel
      Pixel pixel{ position % width_, position / width_ };

      Color color = trace_ray(scene_, ray, 0);

      for (int i = 0; i < 3; ++i)
      {
          pixel.color[i] = color[i] / (color[i] + 1);
      }
      
      write(pixel);
  }
  ppm_.save(filename_); //writes everything out of ppm_.data_ into a ppm file
}

Color Renderer::trace_ray(Scene const& scene_, Ray const& ray_, unsigned int depth)
{
    ++depth;

    Color color_{ 0, 0, 0 };

    glm::vec3 intersected_shape_surface_normal;

    HitPoint hitpoint{ scene_.root->intersect(ray_) };

    intersected_shape_surface_normal = hitpoint.surface_normal;

    //hitpoint.position_ = hitpoint.position_ + 0.0001f * intersected_shape_surface_normal;

    if (hitpoint.did_intersect_) //if we did intersect an object
    {
        //see if there is an obstacle inbetween the hitpoint and the light source
        //accumulate light from every visible light source
        for (int i = 0; i < 3; ++i)
        {
            color_[i] = scene_.ambient[i] * hitpoint.object_material_->ka[i]; //this is the ambient part
        }

        for (Light const& light : scene_.lights)
        {
            bool light_visibility = 1;

            //get the path to a lightsource
            Ray path{ hitpoint.position_, light.position_ - hitpoint.position_ };

            //check if there's any object inbetween the hitpoint and the light source
            HitPoint closest_obstacle{ scene_.root->intersect(path) };
            if (closest_obstacle.did_intersect_)
            {
                light_visibility = 0;
            }

            //light_visibility * (brightness of light source) * (kd * path_to_light_source * surface_normal
            // + ks * (reflected/outgoing ray * the ray from the hitpoint to the cameras eye))
            glm::vec3 reflected = 2 * glm::dot(path.direction, intersected_shape_surface_normal) * intersected_shape_surface_normal - path.direction;
            reflected = glm::normalize(reflected);
            for (int i = 0; i < 3; ++i)
            {
                color_[i] += light_visibility * light.brightness_ * (hitpoint.object_material_->kd[i]
                    * (std::max(glm::dot(path.direction, intersected_shape_surface_normal), 0.0f))
                    + hitpoint.object_material_->ks[i]
                    * std::pow(std::max(glm::dot(reflected, -ray_.direction), 0.0f), hitpoint.object_material_->exponent));
            }
        }

        if (hitpoint.object_material_->reflectivity > 0.0f && depth < 25)
        {
            float other_part = hitpoint.object_material_->reflectivity;
            float this_materials_part_ = 1.0f - other_part;
            glm::vec3 reflected = -2 * glm::dot(ray_.direction, intersected_shape_surface_normal) * intersected_shape_surface_normal + ray_.direction;
            Color reflected_part_ = trace_ray(scene_, Ray{ hitpoint.position_, reflected }, depth);
            for (int i = 0; i < 3; ++i)
            {
                color_[i] = this_materials_part_ * color_[i] + other_part * reflected_part_[i];
            }
        }
        //surface_normal visualisation
        /*for (int i = 0; i < 3; ++i)
        {
            color_[i] = intersected_shape_surface_normal[i] * 0.5 + 0.5;
        }*/
    }
    else //if no object is hit
    {
        for (int i = 0; i < 3; ++i)
        {
            color_[i] = (*scene_.materials.find("reflective_black")).second->ka[i]; //this could be some sort of background image
        }
    }
    return color_;
}

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    color_buffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}
