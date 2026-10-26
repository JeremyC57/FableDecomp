// CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown&) -- copy constructor: base CCombatActionBase(other), own vtable, the animation name, the hitter pointer, hit point/direction with their flags, FlipFacing
// Leaf: ??0CCombatAction_GenericStrikeResponseKnockdown
// Retail: Fable.exe (TLC, VC7.1) @ 0x00868430, 208 bytes incl. padding (`ret 4`). Own vtable 0x01277344.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CCharString copy ctor @ 0x0099EC30,
//          CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90.
// Layout (retail = Ego_r.pdb - 0x18; the class is byte-packed in the PDB, engine/CCombatAction_GenericStrikeResponseKnockdown.h
//          carries the PDB offsets with the base flattened, so it is modelled locally): AnimName +0x114, PThingThatHit
//          +0x118, PointOfHit +0x120, GotPointOfHit +0x12c, DirectionOfHit +0x12d, GotDirectionOfHit +0x139, FlipFacing +0x13a.
// Shape: memberwise copy in declaration order; the packed vector words copy unaligned.
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
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);  // 0x0099EC30
};
#pragma pack(push, 1)
class CCombatAction_GenericStrikeResponseKnockdown : public CCombatActionBase { public:   // vtable 0x01277344; retail sizeof 0x13c
    CCharString AnimName;                   // +0x114
    CIntelligentPointer<const CThing> PThingThatHit;      // +0x118
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