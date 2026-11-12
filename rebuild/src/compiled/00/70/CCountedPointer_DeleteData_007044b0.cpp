// CCountedPointer<CScriptThing>::DeleteData(void* data) -- the counted pointer's per-type deleter: `delete (CScriptThing*)data`,
// null-guarded, through the virtual destructor (vtable slot 0, flag 1). Was the free stand-in DeleteData(AutoTinyVirtualObject*).
// Leaf: ?DeleteData@?$CCountedPointer@VCScriptThing@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007044B0: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` (static __fastcall).
// Name: FableWin.pdb 0x027C8170 ?DeleteData@?$CCountedPointer@VCScriptThing@@@@SIXPAX@Z (Anniversary publics carry the same member);
//   the landed 0x007045F0 stores this address as the count block's deleter and binds this symbol (rebuild/oracles/reloc_bindings.tsv).
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

template void FABLE_FASTCALL CCountedPointer<CScriptThing>::DeleteData(void*);
