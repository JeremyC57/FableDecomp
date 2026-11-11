// NEntityEvents::CEventBase::OnAdd(CTCEntityEvents const &) -- does nothing
// Leaf: ?OnAdd@CEventBase@NEntityEvents@@UAEXABVCTCEntityEvents@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A4FC, thiscall, `ret 4`.
// Identity: slot 2 of NEntityEvents::CEventVillagerInteraction's vtable 0x012995A8 (defined here: the slot holds this body in 35 vtable(s), all of NEntityEvents::CEventBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts OnAdd(CTCEntityEvents const &) at this slot; Ego_r/FableWin name set: ?OnAdd@CEventBase@NEntityEvents@@UAEXABVCTCEntityEvents@@@Z; `ret` matches the signature.
// Header: engine/NEntityEvents_CEventHitBase.h with the overlay opt-in FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS.
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
void NEntityEvents::CEventBase::OnAdd(const CTCEntityEvents&)
{
}
