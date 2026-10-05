// NV2A emulation: device model (nv2a.cpp) <-> renderer (Vulkan, vk_renderer.cpp).
#pragma once

#include <cstdint>

namespace xb::gpu {

// Kelvin state as the push buffer set it: regs[method / 4] holds the last value written to
// each method. Object/DMA methods hold RAMIN instance offsets (handles already translated).
struct State {
    uint32_t regs[0x2000 / 4] = {};
    uint32_t scanout = 0;  // PCRTC_START: physical address being displayed
    // Vertex program (136 four-word instructions) and its 192 constants, as uploaded through
    // SET_TRANSFORM_PROGRAM / SET_TRANSFORM_CONSTANT at the LOAD pointers.
    uint32_t program[136][4] = {};
    uint32_t programLoad = 0, constantLoad = 0;
    float constants[192][4] = {};
    uint64_t programGeneration = 1;  // bumped when the program changes (shader cache key)
    bool constantsDirty = true;
};
State& state();

// Physical address and limit of a DMA object (RAMIN instance offset).
uint32_t dmaAddress(uint32_t instance, uint32_t* limit);

// Drawing interface the device model calls from the pusher thread (with the device lock held).
class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void flush() {}                    // finish pending work (CPU may read results next)
    virtual void endFrame() {}                 // FLIP_STALL: the frame's rendering is complete
    virtual void vblank() {}                   // 60 Hz: present the scanout surface
    virtual void clear(uint32_t flags) = 0;    // CLEAR_SURFACE
    virtual void begin(uint32_t primitive) = 0;
    virtual void end() = 0;
    virtual void drawArrays(uint32_t start, uint32_t count) = 0;
    virtual void inlineArray(const uint32_t* words, uint32_t n) = 0;
    virtual void arrayElements(const uint32_t* words, uint32_t n, bool sixteenBit) = 0;
    virtual void vertexAttribute(uint32_t method, uint32_t value) = 0;
    // Pixels that passed the depth test since the last CLEAR_REPORT_VALUE (occlusion queries:
    // Fable culls objects whose bounding box reports 0). Until real queries exist: "visible".
    virtual uint32_t zpassCount() { return 0x10000; }
};
Renderer& renderer();

void init();
uint32_t mmioRead(uint32_t a, int size);
void mmioWrite(uint32_t a, uint32_t v, int size);

// Software 2D engine (context surfaces + image blit), in guest memory.
void surfaces2d(uint32_t method, uint32_t param);
void imageBlit(uint32_t method, uint32_t param);

} // namespace xb::gpu
