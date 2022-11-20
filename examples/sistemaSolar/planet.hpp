#ifndef PLANET_HPP_
#define PLANET_HPP_

#include <glm/fwd.hpp>
#include <list>
#include <random>

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position;

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Planet {
public:
  void create(GLuint program, std::string assetsPath, float size,
              glm::vec3 position, glm::vec4 color, float angularVelocity);
  void paint();
  void destroy();
  void update();

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::vec4 m_color{};
  float m_scale{};
  glm::vec3 m_translation{};
  glm::vec3 m_velocity{};
  float m_angularVelocity{};
  float m_sceneScale{0.2};

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_modelMatrixLoc{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void loadModelFromFile(std::string_view path);
};

#endif