#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"
#include "planet.hpp"
#include "moon.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  Planet sun;
  Planet mercury;
  Planet venus;
  Planet earth;
  Planet mars;
  Planet jupiter;
  Planet saturn;
  Planet uranus;
  Planet neptune;

  Moon jupiterMoon;
  Moon uranusMoon;

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};

  glm::mat4 m_projMatrix{1.0f};

  GLuint m_program{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};
};

#endif