#include <GLFW/glfw3.h>
#include <glad/glad.h>   
#include <vector>
#include <cmath>
#include <iostream>

struct Particle {
    float x, y;
    float vx, vy;
    bool pinned;
};

struct Spring {
    int a, b; // spring connects particles w indexes a and b
    float restLength;
    float k; // stiffness
};

const float GRAVITY = -1.8f;
const float TIMESTEP = 0.016f;

std::vector<Particle> particles;
std::vector<Spring> springs;

GLuint vaoParticles, vboParticles;
GLuint vaoSprings, vboSprings;

const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform mat4 uProjection;
void main() {
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
void main() {
    FragColor = vec4(uColor, 1.0);
}
)";

GLuint createShaderProgram() {
    auto compileShader = [](GLenum type, const char* src) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            std::cerr << "Shader compile error:\n" << log << "\n";
        }
        return shader;
    };

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Program link error:\n" << log << "\n";
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void createSoftBody(int rows, int cols, float spacing, float stiffness) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Particle p;
            p.x = j * spacing - (cols * spacing) / 2.0f;
            p.y = i * spacing + 0.5f;
            p.vx = p.vy = 0.0f;
            p.pinned = (i == rows - 1 && (j == 0 || j == cols - 1));
            particles.push_back(p);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            if (j < cols - 1) springs.push_back({idx, idx + 1, spacing, stiffness});
            if (i < rows - 1) springs.push_back({idx, idx + cols, spacing, stiffness});
        }
    }
}

void updatePhysics() {
    for (auto &s : springs) {
        Particle &p1 = particles[s.a];
        Particle &p2 = particles[s.b];

        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        float diff = dist - s.restLength;

        if (dist > 0.0001f) {
            float force = s.k * diff;
            float fx = (dx / dist) * force;
            float fy = (dy / dist) * force;

            if (!p1.pinned) {
                p1.vx += fx * TIMESTEP;
                p1.vy += fy * TIMESTEP;
            }
            if (!p2.pinned) {
                p2.vx -= fx * TIMESTEP;
                p2.vy -= fy * TIMESTEP;
            }
        }
    }

    for (auto &p : particles) {
        if (p.pinned) continue;
        p.vy += GRAVITY * TIMESTEP;
        p.x += p.vx * TIMESTEP;
        p.y += p.vy * TIMESTEP;
        if (p.y < -0.9f) {
            p.y = -0.9f;
            p.vy *= -0.3f;
        }
    }
}

void updateBuffers() {
    std::vector<float> particleVerts;
    for (auto &p : particles) {
        particleVerts.push_back(p.x);
        particleVerts.push_back(p.y);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboParticles);
    glBufferData(GL_ARRAY_BUFFER, particleVerts.size() * sizeof(float),
                 particleVerts.data(), GL_DYNAMIC_DRAW);

    std::vector<float> springVerts;
    for (auto &s : springs) {
        springVerts.push_back(particles[s.a].x);
        springVerts.push_back(particles[s.a].y);
        springVerts.push_back(particles[s.b].x);
        springVerts.push_back(particles[s.b].y);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboSprings);
    glBufferData(GL_ARRAY_BUFFER, springVerts.size() * sizeof(float),
                 springVerts.data(), GL_DYNAMIC_DRAW);
}

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Soft Body Simulation (Modern OpenGL)", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    GLuint shader = createShaderProgram();
    GLint uProjectionLoc = glGetUniformLocation(shader, "uProjection");
    GLint uColorLoc = glGetUniformLocation(shader, "uColor");

    // VAO/VBO for particles
    glGenVertexArrays(1, &vaoParticles);
    glGenBuffers(1, &vboParticles);
    glBindVertexArray(vaoParticles);
    glBindBuffer(GL_ARRAY_BUFFER, vboParticles);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // VAO/VBO for springs
    glGenVertexArrays(1, &vaoSprings);
    glGenBuffers(1, &vboSprings);
    glBindVertexArray(vaoSprings);
    glBindBuffer(GL_ARRAY_BUFFER, vboSprings);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Create soft body
    createSoftBody(6, 6, 0.15f, 100.0f);

    // Projection
    float ortho[16] = {
        2.0f/2.0f, 0, 0, 0,
        0, 2.0f/2.0f, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };

    glPointSize(6.0f);

    while (!glfwWindowShouldClose(window)) {
        updatePhysics();
        updateBuffers();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glUniformMatrix4fv(uProjectionLoc, 1, GL_FALSE, ortho);

        // Draw springs
        glUniform3f(uColorLoc, 0.8f, 0.8f, 0.8f);
        glBindVertexArray(vaoSprings);
        glDrawArrays(GL_LINES, 0, springs.size() * 2);

        // Draw particles
        glUniform3f(uColorLoc, 0.1f, 0.7f, 1.0f);
        glBindVertexArray(vaoParticles);
        glDrawArrays(GL_POINTS, 0, particles.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
#