#include "Primitive.hpp"
#include "polyroots.hpp"



Primitive::~Primitive()
{
}

Intersection Primitive::intersect(const Ray & r){
    Intersection intr (r,0,false);
    return intr;
}


Sphere::Sphere()
:Primitive(){
    type = 0;
}

Sphere::~Sphere()
{
}

Intersection Sphere::intersect(const Ray & r){
    NonhierSphere s = NonhierSphere({0,0,0},1.0);
    Intersection i = s.intersect(r);
    //std::cout<<i.hit<<std::endl;
    return i;
}

Cube::Cube()
{
    type = 1;
}
Cube::~Cube()
{
}

Intersection Cube::intersect(const Ray & r){
    NonhierBox c = NonhierBox({0,0,0},1.0);
    return c.intersect(r);
}

NonhierSphere::NonhierSphere()
{
    type = 0;
}

NonhierSphere::~NonhierSphere()
{
}

Intersection NonhierSphere::intersect(const Ray & r){
    //std::cout<<"sphere  Ray "<<glm::to_string(r.origin)<<" "<<glm::to_string(r.dir)<<std::endl;
    Intersection intr = Intersection(r,0,false);
    glm::dvec4 c = glm::dvec4(m_pos.x,m_pos.y,m_pos.z,1);
    glm::dvec4 d =r.dir;
    glm::dvec4 e = r.origin;
    //std::cout<<glm::to_string(m_pos)<<" "<<m_radius<<std::endl;
    double qdra_a = glm::dot(d,d);
    double qdra_b = 2.0f*glm::dot(d,e-c);
    double qdra_c = glm::dot(e-c,e-c) - m_radius*m_radius;
    double roots[2];

    int num_roots = quadraticRoots(qdra_a,qdra_b,qdra_c,roots);

    if(num_roots == 2){     //2 roots
        if(roots[0]>0&&roots[1]>0){
            intr.t = std::min(roots[0],roots[1]);
            intr.hit = true;
        }else if(roots[0]>0){
            intr.t = roots[0];
            intr.hit = true;
        }else if(roots[1]>0){
            intr.t = roots[1];
            intr.hit = true;
        }else{
            intr.hit = false;
        }
    }else if(num_roots == 1){                 //1 roots
        if(roots[0]>0){
            intr.hit = true;
            intr.t = roots[0];
        }else{
            intr.hit = false;
        }
    }else{                                      //no roots
        intr.hit = false;
    }
    if(intr.hit){
        intr.point = r.origin+r.dir*intr.t;
        intr.normal =glm::normalize(intr.point-c);
        intr.from_index = 1.0f;
        intr.in_index = 0.0f;
        if(glm::distance(glm::vec3(r.origin), m_pos) < m_radius){
            intr.normal = -intr.normal;
            intr.from_index = 0.0f;
            intr.in_index = 1.0f;
        }
    }
    return intr;

}

NonhierBox::NonhierBox()
{
    type = 1;
}

NonhierBox::~NonhierBox()
{
}

Intersection NonhierBox::intersect(const Ray & r){
    Intersection intr(r,0,false);
    glm::vec4 pos = glm::vec4(m_pos,0);
    glm::vec4 origins[6];
    origins[0] = {0.5,0,0.5,1};      //down
    origins[1] = {0.5,1,0.5,1};      //up
    origins[2] = {0,0.5,0.5,1};      //left
    origins[3] = {1,0.5,0.5,1};      //right
    origins[4] = {0.5,0.5,0,1};      //front
    origins[5] = {0.5,0.5,1,1};      //back

    for (int i = 0 ; i<6;i++){
        origins[i] += pos;
    }

    glm::vec4 normals[6];
    normals[0] = {0,-1,0,0};      //down
    normals[1] = {0,1,0,0};      //up
    normals[2] = {-1,0,0,0};      //left
    normals[3] = {1,0,0,0};      //right
    normals[4] = {0,0,-1,0};      //front
    normals[5] = {0,0,1,0};      //back

    float dist,r_dist;
    bool success;

    for (int i = 0; i < 6; i++){
        bool success = intersectRayPlane(intr.t_ray.origin,intr.t_ray.dir,origins[i],normals[i],dist);
        bool r_success = intersectRayPlane(intr.t_ray.origin ,intr.t_ray.dir,origins[i],-normals[i],r_dist);
        if(success){
            if(dist > 0){
                glm::dvec4 point = intr.t_ray.origin + intr.t_ray.dir*dist;
                //std::cout<<"cube"<<glm::to_string(point)<<std::endl;
                if(point.x > -0.001 + m_pos.x && point.x < m_size+0.001 + m_pos.x&&
                   point.y > -0.001 + m_pos.y && point.y < m_size+0.001 + m_pos.y&&
                   point.z > -0.001 + m_pos.z && point.z < m_size+0.001 + m_pos.z){
                    //std::cout<<"cube pass"<<dist<<std::endl;
                    //std::cout<<glm::to_string(point)<<std::endl;
                    if(intr.hit == false){
                        intr.t = dist;
                        intr.hit = true;
                        intr.point = point;
                        intr.normal = glm::dvec4(normals[i]);
                        type = 0;
                        intr.from_index = 1.0f;
                        intr.in_index = 0.0f;
                    }else if(dist < intr.t){
                        intr.t = dist;
                        intr.hit = true;
                        intr.point = point;
                        intr.normal = glm::dvec4(normals[i]);
                        type = 0;
                        intr.from_index = 1.0f;
                        intr.in_index = 0.0f;
                    }
                }
            }
        }
        if(r_success){
            if(r_dist > 0){
                glm::dvec4 point = r.origin + intr.t_ray.dir*r_dist;
           // std::cout<<glm::to_string(point)<<std::endl;
                if(point.x>-0.001 && point.x + m_pos.x < m_size+0.001 + m_pos.x &&
                   point.y>-0.001 && point.y + m_pos.y < m_size+0.001 + m_pos.y &&
                   point.z>-0.001 && point.z + m_pos.z < m_size+0.001 + m_pos.z){
                    if(intr.t_ray.origin.x > 0 && intr.t_ray.origin.x < m_size &&
                       intr.t_ray.origin.y > 0 && intr.t_ray.origin.y < m_size &&
                       intr.t_ray.origin.z > 0 && intr.t_ray.origin.z < m_size){
                        //std::cout<<dist<<std::endl;
                        //std::cout<<"cube inside pass"<<dist<<std::endl;
                        if(intr.hit == false){
                            intr.t = r_dist;
                            intr.hit = true;
                            intr.point = point;
                            intr.normal = -glm::dvec4(normals[i]);
                            type = 1;
                            intr.from_index = 0.0f;
                            intr.in_index = 1.0f;
                        }else if(r_dist < intr.t){
                            intr.t = r_dist;
                            intr.hit = true;
                            intr.point = point;
                            intr.normal = -glm::dvec4(normals[i]);
                            type = 1;
                            intr.from_index = 0.0f;
                            intr.in_index = 1.0f;
                        }
                    }
                }
            }
        }
    }
    //std::cout<<"cube end"<<std::endl;
    return intr;
}

Cylinder::Cylinder(){
    type = 0;
}

Cylinder::~Cylinder(){
}

Intersection Cylinder::intersect(const Ray & r){
    Intersection intr(r,0,false);
    double qdra_a = r.dir.x*r.dir.x + r.dir.z*r.dir.z;
    double qdra_b = 2*(r.origin.x*r.dir.x + r.origin.z*r.dir.z);
    double qdra_c = r.origin.x*r.origin.x + r.origin.z*r.origin.z - 1;

    double roots[2];
    int num_roots = quadraticRoots(qdra_a,qdra_b,qdra_c,roots);
    if(num_roots == 0){

    }else if(num_roots == 1){

    }else if(num_roots == 2){
        double t1,t2;

        if(roots[0] <= roots[1]){
            t1 = roots[0];
            t2 = roots[1];
        }else{
            t1 = roots[1];
            t2 = roots[0];
        }

        double y1 = r.origin.y + r.dir.y * t1;
        double y2 = r.origin.y + r.dir.y * t2;

        if((y1 < -1)&&(y2 >= -1)){
            //hit bottom cap
            if(t1 > 0 && t2 > 0){
                double k = t1 + (t2 - t1) * (y1 + 1)/(y1 - y2);
                if(k > 0){
                    intr.hit = true;
                    intr.t = k;
                    intr.normal = glm::dvec4(0,-1,0,0);
                    intr.point = r.origin + r.dir * intr.t;
                    type = 0;
                }
            }else if(t1 < 0 && t2 > 0){
                    //inside top cap
                    double k = t2 - t2 * (y2 - 1)/(y2 - r.origin.y);
                    if(k > 0){
                        intr.hit = true;
                        intr.t = k;
                        intr.normal = glm::dvec4(0,1,0,0);
                        intr.point = r.origin + r.dir * intr.t;
                        type = 0;
                    }
                    if(y2 < 1){
                        intr.hit = true;
                        intr.t = t2;
                        intr.point = r.origin + r.dir * intr.t;
                        intr.normal = -glm::normalize(glm::dvec4(intr.point.x,0.0,intr.point.z,0.0));
                        type = 1;
                    }
            }
        }else if(y1 > -1 && y1 < 1){
            if(t1 > 0 && t2 > 0){
                intr.hit = true;
                intr.t = t1;
                intr.point = r.origin + r.dir * intr.t;
                intr.normal = glm::normalize(glm::dvec4(intr.point.x,0.0,intr.point.z,0.0));
                type = 1;
            }else if(t1 < 0 && t2 > 0){
                if(y2 > 1){
                    //inside top cap
                    double k = t2 - t2 * (y2 - 1)/(y2 - r.origin.y);
                    if(k > 0){
                        intr.hit = true;
                        intr.t = k;
                        intr.normal = glm::dvec4(0,1,0,0);
                        intr.point = r.origin + r.dir * intr.t;
                        type = 0;
                    }
                }else if(y2 < -1){
                    //inside bottom cap
                    double k = t2 - t2 * (-1 - y2)/(r.origin.y - y2);
                    if(k > 0){
                        intr.hit = true;
                        intr.t = k;
                        intr.point = r.origin + r.dir * k;
                        intr.normal = glm::dvec4(0,-1,0,0);
                        type = 0;
                        }
                }else{
                    intr.hit = true;
                    intr.t = t2;
                    intr.point = r.origin + r.dir * intr.t;
                    intr.normal = glm::normalize(glm::dvec4(intr.point.x,0.0,intr.point.z,0.0));
                    type = 1;
                }

            }
        }else if(y1 > 1 && y2 <= 1){
            if(t2 > 0 && t1 > 0){
                //hit top cap
                double k = t1 + (t2 - t1) * (y1 - 1)/(y1 - y2);
                if(k > 0){
                    intr.hit = true;
                    intr.t = k;
                    intr.point = r.origin + r.dir * k;
                    intr.normal = glm::dvec4(0,1,0,0);
                    type = 0;
                }
            }else if(t2 > 0 && t1 < 0){
                    //inside bottom cap
                    double k = t2 - t2 * (-1 - y2)/(r.origin.y - y2);
                    if(k > 0){
                        intr.hit = true;
                        intr.t = k;
                        intr.point = r.origin + r.dir * k;
                        intr.normal = glm::dvec4(0,-1,0,0);
                        type = 0;
                        }
                    if(y2 > -1){
                        intr.hit = true;
                        intr.t = t2;
                        intr.point = r.origin + r.dir * intr.t;
                        intr.normal = -glm::normalize(glm::dvec4(intr.point.x,0.0,intr.point.z,0.0));
                        type = 1;
                    }
            }
        }
    }
    if(intr.hit){
        double in_x = intr.t_ray.origin.x;
        double in_y = intr.t_ray.origin.y;
        double in_z = intr.t_ray.origin.z;
        //std::cout<<"cylinder"<<std::endl;
        intr.from_index = 1.0f;
        intr.in_index = 0.0f;
        if(std::abs(in_y) <= 1.0 && (sqrt(in_x*in_x + in_z*in_z) <= 1)){
            //inside
            intr.normal = -intr.normal;
            intr.from_index = 0.0f;
            intr.in_index = 1.0f;
        }
    }
    return intr;
}

Cone::Cone(){
    type = 0;
}

Cone::~Cone(){
}

Intersection Cone::intersect(const Ray & r){
    Intersection intr(r,0,false);
    double qdra_a = r.dir.x*r.dir.x + r.dir.z*r.dir.z - r.dir.y*r.dir.y;
    double qdra_b = 2.0*(r.origin.x*r.dir.x + r.origin.z*r.dir.z - r.origin.y*r.dir.y);
    double qdra_c = r.origin.x*r.origin.x + r.origin.z*r.origin.z - r.origin.y*r.origin.y;

    double roots[2];
    int num_roots = quadraticRoots(qdra_a,qdra_b,qdra_c,roots);
    double k;
///////////////////////////////////////////////////////////////////////////////
    if(num_roots == 0){

    }else if(num_roots == 1){
        //not useful
        double t = roots[0];
        glm::vec4 p = r.origin + r.dir * t;
        if(p.y < 1 && p.y > 0){
            k = (1.0 - r.origin.y)/r.dir.y;
            intr.point = r.origin + r.dir*k;
            if(k > 0
                && (intr.point.x <= 1 && intr.point.x >= -1)
                && (intr.point.z <= 1 && intr.point.z >= -1)){
                intr.hit = true;
                intr.t = k;
                intr.normal = glm::dvec4(0.0, 1.0, 0.0, 0.0);
                type = 1;
                intr.from_index = 0.0f;
                intr.in_index = 1.0f;
            }
        }else if(t > 0){
            intr.hit = true;
            intr.t = t;
            intr.point = r.origin + r.dir*t;
            double xz_sqrt = std::sqrt(intr.point.x*intr.point.x + intr.point.z*intr.point.z);
            glm::vec3 p_x = {1.0,intr.point.x/xz_sqrt,0.0};
            glm::vec3 p_z = {0.0,intr.point.z/xz_sqrt,1.0};
            glm::vec3 n = glm::cross(p_x,p_z);
            intr.normal = glm::dvec4(glm::normalize(n),0.0);
            intr.from_index = 1.0f;
            intr.in_index = 0.0f;
            type = 0;

        }
    }else if(num_roots == 2){
        double t1,t2;
        glm::vec4 p1,p2;
        if(roots[0] <= roots[1]){
            t1 = roots[0];
            t2 = roots[1];
        }else{
            t1 = roots[1];
            t2 = roots[0];
        }

        p1 = r.origin + r.dir * t1;
        p2 = r.origin + r.dir * t2;


        if(p1.y < 0) {
            if(p2.y >= 0 && p2.y <= 1){
                if(t2 > 0){
                        intr.hit = true;
                        intr.t = t2;
                        intr.point = r.origin + r.dir*t2;
                        double xz_sqrt = std::sqrt(intr.point.x*intr.point.x + intr.point.z*intr.point.z);
                        glm::vec3 p_x = {1.0,intr.point.x/xz_sqrt,0.0};
                        glm::vec3 p_z = {0.0,intr.point.z/xz_sqrt,1.0};
                        glm::vec3 n = glm::cross(p_x,p_z);
                        intr.normal = glm::dvec4(glm::normalize(n),0.0);
                        intr.from_index = 1.0f;
                        intr.in_index = 0.0f;
                        type = 0;
                }
            }
        }else if(p1.y >= 0 && p1.y <= 1){
            if(t1 > 0){
                intr.hit = true;
                intr.t = t1;
                intr.point = r.origin + r.dir*t1;
                double xz_sqrt = std::sqrt(intr.point.x*intr.point.x + intr.point.z*intr.point.z);
                glm::vec3 p_x = {1.0,intr.point.x/xz_sqrt,0.0};
                glm::vec3 p_z = {0.0,intr.point.z/xz_sqrt,1.0};
                glm::vec3 n = glm::cross(p_x,p_z);
                intr.normal = glm::dvec4(glm::normalize(n),0.0);
                intr.from_index = 1.0f;
                intr.in_index = 0.0f;
                type = 0;
            }
            if(p2.y < 0){
                if(t2 > 0){
                    double k = (1.0 - r.origin.y)/r.dir.y;
                    intr.point = r.origin + r.dir*k;
                    if(k > 0
                       && (intr.point.x <= 1 && intr.point.x >= -1)
                       && (intr.point.z <= 1 && intr.point.z >= -1)){
                        intr.hit = true;
                        intr.t = k;
                        intr.normal = glm::dvec4(0.0, 1.0, 0.0, 0.0);
                        intr.from_index = 1.0f;
                        intr.in_index = 0.0f;
                        type = 1;
                       }
                }
            }
        }else if(p1.y > 1){
            if(p2.y <= 1 && p2.y >= 0){
                if(t2 > 0){
                    //top cap
                    double k = (1.0 - r.origin.y)/r.dir.y;
                    intr.point = r.origin + r.dir*k;
                    if(k > 0
                       && (intr.point.x <= 1 && intr.point.x >= -1)
                       && (intr.point.z <= 1 && intr.point.z >= -1)){
                        intr.hit = true;
                        intr.t = k;
                        intr.normal = glm::dvec4(0.0, 1.0, 0.0, 0.0);
                        intr.from_index = 1.0f;
                        intr.in_index = 0.0f;
                        type = 1;
                       }
                }
            }
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////////inside
    double in_x = intr.t_ray.origin.x;
    double in_y = intr.t_ray.origin.y;
    double in_z = intr.t_ray.origin.z;
    if((in_y > 0 && in_y < 1)&&(in_x*in_x + in_z*in_z < 0.99*in_y*in_y)){
        //std::cout<<"csalknfkaknfalkjdlknaf;lnasfoi;j"<<std::endl;
        if(num_roots == 0){
            //not gonna happen
        }else if(num_roots == 1){
            double t = roots[0];
            glm::vec4 p = r.origin + r.dir * t;
            if(t > 0){
                //cone inside
                intr.hit = true;
                intr.t = t;
                intr.point = r.origin + r.dir*t;
                double xz_sqrt = std::sqrt(intr.point.x*intr.point.x + intr.point.z*intr.point.z);
                glm::vec3 p_x = {1.0,intr.point.x/xz_sqrt,0.0};
                glm::vec3 p_z = {0.0,intr.point.z/xz_sqrt,1.0};
                glm::vec3 n = glm::cross(p_x,p_z);
                intr.normal = -glm::dvec4(glm::normalize(n),0.0);
                intr.from_index = 0.0f;
                intr.in_index = 1.0f;
                type = 0;
            }else if(t < 0){
                //cap
                k = (1.0 - r.origin.y)/r.dir.y;
                intr.point = r.origin + r.dir*k;
                if(k > 0
                    && (intr.point.x <= 1 && intr.point.x >= -1)
                    && (intr.point.z <= 1 && intr.point.z >= -1)){
                    intr.hit = true;
                    intr.t = k;
                    intr.normal = glm::dvec4(0.0, -1.0, 0.0, 0.0);
                    type = 1;
                    intr.from_index = 0.0f;
                    intr.in_index = 1.0f;
                }
            }
        }else if(num_roots == 2){
            double t1,t2;
            glm::vec4 p1,p2;
            if(roots[0] <= roots[1]){
                t1 = roots[0];
                t2 = roots[1];
            }else{
                t1 = roots[1];
                t2 = roots[0];
            }
            p1 = r.origin + r.dir * t1;
            p2 = r.origin + r.dir * t2;
            if(p1.y < 0) {
                if(t1 < 0 && t2 < 0){
                    k = (1.0 - r.origin.y)/r.dir.y;
                    intr.point = r.origin + r.dir*k;
                    if(k > 0
                        && (intr.point.x <= 1 && intr.point.x >= -1)
                        && (intr.point.z <= 1 && intr.point.z >= -1)){
                        intr.hit = true;
                        intr.t = k;
                        intr.normal = glm::dvec4(0.0, -1.0, 0.0, 0.0);
                        type = 1;
                        intr.from_index = 0.0f;
                        intr.in_index = 1.0f;
                    }
                }
            }else if (p1.y >= 0 && p1.y <= 1){
                if(p2.y >= 0 && p2.y <= 1){
                    if(t1 < 0 && t2 > 0){
                        intr.hit = true;
                        intr.t = t2;
                        intr.point = r.origin + r.dir*t2;
                        double xz_sqrt = std::sqrt(intr.point.x*intr.point.x + intr.point.z*intr.point.z);
                        glm::vec3 p_x = {1.0,intr.point.x/xz_sqrt,0.0};
                        glm::vec3 p_z = {0.0,intr.point.z/xz_sqrt,1.0};
                        glm::vec3 n = glm::cross(p_x,p_z);
                        intr.normal = -glm::dvec4(glm::normalize(n),0.0);
                        intr.from_index = 0.0f;
                        intr.in_index = 1.0f;
                        type = 0;
                    }
                }
                if(p2.y < 0){
                    if(t1 < 0 && t2 < 0){
                        double k = (1.0 - r.origin.y)/r.dir.y;
                        intr.point = r.origin + r.dir*k;
                        if(k > 0
                           && (intr.point.x <= 1 && intr.point.x >= -1)
                           && (intr.point.z <= 1 && intr.point.z >= -1)){
                            intr.hit = true;
                            intr.t = k;
                            intr.normal = glm::dvec4(0.0, -1.0, 0.0, 0.0);
                            intr.from_index = 0.0f;
                            intr.in_index = 1.0f;
                            type = 1;
                           }
                    }else if(t1>0 && t2>0){
                        intr.hit = true;
                        intr.t = t1;
                        intr.point = r.origin + r.dir*t1;
                        double xz_sqrt = std::sqrt(intr.point.x*intr.point.x + intr.point.z*intr.point.z);
                        glm::vec3 p_x = {1.0,intr.point.x/xz_sqrt,0.0};
                        glm::vec3 p_z = {0.0,intr.point.z/xz_sqrt,1.0};
                        glm::vec3 n = glm::cross(p_x,p_z);
                        intr.normal = -glm::dvec4(glm::normalize(n),0.0);
                        intr.from_index = 0.0f;
                        intr.in_index = 1.0f;
                        type = 0;
                    }
                }


            }else if(p1.y > 1){
                if(t1 < 0 && t2 > 0){
                    intr.hit = true;
                    intr.t = t2;
                    intr.point = r.origin + r.dir*t2;
                    double xz_sqrt = std::sqrt(intr.point.x*intr.point.x + intr.point.z*intr.point.z);
                    glm::vec3 p_x = {1.0,intr.point.x/xz_sqrt,0.0};
                    glm::vec3 p_z = {0.0,intr.point.z/xz_sqrt,1.0};
                    glm::vec3 n = glm::cross(p_x,p_z);
                    intr.normal = -glm::dvec4(glm::normalize(n),0.0);
                    intr.from_index = 0.0f;
                    intr.in_index = 1.0f;
                    type = 0;
                }
            }
        }
    }
    return intr;
}
