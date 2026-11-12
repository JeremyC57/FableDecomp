// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NParticleEngine::CAEnvironmentInteraction>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00419086, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00419086`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00419086, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@SIXPAX@Z; stored by the CCountedPointer<NParticleEngine::CAEnvironmentInteraction>(T*) ctor 0x0041956C (reloc_bindings ??0?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@QAE@PAV...@Z, caller 0x004174F1).
// Model (agentFAM11, 2026-11-12): `this` is the shared engine/CAEnvironmentInteraction.h (polymorphic, virtual destructor in slot 0); no local model.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CAEnvironmentInteraction.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NParticleEngine::CAEnvironmentInteraction>::DeleteData(void*);
