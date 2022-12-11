#include "window.hpp"

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
  onDestroy();

  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  auto const path{assetsPath + "shaders/texture"};
  m_program = abcg::createOpenGLProgram(
      {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
       {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}});

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");

  // SUN
  sun.create(assetsPath + "orbs/sun.obj",
             assetsPath + "maps/13913_Sun_diff.jpg", m_program, 0.5f,
             {0.0f, 0.0f, 0.0f}, 0.0f);

  // MERCURY
  mercury.create(assetsPath + "orbs/mercury.obj",
                 assetsPath + "maps/mercury-texture.jpeg", m_program, 0.003f,
                 {2.5f, 0.0f, 0.0f}, 0.8f);

  // VENUS
  venus.create(assetsPath + "orbs/venus.obj",
               assetsPath + "maps/venus-texture.jpg", m_program, 0.009f,
               {3.5f, 0.0f, 0.0f}, 0.7f);

  // EARTH
  earth.create(assetsPath + "orbs/Earth.obj", assetsPath + "maps/earth.jpg",
               m_program, 0.01f, {4.5f, 0.0f, 0.0f}, 0.6f);

  // MARS
  mars.create(assetsPath + "orbs/mars.obj",
              assetsPath + "maps/mars-texture.jpg", m_program, 0.006f,
              {5.5f, 0.0f, 0.0f}, 0.5f);

  // JUPITER
  jupiter.create(assetsPath + "orbs/jupiter.obj",
                 assetsPath + "maps/jupiter-texture.jpg", m_program, 0.2f,
                 {6.5f, 0.0f, 0.0f}, 0.4f);

  // SATURN
  saturn.create(assetsPath + "orbs/saturn.obj",
                assetsPath + "maps/Saturn_diff.jpg", m_program, 0.15f,
                {7.5f, 0.0f, 0.0f}, 0.3f);

  // URANUS
  uranus.create(assetsPath + "orbs/uranus.obj",
                assetsPath + "maps/13907_Uranus_planet_diff.jpg", m_program,
                0.1f, {8.5f, 0.0f, 0.0f}, 0.2f);

  // NEPTUNE
  neptune.create(assetsPath + "orbs/neptune.obj",
                 assetsPath + "maps/13908_Neptune_planet_diff.jpg", m_program,
                 0.11f, {9.5f, 0.0f, 0.0f}, 0.1f);

  // JUPITER MOON
  jupiterMoon.create(assetsPath + "orbs/moon.obj",
                 assetsPath + "maps/moon-texture.jpeg", m_program, 0.05f, jupiter, 3.0f, 0.7f);

  // URANUS MOON
  uranusMoon.create(assetsPath + "orbs/moon.obj",
                 assetsPath + "maps/moon-texture.jpeg", m_program, 0.03f, uranus, 6.0f, 0.5f);

  m_mappingMode = 3; // "From mesh" option
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Use currently selected program
  abcg::glUseProgram(m_program);

  auto const diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  auto const mappingModeLoc{
      abcg::glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  sun.paint(sun.getNumTriangles());
  mercury.paint(mercury.getNumTriangles());
  venus.paint(venus.getNumTriangles());
  earth.paint(earth.getNumTriangles());
  mars.paint(mars.getNumTriangles());
  jupiter.paint(jupiter.getNumTriangles());
  saturn.paint(saturn.getNumTriangles());
  uranus.paint(uranus.getNumTriangles());
  neptune.paint(neptune.getNumTriangles());

  jupiterMoon.paint(jupiterMoon.getNumTriangles());
  uranusMoon.paint(uranusMoon.getNumTriangles());

  auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                    gsl::narrow<float>(m_viewportSize.y)};
  m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5.0f);

  abcg::glUseProgram(0);
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

  // m_viewMatrix =
  //     glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
  //                 glm::vec3(0.0f, 1.0f, 0.0f));

  // Update camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
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

  abcg::glDeleteProgram(m_program);
}