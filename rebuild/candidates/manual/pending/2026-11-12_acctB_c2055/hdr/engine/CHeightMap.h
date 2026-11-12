#ifndef FABLE_ENGINE_CHEIGHTMAP_H
#define FABLE_ENGINE_CHEIGHTMAP_H
#ifdef FABLE_RETAIL_CHEIGHTMAP
// New header (no repo copy; chunk 2055 scout, row 0x00821A80 CMap::LoadFromFile). Merges the two pending hand-back overlays
// (rebuild/candidates/manual/pending/2026-11-10_linux_hand1017 and 2026-11-11_linux_handQ3, neither landed) -- RECONCILE AT LANDING.
// Retail class: RTTI .?AVCHeightMap@@ : CBaseClassNonCopyable@0 (ghidra_out/rtti_bases.tsv), vtable 0x01288BEC (slot 0 = ??_E
// 0x008F6DE0, rtti-vtables.tsv); sizeof 0x214 (`push 214h`, InitHeightMap 0x0081ED90). Ego_r.pdb member order and names
// (struct_layouts_egor.tsv; the Ego_r vector is 16 bytes, the retail STLport vector 12, so every member behind Cells sits 4 below
// the PDB offset): World +0x04, Map +0x08, WorldSeed +0x0C, Cells +0x10, MapUpdateAreas +0x1C, NoMapUpdateAreas +0x1FC, Changed
// +0x200, SizeX +0x204, SizeY +0x208, SizeXIncludingExtraCells +0x20C, SizeYIncludingExtraCells +0x210 (retail 0x00821FFF..0x0082201B
// reads +0x10 / +0x204 / +0x208). Every member but the destructor and GetCell is private in the PDBs (?Clear@CHeightMap@@AAEXXZ,
// ?GetIndexIntoCells@CHeightMap@@ABEJJJJJ@Z, ?GetExtraCells@CHeightMap@@CIJXZ, ...): CMap is a friend (hand1017).
// Needs FABLE_RETAIL_CHEIGHTMAPCELL (cell flag bits) and FABLE_RETAIL_C2DBOXI defined before the include.
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CHeightMapCell.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DCoordI.h"
class CWorld;class CMap;
class CHeightMap : public CBaseClassNonCopyable {
    friend class CMap;
public:
    virtual ~CHeightMap();                                                         // 0x008F6170 (??_E 0x008F6DE0)
#ifdef FABLE_RETAIL_CHEIGHTMAP_GETCELL
    // Nested opt-in (chunk 2055 scout): FableWin.pdb ?GetCell@CHeightMap@@QAEAAVCHeightMapCell@@ABVC2DCoordI@@@Z (public; FableWin
    // 202i debug = asserts, so the size proves nothing). Body UNVERIFIED: the release index of the private accessor below.
    CHeightMapCell& GetCell(const C2DCoordI& pos) { return GetCellIncludingExtraCellsAt(pos); }
#endif
private:
    void Clear();                                                                  // 0x008F6050 ?Clear@CHeightMap@@AAEXXZ (Ego_r + FableWin)
    void AlterHeightBy(long x, long y, float delta);                               // 0x008F60C0 ?AlterHeightBy@CHeightMap@@AAEXJJM@Z (Ego_r)
    void Resize(long sizeX, long sizeY, long offsetX, long offsetY);              // 0x008F61A0 ?Resize@CHeightMap@@AAEXJJJJ@Z (Ego_r)
    long GetSizeX() const { return SizeX; }                                        // ?GetSizeX@CHeightMap@@ABEJXZ (inline; hand1017)
    long GetSizeY() const { return SizeY; }                                        // ?GetSizeY@CHeightMap@@ABEJXZ (inline; hand1017)
    long GetSizeXIncludingExtraCells() const { return SizeXIncludingExtraCells; }  // ?GetSizeXIncludingExtraCells@CHeightMap@@ABEJXZ
    long GetSizeYIncludingExtraCells() const { return SizeYIncludingExtraCells; }  // ?GetSizeYIncludingExtraCells@CHeightMap@@ABEJXZ
#ifdef FABLE_RETAIL_CHEIGHTMAP_CELLINDEX
    // Nested opt-in (chunk 2055 scout, row 0x00821A80): FableWin.pdb ?GetExtraCells@CHeightMap@@CIJXZ (private static fastcall; FableWin
    // debug 5i/10b = plain /Od of `return 1;`, fwsize SAME) and ?GetIndexIntoCells@CHeightMap@@ABEJJJJJ@Z (FableWin 16i/37b = plain /Od of
    // the text below, fwsize SAME; `(sizeY + GetExtraCells() - 1 - y) * ...` is 18i/46b, a named row local 14i/39b). Retail 0x00821FFF..
    // 0x0082201B: `SizeY - y`, `SizeX + 1`, imul, `+ x`, *12 into Cells (+0x10). NOTE: the pending Q3 overlay's `sizeX * y + x` is
    // 13i at /Od, not FableWin's 16i. ?GetCellIncludingExtraCellsAt@CHeightMap@@AAEAAVCHeightMapCell@@ABVC2DCoordI@@@Z (FableWin 146i =
    // asserts): release body UNVERIFIED beyond the index (retail reads SizeX/SizeY, not the IncludingExtraCells pair).
    static long FABLE_FASTCALL GetExtraCells() { return 1; }
    long GetIndexIntoCells(long x, long y, long sizeX, long sizeY) const { return (sizeY - y) * (sizeX + GetExtraCells()) + x; }
    CHeightMapCell& GetCellIncludingExtraCellsAt(const C2DCoordI& pos) { return Cells[GetIndexIntoCells(pos.X, pos.Y, SizeX, SizeY)]; }
#endif
    CWorld*                     World;                                             // +0x004
    CMap*                       Map;                                               // +0x008
    unsigned long*              WorldSeed;                                         // +0x00C
    std::vector<CHeightMapCell> Cells;                                             // +0x010
    C2DBoxI                     MapUpdateAreas[30];                                // +0x01C
    long                        NoMapUpdateAreas;                                  // +0x1FC
    bool                        Changed;                                           // +0x200
    long                        SizeX;                                             // +0x204
    long                        SizeY;                                             // +0x208
    long                        SizeXIncludingExtraCells;                          // +0x20C
    long                        SizeYIncludingExtraCells;                          // +0x210
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CHeightMap) == 0x214);                  // member offsets: private (asserted by the row's grade, not here)
#endif
#else
class CHeightMap;
#endif // FABLE_RETAIL_CHEIGHTMAP
#endif // FABLE_ENGINE_CHEIGHTMAP_H
