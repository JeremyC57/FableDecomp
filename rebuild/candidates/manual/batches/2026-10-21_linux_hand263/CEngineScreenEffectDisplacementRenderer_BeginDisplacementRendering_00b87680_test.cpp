// behaviour test for 00b87680 CEngineScreenEffectDisplacementRenderer::BeginDisplacementRendering: inactive returns false;
// active fetches a missing texture, saves the display target, clears only once to (R 255, G ClearY, B ClearX, A 0) and
// switches the cached render state to 14, recording the capture and dirty entries only on a change
#include <cstdio>
struct State { unsigned int value, captured; bool dirty; };
struct Core { unsigned int mask; int captureCount, dirtyCount; State st; };
static void SetState(Core& c, State& s, unsigned int v)
{
    if (s.value == v) return;
    if (!(s.captured & c.mask)) { s.captured |= c.mask; ++c.captureCount; }
    s.value = v;
    if (!s.dirty) { s.dirty = true; ++c.dirtyCount; }
}
struct Eff { bool active, hasContent; int tex, handle, clearX, clearY; };
static int g_clears = 0; static unsigned int g_colour = 0;
static bool Begin(Eff& e, Core& c)
{
    if (!e.active) return false;
    if (!e.tex) e.tex = 100 + e.handle;
    if (!e.hasContent) { g_colour = (0u << 24) | (0xffu << 16) | ((unsigned)(e.clearY & 0xff) << 8) | (unsigned)(e.clearX & 0xff); ++g_clears; e.hasContent = true; }
    SetState(c, c.st, 0xe);
    return true;
}
int main()
{
    Core c = { 2, 0, 0, { 3, 0, false } };
    Eff e = { false, false, 0, 5, 0x80, 0x81 };
    if (Begin(e, c)) { std::printf("00b87680_TEST FAIL\n"); return 1; }
    e.active = true;
    if (!Begin(e, c) || e.tex != 105 || g_colour != 0x00ff8180u || c.st.value != 14 || c.captureCount != 1 || c.dirtyCount != 1) {
        std::printf("00b87680_TEST FAIL\n"); return 1;
    }
    if (!Begin(e, c) || g_clears != 1 || c.captureCount != 1 || c.dirtyCount != 1) { std::printf("00b87680_TEST FAIL\n"); return 1; }
    std::printf("00b87680_TEST PASS\n");
    return 0;
}
