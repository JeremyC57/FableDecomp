#include <cstdio>
// Model of RenderMeshDecalStencilClear: apply the stencil-clear shaders, set nine tracked render states,
// build the eight box corners (min/max per axis in the retail order) and draw 12 indexed triangles.
struct V3 { float X, Y, Z; };
struct State { unsigned long value; bool queued; };
static State g_states[9]; static int g_dirty = 0;
static const V3* g_drawn = 0; static int g_drawnCount = 0, g_drawnTris = 0, g_prepared = -1;
static V3 g_copy[8];
static int g_vs = 0, g_ps = 0;
static void SetState(State& s, unsigned long v) { if (s.value != v) { s.value = v; if (!s.queued) { s.queued = true; ++g_dirty; } } }
static void DrawAsIndexedTris(const V3* v, int stride, int count, int tris)
{
    g_drawn = v; g_drawnCount = count; g_drawnTris = tris;
    for (int i = 0; i < 8; ++i) g_copy[i] = v[i];
    if (stride != (int)sizeof(V3)) g_drawnCount = -1;
}
static void Clear(const V3& minimum, const V3& maximum)
{
    ++g_vs; ++g_ps;
    unsigned long f3c = 0x3c, e34 = 0x34, f9c = 0x9c;
    SetState(g_states[0], f3c); SetState(g_states[1], f3c); SetState(g_states[2], f3c); SetState(g_states[3], e34);
    SetState(g_states[4], 0); SetState(g_states[5], 0); SetState(g_states[6], 0); SetState(g_states[7], 0);
    SetState(g_states[8], f9c);
    V3 corners[8] = {
        { minimum.X, maximum.Y, maximum.Z }, { maximum.X, maximum.Y, maximum.Z },
        { minimum.X, minimum.Y, maximum.Z }, { maximum.X, minimum.Y, maximum.Z },
        { minimum.X, maximum.Y, minimum.Z }, { maximum.X, maximum.Y, minimum.Z },
        { minimum.X, minimum.Y, minimum.Z }, { maximum.X, minimum.Y, minimum.Z },
    };
    g_prepared = 0;
    DrawAsIndexedTris(corners, sizeof(V3), 8, 12);
}
int main()
{
    int bad = 0;
    g_states[4].value = 7;
    V3 mn = { -1.0f, -2.0f, -3.0f }; V3 mx = { 4.0f, 5.0f, 6.0f };
    Clear(mn, mx);
    if (g_vs != 1 || g_ps != 1 || g_prepared != 0 || g_drawnCount != 8 || g_drawnTris != 12) bad = 1;
    if (g_dirty != 6) bad = 1;   // states 0..4 and 8 change; 5..7 are already 0
    if (g_copy[0].X != -1.0f || g_copy[0].Y != 5.0f || g_copy[0].Z != 6.0f) bad = 1;
    if (g_copy[3].X != 4.0f || g_copy[3].Y != -2.0f || g_copy[3].Z != 6.0f) bad = 1;
    if (g_copy[6].X != -1.0f || g_copy[6].Y != -2.0f || g_copy[6].Z != -3.0f) bad = 1;
    if (g_copy[7].X != 4.0f || g_copy[7].Y != -2.0f || g_copy[7].Z != -3.0f) bad = 1;
    if (g_states[3].value != 0x34 || g_states[8].value != 0x9c || g_states[4].value != 0) bad = 1;
    if (bad) { std::printf("00bb8090_TEST FAIL\n"); return 1; }
    std::printf("00bb8090_TEST PASS\n");
    return 0;
}