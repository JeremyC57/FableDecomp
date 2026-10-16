#include <cstdio>
static int g_swizzled = 0, g_live = 0;
struct CSurface { int level; CSurface() : level(-1) { ++g_live; } CSurface(const CSurface& o) : level(o.level) { ++g_live; } ~CSurface() { --g_live; } void Swizzle(unsigned int f) { if (f == 7) g_swizzled |= 1 << level; } };
struct CTexture {
    long levels;
    CSurface GetSurfaceLevel(unsigned long i) const { CSurface s; s.level = (int)i; return s; }
    void UnswizzleAll(unsigned int f) { long n = levels; for (long i = 0; i < n; ++i) { CSurface s = GetSurfaceLevel(i); s.Swizzle(f); } }
};
int main()
{
    CTexture t = { 4 };
    t.UnswizzleAll(7);
    if (g_swizzled != 0xf || g_live != 0) { std::printf("009f9fa0_TEST FAIL\n"); return 1; }
    std::printf("009f9fa0_TEST PASS\n");
    return 0;
}