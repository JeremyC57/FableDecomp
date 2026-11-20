// CCountedPointer<NCameraModes::CScriptModuleBase>::CCountedPointer(T*) -- adopt the object: null-checked count block {1, DeleteData, object}
// Leaf: ??0?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@QAE@PAVCScriptModuleBase@NCameraModes@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ED270, oracle length 78 bytes.
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; push esi ; mov esi,ecx ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; N: xor eax,eax ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; Z: mov dword ptr [esi+4],0 ; mov eax,esi ; pop esi ; ret 4
// Callees: operator new @ 0x00BFEA1A; the block's deleter is CCountedPointer<NCameraModes::CScriptModuleBase>::DeleteData 0x005ECF50.
// Naming (NF157, 2026-11-20; PRIORITY q11649): the per-type deleter 0x005ECF50 stored in the block is the shared engine/CCountedPointer.h
//   static member, FableWin.pdb 0x01DE64C0 ?DeleteData@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@SIXPAX@Z (the self row 0x005ECF50
//   explicitly instantiates it; its body `delete static_cast<T*>(data)` needs the complete class: engine/NCameraModes_CScriptModuleBase.h).
// Naming (NF198, 2026-11-20, q11955): moved off its address-suffixed stand-in (CCountedPointer_005ecf50 with a local SCountBlock) onto the
//   shared engine/CCountedPointer.h in-class (T*) constructor, emitted out of line by explicit instantiation (the 0x007A4210 recipe):
//   FableWin.pdb 0x01DE5A80 ??0?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@QAE@PAVCScriptModuleBase@NCameraModes@@@Z, which all six
//   landed callers (0x005EBFA0 0x0072A090 0x007A9B10 0x00847DF0 0x008924B0 0x008926D0) already bind (NF189 HEAD census). Was the self leaf
//   ??0CCountedPointer_005ecf50@@QAE@PAX@Z.
#include "engine/CCountedPointer.h"
#include "engine/NCameraModes_CScriptModuleBase.h"

template CCountedPointer<NCameraModes::CScriptModuleBase>::CCountedPointer(NCameraModes::CScriptModuleBase*);
