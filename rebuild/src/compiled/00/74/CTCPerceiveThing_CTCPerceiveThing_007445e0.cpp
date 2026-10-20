// CTCPerceiveThing::CTCPerceiveThing(CThing* parent) -- CTCBase(parent), the CTCPerceiveThing vtable, an empty perceived-thing intelligent pointer, every perception flag/radius cleared, FramesSinceDirectlyPerceivedThing = -1 and the update offset taken from the parent thing
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- PDef as a CDefPointer record with an inline ctor (the -1 store stays above it) and UpdateOffset assigned in the body (its `and edx,3` sinks below the radius stores)
// Leaf: ??0CTCPerceiveThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x007445E0, 128 bytes. Vtable 0x012688CC (CTCPerceiveThing, RTTI: slot 0 0x007462D0, slot 4
//   OnCreate 0x00744ED0); the member's vtable 0x01237F1C is CIntelligentPointer<CThing const>.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
//   CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Layout (engine/CTCPerceiveThing.h, 0x58 bytes; modelled locally because the header flattens the CIntelligentPointer member
//   to a byte array): PerceivedThing +0x0c, DistanceFromPerceivedThingSquared +0x14, AwarenessTimer +0x18,
//   FramesSinceDirectlyPerceivedThing +0x1c, LastPerceivedPos +0x20 (C3DVector, left uninitialised), the eight flag bytes
//   +0x2c..+0x33, PDef +0x34, UpdateOffset +0x38, SoundRadius .. GiveUpChaseRadius +0x3c..+0x4c, PerceptionCombat +0x50,
//   FrameHitByPerceivedThing +0x54. UpdateOffset = (ParentThing word at +0x18) & 3 spreads the perception updates over 4 frames.
// Shape: init list in declaration order (also tried: body assignments in retail order; same instructions).
//   One shared zero register.
struct CThing {
    unsigned char _pad00[0x18];
    unsigned long m_uidLow;                     // +0x18 (low word of the thing's UID)
};
struct CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
};
struct CDefPointeeBase;
struct CTCBase {
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* parent);                    // 0x00686800
};
struct C3DVector { float X, Y, Z; };
struct CPerceivedThingDef;
template <class T> struct CDefPointer {          // CDefPointer<T const>: the def object pointer with its inline constructor (released by the out-of-line destructor)
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CTCPerceiveThing : CTCBase {
    CIntelligentPointer PerceivedThing;         // +0x0c
    float     DistanceFromPerceivedThingSquared;// +0x14
    float     AwarenessTimer;                   // +0x18
    long      FramesSinceDirectlyPerceivedThing;// +0x1c
    C3DVector LastPerceivedPos;                 // +0x20
    bool      Perceiving;                       // +0x2c
    bool      LineOfSight;                      // +0x2d
    bool      ExtendedLineOfSight;              // +0x2e
    bool      IgnoreFOVLineOfSight;             // +0x2f
    bool      DirectAwareness;                  // +0x30
    bool      Smelt;                            // +0x31
    bool      Heard;                            // +0x32
    bool      SearchingForThingToPerceive;      // +0x33
    CDefPointer<const CPerceivedThingDef> PDef; // +0x34
    long      UpdateOffset;                     // +0x38
    float     SoundRadius;                      // +0x3c
    float     SightRadius;                      // +0x40
    float     ExtendedSightRadius;              // +0x44
    float     SmellRadius;                      // +0x48
    float     GiveUpChaseRadius;                // +0x4c
    bool      PerceptionCombat;                 // +0x50
    long      FrameHitByPerceivedThing;         // +0x54
    virtual ~CTCPerceiveThing();
    CTCPerceiveThing(CThing* parent);
};

CTCPerceiveThing::CTCPerceiveThing(CThing* parent) : CTCBase(parent)
    , DistanceFromPerceivedThingSquared(0), AwarenessTimer(0), FramesSinceDirectlyPerceivedThing(-1)
    , Perceiving(false), LineOfSight(false), ExtendedLineOfSight(false), IgnoreFOVLineOfSight(false), DirectAwareness(false)
    , Smelt(false), Heard(false), SearchingForThingToPerceive(false), PDef()
    , SoundRadius(0), SightRadius(0), ExtendedSightRadius(0), SmellRadius(0), GiveUpChaseRadius(0)
    , PerceptionCombat(false), FrameHitByPerceivedThing(0)
{
    UpdateOffset = ParentThing->m_uidLow & 3;
}