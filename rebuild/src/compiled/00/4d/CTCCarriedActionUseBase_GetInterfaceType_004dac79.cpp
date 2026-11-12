// CTCCarriedActionUseBase::GetInterfaceType() const -- the component's interface type, TCI_CARRIED_ON_ACTION_USE (0xE)
// Leaf: ?GetInterfaceType@CTCCarriedActionUseBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAC79, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCCarriedActionUseAbility's vtable 0x01240B1C (the slot holds this body in 19 vtable(s), owner CTCCarriedActionUseBase); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCCarriedActionUseBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCCarriedActionUseBase::vfunc_21 (rebuild/src/compiled/00/4d/CTCCarriedActionUseBase_vfunc21_004dac79.cpp).
// No shared header for CTCCarriedActionUseBase: minimal local definition on its RTTI base CTCActionUseBase.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCActionUseBase.h"
class CTCCarriedActionUseBase : public CTCActionUseBase {
public:
    virtual ETCInterfaceType GetInterfaceType() const;              // slot 21
};
ETCInterfaceType CTCCarriedActionUseBase::GetInterfaceType() const
{
    return TCI_CARRIED_ON_ACTION_USE;
}
