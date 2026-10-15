// CTCDeadShotInThingTracker::CTCDeadShotInThingTracker(CThing* parent) -- CTCBase(parent), own vtable, null PThingStuckIn, HasThingStuckIn = false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E3450, length 56 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10;
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Vtable 0x0127040C = CTCDeadShotInThingTracker, 0x01237F1C = CIntelligentPointer (rtti-vtables.tsv).
// PDB CTCDeadShotInThingTracker (0x18 bytes): PThingStuckIn (CIntelligentPointer<const CThing>) +0x0c,
// HasThingStuckIn +0x14.
// Shape: speed TU (argument loaded and pushed); the derived vtable store sits between `lea edi` and the
// member's base constructor; the CIntelligentPointer(item = 0) constructor is inlined (base ctor, its
// vtable, SetPItem(0)); the bool is stored after the member constructor (declaration order).
struct CThing;
struct CBaseObject;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(CBaseObject* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CTCDeadShotInThingTracker : CTCBase_Calls {
    CIntelligentPointer PThingStuckIn;                      // +0x0c CIntelligentPointer<const CThing>
    bool                HasThingStuckIn;                    // +0x14
    virtual void v00();                                     // own vtable 0x0127040C
    CTCDeadShotInThingTracker(CThing* parent);
};

CTCDeadShotInThingTracker::CTCDeadShotInThingTracker(CThing* parent)
    : CTCBase_Calls(parent)
    , PThingStuckIn()
    , HasThingStuckIn(false)
{
}
