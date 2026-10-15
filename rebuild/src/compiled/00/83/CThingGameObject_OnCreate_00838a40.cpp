// CThingGameObject::OnCreate() -- run CThingPhysical::OnCreate; on success clear PThingStandingOn (assign a null intelligent pointer) and the four combo/hero-stat override bits, return true
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00838A40, length 94 bytes (+ int3 padding).
// Callees: CThingPhysical::OnCreate @ 0x006A4D20, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// ::SetPItem @ 0x00A01B90, ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10;
// CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: local model. Retail PThingStandingOn (CIntelligentPointer<CThing>, 8 bytes) at +0xC0 and the override bit
// byte at +0xC8 (PDB +0xD8 / +0xE0 less 0x18; engine/CThingGameObject.h still places PThingStandingOn at +0xD8 while
// its retail `flags` byte is +0xC8, so the pointer member is modelled locally).
// Shape: `PThingStandingOn = CIntelligentPointer<CThing>(0);` -- the temporary's inline constructor stores the derived
// vtable after the out-of-line base constructor, its compiler-generated destructor drops the vtable reset and calls the
// base destructor; the inline operator= is `SetPItem(other.GetPItem())`; the four one-bit clears merge into
// `and byte ptr [esi+0C8h],0F0h`; the success path is the `if` body (an early `return false` puts its tail first). No EH frame (exceptions off in this TU).
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThingPhysical {
    bool OnCreate();                                // 0x006A4D20
};

struct CThingGameObject : CThingPhysical {
    unsigned char            _pad00[0xc0];
    CIntelligentPointerThing PThingStandingOn;                    // +0xc0
    unsigned char            AddToComboMultiplierOnHit : 1;               // +0xc8 bit 0
    unsigned char            AddToComboMultiplierOnHitOverrideSet : 1;    // +0xc8 bit 1
    unsigned char            GiveHeroStatChangesOnBeingHit : 1;           // +0xc8 bit 2
    unsigned char            GiveHeroStatChangesOnBeingHitOverrideSet : 1;// +0xc8 bit 3
    bool OnCreate();
};

bool CThingGameObject::OnCreate()
{
    if (CThingPhysical::OnCreate()) {
        PThingStandingOn = CIntelligentPointerThing(0);
        AddToComboMultiplierOnHit = 0;
        AddToComboMultiplierOnHitOverrideSet = 0;
        GiveHeroStatChangesOnBeingHit = 0;
        GiveHeroStatChangesOnBeingHitOverrideSet = 0;
        return true;
    }
    return false;
}