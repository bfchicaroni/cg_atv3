#ifndef MOON_HPP_
#define MOON_HPP_

#include <glm/fwd.hpp>
#include <list>
#include <random>

#include "abcgOpenGL.hpp"
#include "planet.hpp"

struct VertexM {
  glm::vec3 position;

  friend bool operator==(VertexM const &, VertexM const &) = default;
};

class Moon {
public:
  void create(GLuint program, std::string assetsPath, float size,
              Planet &planet, glm::vec4 color, float angularVelocity,
              float radius);
  void paint();
  void destroy();
  void update(const Planet &planet);

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::vec4 m_color{};
  float m_scale{};
  glm::vec3 m_translation{};
  glm::vec3 m_planetCenter{};
  glm::vec3 m_velocity{};
  float m_angularVelocity{};
  float m_sceneScale{0.2};

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_modelMatrixLoc{};

  std::vector<VertexM> m_vertices;
  std::vector<GLuint> m_indices;

  void loadModelFromFile(std::string_view path);
};

#endif