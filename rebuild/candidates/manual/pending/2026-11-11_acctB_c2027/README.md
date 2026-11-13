# 2026-11-11_acctB_c2027 -- acctB hand-back, chunk 2027 (2 reserved rows: 1 matched, 1 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00DCD350 NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Main | RELOCATION_MATCH 1833v1833, test PASS (g++ and VC7.1) | generated-script shape (`TRetVoid _Fail_Val`, terminate check per block and after each loop); retail's dead `HeroTeam` / `TeamID` loads (0x00DCD4C5) come from `if (TeamID == ParentClass->HeroTeam) { check } else { check }` with only the terminate check in each arm. `FABLE_FORCEINLINE CTimer::operator=` / `GetTime` as in the 64 landed NScript rows. Aeon's Lua ports are not on the acctB machine: structure derived from retail bytes only. 2 grades |
| 0x007C6E50 CTCTurncoatSpell::FrameUpdate | parked, DIFFER 1804v1806 | one allocation decision in the XP block: retail loads the caster into a codegen temp (0x007C741C `mov edx,[esp+14h]` above `mov [esi+90h],ecx`, 0x007C7426 `mov eax,[edx+2Ch]`, 0x007C7432 `mov esi,edx`), ours gives the caster's split piece esi (+2 bytes) and the temp rotation after it shifts by one. 4 grades; **passes 2 and 3 not graded** (18 further spellings measured with c2prio left the deciding allocation unchanged). Source + passing test in `rows/parked/`. Queued for the acctB near-miss agent |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean. Parked row regraded:
DIFFER(1804v1806), as recorded.

Review items for landing:

- NAME_FIXES.tsv (5): **0x00DCDA80 MakeTeamMemberComment** is `QAE_NABVCCharString@@AAVCScriptThing@@W4ERaidComment@12@@Z`
  in both PDBs; the landed row defines a different signature, so respell it when landing this caller or vland reports
  RELOC_NAME. 0x0072AC40 is `CTCSpecialAbilities::FadeOrKillEffect(CThing*)` (static), 0x0072AB70
  `CTCSpecialAbilities::CreateParticleEffect` (static); 0x00669580 / 0x00669D30 are the shared GetSubDef<CSpecialEffectsDef>
  and CSpecialEffectsDef::GetEffectFromString (landed StartTurncoatingEffects 0x007C6AA0 uses other names).
- `engine/CQ_OrchardFarmRaidScript.h` types MissionFailed as `long` (the PDB type is unnamed): header debt. ERaidComment's
  enumerator names are descriptive (not in either PDB name set).

To land: the matched row needs only the new `hdr/engine/CQ_OrchardFarmRaidScript.h` (CTeamSpawn behind
`FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN`); the other six overlay files serve the parked row. Evidence in
`hdr/README.md`. Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
