#pragma once

#include <stdio.h>
#include <glm/ext.hpp>

#include "Ray.hpp"
#include "PhongMaterial.hpp"


struct GeometryNode;

struct Intersection {
    //the trcing ray
    Ray t_ray;
    double t;                             //distance from the origin.

    bool hit;                             //hit or not

    PhongMaterial *material;
    glm::dvec4 normal;
    GeometryNode *node;
    glm::dvec4 point;
    float from_index;
    float in_index;

    Intersection();
    Intersection(const Ray & ray,double t,bool hit);
};
