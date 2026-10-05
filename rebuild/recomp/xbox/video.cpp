// Main-thread video loop: the SDL window, Vulkan start-up, presentation at vblank.
//
// Until the renderer owns the scanout surface, the frame shown is the guest framebuffer at
// PCRTC_START, uploaded from guest memory. Without a display (FABLE_HEADLESS=1, or no video
// driver) the software renderer runs and frames can be dumped to PNG.
#include "gpu.hpp"
#include "input.hpp"
#include "settings.hpp"
#include "vk.hpp"
#include "vk_renderer.hpp"
#include "xhost.hpp"

#include <SDL.h>

#include "../posix/third_party/stb_image_write.h"

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

namespace xb {
#if defined(__ANDROID__)
void* androidVulkanDriver();
#endif
extern uint32_t g_avFramebuffer, g_avPitch, g_avFormat;

namespace gpu {
Renderer& softRenderer();
Renderer* g_renderer;
Renderer& renderer() { return g_renderer ? *g_renderer : softRenderer(); }
std::atomic<uint32_t> g_vblanks{0};
}

namespace {
SDL_Window* g_window;

// Scanout upload (CPU copy of the guest framebuffer).
struct Scanout {
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory stagingMem = VK_NULL_HANDLE;
    void* mapped = nullptr;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory imageMem = VK_NULL_HANDLE;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer cmd = VK_NULL_HANDLE;
    VkFence fence = VK_NULL_HANDLE;
    uint32_t w = 0, h = 0;
} g_scan;

bool createScanout(uint32_t w, uint32_t h) {
    using namespace vk;
    VkDevice dev = ctx().device;
    if (g_scan.image) {
        vkDeviceWaitIdle(dev);
        vkDestroyImage(dev, g_scan.image, nullptr);
        vkFreeMemory(dev, g_scan.imageMem, nullptr);
        vkDestroyBuffer(dev, g_scan.staging, nullptr);
        vkFreeMemory(dev, g_scan.stagingMem, nullptr);
    }
    g_scan.w = w;
    g_scan.h = h;
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.size = static_cast<VkDeviceSize>(w) * h * 4;
    bci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    vkCreateBuffer(dev, &bci, nullptr, &g_scan.staging);
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(dev, g_scan.staging, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkAllocateMemory(dev, &mai, nullptr, &g_scan.stagingMem);
    vkBindBufferMemory(dev, g_scan.staging, g_scan.stagingMem, 0);
    vkMapMemory(dev, g_scan.stagingMem, 0, VK_WHOLE_SIZE, 0, &g_scan.mapped);
    VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    ici.imageType = VK_IMAGE_TYPE_2D;
    ici.format = VK_FORMAT_B8G8R8A8_UNORM;
    ici.extent = {w, h, 1};
    ici.mipLevels = 1;
    ici.arrayLayers = 1;
    ici.samples = VK_SAMPLE_COUNT_1_BIT;
    ici.tiling = VK_IMAGE_TILING_OPTIMAL;
    ici.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    vkCreateImage(dev, &ici, nullptr, &g_scan.image);
    vkGetImageMemoryRequirements(dev, g_scan.image, &req);
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkAllocateMemory(dev, &mai, nullptr, &g_scan.imageMem);
    vkBindImageMemory(dev, g_scan.image, g_scan.imageMem, 0);
    if (!g_scan.pool) {
        VkCommandPoolCreateInfo pci{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        pci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pci.queueFamilyIndex = ctx().queueFamily;
        vkCreateCommandPool(dev, &pci, nullptr, &g_scan.pool);
        VkCommandBufferAllocateInfo ai{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        ai.commandPool = g_scan.pool;
        ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        ai.commandBufferCount = 1;
        vkAllocateCommandBuffers(dev, &ai, &g_scan.cmd);
        VkFenceCreateInfo fci{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        fci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(dev, &fci, nullptr, &g_scan.fence);
    }
    return true;
}

// Copies the guest framebuffer into the scanout image (TRANSFER_SRC layout afterwards).
void uploadScanout() {
    using namespace vk;
    const uint32_t pitch = g_avPitch ? g_avPitch : 640 * 4;
    const uint32_t w = pitch / 4, h = (g_avFormat & 0xFFFF0000u) ? 480 : 480;
    const uint32_t fb = kContigBase + (gpu::state().scanout ? gpu::state().scanout : physOf(g_avFramebuffer));
    if (w != g_scan.w || h != g_scan.h) createScanout(w, h);
    VkDevice dev = ctx().device;
    vkWaitForFences(dev, 1, &g_scan.fence, VK_TRUE, UINT64_MAX);
    vkResetFences(dev, 1, &g_scan.fence);
    if (fb > kContigBase) std::memcpy(g_scan.mapped, gp(fb), static_cast<size_t>(w) * h * 4);
    VkCommandBuffer cb = g_scan.cmd;
    vkResetCommandBuffer(cb, 0);
    VkCommandBufferBeginInfo bi{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cb, &bi);
    VkImageMemoryBarrier b{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    b.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    b.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    b.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    b.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    b.srcQueueFamilyIndex = b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.image = g_scan.image;
    b.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &b);
    VkBufferImageCopy copy{};
    copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    copy.imageExtent = {w, h, 1};
    vkCmdCopyBufferToImage(cb, g_scan.staging, g_scan.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
    b.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    b.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    b.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    b.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &b);
    vkEndCommandBuffer(cb);
    VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cb;
    submit(si, g_scan.fence);
}

// FABLE_DUMP_FRAMES=N: every Nth presented frame is read back and written to vkframe_*.png.
void dumpImage(VkImage src, uint32_t w, uint32_t h, uint64_t n) {
    using namespace vk;
    VkDevice dev = ctx().device;
    static VkImage img = VK_NULL_HANDLE;
    static VkDeviceMemory imgMem, bufMem;
    static VkBuffer buf;
    static void* mapped;
    static uint32_t cw, ch;
    static VkCommandPool pool;
    static VkCommandBuffer cb;
    static VkFence fence;
    if (!pool) {
        VkCommandPoolCreateInfo pci{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        pci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pci.queueFamilyIndex = ctx().queueFamily;
        vkCreateCommandPool(dev, &pci, nullptr, &pool);
        VkCommandBufferAllocateInfo ai{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        ai.commandPool = pool;
        ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        ai.commandBufferCount = 1;
        vkAllocateCommandBuffers(dev, &ai, &cb);
        VkFenceCreateInfo fci{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        vkCreateFence(dev, &fci, nullptr, &fence);
    }
    if (w != cw || h != ch) {
        vkDeviceWaitIdle(dev);
        if (img) {
            vkDestroyImage(dev, img, nullptr);
            vkFreeMemory(dev, imgMem, nullptr);
            vkDestroyBuffer(dev, buf, nullptr);
            vkFreeMemory(dev, bufMem, nullptr);
        }
        cw = w;
        ch = h;
        VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        ici.imageType = VK_IMAGE_TYPE_2D;
        ici.format = VK_FORMAT_B8G8R8A8_UNORM;
        ici.extent = {w, h, 1};
        ici.mipLevels = ici.arrayLayers = 1;
        ici.samples = VK_SAMPLE_COUNT_1_BIT;
        ici.tiling = VK_IMAGE_TILING_OPTIMAL;
        ici.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        vkCreateImage(dev, &ici, nullptr, &img);
        VkMemoryRequirements req;
        vkGetImageMemoryRequirements(dev, img, &req);
        VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        mai.allocationSize = req.size;
        mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkAllocateMemory(dev, &mai, nullptr, &imgMem);
        vkBindImageMemory(dev, img, imgMem, 0);
        VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bci.size = static_cast<VkDeviceSize>(w) * h * 4;
        bci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        vkCreateBuffer(dev, &bci, nullptr, &buf);
        vkGetBufferMemoryRequirements(dev, buf, &req);
        mai.allocationSize = req.size;
        mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vkAllocateMemory(dev, &mai, nullptr, &bufMem);
        vkBindBufferMemory(dev, buf, bufMem, 0);
        vkMapMemory(dev, bufMem, 0, VK_WHOLE_SIZE, 0, &mapped);
    }
    vkResetCommandBuffer(cb, 0);
    VkCommandBufferBeginInfo bi{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cb, &bi);
    VkImageMemoryBarrier b{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    b.srcAccessMask = 0;
    b.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    b.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    b.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    b.srcQueueFamilyIndex = b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.image = img;
    b.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &b);
    VkImageBlit blit{};
    blit.srcSubresource = blit.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    blit.srcOffsets[1] = blit.dstOffsets[1] = {static_cast<int32_t>(w), static_cast<int32_t>(h), 1};
    vkCmdBlitImage(cb, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, img, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_NEAREST);
    b.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    b.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    b.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    b.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &b);
    VkBufferImageCopy copy{};
    copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    copy.imageExtent = {w, h, 1};
    vkCmdCopyImageToBuffer(cb, img, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buf, 1, &copy);
    vkEndCommandBuffer(cb);
    VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cb;
    submit(si, fence);
    vkWaitForFences(dev, 1, &fence, VK_TRUE, UINT64_MAX);
    vkResetFences(dev, 1, &fence);
    std::vector<uint8_t> rgb(static_cast<size_t>(w) * h * 3);
    const auto* p = static_cast<const uint8_t*>(mapped);
    for (size_t i = 0; i < static_cast<size_t>(w) * h; ++i) {
        rgb[3 * i] = p[4 * i + 2];
        rgb[3 * i + 1] = p[4 * i + 1];
        rgb[3 * i + 2] = p[4 * i];
    }
    char name[64];
    snprintf(name, sizeof name, "vkframe_%05llu.png", static_cast<unsigned long long>(n));
    stbi_write_png(name, static_cast<int>(w), static_cast<int>(h), 3, rgb.data(), static_cast<int>(w * 3));
}

void headlessLoop() {
    input::init();
    for (;;) {
        input::update();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
}  // namespace

void videoMain() {
    const Settings& s = settings();
    if (getenv("FABLE_HEADLESS") || SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        XLOG(1, "video: headless (%s)", getenv("FABLE_HEADLESS") ? "FABLE_HEADLESS" : SDL_GetError());
        headlessLoop();
        return;
    }
    const int ww = s.widescreen ? 1280 : 960, wh = s.widescreen ? 720 : 720;
    Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | (s.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#if defined(__ANDROID__)
    // Without a graphics flag SDL gives an Android window an EGL surface, which takes the
    // native window: vkCreateAndroidSurfaceKHR then fails (NATIVE_WINDOW_IN_USE) and nothing shows.
    flags |= SDL_WINDOW_VULKAN;
#endif
    g_window = SDL_CreateWindow("Fable: The Lost Chapters (Xbox)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, flags);
    vk::Settings vs;
    vs.driverPath = s.vulkanDriver;
#if defined(__ANDROID__)
    vs.driverHandle = androidVulkanDriver();  // the launcher's custom driver, via adrenotools
#endif
    vs.vsync = s.vsync;
    if (!g_window || !vk::init(g_window, vs)) {
        XLOG(0, "video: Vulkan unavailable, running headless (%s)", g_window ? "see the Vulkan messages above" : SDL_GetError());
        headlessLoop();
        return;
    }
    if (!getenv("FABLE_SOFT_RENDER")) {
        static gpu::VkRenderer* vr = new gpu::VkRenderer();
        gpu::g_renderer = vr;
    }
    input::init();
    uint32_t seen = 0;
    for (;;) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            input::handleEvent(e);
            if (e.type == SDL_QUIT) {
                XLOG(0, "video: window closed");
                fflush(stderr);
                _exit(0);
            }
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) vk::resize();
#if defined(__ANDROID__)
            if (e.type == SDL_APP_DIDENTERFOREGROUND) vk::surfaceChanged();  // SDL made a new native window
#endif
        }
        input::update();
        const uint32_t v = gpu::g_vblanks.load();
        if (v == seen) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        seen = v;
        const float aspect = s.widescreen ? 16.0f / 9.0f : 4.0f / 3.0f;
        VkImage img;
        uint32_t iw, ih;
        auto* vr = dynamic_cast<gpu::VkRenderer*>(gpu::g_renderer);
        const uint32_t scan = gpu::state().scanout ? gpu::state().scanout : physOf(g_avFramebuffer);
        if (vr && vr->scanoutImage(scan, &img, &iw, &ih)) {
            static const int every = getenv("FABLE_DUMP_FRAMES") ? atoi(getenv("FABLE_DUMP_FRAMES")) : 0;
            static uint64_t presented = 0;
            if (every > 0 && ++presented % static_cast<uint64_t>(every) == 0) dumpImage(img, iw, ih, presented);
            vk::present(img, iw, ih, aspect, VK_NULL_HANDLE);
        } else {
            uploadScanout();
            vk::present(g_scan.image, g_scan.w, g_scan.h, aspect, VK_NULL_HANDLE);
        }
    }
}

} // namespace xb
