// CCountedPointer<CMeshDataBankEntry>::DeleteData(void* data) -- the counted pointer's per-type deleter: null-guarded virtual destroy
//   (vtable slot 0, flag arg 1), i.e. `delete (T*)data` through the scalar deleting destructor.
// Leaf: ?DeleteData@?$CCountedPointer@VCMeshDataBankEntry@@@@SIXPAX@Z
// __fastcall pointer=ecx; the callee is the object's own slot 0.
// Names (NF366, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog sub_00a28370): FableWin.pdb
//   ?DeleteData@?$CCountedPointer@VCMeshDataBankEntry@@@@SIXPAX@Z (static __fastcall void(void*)); retail stores this address as the count
//   block's DeleteFunc (`mov dword ptr [eax+4], 0x00A28370`) in CCountedPointer<CMeshDataBankEntry>::Reset 0x00A29D20, called by
//   CMeshDataBank::NotifyEntryAdded 0x00A26D40 (+0x2B0) with a fresh C3DAnimationInfo (RTTI vtable 0x0129CD64, base CMeshDataBankEntry);
//   FableWin.pdb also names Reset(CMeshDataBankEntry*), and Ego_r.pdb types CMeshDataBank::DynamicInfoBuffer as
//   CArray<CCountedPointer<CMeshDataBankEntry>>.
// Respelled in place in the 0x00428B10 form: an explicit specialization of engine/CCountedPointer.h's member (a strong definition, as the
//   landed extern "C" function was) with the landed body over its local slot-0 model; T is only declared by its shared header.
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the
//   header's in-class body VC7.1 emits no symbol for the specialization (NF360 measurement).
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

#include "engine/CMeshDataBankEntry.h"

struct Obj { virtual void Destroy(int flags); };

template <>
void FABLE_FASTCALL CCountedPointer<CMeshDataBankEntry>::DeleteData(void* data)
{
    if (data)
        static_cast<Obj*>(data)->Destroy(1);
}
