// 1-апта: терезе ашу, негізгі цикл
// Тапсырмалар: 1280x720, жылдам фон, Пробел = ақ фон, FPS (+ қосымша: toggle, FPS тақырыпта)

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <string>

// --- 1-тапсырма: терезе өлшемі ---
const int WINDOW_WIDTH  = 1280;
const int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "cg";

// Қосымша тапсырма: true болса — Пробел басқан сайын ауысады (toggle),
// false болса — тек басып тұрғанда ақ болады (негізгі тапсырма).
const bool SPACE_TOGGLE_MODE = false;

// --- Жаһандық күй (processInput осыларға жазады) ---
bool g_whiteBackground = false;   // фон қазір ақ па
bool g_spaceWasPressed = false;   // алдыңғы кадрдағы Пробел күйі (toggle үшін)

// Терезе өлшемі өзгергенде viewport-ты жаңарту — сурет созылмайды
void onResize(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

// --- 3-тапсырма: Пробелді тексеру ---
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (SPACE_TOGGLE_MODE) {
        // Тек "басылмаған → басылған" өтуінде ауыстырамыз,
        // әйтпесе секундына 60+ рет ауысып кетеді
        if (spacePressed && !g_spaceWasPressed)
            g_whiteBackground = !g_whiteBackground;
    } else {
        g_whiteBackground = spacePressed;
    }
    g_spaceWasPressed = spacePressed;
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }

    std::cout << "OpenGL:   " << glGetString(GL_VERSION)  << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    // --- 4-тапсырма: VSync өшіру ---
    glfwSwapInterval(0);

    double fpsTimer   = glfwGetTime();  // соңғы FPS шыққан уақыт
    int    frameCount = 0;              // осы секундтағы кадрлар саны

    while (!glfwWindowShouldClose(window)) {
        // Кіріс glClear-дан БҰРЫН тексеріледі — әйтпесе түс бір кадрға кешігеді
        processInput(window);

        float t = (float)glfwGetTime();

        // --- 2-тапсырма: жиілік 0.5 → 1.0 (2 есе жылдам), амплитуда 0.3 өзгермейді ---
        // sin(t * ЖИІЛІК) — қаншалықты тез;  * 0.3f — АМПЛИТУДА, қаншалықты қанық
        float r = (std::sin(t * 1.0f) + 1.0f) * 0.5f * 0.3f;
        float g = (std::sin(t * 1.4f) + 1.0f) * 0.5f * 0.3f;
        float b = (std::sin(t * 1.8f) + 1.0f) * 0.5f * 0.3f;

        if (g_whiteBackground)
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        else
            glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // --- 4-тапсырма: FPS, секундына бір рет ---
        frameCount++;
        double now = glfwGetTime();
        if (now - fpsTimer >= 1.0) {
            double fps = frameCount / (now - fpsTimer);
            std::cout << "FPS: " << (int)fps << std::endl;

            // Қосымша: FPS терезе тақырыбында
            std::string title = std::string(WINDOW_TITLE) + " | FPS: " + std::to_string((int)fps);
            glfwSetWindowTitle(window, title.c_str());

            frameCount = 0;   // санауышты нөлдеу
            fpsTimer   = now;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
