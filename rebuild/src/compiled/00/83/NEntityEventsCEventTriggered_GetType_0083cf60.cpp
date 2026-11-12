// NEntityEvents::CEventTriggered::GetType() const -- the event type of a trigger event (7)
// Leaf: ?GetType@CEventTriggered@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083CF60, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of NEntityEvents::CEventTriggered's vtable 0x01273014 (the slot holds this body in 1 vtable(s), owner NEntityEvents::CEventTriggered); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetType@CEventTriggered@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
// Was: NEntityEvents::CEventTriggered::vfunc_1 (rebuild/src/compiled/00/83/gapscan_sub0083cf60_0083cf60.cpp).
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventTriggered.h"
namespace NEntityEvents {
EEventType CEventTriggered::GetType() const
{
    return (EEventType)7;
}
}
