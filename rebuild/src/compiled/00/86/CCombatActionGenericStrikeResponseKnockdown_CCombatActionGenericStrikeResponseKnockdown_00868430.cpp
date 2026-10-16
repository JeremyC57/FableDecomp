// CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown&) -- copy constructor: base CCombatActionBase(other), own vtable, the animation name, the hitter pointer, hit point/direction with their flags, FlipFacing
// Leaf: ??0CCombatAction_GenericStrikeResponseKnockdown
// Retail: Fable.exe (TLC, VC7.1) @ 0x00868430, 208 bytes incl. padding (`ret 4`). Own vtable 0x01277344.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CCharString copy ctor @ 0x0099EC30,
//          CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90.
// Layout (retail = Ego_r.pdb - 0x18; the class is byte-packed in the PDB, engine/CCombatAction_GenericStrikeResponseKnockdown.h
//          carries the PDB offsets with the base flattened, so it is modelled locally): AnimName +0x114, PThingThatHit
//          +0x118, PointOfHit +0x120, GotPointOfHit +0x12c, DirectionOfHit +0x12d, GotDirectionOfHit +0x139, FlipFacing +0x13a.
// Shape: memberwise copy in declaration order; the packed vector words copy unaligned.
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
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);  // 0x0099EC30
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
    CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown& other);
};
#pragma pack(pop)

CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown& other)
    : CCombatActionBase(other)
    , AnimName(other.AnimName)
    , PThingThatHit(other.PThingThatHit)
    , PointOfHit(other.PointOfHit)
    , GotPointOfHit(other.GotPointOfHit)
    , DirectionOfHit(other.DirectionOfHit)
    , GotDirectionOfHit(other.GotDirectionOfHit)
    , FlipFacing(other.FlipFacing)
{
}