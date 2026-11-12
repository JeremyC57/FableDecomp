// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CVillageTaskPatrol>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCVillageTaskPatrol@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065D258, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0065D258`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0065d258, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCVillageTaskPatrol@@@@SIXPAX@Z; stored by the CCountedPointer<CVillageTaskPatrol>(T*) ctor 0x0065E198 (reloc_bindings ??0?$CCountedPointer@VCVillageTaskPatrol@@@@QAE@PAVCVillageTaskPatrol@@@Z, caller 0x0065B854).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKPATROL
#include "engine/CVillageTaskPatrol.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CVillageTaskPatrol>::DeleteData(void*);
