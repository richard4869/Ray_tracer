#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glm/glm.hpp>
#include "Intersection.hpp"
#include "GeometryNode.hpp"

typedef struct Photon{
    float pos[3];
    short plane;
    unsigned char theta, phi;
    float power[3];
}Photon;

typedef struct NearestPhotons{
    int max;
    int found;
    float pos[3];
    int got_heap;
    float *dist2;
    const Photon **index;
}NearestPhotons;

class Photon_map{
    public:
        Photon_map(int max_phot);
        ~Photon_map();
        void store(glm::vec3 power,glm::vec3 pos,glm::vec3 dir);
        void scale_photon_power(const float scale);
        void balance();
        glm::vec3 irradiance_estimate(Intersection & intr,const float max_dist,const int nphotons)const;
        glm::vec3 scattering_estimate(Intersection & intr,const float max_dist,const int nphotons)const;
        void locate_photons(NearestPhotons *const np,const int index)const;
        void photon_dir(float *dir,const Photon *p)const;
    private:
        void balance_segment(Photon **pbal,
                             Photon **porg,
                             const int index,
                             const int start,
                             const int end);
        void median_split(Photon **p,
                          const int start,
                          const int end,
                          const int median,
                          const int axis );
        Photon *photons;

        int stored_photons;
        int half_stored_photons;
        int max_photons;
        int prev_scale;

        float costheta[256];
        float sintheta[256];
        float cosphi[256];
        float sinphi[256];

        float bbox_min[3];
        float bbox_max[3];
};
