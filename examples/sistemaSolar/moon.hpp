#ifndef MOON_HPP_
#define MOON_HPP_

#include <glm/fwd.hpp>
#include <list>
#include <random>

#include "abcgOpenGL.hpp"
#include "planet.hpp"

struct VertexM {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  friend bool operator==(VertexM const &, VertexM const &) = default;
};

class Moon {
public:
  void create(std::string_view path, std::string_view pathTexture,
              GLuint program, float size,
              Planet &planet, float angularVelocity, float radius);
  void paint(int numTriangles = -1) const;
  void destroy();
  void update(const Planet &planet);

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  float m_scale{};
  glm::vec3 m_translation{};
  glm::vec3 m_planetCenter{};
  glm::vec3 m_velocity{};
  float m_angularVelocity{};
  float m_sceneScale{0.2};

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_modelMatrixLoc{};

  std::vector<VertexM> m_vertices;
  std::vector<GLuint> m_indices;

  GLuint m_diffuseTexture{};

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  void computeNormals();
  void createBuffers();
  void standardize();
};

#endif