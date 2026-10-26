// CCreatureAction_SpiritAttack::CCreatureAction_SpiritAttack(const CCreatureAction_SpiritAttack& other) -- the member-wise copy constructor (reached from Clone): the CCreatureActionBase copy, the own vtable, then the two intelligent pointers PTarget and PParticleEmitter re-attached to the source's pointees
// Leaf: ??0CCreatureAction_SpiritAttack@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855260, 105 bytes (+ int3 padding to 112), thiscall, `ret 4`, returns this.
// Identity (proven): CCreatureAction_SpiritAttack::CCreatureAction_SpiritAttack(const CCreatureAction_SpiritAttack&); evidence: the vtable it stores at +0 (0x01275D74) has the RTTI .?AVCCreatureAction_SpiritAttack@@; the PDB name sets hold ??0CCreatureAction_SpiritAttack@@QAE@ABV0@@Z; its only caller 0x00855230 is the class's Clone (scalar new of the object, then this constructor); Ego_r.pdb CCreatureAction_SpiritAttack ends with PTarget (CIntelligentPointer<CThing>) and PParticleEmitter (CIntelligentPointer<CThing>), whose vtables 0x01238C6C / 0x01238C6C it stores after each pointer's base constructor; the base copy is CCreatureActionBase 0x0062EBC0.
// Label: the manifest's "CCreatureAction_BattleChargeImpact" is propagated across byte-identical bodies (family H017: seven creature-action copy
//   constructors with the same shape; retail has no identical-code folding, so each address is one class's constructor).
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90;
//   vtables CCreatureAction_SpiritAttack 0x01275D74, CIntelligentPointer<CThing> 0x01238C6C, CIntelligentPointer<CThing> 0x01238C6C.
// Layout: retail CCreatureActionBase is 0xA8 bytes (12-byte containers), so PTarget sits at +0xA8 and PParticleEmitter at +0xB0
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

class CCreatureAction_SpiritAttack : public CCreatureActionBase {
public:
    CCreatureAction_SpiritAttack(const CCreatureAction_SpiritAttack& other);
    virtual ~CCreatureAction_SpiritAttack();
    CIntelligentPointer<CThing> PTarget;          // +0xA8
    CIntelligentPointer<CThing> PParticleEmitter;          // +0xB0
};
CCreatureAction_SpiritAttack::CCreatureAction_SpiritAttack(const CCreatureAction_SpiritAttack& other)
    : CCreatureActionBase(other), PTarget(other.PTarget), PParticleEmitter(other.PParticleEmitter)
{
}