// behaviour test for 00b58de0 CEngineStateBlockDiffuseOnly::Apply: four tracked states set from three defaults
#include <cstdio>
struct CTrackedRenderState { long id; unsigned long value; unsigned long mask; long applied; bool queued; };
struct SCaptured { CTrackedRenderState* state; unsigned long prior; };
struct CCore {
    SCaptured captured[16]; CTrackedRenderState* dirty[16];
    long capturedCount, dirtyCount; unsigned long captureMask;
    CTrackedRenderState s291c, s2930, s3204, s32a4;
};
static void SetState(CTrackedRenderState& s, unsigned long v, CCore* core)
{
    if (s.value != v) {
        if ((core->captureMask & s.mask) == 0) {
            s.mask |= core->captureMask;
            core->captured[core->capturedCount].state = &s; core->captured[core->capturedCount].prior = s.value; ++core->capturedCount;
        }
        s.value = v;
        if (!s.queued) { s.queued = true; core->dirty[core->dirtyCount] = &s; ++core->dirtyCount; }
    }
}
static CCore g_core;
static unsigned long g_6c = 5, g_68 = 6, g_70 = 2;
static void Apply()
{
    SetState(g_core.s291c, g_6c, &g_core);
    SetState(g_core.s2930, g_68, &g_core);
    SetState(g_core.s3204, g_70, &g_core);
    SetState(g_core.s32a4, g_70, &g_core);
}
int main()
{
    CTrackedRenderState z = { 0, 0, 0, 0, false };
    g_core.s291c = z; g_core.s2930 = z; g_core.s3204 = z; g_core.s32a4 = z;
    g_core.s2930.value = 6;                 // already at its default
    g_core.s3204.queued = true;             // already queued: captured, not re-queued
    g_core.capturedCount = 0; g_core.dirtyCount = 0; g_core.captureMask = 2;
    Apply();
    if (g_core.s291c.value != 5 || g_core.s3204.value != 2 || g_core.s32a4.value != 2) { std::printf("00b58de0_TEST FAIL\n"); return 1; }
    if (g_core.capturedCount != 3 || g_core.captured[1].state != &g_core.s3204) { std::printf("00b58de0_TEST FAIL\n"); return 1; }
    if (g_core.dirtyCount != 2 || g_core.dirty[1] != &g_core.s32a4 || g_core.s2930.mask != 0) { std::printf("00b58de0_TEST FAIL\n"); return 1; }
    std::printf("00b58de0_TEST PASS\n");
    return 0;
}
