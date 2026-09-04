#include "Planet.hpp"
#include "glad/glad.h"
#include <utility>
#include <cmath>

namespace Planet {

    Planet::Planet(float radius, unsigned int stacks, unsigned int sectors,
                   float orbitDistance, float orbitSpeed, float spinSpeed,
                   const std::string& texturePath, bool isSun)
        : sphere(radius, stacks, sectors, texturePath), orbitDistance(orbitDistance),
          orbitSpeed(orbitSpeed), spinSpeed(spinSpeed), sun(isSun)
    {}

    Planet::~Planet() {}

    void Planet::addSatellite(Planet&& satellite) {
        satellites.push_back(std::move(satellite));
    }

    void Planet::addRing(float innerRadius, float outerRadius, const std::string& texturePath) {
        ring = std::make_unique<Ring::Ring>(innerRadius, outerRadius, 128, texturePath);
    }

    void Planet::drawOrbitLine() const {
        const float PI = 3.14159265359f;
        const int segments = 64;

        glDisable(GL_TEXTURE_2D);
        glColor3f(0.4f, 0.4f, 0.4f);

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float angle = 2.0f * PI * (float)i / segments;
            glVertex3f(orbitDistance * std::cos(angle), 0.0f, orbitDistance * std::sin(angle));
        }
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
    }

    void Planet::draw(float time) {
        glPushMatrix();
        if (!sun) {
            drawOrbitLine();
            glRotatef(time * orbitSpeed, 0.0f, 1.0f, 0.0f);
            glTranslatef(orbitDistance, 0.0f, 0.0f);
        }

        glPushMatrix();
        glRotatef(time * spinSpeed, 0.0f, 1.0f, 0.0f);
        sphere.draw();
        glPopMatrix();

        if (ring) {
            ring->draw();
        }

        for (auto& satellite : satellites) {
            satellite.draw(time);
        }
        glPopMatrix();
    }

    bool Planet::isSun() const {
        return sun;
    }

    float Planet::getOrbitDistance() const {
        return orbitDistance;
    }
}
