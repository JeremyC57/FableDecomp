// Behaviour test for CSurface::SetPixel (0x009F4120): pixel address = PMem + bytesPerPixel * x + Pitch * y, unlock afterwards.
#include <cstdio>
static unsigned char g_mem[4096];
static int g_unlocks; static unsigned char* g_dest; static unsigned int g_colour;
struct Locked { long X, Y; long Pitch; unsigned char* PMem; };
struct Format {
    long index;
    unsigned int GetByteColourDepth() const { return index == 1 ? 4 : 2; }
    void WritePixel(const unsigned int& colour, void* dest) const { g_colour = colour; g_dest = (unsigned char*)dest; }
};
struct Surface {
    Locked Lock(unsigned int) { Locked l = { 16, 16, 64, g_mem }; return l; }
    Format GetPixelFormat() const { Format f = { 1 }; return f; }
    void Unlock() { ++g_unlocks; }
    void SetPixel(long x, long y, const unsigned int& colour)
    {
        Locked locked = Lock(0);
        Format format = GetPixelFormat();
        unsigned char* dest = locked.PMem + format.GetByteColourDepth() * x + locked.Pitch * y;
        format.WritePixel(colour, dest);
        Unlock();
    }
};
int main()
{
    Surface s; unsigned int c = 0xff102030u;
    s.SetPixel(3, 5, c);
    if (g_dest != g_mem + 3 * 4 + 5 * 64 || g_colour != c || g_unlocks != 1) { std::printf("009f4120_TEST FAIL\n"); return 1; }
    std::printf("009f4120_TEST PASS\n");
    return 0;
}