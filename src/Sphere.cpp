#include "Sphere.hpp"
#include <cmath>

namespace Sphere {

    Sphere::Sphere(float radius, unsigned int stacks, unsigned int sectors)
    : VAO(0), VBO(0), EBO(0), indexCount(0)
    {
        drawSphere(radius, stacks, sectors);
    }

    Sphere::~Sphere() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);
    }

    Sphere::Sphere(Sphere&& other) noexcept
    : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), indexCount(other.indexCount)
    {
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.indexCount = 0;
    }

    Sphere& Sphere::operator=(Sphere&& other) noexcept {
        if (this != &other) {
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (EBO != 0) glDeleteBuffers(1, &EBO);

            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            indexCount = other.indexCount;

            other.VAO = 0;
            other.VBO = 0;
            other.EBO = 0;
            other.indexCount = 0;
        }
        return *this;
    }

    void Sphere::drawSphere(float radius, unsigned int stacks, unsigned int sectors) {
            const float PI = 3.14159265359f;
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            // Up to Above Stacks
            for (unsigned int i = 0; i <= stacks; ++i) {
                float stackAngle = PI / 2.0f - (float)i / stacks * PI;

                float xy = radius * std::cos(stackAngle);
                float y  = radius * std::sin(stackAngle);

                // Left to Right Sectors
                for (unsigned int j = 0; j <= sectors; ++j) {
                    float sectorAngle = (float)j / sectors * 2.0f * PI;

                    Vertex v;
                    v.x = xy * std::cos(sectorAngle);
                    v.y = y;
                    v.z = xy * std::sin(sectorAngle);

                    vertices.push_back(v);
                }
            }

            for (unsigned int i = 0; i < stacks; ++i) {
                unsigned int k1 = i * (sectors + 1);
                unsigned int k2 = k1 + sectors + 1;

                for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
                    if (i != 0) {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }

                    if (i != (stacks - 1)) {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }

            indexCount = static_cast<unsigned int>(indices.size());

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            glBindVertexArray(0);

    }

    void Sphere::draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}