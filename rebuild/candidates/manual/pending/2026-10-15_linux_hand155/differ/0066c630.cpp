// CRegionMinimap::SetRegionMapExtents(const CMap& map) -- grow the region map box to include a map's world rectangle (a -1 edge counts as unset) and set the minimap size to 256
// Grade: DIFFER(107v108) -- register allocation only: retail forms the right edge with `lea ebx,[edx+esi]` and loads the box TLX into the freed esi just before its test; ours preloads TLX into ebx and adds in place (`add esi,edx`), shifting every later register. Tried: locals, inline sums in other operand orders, a C2DBoxI local.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C630, length 107 bytes (`ret 4`). No callees.
// Layout: engine/CRegionMinimap.h (PDB names; the leading members agree with retail): RegionMinimapSize +0,
// RegionMapBox TLX +4, TLY +8, BRX +0xc, BRY +0x10. CMap (retail offsets, 0x10 below the PDB as in the landed
// CMap::CreateScriptedMapBrush 0x00820F80): WorldSizeX +0x1d0c, WorldSizeY +0x1d10, C2DCoordI WorldPos +0x1d24.
// Shape: the four map values are loaded up front, the right/bottom edges summed into locals; each edge is a
// two-condition `if (v < edge || edge == -1)` (the comparison operand order picks `jl`/`jg`), the size store last.
// The method is attached through a derived shell because the generated header is a plain layout struct.
// Leaf: SetRegionMapExtents
#include "engine/CRegionMinimap.h"

struct C2DCoordI { long X, Y; };

struct CMap {
    unsigned char _pad[0x1d0c];
    long          WorldSizeX;                       // +0x1d0c (PDB +0x1d1c)
    long          WorldSizeY;                       // +0x1d10 (PDB +0x1d20)
    unsigned char _pad1d14[0x10];
    C2DCoordI     WorldPos;                         // +0x1d24 (PDB +0x1d34)
};

struct CRegionMinimap_SetRegionMapExtents : CRegionMinimap {
    void SetRegionMapExtents(const CMap& map);
};

void CRegionMinimap_SetRegionMapExtents::SetRegionMapExtents(const CMap& map)
{
    long left = map.WorldPos.X;
    long top = map.WorldPos.Y;
    long right = left + map.WorldSizeX;
    long bottom = top + map.WorldSizeY;
    if (left < RegionMapBox_TLX || RegionMapBox_TLX == -1)
        RegionMapBox_TLX = left;
    if (top < RegionMapBox_TLY || RegionMapBox_TLY == -1)
        RegionMapBox_TLY = top;
    if (right > RegionMapBox_BRX || RegionMapBox_BRX == -1)
        RegionMapBox_BRX = right;
    if (bottom > RegionMapBox_BRY || RegionMapBox_BRY == -1)
        RegionMapBox_BRY = bottom;
    RegionMinimapSize = 256;
}
