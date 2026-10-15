// CCombatAbilityMeleeAttackData::CCombatAbilityMeleeAttackData(long interfaceType) -- CCombatAbilityData(interfaceType), zero damage, then this class's vtable, blockable and no knockdown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C47B, length 37 bytes.
// Leaf: ??0CCombatAbilityMeleeAttackData
// Callees: CCombatAbilityData::CCombatAbilityData(ECombatAbilityInterfaceType) @ 0x0044C3EE (manifest-named; stores
// PercentageChance 100, SecondsDuration -1, SecondsToWaitBeforeRepeat 0, Anim -1 and the type). Vtable 0x01232498 =
// CCombatAbilityMeleeAttackData (rtti-vtables.tsv); the chunk label "CTCAmbientBird::CTCAmbientBird" is propagated and
// rejected. Ego_r.pdb CCombatAbilityMeleeAttackData (0x20 bytes): bool Blockable +0x18, bool Knockdown +0x19, float Damage +0x1c.
// Shape: the size-optimised module (`push dword ptr [esp+8]` pass-through): `#pragma optimize("s", on)`. As in the base
// constructor, the float zero is `fldz ; fstp` and is scheduled ahead of the vtable store, the byte stores after it. The
// enum argument is a by-value 4-byte `long`. engine/CCombatAbility*.h are flattened PDB PODs, so the polymorphic classes
// are modelled locally.
#pragma optimize("s", on)
struct CCombatAbilityData {                    // vtable 0x01232484, 0x18 bytes
    virtual void v00();
    long  PercentageChance;                    // +0x04
    float SecondsDuration;                     // +0x08
    float SecondsToWaitBeforeRepeat;           // +0x0c
    long  Anim;                                // +0x10 CDefString
    long  InterfaceType;                       // +0x14 ECombatAbilityInterfaceType
    CCombatAbilityData(long interfaceType);    // 0x0044C3EE
};

struct CCombatAbilityMeleeAttackData : CCombatAbilityData {   // vtable 0x01232498
    bool  Blockable;                           // +0x18
    bool  Knockdown;                           // +0x19
    float Damage;                              // +0x1c
    virtual void v00();
    CCombatAbilityMeleeAttackData(long interfaceType);
};

CCombatAbilityMeleeAttackData::CCombatAbilityMeleeAttackData(long interfaceType)
    : CCombatAbilityData(interfaceType), Blockable(true), Knockdown(false), Damage(0.0f)
{
}
