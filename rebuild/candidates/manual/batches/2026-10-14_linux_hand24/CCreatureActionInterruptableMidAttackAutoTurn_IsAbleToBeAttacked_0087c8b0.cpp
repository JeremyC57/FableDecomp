// CCreatureAction_InterruptableMidAttackAutoTurn::IsAbleToBeAttacked() const -- false while the attack evades all hits or causes recoil; otherwise true only when the action has no melee ability def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C8B0, length 80 bytes. Vtable slot 47 of CCreatureAction_InterruptableMidAttackAutoTurn (0x012778CC).
// Callees: the action's own virtual slots 45 (+0xb4, IsToEvadeAllHits @ 0x0087C880) and 46 (+0xb8, IsToCauseRecoil @ 0x0087C850),
// then the ability def's virtual slot 1 (+4, Delete) when the temporary counted pointer drops the last reference.
// Shape: PMeleeAbilityDef (+0xb8, Ego_r.pdb name) is an INTRUSIVE counted pointer (count at +4 behind the pointee's vtable);
// the landed by-value getter idiom (0x00856EA0): copy (`test ; je ; inc [p+4]`), the null test taken on the raw pointer
// while the copy lives (`setne bl`), release (`dec [p+4] ; jne ; call [vt+4]`), then the inverted result (`test bl,bl ; sete al`).
// Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so `this` is modelled locally.
struct CMeleeCombatAbilityDef {
    virtual void v00();
    virtual void Delete();                  // slot 1 (+4)
    long m_references;                      // +0x04
};

struct CCountedAbilityPointer {             // CCountedPointer<CMeleeCombatAbilityDef>, intrusive count
    CMeleeCombatAbilityDef* m_pItem;
    CCountedAbilityPointer(const CCountedAbilityPointer& other) : m_pItem(other.m_pItem) { if (m_pItem != 0) ++m_pItem->m_references; }
    ~CCountedAbilityPointer() { if (m_pItem != 0) { if (--m_pItem->m_references == 0) m_pItem->Delete(); } }
    CMeleeCombatAbilityDef* GetPItem() const { return m_pItem; }
};

struct CCombatActionBase {                  // retail sizeof 0x114 (PDB 0x12c)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44();
    virtual bool IsToEvadeAllHits() const;  // slot 45 (+0xb4)
    virtual bool IsToCauseRecoil() const;   // slot 46 (+0xb8)
    unsigned char _pad04[0xb8 - 4];         // +0x04
    CCountedAbilityPointer PMeleeAbilityDef;    // +0xb8
    unsigned char _padbc[0x114 - 0xbc];     // +0xbc
    CCountedAbilityPointer GetPMeleeAbilityDef() const { return PMeleeAbilityDef; }
};

struct CCreatureAction_InterruptableMidAttackAutoTurn : CCombatActionBase {
    bool IsAbleToBeAttacked() const;
};

bool CCreatureAction_InterruptableMidAttackAutoTurn::IsAbleToBeAttacked() const
{
    if (IsToEvadeAllHits() || IsToCauseRecoil())
        return false;
    bool hasAbility = GetPMeleeAbilityDef().GetPItem() != 0;
    return hasAbility ? false : true;
}
