#include "Planet.hpp"
#include "glad/glad.h"
#include <utility>

namespace Planet {

    Planet::Planet(float radius, unsigned int stacks, unsigned int sectors,
                   float orbitDistance, float orbitSpeed, float spinSpeed, bool isSun)
        : sphere(radius, stacks, sectors), orbitDistance(orbitDistance),
          orbitSpeed(orbitSpeed), spinSpeed(spinSpeed), sun(isSun)
    {}

    Planet::~Planet() {}

    void Planet::addSatellite(Planet&& satellite) {
        satellites.push_back(std::move(satellite));
    }

    void Planet::draw(float time) {
        glPushMatrix();
        if (!sun) {
            glRotatef(time * orbitSpeed, 0.0f, 1.0f, 0.0f);
            glTranslatef(orbitDistance, 0.0f, 0.0f);
        }

        glPushMatrix();
        glRotatef(time * spinSpeed, 0.0f, 1.0f, 0.0f);
        sphere.draw();
        glPopMatrix();

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
