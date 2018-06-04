#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <list>
#include <stdio.h>
#include <iostream>

#include "Ray.hpp"
#include "Intersection.hpp"

/*
type
    0 sphere or nh_sphere
    1 mesh or cube
*/

class Primitive {
public:
    int type;
    virtual ~Primitive();
    virtual Intersection intersect(const Ray & r);
};

class Sphere : public Primitive {
public:
    Sphere();
    virtual ~Sphere();
    virtual Intersection intersect(const Ray & r);
};

class Cube : public Primitive {
public:
    Cube();
    virtual ~Cube();
    virtual Intersection intersect(const Ray & r);
};

class NonhierSphere : public Primitive {
public:
    NonhierSphere(const glm::vec3& pos, double radius)
        : m_pos(pos), m_radius(radius)
    {
    }
    NonhierSphere();
    virtual ~NonhierSphere();
    virtual Intersection intersect(const Ray & r);
private:
    glm::vec3 m_pos;
    double m_radius;
};

class NonhierBox : public Primitive {
public:
    NonhierBox(const glm::vec3& pos, double size)
        : m_pos(pos), m_size(size)
    {
    }
    NonhierBox();
    virtual ~NonhierBox();
    virtual Intersection intersect(const Ray & r);
private:
    glm::vec3 m_pos;
    double m_size;
};

class Cylinder : public Primitive {
public:
    Cylinder();
    virtual ~Cylinder();
    virtual Intersection intersect(const Ray & r);
};

class Cone : public Primitive {
public:
    Cone();
    virtual ~Cone();
    virtual Intersection intersect(const Ray & r);
};
