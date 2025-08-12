//
// Created by kitaS24 on 06.08.2025.
//

//Add this to the end of a C Make file

/*

set(GlInclude "include/") # GLFW,GLEW include folder (include/GLEW/,include/GLFW/)
include_directories(${GlInclude})
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/${GlInclude}/GLFW/glfw3.lib ${CMAKE_SOURCE_DIR}/${GlInclude}/GLEW/glew32s.lib OpenGL::GL)

*/

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGlInit(bool GlLegacy){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    if(GlLegacy){
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }else{
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

}

bool CreateGlWindow(GLFWwindow* &window,unsigned int W,unsigned int H,const char *Title){
    //return:TRUE-window is created, FALSE-Failed
    window = glfwCreateWindow(W,H, Title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "\\033[1;31m Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, W, H);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    return true;
}

void OpenGlBeginFrame2D(float W,float H,float Zn/*Z near*/,float Zf/*Z far*/){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, W, H, 0, Zn, Zf);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGlErase(float ClearColorR,float ClearColorG,float ClearColorB,float ClearColorA,bool frameClearColor,bool frameClearDepth){
    glClearColor(ClearColorR,ClearColorG,ClearColorB, ClearColorA);
    GLbitfield Mask = 0;
    if(frameClearColor){
        Mask = GL_COLOR_BUFFER_BIT;
    }
    if(frameClearDepth){
        Mask =Mask| GL_DEPTH_BUFFER_BIT;
    }
    glClear(Mask);
}

void OpenGlBeginFrame3D(GLFWwindow *window,float W,float H,float Zf/*Z far*/,bool UseWindowSize){
    int X=0;
    int Y = 0;
    glfwGetWindowSize(window, &X, &Y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blend
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(UseWindowSize){
    glFrustum(-1.0, 1.0, -1.0 * Y / X, 1.0 * Y / X, 1,Zf);
    }else {
        glFrustum(-1.0, 1.0, -1.0 * H / W, 1.0 * H / W, 1, Zf);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1,1,-1);//flip Z
}
void OpenGlBeginFrame3DAlt(GLFWwindow *window,float W,float H,float Zf/*Z far*/,bool UseWindowSize,bool NotFlip){
    int X=0;
    int Y = 0;
    glfwGetWindowSize(window, &X, &Y);
    //set NotFlip to true to work in the -z space
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blend
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(UseWindowSize){
    glFrustum(-1.0, 1.0, -1.0 * Y / X, 1.0 * Y / X, 1,Zf);
    } else {
        glFrustum(-1.0, 1.0, -1.0 * H / W, 1.0 * H / W, 1, Zf);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(!NotFlip) {
        glScalef(1, 1, -1);//flip Z
    }
}
float OpenGlGetFPS(float &LastTime){
    double newT = glfwGetTime();
    float getfps = 1.0f / (newT - LastTime);
    if (newT > 1000) {

        glfwSetTime(0);
        newT = glfwGetTime();

    }
    LastTime = newT;
    return getfps;
}
void Rotate3D(float RtX,float RtY,float RtZ){
    glRotatef(RtZ,0,0,1);
    glRotatef(RtY,0,1,0);
    glRotatef(RtX,1,0,0);
}
void OpenGlStipple(){
    glLineStipple(1, 0xCCCC);
    glEnable(GL_LINE_STIPPLE);
}
void OpenGlStippleCustom(short pattern){
    glLineStipple(1, pattern);
    glEnable(GL_LINE_STIPPLE);
}
void OpenGlDisableStipple(){
    glDisable(GL_LINE_STIPPLE);
}
GLuint CreateTx(unsigned int W,unsigned int H,unsigned char* data,bool GlLinear,bool RGBA){
    GLuint texture;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGB+RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST+GlLinear);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST+GlLinear);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return texture;
}
GLuint LoadRawTexture(std::string FilePath,int W,int H,bool Interpolate,bool RGBA){

    unsigned char *TxBuffer;
    TxBuffer = new unsigned char [W*H*(3+RGBA)];
    unsigned char *TxBufferFlipped;
    TxBufferFlipped = new unsigned char [W*H*(3+RGBA)];

    std::ifstream File { FilePath, std::ios::binary};
    if (!File.is_open()) {
        std::cout <<"File is absent from path : "<<FilePath<<"\n";
    }else {
        File.read(reinterpret_cast<char *>(TxBuffer), W*H*(3+RGBA));
        File.close();
    }
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < (3+RGBA)*W; ++j) {
            *(TxBufferFlipped+((3+RGBA)*W*(H-1-i))+j) = *(TxBuffer+((3+RGBA)*W*i)+j);
        }
    }
    GLuint Tx = CreateTx(W,H,TxBufferFlipped,Interpolate,RGBA);
    delete[] TxBuffer;
    delete[] TxBufferFlipped;
    return Tx;
}

GLuint LoadBmpTexture(std::string FilePath,bool Interpolate,bool RGBA,bool EnableKeyOutColor,
                      unsigned char KeyOutR,unsigned char KeyOutG,unsigned char KeyOutB) {
    // 24BIT COLOR ONLY
    unsigned int W =0;
    unsigned int H =0;
    unsigned int FileSize = 0;
    unsigned int DataOffset = 0;
    unsigned int PxPointer = 0;
    unsigned int PxReadBytes = 0;


    unsigned char *TxBuffer;


    std::ifstream File { FilePath, std::ios::binary};
    if (!File.is_open()) {
        std::cout <<"File is absent from path : "<<FilePath<<"\n";
        return 0;
    }
    /*
    for (int q = 0; q < 2; ++q) {
        std::cout << char(File.get())<<"\n";
    }*/
    if(File.get() != 'B'){return 0;}
    if(File.get() != 'M'){return 0;}
    FileSize = File.get();
    FileSize =FileSize| (File.get()<< 8);
    FileSize =FileSize| (File.get()<< 16);
    FileSize =FileSize| (File.get()<< 24);
    File.seekg(4, std::ios::cur);

    DataOffset = File.get();
    DataOffset =DataOffset| (File.get()<< 8);
    DataOffset =DataOffset| (File.get()<< 16);
    DataOffset =DataOffset| (File.get()<< 24);

    File.seekg(18, std::ios::beg);

    W = File.get();
    W =W| (File.get()<< 8);
    W =W| (File.get()<< 16);
    W =W| (File.get()<< 24);

    H = File.get();
    H =H| (File.get()<< 8);
    H =H| (File.get()<< 16);
    H =H| (File.get()<< 24);
    TxBuffer = new unsigned char [W*H*(3+RGBA)];
    File.seekg(DataOffset, std::ios::beg);

    for (int i = 0; i < H; ++i) {
        for (int i = 0; i < W; ++i) {

                *(TxBuffer + PxPointer + 2) = File.get();
                *(TxBuffer + PxPointer + 1) = File.get();
                *(TxBuffer + PxPointer + 0) = File.get();
                if(EnableKeyOutColor && RGBA) {
                if(KeyOutR == *(TxBuffer + PxPointer + 0)&&
                   KeyOutG == *(TxBuffer + PxPointer + 1)&&
                   KeyOutB == *(TxBuffer + PxPointer + 2)){
                    *(TxBuffer + PxPointer + 3) = 0;
                    }else{
                    *(TxBuffer + PxPointer + 3) = 255;
                }
                }
                PxPointer = PxPointer + (3 + RGBA);

            PxReadBytes = PxReadBytes+3;
        }
        if(PxReadBytes%4 !=0){
            for (int j = 0; j < 4-(PxReadBytes%4); ++j) {
                File.get();
            }
            PxReadBytes =0;
        }
    }

    //std::cout << FileSize<<"\n";
    GLuint Tx = CreateTx(W,H,TxBuffer,Interpolate,RGBA);
    delete[] TxBuffer;
    return Tx;
}
GLuint LoadBmpTexture(std::string FilePath,bool Interpolate,bool RGBA) {
    // 24BIT COLOR ONLY
    unsigned int W =0;
    unsigned int H =0;
    unsigned int FileSize = 0;
    unsigned int DataOffset = 0;
    unsigned int PxPointer = 0;
    unsigned int PxReadBytes = 0;


    unsigned char *TxBuffer;


    std::ifstream File { FilePath, std::ios::binary};
    if (!File.is_open()) {
        std::cout <<"File is absent from path : "<<FilePath<<"\n";
        return 0;
    }
    /*
    for (int q = 0; q < 2; ++q) {
        std::cout << char(File.get())<<"\n";
    }*/
    if(File.get() != 'B'){return 0;}
    if(File.get() != 'M'){return 0;}
    FileSize = File.get();
    FileSize =FileSize| (File.get()<< 8);
    FileSize =FileSize| (File.get()<< 16);
    FileSize =FileSize| (File.get()<< 24);
    File.seekg(4, std::ios::cur);

    DataOffset = File.get();
    DataOffset =DataOffset| (File.get()<< 8);
    DataOffset =DataOffset| (File.get()<< 16);
    DataOffset =DataOffset| (File.get()<< 24);

    File.seekg(18, std::ios::beg);

    W = File.get();
    W =W| (File.get()<< 8);
    W =W| (File.get()<< 16);
    W =W| (File.get()<< 24);

    H = File.get();
    H =H| (File.get()<< 8);
    H =H| (File.get()<< 16);
    H =H| (File.get()<< 24);
    TxBuffer = new unsigned char [W*H*(3+RGBA)];
    File.seekg(DataOffset, std::ios::beg);

    for (int i = 0; i < H; ++i) {
        for (int i = 0; i < W; ++i) {

            *(TxBuffer + PxPointer + 2) = File.get();
            *(TxBuffer + PxPointer + 1) = File.get();
            *(TxBuffer + PxPointer + 0) = File.get();
            PxPointer = PxPointer + (3 + RGBA);

            PxReadBytes = PxReadBytes+3;
        }
        if(PxReadBytes%4 !=0){
            for (int j = 0; j < 4-(PxReadBytes%4); ++j) {
                File.get();
            }
            PxReadBytes =0;
        }
    }

    //std::cout << FileSize<<"\n";
    GLuint Tx = CreateTx(W,H,TxBuffer,Interpolate,RGBA);
    delete[] TxBuffer;
    return Tx;
}

std::string LoadShaderFile(std::string path) {
    std::string Out = "";
    std::string Line = "";

    std::ifstream FR(path);
    while (getline (FR, Line)) {
        for (int i = 0; i < Line.length(); ++i) {
            Out.append(1,Line.at(i));
        }
        Out.append(1,'\n');
    }
    FR.close();
    return Out;
}
GLuint CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
    }

    return shader;
}
GLuint OpenGlCreateShaderProgram(std::string vertPath, std::string fragPath) {
    GLuint vShader = CompileShader(GL_VERTEX_SHADER, LoadShaderFile(vertPath));
    GLuint fShader = CompileShader(GL_FRAGMENT_SHADER, LoadShaderFile(fragPath));
    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Linking error:\n" << log << std::endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return program;
}

void RenderUVCube(float s,float UvScale){
    //renders cube at origin
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex3f(-s,-s,-s);
    glTexCoord2f(UvScale,0);glVertex3f(s,-s,-s);
    glTexCoord2f(UvScale,UvScale);glVertex3f(s,s,-s);
    glTexCoord2f(0,UvScale);glVertex3f(-s,s,-s);

    glTexCoord2f(0,0);glVertex3f(-s,-s,s);
    glTexCoord2f(UvScale,0);glVertex3f(s,-s,s);
    glTexCoord2f(UvScale,UvScale);glVertex3f(s,s,s);
    glTexCoord2f(0,UvScale);glVertex3f(-s,s,s);

    glTexCoord2f(0,0);glVertex3f(-s,-s,-s);
    glTexCoord2f(UvScale,0);glVertex3f(s,-s,-s);
    glTexCoord2f(UvScale,UvScale);glVertex3f(s,-s,s);
    glTexCoord2f(0,UvScale);glVertex3f(-s,-s,s);

    glTexCoord2f(0,0);glVertex3f(-s,s,-s);
    glTexCoord2f(UvScale,0);glVertex3f(s,s,-s);
    glTexCoord2f(UvScale,UvScale);glVertex3f(s,s,s);
    glTexCoord2f(0,UvScale);glVertex3f(-s,s,s);

    glTexCoord2f(0,0);glVertex3f(-s,-s,-s);
    glTexCoord2f(UvScale,0);glVertex3f(-s,s,-s);
    glTexCoord2f(UvScale,UvScale);glVertex3f(-s,s,s);
    glTexCoord2f(0,UvScale);glVertex3f(-s,-s,s);

    glTexCoord2f(0,0);glVertex3f(s,-s,-s);
    glTexCoord2f(UvScale,0);glVertex3f(s,s,-s);
    glTexCoord2f(UvScale,UvScale);glVertex3f(s,s,s);
    glTexCoord2f(0,UvScale);glVertex3f(s,-s,s);

    glEnd();
}

/*
 * EXAMPLE 1
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
 */

/*
 * EXAMPLE 2
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

 */

/*EXAMPLE 3
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
*/

/*EXAMPLE 4
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
*/
