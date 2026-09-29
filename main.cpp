#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

int main(void){
   
    if(!glfwInit()){
        fprintf(stderr, "ERROR: Could not start GLFW.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"Hello World",NULL,NULL);
    if (!window){
        fprintf(stderr, "ERROR: Could not open window with GLFW.\n");
        return 1;
    }

    glfwMakeContextCurrent(window);

    int glad_ver = gladLoadGL(glfwGetProcAddress);
    if (glad_ver==0){
        fprintf(stderr, "ERROR: Failed to initialize OpenGL context.\n");
        return 1;
    }

    printf("LOADED OPENGL %i.%i\n", GLAD_VERSION_MAJOR(glad_ver), GLAD_VERSION_MINOR(glad_ver));

    printf("Renderer: %s.\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));



    float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0
    };

    GLuint vbo = 0;
    glGenBuffers( 1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);




    glfwTerminate();
    return 0;
}

