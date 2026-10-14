// CCombatActionBase::IsDecapitatingAttack() const -- the melee ability can decapitate; a berserk creature (flag 0x10) additionally asks its CTCBerserk interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00857CB0, length 92 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, `add ecx,8` in place), CVectorMap
// <ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (a second copy of 0x0040F020; the interface map at +0x44
// of the thing, key 0x84 = CTCBerserk, inlined Find: lower_bound then end() unless the key matches), CTCBerserk::
// ShouldAttackDecapitate @ 0x0078BFC0 (its bool result is returned as is).
// PMeleeAbilityDef (+0xb8) is dereferenced raw (no counted-pointer copy); its CanDecapitate byte sits at +0x5c.
// Shape: `if (ability && ability->CanDecapitate) { if (flags & 0x10) return Berserk()->Should(); return true; }
// return false;` -- Find/GetBerserkInterface need __forceinline here (VC leaves the plain inline out of line
// in this body, unlike 0x00866FB0); the one-dword frame holds the enum->long conversion temporary for LowerBound's reference.
// Layout modelled locally (retail CCombatActionBase sizeof 0x114, PDB 0x12c).
struct CTCBase;
enum ETCInterfaceType { TC_INTERFACE_BERSERK = 0x84 };

struct CInterfacePair {
    ETCInterfaceType first;
    CTCBase* second;
};

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    CInterfacePair* m_pFirst;                   // +0x00
    CInterfacePair* m_pLast;                    // +0x04
    CInterfacePair* LowerBound(const long& key);            // 0x004365B0
    __forceinline CInterfacePair* Find(ETCInterfaceType key)
    {
        CInterfacePair* it = LowerBound(key);
        if (it == m_pLast || key < it->first)
            return m_pLast;
        return it;
    }
};

struct CTCBerserk_Calls {              // engine/CTCBerserk.h exists but is not used: only its ShouldAttackDecapitate call is used
    bool ShouldAttackDecapitate();              // 0x0078BFC0
};

struct CThingCreatureBase_Retail {              // engine/CThingCreatureBase.h exists but is not used: the flags dword at +0x30 and the interface map at +0x44 are retail offsets
    unsigned char _pad[0x30];
    unsigned long Flags30;                      // +0x30 (bit 0x10: berserk)
    unsigned char _pad34[0x44 - 0x34];          // +0x34
    CInterfaceMap Interfaces;                   // +0x44
    __forceinline CTCBerserk_Calls* GetBerserkInterface() { return reinterpret_cast<CTCBerserk_Calls*>(Interfaces.Find(TC_INTERFACE_BERSERK)->second); }
};

struct CMeleeCombatAbilityDef {
    unsigned char _pad[0x5c];
    bool CanDecapitate;                         // +0x5c
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
    CMeleeCombatAbilityDef* PMeleeAbilityDef;   // +0xb8 (CCountedPointer, raw pointer used)
    unsigned char _padbc[0x114 - 0xbc];         // +0xbc
    bool IsDecapitatingAttack() const;
};

bool CCombatActionBase::IsDecapitatingAttack() const
{
    if (PMeleeAbilityDef != 0 && PMeleeAbilityDef->CanDecapitate) {
        CThingCreatureBase_Retail* creature = PCreature.GetPItem();
        if (creature->Flags30 & 0x10)
            return creature->GetBerserkInterface()->ShouldAttackDecapitate();
        return true;
    }
    return false;
}