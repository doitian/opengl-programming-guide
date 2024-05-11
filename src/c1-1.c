#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char *description)
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

GLuint load_shader_binary(const char *filename, GLenum type)
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

  glShaderBinary(1, &shader, GL_SPIR_V_BINARY, buffer, len);
  free(buffer);
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

  GLuint vert_shader = load_shader_binary("shaders/triangle.vert.spv", GL_VERTEX_SHADER);
  GLuint frag_shader = load_shader_binary("shaders/triangle.frag.spv", GL_FRAGMENT_SHADER);
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vert_shader);
  glAttachShader(shader_program, frag_shader);
  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

void draw(GLFWwindow *window)
{
  static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
  glClearBufferfv(GL_COLOR, 0, black);
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
}

int main(void)
{
  GLFWwindow *window;
  glfwSetErrorCallback(error_callback);

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
