// CCombatAbilityAttackBase::CCombatAbilityAttackBase(ECombatAbilityInterfaceType interfaceType) -- def base, this class's vtable, then the embedded melee-attack data built for the interface type
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C503, length 32 bytes.
// Leaf: ??0CCombatAbilityAttackBase
// Callees: the def-class base constructor @ 0x0044C0C0 (catalog-wide symbol ??0CShoveHaPennyRegionDef@@QAE@XZ, the
// name 77 landed rows bind to that address -- it is the shared CDefClassBase-level constructor; kept for
// one-symbol-per-address), CCombatAbilityMeleeAttackData::CCombatAbilityMeleeAttackData(long) @ 0x0044C47B (landed).
// Vtable 0x012324D4 = CCombatAbilityAttackBase (rtti-vtables.tsv).
// Label correction: the manifest's "CIntelligentPointer<const CThing>::CIntelligentPointer" is propagated. The
// callers 0x0044C535 / 0x0044C54D / 0x0044C565 are the derived attack-def constructors passing interface types
// 5, 6, 7; Ego_r.pdb CCombatAbilityAttackBase : CDefClassBase holds CombatAbility (CCombatAbilityMeleeAttackData)
// at +0x28 -- the same model as the landed scalar deleting destructor 0x0044FCD7.
// Shape: size-optimised module (`push dword ptr [esp+8]` pass-through): `#pragma optimize("s", on)`. Base call,
// the argument push, the member's `lea`, the vtable store, then the member constructor.
#pragma optimize("s", on)
struct CShoveHaPennyRegionDef {                // the def-class base subobject (0x28 bytes: CResource .. CDefClassBase)
    virtual void v00();
    unsigned char _body[0x24];                 // +0x04 .. +0x28
    CShoveHaPennyRegionDef();                  // 0x0044C0C0
};

struct CCombatAbilityData {                    // vtable 0x01232484, 0x18 bytes
    virtual void v00();
    long  PercentageChance;                    // +0x04
    float SecondsDuration;                     // +0x08
    float SecondsToWaitBeforeRepeat;           // +0x0c
    long  Anim;                                // +0x10 CDefString
    long  InterfaceType;                       // +0x14 ECombatAbilityInterfaceType
};

struct CCombatAbilityMeleeAttackData : CCombatAbilityData {   // vtable 0x01232498
    bool  Blockable;                           // +0x18
    bool  Knockdown;                           // +0x19
    float Damage;                              // +0x1c
    CCombatAbilityMeleeAttackData(long interfaceType);        // 0x0044C47B
};

struct CCombatAbilityAttackBase : CShoveHaPennyRegionDef {   // vtable 0x012324D4
    CCombatAbilityMeleeAttackData CombatAbility;              // +0x28
    virtual void v00();
    CCombatAbilityAttackBase(long interfaceType);
};

CCombatAbilityAttackBase::CCombatAbilityAttackBase(long interfaceType)
    : CombatAbility(interfaceType)
{
}