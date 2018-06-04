#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd,
    const glm::vec3& ks,
    double shininess,
    double reflectivity,
    double refraction,
    double rfr_index,
    double glossiness,
    double pd,
    double ps,
    int vol_s,
    int negative)
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_reflectivity(reflectivity)
	, m_refraction(refraction)
	, m_rfr_index(rfr_index)
	, m_glossiness(glossiness)
    , m_pd(pd)
    , m_ps(ps)
    , m_vol_s(vol_s)
    , m_negative(negative)
{}

PhongMaterial::~PhongMaterial()
{}
