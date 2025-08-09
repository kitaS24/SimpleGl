# SimpleGl
### OPENGl (LEGACY) Quick Start File
***
A single file to easily start OpenGL Legacy projects.
> [!IMPORTANT]
> The Project uses **GLFW** and **GLEW**
## Features
- **Creating A Window**
- **2D And 3D Modes**
- **Getting FPS**
- **Loading RAW Texture** (RGB or RGBA)
- **Loading 24b BMP Texture** (RGB)
- **Loading 24b BMP Texture, Color KeyOut** (RGBA)
- **Creating And Compiling Shaders From Files** (TXT Files,any extension)

## Getting Started

1. Copy OpenGLRenderer.cpp into your project
2. Add the following to the end of the CMakeLists.txt file
```
set(GlInclude "include/") # GLFW,GLEW include folder (include/GLEW/,include/GLFW/)
include_directories(${GlInclude})
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/${GlInclude}/GLFW/glfw3.lib ${CMAKE_SOURCE_DIR}/${GlInclude}/GLEW/glew32s.lib OpenGL::GL)
```
 
3. change `set(GlInclude "include/")` to your folder with folders GLEW and GLFW <br>
**EXAMPLE**\
   `set(GlInclude "include/")` for project structure 
```
/include/GLFW
/include/GLEW
```
or<br>
`set(GlInclude "vendor/")` for project structure
```
/vendor/GLFW
/vendor/GLEW
```
4. Include the OpenGLRenderer.cpp file, by adding `#include "OpenGLRenderer.cpp"`
5. Copy example from README or from the end of the OpenGLRenderer.cpp file

## Functions

* **OpenGlInit**: used to initialize opengl
* **CreateGlWindow**: used to create window
* **OpenGlBeginFrame2D**: used to start frame in 2D mode
* **OpenGlBeginFrame3D**: used to start frame in 3D mode
* **OpenGlBeginFrame3DAlt**: used to start frame in 3D mode, allows to work in -z space
* **OpenGlErase**: used to erase previous frame and fill it with color
* **OpenGlGetFPS**: used to get FPS
* **Rotate3D**: used to rotate in the order Z,Y,X
* **OpenGlStipple**: used to create dotted line
* **OpenGlStippleCustom**: used to create custom dotted line
* **OpenGlDisableStipple**: used to disables dotted lines (same as `glDisable(GL_LINE_STIPPLE);`)
* **LoadRawTexture**: used to load RAW texture data from file
* **LoadBmpTexture**: used to load 24bit BMP file 
* **LoadBmpTexture**: used to load 24bit BMP file and KeyOut color (additional 4 parameters)
* **OpenGlCreateShaderProgram**: used to create shader by providing Vertex and Fragment file shader paths
* **RenderUVCube**: used to render cube with size and Uv scale

## Examples

**EXAMPLE 1**
```
#include <iostream>
#include "OpenGLRenderer.cpp"

int main() {
    OpenGlInit(true);

    //creating window with W 800 and H 600
    GLFWwindow* window;
    if(!CreateGlWindow(window,800,600,"Test")){
        exit(-1);
    }
    while(!glfwWindowShouldClose(window))
    {
        // clear and start in 2D mode
        OpenGlErase(0.0f,0.7f,1.0f,1.0f,true,false);
        OpenGlBeginFrame2D(800,600,-1,1);

        //rendering triangle with color (255,255,255)
        glColor3ub(255,255,255);

        glBegin(GL_TRIANGLES);
        glVertex2f(400,600);
        glVertex2f(800,0);
        glVertex2f(0,0);
        glEnd();
        //ending rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

```

**EXAMPLE 2**
```
#include <iostream>
#include "OpenGLRenderer.cpp"

float T = 0;
float FPS = 0;
float LastFrameT = 0;
int main() {
    OpenGlInit(true);

    //creating window with W 800 and H 600
    GLFWwindow* window;
    if(!CreateGlWindow(window,800,600,"Test")){
        exit(-1);
    }
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
        //rendering triangle with color (255,255,255) or with different colors for each vertex

        glBegin(GL_TRIANGLES);
        if(int(T/2)%2 == 0) {
            glColor3ub(255,255,255);
            glVertex3f(0, 3, 0);
            glVertex3f(4, -3, 0);
            glVertex3f(-4, -3, 0);
        }else{
            glColor3ub(255,0,0);
            glVertex3f(0, 3, 0);
            glColor3ub(0,255,0);
            glVertex3f(4, -3, 0);
            glColor3ub(0,0,255);
            glVertex3f(-4, -3, 0);
        }
        glEnd();

        //ending rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

```

**EXAMPLE 3**
```
#include <iostream>
#include <fstream>

#include "OpenGLRenderer.cpp"

float T = 0;
float FPS = 0;
float LastFrameT = 0;
GLuint Texture = 0;
int main() {
    OpenGlInit(true);

    //creating window with W 800 and H 600
    GLFWwindow* window;
    if(!CreateGlWindow(window,800,600,"Test")){
        exit(-1);
    }
    //loading a texture
    //Texture = LoadRawTexture("tx.data",256,256,false,true);
    Texture =LoadBmpTexture("512x512Test.bmp",false,false);
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
        //binding a texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Texture);
        //rendering triangle with color (255,255,255) or with different colors for each vertex
        glBegin(GL_TRIANGLES);

        if(int(T/2)%2 == 0) {
            glColor3ub(255,255,255);
            glTexCoord2f(0.5,1);
            glVertex3f(0, 3, 0);
            glTexCoord2f(1,0);
            glVertex3f(4, -3, 0);
            glTexCoord2f(0,0);
            glVertex3f(-4, -3, 0);

        }else{
            glColor3ub(255,0,0);
            glTexCoord2f(0.5,1);
            glVertex3f(0, 3, 0);
            glColor3ub(0,255,0);
            glTexCoord2f(1,0);
            glVertex3f(4, -3, 0);
            glColor3ub(0,0,255);
            glTexCoord2f(0,0);
            glVertex3f(-4, -3, 0);
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);

        //ending rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
```

**EXAMPLE 4**
```
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
    if(!CreateGlWindow(window,800,600,"Test")){
        exit(-1);
    }
    //loading a texture
    Texture =LoadBmpTexture("512x512Test.bmp",true,false);
    //creating a shader
    Shader = OpenGlCreateShaderProgram("default.vert.glsl","invert.frag.glsl");
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
        RenderUVCube(4,2);
        glDisable(GL_TEXTURE_2D);

        //ending rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
```

# License
**This project is licensed under the MIT License**

## Third-Party Licenses
**This project uses third-party libraries which have their own licenses**

* [GLFW](licenses/GLFW)
* [GLEW](licenses/GLEW)