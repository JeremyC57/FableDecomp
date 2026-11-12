// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CTCCreatureModeManager::CModeBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007180B0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x007180B0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_007180b0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@SIXPAX@Z; stored by the CCountedPointer<CTCCreatureModeManager::CModeBase>(T*) ctor 0x00718A00 (reloc_bindings ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAV...@Z, landed callers 0x008C1900 / 0x008C1E80; engine/CCountedPointer.h CALLED_CTOR note) and by CTCCreatureModeManager::AllocateMode 0x0073A8E0; Ego_r.pdb CTCCreatureModeManager::PModes is list<CCountedPointer<CModeBase> >.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/ETCInterfaceType.h"
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCCreatureModeManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CTCCreatureModeManager::CModeBase>::DeleteData(void*);
