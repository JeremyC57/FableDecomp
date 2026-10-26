// CCreatureAction_BattleChargeMiss::CCreatureAction_BattleChargeMiss(const CCreatureAction_BattleChargeMiss& other) -- the member-wise copy constructor (reached from Clone): the CCombatActionBase copy, the own vtable, then the two intelligent pointers Left and Right re-attached to the source's pointees
// Leaf: ??0CCreatureAction_BattleChargeMiss@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF190, 105 bytes (+ int3 padding to 112), thiscall, `ret 4`, returns this.
// Identity (proven): CCreatureAction_BattleChargeMiss::CCreatureAction_BattleChargeMiss(const CCreatureAction_BattleChargeMiss&); evidence: the vtable it stores at +0 (0x012847C4) has the RTTI .?AVCCreatureAction_BattleChargeMiss@@; the PDB name sets hold ??0CCreatureAction_BattleChargeMiss@@QAE@ABV0@@Z; its only caller 0x008CF160 is the class's Clone (scalar new of the object, then this constructor); Ego_r.pdb CCreatureAction_BattleChargeMiss ends with Left (CIntelligentPointer<CThing>) and Right (CIntelligentPointer<CThing>), whose vtables 0x01238C6C / 0x01238C6C it stores after each pointer's base constructor; the base copy is CCombatActionBase 0x0062F3B0.
// Label: the manifest's "CCreatureAction_BattleChargeImpact" is propagated across byte-identical bodies (family H017: seven creature-action copy
//   constructors with the same shape; retail has no identical-code folding, so each address is one class's constructor).
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) 0x0062F3B0, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90;
//   vtables CCreatureAction_BattleChargeMiss 0x012847C4, CIntelligentPointer<CThing> 0x01238C6C, CIntelligentPointer<CThing> 0x01238C6C.
// Layout: retail CCombatActionBase is 0x114 bytes (12-byte containers), so Left sits at +0x114 and Right at +0x11C
//   (Ego_r.pdb member names and types).
// Shape: the compiler's member-wise copy (the landed CCombatActionBase copy constructor 0x0062F3B0 pattern): each
//   CIntelligentPointer<T> copy is its base constructor, the template's vtable store and SetPItem(other.GetPItem()).
class CBaseObject;
class CThing;
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
class CCombatActionBase : public CCreatureActionBase {   // 0x114 bytes in retail
public:
    CCombatActionBase(const CCombatActionBase& other);       // 0x0062F3B0
    unsigned char _body[0x114 - 0xA8];
};
class CCreatureAction_BattleChargeMiss : public CCombatActionBase {
public:
    CCreatureAction_BattleChargeMiss(const CCreatureAction_BattleChargeMiss& other);
    virtual ~CCreatureAction_BattleChargeMiss();
    CIntelligentPointer<CThing> Left;          // +0x114
    CIntelligentPointer<CThing> Right;          // +0x11C
};
CCreatureAction_BattleChargeMiss::CCreatureAction_BattleChargeMiss(const CCreatureAction_BattleChargeMiss& other)
    : CCombatActionBase(other), Left(other.Left), Right(other.Right)
{
}