// behaviour test for 00b89a20 CEngineScreenEffectOutlineGlow::BeginOutlineRendering: inactive does nothing; active opens
// the capture block, fetches only the missing textures, saves the display target/viewport, targets the outline texture
// and clears to transparent black
#include <cstdio>
struct Display { int colour, depth; int viewport[4]; int target; };
struct Effect { bool active, captureOpen; int tex[3]; int handle[3]; int oldColour, oldDepth; int oldViewport[4]; };
static int g_fetches = 0, g_captureCount = 0, g_captureMask = 1, g_clearColour = -1;
static int Fetch(int handle) { ++g_fetches; return 100 + handle; }
static void Begin(Effect& e, Display& d)
{
    if (!e.active) return;
    e.captureOpen = true; ++g_captureCount; g_captureMask <<= 1;
    for (int i = 0; i < 3; ++i) if (!e.tex[i]) e.tex[i] = Fetch(e.handle[i]);
    e.oldColour = d.colour; e.oldDepth = d.depth;
    for (int k = 0; k < 4; ++k) e.oldViewport[k] = d.viewport[k];
    d.target = e.tex[0];
    g_clearColour = 0;
}
int main()
{
    Display d = { 7, 8, { 1, 2, 3, 4 }, 0 };
    Effect e = { false, false, { 0, 55, 0 }, { 1, 2, 3 }, 0, 0, { 0, 0, 0, 0 } };
    Begin(e, d);
    if (e.captureOpen || g_fetches) { std::printf("00b89a20_TEST FAIL\n"); return 1; }
    e.active = true; Begin(e, d);
    if (!e.captureOpen || g_fetches != 2 || e.tex[1] != 55 || e.tex[2] != 103 || e.oldColour != 7 || e.oldViewport[3] != 4 ||
        d.target != 101 || g_clearColour != 0 || g_captureMask != 2) { std::printf("00b89a20_TEST FAIL\n"); return 1; }
    std::printf("00b89a20_TEST PASS\n");
    return 0;
}