// CTCRockTrollShield::CTCRockTrollShield(CThing&) -- CTCBase(thing), own vtable, health = 5, rechargeCounter = 0
// Leaf: ??0CTCRockTrollShield@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCRockTrollShield@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AEE10, length 39 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (CBaseClassNonCopyable(), ParentThing, the two
// flags cleared, vtable 0x0125BE0C = CTCBase; the manifest labels it "CEngineSubPrimitive2DClockSprite",
// a propagated name). Vtable 0x0126D41C = CTCRockTrollShield (rtti-vtables.tsv); the manifest's
// "CComponentContainer::CComponentContainer" label is propagated.
// Layout = Ego_r.pdb CTCRockTrollShield (20 bytes): CTCBase 0xc {vptr, ParentThing +4, ToBeRemoved +8,
// IsInGlobalUpdateTCs +9}, health +0xc, rechargeCounter +0x10. The base constructor runs first, then
// the vtable is installed and the two init-list scalars are stored (a shield of 5 hits, no recharge
// in progress). engine/CTCBase.h holds the layout only, so the base is modelled as `CTCBase_Calls`.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

struct CTCRockTrollShield : CTCBase {
    long health;                                            // +0x0c
    long rechargeCounter;                                   // +0x10
    CTCRockTrollShield(CThing& thing);
};

CTCRockTrollShield::CTCRockTrollShield(CThing& thing)
    : CTCBase(thing)
    , health(5)
    , rechargeCounter(0)
{
}