// CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing(const CCreatureAction_GiveItemToThing& other) -- the member-wise copy constructor (reached from Clone): the CCreatureActionBase copy, the own vtable, then the two intelligent pointers PReceivingThing and PItemBeingGiven re-attached to the source's pointees
// Leaf: ??0CCreatureAction_GiveItemToThing@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062EB50, 105 bytes (+ int3 padding to 112), thiscall, `ret 4`, returns this.
// Identity (proven): CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing(const CCreatureAction_GiveItemToThing&); evidence: the vtable it stores at +0 (0x0125919C) has the RTTI .?AVCCreatureAction_GiveItemToThing@@; the PDB name sets hold ??0CCreatureAction_GiveItemToThing@@QAE@ABV0@@Z; its only caller 0x0062EB20 is the class's Clone (scalar new of the object, then this constructor); Ego_r.pdb CCreatureAction_GiveItemToThing ends with PReceivingThing (CIntelligentPointer<CThing>) and PItemBeingGiven (CIntelligentPointer<CThing>), whose vtables 0x01238C6C / 0x01238C6C it stores after each pointer's base constructor; the base copy is CCreatureActionBase 0x0062EBC0.
// Label: the manifest's "CCreatureAction_BattleChargeImpact" is propagated across byte-identical bodies (family H017: seven creature-action copy
//   constructors with the same shape; retail has no identical-code folding, so each address is one class's constructor).
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90;
//   vtables CCreatureAction_GiveItemToThing 0x0125919C, CIntelligentPointer<CThing> 0x01238C6C, CIntelligentPointer<CThing> 0x01238C6C.
// Layout: retail CCreatureActionBase is 0xA8 bytes (12-byte containers), so PReceivingThing sits at +0xA8 and PItemBeingGiven at +0xB0
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

class CCreatureAction_GiveItemToThing : public CCreatureActionBase {
public:
    CCreatureAction_GiveItemToThing(const CCreatureAction_GiveItemToThing& other);
    virtual ~CCreatureAction_GiveItemToThing();
    CIntelligentPointer<CThing> PReceivingThing;          // +0xA8
    CIntelligentPointer<CThing> PItemBeingGiven;          // +0xB0
};
CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing(const CCreatureAction_GiveItemToThing& other)
    : CCreatureActionBase(other), PReceivingThing(other.PReceivingThing), PItemBeingGiven(other.PItemBeingGiven)
{
}