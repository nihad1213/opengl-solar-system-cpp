#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>

namespace Sphere {

    class Sphere {
        public:
            Sphere();
            ~Sphere();
        
        protected:
            void draw();
    };
}