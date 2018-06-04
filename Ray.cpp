#include "Ray.hpp"


Ray::Ray(glm::vec4 o,glm::vec4 dir){
    origin=o;
    this->dir=dir;
}

Ray::Ray(const Ray & r){
    origin = r.origin;
    this->dir = r.dir;
}

