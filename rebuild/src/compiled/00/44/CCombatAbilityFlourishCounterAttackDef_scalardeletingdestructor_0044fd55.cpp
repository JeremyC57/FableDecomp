// CCombatAbilityFlourishCounterAttackDef::`scalar deleting destructor'(unsigned int) -- destroy the combat-ability data and the def base, free when bit 0 is set
// Leaf: ??_GCCombatAbilityFlourishCounterAttackDef@@UAEPAXI@Z
// Retail: Fable.exe @ 0x0044FD55, 41 bytes, `ret 4`; slot 0 of the CCombatAbilityFlourishCounterAttackDef vtable 0x012325A4 (the only reference). Catalog label
// CLandscapeBackgroundPatch::`vector_deleting_destructor' is wrong (no array path). Body: the inline member destructor
// stores the CCombatAbilityData vptr (0x01232484) at +0x28, the inline ~CDefPointeeBase stores 0x01230BA0 at +0, the
// out-of-line ~CResource (0x009FC550) runs, then operator delete (0x00BFE9BC) when (flags & 1); returns this. The six
// combat-ability def deleting destructors 0x0044FCD7..0x0044FE12 are byte-identical.
// Layout: Ego_r.pdb CCombatAbilityFlourishCounterAttackDef (72 bytes, no members of its own) : CCombatAbilityAttackBase (CombatAbility, a CCombatAbilityMeleeAttackData, at +0x28);
// CDefClassBase is the packed 37-byte Ego_r.pdb layout (as 0x00430300), the data class's own members are trivial.
// Opt: size TU (`pop ecx` after the cdecl delete). Shape: all destructors below CDefPointeeBase/CCombatAbilityData are
// implicit (no derived vptr stores); constructing the class in a helper makes VC7.1 emit its vtable and ??_G.
#pragma optimize("s", on)
#include <new>
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CCombatAbilityData {
public:
    virtual ~CCombatAbilityData() {}
    long  PercentageChance;                              // +0x04
    float SecondsDuration;                               // +0x08
    float SecondsToWaitBeforeRepeat;                     // +0x0c
    long  Anim;                                          // +0x10 CDefString
    long  InterfaceType;                                 // +0x14 ECombatAbilityInterfaceType
};
class CCombatAbilityMeleeAttackData : public CCombatAbilityData {
public:
    unsigned char _rest[0x8];                        // +0x18
};
class CCombatAbilityAttackBase : public CDefClassBase {
public:
    CCombatAbilityMeleeAttackData CombatAbility;          // +0x28
};
class CCombatAbilityFlourishCounterAttackDef : public CCombatAbilityAttackBase {
};
void ConstructCCombatAbilityFlourishCounterAttackDef(void* p)          // emission helper, not a retail function: the vtable (and its ??_G) is emitted
{
    new (p) CCombatAbilityFlourishCounterAttackDef;
}