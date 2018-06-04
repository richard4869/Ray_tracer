#pragma once

#include <stdio.h>
#include <glm/ext.hpp>

class Ray{
    public:
    glm::vec4 origin;
    glm::vec4 dir;

    Ray(glm::vec4 o,glm::vec4 dir);
    Ray(const Ray & r);
};
