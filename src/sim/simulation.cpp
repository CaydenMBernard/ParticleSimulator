#include <sim/simulation.h>
#include <glm/glm.hpp>
#include <vector>

void updatePositions(std::vector<Particle>& particles, float deltaTime) {
    for (int i = 0; i < particles.size(); i++) {
        glm::vec3 displacement = particles[i].currPosition - particles[i].prevPosition;
        particles[i].prevPosition = particles[i].currPosition;
        particles[i].acceleration *= (deltaTime * deltaTime);
        particles[i].currPosition += displacement;
        particles[i].currPosition += particles[i].acceleration;
    }
}
