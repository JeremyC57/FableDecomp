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
struct SMapwhoLevel {                           // 16 bytes
    long CellSize;                              // +0x00
    long _pad[3];
};
namespace NThingMap { extern long MaxLayers; }   // 0x013BA848 // agentNF16: NThingMap::MaxLayers 0x013BA848 (?MaxLayers@NThingMap@@3JA; shared engine/NThingMap_CThingMap.h)
extern long g_MaxMapwhoCellSize;                // 0x013BA84C
extern SMapwhoLevel g_MapwhoLevels[];           // 0x0137DAD8
namespace NThingMap {
class CThingMapManager {
protected:
    unsigned char GetThingMapwhoLevel(float radius) const;
};
unsigned char CThingMapManager::GetThingMapwhoLevel(float radius) const
{
    long size = GFFloatToLongFloor(radius + radius);
    if (size >= g_MaxMapwhoCellSize)
        return (unsigned char)(MaxLayers - 1);
    for (long level = 0; level < MaxLayers; ++level)
        if (size < g_MapwhoLevels[level].CellSize)
            return (unsigned char)level;
    __assume(0);
}
}