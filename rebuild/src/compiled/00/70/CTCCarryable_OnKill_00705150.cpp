// CTCCarryable::OnKill() -- when the carryable is held, the carrier's CTCCarrying component drops it
// Leaf: ?OnKill@CTCCarryable@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00705150, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of CTCCarryable's vtable 0x0123B35C (the slot holds this body in 1 vtable(s), owner CTCCarryable); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCCarryable@@UAEXXZ; `ret` matches the signature.
// Was: CTCCarryable::vfunc_14 (rebuild/src/compiled/00/70/CTCCarryable_OnKill_00705150.cpp).
#define FABLE_RETAIL_CTCCARRYABLE_VSLOTS3
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCCarrying.h"
void CTCCarryable::OnKill()
{
    if (PCarryingTC)
        PCarryingTC->RemoveThing(*ParentThing);
}
