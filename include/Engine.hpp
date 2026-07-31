#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Camera.hpp"

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
        

            void processInput(GLFWwindow* window);
            static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        private:
            GLFWwindow* window;
            int width;
            int height;
            const char* title = "Solar System";

            Camera::Camera camera;
            float deltaTime;
            float lastFrame;

            float lastX;
            float lastY;
            bool firstMouse;
    };
}

