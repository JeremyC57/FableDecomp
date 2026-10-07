// Vulkan renderer for the NV2A Kelvin state: render targets as host images (at the chosen
// resolution scale), clears, draws, and the scanout image for presentation.
//
// Threading: the device model calls in on the pusher thread (recording); the video thread
// asks for the scanout image at vblank. Both share the queue through vk::submit().
#include "vk_renderer.hpp"

#include "gpu.hpp"
#include "nv2a_methods.h"
#include "settings.hpp"
#include "vk.hpp"
#include "xhost.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <mutex>
#include <vector>

namespace xb::gpu {
extern uint64_t g_frameCount;

using namespace vk;

namespace {

constexpr int kFrames = 2;

VkFormat colorFormatOf(uint32_t surfaceFormat) {
    switch (surfaceFormat & 0xF) {
    // 16-bit surfaces (Fable's back buffer is X1R5G5B5) are stored with 8 bits per channel: the
    // console dithers its 5-bit output, plain 5-bit storage bands visibly. The game never blends
    // with destination alpha on them.
    case 0x1: case 0x2: case 0x3: return VK_FORMAT_B8G8R8A8_UNORM;
    case 0x9: return VK_FORMAT_R8_UNORM;
    case 0xA: return VK_FORMAT_R8G8_UNORM;
    default: return VK_FORMAT_B8G8R8A8_UNORM;
    }
}

}  // namespace

// ============================================================================================
// construction
// ============================================================================================
VkRenderer::VkRenderer() {
    VkDevice dev = ctx().device;
    wide_ = settings().widescreen;
    outH_ = static_cast<uint32_t>(settings().resolution ? settings().resolution : 480 * std::clamp(settings().resolutionScale, 1, 4));
    outW_ = wide_ ? (((outH_ * 16 + 8) / 9) + 1) & ~1u : outH_ * 4 / 3;  // 854x480, 1280x720, 1920x1080 / 960x720, 1440x1080
    scaleY_ = outH_ / 480.0;
    // Depth format: D24S8 where supported (most desktop), else D32S8 (some mobile).
    depthFormat_ = VK_FORMAT_D24_UNORM_S8_UINT;
    VkFormatProperties fp;
    vkGetPhysicalDeviceFormatProperties(ctx().phys, depthFormat_, &fp);
    if (!(fp.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)) depthFormat_ = VK_FORMAT_D32_SFLOAT_S8_UINT;

    VkCommandPoolCreateInfo pci{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pci.queueFamilyIndex = ctx().queueFamily;
    vkCreateCommandPool(dev, &pci, nullptr, &pool_);
    for (Frame& f : frames_) {
        VkCommandBufferAllocateInfo ai{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        ai.commandPool = pool_;
        ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        ai.commandBufferCount = 1;
        vkAllocateCommandBuffers(dev, &ai, &f.cmd);
        VkFenceCreateInfo fci{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        fci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(dev, &fci, nullptr, &f.fence);
        f.upload = createBuffer(kUploadSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                                                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vkMapMemory(dev, f.upload.mem, 0, VK_WHOLE_SIZE, 0, &f.mapped);
        VkDescriptorPoolSize sizes[] = {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 4096}, {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 16384}};
        VkDescriptorPoolCreateInfo dpi{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        dpi.maxSets = 4096;
        dpi.poolSizeCount = 2;
        dpi.pPoolSizes = sizes;
        vkCreateDescriptorPool(dev, &dpi, nullptr, &f.descPool);
    }
    initPipelineObjects();
    beginFrame();
    XLOG(1, "Vulkan renderer: %ux%u, depth %s", outW_, outH_, depthFormat_ == VK_FORMAT_D24_UNORM_S8_UINT ? "D24S8" : "D32S8");
}

VkRenderer::Buffer VkRenderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props) {
    VkDevice dev = ctx().device;
    Buffer b;
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.size = size;
    bci.usage = usage;
    vkCreateBuffer(dev, &bci, nullptr, &b.buf);
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(dev, b.buf, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = memoryType(req.memoryTypeBits, props);
    vkAllocateMemory(dev, &mai, nullptr, &b.mem);
    vkBindBufferMemory(dev, b.buf, b.mem, 0);
    b.size = size;
    return b;
}

// ============================================================================================
// frames and submission
// ============================================================================================
void VkRenderer::beginFrame() {
    Frame& f = frames_[frame_];
    vkWaitForFences(ctx().device, 1, &f.fence, VK_TRUE, UINT64_MAX);
    vkResetFences(ctx().device, 1, &f.fence);
    vkResetDescriptorPool(ctx().device, f.descPool, 0);
    for (auto& t : f.garbage) t();
    f.garbage.clear();
    f.uploadPos = 0;
    vkResetCommandBuffer(f.cmd, 0);
    VkCommandBufferBeginInfo bi{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(f.cmd, &bi);
    recording_ = true;
}

void VkRenderer::submitFrame(bool wait) {
    endPass();
    Frame& f = frames_[frame_];
    vkEndCommandBuffer(f.cmd);
    VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    si.commandBufferCount = 1;
    si.pCommandBuffers = &f.cmd;
    submit(si, f.fence);
    if (wait) vkWaitForFences(ctx().device, 1, &f.fence, VK_TRUE, UINT64_MAX);
    recording_ = false;
    frame_ = (frame_ + 1) % kFrames;
    beginFrame();
}

void VkRenderer::flush() {
    // CPU-visible results (semaphores, reports) are written by the device model directly;
    // GPU work only needs to be in order on the queue.
}

void logDrawStats(uint64_t frame);

void VkRenderer::endFrame() {
    std::lock_guard<std::mutex> l(surfLock_);
    submitFrame(false);
    lastFrameDraws_ = drawsThisFrame_;
    drawsThisFrame_ = 0;
    // Test harness: FABLE_HEAVY_DRAWS=N logs frames with more draws than N (the targeting void
    // drew ~4200); FABLE_EXIT_FLIP=F ends the run at flip F.
    static const uint64_t heavy = getenv("FABLE_HEAVY_DRAWS") ? strtoull(getenv("FABLE_HEAVY_DRAWS"), nullptr, 0) : 0;
    static const uint64_t exitFlip = getenv("FABLE_EXIT_FLIP") ? strtoull(getenv("FABLE_EXIT_FLIP"), nullptr, 0) : 0;
    static uint32_t heavyCount = 0;
    if (heavy && lastFrameDraws_ > heavy && heavyCount++ < 20)
        XLOG(0, "heavy frame: %llu draws at flip %llu", static_cast<unsigned long long>(lastFrameDraws_), static_cast<unsigned long long>(g_frameCount));
    if (exitFlip && g_frameCount >= exitFlip) {
        XLOG(0, "exit at flip %llu (%u heavy frames)", static_cast<unsigned long long>(g_frameCount), heavyCount);
        fflush(nullptr);
        _exit(0);
    }
    if (++frames_done_ % 60 == 0) {
        logDrawStats(frames_done_);
        if (frames_done_ % 1800 == 0) savePipelineCache();  // each minute, when it grew
        // Textures unused for 600 frames (~20 s) are freed: streaming reuses addresses with other
        // formats, which would otherwise pile up (FABLE_DISABLE=evict keeps them).
        static const bool keep = featureOff("evict");
        VkDevice dev = ctx().device;
        for (auto it = textures_.begin(); !keep && it != textures_.end();) {
            if (it->second.image && frames_done_ - it->second.lastUse > 600) {
                Texture old = it->second;
                frames_[frame_].garbage.push_back([dev, old] {
                    vkDestroyImageView(dev, old.view, nullptr);
                    vkDestroyImage(dev, old.image, nullptr);
                    vkFreeMemory(dev, old.mem, nullptr);
                });
                it = textures_.erase(it);
            } else {
                ++it;
            }
        }
    }
}


// ============================================================================================
// surfaces
// ============================================================================================
// Several surfaces can share an address (the game renders a 320x240 pass into its back
// buffer's memory, then the 640x480 scene again): lookups by address get the one bound as a
// render target most recently.
VkRenderer::Surface* VkRenderer::findSurface(uint32_t addr, bool depth) {
    Surface* best = nullptr;
    for (auto& s : surfaces_)
        if (s.image && s.addr == addr && s.depth == depth && (!best || s.written > best->written)) best = &s;
    return best;
}

VkRenderer::Surface* VkRenderer::getSurface(uint32_t addr, uint32_t w, uint32_t h, uint32_t pitch, VkFormat fmt, bool depth) {
    Surface* s = nullptr;
    for (auto& x : surfaces_)
        if (x.image && x.addr == addr && x.depth == depth && x.w == w && x.h == h && x.format == fmt) {
            x.written = ++surfaceStamp_;
            return &x;
        }
    for (auto& x : surfaces_)  // reuse a destroyed slot
        if (!x.image) { s = &x; break; }
    if (!s) {
        surfaces_.push_back({});
        s = &surfaces_.back();
    }
    s->written = ++surfaceStamp_;
    VkDevice dev = ctx().device;
    s->addr = addr;
    s->w = w;
    s->h = h;
    s->pitch = pitch;
    s->format = fmt;
    s->depth = depth;
    VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    ici.imageType = VK_IMAGE_TYPE_2D;
    ici.format = fmt;
    ici.extent = {hostW(w, h), hostH(h), 1};
    ici.mipLevels = 1;
    ici.arrayLayers = 1;
    ici.samples = VK_SAMPLE_COUNT_1_BIT;
    ici.tiling = VK_IMAGE_TILING_OPTIMAL;
    ici.usage = (depth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) | VK_IMAGE_USAGE_SAMPLED_BIT |
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vkCreateImage(dev, &ici, nullptr, &s->image);
    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(dev, s->image, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkAllocateMemory(dev, &mai, nullptr, &s->mem);
    vkBindImageMemory(dev, s->image, s->mem, 0);
    VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    vci.image = s->image;
    vci.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vci.format = fmt;
    vci.subresourceRange = {static_cast<VkImageAspectFlags>(depth ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_COLOR_BIT), 0, 1, 0, 1};
    vkCreateImageView(dev, &vci, nullptr, &s->view);
    if (depth) {  // sampling a depth surface reads depth only
        vci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        vkCreateImageView(dev, &vci, nullptr, &s->sampleView);
    } else {
        s->sampleView = s->view;
    }
    s->layout = VK_IMAGE_LAYOUT_UNDEFINED;
    // Start from the guest memory contents (cleared images look wrong otherwise for the first frame).
    transition(*s, depth ? VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    if (depth) {
        VkClearDepthStencilValue v{1.0f, 0};
        VkImageSubresourceRange r{VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1};
        vkCmdClearDepthStencilImage(cmd(), s->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &v, 1, &r);
    } else {
        VkClearColorValue v{};
        VkImageSubresourceRange r{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        vkCmdClearColorImage(cmd(), s->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &v, 1, &r);
    }
    XLOG(2, "Vulkan: %s surface 0x%08X %ux%u", depth ? "depth" : "color", addr, w, h);
    return s;
}

void VkRenderer::destroySurface(Surface& s) {
    VkDevice dev = ctx().device;
    Surface copy = s;
    frames_[frame_].garbage.push_back([dev, copy] {
        if (copy.sampleView && copy.sampleView != copy.view) vkDestroyImageView(dev, copy.sampleView, nullptr);
        vkDestroyImageView(dev, copy.view, nullptr);
        vkDestroyImage(dev, copy.image, nullptr);
        vkFreeMemory(dev, copy.mem, nullptr);
    });
    for (auto it = framebuffers_.begin(); it != framebuffers_.end();)
        if (it->first.color == s.image || it->first.depth == s.image) {
            VkFramebuffer fb = it->second;
            frames_[frame_].garbage.push_back([dev, fb] { vkDestroyFramebuffer(dev, fb, nullptr); });
            it = framebuffers_.erase(it);
        } else {
            ++it;
        }
    s = Surface{};
}

void VkRenderer::transition(Surface& s, VkImageLayout to) {
    if (s.layout == to) return;
    endPass();
    VkImageMemoryBarrier b{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    b.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT;
    b.dstAccessMask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT;
    b.oldLayout = s.layout;
    b.newLayout = to;
    b.srcQueueFamilyIndex = b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.image = s.image;
    b.subresourceRange = {static_cast<VkImageAspectFlags>(s.depth ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_COLOR_BIT), 0, 1, 0, 1};
    vkCmdPipelineBarrier(cmd(), VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &b);
    s.layout = to;
}

// The current Kelvin render targets.
void VkRenderer::bindTargets() {
    const State& st = state();
    const uint32_t* R = st.regs;
    const uint32_t fmt = R[NV097_SET_SURFACE_FORMAT / 4];
    const uint32_t clipH = R[NV097_SET_SURFACE_CLIP_HORIZONTAL / 4], clipV = R[NV097_SET_SURFACE_CLIP_VERTICAL / 4];
    uint32_t w = (clipH >> 16) + (clipH & 0xFFFF), h = (clipV >> 16) + (clipV & 0xFFFF);
    if (((fmt >> 8) & 0xF) == 2) {  // swizzled surface: dimensions from the format
        w = 1u << ((fmt >> 16) & 0xFF);
        h = 1u << ((fmt >> 24) & 0xFF);
    }
    if (!w || !h) { w = 640; h = 480; }
    const uint32_t pitch = R[NV097_SET_SURFACE_PITCH / 4];
    const uint32_t colorDma = R[NV097_SET_CONTEXT_DMA_COLOR / 4], zetaDma = R[NV097_SET_CONTEXT_DMA_ZETA / 4];
    const uint32_t colorAddr = colorDma ? dmaAddress(colorDma, nullptr) + R[NV097_SET_SURFACE_COLOR_OFFSET / 4] : 0;
    const uint32_t zetaAddr = zetaDma ? dmaAddress(zetaDma, nullptr) + R[NV097_SET_SURFACE_ZETA_OFFSET / 4] : 0;
    const bool zeta = ((fmt >> 4) & 0xF) != 0 && zetaDma;
    Surface* c = colorAddr ? getSurface(colorAddr, w, h, pitch & 0xFFFF, colorFormatOf(fmt), false) : nullptr;
    Surface* z = zeta ? getSurface(zetaAddr, w, h, pitch >> 16, depthFormat_, true) : nullptr;
    target_.color = c ? c->addr : 0;
    target_.depth = z ? z->addr : 0;
    target_.w = w;
    target_.h = h;
}

// ============================================================================================
// clears
// ============================================================================================
void VkRenderer::clear(uint32_t flags) {
    std::lock_guard<std::mutex> l(surfLock_);
    bindTargets();
    const uint32_t* R = state().regs;
    const uint32_t hr = R[NV097_SET_CLEAR_RECT_HORIZONTAL / 4], vr = R[NV097_SET_CLEAR_RECT_VERTICAL / 4];
    const int32_t x0 = static_cast<int32_t>(hr & 0xFFFF), x1 = static_cast<int32_t>(hr >> 16);
    const int32_t y0 = static_cast<int32_t>(vr & 0xFFFF), y1 = static_cast<int32_t>(vr >> 16);
    if (x1 < x0 || y1 < y0) return;
    Surface* c = target_.color ? findSurface(target_.color, false) : nullptr;
    Surface* z = target_.depth ? findSurface(target_.depth, true) : nullptr;
    beginPass();
    if (!pass_) return;
    VkClearAttachment att[2];
    uint32_t n = 0;
    if ((flags & 0xF0) && c) {
        const uint32_t v = R[NV097_SET_COLOR_CLEAR_VALUE / 4];
        att[n] = {};
        att[n].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        att[n].colorAttachment = 0;
        att[n].clearValue.color.float32[0] = ((v >> 16) & 0xFF) / 255.0f;
        att[n].clearValue.color.float32[1] = ((v >> 8) & 0xFF) / 255.0f;
        att[n].clearValue.color.float32[2] = (v & 0xFF) / 255.0f;
        att[n].clearValue.color.float32[3] = (v >> 24) / 255.0f;
        ++n;
    }
    if ((flags & 0x3) && z) {
        const uint32_t v = R[NV097_SET_ZSTENCIL_CLEAR_VALUE / 4];
        att[n] = {};
        att[n].aspectMask = ((flags & 1) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) | ((flags & 2) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
        att[n].clearValue.depthStencil.depth = static_cast<float>(v >> 8) / 16777215.0f;
        att[n].clearValue.depthStencil.stencil = v & 0xFF;
        ++n;
    }
    if (getenv("FABLE_CAPTURE_IMAGES") || getenv("FABLE_CAPTURE_FLIP")) {
        static FILE* f = fopen("clears.txt", "w");
        if (f) fprintf(f, "frame %llu clear %X color %08X z %08X tgt %08X zt %08X %ux%u rect %d,%d-%d,%d sfmt %08X\n",
                       static_cast<unsigned long long>(frames_done_), flags, R[NV097_SET_COLOR_CLEAR_VALUE / 4], R[NV097_SET_ZSTENCIL_CLEAR_VALUE / 4],
                       target_.color, target_.depth, target_.w, target_.h, x0, y0, x1, y1, R[NV097_SET_SURFACE_FORMAT / 4]), fflush(f);
    }
    if (!n) return;
    VkClearRect rect{};
    const uint32_t fw = hostW(target_.w, target_.h), fh = hostH(target_.h);
    const double sx = target_.w ? static_cast<double>(fw) / target_.w : 1.0;
    const int hx0 = static_cast<int>(x0 * sx), hx1 = static_cast<int>((x1 + 1) * sx);
    const double sy = target_.h ? static_cast<double>(fh) / target_.h : 1.0;
    const int hy0 = static_cast<int>(y0 * sy), hy1 = static_cast<int>((y1 + 1) * sy);
    rect.rect.offset = {hx0, hy0};
    rect.rect.extent = {static_cast<uint32_t>(std::max(0, hx1 - hx0)), static_cast<uint32_t>(std::max(0, hy1 - hy0))};
    rect.layerCount = 1;
    if (rect.rect.offset.x + rect.rect.extent.width > fw) rect.rect.extent.width = fw - std::min<uint32_t>(fw, rect.rect.offset.x);
    if (rect.rect.offset.y + rect.rect.extent.height > fh) rect.rect.extent.height = fh - std::min<uint32_t>(fh, rect.rect.offset.y);
    if (!rect.rect.extent.width || !rect.rect.extent.height) return;
    vkCmdClearAttachments(cmd(), n, att, 1, &rect);
}

// ============================================================================================
// presentation
// ============================================================================================
bool VkRenderer::scanoutImage(uint32_t addr, VkImage* image, uint32_t* w, uint32_t* h) {
    std::lock_guard<std::mutex> l(surfLock_);
    Surface* s = findSurface(addr, false);
    if (!s) return false;
    // Hand the image over in TRANSFER_SRC layout; the next render pass transitions it back.
    transition(*s, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    submitFrame(false);
    *image = s->image;
    *w = hostW(s->w, s->h);
    *h = hostH(s->h);
    return true;
}

} // namespace xb::gpu
