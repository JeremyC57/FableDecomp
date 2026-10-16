// CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase(const CCombatAction_GenericStrikeResponseBase&) -- copy constructor: base CCombatActionBase(other), own vtable, the hitter pointer, the hit point/direction, the dust settings
// Leaf: ??0CCombatAction_GenericStrikeResponseBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F83C0, 224 bytes incl. padding (`ret 4`). Own vtable 0x01264734.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer ctor
//          @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (the inline CIntelligentPointer copy, vtable 0x01237F1C).
// Layout (retail = Ego_r.pdb - 0x18; CCombatActionBase is 0x114 bytes in retail): PThingThatHit +0x114, PointOfHit
//          +0x11c, DirectionOfHit +0x128, MoveOnHitOverride +0x134, CreateDustEffect +0x135, FrameForDust +0x138,
//          DustTravelDirection +0x13c.
// Shape: memberwise copy in declaration order (same form as the landed TrollThrowProjectile copy 0x008C7D80).
struct CThingCreatureBase;
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }                          // leaves the words unset
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy (fixed-offset moves)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);                                                    // 0x0062F3B0
};
struct CCombatAction_GenericStrikeResponseBase : CCombatActionBase {   // vtable 0x01264734; retail sizeof 0x148
    CIntelligentPointer PThingThatHit;      // +0x114
    C3DVector PointOfHit;                   // +0x11c
    C3DVector DirectionOfHit;               // +0x128
    bool MoveOnHitOverride;                 // +0x134
    bool CreateDustEffect;                  // +0x135
    long FrameForDust;                      // +0x138
    C3DVector DustTravelDirection;          // +0x13c
    virtual void v00();
    CCombatAction_GenericStrikeResponseBase(const CCombatAction_GenericStrikeResponseBase& other);
};

CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase(const CCombatAction_GenericStrikeResponseBase& other)
    : CCombatActionBase(other)
    , PThingThatHit(other.PThingThatHit)
    , PointOfHit(other.PointOfHit)
    , DirectionOfHit(other.DirectionOfHit)
    , MoveOnHitOverride(other.MoveOnHitOverride)
    , CreateDustEffect(other.CreateDustEffect)
    , FrameForDust(other.FrameForDust)
    , DustTravelDirection(other.DustTravelDirection)
{
}