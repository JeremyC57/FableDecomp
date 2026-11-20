// CWorldMap::PrepareForLoad() -- unload the region that is loaded now, without waiting (UnloadRegion(LoadedRegion, false))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FF560, 15 bytes: `mov eax,[ecx+9Ch] ; push 0 ; push eax ; call 0x004FEEC0 ; ret`.
// Leaf: ?PrepareForLoad@CWorldMap@@QAEXXZ
// NF161 (2026-11-20, q4647): was the July stand-in `void __fastcall PrepareForLoad(S* self)` calling a free `__stdcall pfl_helper(void*, int)`
//   (own leaf ?PrepareForLoad@@YIXPAUS@@@Z, callee ?pfl_helper@@YGXPAXH@Z). Ego_r.pdb 0x004D37D0 + FableWin.pdb 0x01C95F20
//   ?PrepareForLoad@CWorldMap@@QAEXXZ (its landed callers 0x004A0220 / 0x004A2E90 bind it); the callee 0x004FEEC0 is the protected
//   ?UnloadRegion@CWorldMap@@IAEXJ_N@Z (Ego_r 0x004D2FD0 + FableWin 0x01C89730; landed 0x00501450 / 0x00502500 / 0x005063E0 bind it) on the
//   member LoadedRegion (+0x9c, engine/CWorldMap.h). Moved onto the shared header (NF161 opt-in FABLE_RETAIL_CWORLDMAP_PREPAREFORLOAD).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_PREPAREFORLOAD
#include "engine/CWorldMap.h"

void CWorldMap::PrepareForLoad()
{
    UnloadRegion(LoadedRegion, false);
}
