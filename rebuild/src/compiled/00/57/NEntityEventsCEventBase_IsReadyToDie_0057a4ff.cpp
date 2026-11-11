// NEntityEvents::CEventBase::IsReadyToDie() const -- returns true
// Leaf: ?IsReadyToDie@CEventBase@NEntityEvents@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A4FF, thiscall, `ret`.
// Identity: slot 4 of NEntityEvents::CEventVillagerInteraction's vtable 0x012995A8 (defined here: the slot holds this body in 35 vtable(s), all of NEntityEvents::CEventBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts IsReadyToDie(void) at this slot; Ego_r/FableWin name set: ?IsReadyToDie@CEventBase@NEntityEvents@@UBE_NXZ; `ret` matches the signature.
// Header: engine/NEntityEvents_CEventHitBase.h with the overlay opt-in FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS.
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
bool NEntityEvents::CEventBase::IsReadyToDie() const
{
    return true;
}
