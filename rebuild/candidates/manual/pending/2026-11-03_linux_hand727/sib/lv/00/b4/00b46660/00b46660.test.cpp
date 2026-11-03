// Behaviour test for 0x00B46660 CEngineLightingManager::AddLightsToLookupGrid()
#include <stdio.h>
#include <math.h>
struct Light { float X, Y, Radius; };
struct Link { Light* L; Link* Next; };
static Link g_pool[64];
static int g_used;
struct Manager { bool Enabled; float GridX, GridY; Light* Lights[4]; unsigned int Count; Link* Grid[16][16]; };
static long FloorToLong(float v) { return (long)floor(v); }   // GFFloatToLongFloor
static void AddLights(Manager& m)                               // the row's body
{
    if (!m.Enabled) return;
    for (unsigned long i = 0; i < m.Count; ++i) {
        Light* l = m.Lights[i];
        long minX = FloorToLong((l->X - m.GridX - l->Radius) * 0.0625f), minY = FloorToLong((l->Y - m.GridY - l->Radius) * 0.0625f);
        long maxX = FloorToLong((l->X - m.GridX + l->Radius) * 0.0625f), maxY = FloorToLong((l->Y - m.GridY + l->Radius) * 0.0625f);
        if (minX < 0) minX = 0; if (minY < 0) minY = 0; if (maxX > 15) maxX = 15; if (maxY > 15) maxY = 15;
        for (long y = minY; y <= maxY; ++y)
            for (long x = minX; x <= maxX; ++x) {
                Link* k = &g_pool[g_used++];
                k->L = l; k->Next = m.Grid[x][y]; m.Grid[x][y] = k;
            }
    }
}
int main()
{
    static Manager m = {};
    static Light a = { 20.0f, 20.0f, 6.0f }, b = { 250.0f, 2.0f, 4.0f };
    m.Lights[0] = &a; m.Lights[1] = &b; m.Count = 2;
    int ok = 1;
    AddLights(m);
    ok &= g_used == 0;                                           // disabled: nothing
    m.Enabled = true;
    AddLights(m);
    // a: 14..26 -> cells 0..1 in x and y (4 links); b: x 246..254 -> 15 (clamped), y -2..6 -> 0 (clamped from -1): 1 link
    ok &= g_used == 5 && m.Grid[0][0]->L == &a && m.Grid[1][1]->L == &a && m.Grid[15][0]->L == &b && m.Grid[2][2] == 0;
    printf(ok ? "00b46660_TEST PASS\n" : "00b46660_TEST FAIL\n");
    return ok ? 0 : 1;
}