// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CVillageTaskSecuritySweep>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCVillageTaskSecuritySweep@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065D2B1, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0065D2B1`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0065d2b1, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCVillageTaskSecuritySweep@@@@SIXPAX@Z; stored by the CCountedPointer(T*) ctor 0x0065E2E0, called at 0x00659C17 with a fresh CVillageTaskSecuritySweep (ctor 0x0065DCE8, RTTI vtable .?AVCVillageTaskSecuritySweep@@).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP
#include "engine/CVillageTaskSecuritySweep.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CVillageTaskSecuritySweep>::DeleteData(void*);
