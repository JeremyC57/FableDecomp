#include <cstdio>
// Behaviour test for 00be5d20: RegisterForZSorting hands the renderer the patch's ZSorterHandle (by reference) and
// the patch; the renderer fills the handle.
struct Patch;
struct Renderer { long next; Patch* last;
    void RegisterSubPatchForZSorting(long& handle, Patch* patch) { if (handle < 0) handle = next++; last = patch; } };
static Renderer g_renderer;
struct Patch { long ZSorterHandle; void RegisterForZSorting() { g_renderer.RegisterSubPatchForZSorting(ZSorterHandle, this); } };
int main()
{
    g_renderer.next = 3; g_renderer.last = 0;
    Patch p; p.ZSorterHandle = -1;
    p.RegisterForZSorting();
    int ok = (p.ZSorterHandle == 3 && g_renderer.last == &p);
    p.RegisterForZSorting();
    ok = ok && p.ZSorterHandle == 3 && g_renderer.next == 4;
    if (!ok) { std::printf("00be5d20_TEST FAIL\n"); return 1; }
    std::printf("00be5d20_TEST PASS\n");
    return 0;
}
