#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Engine {
    
    class Application {

        public:
            Application();
            ~Application();

            int run();
        
        private:
            bool initialize();
            bool initializeGLFW();
            bool initializeGLAD();

            void setupWindowHints();
            bool createWindow();

            void mainLoop();
            void shutdown();
        
        private:
            GLFWwindow* window;
            int width;
            int height;
            
            const char* title = "Solar System";
    };
}

