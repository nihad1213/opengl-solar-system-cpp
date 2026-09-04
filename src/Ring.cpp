#include "Ring.hpp"
#include "stb/stb_image.h"
#include <cmath>
#include <vector>
#include <iostream>

namespace Ring {

    Ring::Ring(float innerRadius, float outerRadius, unsigned int segments, const std::string& texturePath)
    : VAO(0), VBO(0), textureID(0), vertexCount(0)
    {
        buildMesh(innerRadius, outerRadius, segments);
        if (!texturePath.empty()) {
            loadTexture(texturePath);
        }
    }

    Ring::~Ring() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (textureID != 0) glDeleteTextures(1, &textureID);
    }

    Ring::Ring(Ring&& other) noexcept
    : VAO(other.VAO), VBO(other.VBO), textureID(other.textureID), vertexCount(other.vertexCount)
    {
        other.VAO = 0;
        other.VBO = 0;
        other.textureID = 0;
        other.vertexCount = 0;
    }

    Ring& Ring::operator=(Ring&& other) noexcept {
        if (this != &other) {
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (textureID != 0) glDeleteTextures(1, &textureID);

            VAO = other.VAO;
            VBO = other.VBO;
            textureID = other.textureID;
            vertexCount = other.vertexCount;

            other.VAO = 0;
            other.VBO = 0;
            other.textureID = 0;
            other.vertexCount = 0;
        }
        return *this;
    }

    void Ring::buildMesh(float innerRadius, float outerRadius, unsigned int segments) {
        const float PI = 3.14159265359f;
        std::vector<Vertex> vertices;

        // Flat annulus in the XZ plane, built as a triangle strip alternating
        // outer/inner ring vertices so it can be drawn with a single draw call.
        for (unsigned int i = 0; i <= segments; ++i) {
            float angle = 2.0f * PI * (float)i / segments;
            float c = std::cos(angle);
            float s = std::sin(angle);

            Vertex outer;
            outer.x = outerRadius * c;
            outer.y = 0.0f;
            outer.z = outerRadius * s;
            outer.u = 1.0f;
            outer.v = (float)i / segments;
            vertices.push_back(outer);

            Vertex inner;
            inner.x = innerRadius * c;
            inner.y = 0.0f;
            inner.z = innerRadius * s;
            inner.u = 0.0f;
            inner.v = (float)i / segments;
            vertices.push_back(inner);
        }

        vertexCount = static_cast<unsigned int>(vertices.size());

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
    }

    void Ring::loadTexture(const std::string& path) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "[Ring] Failed to load texture: " << path << std::endl;
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    void Ring::draw() {
        glBindVertexArray(VAO);

        if (textureID != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
        } else {
            glDisable(GL_TEXTURE_2D);
        }

        // The ring alpha texture has transparent gaps, so blend it and skip
        // depth writes to avoid ordering artifacts against whatever is behind it.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        glBindVertexArray(0);
    }
}
