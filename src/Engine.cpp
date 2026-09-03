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
        std::vector<Planet::Planet> planets;
        planets.emplace_back(1.0f, 70, 70, 0.0f, true);
        planets.emplace_back(0.3f, 40, 40, 2.5f, false);

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

            // glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
            glRotatef((float)glfwGetTime() * 20.0f, 0.0f, 1.0f, 0.0f);

            for (auto& planet : planets) {
                planet.draw();
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