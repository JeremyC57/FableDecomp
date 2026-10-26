// CCreatureAction_InterruptableMidAttackAutoTurn::IsToCauseRecoil() const -- true only for a melee flourish whose ability def says the melee ability causes recoil
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C850, length 48 bytes. Vtable slot 46 of
// CCreatureAction_InterruptableMidAttackAutoTurn (0x012778CC).
// Callees: the action's own virtual slot 21 (+0x54: IsMeleeFlourish, the CCombatActionBase vtable names it).
// Shape: a bool result (`mov al,1` / `xor al,al` exits); PMeleeAbilityDef (+0xb8, Ego_r.pdb name, PDB +0xc4) is read as the
// raw pointer, no counted-pointer copy; CauseRecoil is the def's PDB member at +0x6d (retail +0x65: the def's
// CDefString and vector members are 4 bytes narrower in retail than in the PDB).
class CMeleeCombatAbilityDef { public:             // retail layout: the PDB's +0x6c/+0x6d sit 8 bytes lower (its CDefString/vector are wider)
    unsigned char _pad[0x64];
    unsigned char EvadeAllHits;             // +0x64 (bool)
    unsigned char CauseRecoil;              // +0x65 (bool)
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (PDB 0x12c)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual bool IsMeleeFlourish() const;   // slot 21 (+0x54), 0x008565C0 in the base vtable
    unsigned char _pad04[0xb8 - 4];         // +0x04
    CMeleeCombatAbilityDef* PMeleeAbilityDef;   // +0xb8 (CCountedPointer; the raw pointer is read)
    unsigned char _padbc[0x114 - 0xbc];     // +0xbc
};

class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public:
    virtual bool IsToCauseRecoil() const;
};

bool CCreatureAction_InterruptableMidAttackAutoTurn::IsToCauseRecoil() const
{
    if (IsMeleeFlourish() && PMeleeAbilityDef->CauseRecoil)
        return true;
    return false;
}