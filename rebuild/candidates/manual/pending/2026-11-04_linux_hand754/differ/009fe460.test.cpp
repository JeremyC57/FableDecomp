// Behaviour test for CGraphicInfoFrame::CFrameData::FreeTextureMemory (0x009FE460): a model of the row.
#include <stdio.h>

struct Frame { long unused; int mip, minMip; bool reset; long size; };
static bool Free(Frame& f, bool force, long threshold)
{
    if (f.unused > threshold && f.mip < f.minMip) { f.mip++; return true; }
    if (force) { f.reset = true; return true; }
    f.size = 100 >> f.mip; return false;
}
static int run()
{
    Frame f = { 10, 0, 2, false, 0 };
    int ok = Free(f, false, 5) && f.mip == 1;
    Frame g = { 1, 0, 2, false, 0 }; ok = ok && !Free(g, false, 5) && g.size == 100 && Free(g, true, 5) && g.reset;
    return ok;
}
int main()
{
    int ok = run();
    printf(ok ? "009fe460_TEST PASS\n" : "009fe460_TEST FAIL\n");
    return ok ? 0 : 1;
}
