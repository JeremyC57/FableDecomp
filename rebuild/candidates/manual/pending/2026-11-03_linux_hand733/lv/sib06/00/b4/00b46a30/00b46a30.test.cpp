// Behaviour test for 0x00B46A30 CEngineLightingManager::LookupSmallObjectLightCache(float, float, long)
#include <stdio.h>
#include <math.h>
struct Box { float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; };
static int g_calls;
static Box g_lastBox;
static int g_cellsFilled[64][64];
static void* GetLightsInArea(const Box& b, long) { ++g_calls; g_lastBox = b; return &g_cellsFilled[0][0] + g_calls; }
struct Manager { float GridX, GridY; void* Grid[64][64]; };
static long FloorToLong(float v)   // GFFloatToLongFloor: round(v - 0.5), bumped when that is exactly one below v
{
    long r = (long)floor(v);   /* fistp(v - 0.5) to nearest, then the exact-integer bump */
    float next = (float)r + 1.0f;   /* never equal for a floored r */
    if (v == next) ++r;
    return r;
}
static void* Lookup(Manager& m, float x, float y, long mask)   // the row's body
{
    long cx = FloorToLong((x - m.GridX) * 0.25f), cy = FloorToLong((y - m.GridY) * 0.25f);
    if (cx < 0 || cy < 0 || cx >= 64 || cy >= 64) return 0;
    void** cell = &m.Grid[cx][cy];
    if (!*cell) {
        float minX = (float)(cx * 4) + m.GridX, minY = (float)(cy * 4) + m.GridY;
        Box b = { minX, minY, -1e30f, minX + 6.0f, minY + 6.0f, 1e30f };
        *cell = GetLightsInArea(b, mask);
    }
    return *cell;
}
int main()
{
    static Manager m = {};
    m.GridX = -100.0f; m.GridY = 50.0f;
    int ok = 1;
    ok &= Lookup(m, -200.0f, 60.0f, 1) == 0;                 // x cell negative
    ok &= Lookup(m, 200.0f, 60.0f, 1) == 0;                  // x cell 75 >= 64
    void* a = Lookup(m, -90.0f, 61.0f, 1);                   // cell (2, 2)
    ok &= a != 0 && g_calls == 1 && g_lastBox.MinX == -92.0f && g_lastBox.MinY == 58.0f && g_lastBox.MaxX == -86.0f;
    ok &= Lookup(m, -89.0f, 60.0f, 1) == a && g_calls == 1;  // same cell: cached
    ok &= Lookup(m, -80.0f, 62.0f, 1) != a && g_calls == 2;  // cell (5, 3)
    printf(ok ? "00b46a30_TEST PASS\n" : "00b46a30_TEST FAIL\n");
    return ok ? 0 : 1;
}