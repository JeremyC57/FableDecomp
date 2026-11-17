// Behaviour test for CTexture::GenerateMipmapsWithGPU (0x009FAF70): inside a scoped render-state capture block, remember the display's
// render target (its colour + depth surfaces) and viewport; for every mip level mm = 1 .. GetNoLevels()-1 detach the scratch target's
// colour surface, attach mip level mm's surface (GetSurfaceLevel: the D3D level wrapped in a CSurface), make it the display's render
// target and blit the texture with the GPU (full target, half scale, filtered); then restore the remembered target and viewport.
#include <cstdio>
#include <string>

static std::string log_;
static void L(const char* s) { log_ += s; log_ += ' '; }
static void Ln(const char* s, long n) { char b[32]; std::sprintf(b, "%s%ld ", s, n); log_ += b; }

struct Surface { long level; };
struct RenderTarget { Surface colour; Surface depth;
    RenderTarget() { colour.level = -1; depth.level = -1; }
    RenderTarget(const Surface& c, const Surface& d) : colour(c), depth(d) { }
    void DetachTarget() { colour.level = -1; L("detach"); }
    void AttachTarget(const Surface& s) { colour = s; Ln("attach", s.level); } };
struct Display { Surface back, depth; int viewport; RenderTarget current; int current_viewport;
    RenderTarget GetRenderTarget() { return RenderTarget(back, depth); }
    void SetRenderTarget(RenderTarget& t) { current = t; Ln("set", t.colour.level); }
    void SetViewport(const int& v) { current_viewport = v; Ln("viewport", v); } };
struct CaptureBlock { CaptureBlock() { L("capture"); } ~CaptureBlock() { L("restore"); } };
struct Texture { long levels;
    long GetNoLevels() const { return levels; }
    Surface GetSurfaceLevel(long level) const { Surface s; s.level = level; return s; }
    void Blt(bool a, bool b, bool c) const { if (a && b && !c) L("blt"); else L("bad-blt"); }
    void GenerateMipmapsWithGPU(Display* display_manager)
    {
        CaptureBlock cb;
        const int& old_viewport = display_manager->viewport;
        RenderTarget old_rt = display_manager->GetRenderTarget();
        RenderTarget rt;
        for (long mm = 1; mm < GetNoLevels(); ++mm) {
            rt.DetachTarget();
            rt.AttachTarget(GetSurfaceLevel(mm));
            display_manager->SetRenderTarget(rt);
            Blt(true, true, false);
        }
        display_manager->SetRenderTarget(old_rt);
        display_manager->SetViewport(old_viewport);
    } };

static int fail(int n) { std::printf("009faf70_TEST FAIL %d: %s\n", n, log_.c_str()); return 1; }

int main()
{
    Display d; d.back.level = 100; d.depth.level = 200; d.viewport = 7; d.current_viewport = 0;
    Texture t; t.levels = 3;
    t.GenerateMipmapsWithGPU(&d);
    if (log_ != "capture detach attach1 set1 blt detach attach2 set2 blt set100 viewport7 restore ") return fail(1);
    if (d.current.colour.level != 100 || d.current.depth.level != 200 || d.current_viewport != 7) return fail(2);
    log_ = ""; t.levels = 1;                   // a single level: nothing to generate, the target is still restored
    t.GenerateMipmapsWithGPU(&d);
    if (log_ != "capture set100 viewport7 restore ") return fail(3);
    std::printf("009faf70_TEST PASS\n");
    return 0;
}
