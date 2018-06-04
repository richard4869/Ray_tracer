#include "GeometryNode.hpp"


//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat, Texture *texture)
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
	, m_texture(texture)
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

void GeometryNode::setTexture( Texture *texture ){
    m_texture = texture;
}

glm::vec3 GeometryNode::get_colour(glm::dvec4 point){
    glm::vec3 colour;
    if(m_texture == nullptr){
        PhongMaterial* m = (PhongMaterial*)m_material;
        colour = m->m_kd;
    }else{
        //std::cout<<"Gget_colour texture"<<std::endl;
        double u,v;
        glm::vec3 vp = glm::normalize(glm::vec3(point));
        glm::vec3 vn = {0.0,1.0,0.0};
        glm::vec3 ve = {1.0,0.0,0.0};
        double phi = acos(-glm::dot(vn,vp));
        v = phi/M_PI;

        double theta = ( acos( glm::dot(vp,ve)/sin(phi)) )/(2 * M_PI);
        if( glm::dot(glm::cross(vn,ve), vp) > 0) u = theta;
        else u = 1.0 - theta;
        colour = m_texture->get_colour(u,v);
    }
    return colour;
}

Intersection GeometryNode::intersect(const Ray & r){
    glm::vec4 c_origin=invtrans*r.origin;
    glm::vec4 c_dir=invtrans*r.dir;
    Ray c_ray=Ray(c_origin,c_dir);
/////////////////////////////////////////////////////////////////primitive
    Intersection n_intr = m_primitive->intersect(c_ray);
    if(n_intr.hit){
        n_intr.node = this;
        n_intr.material = (PhongMaterial*)this->m_material;
    }
/////////////////////////////////////////////////////////////////loop children


    for(auto child : children){
        Intersection c_intr = child->intersect(c_ray);
        if(c_intr.hit){
            if(n_intr.hit==false||c_intr.t<n_intr.t){
                assert(c_intr.t>0);
                n_intr = c_intr;                                                //renew intersection
            }
        }
    }
/////////////////////////////////////////////////////////////////go back

        if(n_intr.hit){
            if(n_intr.from_index == 1.0f) {
                n_intr.in_index = n_intr.material->m_rfr_index;
            }else{
                n_intr.from_index = n_intr.material->m_rfr_index;
            }
            glm::vec3 norm = glm::vec3(n_intr.normal);
            glm::mat3 inv = glm::mat3(invtrans);
            n_intr.normal = glm::vec4(glm::transpose(inv)*norm,0);
            n_intr.normal = glm::normalize(n_intr.normal);
            n_intr.t_ray.origin = trans*n_intr.t_ray.origin;
            n_intr.t_ray.dir = trans*n_intr.t_ray.dir;
        }
        //std::cout<<"Geometrynode"<<n_intr.hit<<" "<<n_intr.t<<std::endl;
    return n_intr;
}
