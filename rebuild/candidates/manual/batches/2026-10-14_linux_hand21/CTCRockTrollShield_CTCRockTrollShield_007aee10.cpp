// CTCRockTrollShield::CTCRockTrollShield(CThing*) -- CTCBase(thing), own vtable, health = 5, rechargeCounter = 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AEE10, length 39 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (CBaseClassNonCopyable(), ParentThing, the two
// flags cleared, vtable 0x0125BE0C = CTCBase; the manifest labels it "CEngineSubPrimitive2DClockSprite",
// a propagated name). Vtable 0x0126D41C = CTCRockTrollShield (rtti-vtables.tsv); the manifest's
// "CComponentContainer::CComponentContainer" label is propagated.
// Layout = Ego_r.pdb CTCRockTrollShield (20 bytes): CTCBase 0xc {vptr, ParentThing +4, ToBeRemoved +8,
// IsInGlobalUpdateTCs +9}, health +0xc, rechargeCounter +0x10. The base constructor runs first, then
// the vtable is installed and the two init-list scalars are stored (a shield of 5 hits, no recharge
// in progress). engine/CTCBase.h holds the layout only, so the base is modelled as `CTCBase_Calls`.
struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CTCRockTrollShield : CTCBase_Calls {
    long health;                                            // +0x0c
    long rechargeCounter;                                   // +0x10
    virtual void v00();                                     // own vtable 0x0126D41C
    CTCRockTrollShield(CThing* thing);
};

CTCRockTrollShield::CTCRockTrollShield(CThing* thing)
    : CTCBase_Calls(thing)
    , health(5)
    , rechargeCounter(0)
{
}
