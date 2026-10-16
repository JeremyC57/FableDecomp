#include <cstdio>
// Behaviour model of CVolume::CopyFromVolume: C3DBoxI -> D3DBOX conversion (null box = whole volume) handed to the loader.
struct Box3 { long tlx, tly, brx, bry, tlz, brz; };
struct DBox { unsigned int Left, Top, Right, Bottom, Front, Back; };
static const DBox* g_dest; static const DBox* g_src; static DBox g_lastSrc; static unsigned int g_filter;
static long Load(const DBox* d, const DBox* s, unsigned int f) { g_dest = d; g_src = s; if (s) g_lastSrc = *s; g_filter = f; return 0; }
static void ToD3DBox(DBox& o, const Box3& b) { o.Top = b.tly; o.Bottom = b.bry; o.Left = b.tlx; o.Right = b.brx; o.Front = b.tlz; o.Back = b.brz; }
static long CopyFromVolume(const Box3* destBox, const Box3* sourceBox, unsigned int filter)
{
    DBox dest, src; const DBox* pd = 0; const DBox* ps = 0;
    if (destBox) { ToD3DBox(dest, *destBox); pd = &dest; }
    if (sourceBox) { ToD3DBox(src, *sourceBox); ps = &src; }
    return Load(pd, ps, filter);
}
int main()
{
    Box3 b = { 1, 2, 3, 4, 5, 6 };
    CopyFromVolume(0, &b, 7);
    if (g_dest || !g_src || g_lastSrc.Left != 1 || g_lastSrc.Top != 2 || g_lastSrc.Right != 3 || g_lastSrc.Bottom != 4 || g_lastSrc.Front != 5 || g_lastSrc.Back != 6 || g_filter != 7) {
        std::printf("00a67670_TEST FAIL\n"); return 1;
    }
    std::printf("00a67670_TEST PASS\n");
    return 0;
}