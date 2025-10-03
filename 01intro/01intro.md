# Base code 

`#include <vulkan/vulkan.h>`
 inculde this from LunarG SDK,
 provides function structures and enumeration

`#include <iostream>`
`#include <stdexcept>`
 *these are for reporting errors

`#include <cstdlib>`
 *provides EXIT_SECCESS / EXIT_FAILURE macros

 * **initVulkan()**  = all the private member are vulkan object 
    (which will be called form initVulkan() )
 * **mainLoop()** = after everything has been prepared,we enter the main loop to render frames
    (we will enter the main loop and iterates until the window is closed in a moment)
 * **cleanup()** = after window is closed and mainLoop return we will delocate all the resource 

# resource management 
 * anything allocatated need to be delocated 
 * we can use RAII or smart pointers from <memory> header
 * but we will explictly destroy
 * C++ clases -> acquire vulkan object in Constructor -> relese them in destructor

# Creating and Destroying 
 * vulkan Object -> created vKCreateXXX | allocated vkAllocatedXXX
 * destroy vulkan Object after create -> vkDestroyXXX & vkFreeXXX 
 * all of these share pAllocator (optional parameter) specify callbacks for custom memory allocator = nullptr (for this tut)

# Integrating GLFW
 * replace #include <vulkan/vulkan.h> with 
   define GLFW_INCLUDE_VULKAN
   #include <GLFW/glfw3.h>

 * added a call in run()  
   void run(){
       initWindow();
   }
 * add a obj
   private:
       void initWindow(){
          glfwInit();

          glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // init GLFW library and tell not to create a OpenGl context
          glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // disable window resize (will take care later)
          
          window = glfwCreateWindow(800,600,"Vulkan tut",nullptr,nullptr); // (width,height,"window title",monitor to open the window (optional), for openGl)
       }

  * add a private class member to hold a refrence to it
    private:
    GLFWwindow* window;


  * mainloop
    void mainLoop() { // loops and checks for event
        while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
      }
    }

  * cleanup
    void cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

