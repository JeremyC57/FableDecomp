// CTCHitOnCollision::OnRemove() -- removing the component also removes the parent's solid-to-thing component (TCI_SOLID_TO_THING)
// Leaf: ?OnRemove@CTCHitOnCollision@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C81A0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 2 of CTCHitOnCollision's vtable 0x0126E7A4 (the slot holds this body in 1 vtable(s), owner CTCHitOnCollision); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnRemove(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnRemove@CTCHitOnCollision@@UAEXXZ; `ret` matches the signature.
// Was: CTCHitOnCollision::vfunc_2 (rebuild/src/compiled/00/7c/CTCHitOnCollision_vfunc2_007c81a0.cpp).
// No shared header for CTCHitOnCollision: minimal local definition on its RTTI base CTCBase.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
class CTCHitOnCollision : public CTCBase {
public:
    virtual void OnRemove();                                 // slot 2
};
void CTCHitOnCollision::OnRemove()
{
    ParentThing->RemoveTC(TCI_SOLID_TO_THING);
}
