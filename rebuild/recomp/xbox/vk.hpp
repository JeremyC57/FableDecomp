// Vulkan core: dynamic loading (system loader or a custom driver), instance/device, the
// window swapchain and presentation. Used by the NV2A renderer.
#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cstdint>
#include <mutex>
#include <string>

struct SDL_Window;

namespace xb::vk {

#define XB_VK_INSTANCE_FNS(X)                                                                   \
    X(vkDestroyInstance) X(vkEnumeratePhysicalDevices) X(vkGetPhysicalDeviceProperties)          \
    X(vkGetPhysicalDeviceQueueFamilyProperties) X(vkGetPhysicalDeviceMemoryProperties)           \
    X(vkGetPhysicalDeviceFeatures) X(vkGetPhysicalDeviceFormatProperties) X(vkCreateDevice)      \
    X(vkGetDeviceProcAddr) X(vkEnumerateDeviceExtensionProperties)                               \
    X(vkGetPhysicalDeviceSurfaceSupportKHR) X(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)         \
    X(vkGetPhysicalDeviceSurfaceFormatsKHR) X(vkGetPhysicalDeviceSurfacePresentModesKHR)         \
    X(vkDestroySurfaceKHR)

#define XB_VK_DEVICE_FNS(X)                                                                      \
    X(vkDestroyDevice) X(vkGetDeviceQueue) X(vkDeviceWaitIdle) X(vkQueueSubmit) X(vkQueueWaitIdle) \
    X(vkCreateSwapchainKHR) X(vkDestroySwapchainKHR) X(vkGetSwapchainImagesKHR)                   \
    X(vkAcquireNextImageKHR) X(vkQueuePresentKHR) X(vkCreateCommandPool) X(vkDestroyCommandPool)  \
    X(vkAllocateCommandBuffers) X(vkFreeCommandBuffers) X(vkBeginCommandBuffer)                   \
    X(vkEndCommandBuffer) X(vkResetCommandBuffer) X(vkCreateFence) X(vkDestroyFence)              \
    X(vkGetFenceStatus) X(vkCreateQueryPool) X(vkDestroyQueryPool) X(vkGetQueryPoolResults) X(vkCmdResetQueryPool) X(vkCmdBeginQuery) X(vkCmdEndQuery) \
    X(vkWaitForFences) X(vkResetFences) X(vkCreateSemaphore) X(vkDestroySemaphore)               \
    X(vkCreateBuffer) X(vkDestroyBuffer) X(vkGetBufferMemoryRequirements) X(vkBindBufferMemory)   \
    X(vkCreateImage) X(vkDestroyImage) X(vkGetImageMemoryRequirements) X(vkBindImageMemory)       \
    X(vkCreateImageView) X(vkDestroyImageView) X(vkAllocateMemory) X(vkFreeMemory)               \
    X(vkMapMemory) X(vkUnmapMemory) X(vkFlushMappedMemoryRanges) X(vkInvalidateMappedMemoryRanges) \
    X(vkCmdPipelineBarrier) X(vkCmdCopyBufferToImage) X(vkCmdCopyImageToBuffer) X(vkCmdBlitImage)  \
    X(vkCmdClearColorImage) X(vkCmdClearDepthStencilImage) X(vkCmdCopyImage)                      \
    X(vkCreateSampler) X(vkDestroySampler) X(vkCreateShaderModule) X(vkDestroyShaderModule)       \
    X(vkCreatePipelineLayout) X(vkDestroyPipelineLayout) X(vkCreateGraphicsPipelines)             \
    X(vkDestroyPipeline) X(vkCreateDescriptorSetLayout) X(vkDestroyDescriptorSetLayout)          \
    X(vkCreateDescriptorPool) X(vkDestroyDescriptorPool) X(vkResetDescriptorPool)                \
    X(vkAllocateDescriptorSets) X(vkUpdateDescriptorSets) X(vkCreateRenderPass)                   \
    X(vkDestroyRenderPass) X(vkCreateFramebuffer) X(vkDestroyFramebuffer)                         \
    X(vkCmdBeginRenderPass) X(vkCmdEndRenderPass) X(vkCmdBindPipeline) X(vkCmdBindVertexBuffers)  \
    X(vkCmdBindIndexBuffer) X(vkCmdBindDescriptorSets) X(vkCmdPushConstants) X(vkCmdDraw)         \
    X(vkCmdDrawIndexed) X(vkCmdSetViewport) X(vkCmdSetScissor) X(vkCmdClearAttachments)          \
    X(vkCmdSetStencilReference) X(vkCmdSetBlendConstants) X(vkCmdSetDepthBias)                   \
    X(vkCreatePipelineCache) X(vkDestroyPipelineCache) X(vkGetPipelineCacheData)

#define XB_VK_DECL(f) extern PFN_##f f;
XB_VK_INSTANCE_FNS(XB_VK_DECL)
XB_VK_DEVICE_FNS(XB_VK_DECL)
#undef XB_VK_DECL

struct Settings {
    std::string driverPath;  // empty: system Vulkan loader; else a driver .so (custom ICD)
    void* driverHandle = nullptr;  // already-opened driver (Android: libadrenotools)
    bool vsync = true;
    bool validation = false;
};

struct Context {
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice phys = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
    uint32_t queueFamily = 0;
    VkPhysicalDeviceProperties props{};
    VkPhysicalDeviceMemoryProperties mem{};
    VkPhysicalDeviceFeatures features{};
    std::mutex queueLock;  // the renderer (pusher thread) and presentation share the queue
};
Context& ctx();

// Loads Vulkan, creates the instance and a device that can present to `window`.
bool init(SDL_Window* window, const Settings& s);
uint32_t memoryType(uint32_t bits, VkMemoryPropertyFlags want);
VkResult submit(const VkSubmitInfo& si, VkFence fence);

// Presentation: the image is shown letterboxed with `aspect` (width / height); layouts:
// the image must be in TRANSFER_SRC_OPTIMAL. Blocks for vsync when enabled.
void present(VkImage image, uint32_t width, uint32_t height, float aspect, VkSemaphore waitRendered);
void resize();  // window changed size
void surfaceChanged();  // the native window was replaced (Android: back from the background)

} // namespace xb::vk
