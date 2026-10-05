// Fallback renderer: clears in guest memory, draws counted but not rasterised. Used before
// (or without) Vulkan; FABLE_DUMP_FRAMES=N writes every Nth scanout frame to frame_*.png.
#include "gpu.hpp"
#include "nv2a_methods.h"
#include "xhost.hpp"

#include <atomic>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../posix/third_party/stb_image_write.h"

namespace xb {
extern uint32_t g_avFramebuffer, g_avPitch, g_avFormat;
}

namespace xb::gpu {

namespace {
uint32_t bytesPerPixel(uint32_t colorFormat) {
    switch (colorFormat) {
    case 0x01: case 0x02: case 0x03: return 2;
    case 0x09: return 1;
    case 0x0A: return 2;
    default: return 4;
    }
}
}

void softClear(uint32_t flags) {
    const State& s = state();
    if (!(flags & 0xF0)) return;
    const uint32_t fmt = s.regs[NV097_SET_SURFACE_FORMAT / 4];
    const uint32_t bpp = bytesPerPixel(fmt & 0xF);
    const uint32_t pitch = s.regs[NV097_SET_SURFACE_PITCH / 4] & 0xFFFF;
    const uint32_t base = kContigBase + dmaAddress(s.regs[NV097_SET_CONTEXT_DMA_COLOR / 4], nullptr) + s.regs[NV097_SET_SURFACE_COLOR_OFFSET / 4];
    const uint32_t h = s.regs[NV097_SET_CLEAR_RECT_HORIZONTAL / 4], v = s.regs[NV097_SET_CLEAR_RECT_VERTICAL / 4];
    const uint32_t x0 = h & 0xFFFF, x1 = h >> 16, y0 = v & 0xFFFF, y1 = v >> 16;
    const uint32_t color = s.regs[NV097_SET_COLOR_CLEAR_VALUE / 4];
    if (!pitch || x1 < x0 || y1 < y0 || x1 > 4096 || y1 > 4096) return;
    for (uint32_t y = y0; y <= y1; ++y)
        for (uint32_t x = x0; x <= x1; ++x) {
            const uint32_t a = base + y * pitch + x * bpp;
            if (bpp == 4) wr32(a, color);
            else if (bpp == 2) wr16(a, static_cast<uint16_t>(((color >> 8) & 0xF800) | ((color >> 5) & 0x07E0) | ((color >> 3) & 0x1F)));
            else wr8(a, static_cast<uint8_t>(color));
        }
}

void dumpScanout(const char* path) {
    const uint32_t fb = kContigBase + (state().scanout ? state().scanout : physOf(g_avFramebuffer));
    const uint32_t pitch = g_avPitch ? g_avPitch : 640 * 4;
    const uint32_t w = pitch / 4, h = 480;
    if (!fb || w > 2048) return;
    std::vector<uint8_t> rgb(static_cast<size_t>(w) * h * 3);
    for (uint32_t y = 0; y < h; ++y)
        for (uint32_t x = 0; x < w; ++x) {
            const uint32_t p = rd32(fb + y * pitch + x * 4);
            uint8_t* o = &rgb[(static_cast<size_t>(y) * w + x) * 3];
            o[0] = static_cast<uint8_t>(p >> 16);
            o[1] = static_cast<uint8_t>(p >> 8);
            o[2] = static_cast<uint8_t>(p);
        }
    stbi_write_png(path, static_cast<int>(w), static_cast<int>(h), 3, rgb.data(), static_cast<int>(w * 3));
}

extern std::atomic<uint32_t> g_vblanks;

class SoftRenderer final : public Renderer {
public:
    void vblank() override { ++g_vblanks; }
    void clear(uint32_t flags) override { softClear(flags); }
    void begin(uint32_t prim) override { prim_ = prim; }
    void end() override { ++draws_; }
    void drawArrays(uint32_t, uint32_t) override {}
    void inlineArray(const uint32_t*, uint32_t) override {}
    void arrayElements(const uint32_t*, uint32_t, bool) override {}
    void vertexAttribute(uint32_t, uint32_t) override {}
    void endFrame() override {
        ++frames_;
        if (frames_ % 60 == 0) XLOG(1, "NV2A: %llu frames, %llu draws in the last 60", static_cast<unsigned long long>(frames_),
                                    static_cast<unsigned long long>(draws_ - lastDraws_));
        if (frames_ % 60 == 0) lastDraws_ = draws_;
        static const int every = getenv("FABLE_DUMP_FRAMES") ? atoi(getenv("FABLE_DUMP_FRAMES")) : 0;
        if (every > 0 && frames_ % static_cast<uint64_t>(every) == 0) {
            char name[64];
            snprintf(name, sizeof name, "frame_%05llu.png", static_cast<unsigned long long>(frames_));
            dumpScanout(name);
        }
    }

private:
    uint32_t prim_ = 0;
    uint64_t draws_ = 0, lastDraws_ = 0, frames_ = 0;
};

Renderer& softRenderer() {
    static SoftRenderer r;
    return r;
}

} // namespace xb::gpu
