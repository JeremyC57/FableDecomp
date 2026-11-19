// NEntityEvents::CIsHitEventHitByMelee::operator()(const CEventHitBase*) const -- true when the hit event's parameters are a melee strike
// Leaf: ??RCIsHitEventHitByMelee@NEntityEvents@@QBE_NPBVCEventHitBase@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00810F80, thiscall, `ret 4`: mov eax,[esp+4] ; mov ecx,[eax+8] ; call 0x00868860 ; ret 4.
// Identity (proven, NF93 q11440): FableWin.pdb names this filter's operator() ??RCIsHitEventHitByMelee@NEntityEvents@@QBE_NPBVCEventHitBase@1@@Z
//   (0x008070A0); retail lays the hit filters out in one run -- CIsHitEventBlocked 0x00810F50, (0x00810F60), CIsHitEventHitByShot 0x00810F70
//   (calls CHitParameters::IsShotStrike 0x008688C0, see that landed row), then this body, which calls CHitParameters::IsMeleeStrike
//   0x00868860 on the event's PHitParams; the landed CTCEntityEvents::GetEvent binder 0x007AFB90 (CFilter_Bind3<CIsHitEventHitByMelee, ...>)
//   binds this spelling. Before: a `Cons_val` stand-in.
// Layout: Ego_r.pdb NEntityEvents::CEventHitBase { CEventBase (vptr, WorldFrame); CCountedPointer<CHitParameters> PHitParams +0x08 },
//   CCountedPointer's raw pointer at +0 (engine/NEntityEvents_CEventHitBase.h, engine/CCountedPointer.h).
// Callees: CHitParameters::IsMeleeStrike 0x00868860 (?IsMeleeStrike@CHitParameters@@QBE_NXZ).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/NEntityEvents_CEventHitBase.h"

bool NEntityEvents::CIsHitEventHitByMelee::operator()(const CEventHitBase* event) const
{
    return event->PHitParams->IsMeleeStrike();
}
