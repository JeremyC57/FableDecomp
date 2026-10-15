// CTCStealableItemLocation::CTCStealableItemLocation(CThing* thing) -- CTCBase(thing), own vtable, RadiusToBeWithin = 1.0, RadiusToTakeItemsBackTo = 0.0
// Leaf: ??0CTCStealableItemLocation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4F94, length 34 bytes. Size-optimised TU: `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (landed; the manifest's "CEngineSubPrimitive2DClockSprite" label is propagated).
// Identity: the stored vtable 0x0123C65C is CTCStealableItemLocation (rtti-vtables.tsv); the manifest's
// "CTCCameraPointTrack::CTCCameraPointTrack" label is propagated and rejected (CTCCameraPointTrack is 0x54 bytes).
// Layout: Ego_r.pdb CTCStealableItemLocation (0x14 bytes): RadiusToBeWithin +0xc, RadiusToTakeItemsBackTo +0x10.
// engine/CTCBase.h is a layout-only POD (`void* __vftable`), so the polymorphic base is modelled as CTCBase_Calls
// (as the landed CTCRockTrollShield constructor 0x007AEE10).
// Shape: the pass-through `push dword ptr [esp+8]` marks the size TU; both floats are init-list members
// (fld1 / fldz), the first scheduled before the vtable store.
#pragma optimize("s", on)

struct CThing;

struct CTCBase_Calls {                              // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    CTCBase_Calls(CThing* thing);                   // 0x00686800
};

struct CTCStealableItemLocation : CTCBase_Calls {   // vtable 0x0123C65C
    float RadiusToBeWithin;                         // +0x0c
    float RadiusToTakeItemsBackTo;                  // +0x10
    virtual void v00();
    CTCStealableItemLocation(CThing* thing);
};

CTCStealableItemLocation::CTCStealableItemLocation(CThing* thing)
    : CTCBase_Calls(thing)
    , RadiusToBeWithin(1.0f)
    , RadiusToTakeItemsBackTo(0.0f)
{
}