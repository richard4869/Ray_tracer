#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd,
                const glm::vec3& ks,
                double shininess,
                double reflectivity,
                double refraction,
                double rfr_index,
                double glossiness,
                double pd,
                double ps,
                int vol_s,
                int negative);
  virtual ~PhongMaterial();


  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
  double m_reflectivity;
  double m_refraction;
  double m_rfr_index;
  double m_glossiness;
  double m_pd;
  double m_ps;
  int m_vol_s;
  int m_negative;
};
