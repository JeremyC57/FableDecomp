// NV2A texture decoding to BGRA8 (VK_FORMAT_B8G8R8A8_UNORM): swizzled and linear colour
// formats, DXT1/3/5, palettes.
#include "nv2a_texture.hpp"

#include <cstring>

namespace xb::gpu {

namespace {
// Swizzled (Morton-order) offset of texel (x, y, z) in a w x h x d power-of-two texture.
struct Swizzle {
    uint32_t mx = 0, my = 0, mz = 0;
    Swizzle(uint32_t w, uint32_t h, uint32_t d) {
        uint32_t bit = 1;
        for (uint32_t i = 1; i < w || i < h || i < d; i <<= 1) {
            if (i < w) { mx |= bit; bit <<= 1; }
            if (i < h) { my |= bit; bit <<= 1; }
            if (i < d) { mz |= bit; bit <<= 1; }
        }
    }
    static uint32_t deposit(uint32_t v, uint32_t mask) {
        uint32_t r = 0;
        for (uint32_t bit = 1; mask; bit <<= 1) {
            const uint32_t low = mask & (~mask + 1);
            if (v & bit) r |= low;
            mask &= mask - 1;
        }
        return r;
    }
    uint32_t at(uint32_t x, uint32_t y, uint32_t z) const { return deposit(x, mx) | deposit(y, my) | deposit(z, mz); }
};

inline uint32_t bgra(uint32_t a, uint32_t r, uint32_t g, uint32_t b) { return (a << 24) | (r << 16) | (g << 8) | b; }
inline uint32_t e5(uint32_t v) { return (v << 3) | (v >> 2); }
inline uint32_t e6(uint32_t v) { return (v << 2) | (v >> 4); }
inline uint32_t e4(uint32_t v) { return (v << 4) | v; }

uint32_t texel(uint32_t fmt, const uint8_t* p, const uint32_t* palette) {
    uint16_t v16;
    uint32_t v32;
    switch (fmt) {
    case 0x00: case 0x13:  // Y8
        return bgra(255, p[0], p[0], p[0]);
    case 0x01: case 0x1B:  // AY8: luminance replicated to alpha
        return bgra(p[0], p[0], p[0], p[0]);
    case 0x19: case 0x1F:  // A8
        return bgra(p[0], 0, 0, 0);
    case 0x1A: case 0x20:  // A8Y8
        return bgra(p[1], p[0], p[0], p[0]);
    case 0x02: case 0x10:  // A1R5G5B5
        std::memcpy(&v16, p, 2);
        return bgra((v16 & 0x8000) ? 255 : 0, e5((v16 >> 10) & 31), e5((v16 >> 5) & 31), e5(v16 & 31));
    case 0x03: case 0x1C:  // X1R5G5B5
        std::memcpy(&v16, p, 2);
        return bgra(255, e5((v16 >> 10) & 31), e5((v16 >> 5) & 31), e5(v16 & 31));
    case 0x04: case 0x1D:  // A4R4G4B4
        std::memcpy(&v16, p, 2);
        return bgra(e4(v16 >> 12), e4((v16 >> 8) & 15), e4((v16 >> 4) & 15), e4(v16 & 15));
    case 0x05: case 0x11:  // R5G6B5
        std::memcpy(&v16, p, 2);
        return bgra(255, e5(v16 >> 11), e6((v16 >> 5) & 63), e5(v16 & 31));
    case 0x27:  // R6G5B5
        std::memcpy(&v16, p, 2);
        return bgra(255, e6(v16 >> 10), e5((v16 >> 5) & 31), e5(v16 & 31));
    case 0x06: case 0x12:  // A8R8G8B8
        std::memcpy(&v32, p, 4);
        return v32;
    case 0x07: case 0x1E:  // X8R8G8B8
        std::memcpy(&v32, p, 4);
        return v32 | 0xFF000000u;
    case 0x3A: case 0x3F:  // A8B8G8R8
        std::memcpy(&v32, p, 4);
        return (v32 & 0xFF00FF00u) | ((v32 >> 16) & 0xFF) | ((v32 & 0xFF) << 16);
    case 0x3B: case 0x40:  // B8G8R8A8
        std::memcpy(&v32, p, 4);
        return bgra(v32 & 0xFF, (v32 >> 8) & 0xFF, (v32 >> 16) & 0xFF, v32 >> 24);
    case 0x3C: case 0x41:  // R8G8B8A8
        std::memcpy(&v32, p, 4);
        return bgra(v32 & 0xFF, v32 >> 24, (v32 >> 16) & 0xFF, (v32 >> 8) & 0xFF);
    case 0x17: case 0x28:  // G8B8
        return bgra(255, p[1], p[1], p[0]);
    case 0x29:  // R8B8
        return bgra(255, p[1], 0, p[0]);
    case 0x0B:  // I8 (palette)
        return palette ? palette[p[0]] : bgra(255, p[0], p[0], p[0]);
    case 0x35: case 0x2C: case 0x30: case 0x31:  // 16-bit luminance / depth
        std::memcpy(&v16, p, 2);
        return bgra(255, v16 >> 8, v16 >> 8, v16 >> 8);
    case 0x2E: case 0x2F:  // depth X8_Y24
        std::memcpy(&v32, p, 4);
        return bgra(255, (v32 >> 24) & 0xFF, (v32 >> 16) & 0xFF, (v32 >> 8) & 0xFF);
    default:
        return 0xFFFF00FFu;  // unknown: magenta
    }
}

void dxtColors(const uint8_t* b, uint32_t c[4], bool dxt1) {
    uint16_t c0, c1;
    std::memcpy(&c0, b, 2);
    std::memcpy(&c1, b + 2, 2);
    const uint32_t r0 = e5(c0 >> 11), g0 = e6((c0 >> 5) & 63), b0 = e5(c0 & 31);
    const uint32_t r1 = e5(c1 >> 11), g1 = e6((c1 >> 5) & 63), b1 = e5(c1 & 31);
    c[0] = bgra(255, r0, g0, b0);
    c[1] = bgra(255, r1, g1, b1);
    if (!dxt1 || c0 > c1) {
        c[2] = bgra(255, (2 * r0 + r1) / 3, (2 * g0 + g1) / 3, (2 * b0 + b1) / 3);
        c[3] = bgra(255, (r0 + 2 * r1) / 3, (g0 + 2 * g1) / 3, (b0 + 2 * b1) / 3);
    } else {
        c[2] = bgra(255, (r0 + r1) / 2, (g0 + g1) / 2, (b0 + b1) / 2);
        c[3] = 0;
    }
}

void decodeDxt(uint32_t fmt, const uint8_t* src, uint32_t w, uint32_t h, uint32_t* out) {
    const uint32_t blockBytes = fmt == 0x0C ? 8 : 16;
    const uint32_t bw = (w + 3) / 4, bh = (h + 3) / 4;
    for (uint32_t by = 0; by < bh; ++by)
        for (uint32_t bx = 0; bx < bw; ++bx) {
            const uint8_t* b = src + (by * bw + bx) * blockBytes;
            uint8_t alpha[16];
            std::memset(alpha, 255, 16);
            const uint8_t* cb = b;
            if (fmt == 0x0E) {  // DXT2/3: explicit 4-bit alpha
                for (int i = 0; i < 16; ++i) alpha[i] = static_cast<uint8_t>(e4((b[i / 2] >> ((i & 1) * 4)) & 15));
                cb = b + 8;
            } else if (fmt == 0x0F) {  // DXT4/5: interpolated alpha
                const uint32_t a0 = b[0], a1 = b[1];
                uint32_t pal[8] = {a0, a1};
                if (a0 > a1) for (int i = 1; i < 7; ++i) pal[i + 1] = ((7 - i) * a0 + i * a1) / 7;
                else {
                    for (int i = 1; i < 5; ++i) pal[i + 1] = ((5 - i) * a0 + i * a1) / 5;
                    pal[6] = 0;
                    pal[7] = 255;
                }
                uint64_t bits = 0;
                for (int i = 0; i < 6; ++i) bits |= static_cast<uint64_t>(b[2 + i]) << (8 * i);
                for (int i = 0; i < 16; ++i) alpha[i] = static_cast<uint8_t>(pal[(bits >> (3 * i)) & 7]);
                cb = b + 8;
            }
            uint32_t c[4];
            dxtColors(cb, c, fmt == 0x0C);
            uint32_t idx;
            std::memcpy(&idx, cb + 4, 4);
            for (uint32_t i = 0; i < 16; ++i) {
                const uint32_t x = bx * 4 + (i & 3), y = by * 4 + (i >> 2);
                if (x >= w || y >= h) continue;
                uint32_t px = c[(idx >> (2 * i)) & 3];
                if (fmt != 0x0C) px = (px & 0x00FFFFFFu) | (static_cast<uint32_t>(alpha[i]) << 24);
                out[y * w + x] = px;
            }
        }
}
}  // namespace

uint32_t texelBytes(uint32_t fmt) {
    switch (fmt) {
    case 0x00: case 0x01: case 0x0B: case 0x13: case 0x19: case 0x1B: case 0x1F: return 1;
    case 0x02: case 0x03: case 0x04: case 0x05: case 0x10: case 0x11: case 0x17: case 0x1A: case 0x1C: case 0x1D:
    case 0x20: case 0x27: case 0x28: case 0x29: case 0x2C: case 0x30: case 0x31: case 0x35: return 2;
    case 0x0C: case 0x0E: case 0x0F: return 0;  // block compressed
    default: return 4;
    }
}
bool isLinear(uint32_t fmt) { return (fmt >= 0x10 && fmt <= 0x26 && fmt != 0x19 && fmt != 0x1A) || fmt == 0x2E || fmt == 0x2F || fmt == 0x30 || fmt == 0x31 || fmt == 0x35 || fmt >= 0x3F; }
bool isCompressed(uint32_t fmt) { return fmt == 0x0C || fmt == 0x0E || fmt == 0x0F; }

uint32_t levelBytes(uint32_t fmt, uint32_t w, uint32_t h, uint32_t d) {
    if (isCompressed(fmt)) return ((w + 3) / 4) * ((h + 3) / 4) * (fmt == 0x0C ? 8 : 16) * d;
    return w * h * d * texelBytes(fmt);
}

void decodeLevel(uint32_t fmt, const uint8_t* src, uint32_t w, uint32_t h, uint32_t d, uint32_t pitch, const uint32_t* palette,
                 uint32_t* out) {
    if (isCompressed(fmt)) {
        for (uint32_t z = 0; z < d; ++z) decodeDxt(fmt, src + levelBytes(fmt, w, h, 1) * z, w, h, out + w * h * z);
        return;
    }
    const uint32_t bpp = texelBytes(fmt);
    if (isLinear(fmt)) {
        for (uint32_t y = 0; y < h; ++y)
            for (uint32_t x = 0; x < w; ++x) out[y * w + x] = texel(fmt, src + y * pitch + x * bpp, palette);
        return;
    }
    const Swizzle sw(w, h, d);
    for (uint32_t z = 0; z < d; ++z)
        for (uint32_t y = 0; y < h; ++y)
            for (uint32_t x = 0; x < w; ++x) out[(z * h + y) * w + x] = texel(fmt, src + sw.at(x, y, z) * bpp, palette);
}

} // namespace xb::gpu
