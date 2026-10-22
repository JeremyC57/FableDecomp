#include <cstdio>
// Behaviour model of CLandscapeBackgroundPatch::UpdateTesselation (0x00BE61B0): build the tesselation record, run the land
// tesselator (taking back its bridge buffers and clearing them from the record) and/or the water sub-patch, then mark the
// four edge strips tesselated on success.
struct Data { long worldX, worldY, width, height; int ib; int bridgeIB[4], bridgeVB[4]; long poly[4]; };
struct Patch {
    unsigned short originX, originY, mapX, mapY, width, height;
    int ib; int bridgeIB[4], bridgeVB[4]; unsigned short poly[4];
    bool waterOnly; bool hasWater; bool landOk, waterOk; bool tess[4];
    int waterCalls; long seenX, seenY;
};
static bool g_enabled = true;
static bool Land(Patch& p, Data& d) { p.seenX = d.worldX; p.seenY = d.worldY; for (int i = 0; i < 4; ++i) { d.bridgeIB[i] = 10 + i; d.bridgeVB[i] = 20 + i; d.poly[i] = 30 + i; } return p.landOk; }
static bool Water(Patch& p, Data& d) { ++p.waterCalls; return p.waterOk && d.bridgeIB[0] == 0; }
static void Update(Patch& p)
{
    if (!g_enabled) return;
    Data d; d.worldX = p.originX + p.mapX; d.worldY = p.originY + p.mapY; d.width = p.width; d.height = p.height; d.ib = p.ib;
    for (int i = 0; i < 4; ++i) { d.bridgeIB[i] = p.bridgeIB[i]; d.bridgeVB[i] = p.bridgeVB[i]; d.poly[i] = p.poly[i]; }
    bool ok;
    if (p.waterOnly) ok = Water(p, d);
    else {
        ok = Land(p, d);
        for (int i = 0; i < 4; ++i) { p.bridgeIB[i] = d.bridgeIB[i]; p.bridgeVB[i] = d.bridgeVB[i]; p.poly[i] = (unsigned short)d.poly[i]; d.bridgeIB[i] = 0; d.bridgeVB[i] = 0; }
        d.ib = 0;
        if (ok && p.hasWater) ok = Water(p, d);
    }
    if (ok) for (int i = 0; i < 4; ++i) p.tess[i] = true;
}
int main()
{
    Patch a = { 100, 200, 3, 4, 16, 16, 7, { 1, 1, 1, 1 }, { 2, 2, 2, 2 }, { 5, 5, 5, 5 }, false, true, true, true, { false, false, false, false }, 0, 0, 0 };
    Update(a);
    int ok = a.seenX == 103 && a.seenY == 204 && a.bridgeIB[2] == 12 && a.bridgeVB[3] == 23 && a.poly[1] == 31 && a.waterCalls == 1 && a.tess[0] && a.tess[3];
    Patch b = a; b.landOk = false; b.waterCalls = 0; b.tess[0] = b.tess[3] = false; Update(b);
    ok = ok && b.waterCalls == 0 && !b.tess[0];
    Patch c = a; c.waterOnly = true; c.waterCalls = 0; c.tess[0] = false; c.bridgeIB[0] = 0; Update(c);
    ok = ok && c.waterCalls == 1 && c.tess[0];
    g_enabled = false; Patch e = a; e.tess[1] = false; Update(e); ok = ok && !e.tess[1];
    if (!ok) { std::printf("00be61b0_TEST FAIL\n"); return 1; }
    std::printf("00be61b0_TEST PASS\n");
    return 0;
}
