#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Intersection.hpp"
#include "Photonmap.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);

Intersection intersect(Ray & r,SceneNode * root);
Intersection inside_intersect(Ray & r,SceneNode * root,bool inside);
Intersection inside_detect (Ray & r,SceneNode * root,bool inside);
glm::vec3 direct_light(SceneNode* root,const std::list<Light*> lights,const Intersection & intr);
glm::vec3 reflect(SceneNode* root,const std::list<Light*> lights,const glm::vec3 ambient,const Intersection & intr, int time_remain);
glm::vec3 look_for_photon(Intersection & intr, int time_remain, SceneNode* root,Photon_map *p_map);
void photon_scattering (Ray & r, Photon_map *p_map,double dis,glm::vec3 colour,const glm::vec3 ambient,SceneNode* root);
glm::vec4 refraction(Intersection & intr);
void bounce_photons(SceneNode* root,
                    Ray & photon_ray,
                    const glm::vec3 ambient,
                    Photon_map *p_map,
                    glm::vec3 colour);
void emit_photons(const std::list<Light*> lights,
                  SceneNode* root,
                  const glm::vec3 ambient,
                  Photon_map *p_map);

glm::vec4 Hem_random(glm::vec4 nomal);
glm::vec4 Sphere_random();
