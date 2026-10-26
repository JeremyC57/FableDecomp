// CCombatActionBase::GetDamageMultiplier() const -- the melee ability's damage multiplier, or 1.0 when the action has no melee ability
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856EA0, length 92 bytes.
// Callees: the ability's virtual slot 1 (+4, Delete) when a temporary counted pointer drops the last reference.
// PMeleeAbilityDef (+0xb8, Ego_r.pdb name; +0xc4 in the PDB build) is an INTRUSIVE counted pointer: the pointee
// keeps its reference count at +4 behind its vtable. The getter returns the pointer BY VALUE, so each use copies
// it (`test ; je ; inc [ecx+4]`), reads, and releases (`dec [ecx+4] ; jne ; call [eax+4]`): the null test
// (`setne bl`) is taken on the raw pointer while the copy is alive, and the multiplier (+0x44) is parked in the
// one-dword frame before the second copy is released (`fld [esp+4]` returns it). 1.0f is the pool constant
// @ 0x0122DED8. Retail's CCombatActionBase is 0x114 bytes (PDB 0x12c), so the layout is modelled locally.
class CMeleeCombatAbilityDef { public:
    virtual void v00();
    virtual void Delete();                      // slot 1 (+4)
    long m_references;                          // +0x04
    unsigned char _pad08[0x44 - 8];             // +0x08
    float DamageMultiplier;                     // +0x44
};

struct CCountedAbilityPointer {                 // CCountedPointer<CMeleeCombatAbilityDef>, intrusive count
    CMeleeCombatAbilityDef* m_pItem;
    CCountedAbilityPointer(const CCountedAbilityPointer& other) : m_pItem(other.m_pItem) { if (m_pItem != 0) ++m_pItem->m_references; }
    ~CCountedAbilityPointer() { if (m_pItem != 0) { if (--m_pItem->m_references == 0) m_pItem->Delete(); } }
    CMeleeCombatAbilityDef* GetPItem() const { return m_pItem; }
    CMeleeCombatAbilityDef* operator->() const { return m_pItem; }
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[0xb8 - 4];             // +0x04
    CCountedAbilityPointer PMeleeAbilityDef;    // +0xb8
    unsigned char _padbc[0x114 - 0xbc];         // +0xbc
    CCountedAbilityPointer GetPMeleeAbilityDef() const { return PMeleeAbilityDef; }
    float GetDamageMultiplier() const;
};

float CCombatActionBase::GetDamageMultiplier() const
{
    if (GetPMeleeAbilityDef().GetPItem() != 0)
        return GetPMeleeAbilityDef()->DamageMultiplier;
    return 1.0f;
}