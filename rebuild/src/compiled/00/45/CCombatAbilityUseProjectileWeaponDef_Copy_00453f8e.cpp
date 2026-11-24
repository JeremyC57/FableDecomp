// Leaf: ?Copy@CCombatAbilityUseProjectileWeaponDef@@UAEXPBVCDefClassBase@@@Z
// CCombatAbilityUseProjectileWeaponDef::Copy(const CDefClassBase* other) -- copy every member of another CCombatAbilityUseProjectileWeaponDef: `*this = *(const CCombatAbilityUseProjectileWeaponDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453F8E, 5 bytes: a thiscall tail call, `jmp` to CCombatAbilityUseProjectileWeaponDef::operator= 0x00453F93 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00453F8E (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CCombatAbilityUseProjectileWeaponDef::operator=(const CCombatAbilityUseProjectileWeaponDef&) 0x00453F93, ??4CCombatAbilityUseProjectileWeaponDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCOMBATABILITYPROJECTILEATTACKDATA
#define FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CCombatAbilityUseProjectileWeaponDef.h"

void CCombatAbilityUseProjectileWeaponDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCombatAbilityUseProjectileWeaponDef*)other;
}
