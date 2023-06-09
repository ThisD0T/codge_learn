// glfw will include vulkan itself if defined like this here.
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// iostream and stdexcept are for reporting and propogating errors
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// a little object oriented example application
class HelloTriangleApplication {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  VkInstance instance;

  void initWindow() {
    glfwInit();
    glfwWindowHint(
        GLFW_CLIENT_API,
        GLFW_NO_API); // this is to make it so it doesn't make an OpenGL context
    glfwWindowHint(GLFW_RESIZABLE,
                   GLFW_FALSE); // this is to make it non-resizable because if
                                // it is you need to do special stuff

    // now you create the actual window
    window =
        glfwCreateWindow(800, 600, "Vulkan Window!", nullptr,
                         nullptr); // the third option specifies a monitor and
                                   // the fourth only has relevance to OpenGL
  }

  void createInstance() {

    if (enableValidationLayers && !checkValidationlayerSupport()) {
      throw std::runtime_error(
          "validation layers requested but not available!");
    }

    // this instance is the connection between the program and the vulkan
    // library
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
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (enableValidationLayers) {
      createInfo.enabledLayerCount =
          static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
      createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      throw std::runtime_error("failed to create instance!");
    }

    // object creation function parameters in vulkan are usually:
    // - Pointer to struct with creation info
    // - Pointer to custom allocator callbacks, which will be nullptr here
    // - Pointer to the variable that stores the handle to the new object

    // checking for extensions
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count);

    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count,
                                           extensions.data());

    // now the program lists all available extensions:
    for (const auto &extension : extensions) {
      std::cout << '\t' << extension.extensionName << '\n';
    }
  }

  void initVulkan() { createInstance(); }

  void mainLoop() {
    // this loop makes it so the window closes when an error occurs or when it
    // is closed
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
    }
  }

  // cleanup, self explanatory
  void cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  bool checkValidationlayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
      bool layerFound = false;

      for (const auto &layerProperties : availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
          layerFound = true;
          break;
        }
      }

      if (!layerFound) {
        return false;
      }
    }

    return true;
  }
};

int main() {
  std::cout << "enableValidationLayers: " << enableValidationLayers
            << std::endl;

  HelloTriangleApplication app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
