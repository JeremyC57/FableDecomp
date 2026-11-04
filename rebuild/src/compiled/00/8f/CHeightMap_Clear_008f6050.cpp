// CHeightMap::Clear() -- reset every cell (including the extra row and column) to height 0, palette indices 1, blend {0xff, 0}, and the flags Changed/Passable/ShorePoint/NavPreferred off, CameraPassable on
// Leaf: ?Clear@CHeightMap@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F6050, 109 bytes; Ego_r.pdb ?Clear@CHeightMap@@AAEXXZ (private). No callees.
// Layout: CHeightMap as the landed destructor 0x008F6170 / constructor 0x008F63C0 (CBaseClassNonCopyable base, vtable 0x01288BEC;
//   World +4, Map +8, WorldSeed +0xC, Cells (STLport vector<CHeightMapCell>) +0x10, MapUpdateAreas[30] +0x1C ... the sizes
//   including the extra cells +0x20C / +0x210); no engine header exists for CHeightMap. CHeightMapCell from engine/CHeightMapCell.h
//   with the Ego_r.pdb flag bits (FABLE_RETAIL_CHEIGHTMAPCELL).
// Shape: FableWin.pdb inline CHeightMapCell::Clear() per cell, reached through the inline GetCellIncludingExtraCellsAt(C2DCoordI)
//   / GetIndexIntoCells accessors. The palette bytes are a three-step byte loop that VC7.1 turns into the `mov ebx,01010101h` word +
//   byte fill; an explicit memset(.., 1, 3) instead puts the address in ebp and hoists 0xff (not 0) into ecx (old draft, 123 bytes).
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#include "stlport/fable_containers.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CHeightMapCell.h"
#include "engine/C2DBoxI.h"

class CWorld;
class CMap;

class CHeightMap : public CBaseClassNonCopyable {               // vtable 0x01288BEC, retail 0x214 bytes
public:
    virtual ~CHeightMap();
private:
    void Clear();
    // FableWin.pdb inline accessors: ?GetSizeXIncludingExtraCells@CHeightMap@@ABEJXZ, ?GetSizeYIncludingExtraCells@CHeightMap@@ABEJXZ,
    // ?GetIndexIntoCells@CHeightMap@@ABEJJJJJ@Z, ?GetCellIncludingExtraCellsAt@CHeightMap@@AAEAAVCHeightMapCell@@ABVC2DCoordI@@@Z.
    long GetSizeXIncludingExtraCells() const { return SizeXIncludingExtraCells; }
    long GetSizeYIncludingExtraCells() const { return SizeYIncludingExtraCells; }
    long GetIndexIntoCells(long x, long y, long sizeX, long sizeY) const { return sizeX * y + x; }
    CHeightMapCell& GetCellIncludingExtraCellsAt(const C2DCoordI& pos)
    {
        return Cells[GetIndexIntoCells(pos.X, pos.Y, SizeXIncludingExtraCells, SizeYIncludingExtraCells)];
    }
    CWorld*                     World;                          // +0x004
    CMap*                       Map;                            // +0x008
    unsigned long*              WorldSeed;                      // +0x00c
    std::vector<CHeightMapCell> Cells;                          // +0x010
    C2DBoxI                     MapUpdateAreas[30];             // +0x01c
    long                        NoMapUpdateAreas;               // +0x1fc
    bool                        Changed;                        // +0x200
    long                        SizeX;                          // +0x204
    long                        SizeY;                          // +0x208
    long                        SizeXIncludingExtraCells;       // +0x20c
    long                        SizeYIncludingExtraCells;       // +0x210
};

void CHeightMap::Clear()
{
    for (long y = 0; y < GetSizeYIncludingExtraCells(); ++y)
        for (long x = 0; x < GetSizeXIncludingExtraCells(); ++x)
            GetCellIncludingExtraCellsAt(C2DCoordI(x, y)).Clear();
}