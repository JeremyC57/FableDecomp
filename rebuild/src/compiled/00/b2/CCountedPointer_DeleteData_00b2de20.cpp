// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CCompositeSurfaceCache>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCCompositeSurfaceCache@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2DE20, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B2DE20`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b2de20, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCCompositeSurfaceCache@@@@SIXPAX@Z; stored by CCountedPointer<CCompositeSurfaceCache>::Reset 0x00B2E290 (reloc_bindings, caller 0x00B2C460); Ego_r.pdb CCompositeSurfaceInfo::Cache is CCountedPointer<CCompositeSurfaceCache>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CCompositeSurfaceCache {
public:
    virtual ~CCompositeSurfaceCache();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CCompositeSurfaceCache>::DeleteData(void*);
