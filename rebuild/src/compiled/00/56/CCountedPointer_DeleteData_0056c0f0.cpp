// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NCameraModes::CBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCBase@NCameraModes@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056C0F0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0056C0F0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0056c0f0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCBase@NCameraModes@@@@SIXPAX@Z; stored by CCountedPointer<NCameraModes::CBase>(T*) ctor 0x0056C880 and Reset 0x0069C2F0 (reloc_bindings ??0 / ?Reset@?$CCountedPointer@VCBase@NCameraModes@@@@..., callers 0x00698D10 / 0x00698840); Ego_r.pdb CGameCameraManager::CurrentMode / PendingMode are CCountedPointer<NCameraModes::CBase>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/NCameraModes_CBase.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NCameraModes::CBase>::DeleteData(void*);
