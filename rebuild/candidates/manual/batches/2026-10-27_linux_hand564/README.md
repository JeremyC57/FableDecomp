# Chunk 564 (agent564, 2026-10-29): 10 of 12 never-tried rows land (RELOCATION_MATCH), 2 parked

CTCSpecialAbilities::CancelUsingAbility 0x0072ACF0 (jump table checked entry by entry), the implicit
NCreatureMode::CModeBriarRoseConcentrate dtor 0x00743120, ~CTCChest 0x00750850 (catalog CModeOutOfActionBase), ~CTCLook
0x00753290, ~CTCBerserk 0x0078C080, ~CTCDivineWrath 0x0078F5A0, ~CTCUnholyPower 0x00794AC0, ~CTCForcePushed 0x00798620
(catalog CTCTurncoat), CTCWillLightning::CLightningBeam::~CLightningBeam 0x007A0770, ~CTCBattleCharge 0x007A73F0 (catalog
CTCActionUseBed). Names from the PDB plus the first vtable store's RTTI.

Parked (pending/2026-10-27_linux_hand564/differ):
- 0x0072C430 CTCSpecialAbilities::IsAbilityAvailable (catalog GetAbilityAvailability), 584v584: register allocation in
  the 15 `!HasTC` arms (retail reuses esi).
- 0x0077FDE0 CPersistContext::Transfer<EHeroCentreDoorType> (catalog Transfer<signed_char>), 208v240: over-spanned row,
  byte-exact over its true extent; re-split at 0x0077FEB0 (the 2-argument Transfer) and land as is.

NAME_FIXES.tsv: 22 rows, deferred (naming pass): static CTCSpecialAbilities::FadeOrKillEffect 0x0072ABD0,
CTCPhysicalShieldSpell::IsShieldActive 0x0079DCB0, STLport list clear 0x007A3910, CTCBase::GetWorld 0x00686A70, and more.
