#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "Texture.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim,
		Material *mat = nullptr, Texture *texture = nullptr);

	void setMaterial( Material *material );
	void setTexture(Texture *texture);

	Material *m_material;
	Primitive *m_primitive;
	Texture *m_texture;

	virtual Intersection intersect(const Ray & r);
    glm::vec3 get_colour(glm::dvec4 point);
};
