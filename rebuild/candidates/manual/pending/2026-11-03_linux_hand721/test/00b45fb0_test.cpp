// Behaviour test for 0x00B45FB0 CEngineLightingManager::GetLightsInSphere(const C3DBoundingSphere&, long, const CEnginePrimitiveThemeChannelDesc&)
#include <stdio.h>
#include <math.h>
struct Light { float X, Y, Z, Radius; unsigned long ChannelMask; Light* Next; bool Found; };
struct Link { Light* L; Link* Next; };
struct Theme { unsigned char Index0, Index1, Blend; };
struct Manager { float GridX, GridY; bool Enabled; Link* Grid[16][16]; };
static long g_resetArgs[4], g_maxLights, g_count;
static Light* g_head;
static long FloorToLong(float v) { return (long)floor(v); }   // GFFloatToLongFloor
static void* Query(Manager& m, float cx, float cy, float cz, float r, long maxLights, const Theme& t)   // the row's body
{
    if (maxLights > 8) maxLights = 8;
    if (!m.Enabled) return &m;   // EmptyLightArray
    Light* lights = 0; long count = 0;
    long minX = FloorToLong((cx - r - m.GridX) * 0.0625f), minY = FloorToLong((cy - r - m.GridY) * 0.0625f);
    long maxX = FloorToLong((r + cx - m.GridX) * 0.0625f), maxY = FloorToLong((r + cy - m.GridY) * 0.0625f);
    if (minX < 0) minX = 0; if (minY < 0) minY = 0; if (maxX > 15) maxX = 15; if (maxY > 15) maxY = 15;
    for (long y = minY; y <= maxY; ++y)
        for (long x = minX; x <= maxX; ++x)
            for (Link* k = m.Grid[x][y]; k; k = k->Next) {
                Light* l = k->L;
                bool in0 = t.Blend < 0xFF && (l->ChannelMask & (1 << t.Index0));
                bool inTheme = in0 | (t.Blend > 0 && (l->ChannelMask & (1 << t.Index1)));
                if (!l->Found && inTheme) {
                    float dx = l->X - cx, dy = l->Y - cy, dz = l->Z - cz, rr = l->Radius + r;
                    if (dx * dx + dz * dz + dy * dy < rr * rr) { l->Found = true; l->Next = lights; lights = l; ++count; }
                }
            }
    g_resetArgs[0] = minX; g_resetArgs[1] = minY; g_resetArgs[2] = maxX; g_resetArgs[3] = maxY;
    g_head = lights; g_count = count; g_maxLights = maxLights;
    return 0;
}
int main()
{
    static Manager m = {};
    m.GridX = 0.0f; m.GridY = 0.0f; m.Enabled = true;
    static Light a = { 20.0f, 20.0f, 0.0f, 5.0f, 1, 0, false };    // channel 0, near
    static Light b = { 40.0f, 20.0f, 0.0f, 5.0f, 2, 0, false };    // channel 1, near
    static Light c = { 200.0f, 200.0f, 0.0f, 1.0f, 1, 0, false };  // far
    static Link la = { &a, 0 }, lb = { &b, 0 }, la2 = { &a, 0 }, lc = { &c, 0 };
    m.Grid[1][1] = &la; m.Grid[2][1] = &lb; m.Grid[1][2] = &la2; m.Grid[12][12] = &lc;
    Theme t0 = { 0, 1, 0 };
    int ok = 1;
    Query(m, 30.0f, 20.0f, 0.0f, 12.0f, 20, t0);
    ok &= g_maxLights == 8 && g_count == 1 && g_head == &a;                 // only channel 0; a once although linked twice
    ok &= g_resetArgs[0] == 1 && g_resetArgs[1] == 0 && g_resetArgs[2] == 2 && g_resetArgs[3] == 2;
    a.Found = false;
    Theme tb = { 0, 1, 128 };
    Query(m, 30.0f, 20.0f, 0.0f, 12.0f, 3, tb);
    ok &= g_maxLights == 3 && g_count == 2 && g_head == &b && b.Next == &a;  // blended: both channels
    m.Enabled = false;
    ok &= Query(m, 0, 0, 0, 1, 1, t0) == &m;
    printf(ok ? "00b45fb0_TEST PASS\n" : "00b45fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
