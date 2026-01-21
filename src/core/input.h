#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>

class Camera;

class Input {
public:
    Input(GLFWwindow* window, Camera* camera);

    void ProcessInput(float deltaTime);
    bool IsKeyPressed(int key) const;
    bool IsMouseButtonPressed(int button) const;
    std::pair<double, double> GetMousePosition() const;
    void SetCursorDisabled(bool disabled) const;

private:
    GLFWwindow* m_Window = nullptr;
    Camera* m_Camera = nullptr;

    // state for mouse handling
    bool  m_FirstMouse = true;
    float m_LastX = 0.0f;
    float m_LastY = 0.0f;

    // GLFW callbacks (must match signature) - STATIC so GLFW can call them
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
