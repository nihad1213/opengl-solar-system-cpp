#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Planet {

    class Planet {
        public:
            Planet();
            ~Planet();

            void render();

        protected:
            void update();
            void draw();
    };

}