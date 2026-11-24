// CCountedPointer<C3DMesh2>::DeleteData(void* data) -- the counted pointer's per-type deleter: null-guarded virtual destroy (vtable slot 0,
//   flag arg 1), i.e. `delete (T*)data` through the scalar deleting destructor.
// Leaf: ?DeleteData@?$CCountedPointer@VC3DMesh2@@@@SIXPAX@Z
// __fastcall pointer=ecx; the callee is the object's own slot 0.
// Names (NF366, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog sub_00a281e0): FableWin.pdb
//   ?DeleteData@?$CCountedPointer@VC3DMesh2@@@@SIXPAX@Z (static __fastcall void(void*)); retail stores this address as the count block's
//   DeleteFunc (`mov dword ptr [eax+4], 0x00A281E0`) in CCountedPointer<C3DMesh2>::Reset 0x00A299C0, called twice by
//   C3DMeshLODInfo::BuildMesh 0x00A24520 with a fresh C3DMesh2 (constructor 0x00A89190, RTTI vtable 0x0129DEF0); FableWin.pdb also names
//   Reset(C3DMesh2*), the reloc_bindings line witnessed by 0x00A24520 binds that Reset at 0x00A299C0, and Ego_r.pdb types
//   C3DMeshLODInfo::Mesh as CCountedPointer<C3DMesh2>.
// Respelled in place in the 0x00428B10 form: an explicit specialization of engine/CCountedPointer.h's member (a strong definition, as the
//   landed extern "C" function was) with the landed body over its local slot-0 model; T is only declared by its shared header.
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the
//   header's in-class body VC7.1 emits no symbol for the specialization (NF360 measurement).
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

#include "engine/C3DMesh2.h"

struct Obj { virtual void Destroy(int flags); };

template <>
void FABLE_FASTCALL CCountedPointer<C3DMesh2>::DeleteData(void* data)
{
    if (data)
        static_cast<Obj*>(data)->Destroy(1);
}
