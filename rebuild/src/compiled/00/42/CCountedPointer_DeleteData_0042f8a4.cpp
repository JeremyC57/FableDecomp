// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CASoundBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCASoundBank@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F8A4, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0042F8A4`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0042f8a4, the behavioural extern "C" DeleteIfSet): Ego_r.pdb CNewFrontendGameComponent::PSampleBank (+0x50) is CCountedPointer<CASoundBank>; stored by the landed CCountedPointer<CASoundBank>::Reset 0x0042FD6F (leaf ?Reset@?$CCountedPointer@VCASoundBank@@..., caller 0x0042E17C), called at 0x0042E1AF with a fresh CSoundBankLH (ctor 0x00A0BAC0, RTTI .?AVCSoundBankLH@@ : CASoundBank). Neither PDB name set lists this DeleteData instantiation (Ego_r keeps only 86, FableWin 343); the spelling follows the class template.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CASoundBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CASoundBank>::DeleteData(void*);
