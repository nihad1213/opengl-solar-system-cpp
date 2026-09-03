#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>

namespace Sphere {

    struct Vertex {
        float x, y, z;
    };

    class Sphere {
        public:
            Sphere(float radius = 1.0f, unsigned int stacks = 20, unsigned int sectors = 20);
            ~Sphere();

            Sphere(const Sphere&) = delete;
            Sphere& operator=(const Sphere&) = delete;

            Sphere(Sphere&& other) noexcept;
            Sphere& operator=(Sphere&& other) noexcept;

            void draw();
        
        private:
            unsigned int VAO, VBO, EBO;
            unsigned int indexCount;

            void drawSphere(float radius, unsigned int stacks, unsigned int sectors);
    };
}