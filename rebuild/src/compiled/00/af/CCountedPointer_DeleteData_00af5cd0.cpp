// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NParticleEngine::CPSCBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCPSCBase@NParticleEngine@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF5CD0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00AF5CD0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00af5cd0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCPSCBase@NParticleEngine@@@@SIXPAX@Z (CCountedPointer is global); stored by Reset 0x00AF6010 (reloc_bindings ?Reset@?$CCountedPointer@VCPSCBase@NParticleEngine@@@NParticleEngine@@..., which puts CCountedPointer in NParticleEngine: a binder spelling, not this row's); Ego_r.pdb NParticleEngine::CComponentInstance::Component (+0x4) is CCountedPointer<NParticleEngine::CPSCBase>.
// Model (agentFAM11, 2026-11-12): `this` is the shared engine/CPSCBase.h (polymorphic, virtual destructor in slot 0); no local model.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CPSCBase.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NParticleEngine::CPSCBase>::DeleteData(void*);
