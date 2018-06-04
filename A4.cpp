#include <glm/ext.hpp>
#include <stdlib.h>
#include <random>
#include "A4.hpp"
#include <math.h>

#define GLOSSY_INDEX 30
#define SOFT_SHADOW_RANGE 10
#define SOFT_SHADOW_INDEX 10
#define TIME_OF_REFLECTION 2
#define MAX_PHOTON 500000
#define SEARCH_RANGE 3.0
#define SEARCH_NUM 20000
#define FOCUS 0
#define RAY_TRACER 0
#define PHOTON_MAPPING 1
#define FOCUS_INDEX 10

std::mt19937 rng;
std::uniform_real_distribution<float>distribution2(0.0,1.0);
std::normal_distribution<float>distribution3(0.0,1.0);

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
) {

  // Fill in raytracing code here...
  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;
////////////////////////////////////////////////////////////////////////////
	size_t h = image.height();
	size_t w = image.width();
	double a_width = glm::tan(glm::radians(fovy/2));
	double a_height = glm::tan(glm::radians(fovy/2));
////////////////////////////////////////////////////////////////////////////
    glm::vec3 v = glm::normalize(view);
    glm::vec3 u = glm::normalize(up);
    glm::vec3 left = glm::normalize(glm::cross(v,u));
/////////////////////////////////////////////////////////////////////////////
    Photon_map *p_map;
    if(PHOTON_MAPPING == 1){
        p_map = new Photon_map(3000000);
        emit_photons(lights,root,ambient,p_map);
        //std::cout<<"balance"<<std::endl;
        p_map->balance();
        //std::cout<<"balance over"<<std::endl;
    }
/////////////////////////////////////////////////////////////////////////////
	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
            glm::vec3 colour (0,0,0);
            glm::vec3 dir = glm::normalize(v + (-1 + 2*(0.5+y)/h)*a_height* -u
                                           + (-1 + 2*(0.5+x)/w)*a_width*left);
            if(FOCUS == 0){
                Ray r = Ray(glm::vec4(eye,1),glm::vec4(dir,0));
                Intersection p_intr = intersect(r,root);
                glm::vec3 origin_c = glm::vec3(0);
                glm::vec3 light_c = glm::vec3(0);
                glm::vec3 reflection_c = glm::vec3(0);
                glm::vec3 global_c = glm::vec3(0);
                if(p_intr.hit){
                    glm::vec4 point = p_intr.t_ray.origin + p_intr.t_ray.dir * p_intr.t ;
                    point = point + 0.01*glm::vec4(p_intr.normal);
                    if(PHOTON_MAPPING == 1){
                        global_c =look_for_photon(p_intr,1,root,p_map);
                    }
                    if(RAY_TRACER == 1){
                        origin_c = p_intr.node->get_colour(p_intr.point) * ambient;

                        light_c = direct_light(root,lights,p_intr);
                        if(p_intr.material->m_reflectivity != 0){
                            reflection_c = reflect(root,lights,ambient,p_intr,TIME_OF_REFLECTION);
                        }
                    }
                    colour =  (1.0 - p_intr.material->m_reflectivity) * (origin_c + light_c) + p_intr.material->m_reflectivity * reflection_c+ 120.0*global_c;
                }else{
                    colour = {117.0f/255.0f,163.0f/255.0f,224.0f/255.0f};
                }
            }else{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////FOCUS OF VIEW
                glm::vec3 focus_point = eye + dir*FOCUS;
                double r1= distribution2(rng);
                double r2= distribution2(rng);
                for (int i = 0; i < FOCUS_INDEX; i++){
                    r1 = r1*4-2;
                    r2 = r2*4-2;
                    glm::vec3 n_eye = eye + r1*u + r2*left;
                    glm::vec3 n_dir = glm::normalize(focus_point - n_eye);
                    Ray r = Ray(glm::vec4(n_eye,1),glm::vec4(n_dir,0));
                    Intersection p_intr = intersect(r,root);
                    glm::vec3 origin_c = glm::vec3(0);
                    glm::vec3 light_c = glm::vec3(0);
                    glm::vec3 reflection_c = glm::vec3(0);
                    glm::vec3 global_c = glm::vec3(0);
                    glm::vec3 diffuse_c = glm::vec3(0);
                    if(p_intr.hit){
                        glm::vec4 point = p_intr.t_ray.origin + p_intr.t_ray.dir * p_intr.t ;
                        point = point + 0.01*glm::vec4(p_intr.normal);
                        if(PHOTON_MAPPING == 1){
                            global_c =look_for_photon(p_intr,1,root,p_map);
                        }
                        if(RAY_TRACER == 1){
                            origin_c = p_intr.node->get_colour(p_intr.point) * ambient;

                            light_c = direct_light(root,lights,p_intr);
                            if(p_intr.material->m_reflectivity != 0){
                                reflection_c = reflect(root,lights,ambient,p_intr,TIME_OF_REFLECTION);
                            }
                        }
                        diffuse_c =  (1.0 - p_intr.material->m_reflectivity) * (origin_c + light_c) + p_intr.material->m_reflectivity * reflection_c + 120.0*global_c;
                        colour += diffuse_c/9.0;
                    }else{
                        diffuse_c = {117.0f/255.0f,163.0f/255.0f,224.0f/255.0f};
                        colour += diffuse_c/9.0;
                    }
                }
            }
            image(x,y,0)=colour.r;
            image(x,y,1)=colour.g;
            image(x,y,2)=colour.b;
		}
	}
	if(PHOTON_MAPPING == 1){
        delete p_map;
	}
}

glm::vec3 look_for_photon(Intersection & intr, int time_remain, SceneNode* root,Photon_map *p_map){
    //std::cout<<"look for"<<std::endl;
    glm::vec3 colour = glm::vec3(0);
    glm::vec3 s_colour,rfl_colour,rfr_colour,vs_colour;
    glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
    if(intr.hit){
        s_colour = p_map->irradiance_estimate(intr,SEARCH_RANGE,SEARCH_NUM);
    }else{
        s_colour = {117.0f/255.0f,163.0f/255.0f,224.0f/255.0f};
        s_colour = s_colour/100.0;
    }
    if(intr.hit){
        if(intr.material->m_reflectivity > 0){                                                      //reflection
            //std::cout<<"reflect"<<std::endl;
            glm::vec3 r1 = glm::normalize(glm::vec3(intr.t_ray.dir));
            glm::vec3 n = glm::normalize(glm::vec3(intr.normal));
            glm::vec3 Rm = r1 - 2*n*(glm::dot(r1,n));
            glm::vec4 rfl_dir = glm::vec4(Rm,0);
            Ray rfl_ray = Ray(point+0.01 * rfl_dir, rfl_dir);
            Intersection p_intr = intersect(rfl_ray,root);
            if(p_intr.hit){
                if(time_remain != 0){
                    rfl_colour = look_for_photon(p_intr,time_remain-1,root,p_map);
                }
            }else{
                rfl_colour = {117.0f/255.0f,163.0f/255.0f,224.0f/255.0f};
                rfl_colour = rfl_colour/100.0;
            }
        }
        if(intr.material->m_refraction > 0){                                                               //refraction
            //std::cout<<"refract"<<std::endl;
            glm::vec4 rfr_dir = refraction(intr);
            Ray rfr_ray =  Ray(point+0.001* rfr_dir,rfr_dir);
            Intersection p_intr = intersect(rfr_ray,root);
            if(p_intr.hit){
                if(time_remain > 0){
                    rfr_colour = look_for_photon(p_intr,time_remain-1,root,p_map);
                }else{
                    rfr_colour = look_for_photon(p_intr,time_remain,root,p_map);
                }
            }else{
                rfr_colour = {117.0f/255.0f,163.0f/255.0f,224.0f/255.0f};
                rfr_colour = rfr_colour/100.0;
            }
        }
        if(intr.material->m_vol_s != 0){                                                                    //volume scattering
            //std::cout<<"asdasdasd"<<std::endl;
            Ray con_ray = Ray(point + 0.001*intr.t_ray.dir,intr.t_ray.dir);
            Intersection con_intr = intersect(con_ray,root);
            if(con_intr.hit){
                glm::vec4 point2 = con_intr.t_ray.origin + con_intr.t_ray.dir * con_intr.t;
                double dis = glm::distance(point, point2);
                double times = dis/SEARCH_RANGE;
                glm::vec4 dir = glm::normalize(intr.t_ray.dir);
                double i;
                for(i = 0; i < times; i++){
                    glm::vec3 temp = p_map->scattering_estimate(intr,SEARCH_RANGE,SEARCH_NUM);
                    vs_colour += temp;
                    intr.t += SEARCH_RANGE;
                }
                vs_colour *= times/i;
                vs_colour /= 50;
                if(con_intr.material == intr.material){
                    Ray n_ray = Ray(point2 + 0.001 * con_intr.t_ray.dir,con_intr.t_ray.dir);
                    Intersection n_intr = intersect(n_ray,root);
                    if(n_intr.hit){
                        s_colour = look_for_photon(n_intr,time_remain,root,p_map);
                    }
                }else{
                    s_colour = look_for_photon(con_intr,time_remain,root,p_map);
                }
            }
        }
    }
    colour = intr.material->m_reflectivity * rfl_colour
        + intr.material->m_refraction * rfr_colour
        + (1.0 - intr.material->m_reflectivity - intr.material->m_refraction) * s_colour
        + vs_colour;
    return colour;
}

glm::vec4 refraction(Intersection & intr){
    glm::vec4 out_dir;
    glm::dvec4 normal = glm::normalize(intr.normal);
    glm::dvec4 V = glm::normalize(intr.t_ray.dir);
    double c1 = - glm::dot(normal,V);
    double n = intr.from_index/intr.in_index;
    double c2 = std::sqrt(1.0 - n*n * (1.0 - c1*c1));
    out_dir = (n * V) + (n * c1 - c2) * normal;
    return out_dir;
}

Intersection intersect(Ray & r,SceneNode * root){
    Intersection intr = root->intersect(r);
    //std::cout<<"intersect"<<" "<<intr.hit<<std::endl;
    Intersection m_intr;
    if(intr.hit){
        glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
        Ray c_ray = Ray(point + 0.01*intr.t_ray.dir, intr.t_ray.dir);
        if(intr.material->m_negative == 1){
            if(intr.in_index == 1.0){
                Intersection d_intr = root->intersect(c_ray);
                if(d_intr.hit){
                    if(d_intr.material->m_negative != 1 && d_intr.in_index == 1.0){
                        return intr;
                    }
                }
                m_intr = inside_intersect(c_ray,root,false);
                if(m_intr.hit){
                    m_intr.t += intr.t+0.01;
                    m_intr.t_ray.origin = intr.t_ray.origin;
                    m_intr.t_ray.dir = intr.t_ray.dir;
                }
            }else{
                m_intr = inside_intersect(c_ray,root,false);
                if(m_intr.hit){
                    m_intr.t_ray.origin = intr.t_ray.origin;
                    m_intr.t_ray.dir = intr.t_ray.dir;
                    m_intr.t += intr.t + 0.01;
                }
            }
            return m_intr;
        }else{

            if(intr.in_index == 1.0){
                m_intr = inside_detect(c_ray, root, false);
                if(m_intr.hit && m_intr.material->m_negative == 1 && m_intr.in_index == 1.0){
                    Intersection n_intr = inside_intersect(r,root,true);
                    return n_intr;
                }else{
                    return intr;
                }
            }else{
                m_intr = inside_detect(c_ray, root, true);
                if(m_intr.hit && m_intr.material->m_negative == 1 && m_intr.in_index == 1.0){
                    Intersection n_intr = inside_intersect(r,root,false);
                    return n_intr;
                }else{
                    return intr;
                }
            }
        }
    }else{
        return intr;
    }
}

Intersection inside_detect (Ray & r,SceneNode * root,bool inside){
    Intersection intr = root->intersect(r);
    //std::cout<<"indside"<<" "<<intr.hit<<" "<<inside<<std::endl;
    if(intr.hit){
        if(inside == true){
            if(intr.material->m_negative == 1){
                return intr;
            }else{
                //std::cout<<"inside solid"<<std::endl;
                glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
                //std::cout<<glm::to_string(point)<<std::endl;
                Ray c_ray = Ray(point + 0.01*intr.t_ray.dir, intr.t_ray.dir);
                Intersection m_intr = inside_detect(c_ray,root,false);
                //assert(m_intr.hit);
                if(m_intr.hit){
                    m_intr.t_ray.origin = intr.t_ray.origin;
                    m_intr.t_ray.dir = intr.t_ray.dir;
                    m_intr.t += intr.t+0.01;
                }
                return m_intr;
            }
        }else{
            glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
            Ray c_ray = Ray(point + 0.01*intr.t_ray.dir, intr.t_ray.dir);
            if(intr.material->m_negative == 1){
                return intr;
            }else{
                //std::cout<<"outside solid"<<std::endl;
                //std::cout<<glm::to_string(point)<<std::endl;
                //std::cout<<glm::to_string(intr.t_ray.dir)<<std::endl;
                Intersection m_intr = inside_detect(c_ray,root,true);
                //assert(m_intr.hit);
                if(m_intr.hit){
                    m_intr.t_ray.origin = intr.t_ray.origin;
                    m_intr.t_ray.dir = intr.t_ray.dir;
                    m_intr.t += intr.t+0.01;
                }
                return m_intr;
            }
        }
    }else{
        return intr;
    }
}

Intersection inside_intersect(Ray & r,SceneNode * root,bool inside){
    Intersection intr = root->intersect(r);
    //std::cout<<"indside"<<" "<<intr.hit<<" "<<inside<<std::endl;
    if(intr.hit){
        if(inside == true){
            if(intr.material->m_negative == 1){
                return intr;
            }else{
                //std::cout<<"inside solid"<<std::endl;
                glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
                Ray c_ray = Ray(point + 0.01*intr.t_ray.dir, intr.t_ray.dir);
                Intersection m_intr = inside_intersect(c_ray,root,false);
                //assert(m_intr.hit);
                if(m_intr.hit){
                    m_intr.t_ray.origin = intr.t_ray.origin;
                    m_intr.t_ray.dir = intr.t_ray.dir;
                    m_intr.t += intr.t+0.01;
                }
                return m_intr;
            }
        }else{
            glm::vec4 point = intr.t_ray.origin + intr.t_ray.dir * intr.t ;
            Ray c_ray = Ray(point + 0.01*intr.t_ray.dir, intr.t_ray.dir);
            if(intr.material->m_negative == 1){
                //std::cout<<"outside negative"<<std::endl;
                Intersection m_intr = intersect(c_ray,root);
                if(m_intr.hit){
                    //std::cout<<"hit?"<<m_intr.t<<std::endl;
                    m_intr.t_ray.origin = intr.t_ray.origin;
                    m_intr.t_ray.dir = intr.t_ray.dir;
                    m_intr.t += intr.t+0.01;
                }
                return m_intr;
            }else{
                //std::cout<<"outside solid"<<std::endl;
                Intersection m_intr = inside_intersect(c_ray,root,true);
                //assert(m_intr.hit);
                if(m_intr.hit){
                    m_intr.t_ray.origin = intr.t_ray.origin;
                    m_intr.t_ray.dir = intr.t_ray.dir;
                    m_intr.t += intr.t+0.01;
                }
                return m_intr;
            }
        }
    }else{
        return intr;
    }
}

glm::vec3 direct_light(SceneNode* root,const std::list<Light*> lights,const Intersection & intr){
    glm::vec3 colour,cur_colour;
    //std::default_random_engine rng;
    glm::vec3 r1 = glm::normalize(glm::vec3(intr.t_ray.dir));
    glm::vec3 n = glm::normalize(glm::vec3(intr.normal));
    glm::vec3 Rm = r1 - 2*n*(glm::dot(r1,n));
        for (Light* light : lights){
        glm::vec3 diffuse_light {0,0,0};
        glm::vec3 specular_light {0,0,0};
        glm::vec4 point = intr.t_ray.origin +intr.t_ray.dir * intr.t;
        glm::vec4 shadow_dir = glm::vec4(light->position,1) -point;

        Ray shadow_ray  = Ray(point + 0.01 * glm::normalize(shadow_dir),shadow_dir);
        double light_dis = glm::length(shadow_dir);
        for (int i = 0; i < SOFT_SHADOW_INDEX; i++){
            cur_colour = glm::vec3(0);
            int HI = SOFT_SHADOW_RANGE;
            int LO = -HI;
            std::uniform_real_distribution<float>distribution(HI,LO);
            float rx = distribution(rng);
            float ry = distribution(rng);
            float rz = distribution(rng);

            Ray diffuse_shadow_ray = Ray (point, glm::vec4(0));
            diffuse_shadow_ray.dir.x =shadow_ray.dir.x + rx;
            diffuse_shadow_ray.dir.y =shadow_ray.dir.y + ry;
            diffuse_shadow_ray.dir.z =shadow_ray.dir.z + rz;
            diffuse_shadow_ray.origin = diffuse_shadow_ray.origin + 0.001 * diffuse_shadow_ray.dir;

            Intersection shadow_intr = intersect(diffuse_shadow_ray,root);
            if((shadow_intr.hit)&&(glm::length(shadow_intr.t * diffuse_shadow_ray.dir)<light_dis)){
               // std::cout<<i<<"do nothing"<<std::endl;
                //do nothing
            }else{
                glm::vec4 light_ray = glm::normalize(diffuse_shadow_ray.dir);
                double costheta = std::max(glm::dot(n,glm::vec3(light_ray)),0.0f);
                if(glm::length(intr.node->get_colour(intr.point))!=0){
                    double length = intr.t * glm::length(intr.t_ray.dir);
                    diffuse_light = intr.node->get_colour(intr.point) * costheta * light->colour/(light->falloff[0]
                                                                                    +light->falloff[1]*length
                                                                                    +light->falloff[2]*length*length);
                }
                cur_colour += diffuse_light;
                if(glm::length(intr.material->m_ks)>0){
                    costheta = std::max(glm::dot(Rm,glm::normalize(glm::vec3(diffuse_shadow_ray.dir))),0.0f);
                    double phongcoef = std::pow(costheta,intr.material->m_shininess);
                    specular_light = phongcoef * intr.material->m_ks * light->colour;
                }
                cur_colour += specular_light;
            }
            colour += cur_colour* (1.0f/SOFT_SHADOW_INDEX);
            //std::cout<<i<<" "<<shadow_intr.hit<<" "<<glm::to_string(cur_colour)<<std::endl;
        }
    }
    return colour;
}

glm::vec3 reflect(SceneNode* root,const std::list<Light*> lights,const glm::vec3 ambient,const Intersection & intr, int time_remain){
    glm::vec3 colour,cur_colour = glm::vec3(0);
    glm::vec3 origin_c,light_c,reflection_c;

    if(time_remain == 0){
        return colour;
    }else{
        time_remain -= 1;
        glm::vec3 r1 = glm::normalize(glm::vec3(intr.t_ray.dir));
        glm::vec3 n = glm::normalize(glm::vec3(intr.normal));
        glm::vec3 Rm = r1 - 2*n*(glm::dot(r1,n));
        glm::vec4 point = intr.t_ray.origin +intr.t_ray.dir * intr.t;
        Ray refl_ray = Ray(point,glm::vec4(Rm,0));
        for (int i=0;i<GLOSSY_INDEX;i++){
            float HI = intr.material->m_glossiness;
            float LO = -HI;
            std::uniform_real_distribution<float>distribution(HI,LO);
            float rx = distribution(rng);
            float ry = distribution(rng);
            float rz = distribution(rng);
            Ray diffuse_ray = Ray (point, glm::vec4(0));
            diffuse_ray.dir.x =refl_ray.dir.x + rx;
            diffuse_ray.dir.y =refl_ray.dir.y + ry;
            diffuse_ray.dir.z =refl_ray.dir.z + rz;
            //floating point error correction
            diffuse_ray.origin =  diffuse_ray.origin + diffuse_ray.dir*0.1;
            Intersection refl_intr = intersect(diffuse_ray,root);

            if(refl_intr.hit == true){
                origin_c = refl_intr.node->get_colour(intr.point) * ambient;
                light_c = direct_light(root,lights,refl_intr);
                if(refl_intr.material->m_reflectivity != 0){
                    reflection_c = reflect(root,lights,ambient,refl_intr,time_remain);
                }
                cur_colour = (1.0 - refl_intr.material->m_reflectivity) * (origin_c + light_c) + refl_intr.material->m_reflectivity*reflection_c;
            }else{
                cur_colour = {117.0f/255.0f,163.0f/255.0f,224.0f/255.0f};
            }
            colour += cur_colour * (1.0f/GLOSSY_INDEX);
        }
        return colour;
    }
}

void emit_photons(const std::list<Light*> lights,
                  SceneNode* root,
                  const glm::vec3 ambient,
                  Photon_map *p_map){
        //std::cout<<"emit"<<std::endl;
    for (Light* light : lights){
        for (int i = 0; i<MAX_PHOTON;i++){
            glm::vec4 dir = Sphere_random();
            Ray photon_ray = Ray(glm::vec4(light->position,1),dir);
            bounce_photons(root,photon_ray,ambient,p_map,light->colour);
        }
        float scale = 1.0f/MAX_PHOTON * light->intensity;
        p_map->scale_photon_power(scale);
    }
}


void bounce_photons(SceneNode* root,
                    Ray & photon_ray,
                    const glm::vec3 ambient,
                    Photon_map *p_map,
                    glm::vec3 colour){

    //std::cout<<"bounce"<<std::endl;
    Intersection p_intr = intersect(photon_ray,root);
    if(p_intr.hit){
        glm::vec4 point = p_intr.t_ray.origin + p_intr.t_ray.dir * p_intr.t;
        glm::vec4 normal = glm::normalize(p_intr.normal);
        glm::vec3 r1 = glm::normalize(glm::vec3(p_intr.t_ray.dir));
        glm::vec3 Rm = r1 - 2*glm::vec3(normal)*(glm::dot(r1,glm::vec3(normal)));
        double pd = p_intr.material->m_pd;
        double ps = p_intr.material->m_ps;
        glm::vec3 t_colour= p_intr.node->get_colour(p_intr.point);
        float pr = distribution2(rng);
        if(pr < pd){
            //diffuse reflect
            //std::cout<<"difffuse refl"<<std::endl;
            glm::vec4 dir;
            glm::vec3 power = t_colour * ambient;
            dir = Hem_random(normal);
            Ray sec_photon_ray = Ray(point,dir);
            bounce_photons(root,sec_photon_ray,ambient,p_map,power);
        }else if(pr < ps){
            //specular reflection
            //std::cout<<"specular refl"<<std::endl;
            glm::vec3 power = t_colour * ambient;
            Ray sec_photon_ray = Ray(point,glm::vec4(Rm,0));
            bounce_photons(root,sec_photon_ray,ambient,p_map,power);
        }else{
            //absorb
            if(p_intr.material->m_refraction > 0.0){
                float rfr = distribution2(rng);
                if(rfr < p_intr.material->m_refraction){
                    //refraction
                    //std::cout<<"refraction"<<std::endl;
                    glm::vec4 rfr_dir = refraction(p_intr);
                    Ray rfr_ray = Ray(point+0.001*rfr_dir,rfr_dir);
                    glm::vec3 power = 0.2 * colour + 0.8 * t_colour * ambient;
                    bounce_photons(root,rfr_ray,ambient,p_map,power);
                }else{
                    //store
                    glm::vec3 location = glm::vec3(point) + 0.01*glm::vec3(normal);
                    glm::vec3 power = 0.2 * colour + 0.8 * t_colour * ambient;
                    p_map->store(power,location,glm::vec3(p_intr.t_ray.dir));
                }
            }else if(p_intr.material->m_vol_s == 1){
                Ray con_ray = Ray(glm::vec4(p_intr.point) + 0.001 * p_intr.t_ray.dir,p_intr.t_ray.dir);
                Intersection con_intr = intersect(con_ray,root);
                glm::vec4 point2 = con_intr.t_ray.origin + con_intr.t * con_intr.t_ray.dir;
                double dis = glm::distance(point,point2);
                Ray r = Ray(point,p_intr.t_ray.dir);
                photon_scattering(r, p_map, dis,colour,ambient,root);
            }else{
                glm::vec3 location = glm::vec3(point) + 0.01*glm::vec3(normal);
                glm::vec3 power = 0.2 * colour + 0.8 * t_colour * ambient;
                p_map->store(power,location,glm::vec3(p_intr.t_ray.dir));
            }
        }
    }
    //std::cout<<"bounce over"<<std::endl;
}

void photon_scattering (Ray & r,
                        Photon_map *p_map,
                        double dis,
                        glm::vec3 colour,
                        const glm::vec3 ambient,
                        SceneNode* root){
    double smk_rfl = distribution2(rng);
    double d = -std::log(smk_rfl)/0.2;
    double cha_rfl = 0.001;
    double cha_absorb = 0.1;
    glm::vec4 dir = glm::normalize(r.dir);
    if(d > dis){
        //going out
        r.origin += (dis + 0.01)*dir;
        bounce_photons(root,r,ambient,p_map,colour);
    }else{
        double ctr = distribution2(rng);
        if(ctr < cha_rfl){
            //reflect
            r.origin += dir * d;
            r.dir = Sphere_random();
            Intersection intr = intersect(r,root);
            if(intr.hit){
                glm::vec4 new_point = intr.t_ray.origin + intr.t_ray.dir * intr.t;

                //std::cout<<"refl"<<" "<<glm::to_string(r.origin)<<std::endl;
                double new_dis = glm::distance(r.origin, new_point);
                photon_scattering(r,p_map,new_dis,colour*0.8,ambient,root);
            }else{
                photon_scattering(r,p_map,100,colour*0.8,ambient,root);
            }

        }else if(ctr < cha_absorb){
            //absorb
            //std::cout<<"absorb"<<" "<<glm::to_string(r.origin)<<std::endl;
            r.origin += d * dir;
            p_map->store(colour,glm::vec3(r.origin),glm::vec3(r.dir));
        }else{
            //continue
            //std::cout<<"continue"<<" "<<dis<<std::endl;
            r.origin += d * dir;
            photon_scattering(r,p_map,dis - d,colour,ambient,root);
        }
    }
}

glm::vec4 Hem_random(glm::vec4 normal){
    float dir_x = distribution3(rng);
    float dir_y = distribution3(rng);
    float dir_z = distribution3(rng);
    float defector = 1.0f/(std::sqrt(dir_x*dir_x + dir_y*dir_y + dir_z*dir_z));
    glm::vec4 dir = glm::normalize(glm::vec4 (dir_x,dir_y,dir_z,0));
    dir = defector * dir;
    if(glm::dot(normal,dir) <= 0.0f) dir = -dir;
    dir = glm::normalize(dir);
    return dir;
}

glm::vec4 Sphere_random(){
    float dir_x = distribution3(rng);
    float dir_y = distribution3(rng);
    float dir_z = distribution3(rng);
    glm::vec4 dir = glm::normalize(glm::vec4 (dir_x,dir_y,dir_z,0));
    float defector = 1.0f/(std::sqrt(dir_x*dir_x+dir_y*dir_y+dir_z*dir_z));
    dir = defector * dir;
    dir = glm::normalize(dir);
    return dir;
}
