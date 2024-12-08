
#include <iostream> 
#include <string> 

#include "Parser.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

void drawTriangle(std::vector<Vector3> vertices)
{
    glBegin(GL_TRIANGLES); 

    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);

    glEnd();  
}

void drawWireframeLines(const std::vector<Vector3>& vertices) {
    glBegin(GL_LINES); // Use GL_LINES to draw each edge of the triangle
    // Edge 1: v1 to v2
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);

    // Edge 2: v2 to v3
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);

    // Edge 3: v3 to v1
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    
    glEnd();
}


void drawModel(STLModel* model)
{
    for(auto& triangle : model->chunks)
    {
        drawTriangle(triangle.vertices); 
    }
}

void drawWireframeModel(STLModel* model)
{
    for(const auto& triangle : model->chunks)
    {
        drawWireframeLines(triangle.vertices); 
    }
}


int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cout << "No filepath provided. Exiting...\n";
        return 0; 
    }

    std::string filename = argv[1]; 
    Parser* parser = new Parser();
    STLModel model;  
    
    if(!parser->parse(filename))
    {
        std::cerr << "Failed to parse " << filename << std::endl;  
    }  

    printf("Successfully parsed %s in %4.3f sec\n", filename.c_str(), parser->tookHowLong());
    model = parser->getModel(filename); 
    std::cout << "Model has " << model.chunks.size() << " triangles to render" << std::endl;

    /************ SDL SETUP ****************/
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to init SDL with SDL_INIT_VIDEO. Exiting..." << std::endl; 
        return 1; 
    }

    SDL_Window* window = SDL_CreateWindow("STL-Viewer", 
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                        800, 800, 
                        SDL_WINDOW_OPENGL); 

    if(!window)
    {
        std::cerr << "Cant make window" << std::endl; 
        SDL_Quit(); 
        return 0; 
    }

    SDL_GLContext context = SDL_GL_CreateContext(window); 
    if(!context)
    {
        std::cout << "Cant make context" << std::endl; 
        SDL_DestroyWindow(window); 
        SDL_Quit(); 
        return 0;  
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);


    bool running = true; 
    SDL_Event event; 
    float angle = 0.0f; 

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false; 
            }
        }

        // Clear the screen and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the model-view matrix
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -55.0f); // Move back to see the cube
        glRotatef(angle, 1.0f, 1.0f, 0.0f); // Rotate the model
        angle += 0.5f; 

        if(&model)
        {
            drawWireframeModel(&model); 
        }
        
        SDL_GL_SwapWindow(window); 

    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window); 
    SDL_Quit();
}