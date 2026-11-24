// CCountedPointer<NUISystem::CComponent>::DeleteData(void* data) -- the counted pointer's per-type deleter: null-guarded virtual destroy
//   (vtable slot 0, flag arg 1), i.e. `delete (CComponent*)data` through the scalar deleting destructor.
// Leaf: ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z
// __fastcall pointer=ecx; the callee is the object's own slot 0.
// Names (NF360, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog CCountedPointer<NUISystem::CComponent>::DeleteData):
//   FableWin.pdb ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z (static __fastcall void(void*)); retail stores this
//   address as the count block's DeleteFunc (`mov dword ptr [eax+4], 0x00428B10`) in Reset<NUISystem::CComponent> 0x00429C6F and in the
//   CCountedPointer<NUISystem::CComponent>(CComponent*) constructor 0x00429C15; the reloc_bindings line witnessed by 0x005331A0 binds that
//   symbol here. Respelled in place: an explicit specialization of engine/CCountedPointer.h's member (a strong definition, as the landed
//   extern "C" function was) with the landed body over its local slot-0 model; the header only forward-declares NUISystem::CComponent.
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the header's
//   in-class body instead, VC7.1 emitted no symbol for this specialization (NF360 measurement).
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

namespace NUISystem { class CComponent; }

struct Obj { virtual void Destroy(int flags); };

template <>
void FABLE_FASTCALL CCountedPointer<NUISystem::CComponent>::DeleteData(void* data)
{
    if (data)
        static_cast<Obj*>(data)->Destroy(1);
}
