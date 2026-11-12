// CCountedPointer<CCreatureActionBase>::DeleteData(void* data) -- the counted pointer's per-type deleter: `delete (CCreatureActionBase*)data`,
// null-guarded, through the virtual destructor (vtable slot 0, flag 1). Was the free stand-in DeleteData(AutoTinyVirtualObject*).
// Leaf: ?DeleteData@?$CCountedPointer@VCCreatureActionBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00668610: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` (static __fastcall).
// Name (agentNF32, 2026-11-12): FableWin.pdb 0x01D7A6C0 ?DeleteData@?$CCountedPointer@VCCreatureActionBase@@@@SIXPAX@Z (Anniversary
//   publics carry the same member); the landed CCountedPointer<CCreatureActionBase>(CCreatureActionBase*) 0x00695880 stores this address
//   as the count block's deleter and binds this symbol (rebuild/oracles/reloc_bindings.tsv); engine/CCountedPointer.h DeleteData.
//   Other landed binders still spell it as free stand-ins (Destroy_00668610, DeleteCreatureAction, DeleteAction, CountedDestroy).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CCreatureActionBase>::DeleteData(void*);
