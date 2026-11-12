// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CInputManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCInputManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A6BD0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009A6BD0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009a6bd0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCInputManager@@@@SIXPAX@Z; stored by Reset 0x009A7180 (reloc_bindings stand-in ?Reset@CPInput@@QAEXPAVCInputManagerDX@@@Z, caller 0x009A6610), called at 0x009A6855 with a fresh CInputManagerDX (ctor 0x00A60050, RTTI .?AVCInputManagerDX@@ : CInputManager); Ego_r.pdb CSystemManager::PInputManager is CCountedPointer<CInputManager>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CInputManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CInputManager>::DeleteData(void*);
