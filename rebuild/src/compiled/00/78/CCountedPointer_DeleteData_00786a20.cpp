// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<C3DSkeletalMorph>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DSkeletalMorph@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00786A20, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00786A20`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00786a20, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VC3DSkeletalMorph@@@@SIXPAX@Z; stored by CCountedPointer<C3DSkeletalMorph>::Reset 0x00786BF0 (reloc_bindings, caller 0x007865E0); Ego_r.pdb CTCSkeletalMorph::PSkeletalMorph is CCountedPointer<C3DSkeletalMorph>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class C3DSkeletalMorph {
public:
    virtual ~C3DSkeletalMorph();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<C3DSkeletalMorph>::DeleteData(void*);
