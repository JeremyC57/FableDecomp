#include <cstdio>
// Behaviour model of CSurfaceCompositor::StartComposition 0x00F39730: capture-block push, render target switch,
// WasRendering bookkeeping, window save/replace, five tracked render states with bookmark/dirty-list logic.
struct StateInfo { unsigned long Current, Desired, Bookmark, pad, Dirty; };
struct Mgr {
    StateInfo* stack[64]; int stackSize; StateInfo* dirty[64]; int dirtySize; unsigned long mask; StateInfo dummy;
    StateInfo colour, uvU, uvV, fMin, fMag;
    float win[4]; int defaults;
    void Set(unsigned long v, StateInfo& s) {
        if (v == s.Desired) return;
        if (!(mask & s.Bookmark)) { s.Bookmark |= mask; stack[stackSize++] = &s; }
        s.Desired = v;
        if (!s.Dirty) { s.Dirty = 1; dirty[dirtySize++] = &s; }
    }
};
static unsigned long UVTable[4] = { 1, 2, 3, 4 };
static unsigned long FilterTable[4] = { 1, 2, 3, 4 };
struct Display { bool rendering; int begun; void* target; };
struct Comp { bool open, wasRendering, alpha; int surfW, surfH; float oldWin[4]; long layers; int base; };
static void Start(Comp& c, Mgr& m, Display& d, bool alpha)
{
    c.open = true; m.stack[m.stackSize++] = &m.dummy; m.mask <<= 1;
    c.alpha = alpha;
    m.Set(0xF, m.colour);
    d.target = &c;
    c.wasRendering = true;
    if (!d.rendering) { d.begun++; d.rendering = true; c.wasRendering = false; }
    m.defaults++;
    for (int i = 0; i < 4; ++i) c.oldWin[i] = m.win[i];
    m.win[0] = 0; m.win[1] = 0; m.win[2] = (float)c.surfW; m.win[3] = (float)c.surfH;
    m.Set(UVTable[0], m.uvU); m.Set(UVTable[0], m.uvV);
    m.Set(FilterTable[0], m.fMin); m.Set(FilterTable[0], m.fMag);
    c.base++; c.layers = 0;
}
int main()
{
    static Mgr m = {}; m.mask = 1; m.win[2] = 640; m.win[3] = 480; m.uvV.Desired = 1;
    Display d = { false, 0, 0 };
    Comp c = {}; c.surfW = 256; c.surfH = 128; c.layers = 5;
    Start(c, m, d, true);
    bool ok = c.open && c.alpha && !c.wasRendering && d.begun == 1 && d.target == &c && m.mask == 2
        && m.colour.Desired == 0xF && m.uvU.Desired == 1 && m.fMin.Desired == 1 && m.fMag.Desired == 1
        && m.dirtySize == 4 && m.stackSize == 5 && c.oldWin[2] == 640.0f && m.win[2] == 256.0f && m.win[3] == 128.0f
        && c.layers == 0 && c.base == 1 && m.defaults == 1;
    Comp c2 = {}; c2.surfW = 1; c2.surfH = 1;
    Start(c2, m, d, false);
    ok = ok && c2.wasRendering && d.begun == 1 && !c2.alpha && m.mask == 4 && m.dirtySize == 4;
    if (!ok) { std::printf("00f39730_TEST FAIL\n"); return 1; }
    std::printf("00f39730_TEST PASS\n");
    return 0;
}