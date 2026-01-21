#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb_image.h>

#include "render/texture.h"
#include "render/renderer.h"
#include "render/shader.h"

#include <iostream>

unsigned int textureInit(Shader& shader) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    shader.setInt("ourTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true); 

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/wavy_texture.jpg", &width, &height, &nrChannels, 0);
    
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        return 0;
    }

    auto format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);

    return textureID;
}