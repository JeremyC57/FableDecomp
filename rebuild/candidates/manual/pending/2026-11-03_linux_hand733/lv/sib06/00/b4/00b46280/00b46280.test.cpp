// Behaviour test for 0x00B46280 CEngineLightingManager::GetLightsInArea(const C3DBoundingBox&, long, const CEnginePrimitiveThemeChannelDesc&)
#include <stdio.h>
#include <math.h>
struct V3 { float X, Y, Z; };
struct Box { V3 Min, Max; };
struct Light { V3 Pos; float RadiusSquared; unsigned long ChannelMask; Light* Next; bool Found; };
struct Link { Light* L; Link* Next; };
struct Theme { unsigned char Index0, Index1, Blend; };
struct Manager { float GridX, GridY; bool Enabled; Link* Grid[16][16]; };
static long g_reset[4], g_maxLights, g_count;
static Light* g_head;
static V3 g_centre;
static long FloorToLong(float v) { return (long)floor(v); }   // GFFloatToLongFloor
static float BoxDistSq(const Box& b, const V3& p)              // C3DBoundingBox::CalcShortestDistanceToPointSquared (x87 fallback)
{
    V3 c = p;
    if (c.X < b.Min.X) c.X = b.Min.X; else if (c.X > b.Max.X) c.X = b.Max.X;
    if (c.Y < b.Min.Y) c.Y = b.Min.Y; else if (c.Y > b.Max.Y) c.Y = b.Max.Y;
    if (c.Z < b.Min.Z) c.Z = b.Min.Z; else if (c.Z > b.Max.Z) c.Z = b.Max.Z;
    float dx = p.X - c.X, dy = p.Y - c.Y, dz = p.Z - c.Z;
    return dx * dx + dz * dz + dy * dy;
}
static void* Query(Manager& m, const Box& box, long maxLights, const Theme& t)   // the row's body
{
    if (maxLights > 8) maxLights = 8;
    if (!m.Enabled) return &m;
    Light* lights = 0; long count = 0;
    long minX = FloorToLong((box.Min.X - m.GridX) * 0.0625f), minY = FloorToLong((box.Min.Y - m.GridY) * 0.0625f);
    long maxX = FloorToLong((box.Max.X - m.GridX) * 0.0625f), maxY = FloorToLong((box.Max.Y - m.GridY) * 0.0625f);
    if (minX < 0) minX = 0; if (minY < 0) minY = 0; if (maxX > 15) maxX = 15; if (maxY > 15) maxY = 15;
    for (long y = minY; y <= maxY; ++y)
        for (long x = minX; x <= maxX; ++x)
            for (Link* k = m.Grid[x][y]; k; k = k->Next) {
                Light* l = k->L;
                bool in0 = t.Blend < 0xFF && (l->ChannelMask & (1 << t.Index0));
                bool inTheme = in0 | (t.Blend > 0 && (l->ChannelMask & (1 << t.Index1)));
                if (!l->Found && inTheme && BoxDistSq(box, l->Pos) < l->RadiusSquared) {
                    k->L->Found = true; l->Next = lights; lights = l; ++count;
                }
            }
    g_reset[0] = minX; g_reset[1] = minY; g_reset[2] = maxX; g_reset[3] = maxY;
    g_head = lights; g_count = count; g_maxLights = maxLights;
    g_centre.X = (box.Max.X + box.Min.X) * 0.5f; g_centre.Y = (box.Max.Y + box.Min.Y) * 0.5f; g_centre.Z = (box.Max.Z + box.Min.Z) * 0.5f;
    return 0;
}
int main()
{
    static Manager m = {};
    m.Enabled = true;
    static Light a = { { 20.0f, 20.0f, 0.0f }, 25.0f, 1, 0, false };    // inside the box
    static Light b = { { 40.0f, 20.0f, 0.0f }, 25.0f, 1, 0, false };    // 8 units right of the box: radius 5 misses
    static Light c = { { 34.0f, 20.0f, 0.0f }, 25.0f, 2, 0, false };    // 2 units right, channel 1 only
    static Link la = { &a, 0 }, lb = { &b, 0 }, lc = { &c, 0 };
    m.Grid[1][1] = &la; m.Grid[2][1] = &lb; m.Grid[2][1]->Next = &lc;
    Box box = { { 10.0f, 10.0f, -1.0f }, { 32.0f, 30.0f, 1.0f } };
    Theme t0 = { 0, 1, 0 }, tb = { 0, 1, 64 };
    int ok = 1;
    Query(m, box, 9, t0);
    ok &= g_count == 1 && g_head == &a && g_maxLights == 8 && g_reset[0] == 0 && g_reset[2] == 2 && g_reset[3] == 1;
    ok &= g_centre.X == 21.0f && g_centre.Y == 20.0f && g_centre.Z == 0.0f;
    a.Found = false;
    Query(m, box, 4, tb);
    ok &= g_count == 2 && g_head == &c && c.Next == &a && g_maxLights == 4;
    printf(ok ? "00b46280_TEST PASS\n" : "00b46280_TEST FAIL\n");
    return ok ? 0 : 1;
}