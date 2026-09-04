#include "Sphere.hpp"
#include "stb/stb_image.h"
#include <cmath>
#include <iostream>

namespace Sphere {

    Sphere::Sphere(float radius, unsigned int stacks, unsigned int sectors, const std::string& texturePath)
    : VAO(0), VBO(0), EBO(0), textureID(0), indexCount(0)
    {
        drawSphere(radius, stacks, sectors);
        if (!texturePath.empty()) {
            loadTexture(texturePath);
        }
    }

    Sphere::~Sphere() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);
        if (textureID != 0) glDeleteTextures(1, &textureID);
    }

    Sphere::Sphere(Sphere&& other) noexcept
    : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), textureID(other.textureID), indexCount(other.indexCount)
    {
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.textureID = 0;
        other.indexCount = 0;
    }

    Sphere& Sphere::operator=(Sphere&& other) noexcept {
        if (this != &other) {
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (EBO != 0) glDeleteBuffers(1, &EBO);
            if (textureID != 0) glDeleteTextures(1, &textureID);

            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            textureID = other.textureID;
            indexCount = other.indexCount;

            other.VAO = 0;
            other.VBO = 0;
            other.EBO = 0;
            other.textureID = 0;
            other.indexCount = 0;
        }
        return *this;
    }

    void Sphere::loadTexture(const std::string& path) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "[Sphere] Failed to load texture: " << path << std::endl;
            return;
        }

        GLenum format = GL_RGB;
        if (channels == 1) format = GL_RED;
        else if (channels == 4) format = GL_RGBA;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
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
                    v.u = (float)j / sectors;
                    v.v = 1.0f - (float)i / stacks;

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

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

            glBindVertexArray(0);

    }

    void Sphere::draw() {
        glBindVertexArray(VAO);

        if (textureID != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
        } else {
            glDisable(GL_TEXTURE_2D);
        }

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
