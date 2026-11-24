// CCountedPointer<const CInputTypeBase>::DeleteData(void* data) -- the counted pointer's per-type deleter: null-guarded virtual destroy
//   (vtable slot 0, flag arg 1), i.e. `delete (T*)data` through the scalar deleting destructor.
// Leaf: ?DeleteData@?$CCountedPointer@$$CBVCInputTypeBase@@@@SIXPAX@Z
// __fastcall pointer=ecx; the callee is the object's own slot 0.
// Names (NF366, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog sub_00447660): FableWin.pdb
//   ?DeleteData@?$CCountedPointer@$$CBVCInputTypeBase@@@@SIXPAX@Z (static __fastcall void(void*)); retail stores this address as the count
//   block's DeleteFunc (`mov dword ptr [eax+4], 0x00447660`) in CCountedPointer<const CInputTypeBase>::Reset 0x00447D90, called at
//   0x00447216 with a fresh CInputTypeXboxPadButtonEvent / LeftStickEvent / RightStickEvent (RTTI vtables 0x012317A8 / 0x012317CC /
//   0x012317F0, each deriving CInputTypeBase); FableWin.pdb also names Reset(const CInputTypeBase*), and Ego_r.pdb types
//   CGamePlayerInterface::Controls / *Buttons as CCountedPointer<const CInputTypeBase>.
// Respelled in place in the 0x00428B10 form: an explicit specialization of engine/CCountedPointer.h's member (a strong definition, as the
//   landed extern "C" function was) with the landed body over its local slot-0 model; T is only forward-declared.
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the
//   header's in-class body VC7.1 emits no symbol for the specialization (NF360 measurement).
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

class CInputTypeBase;

struct Obj { virtual void Destroy(int flags); };

template <>
void FABLE_FASTCALL CCountedPointer<const CInputTypeBase>::DeleteData(void* data)
{
    if (data)
        static_cast<Obj*>(data)->Destroy(1);
}
