#pragma once
//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "imGuIZMOquat.h"
#include "../Camera.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#define GLM_FORCE_RADIANS //使用弧度作为单位
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //glm库投影矩阵使用opengl深度值范围（-1.0，-1.0）,该宏使用（0,1）
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Windows.h>
#undef max
#undef min
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <functional>
#include <set>
#include <array>
#include <unordered_map>


#define MAX_PATH 260
const int MAX_FRAMES_IN_FLIGHT = 2; //最大并行渲染帧数
const int MODEL_VERTEXBUFFER_COUNT = 2;


//是否启用校验层
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
	bool operator==(const Vertex& other) const {
		return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos)
				^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1)
				^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	float ambientValue;
	float powValue;
};

struct BlinnPhongObject {
	bool BlinnPhong;
	glm::vec3 lightPosition;
	glm::vec3 CameraPositon;
};



struct QueueFamilyIndices {
	uint32_t graphicsFamily = -1;
	uint32_t presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class ModelLoader {

public:
	const char* MODEL_PATH = " ";
	const char* Last_MODEL_PATH = " ";
	const char* TEXTURE_PATH = "../ModelLoader/res/texture/gray.png";

	//imgui 
	ImDrawData* imgui_draw_data;
	std::string path;
	bool IsModelLoaded = false;
	bool IsTextureLoaded = false;
	float last_f = 0.0f;
	int CURRENT_MODEL_VERTEXBUFFER_INDEX = 0;
	int CURRENT_TEXTURE_INDEX = 0;
	bool First_Loaded = true;
	bool Clean_Last_Buffer = false;
	bool Clean_Last_Texture = false;
	ImGui_ImplVulkan_InitInfo init_info = {};
	float scale = 0.0f;

	const int WIDTH = 1440;
	const int HEIGHT = 900;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	int VertexNums = 0;
	int Triangles = 0;

	/* camera */
	Camera camera;

	/* blinn-phong */
	bool Blinn_Phong = false;
	float ambientValue = 0.05;
	float powValue = 32.0;

	/* light Positon */
	glm::vec3 lightPosition = glm::vec3(0.0f, 3.0f, 2.0f);

	/* track rotations */
	quat qRot = quat(1.0f, 0.0f, 0.0f, 0.0f);
	vec3 PanDolly = vec3(0.0f);

	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device; 

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkCommandPool commandPool;

	/* 当前正在渲染的模型以及下一个要渲染模型的buffers */
	std::vector<VkBuffer> vertexBuffer;
	std::vector<VkDeviceMemory> vertexBufferMemory;
	std::vector<VkBuffer> indexBuffer;
	std::vector<VkDeviceMemory> indexBufferMemory;



	std::vector<VkCommandBuffer> commandBuffers;

	//msaa
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	VkDescriptorSetLayout descriptorSetLayout;
	/* ubo */
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	/* bpo */
	std::vector<VkBuffer> blinphongBuffers;
	std::vector<VkDeviceMemory> blinphongBuffersMemory;

	VkDescriptorPool descriptorPool;
	VkDescriptorPool imgui_descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	/* mipmap */
	uint32_t mipLevels;
	std::vector<VkImage> textureImage;
	std::vector<VkDeviceMemory> textureImageMemory;
	std::vector<VkImageView> textureImageView;
	VkSampler textureSampler;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	/* 每一帧独立的信号量 */
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	bool framebufferResized = false;//窗口大小是否改变

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	void run();

	void initWindow();

	void initVulkan();

	void createDescriptorSetLayout();

	void createUniformBuffers();

	void createBlinPhongBuffers();

	void createDescriptorPool();

	void createImGuiDescriptorPool();

	void createDescriptorSets();

	void UpdateDescriptorSets();

	void createTextureImage(uint32_t imageIndex);

	void createTextureImageView(uint32_t imageIndex);

	void createTextureSampler();

	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void createDepthResources();

	VkFormat findDepthFormat();

	bool hasStencilComponent(VkFormat format);

	void loadModel();

	float findScale(std::vector<float>& vec);

	void createColorResources();//msaa

	void mainLoop();

	void cleanupSwapChain();

	std::vector<char> readFile(const std::string& filename);

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	void cleanup();

	void recreateSwapChain();

	void createInstance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void setupDebugMessenger();

	void createSurface();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSwapChain();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	void createImageViews();

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void createRenderPass();

	void createGraphicsPipeline();

	void createFramebuffers();

	void createCommandPool();

	/* 初始化vertex && index buffer */
	void initBuffers();

	/* 创建顶点缓冲 */
	void createVertexBuffer(int index);

	/* 创建索引缓冲 */
	void createIndexBuffer(int index);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void createCommandBuffers();

	void imguiFrameRender(ImDrawData* draw_data, uint32_t imageindex, int modelindex);

	void createSyncObjects();

	void drawFrame();
	//update uniform buffer
	void updateUniformBuffer(uint32_t currentImage);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	bool isDeviceSuitable(VkPhysicalDevice device);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	std::vector<const char*>getRequiredExtensions();

	bool checkValidationLayerSupport();

	//msaa
	virtual VkSampleCountFlagBits getMaxUsableSampleCount();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	/* 窗口自适应static 舍弃 this指针 */
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<ModelLoader*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	//mipmaps
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);


	/* 初始化camera */
	void initCamera();

	/* vgm to glm */
	glm::mat4 vgmCast(mat4 vgmModel, glm::mat4& model);

	/* 加载模型 or 纹理 */
	void loadFile(OPENFILENAME& file, char* filePath);

	/* 设置/获取 顶点/三角形数 */
	void setVertexNums(int nums);
	void setTriangles(int nums);
	int getVertexNums()const; 
	int getTriangles()const; 

};