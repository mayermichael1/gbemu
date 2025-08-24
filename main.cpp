#include <stdio.h>

#include "include/general.h"
#include <GLFW/glfw3.h>

s32 
main (void)
{
    GLFWwindow *window = NULL;
    b8 running = true;

    if(glfwInit())
    {
        window = glfwCreateWindow(160, 144, "GB-Screen", NULL, NULL);
    }

    glfwMakeContextCurrent(window);

    if(window)
    {
        while(running)
        {
            glfwPollEvents();
            if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
                glfwWindowShouldClose(window))
            {
                running = false;
            }
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
        }
    }
    return 0;
}
