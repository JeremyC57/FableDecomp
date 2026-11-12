// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CSkeletalMorphResourceManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCSkeletalMorphResourceManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A8260, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x004A8260`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_004a8260, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCSkeletalMorphResourceManager@@@@SIXPAX@Z; stored by CCountedPointer<CSkeletalMorphResourceManager>::Reset 0x004A9A80 (reloc_bindings ?Reset@?$CCountedPointer@VCSkeletalMorphResourceManager@@@@QAEXPAV...@Z, caller 0x004A2E90).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CSkeletalMorphResourceManager {
public:
    virtual ~CSkeletalMorphResourceManager();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CSkeletalMorphResourceManager>::DeleteData(void*);
