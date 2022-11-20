#include "window.hpp"

#include <unordered_map>

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "astro.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "astro.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif
  // SUN
  sun.create(m_program, assetsPath, 0.5f, {0.0f, 0.0f, 0.0f},
             {1.0f, 1.0f, 0.0f, 1.0f}, 0.0f);

  // MERCURY
  mercury.create(m_program, assetsPath, 0.003f, {2.5f, 0.0f, 0.0f},
                 {0.5f, 0.5f, 0.5f, 1.0f}, 0.8f);

  // VENUS
  venus.create(m_program, assetsPath, 0.009f, {3.5f, 0.0f, 0.0f},
               {1.0f, 0.8f, 0.4f, 1.0f}, 0.7f);

  // EARTH
  earth.create(m_program, assetsPath, 0.01f, {4.5f, 0.0f, 0.0f},
               {0.0f, 0.0f, 1.0f, 0.8f}, 0.6f);

  // MARS
  mars.create(m_program, assetsPath, 0.006f, {5.5f, 0.0f, 0.0f},
              {1.0f, 0.0f, 0.0f, 0.7f}, 0.5f);

  // JUPITER
  jupiter.create(m_program, assetsPath, 0.2f, {6.5f, 0.0f, 0.0f},
                 {0.8f, 0.4f, 0.0f, 1.0f}, 0.4f);

  // SATURN
  saturn.create(m_program, assetsPath, 0.15f, {7.5f, 0.0f, 0.0f},
                {0.8f, 0.6f, 0.4f, 1.0f}, 0.3f);

  // URANUS
  uranus.create(m_program, assetsPath, 0.1f, {8.5f, 0.0f, 0.0f},
                {0.6f, 0.8f, 0.8f, 0.5f}, 0.2f);

  // NEPTUNE
  neptune.create(m_program, assetsPath, 0.11f, {9.5f, 0.0f, 0.0f},
                 {0.0f, 0.4f, 0.8f, 1.0f}, 0.1f);

  // JUPITER MOON
  jupiterMoon.create(m_program, assetsPath, 0.05f, jupiter,
                     {0.8f, 0.8f, 0.8f, 1.0f}, 3.0f, 0.7f);

  // URANUS MOON
  uranusMoon.create(m_program, assetsPath, 0.03f, uranus,
                    {0.8f, 0.8f, 0.8f, 1.0f}, 6.0f, 0.5f);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  sun.paint();
  mercury.paint();
  venus.paint();
  earth.paint();
  mars.paint();
  jupiter.paint();
  saturn.paint();
  uranus.paint();
  neptune.paint();

  jupiterMoon.paint();
  uranusMoon.paint();

  abcg::glUseProgram(0);
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
  sun.destroy();
  mercury.destroy();
  venus.destroy();
  earth.destroy();
  mars.destroy();
  jupiter.destroy();
  saturn.destroy();
  uranus.destroy();
  neptune.destroy();

  jupiterMoon.destroy();
  uranusMoon.destroy();
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  mercury.update();
  venus.update();
  earth.update();
  mars.update();
  jupiter.update();
  saturn.update();
  uranus.update();
  neptune.update();

  jupiterMoon.update(jupiter);
  uranusMoon.update(uranus);

  // Update camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}