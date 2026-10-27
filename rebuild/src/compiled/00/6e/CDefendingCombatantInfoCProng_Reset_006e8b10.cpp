// CDefendingCombatantInfo::CProng::Reset() -- clear POccupier: `POccupier = CIntelligentPointer<CThingCreatureBase>(0)` through a converting frame temporary
// Leaf: ?Reset@CProng@CDefendingCombatantInfo@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8B10, 66 bytes, thiscall, plain `ret`.
// Identity (probable): no xrefs; FableWin.pdb ?Reset@CProng@CDefendingCombatantInfo@@QAEXXZ is CProng's only no-argument mutator; the landed CCombatRing::Reset 0x006E8B60 runs exactly this body (`POccupier = CIntelligentPointer<CThingCreatureBase>(0)`, vtable 0x0123E7D8, member +0x14) inlined on every 28-byte prong of Prongs; PDB POccupier is CIntelligentPointer<CThingCreatureBase> at +20; neighbours are CCombatRing members.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, SetPItem 0x00A01B90 (twice), GetPItem 0x00A01B50,
//   ~CBaseIntelligentPointer 0x00A01C10; the temporary's CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8.
// Shape (family K015, representative 0x006E8B10): the inline converting constructor (base ctor, vtable store, SetPItem(0)),
//   the inline operator= `SetPItem(other.GetPItem())` into the member, then the base destructor called directly (the
//   derived destructor is implicit) -- the landed CAIStateGroup_EnemyCreatureBase::Cleanup 0x0091D850 / CCombatRing::Reset
//   0x006E8B60 form.
class CThingCreatureBase;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };              // 1-aligned in retail
#pragma pack(pop)
class CDefendingCombatantInfo {
public:
    class CProng {                                       // Ego_r.pdb, 28 bytes
    public:
        void Reset();
        bool Blocked;                                    // +0x00
        C3DVector Prong;                                 // +0x01
        long ProngBlockageTimer;                         // +0x10
        CIntelligentPointer<CThingCreatureBase> POccupier;   // +0x14
    };
};

void CDefendingCombatantInfo::CProng::Reset()
{
    POccupier = CIntelligentPointer<CThingCreatureBase>(0);
}