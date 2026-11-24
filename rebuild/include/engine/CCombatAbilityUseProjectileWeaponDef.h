#ifndef FABLE_ENGINE_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_H
#define FABLE_ENGINE_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "engine/CSubDefClassBase.h"
#include "engine/CCombatAbilityProjectileAttackData.h"
#ifdef FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CCombatAbilityUseProjectileWeaponDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
class CCombatAbilityUseProjectileWeaponDef:public CSubDefClassBase {public:CCombatAbilityProjectileAttackData CombatAbility;public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#ifdef FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_STATGETTYPENAME
// Opt-in (NF170, 2026-11-20, rows 0x0097EF60 / 0x0097F3A0 / 0x006696B0): FableWin.pdb ?StatGetTypeName@CCombatAbilityUseProjectileWeaponDef@@SI?AVCCharString@@XZ
// (0x01DA9C60; inline in retail: GetSubDef<CCombatAbilityUseProjectileWeaponDef> 0x006696B0 pushes the class-name literal). No layout change.
public: static CCharString FABLE_FASTCALL StatGetTypeName(){return CCharString("CCombatAbilityUseProjectileWeaponDef",-1);}
#endif
#ifdef FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_COPY_DECLS
    // Opt-in (NF386, 2026-11-24; NF337 form): declarations only, no layout change, for CCombatAbilityUseProjectileWeaponDef::Copy 0x00453F8E, which tail-calls the
    // out-of-line operator= (retail `jmp` 0x00453F93). ?Copy@CCombatAbilityUseProjectileWeaponDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable
    // placement (ghidra_out/labels_rtti_port.tsv) names at 0x00453F8E (it overrides CDefClassBase::Copy: clang override probe,
    // made-up-name control fails); ??4CCombatAbilityUseProjectileWeaponDef@@QAEAAV0@ABV0@@Z (PDB name sets) is the landed operator= row at 0x00453F93.
public:
    virtual void Copy(const CDefClassBase*);
    CCombatAbilityUseProjectileWeaponDef& operator=(const CCombatAbilityUseProjectileWeaponDef&);
#endif
};
#else
class CCombatAbilityUseProjectileWeaponDef:public CSubDefClassBase {public:CCombatAbilityProjectileAttackData CombatAbility;
#ifdef FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_STATGETTYPENAME
// Opt-in (NF170, 2026-11-20, rows 0x0097EF60 / 0x0097F3A0 / 0x006696B0): FableWin.pdb ?StatGetTypeName@CCombatAbilityUseProjectileWeaponDef@@SI?AVCCharString@@XZ
// (0x01DA9C60; inline in retail: GetSubDef<CCombatAbilityUseProjectileWeaponDef> 0x006696B0 pushes the class-name literal). No layout change.
public: static CCharString FABLE_FASTCALL StatGetTypeName(){return CCharString("CCombatAbilityUseProjectileWeaponDef",-1);}
#endif
#ifdef FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_COPY_DECLS
    // Opt-in (NF386, 2026-11-24; NF337 form): declarations only, no layout change, for CCombatAbilityUseProjectileWeaponDef::Copy 0x00453F8E, which tail-calls the
    // out-of-line operator= (retail `jmp` 0x00453F93). ?Copy@CCombatAbilityUseProjectileWeaponDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable
    // placement (ghidra_out/labels_rtti_port.tsv) names at 0x00453F8E (it overrides CDefClassBase::Copy: clang override probe,
    // made-up-name control fails); ??4CCombatAbilityUseProjectileWeaponDef@@QAEAAV0@ABV0@@Z (PDB name sets) is the landed operator= row at 0x00453F93.
public:
    virtual void Copy(const CDefClassBase*);
    CCombatAbilityUseProjectileWeaponDef& operator=(const CCombatAbilityUseProjectileWeaponDef&);
#endif
};
#endif
FABLE_STATIC_ASSERT_AT(7, offsetof(CCombatAbilityUseProjectileWeaponDef,CombatAbility)==0x28);
#endif
