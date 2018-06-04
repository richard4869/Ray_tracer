#include "Intersection.hpp"


Intersection::Intersection():
    t_ray({glm::vec4(),glm::vec4()}),
    hit(false),
    t(0)
{
}

Intersection::Intersection(const Ray & ray,double t,bool hit):
    t_ray(ray),
    t(t),
    hit(hit)
{
}

