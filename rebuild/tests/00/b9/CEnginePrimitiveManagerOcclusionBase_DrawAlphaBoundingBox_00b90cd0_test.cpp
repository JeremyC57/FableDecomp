#include <cstdio>
// Model of DrawAlphaBoundingBox: open a capture block, turn pixel shaders off remembering the old setting,
// set nine tracked render states through the capture-aware SetState, render the test mesh, restore the
// pixel-shader setting and the capture block.
struct State { unsigned long value; unsigned long captureMask; bool queued; };
struct Core {
    State* captured[64]; unsigned long prior[64]; State* dirty[64];
    int capturedCount, dirtyCount; unsigned long captureMask;
    State s3204, s3344, s32a4, s3484, factor, s28b8, s291c, s2930, s2840;
};
static Core g_core;
static bool g_pixelShaders = true; static int g_psSets = 0; static bool g_psDuringRender = true;
static int g_restored = 0;
static unsigned long d74 = 0x74, def4 = 0xf4, df78 = 0x78, df7c = 0x7c;
static void SetPixelShadersEnabled(bool on) { g_pixelShaders = on; ++g_psSets; }
static void SetState(State& s, unsigned long v, Core* c)
{
    if (s.value != v) {
        if ((c->captureMask & s.captureMask) == 0) {
            s.captureMask |= c->captureMask;
            c->captured[c->capturedCount] = &s; c->prior[c->capturedCount] = s.value; ++c->capturedCount;
        }
        s.value = v;
        if (!s.queued) { s.queued = true; c->dirty[c->dirtyCount++] = &s; }
    }
}
static void Draw(unsigned long colour)
{
    g_core.captureMask <<= 1;
    bool pixelShaders = g_pixelShaders;
    SetPixelShadersEnabled(false);
    Core* c = &g_core;
    SetState(c->s3204, d74, c); SetState(c->s3344, def4, c); SetState(c->s32a4, d74, c);
    SetState(c->s3484, def4, c); SetState(c->factor, colour, c); SetState(c->s28b8, 1, c);
    SetState(c->s291c, df78, c); SetState(c->s2930, df7c, c); SetState(c->s2840, 0xf, c);
    g_psDuringRender = g_pixelShaders;
    SetPixelShadersEnabled(pixelShaders);
    ++g_restored;
}
int main()
{
    int bad = 0;
    g_core.captureMask = 4;
    g_core.s2840.value = 0xf;          // unchanged: skipped
    g_core.s3344.captureMask = 8;      // already captured at this level after the shift
    Draw(0x80112233);
    if (g_psDuringRender != false || g_pixelShaders != true || g_psSets != 2 || g_restored != 1) bad = 1;
    if (g_core.factor.value != 0x80112233 || g_core.s3344.value != 0xf4 || g_core.s2930.value != 0x7c) bad = 1;
    if (g_core.capturedCount != 7 || g_core.dirtyCount != 8) bad = 1;
    if (bad) { std::printf("00b90cd0_TEST FAIL\n"); return 1; }
    std::printf("00b90cd0_TEST PASS\n");
    return 0;
}