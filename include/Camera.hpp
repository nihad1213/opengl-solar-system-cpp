#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Camera {

    class Camera {
        public:
            Camera();
            ~Camera();

        private:
            void goForward();
            void goBackward();
            void goLeft();
            void goRight();
    };
}
