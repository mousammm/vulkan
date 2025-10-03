# wtf is validation layers ?
Validation layers are optional components that hook into Vulkan function calls to apply additional operations. Common operations in validation layers are:


# 1st add a toggle switch 
* this we allow us to enable validation layer and remove in release build
```c
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
```

# 2nd  add a fun to check validation layers
```c
    // checks if all of the requested layers are available 
    bool checkValidationLayerSupport() {
    }


```

# 3rd call the fun in createInstance fun()
```c
         if (enableValidationLayers && !checkValidationLayerSupport()) {
             throw std::runtime_error("validation layers requested, but not available!\n\n");
         }


```

# 4rd replace the line for toggle to work

```c
        // createInfo.enabledLayerCount = 0; // remove this

        // validation layer 
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        } // end

```

# message callbacks

# 5th add this fun  getRequiedExtension()

# 6 th replace 
```c
        // message callbacks
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // createInfo.enabledExtensionCount = glfwExtensionCount;
        // createInfo.ppEnabledExtensionNames = glfwExtensions;

```

# 7 add another stsic fun debugCallback()
# 8 add this 
private:
    VkDebugUtilsMessengerEXT debugMessenger;
# 9 setupdebugMessenger
    void initVulkan() {
        createInstance(); // 02
        setupDebugMessenger();
# 10 add fun setupdebugMessenger
void setupDebugMessenger() {
    if (!enableValidationLayers) return;

}
# add proxy
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
}
# call the proxy in setDebugMessanger
        // call from the proxy before the class defination
       if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
           throw std::runtime_error("failed to set up debug messenger!");
       }


# another proxy for destroyig CreateDebugUtilsMessengerEXT
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
}
# add proxy to cleanup 
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }


# debugging instance creation and destruction

# 1 add fun to extract population of the messanger populateDebugMessengerCreateInfo()

# replace in setup debugger
VkDebugUtilsMessengerCreateInfoEXT createInfo;
populateDebugMessengerCreateInfo(createInfo);

if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
throw std::runtime_error("failed to set up debug messenger!");
}

# replace createInstance
        createInfo.pApplicationInfo = &appInfo;
        // appInfo.pApplicationName = "Hello Triangle";

+ add this before vkCreateInstance
        // from populate fun
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
        } else {
        }

+ commment this 
// validation layer 
        // if (enableValidationLayers) {
        //     createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        //     createInfo.ppEnabledLayerNames = validationLayers.data();
        // } else {
        //     createInfo.enabledLayerCount = 0;
        // } // end
        //
        /
