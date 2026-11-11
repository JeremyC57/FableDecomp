// NEntityEvents::CEventReceivedItemInInventory::GetType() const -- the event type of the received-item event (0x28)
// Leaf: ?GetType@CEventReceivedItemInInventory@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BC3E3, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of NEntityEvents::CEventReceivedItemInInventory's vtable 0x01253C8C (the slot holds this body in 1 vtable(s), owner NEntityEvents::CEventReceivedItemInInventory); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetType@CEventReceivedItemInInventory@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
// Was: NEntityEvents::CEventReceivedItemInInventory::vfunc_1 (rebuild/src/compiled/00/5b/NEntityEventsCEventReceivedItemInInventory_vfunc1_005bc3e3.cpp).
// No shared header for NEntityEvents::CEventReceivedItemInInventory: minimal local definition on its RTTI base NEntityEvents::CEventBase.
#pragma optimize("s", on)
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
namespace NEntityEvents {
class CEventReceivedItemInInventory : public CEventBase {
public:
    virtual EEventType GetType() const;                  // slot 1
};
EEventType CEventReceivedItemInInventory::GetType() const
{
    return (EEventType)0x28;
}
}
