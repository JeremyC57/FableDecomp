// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CMap>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCMap@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050C210, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0050C210`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0050c210, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCMap@@@@SIXPAX@Z; stored by CCountedPointer<CMap>::Reset 0x0050FAA0 (reloc_bindings, caller 0x004FF080); Ego_r.pdb CMapInfo::PMap is CCountedPointer<CMap>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CMap.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CMap>::DeleteData(void*);
