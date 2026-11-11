// CTCActivationReceptorBase::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCActivationReceptorBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE64A, thiscall, `ret`.
// Identity: slot 23 of CTCActivationReceptorBase's vtable 0x01271F7C (defined here: the slot holds this body in 5 vtable(s), all of CTCActivationReceptorBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCActivationReceptorBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCActivationReceptorBase.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_CTCACTIVATIONRECEPTORBASE_VSLOTS
#include "engine/CTCActivationReceptorBase.h"
bool CTCActivationReceptorBase::IsComponentSerialisable() const
{
    return true;
}
