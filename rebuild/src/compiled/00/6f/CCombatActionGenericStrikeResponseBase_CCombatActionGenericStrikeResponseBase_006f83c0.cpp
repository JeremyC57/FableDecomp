// CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase(const CCombatAction_GenericStrikeResponseBase&) -- copy constructor: base CCombatActionBase(other), own vtable, the hitter pointer, the hit point/direction, the dust settings
// Leaf: ??0CCombatAction_GenericStrikeResponseBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F83C0, 224 bytes incl. padding (`ret 4`). Own vtable 0x01264734.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer ctor
//          @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (the inline CIntelligentPointer copy, vtable 0x01237F1C).
// Layout (retail = Ego_r.pdb - 0x18; CCombatActionBase is 0x114 bytes in retail): PThingThatHit +0x114, PointOfHit
//          +0x11c, DirectionOfHit +0x128, MoveOnHitOverride +0x134, CreateDustEffect +0x135, FrameForDust +0x138,
//          DustTravelDirection +0x13c.
// Shape: memberwise copy in declaration order (same form as the landed TrollThrowProjectile copy 0x008C7D80).
class CThingCreatureBase;
class CThing;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }                          // leaves the words unset
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy (fixed-offset moves)
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {         // CIntelligentPointer<const CThing>, vtable 0x01237F1C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);                                                    // 0x0062F3B0
};
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase { public:   // vtable 0x01264734; retail sizeof 0x148
    CIntelligentPointer<const CThing> PThingThatHit;      // +0x114
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