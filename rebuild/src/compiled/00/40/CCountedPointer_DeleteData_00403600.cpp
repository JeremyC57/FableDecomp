// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CAProgressDisplay>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCAProgressDisplay@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403600, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00403600`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00403600, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCAProgressDisplay@@@@SIXPAX@Z; stored by the landed CCountedPointer<CAProgressDisplay>(T*) ctor 0x00403D20 (leaf ??0?$CCountedPointer@VCAProgressDisplay@@@@QAE@PAVCAProgressDisplay@@@Z) and its Reset 0x004A98C0.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CAPROGRESSDISPLAY
#include "engine/CAProgressDisplay.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CAProgressDisplay>::DeleteData(void*);
