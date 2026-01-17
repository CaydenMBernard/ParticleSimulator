#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "renderer.h"
#include "camera.h"
#include "shader.h"
#include "spheremesh.h"
#include "texture.h"

#include <vector>
#include <memory>

Renderer::Renderer() {
    int width = 1200;
    int height = 900;
}

Renderer::~Renderer() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (textureID) glDeleteTextures(1, &textureID);
}

void Renderer::init() {
    // initialize sphere mesh
    createSphereMesh(sphereMesh, 24, 24);

    // initialize texture ID
    textureID = textureInit(*shader);

    // set shader to use
    shader = std::make_unique<Shader>("vertex.glsl", "fragment.glsl");
    shader->use();

    // generate and bind buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 sphereMesh.sphereVertices.size() * sizeof(float),
                 sphereMesh.sphereVertices.data(), 
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 sphereMesh.sphereIndices.size() * sizeof(unsigned int),
                 sphereMesh.sphereIndices.data(), 
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Renderer::renderScreen(const std::vector<Particle>& particles, const Camera& camera) {
    shader->use();

    shader->setVec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
    shader->setVec3("viewPos", camera.cameraPos);
    shader->setVec3("lightColor", glm::vec3(1.0f));

    shader->setFloat("ambientStrength", 0.05f);
    shader->setFloat("specularStrength", 1.0f);
    shader->setFloat("shininess", 128.0f);
}