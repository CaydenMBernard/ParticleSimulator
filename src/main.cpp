#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/shader.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "core/input.h"
#include "sim/particle.h"
#include "sim/simulation.h"

#include <iostream>
#include <array>
#include <cmath>
#include <vector>

int main () {
    // Initialize GLFW
    glfwInit();

    // Configure GLFW version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window object and save a pointer to it
    GLFWwindow* window = glfwCreateWindow(1200, 900, "LearnOpenGL", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the context of the window the main context on the current thread
    glfwMakeContextCurrent(window);

    // Load all OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the viewport
    glViewport(0, 0, 1200, 900);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    Input input(window, &camera);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Disable cursor
    input.SetCursorDisabled(true);

    Renderer renderer;
    renderer.init();

    Particle particle1;
    particle1.currPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    std::vector<Particle> particles;
    particles.push_back(particle1);
    Particle particle2;
    particle2.currPosition = glm::vec3(1.0f, 1.0f, 1.0f);
    particles.push_back(particle2);

    particles[0].prevPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    particles[1].prevPosition = glm::vec3(1.0f, 1.0f, 1.0f);
    particles[0].acceleration = glm::vec3(0.01f, 0.01f, 0.01f);
    particles[1].acceleration = glm::vec3(-0.001f, -0.001f, -0.001f);

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // per-frame time logic
        static float lastFrame = 0.0f;
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        input.ProcessInput(deltaTime);

        // render
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.renderScreen(particles, camera);

        // update positions
        updatePositions(particles, deltaTime);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
};

