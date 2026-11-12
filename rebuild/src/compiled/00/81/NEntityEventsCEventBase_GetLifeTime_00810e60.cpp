// NEntityEvents::CEventBase::GetLifeTime() const -- an event lives one second: the constant frame rate (CMainGameComponent::ConstantFPS) in world frames
// Leaf: ?GetLifeTime@CEventBase@NEntityEvents@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810E60, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 3 of NEntityEvents::CEventCreatureCharging's vtable 0x012989D4 (the slot holds this body in 25 vtable(s), owner NEntityEvents::CEventBase); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetLifeTime(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetLifeTime@CEventBase@NEntityEvents@@UBEJXZ; `ret` matches the signature.
// Was: NEntityEvents::CEventBase::vfunc_3 (rebuild/src/compiled/00/81/gapscan_sub00810e60_00810e60.cpp).
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CMainGameComponent.h"
namespace NEntityEvents {
long CEventBase::GetLifeTime() const
{
    return CMainGameComponent::GetConstantFPS();
}
}
