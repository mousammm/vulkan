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
