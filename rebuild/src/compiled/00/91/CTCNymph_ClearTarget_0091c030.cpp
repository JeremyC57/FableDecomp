// CTCNymph::ClearTarget() -- clear PTarget: `PTarget = CIntelligentPointer<CThingCreatureBase>(0)` through a converting frame temporary
// Leaf: ?ClearTarget@CTCNymph@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091C030, 66 bytes, thiscall, plain `ret`.
// Identity (probable): no xrefs; FableWin.pdb ?ClearTarget@CTCNymph@@QAEXXZ; Ego_r.pdb CTCNymph::PTarget is CIntelligentPointer<CThingCreatureBase> at +0x6C, i.e. retail +0x68 (CanAttack/ReadyToAttack PDB +0x5C/+0x5D = retail +0x58/+0x59, the flags the landed neighbour 0x0091C080 clears before the same PTarget reset); it sits among the Nymph AI state group code (0x0091C130 NymphFloatAround::OnReplace).
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
class CTCNymph {                                         // Ego_r.pdb 116 bytes; retail 4 below the PDB after FlightSpline (12-byte vector)
public:
    void ClearTarget();
    unsigned char _body[0x68];                           // vptr .. TotalFadeTime (not touched here)
    CIntelligentPointer<CThingCreatureBase> PTarget;     // +0x68 (PDB +0x6C)
};

void CTCNymph::ClearTarget()
{
    PTarget = CIntelligentPointer<CThingCreatureBase>(0);
}