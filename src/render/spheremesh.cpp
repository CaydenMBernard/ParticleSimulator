#include <glm/glm.hpp>

#include "shader.h"
#include "spheremesh.h"

#include <iostream>
#include <array>
#include <cmath>
#include <vector>

void createSphereMesh(Mesh& mesh, int stacks = 20, int slices = 20)
{
    mesh.sphereVertices.reserve((stacks + 1) * (slices + 1) * 8);
    mesh.sphereIndices.reserve(stacks * slices * 6);

    for (int i = 0; i <= stacks; ++i)
    {
        float v = (float)i / (float)stacks;          // 0..1
        float phi = v * glm::pi<float>();            // 0..PI

        for (int j = 0; j <= slices; ++j)
        {
            float u = (float)j / (float)slices;      // 0..1
            float theta = u * (glm::pi<float>() * 2.0f); // 0..2PI

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            glm::vec3 unitPos(x, y, z);

            glm::vec3 pos = unitPos * 0.5f;
            glm::vec3 normal = glm::normalize(unitPos);

            // position
            mesh.sphereVertices.push_back(pos.x);
            mesh.sphereVertices.push_back(pos.y);
            mesh.sphereVertices.push_back(pos.z);

            // normal
            mesh.sphereVertices.push_back(normal.x);
            mesh.sphereVertices.push_back(normal.y);
            mesh.sphereVertices.push_back(normal.z);

            // texcoord
            mesh.sphereVertices.push_back(u);
            mesh.sphereVertices.push_back(v);
        }
    }

    // indices (two triangles per quad)
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int row1 = i * (slices + 1);
            int row2 = (i + 1) * (slices + 1);

            unsigned int a = row1 + j;
            unsigned int b = row2 + j;
            unsigned int c = row2 + (j + 1);
            unsigned int d = row1 + (j + 1);

            // triangle 1
            mesh.sphereIndices.push_back(a);
            mesh.sphereIndices.push_back(b);
            mesh.sphereIndices.push_back(c);

            // triangle 2
            mesh.sphereIndices.push_back(a);
            mesh.sphereIndices.push_back(c);
            mesh.sphereIndices.push_back(d);
        }
    }

    mesh.indexCount = (int)mesh.sphereIndices.size();
}