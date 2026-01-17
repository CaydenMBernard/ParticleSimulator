#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

#include <shader.h>
#include <camera.h>
#include <sim/particle.h>
#include <spheremesh.h>

#include <vector>
#include <memory>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void renderScreen(const std::vector<Particle>& particles, const Camera& camera);

private:
    // shader program
    std::unique_ptr<Shader> shader;

    // sphere mesh
    Mesh sphereMesh;

    // texture
    unsigned int textureID = 0;

    // buffer IDs
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};
#endif