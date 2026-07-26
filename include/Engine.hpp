#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {
    
    class Application {
        public:
            int run() {
                
                if (!glfwInit()) {
                    std::cerr << "Failed to initialize GLFW" << std::endl;
                    return -1;
                }

                hints();

                GLFWwindow* window = createWindow();
            
                glfwMakeContextCurrent(window);
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                    std::cerr << "Failed to initialize GLAD" << std::endl;
                    return -1;
                }
                
                loop(window);
                return 0;
            }

        private:
            void hints() {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            }
        
            GLFWwindow* createWindow() {
                GLFWwindow* newWindow = glfwCreateWindow(1200, 1000, "OpenGL Window", nullptr, nullptr);
                return newWindow;
            }

            void loop(GLFWwindow* window) {
                while (!glfwWindowShouldClose(window)) {
                    glClear(GL_COLOR_BUFFER_BIT);
                
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                }
            }
    };
}

