// behaviour test for 00b32300 CEnginePrimitiveRenderer::InitialiseGlobalLights: lighting update, two tracked states from the
// default, white colours handed over, third state enabled; capture/dirty bookkeeping as the render core does it
#include <cstdio>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CTrackedRenderState { long id; unsigned long value; unsigned long mask; long applied; bool queued; };
struct SCaptured { CTrackedRenderState* state; unsigned long prior; };
struct CCore {
    SCaptured captured[16]; CTrackedRenderState* dirty[16];
    long capturedCount, dirtyCount; unsigned long captureMask;
    CTrackedRenderState s2a48, s2a5c, s2a70;
    unsigned long colourA, colourB;
    void SetGlobalLightColours(const CRGBColour& a, const CRGBColour& b)
    { colourA = a.R | (a.G << 8) | (a.B << 16) | ((unsigned long)a.A << 24); colourB = b.R | (b.A << 8); }
    void SetState(CTrackedRenderState& s, unsigned long v)
    {
        if (s.value != v) {
            if ((captureMask & s.mask) == 0) {
                s.mask |= captureMask;
                captured[capturedCount].state = &s; captured[capturedCount].prior = s.value; ++capturedCount;
            }
            s.value = v;
            if (!s.queued) { s.queued = true; dirty[dirtyCount] = &s; ++dirtyCount; }
        }
    }
};
static CCore g_core;
static int g_lightingUpdates = 0;
static unsigned long g_default = 7;
static void InitialiseGlobalLights()
{
    ++g_lightingUpdates;
    g_core.SetState(g_core.s2a70, g_default);
    g_core.SetState(g_core.s2a5c, g_default);
    g_core.SetGlobalLightColours(CRGBColour(255, 255, 255, 255), CRGBColour(255, 255, 255, 255));
    g_core.SetState(g_core.s2a48, 1);
}
int main()
{
    CTrackedRenderState z = { 0, 0, 0, 0, false };
    g_core.s2a48 = z; g_core.s2a5c = z; g_core.s2a70 = z;
    g_core.s2a5c.value = 7;                       // already at the default: untouched
    g_core.s2a48.mask = 1;                        // already captured under the active mask
    g_core.capturedCount = 0; g_core.dirtyCount = 0; g_core.captureMask = 1;
    InitialiseGlobalLights();
    if (g_lightingUpdates != 1 || g_core.s2a70.value != 7 || g_core.s2a48.value != 1) { std::printf("00b32300_TEST FAIL\n"); return 1; }
    if (g_core.capturedCount != 1 || g_core.captured[0].state != &g_core.s2a70 || g_core.captured[0].prior != 0) { std::printf("00b32300_TEST FAIL\n"); return 1; }
    if (g_core.dirtyCount != 2 || g_core.dirty[0] != &g_core.s2a70 || g_core.dirty[1] != &g_core.s2a48) { std::printf("00b32300_TEST FAIL\n"); return 1; }
    if (g_core.colourA != 0xffffffffUL || g_core.colourB != 0xffffUL || g_core.s2a5c.queued) { std::printf("00b32300_TEST FAIL\n"); return 1; }
    std::printf("00b32300_TEST PASS\n");
    return 0;
}