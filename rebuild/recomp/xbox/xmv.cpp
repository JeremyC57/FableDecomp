// XMV movie player: native colour conversion.
//
// The XMV decoder's YUV 4:2:0 -> X8R8G8B8 macroblock converter (0x644A5A, stdcall: Y, U, V,
// Y stride, UV stride, dst, dst stride; ret 0x1C) is about half of all CPU time during movies
// when run as lifted MMX/SSE. This does the same arithmetic natively, in the same order and
// precision, so the output is bit-identical:
//   Yk = float((y + mm7.w) * (mm5.w0 + mm5.w1))      pmaddwd of (y, y) with mm5, cvtpi2ps
//   u' = float(u + mm6.d), v' = float(v + mm6.d)       2 chroma samples per 4 pixels, 2 rows
//   B = sat(rint(u' * xmm4 + Yk) >> 15)
//   R = sat(rint(v' * xmm7 + Yk) >> 15)
//   G = sat(rint((Yk + v' * xmm5) + u' * xmm6) >> 15)  pixel = B | G << 8 | R << 16
// The caller (0x644C45) loads the constants into mm4..mm7 / xmm4..xmm7 once per frame and does
// not read mm0..mm3 / xmm0..xmm3 afterwards (it ends with emms).
// FABLE_DISABLE=xmv runs the lifted converter; FABLE_XMV_CHECK=1 runs both and logs mismatches.
#include "xhost.hpp"

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>

using namespace xb;

#pragma clang fp contract(off)

namespace {

uint32_t argw(Ctx* c, int i) { return rd32(c->esp + 4 + 4 * i); }

// cvtps2pi (round to nearest even, out-of-range -> 0x80000000), psrad 15, packssdw, packuswb.
inline uint32_t component(float f) {
    int32_t i = (f >= -2147483648.0f && f < 2147483648.0f) ? static_cast<int32_t>(std::nearbyintf(f)) : INT32_MIN;
    i >>= 15;
    if (i < -32768) i = -32768;
    if (i > 32767) i = 32767;
    return i < 0 ? 0u : (i > 255 ? 255u : static_cast<uint32_t>(i));
}

void convert(Ctx* c, uint32_t y, uint32_t u, uint32_t v, uint32_t ys, uint32_t uvs, uint32_t dst, uint32_t ds) {
    const uint64_t m5 = c->mm[5], m6 = c->mm[6], m7 = c->mm[7];
    const int32_t yk = static_cast<int16_t>(m5 & 0xFFFF) + static_cast<int16_t>((m5 >> 16) & 0xFFFF);
    const int16_t yOff = static_cast<int16_t>(m7 & 0xFFFF);
    const int32_t cOff = static_cast<int32_t>(m6 & 0xFFFFFFFF);
    const float kUB = c->xmm[4].f[0], kVG = c->xmm[5].f[0], kUG = c->xmm[6].f[0], kVR = c->xmm[7].f[0];
    for (int row = 0; row < 16; ++row) {
        const uint8_t* yp = gp(y + static_cast<uint32_t>(row) * ys);
        const uint8_t* up = gp(u + static_cast<uint32_t>(row / 2) * uvs);
        const uint8_t* vp = gp(v + static_cast<uint32_t>(row / 2) * uvs);
        uint8_t* out = gp(dst + static_cast<uint32_t>(row) * ds);
        for (int x = 0; x < 16; ++x) {
            // paddw (16-bit wrap), then pmaddwd: y*w0 + y*w1 in 32 bits.
            const int16_t yw = static_cast<int16_t>(static_cast<uint16_t>(yp[x] + static_cast<uint16_t>(yOff)));
            const float Y = static_cast<float>(static_cast<int32_t>(yw) * yk);
            const float U = static_cast<float>(static_cast<int32_t>(up[x / 2]) + cOff);
            const float V = static_cast<float>(static_cast<int32_t>(vp[x / 2]) + cOff);
            const float b = U * kUB, bsum = b + Y;
            const float r = V * kVR, rsum = r + Y;
            const float g1 = V * kVG, gsum1 = Y + g1;
            const float g2 = U * kUG, gsum = gsum1 + g2;
            const uint32_t px = component(bsum) | component(gsum) << 8 | component(rsum) << 16;
            std::memcpy(out + 4 * x, &px, 4);
        }
    }
}

}  // namespace

extern "C" {

void F_00644A5A_orig(Ctx* c);

void hle_XmvYuvToRgb(Ctx* c) {
    static const bool lifted = featureOff("xmv");
    static const bool check = getenv("FABLE_XMV_CHECK") != nullptr;
    if (lifted) {
        F_00644A5A_orig(c);
        return;
    }
    const uint32_t y = argw(c, 0), u = argw(c, 1), v = argw(c, 2), ys = argw(c, 3), uvs = argw(c, 4), dst = argw(c, 5), ds = argw(c, 6);
    if (check) {
        static uint8_t ref[16][64];
        const uint32_t esp = c->esp;
        F_00644A5A_orig(c);  // reference result in dst
        for (int row = 0; row < 16; ++row) std::memcpy(ref[row], gp(dst + static_cast<uint32_t>(row) * ds), 64);
        c->esp = esp;
        convert(c, y, u, v, ys, uvs, dst, ds);
        static uint64_t blocks = 0, bad = 0;
        ++blocks;
        for (int row = 0; row < 16; ++row)
            if (std::memcmp(ref[row], gp(dst + static_cast<uint32_t>(row) * ds), 64) != 0) {
                if (bad++ < 20) XLOG(0, "XMV convert mismatch: block %llu row %d", static_cast<unsigned long long>(blocks), row);
                break;
            }
        if (blocks % 20000 == 0) XLOG(0, "XMV convert check: %llu blocks, %llu mismatched", static_cast<unsigned long long>(blocks), static_cast<unsigned long long>(bad));
    } else {
        convert(c, y, u, v, ys, uvs, dst, ds);
    }
    c->eax = 0;
    c->esp += 4 + 0x1C;
}

}  // extern "C"
