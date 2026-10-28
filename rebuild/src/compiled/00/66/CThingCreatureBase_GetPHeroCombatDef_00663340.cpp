// CThingCreatureBase::GetPHeroCombatDef() const -- the creature definition's hero combat def, fetched from the thing's definition manager when the creature def names one (else a null def pointer)
// Leaf: ?GetPHeroCombatDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663340, 82 bytes, thiscall, result through the hidden pointer, `ret 4`.
// Identity (proven): Ego_r.pdb donor order IsCombatAnimationValid 0x0053E1F0 -> GetPHeroCombatDef 0x0053E2A0 -> ClearCurrentAction
//   0x0053E2D0 matches retail IsCombatAnimationValid 0x006632A0 (landed) -> this row -> ClearCurrentAction 0x006633A0 (landed); the body
//   reads CThingCreatureDef::HeroCombatDef (PDB +0x134, retail +0x130) through PDef (retail +0xe0). The catalog label
//   "CEventKilledCreature::CEventKilledCreature" is wrong. reloc_bindings carries a non-const CDefPointer<CHeroCombatDef> spelling
//   for this address (Ego_r.pdb has CDefPointer<const CHeroCombatDef>) -> Review at landing.
// Callees: CThing::PeekDefinitionManager() const 0x004C7990 (Ego_r.pdb, const CGameDefinitionManager&),
//   CDefinitionManager::GetDef<CHeroCombatDef>(long, CDefPointer<const CHeroCombatDef>*) const 0x006697E0 (stand-in body below),
//   the def's virtual Destroy (slot 1).
// Shape: as the landed CThingSwitch::GetSwitchDef 0x0083C1E0: a CDefPointer local filled by the manager and returned by value (the
//   copy's add-ref and the local's release share one null test); the def index is read once and tested `> 0` first.
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                  // slot 1
    mutable long m_refCount;                       // +0x04
};
class CHeroCombatDef : public CDefPointeeBase { };
class CThingCreatureDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x130 - 0x08];
    long          HeroCombatDef;                   // +0x130 (PDB +0x134)
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CDefinitionManager {
public:
    template <class T> bool GetDef(long globalIndex, CDefPointer<const T>* out) const;
};
class CGameDefinitionManager : public CDefinitionManager { };
void* FindDef_00663340();                          // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def pointer's
// address does not escape the call (the landed 0x0083C1E0 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<const T>* out) const
{
    void* def = FindDef_00663340();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing {
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
};

class CThingCreatureBase : public CThing {
public:
    unsigned char _pad00[0xe0];
    CDefPointer<const CThingCreatureDef> PDef;     // +0xe0 (PDB +0xfc)
    CDefPointer<const CHeroCombatDef> GetPHeroCombatDef() const;
};

CDefPointer<const CHeroCombatDef> CThingCreatureBase::GetPHeroCombatDef() const
{
    long index = PDef->HeroCombatDef;
    CDefPointer<const CHeroCombatDef> def;
    if (index > 0)
        PeekDefinitionManager().GetDef(index, &def);
    return def;
}