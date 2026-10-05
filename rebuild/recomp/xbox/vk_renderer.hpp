// Vulkan renderer for the NV2A (see vk_renderer.cpp).
#pragma once

#include "gpu.hpp"
#include "vk.hpp"

#include <array>
#include <deque>
#include <functional>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace xb::gpu {

class VkRenderer final : public Renderer {
public:
    VkRenderer();

    void flush() override;
    void endFrame() override;
    void vblank() override;
    void clear(uint32_t flags) override;
    void begin(uint32_t primitive) override;
    void end() override;
    void drawArrays(uint32_t start, uint32_t count) override;
    void inlineArray(const uint32_t* words, uint32_t n) override;
    void arrayElements(const uint32_t* words, uint32_t n, bool sixteenBit) override;
    void vertexAttribute(uint32_t method, uint32_t value) override;

    // Video thread: the host image rendered at guest address `addr`, if any (left in
    // TRANSFER_SRC layout, its rendering submitted).
    bool scanoutImage(uint32_t addr, VkImage* image, uint32_t* w, uint32_t* h);

private:
    static constexpr VkDeviceSize kUploadSize = 64ull << 20;

    struct Buffer {
        VkBuffer buf = VK_NULL_HANDLE;
        VkDeviceMemory mem = VK_NULL_HANDLE;
        VkDeviceSize size = 0;
    };
    struct Frame {
        VkCommandBuffer cmd = VK_NULL_HANDLE;
        VkFence fence = VK_NULL_HANDLE;
        Buffer upload;
        void* mapped = nullptr;
        VkDeviceSize uploadPos = 0;
        VkDescriptorPool descPool = VK_NULL_HANDLE;
        std::vector<std::function<void()>> garbage;  // destroyed when the frame's fence signals
    };
    struct Surface {
        uint32_t addr = 0, w = 0, h = 0, pitch = 0;
        VkFormat format = VK_FORMAT_UNDEFINED;
        bool depth = false;
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory mem = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE, sampleView = VK_NULL_HANDLE;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
        uint64_t written = 0;  // surfaceStamp_ when last bound as a render target
    };
    struct FbKey {
        VkImage color, depth;
        bool operator<(const FbKey& o) const { return color != o.color ? color < o.color : depth < o.depth; }
    };
    struct Texture {
        uint32_t addr = 0, format = 0, w = 0, h = 0, d = 0, levels = 0, pitch = 0, kind = 0;
        uint64_t hash = 0, lastUse = 0;
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory mem = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
    };

    Buffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props);
    VkCommandBuffer cmd() { return frames_[frame_].cmd; }
    void beginFrame();
    void submitFrame(bool wait);
    Surface* findSurface(uint32_t addr, bool depth);
    Surface* getSurface(uint32_t addr, uint32_t w, uint32_t h, uint32_t pitch, VkFormat fmt, bool depth);
    void destroySurface(Surface& s);
    void transition(Surface& s, VkImageLayout to);
    void bindTargets();
    void beginPass();
    void endPass();
    void initPipelineObjects();
    // Upload `bytes` to this frame's ring buffer; returns the offset (aligned to `align`).
    VkDeviceSize upload(const void* data, VkDeviceSize bytes, VkDeviceSize align = 16);
    void draw(const std::vector<uint32_t>* indices, uint32_t first, uint32_t count, const uint8_t* inlineData, uint32_t inlineStride);
    VkPipeline pipeline(uint64_t key, VkRenderPass pass, uint32_t attribMask, const uint32_t* attribFormats, uint32_t topology);
    VkShaderModule vertexShader(uint32_t attribMask, const uint32_t* attribFormats, uint64_t* key);
    VkShaderModule fragmentShader(uint64_t* key);
    VkImageView texture(int stage, uint32_t* kind);
    VkSampler sampler(int stage);

    int scale_ = 1;
    VkFormat depthFormat_ = VK_FORMAT_D24_UNORM_S8_UINT;
    VkCommandPool pool_ = VK_NULL_HANDLE;
    std::array<Frame, 2> frames_;
    int frame_ = 0;
    bool recording_ = false;
    uint64_t frames_done_ = 0;
    std::mutex surfLock_;
    std::deque<Surface> surfaces_;  // stable references
    uint64_t surfaceStamp_ = 0;
    std::map<FbKey, VkFramebuffer> framebuffers_;
    std::map<uint64_t, VkRenderPass> renderPasses_;
    struct Target { uint32_t color = 0, depth = 0, w = 0, h = 0; } target_;
    VkRenderPass pass_ = VK_NULL_HANDLE;   // active render pass (null outside one)
    VkFramebuffer passFb_ = VK_NULL_HANDLE;
    VkRenderPass passRp_ = VK_NULL_HANDLE;

    // Pipelines and shaders.
    VkDescriptorSetLayout setLayout_ = VK_NULL_HANDLE;
    VkPipelineLayout pipeLayout_ = VK_NULL_HANDLE;
    std::unordered_map<uint64_t, VkPipeline> pipelines_;
    std::unordered_map<uint64_t, VkShaderModule> shaders_;
    std::unordered_map<uint64_t, Texture> textures_;
    std::unordered_map<uint64_t, VkSampler> samplers_;
    VkImage dummyImage_ = VK_NULL_HANDLE;
    VkDeviceMemory dummyMem_ = VK_NULL_HANDLE;
    VkImageView dummy2D_ = VK_NULL_HANDLE, dummyCube_ = VK_NULL_HANDLE, dummy3D_ = VK_NULL_HANDLE;
    VkImage dummy3DImage_ = VK_NULL_HANDLE, dummyCubeImage_ = VK_NULL_HANDLE;

    // Primitive assembly.
    uint32_t primitive_ = 0;
    std::vector<uint32_t> indices_;      // ARRAY_ELEMENT batches of the current Begin/End
    std::vector<uint8_t> inline_;        // INLINE_ARRAY data of the current Begin/End
    std::vector<std::pair<uint32_t, uint32_t>> arrays_;  // DRAW_ARRAYS ranges
    float attrib_[16][4] = {};           // current inline attribute values
    std::vector<float> immediate_;       // vertices emitted by SET_VERTEX_DATA (slot 0 write)
};

} // namespace xb::gpu
