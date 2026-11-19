// NThingMap::CThingMapManager::GetThingMapwhoLevel(float radius) -- picks the mapwho level for a thing of the given radius: the first level whose cell size exceeds floor(2*radius), else the last level
// Leaf: ?GetThingMapwhoLevel@CThingMapManager@NThingMap@@IBEEM@Z
// Flags: none
// Retail-asm: 0x00637903 `fistp dword ptr [ebp+8]` (block `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp ; fild ; fadd [ebp-4] ;
//   fstp ; mov eax ; cmp eax ; jne ; inc dword ptr [ebp+8]`) -- the engine's inline-assembly floor helper GFFloatToLongFloor (shared
//   engine/GFFloatToLongFloor.h, out-of-line copy 0x0048D940); VC7.1 emits no bare dword fistp from C++ (`(long)x` = __ftol2,
//   /QIfist = `fistp qword`). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006378E0, 106 bytes, thiscall `ret 4`, frame pointer (`optimize("y", off)` TU, as the landed
//   CThingPhysical::ModifyHealth 0x006A68F0).
// Identity: Ego_r.pdb / FableWin.pdb ?GetThingMapwhoLevel@CThingMapManager@NThingMap@@IBEEM@Z (protected const, unsigned char result;
//   the reloc binding's QBE spelling is wrong, NAME_FIXES.tsv).
// Globals: NThingMap::MaxLayers 0x013BA848 (landed spelling), the 16-byte level records g_MapwhoLevels 0x0137DAD8 (cell size
//   first; landed spelling), the last-level threshold 0x013BA84C (descriptive name, no landed binding).
// Shape: the level walk has no end test: after the loop sits the engine's unreachable assertion (`__assume(0)`), so VC7.1 keeps the
//   count load into edx but drops its compare (retail 0x00637932). Grade: RELOCATION_MATCH (try f, 6 grades).
#pragma optimize("y", off)
#include "engine/GFFloatToLongFloor.h"
// Name fix (NF100, round 9, q11463 / q11282): the level table is NThingMap::LayerInfo (Ego_d.pdb data symbol ?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B, retail 0x0137DAD8;
//   element NThingMap::CLayerInfo, Ego_r.pdb 16 bytes: Size +0, ScaleShift +4), declared by engine/NThingMap_CThingMap.h (opt-in FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY:
//   the table alone, beside this row's local NThingMap model). It was the global stand-in ?g_MapwhoLevels@@3PAUSMapwhoLevel@@A.
#define FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY
#include "engine/NThingMap_CThingMap.h"
namespace NThingMap { extern long MaxLayers; }   // 0x013BA848 // agentNF16: NThingMap::MaxLayers 0x013BA848 (?MaxLayers@NThingMap@@3JA; shared engine/NThingMap_CThingMap.h)
namespace NThingMap { extern long MaxLayerCellSize; }   // 0x013BA84C (FableWin.pdb public ?MaxLayerCellSize@NThingMap@@3JA, written by InitLayerInfo 0x006379D0)
namespace NThingMap {
class CThingMapManager {
protected:
    unsigned char GetThingMapwhoLevel(float radius) const;
};
unsigned char CThingMapManager::GetThingMapwhoLevel(float radius) const
{
    long size = GFFloatToLongFloor(radius + radius);
    if (size >= MaxLayerCellSize)
        return (unsigned char)(MaxLayers - 1);
    for (long level = 0; level < MaxLayers; ++level)
        if (size < NThingMap::LayerInfo[level].Size)
            return (unsigned char)level;
    // Inferred: unreachable-end marker (EXTRA_RULES 2026-11-19 advisor rule): retail has no end test after the walk (0x00637932).
#ifdef _MSC_VER
    __assume(0);
#elif defined(__GNUC__)
    __builtin_unreachable();
#endif
}
}