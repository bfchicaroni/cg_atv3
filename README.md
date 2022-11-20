# Sistema Solar

Este repositório se trata de um fork da biblioteca ABCG implementada pelo Dr. Harlen Batagelo, aqui foi implementado uma simulação do sistema solar para a fixação dos conceitos aprendidos na matéria MCTA008-17 Computação Gráfica. 
## Autores:
Nome: Beatriz Favini Chicaroni  
 RA 11201721608

Nome: Thais Amorim Souza  
 RA 11201810655

### Descrição do projeto

- O nosso sistema solar contem os seguintes elementos:
  * [X] Sol;
  * [X] Mercúrio;
  * [X] Vênus;
  * [X] Terra;
  * [X] Marte;
  * [X] Júpiter;
  * [X] Saturno;
  * [X] Urano;
  * [X] Netuno;
  * [ ] Luas;
  * [ ] Aneis de Saturno;
  * [ ] Universo Estrelado;
  * [ ] Asteroides;

- Este projeto tem o proposito de simular:
  * [X] O comportamento de rotação dos planetas em relação ao Sol;
  * [ ] O comportamento de rotação das luas dos planetas;
  * [X] A ideia de escala entre os tamanhos dos elementos;
  * [X] Mostrar graficamente os elementos do Sistema Solar em uma perspectiva 3D;
  * [ ] Explorar os conceitos de texturização do Sistema Solar;
  * [ ] Explorar os conceitos de iluminação do Sistema Solar;

- O objeto original ```sphere.obj```, que foi utilizado para representar todos os planetas e o Sol, está disponível [neste link](http://web.mit.edu/djwendel/www/weblogo/shapes/basic-shapes/sphere/sphere.obj) ;

- Os arquivos ```astros.frag``` e ```astros.vert``` são idênticos ao Vertex Shader e Fragment Shader do projeto [LookAt](https://hbatagelo.github.io/cg/lookat.html) visto em sala de aula;

 - A definição da classe ```window.hpp``` permanece similar ao que estudamos na sala de aula, com o adendo de algumas variaveis novas como as referente aos planetas e o Sol:
  ```c++
  #ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "planet.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_colorLocation{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};

  Planet sun;
  Planet mercury;
  Planet venus;
  Planet earth;
  Planet mars;
  Planet jupiter;
  Planet saturn;
  Planet uranus;
  Planet neptune;
};

#endif
  ```

- Para a parte da câmera virtual deste projeto, foi utilizada a câmera implementada no projeto [LookAt](https://hbatagelo.github.io/cg/lookat.html) incluindo os comandos de controle da câmera com o teclado. A classe câmera está implementada nos arquivos ```camera.hpp``` e ```camera.cpp```. O controle da câmera com o teclado está instanciado na classe ```Window.cpp``` nas linhas exibidas a seguir:
 ```c++
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
 ```
 - Todos os planetas e o Sol são criados na função ```void Window::onCreate()```; sendo passado parametros como escala do [sphere.obj](http://web.mit.edu/djwendel/www/weblogo/shapes/basic-shapes/sphere/sphere.obj), cores dos objetos e suas respectivas velocidades angulares, na classe ```window.cpp```:
  ```c++
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
  ```
 - Na função ```void Window::onPaint()``` da classe ```window.cpp``` sao chamadas as funções ```paint()``` dos planetas e Sol para pintar as esferas referente a eles:
  ```c++
  sun.paint();
  mercury.paint();
  venus.paint();
  earth.paint();
  mars.paint();
  jupiter.paint();
  saturn.paint();
  uranus.paint();
  neptune.paint();
  ```
 - Na função ```void Window::onDestroy()``` da classe ```window.cpp``` sao chamadas as funções ```destroy()``` dos planetas e Sol para destruir as esferas referente a eles:
  ```c++
  sun.destroy();
  mercury.destroy();
  venus.destroy();
  earth.destroy();
  mars.destroy();
  jupiter.destroy();
  saturn.destroy();
  uranus.destroy();
  neptune.destroy();
  ```
 - Na função ```void Window::onUpdate()``` da classe ```window.cpp``` sao chamadas as funções ```update()``` dos planetas e Sol para atualizar as esferas referente a eles:
  ```c++
  mercury.update();
  venus.update();
  earth.update();
  mars.update();
  jupiter.update();
  saturn.update();
  uranus.update();
  neptune.update();
  ```
 - A câmera é atualizada nesse mesmo bloco:
  ```c++
  // Update camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  ```
  
- As classes ```planet.hpp``` e ```planet.cpp``` foram criadas para abstrair a configuração dos planetas e as funções referentes ao planetas. O arquivo ```planet.hpp``` contém a estrutura da classe ```Planet```, que possui essencialmente as funções ```create()```, ```paint()```, ```destroy()``` e ```update()```, e os atributos cor (m_color), tamanho (m_scale), posição (m_translate), velocidade (m_velocity), velocidade angular (m_angularVelocity) e escala (m_sceneScale).


 - ```planet.hpp```:
  ```c++
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
  ```
 - ```planet.cpp```:
  ```c++
  #include "planet.hpp"

#include <cstdio>
#include <glm/gtx/fast_trigonometry.hpp>
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

void Planet::create(GLuint program, std::string assetsPath, float size,
                    glm::vec3 position, glm::vec4 color,
                    float angularVelocity) {
  destroy();

  m_program = program;

  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");

  m_translation = position;
  m_scale = size;
  m_color = color;
  m_velocity = glm::vec3(0.5, 0, 0);
  m_angularVelocity = angularVelocity;

  // Load model
  loadModelFromFile(assetsPath + "sphere.obj");

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Planet::loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }
}

void Planet::paint() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  glm::mat4 model{1.0f};
  glm::vec3 translate = {m_sceneScale * m_translation.x,
                         m_sceneScale * m_translation.y,
                         m_sceneScale * m_translation.z};
  model = glm::translate(model, translate);
  model = glm::scale(model, glm::vec3(m_sceneScale * m_scale));

  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLoc, m_color.r, m_color.g, m_color.b, m_color.a);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Planet::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Planet::update() {
  const double PI = 3.141592653589793238463;
  float x = m_translation.x * cos(PI * m_angularVelocity / 180) -
            m_translation.z * sin(PI * m_angularVelocity / 180);
  float z = m_translation.z * cos(PI * m_angularVelocity / 180) +
            m_translation.x * sin(PI * m_angularVelocity / 180);

  m_translation = glm::vec3{x, 0.0f, z};
}
  ```

- Para simular o efeito de rotação dos planetas em relação ao Sol utilizamos dos conceitos de [Movimento Circular Uniforme](https://wp.ufpel.edu.br/diehl/files/2018/08/FGA_aula6.pdf) aplicando uma velocidade angular constante parametrizada para cada planeta, e o conceito de [Transformações](https://hbatagelo.github.io/cg/transforms.html) para obter as coordenadas cartesianas atualizadas no movimento de Rotação dos planetas na função ```update()``` na classe ```planet.cpp```.
 ```c++
 void Planet::update() {
  const double PI = 3.141592653589793238463;
  float x = m_translation.x * cos(PI * m_angularVelocity / 180) -
            m_translation.z * sin(PI * m_angularVelocity / 180);
  float z = m_translation.z * cos(PI * m_angularVelocity / 180) +
            m_translation.x * sin(PI * m_angularVelocity / 180);

  m_translation = glm::vec3{x, 0.0f, z};
}
 ```

- Ideia da escala entre os planetas:
 <img src="https://github.com/bfchicaroni/cg_atv3/blob/master/examples/sistemaSolar/assets/1200px-Size_planets_comparison.jpg" width="800" height="450">
 
- Para atualizar a taxa de atualização optamos por habilitar o modo Vsync (sincronização vertical) através da seguinte modificação em ```main.cpp```, geralmente limitando 60 Hz, para que tivessemos a mesma taxa tanto na versão desktop quanto na versão web:

 ```c++
 window.setOpenGLSettings(
        {.samples = 4, .vSync = true, .doubleBuffering = true});
 ```

- Acesse a [Simulação na versão Web](https://bfchicaroni.github.io/cg_atividades/sistemaSolar/).








