#include "moon.hpp"
#include "planet.hpp"

#include <filesystem>
#include <cstdio>
#include <glm/gtx/fast_trigonometry.hpp>
#include <unordered_map>

// Explicit specialization of std::hash for VertexM
template <> struct std::hash<VertexM> {
  size_t operator()(VertexM const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    auto const h3{std::hash<glm::vec2>()(vertex.texCoord)};
    return abcg::hashCombine(h1, h2, h3);
  }
};

void Moon::create(std::string_view path, std::string_view pathTexture,
                  GLuint program, float size,
                  Planet &planet, float angularVelocity, float radius) {
  destroy();

  m_program = program;

  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");

  m_planetCenter = planet.m_translation;
  m_translation = {m_planetCenter.x + radius, m_planetCenter.y,
                   m_planetCenter.z};
  m_scale = size;
  m_velocity = glm::vec3(0.3, 0, 0);
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

  // A key:value map with key=VertexM and value=index
  std::unordered_map<VertexM, GLuint> hash{};

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

      VertexM const vertex{
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
                                  sizeof(VertexM), nullptr);
    }

    auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
    if (normalAttribute >= 0) {
      abcg::glEnableVertexAttribArray(normalAttribute);
      auto const offset{offsetof(VertexM, normal)};
      abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(VertexM),
                                  reinterpret_cast<void *>(offset));
    }

    auto const texCoordAttribute{
        abcg::glGetAttribLocation(program, "inTexCoord")};
    if (texCoordAttribute >= 0) {
      abcg::glEnableVertexAttribArray(texCoordAttribute);
      auto const offset{offsetof(VertexM, texCoord)};
      abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(VertexM),
                                  reinterpret_cast<void *>(offset));
    }

    // End of binding
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
    abcg::glBindVertexArray(0);
  }
}

void Moon::paint(int numTriangles) const {
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

void Moon::destroy() {
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Moon::update(const Planet &planet) {
  const double PI = 3.141592653589793238463;
  double thetha = PI * m_angularVelocity / 180;
  float xMoon = m_translation.x - m_planetCenter.x;
  float zMoon = m_translation.z - m_planetCenter.z;
  float x = xMoon * cos(thetha) - zMoon * sin(thetha);
  float z = zMoon * cos(thetha) + xMoon * sin(thetha);
  m_planetCenter = planet.m_translation;

  m_translation = {m_planetCenter.x + x, 0.0f, m_planetCenter.z + z};
}

void Moon::computeNormals() {
  // Clear previous vertex normals
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::vec3(0.0f);
  }

  // Compute face normals
  for (auto const offset : iter::range(0UL, m_indices.size(), 3UL)) {
    // Get face vertices
    auto &a{m_vertices.at(m_indices.at(offset + 0))};
    auto &b{m_vertices.at(m_indices.at(offset + 1))};
    auto &c{m_vertices.at(m_indices.at(offset + 2))};

    // Compute normal
    auto const edge1{b.position - a.position};
    auto const edge2{c.position - b.position};
    auto const normal{glm::cross(edge1, edge2)};

    // Accumulate on vertices
    a.normal += normal;
    b.normal += normal;
    c.normal += normal;
  }

  // Normalize
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::normalize(vertex.normal);
  }

  m_hasNormals = true;
}

void Moon::createBuffers() {
  // Delete previous buffers
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}