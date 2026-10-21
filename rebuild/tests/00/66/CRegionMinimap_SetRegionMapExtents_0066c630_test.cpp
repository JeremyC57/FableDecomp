// CRegionMinimap::SetRegionMapExtents(const CMap& map) -- grow the region map box to include a map's world rectangle (a -1 edge counts as unset) and set the minimap size to 256
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C630, length 107 bytes (`ret 4`). No callees.
// Layout: engine/CRegionMinimap.h (PDB names; the leading members agree with retail): RegionMinimapSize +0,
// RegionMapBox TLX +4, TLY +8, BRX +0xc, BRY +0x10. CMap (retail offsets, 0x10 below the PDB as in the landed
// CMap::CreateScriptedMapBrush 0x00820F80): WorldSizeX +0x1d0c, WorldSizeY +0x1d10, C2DCoordI WorldPos +0x1d24.
// Shape: the map's top-left and size are copied into C2DCoordI locals up front (size first: X +0x1d24 into edx, then
// WorldSizeX/Y, then Y last into eax), the bottom-right is `size + tl` through the inline member operator+ (`lea ebx,
// [edx+esi]` for the right edge, `add edi,eax` in place for the bottom); `tl + size` swaps the lea operands, plain long
// locals keep TLX in ebx and shift every register. Each edge is a two-condition `if (v < edge || edge == -1)` (the
// comparison operand order picks `jl`/`jg`), the size store last. CMap::GetWorldSize is an inline stand-in (no
// out-of-line copy found; the PDB has WorldSizeX/WorldSizeY as two longs).
// The method is attached through a derived shell because the generated header is a plain layout struct.
// Leaf: SetRegionMapExtents
#include "engine/CRegionMinimap.h"

struct C2DCoordI {
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(long x, long y) : X(x), Y(y) { }
    C2DCoordI operator+(const C2DCoordI& o) const { return C2DCoordI(X + o.X, Y + o.Y); }
};

struct CMap {
    unsigned char _pad[0x1d0c];
    long          WorldSizeX;                       // +0x1d0c (PDB +0x1d1c)
    long          WorldSizeY;                       // +0x1d10 (PDB +0x1d20)
    unsigned char _pad1d14[0x10];
    C2DCoordI     WorldPos;                         // +0x1d24 (PDB +0x1d34)
    C2DCoordI GetWorldSize() const { return C2DCoordI(WorldSizeX, WorldSizeY); }
};

struct CRegionMinimap_SetRegionMapExtents : CRegionMinimap {
    void SetRegionMapExtents(const CMap& map);
};

void CRegionMinimap_SetRegionMapExtents::SetRegionMapExtents(const CMap& map)
{
    C2DCoordI size = map.GetWorldSize();
    C2DCoordI tl = map.WorldPos;
    C2DCoordI br = size + tl;
    if (tl.X < RegionMapBox_TLX || RegionMapBox_TLX == -1)
        RegionMapBox_TLX = tl.X;
    if (tl.Y < RegionMapBox_TLY || RegionMapBox_TLY == -1)
        RegionMapBox_TLY = tl.Y;
    if (br.X > RegionMapBox_BRX || RegionMapBox_BRX == -1)
        RegionMapBox_BRX = br.X;
    if (br.Y > RegionMapBox_BRY || RegionMapBox_BRY == -1)
        RegionMapBox_BRY = br.Y;
    RegionMinimapSize = 256;
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    static CMap map;
    static unsigned char mem[sizeof(CRegionMinimap_SetRegionMapExtents)];
    CRegionMinimap_SetRegionMapExtents* r = (CRegionMinimap_SetRegionMapExtents*)mem;
    r->RegionMinimapSize = 0;
    r->RegionMapBox_TLX = -1; r->RegionMapBox_TLY = -1; r->RegionMapBox_BRX = -1; r->RegionMapBox_BRY = -1;
    map.WorldPos.X = 10; map.WorldPos.Y = 20; map.WorldSizeX = 100; map.WorldSizeY = 50;
    r->SetRegionMapExtents(map);
    CHECK(r->RegionMapBox_TLX == 10 && r->RegionMapBox_TLY == 20 && r->RegionMapBox_BRX == 110 && r->RegionMapBox_BRY == 70);
    CHECK(r->RegionMinimapSize == 256);
    map.WorldPos.X = 5; map.WorldPos.Y = 30; map.WorldSizeX = 50; map.WorldSizeY = 100;
    r->SetRegionMapExtents(map);
    CHECK(r->RegionMapBox_TLX == 5 && r->RegionMapBox_TLY == 20 && r->RegionMapBox_BRX == 110 && r->RegionMapBox_BRY == 130);
    map.WorldPos.X = 50; map.WorldPos.Y = 25; map.WorldSizeX = 70; map.WorldSizeY = 10;
    r->SetRegionMapExtents(map);
    CHECK(r->RegionMapBox_TLX == 5 && r->RegionMapBox_TLY == 20 && r->RegionMapBox_BRX == 120 && r->RegionMapBox_BRY == 130);
    if (fails == 0) printf("0066c630_TEST PASS\n");
    return fails;
}