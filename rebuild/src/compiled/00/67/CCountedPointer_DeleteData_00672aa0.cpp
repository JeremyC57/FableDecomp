// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CCreatureStatsDef>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCCreatureStatsDef@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00672AA0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00672AA0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00672aa0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCCreatureStatsDef@@@@SIXPAX@Z; stored by CCountedPointer<CCreatureStatsDef>::Reset 0x00673500 (reloc_bindings, caller 0x006765A0); Ego_r.pdb CCreatureDef::PStats is CCountedPointer<CCreatureStatsDef>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CCREATURESTATSDEF_VIRTUAL_DTOR
#include "engine/CCreatureStatsDef.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CCreatureStatsDef>::DeleteData(void*);
