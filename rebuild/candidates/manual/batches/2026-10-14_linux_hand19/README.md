# Batch `2026-10-14_linux_hand19` — delegated hand chunk 19 (CCombatAbilityUseProjectileWeaponDef .. CCombatRing): 36 bodies (36 landed)

Nineteenth delegated hand chunk of the Wine lane (`$S/w22/chunk19.tsv`, 40 small-module bodies of
17–96 bytes authored by a background agent against the live VC7.1 compiler, re-proved through the
lander's own parity and behaviour checks): creature-action and combat-action constructors
(`CCreatureAction_ForcePushInto` / `_OnGenerate` / `_TrollIdle` / `_KrakenRiseUp` /
`_JackFloatUp`, `CCombatAction_WillHealInto`, the `CCombatActionBase(owner, code, CSoundPair)`
family), two `CIntelligentPointer_01238c6c` constructors, the `CStaticMapBankFile` constructor,
`CCombatActionBase::GetFrameUpdateTimeScaled`, the strike-event helpers (`_For_each` /
`_Find_if` functors over the +0xbc list), `CCombatRing` scale getters/setters and the
`GFLoadScriptDef` script-region free function (`#pragma optimize("gsy", off)` frame).

Label corrections and callee identities are in the batch sources; retail `CCombatActionBase` is
0x114 bytes (PDB build 0x12c), so the derived-class headers disagree with the bytes and the
sources carry local `<Class>_Retail` models. Parked (scheduling / register allocation /
tail-merge only): 0x00866FB0, 0x007F3D60, 0x006E8050, 0x00811380.

All graded `RELOCATION_MATCH` (one `MATCH`) + `PASS` on 2026-10-14.
