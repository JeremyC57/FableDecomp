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

class C2DCoordI { public:
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(long x, long y) : X(x), Y(y) { }
    C2DCoordI operator+(const C2DCoordI& o) const { return C2DCoordI(X + o.X, Y + o.Y); }
};

class CMap { public:
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