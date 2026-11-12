// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CResourceBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCResourceBank@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991E00, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00991E00`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00991e00, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCResourceBank@@@@SIXPAX@Z; stored by CCountedPointer<CResourceBank>::Reset 0x00992170 (reloc_bindings, caller 0x00991910); Ego_r.pdb CLipSyncDataBank::ResourceBank is CCountedPointer<CResourceBank>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CResourceBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CResourceBank>::DeleteData(void*);
