#include "core/input.h"
#include "render/camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Input::Input(GLFWwindow* window, Camera* camera)
    : m_Window(window), m_Camera(camera)
{
    // set initial mouse reference point (optional but nice)
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    m_LastX = w * 0.5f;
    m_LastY = h * 0.5f;

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, Input::FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, Input::MouseCallback);
    glfwSetScrollCallback(window, Input::ScrollCallback);
}

void Input::ProcessInput(float deltaTime)
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        m_Camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        m_Camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        m_Camera->ProcessKeyboard(RIGHT, deltaTime);
}

bool Input::IsKeyPressed(int key) const
{
    return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button) const
{
    return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
}

std::pair<double, double> Input::GetMousePosition() const
{
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return { x, y };
}

void Input::SetCursorDisabled(bool disabled) const
{
    glfwSetInputMode(m_Window, GLFW_CURSOR,
        disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Input::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Input::MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    auto* state = static_cast<Input*>(glfwGetWindowUserPointer(window));
    if (!state || !state->m_Camera) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (state->m_FirstMouse) {
        state->m_LastX = xpos;
        state->m_LastY = ypos;
        state->m_FirstMouse = false;
    }

    float xoffset = xpos - state->m_LastX;
    float yoffset = state->m_LastY - ypos; // reversed Y

    state->m_LastX = xpos;
    state->m_LastY = ypos;

    state->m_Camera->ProcessMouseMovement(xoffset, yoffset);
}

void Input::ScrollCallback(GLFWwindow* window, double /*xoffset*/, double yoffset)
{
    auto* state = static_cast<Input*>(glfwGetWindowUserPointer(window));
    if (!state || !state->m_Camera) return;

    state->m_Camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
