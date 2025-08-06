#include <iostream>
#include <fstream>

#include "OpenGLRenderer.cpp"

float T = 0;
float FPS = 0;
float LastFrameT = 0;
GLuint Texture = 0;
GLuint Shader  = 0;
int main() {
    OpenGlInit(true);

    //creating window with W 800 and H 600
    GLFWwindow* window;
    if(!CreateWindow(window,800,600,"Test")){
        exit(-1);
    }
    //loading a texture
    Texture =LoadBmpTexture("512x512Test.bmp",true,false);
    //creating a shader
    Shader = OpenGlCreateShaderProgram("default.vert.glsl","invert.frag.glsl");
    glfwSwapInterval(1);
    while(!glfwWindowShouldClose(window))
    {
        // clear and start in 3D mode
        OpenGlErase(0.0f,0.7f,1.0f,1.0f,true,true);
        OpenGlBeginFrame3D(window,800,600,2000,true);

        //get FPS and time from start
        FPS = OpenGlGetFPS(LastFrameT);
        T = T+(1.0f/FPS);

        //rotate and transform triangle
        glTranslatef(0,0,10);
        Rotate3D(T*20.0f,T*90.0f,T*30.0f);
        //using shader and binding a texture
        glUseProgram(Shader);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Texture);
        //rendering triangle with color (255,255,255) or with different colors for each vertex
        RenderUVCube(4,1);
        glDisable(GL_TEXTURE_2D);

        //ending rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
