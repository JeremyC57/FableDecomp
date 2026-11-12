// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CQuestManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCQuestManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A8220, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x004A8220`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_004a8220, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCQuestManager@@@@SIXPAX@Z; stored by CCountedPointer<CQuestManager>::Reset 0x004A9A10 (reloc_bindings, caller 0x004A6550).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CQUESTMANAGER
#include "engine/CQuestManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CQuestManager>::DeleteData(void*);
