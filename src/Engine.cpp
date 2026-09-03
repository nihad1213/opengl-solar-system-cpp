#include "Engine.hpp"
#include "Planet.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

namespace Engine {

    Application::Application() : 
        window(nullptr),
        width(1240), 
        height(680), 
        title("Solar System"),
        camera(glm::vec3(0.0f, 0.0f, 5.0f)),
        deltaTime(0.0f), 
        lastFrame(0.0f),
        lastX(1240.0f / 2.0f), 
        lastY(680.0f / 2.0f), 
        firstMouse(true) 
    {}

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
        glfwSwapInterval(1);

        glfwSetWindowUserPointer(window, this);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouseCallback);
        
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
        std::vector<Planet::Planet> bodies;
        bodies.reserve(10);

        bodies.emplace_back(1.0f, 40, 40, 0.0f, 0.0f, 2.0f, true); // Sun

        bodies.emplace_back(0.08f, 20, 20, 1.8f, 40.0f, 10.0f); // Mercury
        bodies.emplace_back(0.15f, 24, 24, 2.4f, 30.0f, 8.0f);  // Venus

        bodies.emplace_back(0.16f, 24, 24, 3.0f, 24.0f, 50.0f); // Earth
        bodies.back().addSatellite(Planet::Planet(0.05f, 14, 14, 0.35f, 120.0f, 20.0f)); // Moon

        bodies.emplace_back(0.12f, 20, 20, 3.6f, 18.0f, 48.0f); // Mars
        bodies.back().addSatellite(Planet::Planet(0.02f, 10, 10, 0.20f, 150.0f, 30.0f));  // Phobos
        bodies.back().addSatellite(Planet::Planet(0.015f, 10, 10, 0.28f, 130.0f, 25.0f)); // Deimos

        bodies.emplace_back(0.5f, 32, 32, 4.6f, 10.0f, 90.0f); // Jupiter
        bodies.back().addSatellite(Planet::Planet(0.04f, 14, 14, 0.65f, 90.0f, 25.0f));  // Io
        bodies.back().addSatellite(Planet::Planet(0.035f, 14, 14, 0.75f, 80.0f, 22.0f)); // Europa
        bodies.back().addSatellite(Planet::Planet(0.045f, 14, 14, 0.9f, 70.0f, 20.0f));  // Ganymede
        bodies.back().addSatellite(Planet::Planet(0.04f, 14, 14, 1.05f, 60.0f, 18.0f));  // Callisto

        bodies.emplace_back(0.45f, 32, 32, 5.8f, 8.0f, 85.0f); // Saturn
        bodies.back().addSatellite(Planet::Planet(0.04f, 14, 14, 0.7f, 50.0f, 15.0f)); // Titan

        bodies.emplace_back(0.3f, 28, 28, 6.8f, 6.0f, 70.0f); // Uranus

        bodies.emplace_back(0.29f, 28, 28, 7.6f, 5.0f, 75.0f); // Neptune
        bodies.back().addSatellite(Planet::Planet(0.035f, 14, 14, 0.45f, 60.0f, 15.0f)); // Triton

        bodies.emplace_back(0.06f, 16, 16, 8.3f, 4.0f, 20.0f); // Pluto
        bodies.back().addSatellite(Planet::Planet(0.02f, 10, 10, 0.15f, 100.0f, 12.0f)); // Charon

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);
            
            glClearColor(0.0f, 0.470f, 0.509f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            float aspect = (float)width / (float)height;
            glFrustum(-0.1f * aspect, 0.1f * aspect, -0.1f, 0.1f, 0.1f, 100.0f);

            glMatrixMode(GL_MODELVIEW);
            glm::mat4 view = camera.getViewMatrix();
            glLoadMatrixf(glm::value_ptr(view));

            for (auto& body : bodies) {
                body.draw(currentFrame);
            }

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

    void Application::processInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(Camera::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(Camera::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(Camera::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(Camera::RIGHT, deltaTime);
    }

    void Application::mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (!app) return;

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (app->firstMouse) {
            app->lastX = xpos;
            app->lastY = ypos;
            app->firstMouse = false;
        }

        float xoffset = xpos - app->lastX;
        float yoffset = app->lastY - ypos;

        app->lastX = xpos;
        app->lastY = ypos;

        app->camera.processMouseMovement(xoffset, yoffset);
    }
}