#include <iostream>
#include <fstream>
#include <math.h>
#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
    type = 1;
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;
    glm::dvec3 minimum,maximum;
    bool first_time=true;
    std::string n_fname;
    size_t found = fname.find("Assets/");
    if(found=std::string::npos){
        n_fname = "Assets/"+fname;
    }else{
        n_fname = fname;
    }
	std::ifstream ifs(n_fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			if(first_time){
                minimum.x=vx;
                minimum.y=vy;
                minimum.z=vz;
                maximum.x=vx;
                maximum.y=vy;
                maximum.z=vz;
                first_time = false;
			}else{
                minimum.x=std::min(vx,minimum.x);
                minimum.y=std::min(vy,minimum.y);
                minimum.z=std::min(vz,minimum.z);
                maximum.x=std::max(vx,maximum.x);
                maximum.y=std::max(vy,maximum.y);
                maximum.z=std::max(vz,maximum.z);
			}
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
	//bounding volume
	glm::dvec3 m_pos = (minimum+maximum)/2.0;
    std::cout<<glm::to_string(m_pos)<<std::endl;
	double radius = glm::length(maximum-m_pos);

	if(fname == "plane.obj"||fname == "Assets/plane.obj"){
        b_volume = NULL;
	}else{
        b_volume = new NonhierSphere(m_pos,radius);
	}
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*

  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

Intersection Mesh::intersect(const Ray & r){
    Intersection intr(r,0,false);
    glm::vec3 b_pos;
    if(b_volume != NULL){
        Intersection bounce = b_volume->intersect(r);
        if(!bounce.hit){
            return intr;
        }
    }
    for (int i = 0; i < m_faces.size();i++ ){
        glm::vec3 e = glm::vec3(r.origin);
        glm::vec3 d = glm::vec3(r.dir);

        glm::vec3 a = m_vertices[m_faces[i].v1];
        glm::vec3 b = m_vertices[m_faces[i].v2];
        glm::vec3 c = m_vertices[m_faces[i].v3];
        //std::cout<<"Mesh"<<i<<" "<<glm::to_string(a)<<" "<<glm::to_string(b)<<" "<<glm::to_string(c)<<" "<<std::endl;
        bool success =glm::intersectRayTriangle(e,d,a,b,c,b_pos);

        if(success && b_pos.z > 0){
            if(intr.hit == false){
                intr.t = b_pos.z;
                intr.hit = true;
                glm::vec3 norm= -glm::normalize(glm::cross(c-a,a-b));
                //std::cout<<"Mesh"<<i<<" "<<glm::to_string(norm)<<" "<<intr.t<<std::endl;
                intr.normal = -glm::vec4(norm,0);
                intr.point = intr.t_ray.origin + intr.t_ray.dir*intr.t;
            }else if(b_pos.z < intr.t){
                intr.t = b_pos.z;
                glm::vec3 norm= -glm::normalize(glm::cross(c-a,a-b));
                intr.normal = glm::vec4(norm,0);
                intr.point = intr.t_ray.origin + intr.t_ray.dir*intr.t;
            }
        }
    }
    intr.from_index = 1.0f;
    intr.in_index = 0.0f;
    if(glm::dot(glm::vec4(intr.normal),intr.t_ray.dir) > -0.001){
        intr.normal = -intr.normal;
        intr.from_index = 0.0f;
        intr.in_index = 1.0f;
    }
    return intr;
}
