#pragma once
#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include <vector>

struct Mesh {
    int indexCount = 0;
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
};

void createSphereMesh(Mesh& mesh, int stacks = 20, int slices = 20);

#endif