// CCountedPointer<CLoadingMemoryFailureHandler>::DeleteData(void* data) -- the counted pointer's per-type deleter: null-guarded virtual
//   destroy (vtable slot 0, flag arg 1), i.e. `delete (T*)data` through the scalar deleting destructor.
// Leaf: ?DeleteData@?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@SIXPAX@Z
// __fastcall pointer=ecx; the callee is the object's own slot 0.
// Names (NF366, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog sub_00b3e080): FableWin.pdb
//   ?DeleteData@?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@SIXPAX@Z (static __fastcall void(void*)); retail stores this address as
//   the count block's DeleteFunc (`mov dword ptr [eax+4], 0x00B3E080`) in CCountedPointer<CLoadingMemoryFailureHandler>::Reset 0x00B3E510,
//   called by CEngineResourceManager::AllocatePools 0x00B3B240 (+0x247) with a fresh CLoadingMemoryFailureHandler (constructor 0x009D5EB0,
//   RTTI vtable 0x0129B8C4); FableWin.pdb also names Reset(CLoadingMemoryFailureHandler*), and Ego_r.pdb types
//   CEngineResourceManager::LoadingMemoryFailureHandler as that pointer.
// Respelled in place in the 0x00428B10 form: an explicit specialization of engine/CCountedPointer.h's member (a strong definition, as the
//   landed extern "C" function was) with the landed body over its local slot-0 model; T is only forward-declared.
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the
//   header's in-class body VC7.1 emits no symbol for the specialization (NF360 measurement).
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

class CLoadingMemoryFailureHandler;

struct Obj { virtual void Destroy(int flags); };

template <>
void FABLE_FASTCALL CCountedPointer<CLoadingMemoryFailureHandler>::DeleteData(void* data)
{
    if (data)
        static_cast<Obj*>(data)->Destroy(1);
}
