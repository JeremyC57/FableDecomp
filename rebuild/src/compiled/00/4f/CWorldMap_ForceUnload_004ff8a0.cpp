// CWorldMap::ForceUnload(long) -- unload one map now: forwards to the protected UnloadLevel
// Leaf: ?ForceUnload@CWorldMap@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FF8A0, 5 bytes (`jmp 0x004FEDE0`).
// Body port (NF321, 2026-11-23): defined on the real class of engine/CWorldMap.h (FABLE_RETAIL_CWORLDMAP; was
//   ?ForceUnload@CWorldMap_Methods@@QAEXJ@Z); the retail RTTI vftable placement names this PDB member here. Callee: Ego_r.pdb / FableWin.pdb
//   ?UnloadLevel@CWorldMap@@IAEXJ@Z (protected), retail 0x004FEDE0, which the donor FableWin.exe ForceUnload calls; declared by the
//   add-only opt-in FABLE_RETAIL_CWORLDMAP_UNLOADLEVEL_DECL (was the made-up alias member Impl(long)).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_UNLOADLEVEL_DECL
#include "engine/CWorldMap.h"

void CWorldMap::ForceUnload(long map)
{
    UnloadLevel(map);
}
