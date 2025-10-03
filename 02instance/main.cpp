#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector> 
#include <cstring>  // strcmp

class HelloTriangleApplication {

private:
    GLFWwindow* window;
    VkInstance instance; // 02
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance(); // 02

    }

    void createInstance() {
         // log available extension
         uint32_t extensionCount = 0;
         vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        
         std::vector<VkExtensionProperties> extensions(extensionCount);
         vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
         std::cout << "available extensions:\n";
         
         for (const auto& extension : extensions) {
             std::cout << '\t' << extension.extensionName << '\n';
         }

        // checks if extension vulkan suport or not
        if(!checkGLFWExtensionSupport()){
            throw std::runtime_error("GLFW required extension not supported!\n");
        }

        // create a instance that work with GLFW
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

    }

    bool checkGLFWExtensionSupport(){  
        // Get glfw required extension 
        uint32_t glfwExtensionCount = 0;
    
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
        // get available ext 
        uint32_t availableExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);  // Fixed function name
        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);  // Fixed variable name and constructor
        vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());  // Fixed function name
    
        // check each glfw extension
        for(uint32_t i = 0; i < glfwExtensionCount; i++){
            const char* requiredExt = glfwExtensions[i];  
            bool found = false;
            for(const auto& availableExt : availableExtensions){  
                if(strcmp(requiredExt, availableExt.extensionName) == 0){  
                    found = true;
                    break;
                }
            } // for end
    
            if(!found){
                std::cout << "missing GLFW req extension:" << requiredExt << "\n";
                return false;
            }
        }  
    
        std::cout << "ALL GLFW req ext are supported!\n";
        return true;
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
