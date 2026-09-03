#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

namespace Sphere {

    struct Vertex {
        float x, y, z;
        float u, v;
    };

    class Sphere {
        public:
            Sphere(float radius = 1.0f, unsigned int stacks = 20, unsigned int sectors = 20,
                   const std::string& texturePath = "");
            ~Sphere();

            Sphere(const Sphere&) = delete;
            Sphere& operator=(const Sphere&) = delete;

            Sphere(Sphere&& other) noexcept;
            Sphere& operator=(Sphere&& other) noexcept;

            void draw();

        private:
            unsigned int VAO, VBO, EBO;
            unsigned int textureID;
            unsigned int indexCount;

            void drawSphere(float radius, unsigned int stacks, unsigned int sectors);
            void loadTexture(const std::string& path);
    };
}
