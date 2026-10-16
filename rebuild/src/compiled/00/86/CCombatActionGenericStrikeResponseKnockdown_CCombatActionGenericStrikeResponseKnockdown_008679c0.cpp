// CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase*, const CThing*, const C3DVector&, const C3DVector&) -- knockdown response: base (owner, action code 250, empty sound pair), empty animation name, the hitter, the hit point and direction (both flagged as given)
// Leaf: ??0CCombatAction_GenericStrikeResponseKnockdown
// Retail: Fable.exe (TLC, VC7.1) @ 0x008679C0, 176 bytes incl. padding (`ret 10h`). Own vtable 0x01277344.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCharString::CCharString()
//          @ 0x0099E4B0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (the inline CIntelligentPointer(T*) ctor).
// Layout (retail = Ego_r.pdb - 0x18; the class is byte-packed in the PDB, engine/CCombatAction_GenericStrikeResponseKnockdown.h
//          carries the PDB offsets with the base flattened, so it is modelled locally): AnimName +0x114, PThingThatHit
//          +0x118, PointOfHit +0x120, GotPointOfHit +0x12c, DirectionOfHit +0x12d, GotDirectionOfHit +0x139, FlipFacing +0x13a.
// Shape: the empty CSoundPair temporary is zeroed in the frame first; the members are initialised in declaration order,
//          the two Got* flags share one `mov cl,1`; FlipFacing is left unset.
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
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};
struct CCharString {
    void* PStringData;
    CCharString();                          // 0x0099E4B0
};
#pragma pack(push, 1)
struct CCombatAction_GenericStrikeResponseKnockdown : CCombatActionBase {   // vtable 0x01277344; retail sizeof 0x13c
    CCharString AnimName;                   // +0x114
    CIntelligentPointer PThingThatHit;      // +0x118
    C3DVector PointOfHit;                   // +0x120
    bool GotPointOfHit;                     // +0x12c
    C3DVector DirectionOfHit;               // +0x12d
    bool GotDirectionOfHit;                 // +0x139
    bool FlipFacing;                        // +0x13a
    virtual void v00();
    CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* owner, CThing* thingThatHit, const C3DVector& pointOfHit,
                                                 const C3DVector& directionOfHit);
};
#pragma pack(pop)

CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* owner, CThing* thingThatHit,
                                                                                           const C3DVector& pointOfHit,
                                                                                           const C3DVector& directionOfHit)
    : CCombatActionBase(owner, 250, CSoundPair())
    , PThingThatHit(thingThatHit)
    , PointOfHit(pointOfHit)
    , GotPointOfHit(true)
    , DirectionOfHit(directionOfHit)
    , GotDirectionOfHit(true)
{
}