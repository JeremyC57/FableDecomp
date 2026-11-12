// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NCameraModes::CScriptModuleBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ECF50, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x005ECF50`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_005ecf50, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@SIXPAX@Z; stored by the CCountedPointer<NCameraModes::CScriptModuleBase>(T*) ctor 0x005ED270 and Reset 0x0069C3D0 (reloc_bindings ??0 / ?Reset@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@..., callers 0x008924B0 / 0x0089E190); Ego_r.pdb CGameCameraManager PCurrentScriptModule / PPendingScriptModule are CCountedPointer<NCameraModes::CScriptModuleBase>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();                                   // slot 0 (scalar deleting destructor)
};
}

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NCameraModes::CScriptModuleBase>::DeleteData(void*);
