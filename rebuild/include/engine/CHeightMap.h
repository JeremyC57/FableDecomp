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
#ifdef FABLE_RETAIL_CHEIGHTMAP_CELLINDEX
    // Nested opt-in (chunk 2055, row 0x00821A80 CMap::LoadFromFile): FableWin.pdb ?GetExtraCells@CHeightMap@@CIJXZ (private static
    // fastcall, `return 1;`) and the release index of ?GetIndexIntoCells@CHeightMap@@ABEJJJJJ@Z as retail 0x00821FFF..0x0082201B
    // computes it (`SizeY - y`, `SizeX + 1`, imul, `+ x`, *12 into Cells +0x10; the same rows-top-down formula as GetCell below,
    // 0x008F6FC0), with ?GetCellIncludingExtraCellsAt@CHeightMap@@AAEAAVCHeightMapCell@@ABVC2DCoordI@@@Z passing SizeX / SizeY.
    // CMap reads these private members in place, so CMap is a friend (as under FABLE_RETAIL_CHEIGHTMAP_GETCELL).
    friend class CMap;
    static long FABLE_FASTCALL GetExtraCells() { return 1; }
    long GetIndexIntoCells(long x, long y, long sizeX, long sizeY) const { return (sizeY - y) * (sizeX + GetExtraCells()) + x; }
    CHeightMapCell& GetCellIncludingExtraCellsAt(const C2DCoordI& pos) { return Cells[GetIndexIntoCells(pos.X, pos.Y, SizeX, SizeY)]; }
#else
    long GetIndexIntoCells(long x, long y, long sizeX, long sizeY) const { return sizeX * y + x; }
    CHeightMapCell& GetCellIncludingExtraCellsAt(const C2DCoordI& pos)
    {
        return Cells[GetIndexIntoCells(pos.X, pos.Y, SizeXIncludingExtraCells, SizeYIncludingExtraCells)];
    }
#endif
#ifdef FABLE_RETAIL_CHEIGHTMAP_GETCELL
    // Chunk Q28 (row 0x008F70F0, whose TU holds the real body of CMap::ZeroAllUnusedThemePaletteEntries 0x008F6FC0): FableWin.pdb
    // ?GetCell@CHeightMap@@QAEAAVCHeightMapCell@@ABVC2DCoordI@@@Z (public; no out-of-line copy in Ego_r.pdb or Fable.exe: 0x008F6FC0
    // expands it as `Cells[(SizeY - y) * (SizeX + 1) + x]`, rows stored top-down). CMap reads SizeX / SizeY in place there although
    // the PDB accessors GetSizeX / GetSizeY are private: CMap is taken to be a friend (inference from those bytes). No layout change.
    friend class CMap;
public:
    CHeightMapCell& GetCell(const C2DCoordI& pos) { return Cells[(SizeY - pos.Y) * (SizeX + 1) + pos.X]; }
private:
#endif

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
