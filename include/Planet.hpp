#pragma once

#include "Sphere.hpp"
#include "Ring.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

namespace Planet {

    class Planet {
        public:
            Planet(float radius, unsigned int stacks, unsigned int sectors,
                   float orbitDistance, float orbitSpeed, float spinSpeed,
                   const std::string& texturePath = "", bool isSun = false);
            ~Planet();

            Planet(const Planet&) = delete;
            Planet& operator=(const Planet&) = delete;

            Planet(Planet&&) noexcept = default;
            Planet& operator=(Planet&&) noexcept = default;

            void addSatellite(Planet&& satellite);
            void addRing(float innerRadius, float outerRadius, const std::string& texturePath);
            void draw(float time);

            bool isSun() const;
            float getOrbitDistance() const;

        private:
            Sphere::Sphere sphere;
            float orbitDistance;
            float orbitSpeed;
            float spinSpeed;
            bool sun;
            std::vector<Planet> satellites;
            std::unique_ptr<Ring::Ring> ring;

            void drawOrbitLine() const;
    };

}
