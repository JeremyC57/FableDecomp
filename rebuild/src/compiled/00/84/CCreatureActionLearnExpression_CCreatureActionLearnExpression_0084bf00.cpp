// CCreatureAction_LearnExpression::CCreatureAction_LearnExpression(const CCreatureAction_LearnExpression& other) -- the member-wise copy constructor (reached from Clone): the CCreatureActionBase copy, the own vtable, then the two intelligent pointers PPlayerCreature and PLearnFromThing re-attached to the source's pointees
// Leaf: ??0CCreatureAction_LearnExpression@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084BF00, 105 bytes (+ int3 padding to 112), thiscall, `ret 4`, returns this.
// Identity (proven): CCreatureAction_LearnExpression::CCreatureAction_LearnExpression(const CCreatureAction_LearnExpression&); evidence: the vtable it stores at +0 (0x0127461C) has the RTTI .?AVCCreatureAction_LearnExpression@@; the PDB name sets hold ??0CCreatureAction_LearnExpression@@QAE@ABV0@@Z; its only caller 0x0084BED0 is the class's Clone (scalar new of the object, then this constructor); Ego_r.pdb CCreatureAction_LearnExpression ends with PPlayerCreature (CIntelligentPointer<CThingPlayerCreature>) and PLearnFromThing (CIntelligentPointer<CThing>), whose vtables 0x01231C4C / 0x01238C6C it stores after each pointer's base constructor; the base copy is CCreatureActionBase 0x0062EBC0.
// Label: the manifest's "CCreatureAction_BattleChargeImpact" is propagated across byte-identical bodies (family H017: seven creature-action copy
//   constructors with the same shape; retail has no identical-code folding, so each address is one class's constructor).
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90;
//   vtables CCreatureAction_LearnExpression 0x0127461C, CIntelligentPointer<CThingPlayerCreature> 0x01231C4C, CIntelligentPointer<CThing> 0x01238C6C.
// Layout: retail CCreatureActionBase is 0xA8 bytes (12-byte containers), so PPlayerCreature sits at +0xA8 and PLearnFromThing at +0xB0
//   (Ego_r.pdb member names and types).
// Shape: the compiler's member-wise copy (the landed CCombatActionBase copy constructor 0x0062F3B0 pattern): each
//   CIntelligentPointer<T> copy is its base constructor, the template's vtable store and SetPItem(other.GetPItem()).
class CBaseObject;
class CThing;
class CThingPlayerCreature;
class CBaseIntelligentPointer {                     // 8 bytes: vptr, the shared pointee record
public:
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
    void* PRecord;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other) { SetPItem(other.GetPItem()); }
};
class CCreatureActionBase {                         // 0xA8 bytes in retail (vptr first)
public:
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
    virtual ~CCreatureActionBase();
    unsigned char _body[0xA8 - 4];
};

class CCreatureAction_LearnExpression : public CCreatureActionBase {
public:
    CCreatureAction_LearnExpression(const CCreatureAction_LearnExpression& other);
    virtual ~CCreatureAction_LearnExpression();
    CIntelligentPointer<CThingPlayerCreature> PPlayerCreature;          // +0xA8
    CIntelligentPointer<CThing> PLearnFromThing;          // +0xB0
};
CCreatureAction_LearnExpression::CCreatureAction_LearnExpression(const CCreatureAction_LearnExpression& other)
    : CCreatureActionBase(other), PPlayerCreature(other.PPlayerCreature), PLearnFromThing(other.PLearnFromThing)
{
}