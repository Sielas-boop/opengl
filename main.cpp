
#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>




void glfw_error_callback(int code,const char* description){
  fprintf(stderr, "GLFW ERROR: Code %i Message %s\n", code, description);
}

int main(){

  printf("Starting GLFW: Version %s\n", glfwGetVersionString());
  glfwSetErrorCallback(glfw_error_callback);

  /* WINDOW SETUP */
    if(!glfwInit()){
        fprintf(stderr, "ERROR: Could not start GLFW.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 16);

    bool full_screen = false;
    GLFWmonitor *monitor = NULL;
    int w=800, h=600;

    if(full_screen){
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode -> refreshRate);

        w=mode->width;
        h=mode->height;
    }
    GLFWwindow* window = glfwCreateWindow(w,h,"Hello World",monitor,NULL);

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
        -0.5f, -0.5f, 0.0f
    };

    /* CREATING SHADERS */
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

    const char* vertex_shader=
    "#version 410 core\n"
    "in vec3 vp;"
    "void main(){"
    " gl_Position=vec4(vp,1.0);"
    "}";

    const char* fragment_shader=
    "#version 410 core\n"
    "out vec4 frag_colour;"
    "void main() {"
    "frag_colour=vec4(.5,0.0,.5,1.0);"
    "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    int params = -1;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
    if(GL_TRUE !=params){
        int max_length = 2048, actual_length=0;
        char slog[2048];
        glGetShaderInfoLog(vs, max_length, &actual_length, slog);
        fprintf(stderr, "ERROR: Shader index %u did not compile.\n%s\n", vs, slog);
        return 1;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params)
    {
        int max_length = 2048, actual_length = 0;
        char slog[2048];
        glGetShaderInfoLog(fs, max_length, &actual_length, slog);
        fprintf(stderr, "ERROR: Shader index %u did not compile.\n%s\n", fs, slog);
        return 1;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);
    // glDeleteProgram(fs); delete shader
    // glDeleteShader(); delete shader program
    // glDeleteVertexArrays(1, &vao); delete single VAO
    // glDeleteBuffers(1, &vbo); delete single VBO


    glGetProgramiv(shader_program, GL_LINK_STATUS, &params);
    if(GL_TRUE!=params){
        int max_length = 2048, actual_length = 0;
        char plog[2048];
        glGetShaderInfoLog(shader_program, max_length, &actual_length, plog);
        fprintf(stderr, "ERROR: Could not link shader program index %u.\n%s\n", shader_program, plog);
        return 1;
    }

    double prev_time=glfwGetTime();
    double title_ctd_time=0.1;

    /* DRAWING ON SCREEN */ 
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        
        glfwGetWindowSize(window, &w, &h);
        glViewport(0,0,w,h);

        double curr_time = glfwGetTime();
        double elapsed_time = curr_time - prev_time;
        prev_time = curr_time;
            
        title_ctd_time -= elapsed_time;
        if (title_ctd_time<=0.0 && elapsed_time >0.0){
            double fps = 1/elapsed_time;
            char tmp[256];
            sprintf(tmp, "FPS %.2lf", fps);
            glfwSetWindowTitle(window, tmp);
            title_ctd_time = 0.1;
        }
        glClearColor(0.5f, .5f, .8f, .8f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glfwSwapInterval(0); //n screen updates before swapping buffers; 0: swap immediately, 1: normal monitor refresh
        glfwSwapBuffers(window);

        if (GLFW_PRESS==glfwGetKey(window, GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, 1); 
        }
    } 

    glfwTerminate();
    return 0;
}

