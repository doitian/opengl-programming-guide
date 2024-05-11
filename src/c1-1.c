#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void errorCallback(int error, const char *description)
{
  printf("Error: %s\n", description);
}

#define NUM_VERTICES 6
#define BUFFER_OFFSET(offset) ((void *)(offset))

enum VAO_IDs
{
  Triangles,
  NumVAOs
};
enum Buffer_IDs
{
  ArrayBuffer,
  NumBuffers
};
enum Attrib_IDs
{
  vPosition = 0
};
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

GLuint loadShaderBinary(const char *filename, GLenum type)
{
  GLsizei len;
  void *buffer;
  GLuint shader = glCreateShader(type);

  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    fprintf(stderr, "Failed to open file %s\n", filename);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  len = ftell(file);
  fseek(file, 0, SEEK_SET);
  buffer = (void *)malloc((GLuint)len);
  if (!buffer)
  {
    fprintf(stderr, "Failed to allocate memory for file %s\n", filename);
    exit(1);
  }

  if (fread(buffer, 1, len, file) != len)
  {
    fprintf(stderr, "Failed to read file %s\n", filename);
    exit(1);
  }

  fclose(file);

  glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer, len);
  glSpecializeShader(shader, "main", 0, NULL, NULL);
  return shader;
}

void init(GLFWwindow *window)
{
  static const GLfloat vertices[NUM_VERTICES][2] = {
      {-0.90, -0.90}, // Triangle 1
      {0.85, -0.90},
      {-0.90, 0.85},
      {0.90, -0.85}, // Triangle 2
      {0.90, 0.90},
      {-0.85, 0.90}};

  glCreateBuffers(NumBuffers, Buffers);
  glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

  GLuint vertShader = loadShaderBinary("shaders/triangle.vert.spv", GL_VERTEX_SHADER);
  GLuint fragShader = loadShaderBinary("shaders/triangle.frag.spv", GL_FRAGMENT_SHADER);
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);
  GLint linked = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    GLint logSize = 0;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logSize);
    char *errorLog = (char *)malloc(logSize + 1);
    errorLog[logSize] = '\0';
    glGetProgramInfoLog(shaderProgram, logSize, NULL, errorLog);
    fprintf(stderr, "Error linking program: %s\n", errorLog);
    free(errorLog);
  }
  glUseProgram(shaderProgram);

  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

void draw(GLFWwindow *window)
{
  static const float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glClearBufferfv(GL_COLOR, 0, black);
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
}

int main(void)
{
  GLFWwindow *window;
  glfwSetErrorCallback(errorCallback);

  /* Initialize the library */
  if (!glfwInit())
  {
    return -1;
  }

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return -1;
  }

  init(window);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    draw(window);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
