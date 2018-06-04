#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "lodepng/lodepng.h"

class Texture {
public:
    Texture(const char *file);
    ~Texture();
    glm::vec3 get_colour(double u,double v);
private:
    std::vector<unsigned char> image;
    unsigned width,height;
};
