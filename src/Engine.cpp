#include "Engine.hpp"
#include "Sphere.hpp"
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

        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
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
        Sphere::Sphere testSphere(0.7f, 20, 20);

        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.0f, 0.470f, 0.509f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            float aspect = (float)width / (float)height;
            glFrustum(-0.1f * aspect, 0.1f * aspect, -0.1f, 0.1f, 0.1f, 100.0f);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glTranslatef(0.0f, 0.0f, -2.5f);

            glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
            glRotatef((float)glfwGetTime() * 20.0f, 0.0f, 1.0f, 0.0f);

            testSphere.draw();

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