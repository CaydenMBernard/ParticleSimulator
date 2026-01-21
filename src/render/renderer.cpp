#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/renderer.h"
#include "render/camera.h"
#include "render/shader.h"
#include "render/spheremesh.h"
#include "render/texture.h"

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
    // set shader to use
    shader = std::make_unique<Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
    shader->use();

    // initialize texture ID
    textureID = textureInit(*shader);

    // initialize sphere mesh
    createSphereMesh(sphereMesh, 24, 24);

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
    // use shader program
    shader->use();
    
    // set shader uniforms for lighting
    shader->setVec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
    shader->setVec3("viewPos", camera.Position);
    shader->setVec3("lightColor", glm::vec3(1.0f));

    // light properties
    shader->setFloat("ambientStrength", 0.05f);
    shader->setFloat("specularStrength", 1.0f);
    shader->setFloat("shininess", 128.0f);

    // set model, view, and projection matrices
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view;
    view = glm::lookAt(camera.Position,
                       camera.Position + camera.Front,
                       camera.Up);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            1200.0f / 900.0f,
                                            0.1f, 100.0f);
    
    // get uniform locations and set matrices
    int modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    int viewLoc = glGetUniformLocation(shader->ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projectionLoc = glGetUniformLocation(shader->ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < particles.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, particles[i].currPosition);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", model);

        glDrawElements(GL_TRIANGLES, sphereMesh.indexCount, GL_UNSIGNED_INT, 0);
    }
}