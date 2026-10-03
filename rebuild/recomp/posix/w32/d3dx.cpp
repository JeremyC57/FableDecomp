// The D3DX 9 functions the game uses, on the plain Direct3D 9 API (DXVK Native has no
// D3DX). Registered as "d3dx9_43.dll" so the host's D3DX forwarding (win/d3d9.cpp) finds
// them. Pixels go through RGBA float images: decode from any supported format (including
// DXT1-5 and palettised P8), optional resampling (point / bilinear / box), colour key,
// encode to the destination format (including a DXT encoder). Files: DDS (own loader) and
// TGA / BMP / PNG / JPG (stb_image); screenshots are written with stb_image_write.
#include "w32.hpp"  // first: the Windows headers' configuration

#include <d3d9.h>
#include <d3dx9.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_NO_SIMD
#include "../third_party/stb_image.h"
#include "../third_party/stb_image_write.h"

namespace w32 {
void registerHostModule(const char* dll, const char* fn, void* addr);

namespace {

// ============================================================================
// images and pixel formats
// ============================================================================
struct Px { float r = 0, g = 0, b = 0, a = 1; };
struct Image {
    int w = 0, h = 0;
    std::vector<Px> px;
    Px& at(int x, int y) { return px[static_cast<size_t>(y) * w + x]; }
    const Px& at(int x, int y) const { return px[static_cast<size_t>(y) * w + x]; }
};

bool isDxt(D3DFORMAT f) {
    return f == D3DFMT_DXT1 || f == D3DFMT_DXT2 || f == D3DFMT_DXT3 || f == D3DFMT_DXT4 || f == D3DFMT_DXT5;
}
int blockBytes(D3DFORMAT f) { return f == D3DFMT_DXT1 ? 8 : 16; }
// Bits per pixel of uncompressed formats this file handles; 0 = unsupported.
int bitsPerPixel(D3DFORMAT f) {
    switch (f) {
        case D3DFMT_A32B32G32R32F: return 128;
        case D3DFMT_A16B16G16R16F: case D3DFMT_A16B16G16R16: case D3DFMT_G32R32F: return 64;
        case D3DFMT_A8R8G8B8: case D3DFMT_X8R8G8B8: case D3DFMT_A8B8G8R8: case D3DFMT_X8B8G8R8: case D3DFMT_A2R10G10B10:
        case D3DFMT_A2B10G10R10: case D3DFMT_G16R16: case D3DFMT_Q8W8V8U8: case D3DFMT_R32F: case D3DFMT_G16R16F: return 32;
        case D3DFMT_R8G8B8: return 24;
        case D3DFMT_R5G6B5: case D3DFMT_X1R5G5B5: case D3DFMT_A1R5G5B5: case D3DFMT_A4R4G4B4: case D3DFMT_X4R4G4B4: case D3DFMT_A8L8:
        case D3DFMT_V8U8: case D3DFMT_L16: case D3DFMT_R16F: case D3DFMT_A8R3G3B2: return 16;
        case D3DFMT_A8: case D3DFMT_L8: case D3DFMT_A4L4: case D3DFMT_P8: case D3DFMT_R3G3B2: return 8;
        default: return 0;
    }
}
bool supported(D3DFORMAT f) { return isDxt(f) || bitsPerPixel(f) != 0; }

inline float u(uint32_t v, int bits) { return static_cast<float>(v) / static_cast<float>((1u << bits) - 1); }
inline uint32_t q(float v, int bits) {
    const float m = static_cast<float>((1u << bits) - 1);
    return static_cast<uint32_t>(std::clamp(v, 0.0f, 1.0f) * m + 0.5f);
}
inline float s8(uint8_t v) { return std::max(-1.0f, static_cast<int8_t>(v) / 127.0f); }
inline uint8_t qs8(float v) { return static_cast<uint8_t>(static_cast<int8_t>(std::lround(std::clamp(v, -1.0f, 1.0f) * 127.0f))); }

float half(uint16_t h) {
    const uint32_t s = (h >> 15) & 1, e = (h >> 10) & 31, m = h & 1023;
    float v;
    if (e == 0) v = std::ldexp(static_cast<float>(m), -24);
    else if (e == 31) v = m ? NAN : INFINITY;
    else v = std::ldexp(static_cast<float>(m | 1024), static_cast<int>(e) - 25);
    return s ? -v : v;
}
uint16_t toHalf(float f) {
    uint32_t x;
    std::memcpy(&x, &f, 4);
    const uint32_t s = (x >> 16) & 0x8000;
    int e = static_cast<int>((x >> 23) & 255) - 127 + 15;
    uint32_t m = x & 0x7FFFFF;
    if (e <= 0) return static_cast<uint16_t>(s);
    if (e >= 31) return static_cast<uint16_t>(s | 0x7C00);
    return static_cast<uint16_t>(s | (static_cast<uint32_t>(e) << 10) | (m >> 13));
}

Px readPixel(D3DFORMAT f, const uint8_t* p, const PALETTEENTRY* pal) {
    Px o;
    uint32_t v = 0;
    switch (bitsPerPixel(f)) {
        case 8: v = p[0]; break;
        case 16: v = p[0] | (p[1] << 8); break;
        case 24: v = p[0] | (p[1] << 8) | (p[2] << 16); break;
        default: v = p[0] | (p[1] << 8) | (p[2] << 16) | (static_cast<uint32_t>(p[3]) << 24); break;
    }
    switch (f) {
        case D3DFMT_A8R8G8B8: o = {u((v >> 16) & 255, 8), u((v >> 8) & 255, 8), u(v & 255, 8), u(v >> 24, 8)}; break;
        case D3DFMT_X8R8G8B8: case D3DFMT_R8G8B8: o = {u((v >> 16) & 255, 8), u((v >> 8) & 255, 8), u(v & 255, 8), 1}; break;
        case D3DFMT_A8B8G8R8: o = {u(v & 255, 8), u((v >> 8) & 255, 8), u((v >> 16) & 255, 8), u(v >> 24, 8)}; break;
        case D3DFMT_X8B8G8R8: o = {u(v & 255, 8), u((v >> 8) & 255, 8), u((v >> 16) & 255, 8), 1}; break;
        case D3DFMT_A2R10G10B10: o = {u((v >> 20) & 1023, 10), u((v >> 10) & 1023, 10), u(v & 1023, 10), u(v >> 30, 2)}; break;
        case D3DFMT_A2B10G10R10: o = {u(v & 1023, 10), u((v >> 10) & 1023, 10), u((v >> 20) & 1023, 10), u(v >> 30, 2)}; break;
        case D3DFMT_G16R16: o = {u(v & 0xFFFF, 16), u(v >> 16, 16), 1, 1}; break;
        case D3DFMT_R5G6B5: o = {u((v >> 11) & 31, 5), u((v >> 5) & 63, 6), u(v & 31, 5), 1}; break;
        case D3DFMT_X1R5G5B5: o = {u((v >> 10) & 31, 5), u((v >> 5) & 31, 5), u(v & 31, 5), 1}; break;
        case D3DFMT_A1R5G5B5: o = {u((v >> 10) & 31, 5), u((v >> 5) & 31, 5), u(v & 31, 5), static_cast<float>(v >> 15)}; break;
        case D3DFMT_A4R4G4B4: o = {u((v >> 8) & 15, 4), u((v >> 4) & 15, 4), u(v & 15, 4), u((v >> 12) & 15, 4)}; break;
        case D3DFMT_X4R4G4B4: o = {u((v >> 8) & 15, 4), u((v >> 4) & 15, 4), u(v & 15, 4), 1}; break;
        case D3DFMT_A8R3G3B2: o = {u((v >> 5) & 7, 3), u((v >> 2) & 7, 3), u(v & 3, 2), u(v >> 8, 8)}; break;
        case D3DFMT_R3G3B2: o = {u((v >> 5) & 7, 3), u((v >> 2) & 7, 3), u(v & 3, 2), 1}; break;
        case D3DFMT_A8: o = {0, 0, 0, u(v, 8)}; break;
        case D3DFMT_L8: o = {u(v, 8), u(v, 8), u(v, 8), 1}; break;
        case D3DFMT_A8L8: o = {u(v & 255, 8), u(v & 255, 8), u(v & 255, 8), u(v >> 8, 8)}; break;
        case D3DFMT_A4L4: o = {u(v & 15, 4), u(v & 15, 4), u(v & 15, 4), u(v >> 4, 4)}; break;
        case D3DFMT_L16: o = {u(v, 16), u(v, 16), u(v, 16), 1}; break;
        case D3DFMT_V8U8: o = {s8(static_cast<uint8_t>(v)), s8(static_cast<uint8_t>(v >> 8)), 1, 1}; break;
        case D3DFMT_Q8W8V8U8:
            o = {s8(static_cast<uint8_t>(v)), s8(static_cast<uint8_t>(v >> 8)), s8(static_cast<uint8_t>(v >> 16)), s8(static_cast<uint8_t>(v >> 24))};
            break;
        case D3DFMT_P8:
            if (pal) o = {u(pal[v].peRed, 8), u(pal[v].peGreen, 8), u(pal[v].peBlue, 8), u(pal[v].peFlags, 8)};
            break;
        case D3DFMT_R16F: o = {half(static_cast<uint16_t>(v)), 1, 1, 1}; break;
        case D3DFMT_G16R16F: o = {half(static_cast<uint16_t>(v)), half(static_cast<uint16_t>(v >> 16)), 1, 1}; break;
        case D3DFMT_R32F: { float f32; std::memcpy(&f32, p, 4); o = {f32, 1, 1, 1}; break; }
        case D3DFMT_G32R32F: { float f2[2]; std::memcpy(f2, p, 8); o = {f2[0], f2[1], 1, 1}; break; }
        case D3DFMT_A16B16G16R16F: {
            uint16_t h4[4];
            std::memcpy(h4, p, 8);
            o = {half(h4[0]), half(h4[1]), half(h4[2]), half(h4[3])};
            break;
        }
        case D3DFMT_A16B16G16R16: {
            uint16_t h4[4];
            std::memcpy(h4, p, 8);
            o = {u(h4[0], 16), u(h4[1], 16), u(h4[2], 16), u(h4[3], 16)};
            break;
        }
        case D3DFMT_A32B32G32R32F: std::memcpy(&o, p, 16); break;
        default: break;
    }
    return o;
}

void writePixel(D3DFORMAT f, uint8_t* p, const Px& c) {
    uint32_t v = 0;
    switch (f) {
        case D3DFMT_A8R8G8B8: v = (q(c.a, 8) << 24) | (q(c.r, 8) << 16) | (q(c.g, 8) << 8) | q(c.b, 8); break;
        case D3DFMT_X8R8G8B8: v = 0xFF000000u | (q(c.r, 8) << 16) | (q(c.g, 8) << 8) | q(c.b, 8); break;
        case D3DFMT_R8G8B8: v = (q(c.r, 8) << 16) | (q(c.g, 8) << 8) | q(c.b, 8); break;
        case D3DFMT_A8B8G8R8: v = (q(c.a, 8) << 24) | (q(c.b, 8) << 16) | (q(c.g, 8) << 8) | q(c.r, 8); break;
        case D3DFMT_X8B8G8R8: v = 0xFF000000u | (q(c.b, 8) << 16) | (q(c.g, 8) << 8) | q(c.r, 8); break;
        case D3DFMT_A2R10G10B10: v = (q(c.a, 2) << 30) | (q(c.r, 10) << 20) | (q(c.g, 10) << 10) | q(c.b, 10); break;
        case D3DFMT_A2B10G10R10: v = (q(c.a, 2) << 30) | (q(c.b, 10) << 20) | (q(c.g, 10) << 10) | q(c.r, 10); break;
        case D3DFMT_G16R16: v = (q(c.g, 16) << 16) | q(c.r, 16); break;
        case D3DFMT_R5G6B5: v = (q(c.r, 5) << 11) | (q(c.g, 6) << 5) | q(c.b, 5); break;
        case D3DFMT_X1R5G5B5: v = 0x8000u | (q(c.r, 5) << 10) | (q(c.g, 5) << 5) | q(c.b, 5); break;
        case D3DFMT_A1R5G5B5: v = (c.a >= 0.5f ? 0x8000u : 0) | (q(c.r, 5) << 10) | (q(c.g, 5) << 5) | q(c.b, 5); break;
        case D3DFMT_A4R4G4B4: v = (q(c.a, 4) << 12) | (q(c.r, 4) << 8) | (q(c.g, 4) << 4) | q(c.b, 4); break;
        case D3DFMT_X4R4G4B4: v = 0xF000u | (q(c.r, 4) << 8) | (q(c.g, 4) << 4) | q(c.b, 4); break;
        case D3DFMT_A8R3G3B2: v = (q(c.a, 8) << 8) | (q(c.r, 3) << 5) | (q(c.g, 3) << 2) | q(c.b, 2); break;
        case D3DFMT_R3G3B2: v = (q(c.r, 3) << 5) | (q(c.g, 3) << 2) | q(c.b, 2); break;
        case D3DFMT_A8: v = q(c.a, 8); break;
        case D3DFMT_L8: v = q(0.299f * c.r + 0.587f * c.g + 0.114f * c.b, 8); break;
        case D3DFMT_A8L8: v = (q(c.a, 8) << 8) | q(0.299f * c.r + 0.587f * c.g + 0.114f * c.b, 8); break;
        case D3DFMT_A4L4: v = (q(c.a, 4) << 4) | q(0.299f * c.r + 0.587f * c.g + 0.114f * c.b, 4); break;
        case D3DFMT_L16: v = q(0.299f * c.r + 0.587f * c.g + 0.114f * c.b, 16); break;
        case D3DFMT_V8U8: v = qs8(c.r) | (static_cast<uint32_t>(qs8(c.g)) << 8); break;
        case D3DFMT_Q8W8V8U8: v = qs8(c.r) | (static_cast<uint32_t>(qs8(c.g)) << 8) | (static_cast<uint32_t>(qs8(c.b)) << 16) | (static_cast<uint32_t>(qs8(c.a)) << 24); break;
        case D3DFMT_R16F: v = toHalf(c.r); break;
        case D3DFMT_G16R16F: v = toHalf(c.r) | (static_cast<uint32_t>(toHalf(c.g)) << 16); break;
        case D3DFMT_R32F: std::memcpy(p, &c.r, 4); return;
        case D3DFMT_G32R32F: std::memcpy(p, &c.r, 8); return;
        case D3DFMT_A16B16G16R16F: {
            const uint16_t h4[4] = {toHalf(c.r), toHalf(c.g), toHalf(c.b), toHalf(c.a)};
            std::memcpy(p, h4, 8);
            return;
        }
        case D3DFMT_A16B16G16R16: {
            const uint16_t h4[4] = {static_cast<uint16_t>(q(c.r, 16)), static_cast<uint16_t>(q(c.g, 16)), static_cast<uint16_t>(q(c.b, 16)), static_cast<uint16_t>(q(c.a, 16))};
            std::memcpy(p, h4, 8);
            return;
        }
        case D3DFMT_A32B32G32R32F: std::memcpy(p, &c, 16); return;
        default: return;
    }
    const int n = bitsPerPixel(f) / 8;
    for (int i = 0; i < n; ++i) p[i] = static_cast<uint8_t>(v >> (8 * i));
}

// ---- DXT ----------------------------------------------------------------------------------
Px from565(uint16_t c) { return {u((c >> 11) & 31, 5), u((c >> 5) & 63, 6), u(c & 31, 5), 1}; }
uint16_t to565(const Px& c) { return static_cast<uint16_t>((q(c.r, 5) << 11) | (q(c.g, 6) << 5) | q(c.b, 5)); }

void decodeBlock(D3DFORMAT f, const uint8_t* b, Px out[16]) {
    const uint8_t* colour = f == D3DFMT_DXT1 ? b : b + 8;
    const uint16_t c0 = colour[0] | (colour[1] << 8), c1 = colour[2] | (colour[3] << 8);
    Px pal[4] = {from565(c0), from565(c1), {}, {}};
    if (c0 > c1 || f != D3DFMT_DXT1) {
        pal[2] = {(2 * pal[0].r + pal[1].r) / 3, (2 * pal[0].g + pal[1].g) / 3, (2 * pal[0].b + pal[1].b) / 3, 1};
        pal[3] = {(pal[0].r + 2 * pal[1].r) / 3, (pal[0].g + 2 * pal[1].g) / 3, (pal[0].b + 2 * pal[1].b) / 3, 1};
    } else {
        pal[2] = {(pal[0].r + pal[1].r) / 2, (pal[0].g + pal[1].g) / 2, (pal[0].b + pal[1].b) / 2, 1};
        pal[3] = {0, 0, 0, 0};
    }
    const uint32_t idx = colour[4] | (colour[5] << 8) | (colour[6] << 16) | (static_cast<uint32_t>(colour[7]) << 24);
    for (int i = 0; i < 16; ++i) out[i] = pal[(idx >> (2 * i)) & 3];
    if (f == D3DFMT_DXT2 || f == D3DFMT_DXT3) {
        for (int i = 0; i < 16; ++i) out[i].a = u((b[i / 2] >> (4 * (i & 1))) & 15, 4);
    } else if (f == D3DFMT_DXT4 || f == D3DFMT_DXT5) {
        const float a0 = u(b[0], 8), a1 = u(b[1], 8);
        float a[8] = {a0, a1};
        if (b[0] > b[1]) for (int k = 1; k < 7; ++k) a[k + 1] = ((7 - k) * a0 + k * a1) / 7;
        else {
            for (int k = 1; k < 5; ++k) a[k + 1] = ((5 - k) * a0 + k * a1) / 5;
            a[6] = 0, a[7] = 1;
        }
        uint64_t bits = 0;
        for (int k = 0; k < 6; ++k) bits |= static_cast<uint64_t>(b[2 + k]) << (8 * k);
        for (int i = 0; i < 16; ++i) out[i].a = a[(bits >> (3 * i)) & 7];
    }
}

float dist2(const Px& a, const Px& b) { return (a.r - b.r) * (a.r - b.r) + (a.g - b.g) * (a.g - b.g) + (a.b - b.b) * (a.b - b.b); }

void encodeBlock(D3DFORMAT f, const Px in[16], uint8_t* b) {
    // colour endpoints: extremes along the luminance axis of the opaque texels
    const bool punch = f == D3DFMT_DXT1 && std::any_of(in, in + 16, [](const Px& p) { return p.a < 0.5f; });
    int lo = 0, hi = 0;
    float lmin = 1e9f, lmax = -1e9f;
    for (int i = 0; i < 16; ++i) {
        if (punch && in[i].a < 0.5f) continue;
        const float l = 0.299f * in[i].r + 0.587f * in[i].g + 0.114f * in[i].b;
        if (l < lmin) lmin = l, lo = i;
        if (l > lmax) lmax = l, hi = i;
    }
    uint16_t c0 = to565(in[hi]), c1 = to565(in[lo]);
    if (!punch && c0 < c1) std::swap(c0, c1);
    if (punch && c0 > c1) std::swap(c0, c1);
    if (!punch && c0 == c1) { if (c1 > 0) --c1; else ++c0; }
    Px pal[4] = {from565(c0), from565(c1), {}, {}};
    if (!punch) {
        pal[2] = {(2 * pal[0].r + pal[1].r) / 3, (2 * pal[0].g + pal[1].g) / 3, (2 * pal[0].b + pal[1].b) / 3, 1};
        pal[3] = {(pal[0].r + 2 * pal[1].r) / 3, (pal[0].g + 2 * pal[1].g) / 3, (pal[0].b + 2 * pal[1].b) / 3, 1};
    } else {
        pal[2] = {(pal[0].r + pal[1].r) / 2, (pal[0].g + pal[1].g) / 2, (pal[0].b + pal[1].b) / 2, 1};
    }
    uint32_t idx = 0;
    for (int i = 0; i < 16; ++i) {
        int best = 0;
        if (punch && in[i].a < 0.5f) best = 3;
        else {
            float bd = 1e9f;
            for (int k = 0; k < (punch ? 3 : 4); ++k)
                if (const float d = dist2(in[i], pal[k]); d < bd) bd = d, best = k;
        }
        idx |= static_cast<uint32_t>(best) << (2 * i);
    }
    uint8_t* colour = f == D3DFMT_DXT1 ? b : b + 8;
    colour[0] = static_cast<uint8_t>(c0), colour[1] = static_cast<uint8_t>(c0 >> 8);
    colour[2] = static_cast<uint8_t>(c1), colour[3] = static_cast<uint8_t>(c1 >> 8);
    for (int k = 0; k < 4; ++k) colour[4 + k] = static_cast<uint8_t>(idx >> (8 * k));
    if (f == D3DFMT_DXT2 || f == D3DFMT_DXT3) {
        for (int i = 0; i < 8; ++i) b[i] = static_cast<uint8_t>(q(in[2 * i].a, 4) | (q(in[2 * i + 1].a, 4) << 4));
    } else if (f == D3DFMT_DXT4 || f == D3DFMT_DXT5) {
        float amin = 1, amax = 0;
        for (int i = 0; i < 16; ++i) amin = std::min(amin, in[i].a), amax = std::max(amax, in[i].a);
        uint8_t a0 = static_cast<uint8_t>(q(amax, 8)), a1 = static_cast<uint8_t>(q(amin, 8));
        if (a0 == a1) { if (a0 < 255) ++a0; else --a1; }
        b[0] = a0, b[1] = a1;
        float a[8] = {u(a0, 8), u(a1, 8)};
        for (int k = 1; k < 7; ++k) a[k + 1] = ((7 - k) * a[0] + k * a[1]) / 7;
        uint64_t bits = 0;
        for (int i = 0; i < 16; ++i) {
            int best = 0;
            float bd = 9;
            for (int k = 0; k < 8; ++k)
                if (const float d = std::fabs(in[i].a - a[k]); d < bd) bd = d, best = k;
            bits |= static_cast<uint64_t>(best) << (3 * i);
        }
        for (int k = 0; k < 6; ++k) b[2 + k] = static_cast<uint8_t>(bits >> (8 * k));
    }
}

// ---- whole-image decode/encode of a locked region ---------------------------------------------
// `mem` points at the region's first row (for DXT: its first block row); w/h are the region size.
Image decodeRegion(D3DFORMAT f, const uint8_t* mem, UINT pitch, int w, int h, const PALETTEENTRY* pal) {
    Image img;
    img.w = w, img.h = h;
    img.px.resize(static_cast<size_t>(w) * h);
    if (isDxt(f)) {
        const int bb = blockBytes(f);
        for (int by = 0; by < (h + 3) / 4; ++by)
            for (int bx = 0; bx < (w + 3) / 4; ++bx) {
                Px blk[16];
                decodeBlock(f, mem + static_cast<size_t>(by) * pitch + static_cast<size_t>(bx) * bb, blk);
                for (int i = 0; i < 16; ++i) {
                    const int x = bx * 4 + (i & 3), y = by * 4 + (i >> 2);
                    if (x < w && y < h) img.at(x, y) = blk[i];
                }
            }
        return img;
    }
    const int bpp = bitsPerPixel(f) / 8;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) img.at(x, y) = readPixel(f, mem + static_cast<size_t>(y) * pitch + static_cast<size_t>(x) * bpp, pal);
    return img;
}
void encodeRegion(D3DFORMAT f, uint8_t* mem, UINT pitch, const Image& img) {
    if (isDxt(f)) {
        const int bb = blockBytes(f);
        for (int by = 0; by < (img.h + 3) / 4; ++by)
            for (int bx = 0; bx < (img.w + 3) / 4; ++bx) {
                Px blk[16];
                for (int i = 0; i < 16; ++i) {
                    const int x = std::min(bx * 4 + (i & 3), img.w - 1), y = std::min(by * 4 + (i >> 2), img.h - 1);
                    blk[i] = img.at(x, y);
                }
                encodeBlock(f, blk, mem + static_cast<size_t>(by) * pitch + static_cast<size_t>(bx) * bb);
            }
        return;
    }
    const int bpp = bitsPerPixel(f) / 8;
    for (int y = 0; y < img.h; ++y)
        for (int x = 0; x < img.w; ++x) writePixel(f, mem + static_cast<size_t>(y) * pitch + static_cast<size_t>(x) * bpp, img.at(x, y));
}

Image crop(const Image& src, const RECT& r) {
    Image o;
    o.w = r.right - r.left, o.h = r.bottom - r.top;
    o.px.resize(static_cast<size_t>(o.w) * o.h);
    for (int y = 0; y < o.h; ++y)
        for (int x = 0; x < o.w; ++x) o.at(x, y) = src.at(std::clamp<int>(r.left + x, 0, src.w - 1), std::clamp<int>(r.top + y, 0, src.h - 1));
    return o;
}

Image resample(const Image& src, int w, int h, DWORD filter) {
    if (src.w == w && src.h == h) return src;
    Image o;
    o.w = w, o.h = h;
    o.px.resize(static_cast<size_t>(w) * h);
    const DWORD kind = filter == D3DX_DEFAULT ? D3DX_FILTER_TRIANGLE : (filter & 0xFF);
    const float sx = static_cast<float>(src.w) / w, sy = static_cast<float>(src.h) / h;
    if (kind == D3DX_FILTER_NONE) {  // no scaling: crop / pad
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x) o.at(x, y) = (x < src.w && y < src.h) ? src.at(x, y) : Px{0, 0, 0, 0};
        return o;
    }
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) {
            if (kind == D3DX_FILTER_POINT) {
                o.at(x, y) = src.at(std::min(static_cast<int>(x * sx), src.w - 1), std::min(static_cast<int>(y * sy), src.h - 1));
            } else if (sx > 1.0f || sy > 1.0f) {
                // area average over the source footprint (box / triangle when shrinking)
                const int x0 = static_cast<int>(x * sx), x1 = std::max(x0 + 1, static_cast<int>((x + 1) * sx));
                const int y0 = static_cast<int>(y * sy), y1 = std::max(y0 + 1, static_cast<int>((y + 1) * sy));
                Px acc{0, 0, 0, 0};
                int n = 0;
                for (int yy = y0; yy < y1 && yy < src.h; ++yy)
                    for (int xx = x0; xx < x1 && xx < src.w; ++xx) {
                        const Px& p = src.at(xx, yy);
                        acc.r += p.r, acc.g += p.g, acc.b += p.b, acc.a += p.a, ++n;
                    }
                if (n) acc.r /= n, acc.g /= n, acc.b /= n, acc.a /= n;
                o.at(x, y) = acc;
            } else {
                // bilinear when enlarging
                const float fx = std::max(0.0f, (x + 0.5f) * sx - 0.5f), fy = std::max(0.0f, (y + 0.5f) * sy - 0.5f);
                const int ix = std::min(static_cast<int>(fx), src.w - 1), iy = std::min(static_cast<int>(fy), src.h - 1);
                const int jx = std::min(ix + 1, src.w - 1), jy = std::min(iy + 1, src.h - 1);
                const float tx = fx - ix, ty = fy - iy;
                auto lerp = [](const Px& a, const Px& b, float t) { return Px{a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t}; };
                o.at(x, y) = lerp(lerp(src.at(ix, iy), src.at(jx, iy), tx), lerp(src.at(ix, jy), src.at(jx, jy), tx), ty);
            }
        }
    return o;
}

void colourKey(Image& img, D3DCOLOR key) {
    if (!key) return;
    const Px k{u((key >> 16) & 255, 8), u((key >> 8) & 255, 8), u(key & 255, 8), u(key >> 24, 8)};
    for (Px& p : img.px)
        if (std::fabs(p.r - k.r) < 0.5f / 255 && std::fabs(p.g - k.g) < 0.5f / 255 && std::fabs(p.b - k.b) < 0.5f / 255 && std::fabs(p.a - k.a) < 0.5f / 255)
            p = {0, 0, 0, 0};
}

// ============================================================================
// surfaces
// ============================================================================
RECT fullRect(UINT w, UINT h) { return RECT{0, 0, static_cast<LONG>(w), static_cast<LONG>(h)}; }

// Region locks of block-compressed surfaces must be block aligned: lock the enclosing
// aligned rectangle and work on that (re-encoding untouched texels as they were).
RECT alignRect(D3DFORMAT f, const RECT& r, UINT w, UINT h) {
    if (!isDxt(f)) return r;
    RECT a{r.left & ~3, r.top & ~3, std::min<LONG>((r.right + 3) & ~3, static_cast<LONG>(w)), std::min<LONG>((r.bottom + 3) & ~3, static_cast<LONG>(h))};
    return a;
}

bool readSurface(IDirect3DSurface9* s, const RECT* rect, Image& out) {
    D3DSURFACE_DESC d;
    if (FAILED(s->GetDesc(&d)) || !supported(d.Format)) return false;
    const RECT r = rect ? *rect : fullRect(d.Width, d.Height);
    const RECT a = alignRect(d.Format, r, d.Width, d.Height);
    D3DLOCKED_RECT lr;
    IDirect3DSurface9* tmp = nullptr;
    HRESULT hr = s->LockRect(&lr, &a, D3DLOCK_READONLY);
    if (FAILED(hr)) {
        // Render targets and other unlockable surfaces: copy into system memory first.
        IDirect3DDevice9* dev = nullptr;
        s->GetDevice(&dev);
        if (dev && SUCCEEDED(dev->CreateOffscreenPlainSurface(d.Width, d.Height, d.Format, D3DPOOL_SYSTEMMEM, &tmp, nullptr)) &&
            SUCCEEDED(dev->GetRenderTargetData(s, tmp)))
            hr = tmp->LockRect(&lr, &a, D3DLOCK_READONLY);
        if (dev) dev->Release();
        if (FAILED(hr)) { if (tmp) tmp->Release(); return false; }
    }
    Image whole = decodeRegion(d.Format, static_cast<const uint8_t*>(lr.pBits), static_cast<UINT>(lr.Pitch), a.right - a.left, a.bottom - a.top, nullptr);
    (tmp ? tmp : s)->UnlockRect();
    if (tmp) tmp->Release();
    out = (a.left == r.left && a.top == r.top && a.right == r.right && a.bottom == r.bottom)
              ? std::move(whole)
              : crop(whole, RECT{r.left - a.left, r.top - a.top, r.right - a.left, r.bottom - a.top});
    return true;
}

HRESULT writeSurface(IDirect3DSurface9* s, const RECT* rect, const Image& src, DWORD filter) {
    D3DSURFACE_DESC d;
    if (FAILED(s->GetDesc(&d))) return D3DERR_INVALIDCALL;
    if (!supported(d.Format)) {
        std::fprintf(stderr, "d3dx: unsupported destination format %d\n", d.Format);
        return D3DERR_INVALIDCALL;
    }
    const RECT r = rect ? *rect : fullRect(d.Width, d.Height);
    Image img = resample(src, r.right - r.left, r.bottom - r.top, filter);
    const RECT a = alignRect(d.Format, r, d.Width, d.Height);
    D3DLOCKED_RECT lr;
    IDirect3DSurface9* tmp = nullptr;
    IDirect3DSurface9* target = s;
    if (FAILED(s->LockRect(&lr, &a, 0))) {
        // Default-pool surfaces: write a system-memory copy and upload it.
        IDirect3DDevice9* dev = nullptr;
        s->GetDevice(&dev);
        if (!dev || FAILED(dev->CreateOffscreenPlainSurface(d.Width, d.Height, d.Format, D3DPOOL_SYSTEMMEM, &tmp, nullptr))) {
            if (dev) dev->Release();
            return D3DERR_INVALIDCALL;
        }
        dev->Release();
        target = tmp;
        if (FAILED(tmp->LockRect(&lr, &a, 0))) { tmp->Release(); return D3DERR_INVALIDCALL; }
    }
    if (a.left != r.left || a.top != r.top || a.right != r.right || a.bottom != r.bottom) {
        // merge into the aligned block region
        Image whole = decodeRegion(d.Format, static_cast<const uint8_t*>(lr.pBits), static_cast<UINT>(lr.Pitch), a.right - a.left, a.bottom - a.top, nullptr);
        for (int y = 0; y < img.h; ++y)
            for (int x = 0; x < img.w; ++x) whole.at(r.left - a.left + x, r.top - a.top + y) = img.at(x, y);
        img = std::move(whole);
    }
    encodeRegion(d.Format, static_cast<uint8_t*>(lr.pBits), static_cast<UINT>(lr.Pitch), img);
    target->UnlockRect();
    if (tmp) {
        IDirect3DDevice9* dev = nullptr;
        s->GetDevice(&dev);
        POINT p{a.left, a.top};
        const HRESULT hr = dev->UpdateSurface(tmp, &a, s, &p);
        dev->Release();
        tmp->Release();
        return hr;
    }
    return D3D_OK;
}

// ============================================================================
// image files
// ============================================================================
struct FileImage {
    std::vector<Image> mips;  // decoded levels (at least the top one)
    D3DFORMAT format = D3DFMT_A8R8G8B8;
    D3DXIMAGE_FILEFORMAT fileFormat = D3DXIFF_BMP;
    UINT mipCount = 1;
    // raw data of each level for DDS, so same-format textures can be copied without re-encoding
    std::vector<std::vector<uint8_t>> raw;
    std::vector<UINT> rawPitch;
};

std::vector<uint8_t> readAll(LPCWSTR file) {
    std::vector<uint8_t> data;
    FILE* f = std::fopen(toPosixPath(file).c_str(), "rb");
    if (!f) return data;
    std::fseek(f, 0, SEEK_END);
    const long n = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (n > 0) {
        data.resize(static_cast<size_t>(n));
        if (std::fread(data.data(), 1, data.size(), f) != data.size()) data.clear();
    }
    std::fclose(f);
    return data;
}

D3DFORMAT ddsFormat(const uint8_t* pf) {
    auto rd = [&](int o) { uint32_t v; std::memcpy(&v, pf + o, 4); return v; };
    const uint32_t flags = rd(4), fourcc = rd(8), bits = rd(12), rm = rd(16), gm = rd(20), bm = rd(24), am = rd(28);
    if (flags & 4 /* DDPF_FOURCC */) {
        if (fourcc == MAKEFOURCC('D', 'X', 'T', '1')) return D3DFMT_DXT1;
        if (fourcc == MAKEFOURCC('D', 'X', 'T', '2')) return D3DFMT_DXT2;
        if (fourcc == MAKEFOURCC('D', 'X', 'T', '3')) return D3DFMT_DXT3;
        if (fourcc == MAKEFOURCC('D', 'X', 'T', '4')) return D3DFMT_DXT4;
        if (fourcc == MAKEFOURCC('D', 'X', 'T', '5')) return D3DFMT_DXT5;
        return static_cast<D3DFORMAT>(fourcc);  // D3DFMT values stored directly (e.g. 113)
    }
    if (bits == 32) {
        if (rm == 0xFF0000 && am == 0xFF000000u) return D3DFMT_A8R8G8B8;
        if (rm == 0xFF0000) return D3DFMT_X8R8G8B8;
        if (rm == 0xFF && am) return D3DFMT_A8B8G8R8;
        if (rm == 0xFF) return D3DFMT_X8B8G8R8;
        if (rm == 0xFFFF) return D3DFMT_G16R16;
    }
    if (bits == 24) return D3DFMT_R8G8B8;
    if (bits == 16) {
        if (rm == 0xF800) return D3DFMT_R5G6B5;
        if (rm == 0x7C00 && am) return D3DFMT_A1R5G5B5;
        if (rm == 0x7C00) return D3DFMT_X1R5G5B5;
        if (rm == 0xF00 && am) return D3DFMT_A4R4G4B4;
        if (rm == 0xF00) return D3DFMT_X4R4G4B4;
        if (rm == 0xFF && am == 0xFF00) return D3DFMT_A8L8;
        if (rm == 0xFFFF) return D3DFMT_L16;
    }
    if (bits == 8) {
        if (am == 0xFF && !rm) return D3DFMT_A8;
        if (rm == 0xFF) return D3DFMT_L8;
    }
    return D3DFMT_UNKNOWN;
}

bool loadImageFile(const std::vector<uint8_t>& data, FileImage& out) {
    if (data.size() >= 128 && std::memcmp(data.data(), "DDS ", 4) == 0) {
        auto rd = [&](size_t o) { uint32_t v; std::memcpy(&v, data.data() + o, 4); return v; };
        const UINT h = rd(12), w = rd(16), mips = std::max<uint32_t>(1, rd(28));
        const D3DFORMAT f = ddsFormat(data.data() + 76);
        if (!supported(f)) { std::fprintf(stderr, "d3dx: DDS format %d not supported\n", f); return false; }
        out.format = f;
        out.fileFormat = D3DXIFF_DDS;
        out.mipCount = mips;
        size_t off = 128;
        UINT mw = w, mh = h;
        for (UINT m = 0; m < mips; ++m) {
            const UINT pitch = isDxt(f) ? std::max(1u, (mw + 3) / 4) * blockBytes(f) : mw * bitsPerPixel(f) / 8;
            const UINT rows = isDxt(f) ? std::max(1u, (mh + 3) / 4) : mh;
            const size_t size = static_cast<size_t>(pitch) * rows;
            if (off + size > data.size()) break;
            out.raw.emplace_back(data.begin() + off, data.begin() + off + size);
            out.rawPitch.push_back(pitch);
            out.mips.push_back(decodeRegion(f, data.data() + off, pitch, static_cast<int>(mw), static_cast<int>(mh), nullptr));
            off += size;
            mw = std::max(1u, mw / 2), mh = std::max(1u, mh / 2);
        }
        return !out.mips.empty();
    }
    int w, h, n;
    stbi_uc* px = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &w, &h, &n, 4);
    if (!px) return false;
    Image img;
    img.w = w, img.h = h;
    img.px.resize(static_cast<size_t>(w) * h);
    for (size_t i = 0; i < img.px.size(); ++i) img.px[i] = {u(px[4 * i], 8), u(px[4 * i + 1], 8), u(px[4 * i + 2], 8), u(px[4 * i + 3], 8)};
    stbi_image_free(px);
    out.mips.push_back(std::move(img));
    out.format = n == 4 || n == 2 ? D3DFMT_A8R8G8B8 : D3DFMT_X8R8G8B8;
    const bool tga = data.size() > 18 && (data[2] == 2 || data[2] == 10 || data[2] == 3 || data[2] == 11);
    out.fileFormat = data[0] == 'B' && data[1] == 'M' ? D3DXIFF_BMP : data[0] == 0x89 ? D3DXIFF_PNG : data[0] == 0xFF ? D3DXIFF_JPG : tga ? D3DXIFF_TGA : D3DXIFF_BMP;
    return true;
}

void fillInfo(D3DXIMAGE_INFO* info, const FileImage& fi) {
    if (!info) return;
    info->Width = static_cast<UINT>(fi.mips[0].w);
    info->Height = static_cast<UINT>(fi.mips[0].h);
    info->Depth = 1;
    info->MipLevels = fi.mipCount;
    info->Format = fi.format;
    info->ResourceType = D3DRTYPE_TEXTURE;
    info->ImageFileFormat = fi.fileFormat;
}

UINT nextPow2(UINT v) { UINT p = 1; while (p < v) p <<= 1; return p; }

}  // namespace
}  // namespace w32

using namespace w32;

extern "C" {

HRESULT WINAPI D3DXLoadSurfaceFromMemory(LPDIRECT3DSURFACE9 dst, const PALETTEENTRY*, const RECT* dstRect, LPCVOID src, D3DFORMAT srcFmt, UINT srcPitch,
                                         const PALETTEENTRY* srcPal, const RECT* srcRect, DWORD filter, D3DCOLOR key) {
    if (!dst || !src || !srcRect || !supported(srcFmt)) return D3DERR_INVALIDCALL;
    // srcRect is relative to `src`, the start of the source surface
    const RECT a = srcRect ? *srcRect : RECT{};
    const RECT al = isDxt(srcFmt) ? RECT{a.left & ~3, a.top & ~3, (a.right + 3) & ~3, (a.bottom + 3) & ~3} : a;
    const auto* base = static_cast<const uint8_t*>(src) + static_cast<size_t>(isDxt(srcFmt) ? al.top / 4 : al.top) * srcPitch +
                       static_cast<size_t>(isDxt(srcFmt) ? (al.left / 4) * blockBytes(srcFmt) : al.left * bitsPerPixel(srcFmt) / 8);
    Image img = decodeRegion(srcFmt, base, srcPitch, al.right - al.left, al.bottom - al.top, srcPal);
    if (al.left != a.left || al.top != a.top || al.right != a.right || al.bottom != a.bottom)
        img = crop(img, RECT{a.left - al.left, a.top - al.top, a.right - al.left, a.bottom - al.top});
    colourKey(img, key);
    return writeSurface(dst, dstRect, img, filter);
}

HRESULT WINAPI D3DXLoadSurfaceFromSurface(LPDIRECT3DSURFACE9 dst, const PALETTEENTRY*, const RECT* dstRect, LPDIRECT3DSURFACE9 src, const PALETTEENTRY*,
                                          const RECT* srcRect, DWORD filter, D3DCOLOR key) {
    Image img;
    if (!dst || !src || !readSurface(src, srcRect, img)) return D3DERR_INVALIDCALL;
    colourKey(img, key);
    return writeSurface(dst, dstRect, img, filter);
}

HRESULT WINAPI D3DXLoadSurfaceFromFileW(LPDIRECT3DSURFACE9 dst, const PALETTEENTRY*, const RECT* dstRect, LPCWSTR file, const RECT* srcRect, DWORD filter,
                                        D3DCOLOR key, D3DXIMAGE_INFO* info) {
    const std::vector<uint8_t> data = readAll(file);
    FileImage fi;
    if (data.empty() || !loadImageFile(data, fi)) return D3DXERR_INVALIDDATA;
    fillInfo(info, fi);
    Image img = srcRect ? crop(fi.mips[0], *srcRect) : fi.mips[0];
    colourKey(img, key);
    return writeSurface(dst, dstRect, img, filter);
}

HRESULT WINAPI D3DXFilterTexture(LPDIRECT3DBASETEXTURE9 base, const PALETTEENTRY*, UINT srcLevel, DWORD filter) {
    IDirect3DTexture9* tex = nullptr;
    if (!base || FAILED(base->QueryInterface(IID_IDirect3DTexture9, reinterpret_cast<void**>(&tex)))) return D3DERR_INVALIDCALL;
    if (srcLevel == D3DX_DEFAULT) srcLevel = 0;
    const DWORD levels = tex->GetLevelCount();
    HRESULT hr = D3D_OK;
    IDirect3DSurface9* s = nullptr;
    Image cur;
    if (SUCCEEDED(tex->GetSurfaceLevel(srcLevel, &s))) {
        if (!readSurface(s, nullptr, cur)) hr = D3DERR_INVALIDCALL;
        s->Release();
    }
    for (DWORD l = srcLevel + 1; SUCCEEDED(hr) && l < levels; ++l) {
        if (FAILED(tex->GetSurfaceLevel(l, &s))) break;
        D3DSURFACE_DESC d;
        s->GetDesc(&d);
        cur = resample(cur, static_cast<int>(d.Width), static_cast<int>(d.Height), filter == D3DX_DEFAULT ? D3DX_FILTER_BOX : filter);
        hr = writeSurface(s, nullptr, cur, D3DX_FILTER_NONE);
        s->Release();
    }
    tex->Release();
    return hr;
}

HRESULT WINAPI D3DXCreateTextureFromFileExW(LPDIRECT3DDEVICE9 dev, LPCWSTR file, UINT w, UINT h, UINT mips, DWORD usage, D3DFORMAT fmt, D3DPOOL pool,
                                            DWORD filter, DWORD mipFilter, D3DCOLOR key, D3DXIMAGE_INFO* info, PALETTEENTRY*, LPDIRECT3DTEXTURE9* out) {
    if (out) *out = nullptr;
    const std::vector<uint8_t> data = readAll(file);
    FileImage fi;
    if (!dev || !out || data.empty() || !loadImageFile(data, fi)) return D3DXERR_INVALIDDATA;
    fillInfo(info, fi);
    const UINT fw = static_cast<UINT>(fi.mips[0].w), fh = static_cast<UINT>(fi.mips[0].h);
    if (w == 0 || w == D3DX_DEFAULT) w = nextPow2(fw);
    else if (w == D3DX_DEFAULT_NONPOW2 || w == D3DX_FROM_FILE) w = fw;
    if (h == 0 || h == D3DX_DEFAULT) h = nextPow2(fh);
    else if (h == D3DX_DEFAULT_NONPOW2 || h == D3DX_FROM_FILE) h = fh;
    if (fmt == D3DFMT_UNKNOWN || fmt == static_cast<D3DFORMAT>(D3DX_DEFAULT) || fmt == static_cast<D3DFORMAT>(D3DX_FROM_FILE)) fmt = fi.format;
    if (isDxt(fmt)) w = (w + 3) & ~3u, h = (h + 3) & ~3u;
    if (mips == D3DX_FROM_FILE) mips = fi.mipCount;
    else if (mips == D3DX_DEFAULT) mips = 0;
    // Default-pool textures are filled through a system-memory copy.
    const bool staged = pool == D3DPOOL_DEFAULT && !(usage & D3DUSAGE_DYNAMIC);
    IDirect3DTexture9* tex = nullptr;
    HRESULT hr = dev->CreateTexture(w, h, mips, usage, fmt, pool, &tex, nullptr);
    if (FAILED(hr)) return hr;
    IDirect3DTexture9* fillTex = tex;
    if (staged && FAILED(dev->CreateTexture(w, h, tex->GetLevelCount(), 0, fmt, D3DPOOL_SYSTEMMEM, &fillTex, nullptr))) {
        tex->Release();
        return D3DERR_OUTOFVIDEOMEMORY;
    }
    Image cur;
    for (DWORD l = 0; l < fillTex->GetLevelCount(); ++l) {
        D3DSURFACE_DESC d;
        fillTex->GetLevelDesc(l, &d);
        IDirect3DSurface9* s = nullptr;
        fillTex->GetSurfaceLevel(l, &s);
        const bool raw = l < fi.raw.size() && fi.format == fmt && static_cast<UINT>(fi.mips[l].w) == d.Width && static_cast<UINT>(fi.mips[l].h) == d.Height && !key;
        if (raw) {
            D3DLOCKED_RECT lr;
            if (SUCCEEDED(s->LockRect(&lr, nullptr, 0))) {
                const UINT rows = isDxt(fmt) ? std::max(1u, (d.Height + 3) / 4) : d.Height;
                for (UINT y = 0; y < rows; ++y)
                    std::memcpy(static_cast<uint8_t*>(lr.pBits) + static_cast<size_t>(y) * lr.Pitch, fi.raw[l].data() + static_cast<size_t>(y) * fi.rawPitch[l],
                                std::min<UINT>(fi.rawPitch[l], static_cast<UINT>(lr.Pitch)));
                s->UnlockRect();
            }
            cur = fi.mips[l];
        } else {
            if (l == 0) {
                cur = fi.mips[0];
                colourKey(cur, key);
            } else if (l < fi.mips.size() && !key) {
                cur = fi.mips[l];
            }
            cur = resample(cur, static_cast<int>(d.Width), static_cast<int>(d.Height), l == 0 ? filter : (mipFilter == D3DX_DEFAULT ? D3DX_FILTER_BOX : mipFilter));
            hr = writeSurface(s, nullptr, cur, D3DX_FILTER_NONE);
        }
        s->Release();
        if (FAILED(hr)) break;
    }
    if (fillTex != tex) {
        if (SUCCEEDED(hr)) hr = dev->UpdateTexture(fillTex, tex);
        fillTex->Release();
    }
    if (FAILED(hr)) { tex->Release(); return hr; }
    *out = tex;
    return D3D_OK;
}

HRESULT WINAPI D3DXCreateVolumeTexture(LPDIRECT3DDEVICE9 dev, UINT w, UINT h, UINT d, UINT mips, DWORD usage, D3DFORMAT fmt, D3DPOOL pool,
                                       LPDIRECT3DVOLUMETEXTURE9* out) {
    if (mips == D3DX_DEFAULT) mips = 0;
    return dev->CreateVolumeTexture(w, h, d, mips, usage, fmt, pool, out, nullptr);
}

HRESULT WINAPI D3DXLoadVolumeFromMemory(LPDIRECT3DVOLUME9 dst, const PALETTEENTRY*, const D3DBOX* dstBox, LPCVOID src, D3DFORMAT srcFmt, UINT rowPitch,
                                        UINT slicePitch, const PALETTEENTRY* srcPal, const D3DBOX* srcBox, DWORD filter, D3DCOLOR key) {
    D3DVOLUME_DESC vd;
    if (!dst || !src || !srcBox || FAILED(dst->GetDesc(&vd)) || !supported(srcFmt) || !supported(vd.Format) || isDxt(vd.Format)) return D3DERR_INVALIDCALL;
    const D3DBOX db = dstBox ? *dstBox : D3DBOX{0, 0, vd.Width, vd.Height, 0, vd.Depth};
    D3DLOCKED_BOX lb;
    if (FAILED(dst->LockBox(&lb, &db, 0))) return D3DERR_INVALIDCALL;
    const UINT srcDepth = srcBox->Back - srcBox->Front, dstDepth = db.Back - db.Front;
    const int bpp = bitsPerPixel(srcFmt) / 8;
    for (UINT z = 0; z < dstDepth; ++z) {
        const UINT sz = srcBox->Front + std::min(srcDepth - 1, z * srcDepth / std::max(1u, dstDepth));
        const auto* slice = static_cast<const uint8_t*>(src) + static_cast<size_t>(sz) * slicePitch + static_cast<size_t>(srcBox->Top) * rowPitch +
                            static_cast<size_t>(srcBox->Left) * bpp;
        Image img = decodeRegion(srcFmt, slice, rowPitch, static_cast<int>(srcBox->Right - srcBox->Left), static_cast<int>(srcBox->Bottom - srcBox->Top), srcPal);
        colourKey(img, key);
        img = resample(img, static_cast<int>(db.Right - db.Left), static_cast<int>(db.Bottom - db.Top), filter);
        encodeRegion(vd.Format, static_cast<uint8_t*>(lb.pBits) + static_cast<size_t>(z) * lb.SlicePitch, static_cast<UINT>(lb.RowPitch), img);
    }
    dst->UnlockBox();
    return D3D_OK;
}

HRESULT WINAPI D3DXLoadVolumeFromVolume(LPDIRECT3DVOLUME9 dst, const PALETTEENTRY* dstPal, const D3DBOX* dstBox, LPDIRECT3DVOLUME9 src, const PALETTEENTRY* srcPal,
                                        const D3DBOX* srcBox, DWORD filter, D3DCOLOR key) {
    D3DVOLUME_DESC sd;
    if (!src || FAILED(src->GetDesc(&sd))) return D3DERR_INVALIDCALL;
    const D3DBOX sb = srcBox ? *srcBox : D3DBOX{0, 0, sd.Width, sd.Height, 0, sd.Depth};
    D3DLOCKED_BOX lb;
    if (FAILED(src->LockBox(&lb, nullptr, D3DLOCK_READONLY))) return D3DERR_INVALIDCALL;
    const HRESULT hr = D3DXLoadVolumeFromMemory(dst, dstPal, dstBox, lb.pBits, sd.Format, static_cast<UINT>(lb.RowPitch), static_cast<UINT>(lb.SlicePitch), srcPal,
                                                &sb, filter, key);
    src->UnlockBox();
    return hr;
}

HRESULT WINAPI D3DXSaveSurfaceToFileW(LPCWSTR file, D3DXIMAGE_FILEFORMAT format, LPDIRECT3DSURFACE9 surf, const PALETTEENTRY*, const RECT* rect) {
    Image img;
    if (!surf || !readSurface(surf, rect, img)) return D3DERR_INVALIDCALL;
    std::vector<uint8_t> rgba(static_cast<size_t>(img.w) * img.h * 4);
    for (size_t i = 0; i < img.px.size(); ++i) {
        rgba[4 * i] = static_cast<uint8_t>(q(img.px[i].r, 8));
        rgba[4 * i + 1] = static_cast<uint8_t>(q(img.px[i].g, 8));
        rgba[4 * i + 2] = static_cast<uint8_t>(q(img.px[i].b, 8));
        rgba[4 * i + 3] = 255;
    }
    const std::string path = toPosixPath(file, true);
    int ok;
    switch (format) {
        case D3DXIFF_PNG: ok = stbi_write_png(path.c_str(), img.w, img.h, 4, rgba.data(), img.w * 4); break;
        case D3DXIFF_JPG: ok = stbi_write_jpg(path.c_str(), img.w, img.h, 4, rgba.data(), 90); break;
        case D3DXIFF_TGA: ok = stbi_write_tga(path.c_str(), img.w, img.h, 4, rgba.data()); break;
        default: ok = stbi_write_bmp(path.c_str(), img.w, img.h, 4, rgba.data()); break;
    }
    return ok ? D3D_OK : D3DERR_INVALIDCALL;
}

}  // extern "C"

namespace w32 {
namespace {
struct Registrar {
    Registrar() {
        const char* dll = "d3dx9_43.dll";
        registerHostModule(dll, "D3DXLoadSurfaceFromMemory", reinterpret_cast<void*>(&::D3DXLoadSurfaceFromMemory));
        registerHostModule(dll, "D3DXLoadSurfaceFromSurface", reinterpret_cast<void*>(&::D3DXLoadSurfaceFromSurface));
        registerHostModule(dll, "D3DXLoadSurfaceFromFileW", reinterpret_cast<void*>(&::D3DXLoadSurfaceFromFileW));
        registerHostModule(dll, "D3DXFilterTexture", reinterpret_cast<void*>(&::D3DXFilterTexture));
        registerHostModule(dll, "D3DXCreateTextureFromFileExW", reinterpret_cast<void*>(&::D3DXCreateTextureFromFileExW));
        registerHostModule(dll, "D3DXCreateVolumeTexture", reinterpret_cast<void*>(&::D3DXCreateVolumeTexture));
        registerHostModule(dll, "D3DXLoadVolumeFromMemory", reinterpret_cast<void*>(&::D3DXLoadVolumeFromMemory));
        registerHostModule(dll, "D3DXLoadVolumeFromVolume", reinterpret_cast<void*>(&::D3DXLoadVolumeFromVolume));
        registerHostModule(dll, "D3DXSaveSurfaceToFileW", reinterpret_cast<void*>(&::D3DXSaveSurfaceToFileW));
    }
} g_registrar;
}  // namespace
}  // namespace w32
