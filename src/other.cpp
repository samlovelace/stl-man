#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Structure for a 3D vertex
struct Vertex3D {
    float x, y, z;
};

// Structure for a triangle
struct Triangle {
    Vertex3D v1, v2, v3;
};

// Function to parse an ASCII STL file
std::vector<Triangle> parseSTL(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return {};
    }

    std::vector<Triangle> triangles;
    std::string line;

    Triangle triangle;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word == "vertex") {
            float x, y, z;
            iss >> x >> y >> z;
            Vertex3D vertex{x, y, z};

            if (triangle.v1.x == 0 && triangle.v1.y == 0 && triangle.v1.z == 0) {
                triangle.v1 = vertex;
            } else if (triangle.v2.x == 0 && triangle.v2.y == 0 && triangle.v2.z == 0) {
                triangle.v2 = vertex;
            } else {
                triangle.v3 = vertex;
                triangles.push_back(triangle);
                triangle = {}; // Reset for the next triangle
            }
        }
    }

    file.close();
    return triangles;
}

// Function to draw triangles from STL data
void drawTriangles(const std::vector<Triangle>& triangles) {
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : triangles) {
        // Draw each triangle
        glVertex3f(triangle.v1.x, triangle.v1.y, triangle.v1.z);
        glVertex3f(triangle.v2.x, triangle.v2.y, triangle.v2.z);
        glVertex3f(triangle.v3.x, triangle.v3.y, triangle.v3.z);
    }
    glEnd();
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create SDL window with OpenGL context
    SDL_Window* window = SDL_CreateWindow("STL Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // Parse the STL file
    std::vector<Triangle> triangles = parseSTL("../dragon.stl");
    if (triangles.empty()) {
        std::cerr << "Failed to parse STL file." << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    bool running = true;
    SDL_Event event;
    float angle = 0.0f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear the screen and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the model-view matrix
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f); // Move back to see the cube
        glRotatef(angle, 1.0f, 1.0f, 0.0f); // Rotate the model
        angle += 0.5f;

        // Draw the triangles
        drawTriangles(triangles);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
