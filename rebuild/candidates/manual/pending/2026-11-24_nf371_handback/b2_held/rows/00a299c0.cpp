// CCountedPointer::Reset(T* data) -- (CCountedPointer<C3DMesh2>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VC3DMesh2@@@@QAEXPAVC3DMesh2@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A299C0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00A281E0.
// Names (NF371, 2026-11-24; was the stand-in CCountedPointer_00a281e0::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_00a281e0@@QAEXPAX@Z, and the deleter bound as ?Destroy_00a281e0@@YIXPAX@Z): FableWin.pdb
//   ?Reset@?$CCountedPointer@VC3DMesh2@@@@QAEXPAVC3DMesh2@@@Z; the body stores CCountedPointer<C3DMesh2>::DeleteData 0x00A281E0
//   (?DeleteData@?$CCountedPointer@VC3DMesh2@@@@SIXPAX@Z, landed nf366) as the count block's DeleteFunc, and the Reset is called twice by
//   C3DMeshLODInfo::BuildMesh 0x00A24520 with a fresh C3DMesh2 (RTTI 0x0129DEF0); Ego_r C3DMeshLODInfo::Mesh (+0x1C) is CCountedPointer<C3DMesh2>.
// Form: a strong explicit specialization of engine/CCountedPointer.h's member with the landed body (ReleaseData written out: its trailing Data store
//   is dead; Data = data; for a non-null data the count block re-reads Data through `this`; a failed new leaves Info 0).
//   FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (landed opt-in: Reset declaration only) keeps the in-class body out, so the specialization is
//   emitted, as a plain function (COMDAT selection 1) like the landed stand-in; the sibling explicit-instantiation form adds a pick-any ReleaseData
//   COMDAT (measured, NOTES). FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (NF222) leaves the stored deleter an external reference to its owner
//   row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class C3DMesh2;

template <>
void CCountedPointer<C3DMesh2>::Reset(C3DMesh2* data)
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
