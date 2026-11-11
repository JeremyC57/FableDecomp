// NEntityEvents::CEventMoralityChanged::GetType() const -- the event type of the morality-changed event (0x23)
// Leaf: ?GetType@CEventMoralityChanged@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A535, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of NEntityEvents::CEventMoralityChanged's vtable 0x0124F2DC (the slot holds this body in 1 vtable(s), owner NEntityEvents::CEventMoralityChanged); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetType@CEventMoralityChanged@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
// Was: NEntityEvents::CEventMoralityChanged::vfunc_1 (rebuild/src/compiled/00/57/NEntityEventsCEventMoralityChanged_vfunc1_0057a535.cpp).
// No shared header for NEntityEvents::CEventMoralityChanged: minimal local definition on its RTTI base NEntityEvents::CEventBase.
#pragma optimize("s", on)
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
namespace NEntityEvents {
class CEventMoralityChanged : public CEventBase {
public:
    virtual EEventType GetType() const;                  // slot 1
};
EEventType CEventMoralityChanged::GetType() const
{
    return (EEventType)0x23;
}
}
