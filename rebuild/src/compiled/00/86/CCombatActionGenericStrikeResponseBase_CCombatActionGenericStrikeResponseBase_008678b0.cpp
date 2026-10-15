// CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase(CThingCreatureBase*, const CThing*, const C3DVector&, const C3DVector&) -- constructor
// Leaf: ??0CCombatAction_GenericStrikeResponseBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008678B0, 160 bytes. The manifest labels this body
// CCombatAction_GenericResponseToExplosion, but the vtable it stores (0x01264734) is
// CCombatAction_GenericStrikeResponseBase's (rtti-vtables.tsv), and the members match that class's PDB layout.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
//          CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: base constructor with action code 250 and a zeroed CSoundPair temporary (register-zeroed, inline
// constructor); this class's vtable; PThingThatHit (CIntelligentPointer<const CThing>, vtable 0x01237F1C)
// built inline with SetPItem(thing); PointOfHit / DirectionOfHit copied word by word from the references;
// MoveOnHitOverride and CreateDustEffect both set from one `mov al,1`. FrameForDust / DustTravelDirection
// are left alone. Retail offsets = PDB offsets - 0x18 (retail CCombatActionBase is 0x114 bytes).
struct CThingCreatureBase;
struct CThing;
struct CBaseObject;

struct C3DVector {
    float X, Y, Z;
};

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst(const CThing* item) { SetPItem((CBaseObject*)item); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCombatAction_GenericStrikeResponseBase : CCombatActionBase {   // vtable 0x01264734
    CIntelligentPointer_CThingConst PThingThatHit;  // +0x114
    float PointOfHit_X;                     // +0x11c
    float PointOfHit_Y;                     // +0x120
    float PointOfHit_Z;                     // +0x124
    float DirectionOfHit_X;                 // +0x128
    float DirectionOfHit_Y;                 // +0x12c
    float DirectionOfHit_Z;                 // +0x130
    bool MoveOnHitOverride;                 // +0x134
    bool CreateDustEffect;                  // +0x135
    long FrameForDust;                      // +0x138
    float DustTravelDirection_X;            // +0x13c
    float DustTravelDirection_Y;            // +0x140
    float DustTravelDirection_Z;            // +0x144
    virtual void v00();
    CCombatAction_GenericStrikeResponseBase(CThingCreatureBase* owner, const CThing* thingThatHit,
                                            const C3DVector& pointOfHit, const C3DVector& directionOfHit);
};

CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase(CThingCreatureBase* owner, const CThing* thingThatHit,
                                                                                 const C3DVector& pointOfHit, const C3DVector& directionOfHit)
    : CCombatActionBase(owner, 250, CSoundPair()),
      PThingThatHit(thingThatHit),
      PointOfHit_X(pointOfHit.X), PointOfHit_Y(pointOfHit.Y), PointOfHit_Z(pointOfHit.Z),
      DirectionOfHit_X(directionOfHit.X), DirectionOfHit_Y(directionOfHit.Y), DirectionOfHit_Z(directionOfHit.Z),
      MoveOnHitOverride(true),
      CreateDustEffect(true)
{
}