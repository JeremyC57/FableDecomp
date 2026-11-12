#include <cstdio>
#include <cstring>
// Behaviour model of 0x009F3B00 CSurface::CopyFromMemory (4-arg) over a fake 4x3 32bpp surface.
struct Box { long TLX, TLY, BRX, BRY; };
struct Surf { long W, H, Pitch; unsigned char mem[64]; long blocks; };
static void CopyFromMemory(Surf& s, const Box* destRect, const void* source, const Box* sourceRect, long sourcePitch)
{
    Box dst, src;
    if (destRect) dst = *destRect;
    else if (sourceRect) { dst.TLX = 0; dst.TLY = 0; dst.BRX = sourceRect->BRX - sourceRect->TLX; dst.BRY = sourceRect->BRY - sourceRect->TLY; }
    else { dst.TLX = 0; dst.TLY = 0; dst.BRX = s.W; dst.BRY = s.H; }
    if (sourceRect) src = *sourceRect;
    else { src.TLX = 0; src.TLY = 0; src.BRX = dst.BRX - dst.TLX; src.BRY = dst.BRY - dst.TLY; }
    if (sourcePitch == 0) sourcePitch = s.Pitch;
    unsigned long len = (unsigned long)(s.Pitch * s.H);
    if (sourcePitch == s.Pitch && src.TLX == 0 && src.TLY == 0 && src.BRX == s.W && src.BRY == s.H
        && dst.TLX == 0 && dst.TLY == 0 && dst.BRX == s.W && dst.BRY == s.H) {
        std::memcpy(s.mem, source, len); ++s.blocks;
    } else {
        long bpp = (long)((len * 8) / (unsigned long)(s.W * s.H));
        long rowBytes = ((src.BRX - src.TLX) * bpp) / 8;
        const unsigned char* from = (const unsigned char*)source + (bpp * src.TLX) / 8;
        unsigned char* to = s.mem + (bpp * dst.TLX) / 8;
        for (long y = src.TLY; y < src.BRY; ++y)
            std::memcpy(to + y * s.Pitch, from + y * sourcePitch, rowBytes);
    }
}
int main()
{
    unsigned char data[64];
    for (int i = 0; i < 64; ++i) data[i] = (unsigned char)(i + 1);
    Surf s; s.W = 4; s.H = 3; s.Pitch = 16; s.blocks = 0; std::memset(s.mem, 0, sizeof s.mem);
    CopyFromMemory(s, 0, data, 0, 0);
    if (s.blocks != 1 || std::memcmp(s.mem, data, 48) != 0) { std::printf("009f3b00_TEST FAIL\n"); return 1; }
    std::memset(s.mem, 0, sizeof s.mem);
    Box sr = { 1, 1, 3, 3 };   // 2x2 box, rows 1..2, from x=1
    Box dr = { 2, 0, 4, 2 };   // dst x=2 (TLY ignored: rows offset by src TLY)
    CopyFromMemory(s, &dr, data, &sr, 16);
    if (s.blocks != 1) { std::printf("009f3b00_TEST FAIL\n"); return 1; }
    for (int y = 0; y < 3; ++y) for (int b = 0; b < 16; ++b) {
        unsigned char want = 0;
        if (y >= 1 && b >= 8) want = data[y * 16 + 4 + (b - 8)];
        if (s.mem[y * 16 + b] != want) { std::printf("009f3b00_TEST FAIL\n"); return 1; }
    }
    std::printf("009f3b00_TEST PASS\n");
    return 0;
}