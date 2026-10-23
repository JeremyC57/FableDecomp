#include <cstdio>
// Behaviour model of CVolume::ClearArea: clamp the box per axis to [0, extent], then fill every texel of the clamped
// box through one running pointer (row pitch after each row, slice pitch on top after each slice), as retail does.
struct Box { int Left, Top, Right, Bottom, Front, Back; };
static unsigned char mem[4096];
static int writes = 0;
static int Clamp(int v, unsigned int extent)
{
    if (v < 0) return 0;
    if ((unsigned int)v > extent) return (int)extent;
    return v;
}
static void ClearArea(const Box& area, unsigned char value, unsigned int w, unsigned int h, unsigned int d,
                      int bpp, int rowPitch, int slicePitch)
{
    Box b;
    b.Left = Clamp(area.Left, w); b.Top = Clamp(area.Top, h); b.Front = Clamp(area.Front, d);
    b.Right = Clamp(area.Right, w); b.Bottom = Clamp(area.Bottom, h); b.Back = Clamp(area.Back, d);
    unsigned char* dst = mem + bpp * b.Left + slicePitch * b.Front + rowPitch * b.Top;
    for (int z = b.Front; z < b.Back; ++z) {
        for (int y = b.Top; y < b.Bottom; ++y) {
            unsigned char* t = dst;
            for (int x = b.Left; x < b.Right; ++x) { for (int k = 0; k < bpp; ++k) t[k] = value; ++writes; t += bpp; }
            dst += rowPitch;
        }
        dst += slicePitch;
    }
}
int main()
{
    Box a = { -3, 1, 9, 3, 0, 1 };
    ClearArea(a, 0xab, 4, 4, 2, 2, 8, 32);
    if (writes != 8 || mem[8] != 0xab || mem[15] != 0xab || mem[16] != 0xab || mem[7] != 0 || mem[24] != 0)
    { std::printf("00a67a20_TEST FAIL\n"); return 1; }
    Box e = { 2, 2, 1, 3, 0, 2 };
    ClearArea(e, 0xcd, 4, 4, 2, 2, 8, 32);
    if (writes != 8) { std::printf("00a67a20_TEST FAIL\n"); return 1; }
    Box f = { 0, 0, 1, 1, -1, 5 };
    ClearArea(f, 0x11, 4, 4, 2, 1, 8, 32);
    if (writes != 10 || mem[0] != 0x11 || mem[40] != 0x11) { std::printf("00a67a20_TEST FAIL\n"); return 1; }
    std::printf("00a67a20_TEST PASS\n");
    return 0;
}
