#include "Texture.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

Texture::Texture(const char *file){
    unsigned error;
    std::vector<unsigned char> png;
    size_t pngsize;
    lodepng::load_file(png,file);
    error = lodepng::decode(image, width, height, png);
    if(error){
        std::cout<<"decode error"<<error<<":"<<lodepng_error_text(error)<<std::endl;
        assert(!error);
    }
}

Texture::~Texture(){
}

glm::vec3 Texture::get_colour(double u, double v){
    //std::cout<<"Tget_colour"<<std::endl;
    unsigned x = u*width;
    unsigned y = v*height;
    int position = y*width + x;
    double r = image[4*position + 0];
    double g = image[4*position + 1];
    double b = image[4*position + 2];
    double a = image[4*position + 3];
    int checkColour = 191 + 64 * ((x/16) % 2 ) == ((y/16) % 2);
    r = (a * r + (255 - a) * checkColour) / 255;
    g = (a * g + (255 - a) * checkColour) / 255;
    b = (a * b + (255 - a) * checkColour) / 255;
    glm::vec3 colour = {r/255.0,g/255.0,b/255.0};
    //std::cout<<glm::to_string(colour)<<std::endl;
    return colour;
}
