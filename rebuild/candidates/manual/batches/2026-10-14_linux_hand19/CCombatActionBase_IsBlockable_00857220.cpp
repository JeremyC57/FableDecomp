// CCombatActionBase::IsBlockable() const -- false when the creature's flag 0x10 (+0x30) is set, else true only when the action has no melee ability
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00857220, length 34 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8).
// PMeleeAbilityDef (+0xb8, Ego_r.pdb name) is tested as the raw pointer (`sete al`), no counted-pointer copy
// (compare the by-value getter of 0x00856EA0). Protected virtual (`MBE` in the decorated name); bool result:
// `xor al,al` false path, `test eax,eax ; sete al` for the named bool (a direct `return p == 0;` zeroes eax
// first and is 2 bytes longer). Layout modelled locally (retail base sizeof 0x114).
struct CMeleeCombatAbilityDef;

struct CThingCreatureBase_Retail {              // engine/CThingCreatureBase.h exists but is not used: only the flags dword at +0x30 is read (retail offset)
    unsigned char _pad[0x30];
    unsigned long Flags30;                      // +0x30 (bit 0x10: cannot be blocked)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase_Retail* GetPItem() const;       // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xb8 - 0x10];          // +0x10
    CMeleeCombatAbilityDef* PMeleeAbilityDef;   // +0xb8 (CCountedPointer, raw pointer tested)
    unsigned char _padbc[0x114 - 0xbc];         // +0xbc
    bool IsBlockable() const;
};

bool CCombatActionBase::IsBlockable() const
{
    if (PCreature.GetPItem()->Flags30 & 0x10)
        return false;
    bool blockable = (PMeleeAbilityDef == 0);     // named bool: `test eax,eax ; sete al` without the eax zeroing
    return blockable;
}
