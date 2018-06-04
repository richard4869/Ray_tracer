#include "Photonmap.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/ext.hpp>



Photon_map::Photon_map(const int max_phot){
    stored_photons = 0;
    prev_scale = 1;
    max_photons = max_phot;

    photons = (Photon *)malloc(sizeof(Photon)*(max_photons+1));

    if(photons  == NULL){
        //std::cout<<"out of memory"<<std::endl;
        exit(-1);
    }
    bbox_min[0]=bbox_min[1]=bbox_min[2]= 1e8f;
    bbox_max[0]=bbox_max[1]=bbox_max[2]=-1e8f;

    for (int i = 0; i<256; i++){
        double angle = double(i)*(1.0/256.0)*M_PI;
        costheta[i] = cos(angle);
        sintheta[i] = sin(angle);
        cosphi[i] = cos( 2.0*angle);
        sinphi[i] = sin( 2.0 * angle);
    }
}

Photon_map ::~Photon_map(){
    free(photons);
}

void Photon_map::photon_dir(float *dir,const Photon *p)const{
    dir[0]=sintheta[p->theta]*cosphi[p->phi];
    dir[1]=sintheta[p->theta]*sinphi[p->phi];
    dir[2]=costheta[p->theta];
}

glm::vec3 Photon_map::irradiance_estimate(Intersection & intr,const float max_dist,const int nphotons)const{
    glm::vec3 irrad = glm::vec3(0);
    NearestPhotons np;
    np.dist2 = (float*)alloca(sizeof(float)*(nphotons+1));
    np.index = (const Photon**)alloca(sizeof(Photon*)*(nphotons+1));
    glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
    glm::vec4 normal = intr.normal;
    double range;
    if(intr.node->m_primitive->type == 0){
        range = 0.7;
    }else{
        range = 0.1;
    }
    np.pos[0] = point.x;
    np.pos[1] = point.y;
    np.pos[2] = point.z;
    np.max = nphotons;
    np.found = 0;
    np.got_heap = 0;
    np.dist2[0] = max_dist*max_dist;

    locate_photons(&np,1);
    if(np.found<8) return irrad;
    float pdir[3];
    for(int i=1; i<=np.found; i++){
        const Photon *p = np.index[i];

        photon_dir(pdir,p);
        glm::vec3 p_dir;

        p_dir.x = p->pos[0] - point.x;
        p_dir.y = p->pos[1] - point.y;
        p_dir.z = p->pos[2] - point.z;
        if(glm::dot(glm::vec3(normal),p_dir)<range &&glm::dot(glm::vec3(normal),p_dir) > -range){
            if((pdir[0]*normal.x+pdir[1]*normal.y+pdir[2]*normal.z) < 0.00001f){
                double dis = glm::length(p_dir);
                double wdp = 1 - dis/(1.5*max_dist);
                irrad.x += p->power[0]*wdp;
                irrad.y += p->power[1]*wdp;
                irrad.z += p->power[2]*wdp;
            }
        }
    }
    //std::cout<<min_dis1<<" "<<min_dis2<<std::endl;
    const float tmp = (1.0 - 2.0/(3.0*1.5))*M_PI*max_dist*max_dist;
    //std::cout<<"tmp"<<tmp<<std::endl;

    irrad.x /= tmp;
    irrad.y /= tmp;
    irrad.z /= tmp;
    return irrad;
}

glm::vec3 Photon_map::scattering_estimate(Intersection & intr,const float max_dist,const int nphotons)const{
    glm::vec3 irrad = glm::vec3(0);
    NearestPhotons np;
    np.dist2 = (float*)alloca(sizeof(float)*(nphotons+1));
    np.index = (const Photon**)alloca(sizeof(Photon*)*(nphotons+1));
    glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
    double range;
    if(intr.node->m_primitive->type == 0){
        range = 0.7;
    }else{
        range = 0.1;
    }
    np.pos[0] = point.x;
    np.pos[1] = point.y;
    np.pos[2] = point.z;
    np.max = nphotons;
    np.found = 0;
    np.got_heap = 0;
    np.dist2[0] = max_dist*max_dist;

    locate_photons(&np,1);
    if(np.found<1) {
        return irrad;
    }
    float pdir[3];
    for(int i=1; i<=np.found; i++){
        const Photon *p = np.index[i];

        photon_dir(pdir,p);

        irrad.x += p->power[0];
        irrad.y += p->power[1];
        irrad.z += p->power[2];
    }
    const float tmp = (1.0/M_PI)/max_dist*max_dist;

    irrad.x *= tmp;
    irrad.y *= tmp;
    irrad.z *= tmp;
    return irrad;
}

void Photon_map::locate_photons(NearestPhotons *const np,const int index)const{
    const Photon *p=&photons[index];
    float dist1;
    if(index < half_stored_photons){
        dist1 = np->pos[p->plane] - p->pos[p->plane];

        if(dist1 > 0.0){
            locate_photons(np,2*index +1);
            if(dist1*dist1 < np->dist2[0]) locate_photons(np,2*index);
        }else{
            locate_photons(np,2*index);
            if(dist1*dist1 < np->dist2[0]) locate_photons(np,2*index+1);
        }
    }

    dist1 = p->pos[0] - np->pos[0];
    float dist2 = dist1*dist1;
    dist1 = p->pos[1] - np->pos[1];
    dist2 += dist1*dist1;
    dist1 = p->pos[2]-np->pos[2];
    dist2 +=dist1*dist1;

    if(dist2 < np->dist2[0]){
        if(np->found < np->max){
            np->found++;
            np->dist2[np->found] = dist2;
            np->index[np->found] = p;
        }else{
            int j,parent;
            if(np->got_heap == 0){
                //build heap
                float dst2;
                const Photon *phot;
                int half_found = np->found >> 1;
                for(int k=half_found; k>=1; k--){
                    parent = k;
                    phot = np->index[k];
                    dist2 = np->dist2[k];
                    while(parent <= half_found){
                        j = parent+parent;
                        if(j<np->found && np->dist2[j]<np->dist2[j+1]) j++;
                        if(dst2 >= np->dist2[j])break;
                        np->dist2[parent] = np->dist2[j];
                        np->index[parent] = np->index[j];
                        parent = j;
                    }
                    np->dist2[parent] = dst2;
                    np->index[parent] = phot;
                }
                np->got_heap = 1;
            }
            parent = 1;
            j=2;
            while(j <= np->found){
                if(j < np->found && np->dist2[j] < np->dist2[j+1]) j++;
                if(dist2 > np->dist2[j])break;
                np->dist2[parent] = np->dist2[j];
                np->index[parent] = np->index[j];
                parent = j;
                j += j;
            }
            np->index[parent] = p;
            np->dist2[parent] =dist2;

            np->dist2[0]=np->dist2[1];
        }
    }
}

void Photon_map::store(glm::vec3 power,glm::vec3 pos,glm::vec3 dir){
    if(stored_photons > max_photons) return;
    stored_photons++;
    Photon *const node = &photons[stored_photons];

    node->pos[0] = pos.x;
    node->pos[1] = pos.y;
    node->pos[2] = pos.z;

    for(int i=0; i<3; i++){
        if(node->pos[i] < bbox_min[i]) bbox_min[i] = node->pos[i];
        if(node->pos[i] > bbox_max[i]) bbox_max[i] = node->pos[i];
    }
    if(power.x > 1 &&power.y >1 &&power.z >1)std::cout<<"irrad"<<glm::to_string(pos)<<glm::to_string(power)<<std::endl;
    power = power * 255.0;
    node->power[0] = power.x;
    node->power[1] = power.y;
    node->power[2] = power.z;
    //std::cout<<"irrad"<<glm::to_string(pos)<<glm::to_string(power)<<std::endl;
    int theta  = int (acos(dir.z)*(256.0/M_PI));
    if(theta > 255) node->theta =  255;
    else node->theta = (unsigned char)theta;

    int phi = int(atan2(dir.y,dir.x)*(256.0/(2.0*M_PI)));
    if(phi > 255) node->phi = 255;
    else if(phi<0) node->phi = (unsigned char)(phi+256);
    else node->phi = (unsigned char)phi;
}

void Photon_map::scale_photon_power(const float scale){
    //std::cout<<scale<<std::endl;
    for(int i=prev_scale; i<=stored_photons;i++){
        photons[i].power[0] *= scale;
        photons[i].power[1] *= scale;
        photons[i].power[2] *= scale;
    }
    prev_scale = stored_photons;
}

void Photon_map::balance(){
    if(stored_photons > 1){
        Photon **pa1 = (Photon **)malloc(sizeof(Photon*)*(stored_photons+1));
        Photon **pa2 = (Photon **)malloc(sizeof(Photon*)*(stored_photons+1));

        for(int i=0; i<=stored_photons; i++){
            pa2[i] = &photons[i];
        }
        balance_segment(pa1,pa2,1,1,stored_photons);
        free(pa2);

        int d,j=1,foo=1;
        Photon foo_photon = photons[j];

        for(int i=1; i<=stored_photons; i++){
            d = pa1[j]-photons;
            pa1[j] = NULL;
            if(d != foo) photons[j] = photons[d];
            else {
                photons[j] = foo_photon;
                if(i < stored_photons){
                    for(;foo<=stored_photons;foo++){
                        if(pa1[foo] != NULL) break;
                    }
                    foo_photon = photons[foo];
                    j = foo;
                }
                continue;
            }
            j = d;
        }
        free(pa1);
    }
    half_stored_photons = stored_photons/2-1;
}

#define swap(ph,a,b) {Photon *ph2=ph[a]; ph[a]=ph[b]; ph[b]=ph2;}

void Photon_map::median_split(Photon **p,const int start,const int end,const int median,const int axis ){
    int left = start;
    int right = end;
    while (right > left){
        const float v = p[right]->pos[axis];
        int i = left-1;
        int j = right;
        for(;;){
            while(p[++i]->pos[axis] < v);
            while(p[--j]->pos[axis] > v && j>left);
            if( i >= j) break;
            swap(p,i,j);
        }
        swap(p,i,right);
        if(i >= median) right = i-1;
        if(i <= median) left = i+1;
    }
}

void Photon_map::balance_segment(Photon **pbal,Photon **porg,const int index,const int start,const int end){
    int median = 1;
    while((4*median)<=(end-start+1)){
        median += median;
    }
    if((3*median)<=(end-start+1)){
        median += median;
        median += start - 1;
    }else{
        median = end-median+1;
    }
    int axis = 2;
    if((bbox_max[0]-bbox_min[0])>(bbox_max[1]-bbox_min[1])&&
       (bbox_max[0]-bbox_min[0])>(bbox_max[2]-bbox_min[2]))
        axis = 0;
    else if((bbox_max[1]-bbox_min[1])>(bbox_max[2]-bbox_min[2]))
        axis = 1;

    median_split(porg,start,end,median,axis);

    pbal[index] = porg[median];
    pbal[index]->plane = axis;

    if(median > start){
        if(start < median - 1){
            const float tmp = bbox_max[axis];
            bbox_max[axis] = pbal[index]->pos[axis];
            balance_segment(pbal,porg,2*index,start,median-1);
            bbox_max[axis] = tmp;
        }else{
            pbal[2*index] = porg[start];
        }
    }
    if(median < end){
        if(median+1 < end){
            const float tmp = bbox_min[axis];
            bbox_min[axis] = pbal[index]->pos[axis];
            balance_segment(pbal,porg,2*index+1,median+1,end);
            bbox_min[axis] = tmp;
        }else{
            pbal[2*index+1]=porg[end];
        }
    }
}
