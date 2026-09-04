#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

namespace Ring {

    struct Vertex {
        float x, y, z;
        float u, v;
    };

    class Ring {
        public:
            Ring(float innerRadius, float outerRadius, unsigned int segments,
                 const std::string& texturePath);
            ~Ring();

            Ring(const Ring&) = delete;
            Ring& operator=(const Ring&) = delete;

            Ring(Ring&& other) noexcept;
            Ring& operator=(Ring&& other) noexcept;

            void draw();

        private:
            unsigned int VAO, VBO;
            unsigned int textureID;
            unsigned int vertexCount;

            void buildMesh(float innerRadius, float outerRadius, unsigned int segments);
            void loadTexture(const std::string& path);
    };
}
