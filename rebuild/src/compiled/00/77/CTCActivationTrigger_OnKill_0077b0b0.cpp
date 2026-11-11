// CTCActivationTrigger::OnKill() -- detaches the trigger from its receptor
// Leaf: ?OnKill@CTCActivationTrigger@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077B0B0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of CTCActivationTrigger's vtable 0x0123BBAC (the slot holds this body in 1 vtable(s), owner CTCActivationTrigger); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCActivationTrigger@@UAEXXZ; `ret` matches the signature.
// Was: CTCActivationTrigger::vfunc_14 (rebuild/src/compiled/00/77/CTCActivationTrigger_OnKill_0077b0b0.cpp).
// No shared header for CTCActivationTrigger: minimal local definition on its RTTI base CTCBase (Ego_r.pdb members PTCReceptor +0x0C,
// ReceptorUID +0x10, Activated +0x18).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCACTIVATIONRECEPTORBASE_REMOVETRIGGER
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CTCActivationReceptorBase.h"
class CTCActivationTrigger : public CTCBase {
public:
    virtual void OnKill();                                   // slot 14
    CTCActivationReceptorBase* PTCReceptor;                  // +0x0C
    unsigned __int64 ReceptorUID;                            // +0x10
    bool Activated;                                          // +0x18
};
void CTCActivationTrigger::OnKill()
{
    if (PTCReceptor) {
        PTCReceptor->RemoveTrigger(*this);
        PTCReceptor = 0;
    }
}
