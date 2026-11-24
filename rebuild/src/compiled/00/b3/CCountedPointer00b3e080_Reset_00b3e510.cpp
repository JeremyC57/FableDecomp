// CCountedPointer::Reset(T* data) -- (CCountedPointer<CLoadingMemoryFailureHandler>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@QAEXPAVCLoadingMemoryFailureHandler@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3E510, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00B3E080.
// Names (NF371, 2026-11-24; was the stand-in CCountedPointer_00b3e080::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_00b3e080@@QAEXPAX@Z, and the deleter bound as ?Destroy_00b3e080@@YIXPAX@Z): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@QAEXPAVCLoadingMemoryFailureHandler@@@Z; the body stores
//   CCountedPointer<CLoadingMemoryFailureHandler>::DeleteData 0x00B3E080 (?DeleteData@?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@SIXPAX@Z,
//   landed nf366) as the count block's DeleteFunc, and the Reset is called by CEngineResourceManager::AllocatePools 0x00B3B240 with a fresh
//   CLoadingMemoryFailureHandler (ctor 0x009D5EB0, RTTI 0x0129B8C4); Ego_r CEngineResourceManager::LoadingMemoryFailureHandler is
//   CCountedPointer<CLoadingMemoryFailureHandler>.
// Form: a strong explicit specialization of engine/CCountedPointer.h's member with the landed body (ReleaseData written out: its trailing Data store
//   is dead; Data = data; for a non-null data the count block re-reads Data through `this`; a failed new leaves Info 0).
//   FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (landed opt-in: Reset declaration only) keeps the in-class body out, so the specialization is
//   emitted, as a plain function (COMDAT selection 1) like the landed stand-in; the sibling explicit-instantiation form adds a pick-any ReleaseData
//   COMDAT (measured, NOTES). FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (NF222) leaves the stored deleter an external reference to its owner
//   row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CLoadingMemoryFailureHandler;

template <>
void CCountedPointer<CLoadingMemoryFailureHandler>::Reset(CLoadingMemoryFailureHandler* data)
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Info = 0;
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);   // re-read through `this` (retail loads [esi] for the block), not the argument
}
