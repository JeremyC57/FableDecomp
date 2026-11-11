#ifndef FABLE_ENGINE_CHEIGHTMAP_H
#define FABLE_ENGINE_CHEIGHTMAP_H
// NEW class header (chunk Q3, row 0x008F61A0 CHeightMap::Resize). Donor: Ego_r.pdb CHeightMap (ghidra_out/struct_layouts_egor.tsv,
// 0x218 bytes with a 16-byte Dinkumware-debug vector); retail is 0x214 bytes (`push 214h` at 0x0081ED90) with the 12-byte STLport
// vector, so every member behind Cells sits 4 lower than the PDB offset. Vtable 0x01288BEC (CHeightMap : CBaseClassNonCopyable).
// Retail evidence: the landed rows 0x008F6050 (Clear), 0x008F6170 (dtor), 0x008F63C0 (ctor), 0x008F60C0 (AlterHeightBy) use
// the same layout in local models and can migrate here.
// Requires FABLE_RETAIL_CHEIGHTMAPCELL (the cell's flag bits + inline Clear) and FABLE_RETAIL_C2DBOXI before the includes.
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CHeightMapCell.h"
#include "engine/C2DBoxI.h"

class CWorld;
class CMap;

class CHeightMap : public CBaseClassNonCopyable {
public:
    virtual ~CHeightMap();                                      // 0x008F6170
private:
    void Clear();                                               // 0x008F6050 ?Clear@CHeightMap@@AAEXXZ
    void AlterHeightBy(long x, long y, float delta);            // 0x008F60C0 ?AlterHeightBy@CHeightMap@@AAEXJJM@Z
    void Resize(long sizeX, long sizeY, long offsetX, long offsetY);   // 0x008F61A0 ?Resize@CHeightMap@@AAEXJJJJ@Z
    // FableWin.pdb inline accessors (?GetSizeXIncludingExtraCells@CHeightMap@@ABEJXZ, ?GetSizeYIncludingExtraCells@CHeightMap@@ABEJXZ,
    // ?GetIndexIntoCells@CHeightMap@@ABEJJJJJ@Z, ?GetCellIncludingExtraCellsAt@CHeightMap@@AAEAAVCHeightMapCell@@ABVC2DCoordI@@@Z).
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CHeightMap) == 0x214);
#endif

#endif // FABLE_ENGINE_CHEIGHTMAP_H
