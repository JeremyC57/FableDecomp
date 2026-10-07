// Vulkan core: loading, instance, device, swapchain, presentation.
#include "vk.hpp"
#include "xhost.hpp"

#include <SDL.h>
#include <SDL_syswm.h>
#include <dlfcn.h>

#include <algorithm>
#include <vector>

#if defined(__ANDROID__)
#include <vulkan/vulkan_android.h>
#else
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#include <vulkan/vulkan_wayland.h>
#endif

namespace xb::vk {

#define XB_VK_DEF(f) PFN_##f f;
XB_VK_INSTANCE_FNS(XB_VK_DEF)
XB_VK_DEVICE_FNS(XB_VK_DEF)
#undef XB_VK_DEF

namespace {
Context g_ctx;
Settings g_set;
PFN_vkGetInstanceProcAddr g_gipa;
SDL_Window* g_window;
VkSurfaceKHR g_surface = VK_NULL_HANDLE;

struct Swap {
    VkSwapchainKHR chain = VK_NULL_HANDLE;
    VkFormat format = VK_FORMAT_B8G8R8A8_UNORM;
    VkExtent2D extent{};
    std::vector<VkImage> images;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer cmd[3]{};
    VkFence fence[3]{};
    VkSemaphore acquired[3]{}, done[3]{};
    uint32_t frame = 0;
    bool dirty = true;
    bool surfaceLost = false;  // the window's surface went away (Android: background, rotation)
    bool rotated = false;      // IDENTITY swapchain on a rotated display: SUBOPTIMAL is expected
    int failures = 0;          // swapchain creations failed in a row (logged, then rate-limited)
} g_swap;

bool loadLoader() {
    void* lib = g_set.driverHandle;
    if (!lib && !g_set.driverPath.empty()) {
        lib = dlopen(g_set.driverPath.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!lib) XLOG(0, "Vulkan: cannot load driver %s (%s); using the system loader", g_set.driverPath.c_str(), dlerror());
    }
    if (!lib) {
#if defined(__ANDROID__)
        lib = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#else
        lib = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
        if (!lib) lib = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#endif
    }
    if (!lib) return false;
    g_gipa = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(lib, "vkGetInstanceProcAddr"));
    // A bare driver (ICD) exports its entry point under the ICD name.
    if (!g_gipa) g_gipa = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(lib, "vk_icdGetInstanceProcAddr"));
    return g_gipa != nullptr;
}

bool createSurface() {
    SDL_SysWMinfo wm;
    SDL_VERSION(&wm.version);
    if (!SDL_GetWindowWMInfo(g_window, &wm)) return false;
#if defined(__ANDROID__)
    auto create = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(g_gipa(g_ctx.instance, "vkCreateAndroidSurfaceKHR"));
    VkAndroidSurfaceCreateInfoKHR ci{VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR};
    ci.window = wm.info.android.window;
    if (!ci.window || !create) return false;
    const VkResult r = create(g_ctx.instance, &ci, nullptr, &g_surface);
    if (r != VK_SUCCESS) XLOG(0, "Vulkan: vkCreateAndroidSurfaceKHR failed (%d)", static_cast<int>(r));
    return r == VK_SUCCESS;
#else
    if (wm.subsystem == SDL_SYSWM_WAYLAND) {
        auto create = reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>(g_gipa(g_ctx.instance, "vkCreateWaylandSurfaceKHR"));
        VkWaylandSurfaceCreateInfoKHR ci{VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR};
        ci.display = wm.info.wl.display;
        ci.surface = wm.info.wl.surface;
        return create && create(g_ctx.instance, &ci, nullptr, &g_surface) == VK_SUCCESS;
    }
    auto create = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(g_gipa(g_ctx.instance, "vkCreateXlibSurfaceKHR"));
    VkXlibSurfaceCreateInfoKHR ci{VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR};
    ci.dpy = wm.info.x11.display;
    ci.window = wm.info.x11.window;
    return create && create(g_ctx.instance, &ci, nullptr, &g_surface) == VK_SUCCESS;
#endif
}

void destroySwapchain() {
    if (!g_swap.chain) return;
    vkDeviceWaitIdle(g_ctx.device);
    vkDestroySwapchainKHR(g_ctx.device, g_swap.chain, nullptr);
    g_swap.chain = VK_NULL_HANDLE;
    g_swap.images.clear();
}

bool createSurface();

bool swapchainFailed(const char* what, VkResult r) {
    if (r == VK_ERROR_SURFACE_LOST_KHR || r == VK_ERROR_NATIVE_WINDOW_IN_USE_KHR) g_swap.surfaceLost = true;
    if (g_swap.failures++ < 5 || g_swap.failures % 300 == 0) XLOG(0, "Vulkan: %s failed (%d)", what, static_cast<int>(r));
    return false;
}

// A new VkSurfaceKHR for the window's current native window (the old one is dead).
bool recreateSurface() {
    vkDeviceWaitIdle(g_ctx.device);
    if (g_swap.chain) {
        vkDestroySwapchainKHR(g_ctx.device, g_swap.chain, nullptr);
        g_swap.chain = VK_NULL_HANDLE;
        g_swap.images.clear();
    }
    if (g_surface) vkDestroySurfaceKHR(g_ctx.instance, g_surface, nullptr);
    g_surface = VK_NULL_HANDLE;
    g_swap.surfaceLost = false;
    if (!createSurface()) {
        g_swap.surfaceLost = true;
        return swapchainFailed("recreating the window surface", VK_ERROR_SURFACE_LOST_KHR);
    }
    VkBool32 present = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(g_ctx.phys, g_ctx.queueFamily, g_surface, &present);
    XLOG(1, "Vulkan: window surface recreated");
    return true;
}

bool createSwapchain() {
    if (g_swap.surfaceLost && !recreateSurface()) return false;
    VkSurfaceCapabilitiesKHR caps;
    if (VkResult r = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(g_ctx.phys, g_surface, &caps); r != VK_SUCCESS)
        return swapchainFailed("vkGetPhysicalDeviceSurfaceCapabilitiesKHR", r);
    uint32_t n = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(g_ctx.phys, g_surface, &n, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(n);
    vkGetPhysicalDeviceSurfaceFormatsKHR(g_ctx.phys, g_surface, &n, formats.data());
    g_swap.format = formats.empty() ? VK_FORMAT_B8G8R8A8_UNORM : formats[0].format;
    VkColorSpaceKHR space = formats.empty() ? VK_COLOR_SPACE_SRGB_NONLINEAR_KHR : formats[0].colorSpace;
    for (const auto& f : formats)
        if (f.format == VK_FORMAT_B8G8R8A8_UNORM || f.format == VK_FORMAT_R8G8B8A8_UNORM) {
            g_swap.format = f.format;
            space = f.colorSpace;
            break;
        }
    VkExtent2D ext = caps.currentExtent;
    if (ext.width == 0xFFFFFFFFu) {
        int w, h;
        SDL_GetWindowSize(g_window, &w, &h);
        ext = {static_cast<uint32_t>(w), static_cast<uint32_t>(h)};
    }
    // Android reports currentExtent in the window's current orientation (1920x1080 on a phone
    // held sideways, with currentTransform ROTATE_90): with an IDENTITY swapchain that is the
    // size to use, and the compositor rotates. (Its presents then report SUBOPTIMAL, see present().)
    const bool identity = caps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    ext.width = std::clamp(ext.width, caps.minImageExtent.width, std::max(caps.maxImageExtent.width, caps.minImageExtent.width));
    ext.height = std::clamp(ext.height, caps.minImageExtent.height, std::max(caps.maxImageExtent.height, caps.minImageExtent.height));
    if (!ext.width || !ext.height) return false;  // minimised
    uint32_t pm = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(g_ctx.phys, g_surface, &pm, nullptr);
    std::vector<VkPresentModeKHR> modes(pm);
    vkGetPhysicalDeviceSurfacePresentModesKHR(g_ctx.phys, g_surface, &pm, modes.data());
    VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;
    if (!g_set.vsync)
        for (auto m : modes)
            if (m == VK_PRESENT_MODE_MAILBOX_KHR || m == VK_PRESENT_MODE_IMMEDIATE_KHR) mode = m;
    VkSwapchainCreateInfoKHR ci{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    ci.surface = g_surface;
    ci.minImageCount = std::max(caps.minImageCount, 3u);
    if (caps.maxImageCount) ci.minImageCount = std::min(ci.minImageCount, caps.maxImageCount);
    ci.imageFormat = g_swap.format;
    ci.imageColorSpace = space;
    ci.imageExtent = ext;
    ci.imageArrayLayers = 1;
    ci.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | (caps.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    if (!(caps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) XLOG(0, "Vulkan: the swapchain does not allow transfers (blits)");
    ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ci.preTransform = identity ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : caps.currentTransform;
    ci.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    for (VkCompositeAlphaFlagBitsKHR a : {VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR, VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
                                          VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR})
        if (caps.supportedCompositeAlpha & a) {
            ci.compositeAlpha = a;
            break;
        }
    ci.presentMode = mode;
    ci.clipped = VK_TRUE;
    ci.oldSwapchain = g_swap.chain;
    VkSwapchainKHR chain;
    if (VkResult r = vkCreateSwapchainKHR(g_ctx.device, &ci, nullptr, &chain); r != VK_SUCCESS) return swapchainFailed("vkCreateSwapchainKHR", r);
    g_swap.failures = 0;
    if (g_swap.chain) {
        vkDeviceWaitIdle(g_ctx.device);
        vkDestroySwapchainKHR(g_ctx.device, g_swap.chain, nullptr);
    }
    g_swap.chain = chain;
    g_swap.extent = ext;
    uint32_t count = 0;
    vkGetSwapchainImagesKHR(g_ctx.device, chain, &count, nullptr);
    g_swap.images.resize(count);
    vkGetSwapchainImagesKHR(g_ctx.device, chain, &count, g_swap.images.data());
    g_swap.dirty = false;
    g_swap.rotated = identity && caps.currentTransform != VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    XLOG(1, "Vulkan: swapchain %ux%u, %u images, format %d, transform 0x%X, %s", ext.width, ext.height, count, static_cast<int>(g_swap.format),
         static_cast<unsigned>(caps.currentTransform), mode == VK_PRESENT_MODE_FIFO_KHR ? "vsync" : "no vsync");
    return true;
}
}  // namespace

Context& ctx() { return g_ctx; }

uint32_t memoryType(uint32_t bits, VkMemoryPropertyFlags want) {
    for (uint32_t i = 0; i < g_ctx.mem.memoryTypeCount; ++i)
        if ((bits & (1u << i)) && (g_ctx.mem.memoryTypes[i].propertyFlags & want) == want) return i;
    for (uint32_t i = 0; i < g_ctx.mem.memoryTypeCount; ++i)
        if (bits & (1u << i)) return i;
    return 0;
}

VkResult submit(const VkSubmitInfo& si, VkFence fence) {
    std::lock_guard<std::mutex> l(g_ctx.queueLock);
    return vkQueueSubmit(g_ctx.queue, 1, &si, fence);
}

bool init(SDL_Window* window, const Settings& s) {
    g_set = s;
    g_window = window;
    if (!loadLoader()) {
        XLOG(0, "Vulkan: no loader or driver could be opened");
        return false;
    }
    auto createInstance = reinterpret_cast<PFN_vkCreateInstance>(g_gipa(VK_NULL_HANDLE, "vkCreateInstance"));
    std::vector<const char*> exts = {VK_KHR_SURFACE_EXTENSION_NAME};
#if defined(__ANDROID__)
    exts.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#else
    SDL_SysWMinfo wm;
    SDL_VERSION(&wm.version);
    SDL_GetWindowWMInfo(window, &wm);
    exts.push_back(wm.subsystem == SDL_SYSWM_WAYLAND ? VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME : VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app.pApplicationName = "Fable Xbox";
    app.apiVersion = VK_API_VERSION_1_1;
    VkInstanceCreateInfo ici{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    ici.pApplicationInfo = &app;
    ici.enabledExtensionCount = static_cast<uint32_t>(exts.size());
    ici.ppEnabledExtensionNames = exts.data();
    const char* layer = "VK_LAYER_KHRONOS_validation";
    if (s.validation) {
        ici.enabledLayerCount = 1;
        ici.ppEnabledLayerNames = &layer;
    }
    if (!createInstance || createInstance(&ici, nullptr, &g_ctx.instance) != VK_SUCCESS) {
        XLOG(0, "Vulkan: vkCreateInstance failed");
        return false;
    }
#define XB_VK_LOADI(f) f = reinterpret_cast<PFN_##f>(g_gipa(g_ctx.instance, #f));
    XB_VK_INSTANCE_FNS(XB_VK_LOADI)
#undef XB_VK_LOADI
    if (!createSurface()) {
        XLOG(0, "Vulkan: cannot create the window surface");
        return false;
    }
    uint32_t n = 0;
    vkEnumeratePhysicalDevices(g_ctx.instance, &n, nullptr);
    std::vector<VkPhysicalDevice> devs(n);
    vkEnumeratePhysicalDevices(g_ctx.instance, &n, devs.data());
    // Prefer a discrete/integrated GPU over a CPU implementation.
    int best = -1, bestScore = -1;
    for (uint32_t i = 0; i < n; ++i) {
        VkPhysicalDeviceProperties p;
        vkGetPhysicalDeviceProperties(devs[i], &p);
        const int score = p.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 3 : p.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? 2 : 1;
        if (score > bestScore) { best = static_cast<int>(i); bestScore = score; }
    }
    if (best < 0) {
        XLOG(0, "Vulkan: no physical device");
        return false;
    }
    g_ctx.phys = devs[static_cast<size_t>(best)];
    vkGetPhysicalDeviceProperties(g_ctx.phys, &g_ctx.props);
    vkGetPhysicalDeviceMemoryProperties(g_ctx.phys, &g_ctx.mem);
    vkGetPhysicalDeviceFeatures(g_ctx.phys, &g_ctx.features);
    uint32_t qn = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(g_ctx.phys, &qn, nullptr);
    std::vector<VkQueueFamilyProperties> qf(qn);
    vkGetPhysicalDeviceQueueFamilyProperties(g_ctx.phys, &qn, qf.data());
    bool found = false;
    for (uint32_t i = 0; i < qn && !found; ++i) {
        VkBool32 present = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(g_ctx.phys, i, g_surface, &present);
        if ((qf[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && present) {
            g_ctx.queueFamily = i;
            found = true;
        }
    }
    if (!found) {
        XLOG(0, "Vulkan: no graphics queue that can present");
        return false;
    }
    const float prio = 1.0f;
    VkDeviceQueueCreateInfo qci{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    qci.queueFamilyIndex = g_ctx.queueFamily;
    qci.queueCount = 1;
    qci.pQueuePriorities = &prio;
    VkPhysicalDeviceFeatures want{};
    want.samplerAnisotropy = g_ctx.features.samplerAnisotropy;
    want.fillModeNonSolid = g_ctx.features.fillModeNonSolid;
    want.depthClamp = g_ctx.features.depthClamp;
    want.depthBiasClamp = g_ctx.features.depthBiasClamp;
    want.wideLines = g_ctx.features.wideLines;
    want.textureCompressionBC = g_ctx.features.textureCompressionBC;
    want.occlusionQueryPrecise = g_ctx.features.occlusionQueryPrecise;
    const char* dext[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkDeviceCreateInfo dci{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    dci.enabledExtensionCount = 1;
    dci.ppEnabledExtensionNames = dext;
    dci.pEnabledFeatures = &want;
    if (vkCreateDevice(g_ctx.phys, &dci, nullptr, &g_ctx.device) != VK_SUCCESS) {
        XLOG(0, "Vulkan: vkCreateDevice failed");
        return false;
    }
#define XB_VK_LOADD(f) f = reinterpret_cast<PFN_##f>(vkGetDeviceProcAddr(g_ctx.device, #f));
    XB_VK_DEVICE_FNS(XB_VK_LOADD)
#undef XB_VK_LOADD
    vkGetDeviceQueue(g_ctx.device, g_ctx.queueFamily, 0, &g_ctx.queue);
    XLOG(1, "Vulkan: %s (driver %u.%u.%u, API %u.%u)", g_ctx.props.deviceName, VK_VERSION_MAJOR(g_ctx.props.driverVersion),
         VK_VERSION_MINOR(g_ctx.props.driverVersion), VK_VERSION_PATCH(g_ctx.props.driverVersion), VK_VERSION_MAJOR(g_ctx.props.apiVersion),
         VK_VERSION_MINOR(g_ctx.props.apiVersion));

    VkCommandPoolCreateInfo pci{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pci.queueFamilyIndex = g_ctx.queueFamily;
    vkCreateCommandPool(g_ctx.device, &pci, nullptr, &g_swap.pool);
    VkCommandBufferAllocateInfo ai{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    ai.commandPool = g_swap.pool;
    ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = 3;
    vkAllocateCommandBuffers(g_ctx.device, &ai, g_swap.cmd);
    for (int i = 0; i < 3; ++i) {
        VkFenceCreateInfo fci{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        fci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(g_ctx.device, &fci, nullptr, &g_swap.fence[i]);
        VkSemaphoreCreateInfo sci{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        vkCreateSemaphore(g_ctx.device, &sci, nullptr, &g_swap.acquired[i]);
        vkCreateSemaphore(g_ctx.device, &sci, nullptr, &g_swap.done[i]);
    }
    return createSwapchain();
}

void resize() { g_swap.dirty = true; }
void surfaceChanged() {
    g_swap.dirty = true;
    g_swap.surfaceLost = true;
}

static void barrier(VkCommandBuffer cb, VkImage img, VkImageLayout from, VkImageLayout to) {
    VkImageMemoryBarrier b{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    b.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
    b.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
    b.oldLayout = from;
    b.newLayout = to;
    b.srcQueueFamilyIndex = b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.image = img;
    b.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &b);
}

void present(VkImage image, uint32_t w, uint32_t h, float aspect, VkSemaphore waitRendered) {
    if (g_swap.dirty || !g_swap.chain) {
        if (!createSwapchain()) return;
    }
    const uint32_t f = g_swap.frame++ % 3;
    vkWaitForFences(g_ctx.device, 1, &g_swap.fence[f], VK_TRUE, UINT64_MAX);
    uint32_t idx = 0;
    VkResult r = vkAcquireNextImageKHR(g_ctx.device, g_swap.chain, UINT64_MAX, g_swap.acquired[f], VK_NULL_HANDLE, &idx);
    if (r != VK_SUCCESS && r != VK_SUBOPTIMAL_KHR) {  // nothing acquired: the semaphore stays unsignalled
        g_swap.dirty = true;
        if (r != VK_ERROR_OUT_OF_DATE_KHR) swapchainFailed("vkAcquireNextImageKHR", r);
        return;
    }
    vkResetFences(g_ctx.device, 1, &g_swap.fence[f]);
    VkCommandBuffer cb = g_swap.cmd[f];
    vkResetCommandBuffer(cb, 0);
    VkCommandBufferBeginInfo bi{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cb, &bi);
    VkImage dst = g_swap.images[idx];
    barrier(cb, dst, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    VkClearColorValue black{};
    VkImageSubresourceRange all{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vkCmdClearColorImage(cb, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &black, 1, &all);
    if (image) {
        // Letterbox: fit `aspect` inside the window.
        const float winAspect = static_cast<float>(g_swap.extent.width) / static_cast<float>(g_swap.extent.height);
        int32_t ow = static_cast<int32_t>(g_swap.extent.width), oh = static_cast<int32_t>(g_swap.extent.height);
        if (winAspect > aspect) ow = static_cast<int32_t>(oh * aspect);
        else oh = static_cast<int32_t>(ow / aspect);
        const int32_t ox = (static_cast<int32_t>(g_swap.extent.width) - ow) / 2, oy = (static_cast<int32_t>(g_swap.extent.height) - oh) / 2;
        VkImageBlit blit{};
        blit.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
        blit.srcOffsets[1] = {static_cast<int32_t>(w), static_cast<int32_t>(h), 1};
        blit.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
        blit.dstOffsets[0] = {ox, oy, 0};
        blit.dstOffsets[1] = {ox + ow, oy + oh, 1};
        vkCmdBlitImage(cb, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);
    }
    barrier(cb, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    vkEndCommandBuffer(cb);
    VkSemaphore waits[2] = {g_swap.acquired[f], waitRendered};
    VkPipelineStageFlags stages[2] = {VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT};
    VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    si.waitSemaphoreCount = waitRendered ? 2 : 1;
    si.pWaitSemaphores = waits;
    si.pWaitDstStageMask = stages;
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cb;
    si.signalSemaphoreCount = 1;
    si.pSignalSemaphores = &g_swap.done[f];
    submit(si, g_swap.fence[f]);
    VkPresentInfoKHR pi{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    pi.waitSemaphoreCount = 1;
    pi.pWaitSemaphores = &g_swap.done[f];
    pi.swapchainCount = 1;
    pi.pSwapchains = &g_swap.chain;
    pi.pImageIndices = &idx;
    {
        std::lock_guard<std::mutex> l(g_ctx.queueLock);
        r = vkQueuePresentKHR(g_ctx.queue, &pi);
    }
    if (r == VK_ERROR_OUT_OF_DATE_KHR || (r == VK_SUBOPTIMAL_KHR && !g_swap.rotated)) g_swap.dirty = true;
    else if (r == VK_SUBOPTIMAL_KHR) {}  // Android's hint to pre-rotate; recreating would not change it
    else if (r != VK_SUCCESS) {
        g_swap.dirty = true;
        swapchainFailed("vkQueuePresentKHR", r);
    }
}

} // namespace xb::vk
