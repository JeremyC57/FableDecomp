// Leaf: ?Copy@CCombatAbilityStrafeDef@@UAEXPBVCDefClassBase@@@Z
// CCombatAbilityStrafeDef::Copy(const CDefClassBase* other) -- copy every member of another CCombatAbilityStrafeDef: `*this = *(const CCombatAbilityStrafeDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453F5E, 5 bytes: a thiscall tail call, `jmp` to CCombatAbilityStrafeDef::operator= 0x00453F63 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00453F5E (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CCombatAbilityStrafeDef::operator=(const CCombatAbilityStrafeDef&) 0x00453F63, ??4CCombatAbilityStrafeDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCOMBATABILITYSTRAFINGDATA
#define FABLE_RETAIL_CCOMBATABILITYSTRAFEDEF
#define FABLE_RETAIL_CCOMBATABILITYSTRAFEDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CCombatAbilityStrafeDef.h"

void CCombatAbilityStrafeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCombatAbilityStrafeDef*)other;
}
