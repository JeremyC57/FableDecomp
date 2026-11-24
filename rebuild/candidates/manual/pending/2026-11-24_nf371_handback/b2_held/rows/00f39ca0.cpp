// CCountedPointer::Reset(T* data) -- (CCountedPointer<CRenderTarget>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCRenderTarget@@@@QAEXPAVCRenderTarget@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F39CA0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00F38920.
// Names (NF371, 2026-11-24; was the stand-in CCountedPointer_00f38920::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_00f38920@@QAEXPAX@Z, and the deleter bound as ?Destroy_00f38920@@YIXPAX@Z): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCRenderTarget@@@@QAEXPAVCRenderTarget@@@Z; the body stores CCountedPointer<CRenderTarget>::DeleteData 0x00F38920
//   (?DeleteData@?$CCountedPointer@VCRenderTarget@@@@SIXPAX@Z, landed nf366) as the count block's DeleteFunc, and the Reset is called by
//   CSurfaceCompositor::CSurfaceCompositor 0x00F38990 with a fresh CRenderTarget (RTTI 0x0129C800); Ego_r CSurfaceCompositor::RenderTarget is
//   CCountedPointer<CRenderTarget>.
// Form: a strong explicit specialization of engine/CCountedPointer.h's member with the landed body (ReleaseData written out: its trailing Data store
//   is dead; Data = data; for a non-null data the count block re-reads Data through `this`; a failed new leaves Info 0).
//   FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (landed opt-in: Reset declaration only) keeps the in-class body out, so the specialization is
//   emitted, as a plain function (COMDAT selection 1) like the landed stand-in; the sibling explicit-instantiation form adds a pick-any ReleaseData
//   COMDAT (measured, NOTES). FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (NF222) leaves the stored deleter an external reference to its owner
//   row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CRenderTarget;

template <>
void CCountedPointer<CRenderTarget>::Reset(CRenderTarget* data)
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
