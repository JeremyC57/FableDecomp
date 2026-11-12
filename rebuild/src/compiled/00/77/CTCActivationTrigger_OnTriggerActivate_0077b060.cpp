// CTCActivationTrigger::OnTriggerActivate(bool) -- records whether the trigger is activated
// Leaf: ?OnTriggerActivate@CTCActivationTrigger@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077B060, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCActivationTrigger's vtable 0x0123BBAC (the slot holds this body in 1 vtable(s), owner CTCActivationTrigger); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnTriggerActivate(bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnTriggerActivate@CTCActivationTrigger@@UAEX_N@Z; `ret` matches the signature.
// Was: CTCActivationTrigger::vfunc_28 (rebuild/src/compiled/00/77/gapscan_sub0077b060_0077b060.cpp).
// No shared header for CTCActivationTrigger: minimal local definition on its RTTI base CTCBase (Ego_r.pdb members PTCReceptor +0x0C,
// ReceptorUID +0x10, Activated +0x18).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCActivationReceptorBase;
class CTCActivationTrigger : public CTCBase {
public:
    virtual void OnTriggerActivate(bool);                    // slot 28
    CTCActivationReceptorBase* PTCReceptor;                  // +0x0C
    unsigned __int64 ReceptorUID;                            // +0x10
    bool Activated;                                          // +0x18
};
void CTCActivationTrigger::OnTriggerActivate(bool activated)
{
    Activated = activated;
}
