// CTCStealableItemLocation::CTCStealableItemLocation(CThing* thing) -- CTCBase(thing), own vtable, RadiusToBeWithin = 1.0, RadiusToTakeItemsBackTo = 0.0
// Leaf: ??0CTCStealableItemLocation@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCStealableItemLocation@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4F94, length 34 bytes. Size-optimised TU: `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (landed; the manifest's "CEngineSubPrimitive2DClockSprite" label is propagated).
// Identity: the stored vtable 0x0123C65C is CTCStealableItemLocation (rtti-vtables.tsv); the manifest's
// "CTCCameraPointTrack::CTCCameraPointTrack" label is propagated and rejected (CTCCameraPointTrack is 0x54 bytes).
// Layout: Ego_r.pdb CTCStealableItemLocation (0x14 bytes): RadiusToBeWithin +0xc, RadiusToTakeItemsBackTo +0x10.
// engine/CTCBase.h is a layout-only POD (`void* __vftable`), so the polymorphic base is modelled as CTCBase_Calls
// (as the landed CTCRockTrollShield constructor 0x007AEE10).
// Shape: the pass-through `push dword ptr [esp+8]` marks the size TU; both floats are init-list members
// (fld1 / fldz), the first scheduled before the vtable store.
#pragma optimize("s", on)

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CTCStealableItemLocation : public CTCBase { public:   // vtable 0x0123C65C
    float RadiusToBeWithin;                         // +0x0c
    float RadiusToTakeItemsBackTo;                  // +0x10
    CTCStealableItemLocation(CThing& thing);
};

CTCStealableItemLocation::CTCStealableItemLocation(CThing& thing)
    : CTCBase(thing)
    , RadiusToBeWithin(1.0f)
    , RadiusToTakeItemsBackTo(0.0f)
{
}