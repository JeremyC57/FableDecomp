// CCountedPointer<CRenderTarget>::DeleteData(void* data) -- the counted pointer's per-type deleter: null-guarded virtual destroy (vtable
//   slot 0, flag arg 1), i.e. `delete (T*)data` through the scalar deleting destructor.
// Leaf: ?DeleteData@?$CCountedPointer@VCRenderTarget@@@@SIXPAX@Z
// __fastcall pointer=ecx; the callee is the object's own slot 0.
// Names (NF366, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog sub_00f38920): FableWin.pdb
//   ?DeleteData@?$CCountedPointer@VCRenderTarget@@@@SIXPAX@Z (static __fastcall void(void*)); retail stores this address as the count
//   block's DeleteFunc (`mov dword ptr [eax+4], 0x00F38920`) in CCountedPointer<CRenderTarget>::Reset 0x00F39CA0, called by
//   CSurfaceCompositor::CSurfaceCompositor 0x00F38990 with a fresh CRenderTarget (RTTI vtable 0x0129C800); FableWin.pdb also names
//   Reset(CRenderTarget*), and Ego_r.pdb types CSurfaceCompositor::RenderTarget as CCountedPointer<CRenderTarget>.
// Respelled in place in the 0x00428B10 form: an explicit specialization of engine/CCountedPointer.h's member (a strong definition, as the
//   landed extern "C" function was) with the landed body over its local slot-0 model; T is only forward-declared.
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the
//   header's in-class body VC7.1 emits no symbol for the specialization (NF360 measurement).
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

class CRenderTarget;

struct Obj { virtual void Destroy(int flags); };

template <>
void FABLE_FASTCALL CCountedPointer<CRenderTarget>::DeleteData(void* data)
{
    if (data)
        static_cast<Obj*>(data)->Destroy(1);
}
