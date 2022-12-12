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
  * [X] Luas;
  * [ ] Aneis de Saturno;
  * [ ] Universo Estrelado;
  * [ ] Asteroides;

- Este projeto tem o proposito de simular:
  * [X] O comportamento de rotação dos planetas em relação ao Sol;
  * [X] O comportamento de rotação das luas dos planetas;
  * [X] A ideia de escala entre os tamanhos dos elementos;
  * [X] Mostrar graficamente os elementos do Sistema Solar em uma perspectiva 3D;
  * [X] Explorar os conceitos de texturização do Sistema Solar;
  * [ ] Explorar os conceitos de iluminação do Sistema Solar;

- Os arquivos ```.obj```, ```.mtl``` e ```.jpg```, que foram utilizados para representar todos os planetas e o Sol; e suas respectivas texturas, estam disponíveis em [neste repo](https://github.com/keynekassapa13/solar-system/tree/master/COSC3000/resources/models), exceto pela [textura da Terra](https://www.solarsystemscope.com/textures/);

- Os arquivos ```texture.frag``` e ```texture.vert``` são idênticos ao Vertex Shader e Fragment Shader do projeto [Viewer4](https://hbatagelo.github.io/cg/viewer4.html) visto em sala de aula;

 - A definição da classe ```window.hpp``` permanece similar ao que estudamos na sala de aula, com o adendo de algumas variaveis novas como as referente aos planetas, Sol e luas dos planetas:
  ```c++
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

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  GLuint m_program{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  void loadModel(std::string_view path);
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
 - Todos os planetas, Sol e luas são criados na função ```void Window::onCreate()```; sendo passado parametros como: onde encontrar os arquivos ```.obj``` e ```.mtl```, local onde estâo os arquivos referente as texturas dos objetos, escala dos objetos carregados e suas respectivas velocidades angulares; na classe ```window.cpp```:
  ```c++
  / SUN
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
  ```
 - Na função ```void Window::onPaint()``` da classe ```window.cpp``` sao chamadas as funções ```paint()``` dos planetas, Sol e luas para pintar com texturização os objetos referente a eles:
  ```c++
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
  ```
 - Na função ```void Window::onDestroy()``` da classe ```window.cpp``` sao chamadas as funções ```destroy()``` dos planetas, Sol e luas para destruir os objetos rederizados referente a eles:
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

  jupiterMoon.destroy();
  uranusMoon.destroy();
  ```
 - Na função ```void Window::onUpdate()``` da classe ```window.cpp``` sao chamadas as funções ```update()``` dos planetas, Sol e luas para atualizar a localização dos objetos referente a eles:
  ```c++
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
  ```
 - A câmera é atualizada nesse mesmo bloco:
  ```c++
  // Update camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  ```
  
  #### Classe Planet
- A classe ```Planet``` foi criada para abstrair a configuração dos planetas e as funções referentes aos planetas. 
- O arquivo ```planet.hpp``` contém a estrutura da classe ```Planet```, que possui essencialmente as funções ```create()```, ```paint()```, ```destroy()``` e ```update()```, e os atributos tamanho (m_scale), posição (m_translation), velocidade (m_velocity), velocidade angular (m_angularVelocity) e escala (m_sceneScale).

```c++
#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Planet {
public:
  void create(std::string_view path, std::string_view pathTexture,
              GLuint program, float size, glm::vec3 position,
              float angularVelocity);
  void paint(int numTriangles = -1) const;
  void update();
  void destroy();

  float m_scale{};
  glm::vec3 m_translation{};
  glm::vec3 m_velocity{};
  float m_angularVelocity{};
  float m_sceneScale{0.2};

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLuint m_diffuseTexture{};

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_modelMatrixLoc{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

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
  ```
 
 - O arquivo ```planet.cpp``` implementa as funções da classe ```Planet```. A função ```create()``` recebe as características do planeta como parâmetros e cria uma instância da classe com as características recebidas, o modelo do objeto e suas propriedades e carrega a sua texturização. Gera o VBO e o EBO, cria o VAO, e atribui os vértices. A geração do VBO, EBO e VAO ocorre na função ```void Planet::createBuffers()```. Essa classe foi criada utilizando como base a classe ```model.cpp``` do projeto [Viewer4](https://hbatagelo.github.io/cg/viewer4.html) visto em sala de aula:
  ```c++

void Planet::create(std::string_view path, std::string_view pathTexture,
                    GLuint program, float size, glm::vec3 position,
                    float angularVelocity) {

  m_program = program;

  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");

  m_translation = position;
  m_scale = size;
  m_velocity = glm::vec3(0.5, 0, 0);
  m_angularVelocity = angularVelocity;

  if (!std::filesystem::exists(pathTexture))
    return;

  abcg::glDeleteTextures(1, &m_diffuseTexture);
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = pathTexture});

  auto const basePath{std::filesystem::path{path}.parent_path().string() + "/"};

  tinyobj::ObjReaderConfig readerConfig;
  readerConfig.mtl_search_path = basePath; // Path to material files

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data(), readerConfig)) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attrib{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};
  auto const &materials{reader.GetMaterials()};

  m_vertices.clear();
  m_indices.clear();

  m_hasNormals = false;
  m_hasTexCoords = false;

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Position
      auto const startIndex{3 * index.vertex_index};
      glm::vec3 position{attrib.vertices.at(startIndex + 0),
                         attrib.vertices.at(startIndex + 1),
                         attrib.vertices.at(startIndex + 2)};

      // Normal
      glm::vec3 normal{};
      if (index.normal_index >= 0) {
        m_hasNormals = true;
        auto const normalStartIndex{3 * index.normal_index};
        normal = {attrib.normals.at(normalStartIndex + 0),
                  attrib.normals.at(normalStartIndex + 1),
                  attrib.normals.at(normalStartIndex + 2)};
      }

      // Texture coordinates
      glm::vec2 texCoord{};
      if (index.texcoord_index >= 0) {
        m_hasTexCoords = true;
        auto const texCoordsStartIndex{2 * index.texcoord_index};
        texCoord = {attrib.texcoords.at(texCoordsStartIndex + 0),
                    attrib.texcoords.at(texCoordsStartIndex + 1)};
      }

      Vertex const vertex{
          .position = position, .normal = normal, .texCoord = texCoord};

      // If hash doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }

  // Use properties of first material, if available
  if (!materials.empty()) {
    auto const &mat{materials.at(0)}; // First material
    m_Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2], 1};
    m_Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1};
    m_Ks = {mat.specular[0], mat.specular[1], mat.specular[2], 1};
    m_shininess = mat.shininess;

    if (!mat.diffuse_texname.empty()) {
      abcg::glDeleteTextures(1, &m_diffuseTexture);
      m_diffuseTexture =
          abcg::loadOpenGLTexture({.path = basePath + mat.diffuse_texname});
    } else {
      // Default values
      m_Ka = {0.1f, 0.1f, 0.1f, 1.0f};
      m_Kd = {0.7f, 0.7f, 0.7f, 1.0f};
      m_Ks = {1.0f, 1.0f, 1.0f, 1.0f};
      m_shininess = 25.0f;
    }

    if (!m_hasNormals) {
      computeNormals();
    }

    createBuffers();

    // Release previous VAO
    abcg::glDeleteVertexArrays(1, &m_VAO);

    // Create VAO
    abcg::glGenVertexArrays(1, &m_VAO);
    abcg::glBindVertexArray(m_VAO);

    // Bind EBO and VBO
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Bind vertex attributes
    auto const positionAttribute{
        abcg::glGetAttribLocation(program, "inPosition")};
    if (positionAttribute >= 0) {
      abcg::glEnableVertexAttribArray(positionAttribute);
      abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex), nullptr);
    }

    auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
    if (normalAttribute >= 0) {
      abcg::glEnableVertexAttribArray(normalAttribute);
      auto const offset{offsetof(Vertex, normal)};
      abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  reinterpret_cast<void *>(offset));
    }

    auto const texCoordAttribute{
        abcg::glGetAttribLocation(program, "inTexCoord")};
    if (texCoordAttribute >= 0) {
      abcg::glEnableVertexAttribArray(texCoordAttribute);
      auto const offset{offsetof(Vertex, texCoord)};
      abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  reinterpret_cast<void *>(offset));
    }

    // End of binding
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
    abcg::glBindVertexArray(0);
  }
}
```

- A função ```paint()``` é responsável por desenhar a instância do planeta na posição, escala e textura correspondentes.
 ```c++
void Planet::paint(int numTriangles) const {
  abcg::glUseProgram(m_program);

  auto const shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};

  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  abcg::glBindVertexArray(m_VAO);

  glm::mat4 model{1.0f};
  glm::vec3 translate = {m_sceneScale * m_translation.x,
                         m_sceneScale * m_translation.y,
                         m_sceneScale * m_translation.z};
  model = glm::translate(model, translate);
  model = glm::scale(model, glm::vec3(m_sceneScale * m_scale));

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  auto const numIndices{(numTriangles < 0) ? m_indices.size()
                                           : numTriangles * 3};

  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}
  ```
- Para simular o efeito de rotação dos planetas em relação ao Sol utilizamos dos conceitos de [Movimento Circular Uniforme](https://wp.ufpel.edu.br/diehl/files/2018/08/FGA_aula6.pdf) aplicando uma velocidade angular constante parametrizada para cada planeta, e o conceito de [Transformações](https://hbatagelo.github.io/cg/transforms.html) para obter as coordenadas cartesianas atualizadas no movimento de Rotação dos planetas na função ```update()``.
 ```c++
 void Planet::update() {
  const double PI = 3.141592653589793238463;
  float thetha = PI * m_angularVelocity / 180;
  float x = m_translation.x * cos(thetha) - m_translation.z * sin(thetha);
  float z = m_translation.z * cos(thetha) + m_translation.x * sin(thetha);

  m_translation = glm::vec3{x, 0.0f, z};
}
 ```
 
### Classe Moon

- A classe ```Moon``` foi criada para abstrair a configuração das luas e as funções referentes as luas. 
- O arquivo ```moon.hpp``` contém a estrutura da classe ```Moon```, que possui essencialmente as funções ```create()```, ```paint()```, ```destroy()``` e ```update(const Planet &planet)```, e os atributos tamanho (m_scale), posição (m_translation), velocidade (m_velocity), velocidade angular (m_angularVelocity), escala (m_sceneScale) e centro do planeta(m_planetCenter).

 ```c++
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
 ```
 
- O arquivo ```moon.cpp``` implementa as funções da classe ```Moon```. A função ```create()``` recebe as características da lua como parâmetros e cria uma instância da classe com as características recebidas, o modelo do objeto e suas proriedades e sua texturização. Gera o VBO e o EBO, cria o VAO, e atribui os vértices.  A geração do VBO, EBO e VAO ocorre na função ```void Moon::createBuffers()```. Essa classe foi criada utilizando como base a classe ```model.cpp``` do projeto [Viewer4](https://hbatagelo.github.io/cg/viewer4.html) visto em sala de aula:
  ```c++
  void Planet::create(std::string_view path, std::string_view pathTexture,
                    GLuint program, float size, glm::vec3 position,
                    float angularVelocity) {

  m_program = program;

  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");

  m_translation = position;
  m_scale = size;
  m_velocity = glm::vec3(0.5, 0, 0);
  m_angularVelocity = angularVelocity;

  if (!std::filesystem::exists(pathTexture))
    return;

  abcg::glDeleteTextures(1, &m_diffuseTexture);
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = pathTexture});

  auto const basePath{std::filesystem::path{path}.parent_path().string() + "/"};

  tinyobj::ObjReaderConfig readerConfig;
  readerConfig.mtl_search_path = basePath; // Path to material files

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data(), readerConfig)) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attrib{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};
  auto const &materials{reader.GetMaterials()};

  m_vertices.clear();
  m_indices.clear();

  m_hasNormals = false;
  m_hasTexCoords = false;

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Position
      auto const startIndex{3 * index.vertex_index};
      glm::vec3 position{attrib.vertices.at(startIndex + 0),
                         attrib.vertices.at(startIndex + 1),
                         attrib.vertices.at(startIndex + 2)};

      // Normal
      glm::vec3 normal{};
      if (index.normal_index >= 0) {
        m_hasNormals = true;
        auto const normalStartIndex{3 * index.normal_index};
        normal = {attrib.normals.at(normalStartIndex + 0),
                  attrib.normals.at(normalStartIndex + 1),
                  attrib.normals.at(normalStartIndex + 2)};
      }

      // Texture coordinates
      glm::vec2 texCoord{};
      if (index.texcoord_index >= 0) {
        m_hasTexCoords = true;
        auto const texCoordsStartIndex{2 * index.texcoord_index};
        texCoord = {attrib.texcoords.at(texCoordsStartIndex + 0),
                    attrib.texcoords.at(texCoordsStartIndex + 1)};
      }

      Vertex const vertex{
          .position = position, .normal = normal, .texCoord = texCoord};

      // If hash doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }

  // Use properties of first material, if available
  if (!materials.empty()) {
    auto const &mat{materials.at(0)}; // First material
    m_Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2], 1};
    m_Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1};
    m_Ks = {mat.specular[0], mat.specular[1], mat.specular[2], 1};
    m_shininess = mat.shininess;

    if (!mat.diffuse_texname.empty()) {
      abcg::glDeleteTextures(1, &m_diffuseTexture);
      m_diffuseTexture =
          abcg::loadOpenGLTexture({.path = basePath + mat.diffuse_texname});
    } else {
      // Default values
      m_Ka = {0.1f, 0.1f, 0.1f, 1.0f};
      m_Kd = {0.7f, 0.7f, 0.7f, 1.0f};
      m_Ks = {1.0f, 1.0f, 1.0f, 1.0f};
      m_shininess = 25.0f;
    }

    if (!m_hasNormals) {
      computeNormals();
    }

    createBuffers();

    // Release previous VAO
    abcg::glDeleteVertexArrays(1, &m_VAO);

    // Create VAO
    abcg::glGenVertexArrays(1, &m_VAO);
    abcg::glBindVertexArray(m_VAO);

    // Bind EBO and VBO
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Bind vertex attributes
    auto const positionAttribute{
        abcg::glGetAttribLocation(program, "inPosition")};
    if (positionAttribute >= 0) {
      abcg::glEnableVertexAttribArray(positionAttribute);
      abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex), nullptr);
    }

    auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
    if (normalAttribute >= 0) {
      abcg::glEnableVertexAttribArray(normalAttribute);
      auto const offset{offsetof(Vertex, normal)};
      abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  reinterpret_cast<void *>(offset));
    }

    auto const texCoordAttribute{
        abcg::glGetAttribLocation(program, "inTexCoord")};
    if (texCoordAttribute >= 0) {
      abcg::glEnableVertexAttribArray(texCoordAttribute);
      auto const offset{offsetof(Vertex, texCoord)};
      abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  reinterpret_cast<void *>(offset));
    }

    // End of binding
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
    abcg::glBindVertexArray(0);
  }
}

- A função ```paint()``` é responsável por desenhar a instância da lua na posição, escala e textura correspondente.
 ```c++
 void Planet::paint(int numTriangles) const {
  abcg::glUseProgram(m_program);

  auto const shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};

  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  abcg::glBindVertexArray(m_VAO);

  glm::mat4 model{1.0f};
  glm::vec3 translate = {m_sceneScale * m_translation.x,
                         m_sceneScale * m_translation.y,
                         m_sceneScale * m_translation.z};
  model = glm::translate(model, translate);
  model = glm::scale(model, glm::vec3(m_sceneScale * m_scale));

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  auto const numIndices{(numTriangles < 0) ? m_indices.size()
                                           : numTriangles * 3};

  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}
 ```
 
- A função ```void Moon::update(const Planet &planet)``` é responsável por atualizar a posição no espaço, para isso, ela pega a posição da lua no espaço, ve a posição da lua em relação ao planeta e atualiza as coordenadas polares da lua em relação ao planeta no espaço:

 ```c++
 void Planet::update() {
  const double PI = 3.141592653589793238463;
  float thetha = PI * m_angularVelocity / 180;
  float x = m_translation.x * cos(thetha) - m_translation.z * sin(thetha);
  float z = m_translation.z * cos(thetha) + m_translation.x * sin(thetha);

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








