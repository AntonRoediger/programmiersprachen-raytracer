// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"
#include <iostream>



// Scaling/Rotation/ NO Camera basis
Ray transformRay(glm::mat4 const& mat, Ray const& ray) {
    //point transformation
    glm::vec4 point_trans{ ray.origin, 1 };
    point_trans = mat * point_trans;
    // direction transformation
    glm::vec4 direct_trans{ ray.direction, 0 };
    direct_trans = mat * direct_trans;
    Ray ray_transed{ glm::vec3{ point_trans[0], point_trans[1], point_trans[2]}, glm::vec3{direct_trans[0], direct_trans[1], direct_trans[2]} };
    return ray_transed;
 
 
}

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

void Renderer::render()
{
  //read sdf file
  Scene scene_{ read_sdf_file("./example.sdf") };
  Camera test_camera = scene_.camera;
  std::cout << test_camera;

  std::vector<Ray> rays{ test_camera.generate_rays(width_, height_) }; //returns all rays in order of their pixels (top left to bottom right)

  //intersect all rays with all objects...
  for (int position = 0; position < rays.size(); ++position)
  {
      //make the following into a function

      Ray ray{ rays[position] };

      //write the calculated color as a pixel
      Pixel pixel{ position % width_, position / width_ };

      Color color = trace_ray(scene_, ray, 0);

      pixel.color = color;
      
      write(pixel);
  }
  ppm_.save(filename_); //writes everything out of ppm_.data_ into a ppm file
}

Color Renderer::trace_ray(Scene const& scene_, Ray const& ray_, unsigned int depth)
{
    ++depth;

    Color color_{ 0, 0, 0 };

    HitPoint min_distance_hitpoint{ false, std::numeric_limits<float>::max() };
    glm::mat4 Camera_Transformation = glm::mat4(1); //needed for the camera transformation to the objcet
    glm::vec3 intersected_shape_surface_normal;
    bool flag = false; //for normal during the transfomration
    for (auto const& [name, shape] : scene_.shapes) // Find closest intersection object
    {   
        HitPoint hitpoint;
        glm::vec3 transformed_normal;
        //Transform ray
        //if the shape has a non-uni transfomration matrix->TRANSFORM RAY in object coordinate system, find the hitpoint and trasnform back, else as normal
        if (shape->world_transformation_ != glm::mat4(1.0))
        {
            
                                                                               //// Camera_Transformation = { glm::vec4{1,0,0,0},glm::vec4{0,1,0,0}, glm::vec4{0,0,1,0}, glm::vec4{shape->get_center()[0],shape->get_center()[1],shape->get_center()[2],1} };
                                                                               //// Ray shape_oriented = transformRay(Camera_Transformation, ray_);
            
            shape->world_transformation_inv = glm::inverse(shape->world_transformation_);
            Ray ray_transformed = transformRay(shape->world_transformation_inv, ray_);
            hitpoint = { shape->intersect(ray_transformed) };
            
            if (hitpoint.did_intersect_) {
                transformed_normal= shape->get_surface_normal(hitpoint); // in the non-transofmred coordinate ysstem 
                flag = true;
            }
            
            //change back the transformation
            glm::vec4 point_trans{ hitpoint.position_, 1 };
            glm::vec4 result = shape->world_transformation_ * point_trans; 
                                                                                ////to the shape oriented system: glm::inverse(Camera_Transformation)* 
            hitpoint.position_ = { glm::vec3{ result[0], result[1], result[2]} };

        }
        else {
             hitpoint = shape->intersect(ray_);
        }
        
        
        if (hitpoint.did_intersect_ && (hitpoint.distance_ < min_distance_hitpoint.distance_)) //...and remember the closest hitpoint
        {
            if (flag) { //use the transofmred normal
                glm::vec4 temp = glm::transpose(shape->world_transformation_inv) * glm::vec4{ transformed_normal, 0 };
                                                                                        //// glm::inverse(Camera_Transformation)*
                intersected_shape_surface_normal = { temp[0], temp[1], temp[2] };
                flag = false;
            }
            else {
                intersected_shape_surface_normal = shape->get_surface_normal(hitpoint);
            }
            
            //move the hitpoint back a bit by the surface_normal, if shadow acne, check out get_surface_normal in Box and Sphere
            hitpoint.position_ = hitpoint.position_ +0.0001f * intersected_shape_surface_normal;
            min_distance_hitpoint = hitpoint;
        }
    }
    
    if (min_distance_hitpoint.did_intersect_) //if we did intersect an object
    {
        //see if there is an obstacle inbetween the hitpoint and the light source
        HitPoint closest_obstacle{ false, std::numeric_limits<float>::max() };
        //accumulate light from every visible light source
        for (int i = 0; i < 3; ++i)
        {
            color_[i] = 0.1f * min_distance_hitpoint.object_material_->ka[i]; //this is the ambient part
        }
        for (Light const& light : scene_.lights)
        {
            bool light_visibility = 1;
            //get the path to a lightsource
            Ray path{ min_distance_hitpoint.position_, light.position_ - min_distance_hitpoint.position_ };
            //check if any objects are in the way
            for (auto const& [name, shape] : scene_.shapes)
            {
                //remember the hitpoint

                HitPoint obstacle;
                //Transform ray
                //if the shape has a non-uni transfomration matrix->TRANSFORM RAY in object coordinate system, find the hitpoint and trasnform back, else as normal
                if (shape->world_transformation_ != glm::mat4(1.0))
                {
                                                                //// Ray shape_oriented = transformRay(Camera_Transformation, path);

                    shape->world_transformation_inv = glm::inverse(shape->world_transformation_);
                    Ray ray_transformed = transformRay(shape->world_transformation_inv, path);
                    obstacle = { shape->intersect(ray_transformed) };
                    glm::vec4 point_trans{ obstacle.position_, 1 };
                    glm::vec4 result =shape->world_transformation_ * point_trans;
                                                                            //// glm::inverse(Camera_Transformation) * 
                    obstacle.position_ = { glm::vec3{ result[0], result[1], result[2]} };
                }
                else {
                    obstacle = shape->intersect(path);
                }
                
                if (obstacle.did_intersect_ && obstacle.distance_ < closest_obstacle.distance_) //...and remember the closest hitpoint
                {
                    closest_obstacle = obstacle;
                    light_visibility = 0; //if there is an object in the way, this term nullifies diffuse and specular reflections
                }
            }
            //light_visibility * (brightness of light source) * (kd * path_to_light_source * surface_normal
            // + ks * (reflected/outgoing ray * the ray from the hitpoint to the cameras eye))
            glm::vec3 reflected = 2 * glm::dot(path.direction, intersected_shape_surface_normal) * intersected_shape_surface_normal - path.direction;
            reflected = glm::normalize(reflected);
            for (int i = 0; i < 3; ++i)
            {
                color_[i] += light_visibility * light.brightness_ * (min_distance_hitpoint.object_material_->kd[i]
                    * (std::max(glm::dot(path.direction, intersected_shape_surface_normal), 0.0f))
                    + min_distance_hitpoint.object_material_->ks[i]
                    * std::pow(std::max(glm::dot(reflected, -ray_.direction), 0.0f), min_distance_hitpoint.object_material_->exponent));
            }
        }

        if (min_distance_hitpoint.object_material_->reflectivity > 0.0f && depth < 100)
        {
            float other_part = min_distance_hitpoint.object_material_->reflectivity;
            float this_materials_part_ = 1.0f - other_part;
            glm::vec3 reflected = -2 * glm::dot(ray_.direction, intersected_shape_surface_normal) * intersected_shape_surface_normal + ray_.direction;
            Color reflected_part_ = trace_ray(scene_, Ray{ min_distance_hitpoint.position_, reflected }, depth);
            for (int i = 0; i < 3; ++i)
            {
                color_[i] = this_materials_part_ * color_[i] + other_part * reflected_part_[i];
            }
        }
        //surface_normal visualisation
        /*for (int i = 0; i < 3; ++i)
        {
            pixel.color[i] = intersected_shape_surface_normal[i] * 0.5 + 0.5;
        }*/
    }
    else //if no object is hit
    {
        color_ = { 0.25, 0.25, 0.25 }; //this could be some sort of background image
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
