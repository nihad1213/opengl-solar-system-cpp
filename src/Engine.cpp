#include "Engine.hpp"
#include <iostream>

namespace Engine {

    Application::Application() : window(nullptr), width(1360), height(780), title("Solar System") {}

    Application::~Application() {
        shutdown();
    }

    int Application::run() {
        if (!initialize()) {
            std::cerr << "[Engine Error] Application initialization failed." << std::endl;
            shutdown();
            return -1;
        }

        mainLoop();
        shutdown();
        return 0;
    }

    bool Application::initialize() {
        if (!initializeGLFW()) {
            return false;
        }

        setupWindowHints();

        if (!createWindow()) {
            return false;
        }

        if (!initializeGLAD()) {
            return false;
        }

        return true;
    }

    bool Application::initializeGLFW() {
        if (!glfwInit()) {
            std::cerr << "[Engine Error] Failed to initialize GLFW." << std::endl;
            return false;
        }
        return true;
    }

    void Application::setupWindowHints() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //for macOS
#endif
    }

    bool Application::createWindow() {
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            std::cerr << "[Engine Error] Failed to create GLFW window." << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        return true;
    }

    bool Application::initializeGLAD() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "[Engine Error] Failed to initialize GLAD." << std::endl;
            return false;
        }

        glViewport(0, 0, width, height);

        return true;
    }

    void Application::mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.0f, 0.470f, 0.509f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void Application::shutdown() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }
}
