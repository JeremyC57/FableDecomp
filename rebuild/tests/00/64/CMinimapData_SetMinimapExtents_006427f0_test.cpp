// 006427f0 behaviour test for CMinimapData::SetMinimapExtents(const C2DExtentsI&)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class C2DExtentsI { public: long Width; long Height; };
class CMinimapData {
public:
    C2DExtentsI MinimapExtents;
    long After;
    void SetMinimapExtents(const C2DExtentsI& extents);
};
void CMinimapData::SetMinimapExtents(const C2DExtentsI& extents)
{
    MinimapExtents = extents;
}
static void run()
{
    CMinimapData d; d.MinimapExtents.Width = 0; d.MinimapExtents.Height = 0; d.After = 77;
    C2DExtentsI e = { 256, 128 };
    d.SetMinimapExtents(e);
    CHECK(d.MinimapExtents.Width == 256 && d.MinimapExtents.Height == 128);
    CHECK(d.After == 77);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("006427f0_TEST FAIL\n");
        return 1;
    }
    std::printf("006427f0_TEST PASS\n");
    return 0;
}