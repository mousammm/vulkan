#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector> 
#include <cstring>  // strcmp

// validation layer 
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
// end 

// proxy function for vkGetInstanceProcAddr
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// another proxy for destroyig CreateDebugUtilsMessengerEXT
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


class HelloTriangleApplication {

private:
    GLFWwindow* window;
    VkInstance instance; // 02
    VkDebugUtilsMessengerEXT debugMessenger;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

public:
    void run() {
        initWindow();
        initVulkan();
    // void createInstance()
    // checkGLFWExtensionSupport()
    // checkValidationLayerSupport()
    // getRequiredExtensions()
    // debugCallback()
    // setupDebugMessenger()
    // populateDebugMessengerCreateInfo()
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
        setupDebugMessenger();

    }

    void createInstance() {
        // validation layers
         if (enableValidationLayers && !checkValidationLayerSupport()) {
             throw std::runtime_error("validation layers requested, but not available!\n\n");
         }

        // checks if extension vulkan suport or not
        if(!checkGLFWExtensionSupport()){
            throw std::runtime_error("GLFW required extension not supported!\n\n");
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
        
        // message callbacks
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // createInfo.enabledExtensionCount = glfwExtensionCount;
        // createInfo.ppEnabledExtensionNames = glfwExtensions;

        // validation layer 
        // if (enableValidationLayers) {
        //     createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        //     createInfo.ppEnabledLayerNames = validationLayers.data();
        // } else {
        //     createInfo.enabledLayerCount = 0;
        // } // end
        //
        // createInfo.enabledLayerCount = 0;

        // from populate fun
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
    
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
    
            createInfo.pNext = nullptr;
        }
    

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

    }

    bool checkGLFWExtensionSupport(){  
         // log available extension
        //  uint32_t extensionCount = 0;
        //  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        // 
        //  std::vector<VkExtensionProperties> extensions(extensionCount);
        //  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        //  std::cout << "available extensions:\n";
        //  
        //  for (const auto& extension : extensions) {
        //      std::cout << '\t' << extension.extensionName << '\n';
        //  }

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

    // checks if all of the requested layers are available 
    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr); // list of all the available layers
    
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
        // Debug: Print available layers
        std::cout << "Available validation layers:\n";
        for (const auto& layer : availableLayers) {
            std::cout << "\t" << layer.layerName << "\n";
        }

        for (const char* layerName : validationLayers) {
            bool layerFound = false;
        
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
        
            if (!layerFound) {
                std::cout << "Missing validation layer: " << layerName << "\n";
                return false;
            }
        }
        
        std::cout << "All validation layers are available!\n";
        return true;
    }

    // return the required list of extension based on whheter validation layers are enable or not
    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    
        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    
        return extensions;
    }

    // messanger callbacks 
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
    }
    
    void setupDebugMessenger() {
        if (!enableValidationLayers) return;

   //      VkDebugUtilsMessengerCreateInfoEXT createInfo{};
   //      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
   //      createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
   //      createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
   //      createInfo.pfnUserCallback = debugCallback;
   //      createInfo.pUserData = nullptr; // Optional
   //
   //      // call from the proxy before the class defination
   //     if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
   //         throw std::runtime_error("failed to set up debug messenger!");
   //     }
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    // extract population fo the messanger create info inato a separate fun
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
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
