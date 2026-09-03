#include "Planet.hpp"
#include "glad/glad.h"

namespace Planet {

    Planet::Planet(float radius, unsigned int stacks, unsigned int sectors,
                   float orbitDistance, bool isSun)
        : sphere(radius, stacks, sectors), orbitDistance(orbitDistance), sun(isSun)
    {}

    Planet::~Planet() {}

    void Planet::draw() {
        glPushMatrix();
        if (!sun) {
            glTranslatef(orbitDistance, 0.0f, 0.0f);
        }
        sphere.draw();
        glPopMatrix();
    }

    bool Planet::isSun() const {
        return sun;
    }

    float Planet::getOrbitDistance() const {
        return orbitDistance;
    }
}
