// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CAIBrain>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCAIBrain@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00834A10, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00834A10`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00834a10, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCAIBrain@@@@SIXPAX@Z; stored by Reset 0x00834E40 (reloc_bindings stand-in ?Reset@CBrainPointer@@QAEXPAVCAIBrain@@@Z, caller 0x00833010), called at 0x008330A5 with a fresh CAIBrain (ctor 0x0088C160, RTTI vtable .?AVCAIBrain@@); Ego_r.pdb CThingAICreature::PBrain is CCountedPointer<CAIBrain>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#include "engine/CAIBrain.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CAIBrain>::DeleteData(void*);
