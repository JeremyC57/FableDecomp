// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CGameJoystickManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCGameJoystickManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049BC20, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0049BC20`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0049bc20, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCGameJoystickManager@@@@SIXPAX@Z; stored by CCountedPointer<CGameJoystickManager>::Reset 0x0049BCE0 (reloc_bindings, caller 0x0049BB90).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CGAMEJOYSTICKMANAGER_VIRTUAL_DTOR
#include "engine/CGameJoystickManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CGameJoystickManager>::DeleteData(void*);
