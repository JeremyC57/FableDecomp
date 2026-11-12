// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CUserProfileManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCUserProfileManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040D830, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0040D830`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0040d830, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCUserProfileManager@@@@SIXPAX@Z; stored by Reset 0x0040F1B0 (reloc_bindings stand-in ??4CCountedUserProfileManager@@QAEXPAVCUserProfileManager@@@Z, caller 0x0040D2A0), called at 0x0040D2C7 with a fresh CUserProfileManager (ctor 0x0040CEC0, RTTI vtable .?AVCUserProfileManager@@).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VIRTUAL_DTOR
#include "engine/CUserProfileManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CUserProfileManager>::DeleteData(void*);
