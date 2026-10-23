#include <cstdio>
// Model of DrawDebugVisibilityTestBox: open a capture block, set 13 tracked render states through the
// capture-aware SetState (record the prior value once per capture level, queue each changed state once),
// render the test mesh with the box, restore the capture block.
struct State { unsigned long value; unsigned long captureMask; bool queued; };
struct Core {
    State* captured[64]; unsigned long prior[64]; State* dirty[64];
    int capturedCount, dirtyCount; unsigned long captureMask;
    State s3204, s3344, s32a4, s3484, s3218, s32b8, factor, s28b8, s291c, s2930, s2840, s2868, s282c;
};
static Core g_core;
static int g_opened = 0, g_restored = 0, g_rendered = 0;
static const void* g_mesh = 0; static const float* g_box = 0;
static unsigned long d70 = 0x70, d74 = 0x74, def4 = 0xf4, df78 = 0x78, df7c = 0x7c, dfa8 = 0xa8;
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
struct Mgr {
    const void* testMesh;
    void Draw(const float* box, unsigned long colour)
    {
        ++g_opened; g_core.captureMask <<= 1;
        Core* c = &g_core;
        SetState(c->s3204, d74, c); SetState(c->s3344, def4, c); SetState(c->s32a4, d74, c);
        SetState(c->s3484, def4, c); SetState(c->s3218, d70, c); SetState(c->s32b8, d70, c);
        SetState(c->factor, colour, c); SetState(c->s28b8, 1, c); SetState(c->s291c, df78, c);
        SetState(c->s2930, df7c, c); SetState(c->s2840, 0xf, c); SetState(c->s2868, 0, c);
        SetState(c->s282c, dfa8, c);
        ++g_rendered; g_mesh = testMesh; g_box = box;
        ++g_restored;
    }
};
int main()
{
    int bad = 0;
    g_core.captureMask = 1;
    g_core.s2868.value = 0;            // already 0: no capture, no queue
    g_core.s28b8.value = 5; g_core.s28b8.queued = true;   // changed but already queued
    Mgr m; int meshTag = 0; m.testMesh = &meshTag;
    float box[12] = { 0 };
    m.Draw(box, 0xff00ff00);
    if (g_opened != 1 || g_restored != 1 || g_rendered != 1 || g_mesh != &meshTag || g_box != box) bad = 1;
    if (g_core.captureMask != 2) bad = 1;
    if (g_core.factor.value != 0xff00ff00 || g_core.s2840.value != 0xf || g_core.s28b8.value != 1) bad = 1;
    if (g_core.s3204.value != 0x74 || g_core.s3484.value != 0xf4 || g_core.s32b8.value != 0x70 || g_core.s282c.value != 0xa8) bad = 1;
    if (g_core.capturedCount != 12 || g_core.dirtyCount != 11) bad = 1;
    if (g_core.prior[7] != 5 || g_core.captured[0] != &g_core.s3204 || g_core.s3204.captureMask != 2) bad = 1;
    if (bad) { std::printf("00b90700_TEST FAIL\n"); return 1; }
    std::printf("00b90700_TEST PASS\n");
    return 0;
}