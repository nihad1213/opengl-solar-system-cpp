#pragma once

#include "Sphere.hpp"
#include <glm/glm.hpp>

namespace Planet {

    class Planet {
        public:
            Planet(float radius, unsigned int stacks, unsigned int sectors,
                   float orbitDistance, bool isSun = false);
            ~Planet();

            void draw();

            bool isSun() const;
            float getOrbitDistance() const;

        private:
            Sphere::Sphere sphere;
            float orbitDistance;
            bool sun;
    };

}
