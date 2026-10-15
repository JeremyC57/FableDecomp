// Behaviour test for CSurfaceCompositor::CSurfaceCompositor (0x00F38990): destination stored, capture block cleared, a new
// render target with two empty surfaces handed to the counted pointer, blend and layer count zeroed.
#include <cstdio>
struct Surface { void* PD3DSurface; unsigned int AllocationSource; Surface() : PD3DSurface(0), AllocationSource(0) {} };
struct RenderTarget { Surface Colour, Depth; };
static int g_resets; static RenderTarget* g_rt;
struct CountedPtr { RenderTarget* Data; void* Info; CountedPtr() : Data(0), Info(0) {} void Reset(RenderTarget* p) { ++g_resets; Data = p; g_rt = p; } };
struct CaptureBlock { bool Captured; CaptureBlock() : Captured(false) {} };
struct Compositor {
    long CurrentBlend; CaptureBlock StateCaptureBlock; Surface* DestSurface; CountedPtr RenderTargetPtr; long LayerCount;
    Compositor(Surface* dest) : DestSurface(dest)
    {
        RenderTargetPtr.Reset(new RenderTarget);
        CurrentBlend = 0;
        LayerCount = 0;
    }
};
int main()
{
    Surface dest;
    Compositor c(&dest);
    if (c.DestSurface != &dest || c.StateCaptureBlock.Captured || g_resets != 1 || !g_rt || c.RenderTargetPtr.Data != g_rt ||
        g_rt->Colour.PD3DSurface || g_rt->Depth.AllocationSource || c.CurrentBlend != 0 || c.LayerCount != 0) { std::printf("00f38990_TEST FAIL\n"); return 1; }
    delete g_rt;
    std::printf("00f38990_TEST PASS\n");
    return 0;
}
