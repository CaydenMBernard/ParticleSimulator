#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

struct Particle {
    glm::vec3 currPosition;
    glm::vec3 prevPosition;
    float velocity;
    float acceleration;
    float radius;
};
#endif