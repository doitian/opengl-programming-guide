/**
 * Set up a window with the OpenGL context.
 */
#include <stdio.h>
#include <GLFW/glfw3.h>

void errorCallback(int error, const char *description)
{
  printf("Error: %s\n", description);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  printf("Key: %d, Scancode: %d, Action: %d, Mods: %d\n", key, scancode, action, mods);
}

int main(void)
{
  GLFWwindow *window;
  glfwSetErrorCallback(errorCallback);

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  printf("GL_VERSION = %s\n", glGetString(GL_VERSION));
  printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));

  glfwSetKeyCallback(window, keyCallback);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
