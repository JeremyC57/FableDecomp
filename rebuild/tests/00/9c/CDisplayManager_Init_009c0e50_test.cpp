// behaviour test for 009c0e50 CDisplayManager::Init
#include <cstdio>
struct Init { long w, h, depth; bool use2D, shaders; };
struct Display { bool windowed, use2D, shaders; long devW, devH, devDepth; int created, renderManagers, shaderInits; };
struct Failed {};
static int CreateDevice(Display& d, long w, long h, long depth, bool ok) { d.devW = w; d.devH = h; d.devDepth = depth; ++d.created; return ok ? 2 : 0; }
static void DoInit(Display& d, const Init& init, bool windowed, bool deviceOk)
{
    d.windowed = windowed;
    d.use2D = init.use2D;
    d.shaders = init.shaders && init.use2D;
    long width = init.w, height = init.h, depth = init.depth;
    if (width < 32) width = 32;
    if (height < 32) height = 32;
    if (CreateDevice(d, width, height, depth, deviceOk) != 2)
        throw Failed();
    if (d.use2D) {
        ++d.renderManagers;
        if (d.shaders) ++d.shaderInits;
    }
}
int main()
{
    int bad = 0;
    Display d = Display();
    Init i; i.w = 16; i.h = 600; i.depth = 32; i.use2D = true; i.shaders = true;
    DoInit(d, i, true, true);
    if (d.devW != 32 || d.devH != 600 || d.devDepth != 32 || !d.windowed || d.renderManagers != 1 || d.shaderInits != 1) bad |= 1;
    Display e = Display();
    i.w = 800; i.h = 8; i.use2D = false;
    DoInit(e, i, false, true);
    if (e.devW != 800 || e.devH != 32 || e.shaders || e.renderManagers != 0) bad |= 2;
    bool threw = false;
    try { Display f = Display(); DoInit(f, i, false, false); } catch (const Failed&) { threw = true; }
    if (!threw) bad |= 4;
    if (bad) { std::printf("009c0e50_TEST FAIL\n"); return 1; }
    std::printf("009c0e50_TEST PASS\n");
    return 0;
}