# 0x0051F0E0 CThingManager::CopyHeroToHero -- RE-PORT onto shared headers, READY (p25Q, Account B, 2026-11-21)

Land as a `fix:` commit (replaces rebuild/src/compiled/00/51/CThingManager_CopyHeroToHero_0051f0e0.cpp and its test), after the
0x0051E2F0 re-port and BEFORE 0x00521CF0, whose same-TU `// Fidelity:` copy of this row is this file's body verbatim.

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x0051F0E0 | CThingManager::CopyHeroToHero(CThing& loaded_hero, CThing& hero, vector<CThing*>& loaded_things) | 595 | RELOCATION_MATCH (no flags) | PASS (links the REAL row object and its own CVectorMap::LowerBound copies; all 128 subsets of the 7 copied interfaces among unrelated components, call log against a model) | 11 / 11 KILLED |

Strict check (`vland.log`): `LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=hdr vland.py payload.json oracle.tsv --recheck-landed`
-> RELOCATION_MATCH, behaviour PASS, no warnings. `src.diff` = landed file -> re-port.

## Text
- Shared headers only: CThing (GETTC_EXPANDED out-parameter GetTC / PeekTC, ADDANDCOPYTC), CTCBase, CTCAppearanceMorphBase, CTCHeroMorph,
  CTCHeroStats, CTCHeroExperience, CTCHeroAttachableAppearanceModifiers, CTCInventoryAbilities, CTCInventoryStats, CThingManager.
- Ego_d's parameter names and its 11 locals with their Ego_d types (all checked against the TPI records: const pointers for the six
  PeekTC outs, non-const CTCAppearanceMorphBase* for the GetTC out, tc_hero_morph / old_tc_abilities / old_tc_stats in the blocks); the
  source comment decodes the Ego_d lines 1241..1285.
- Dropped: the landed local CTCBase stand-in (with a made-up `CopyStateFrom` slot), the local interface bitset / map model.

## Labels
- `// Inferred:` the top-of-function declaration block of the seven out locals (uninitialised declarations have no Ego_d line;
  point-of-use declarations and the reverse order both grade RELOCATION_MATCH too, measured).
- No Fidelity copies, brackets, flags or inline-budget controls.

## Header changes (hdr/, hdr.diff) -- strictly add-only, pin_asserts: 0 pinned
- engine/CThingManager.h: the FABLE_RETAIL_CTHINGMANAGER_LOADMAINPLAYERCREATURESTATE opt-in (same file as the 0x0051E2F0 batch: private
  CopyHeroToHero declaration with Ego_d parameter names).
- engine/CTCHeroMorph.h, NEW opt-in FABLE_RETAIL_CTCHEROMORPH_KILLPARTICLES: `void KillParticles(); // 0x0071C260` (Ego_r.pdb
  ?KillParticles@CTCHeroMorph@@QAEXXZ). Declaration only.
- engine/CTCHeroAttachableAppearanceModifiers.h, NEW opt-in FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_TCBASE: the class derives
  `public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent` (Fable.exe RTTI base list: CTCBase @0, CAGraphicAppearanceComponent
  @12), so AddAndCopyTC takes it as a const CTCBase&. The default branch's flattened `__vftable` / `_pad_0x04/08/0c` words and the assert that
  checks them are wrapped in `#ifndef` of the opt-in (lines added only; the default text is unchanged). Layout unchanged (sizeof asserts hold).
- Newly flagged rows: none (the landed rows with a local CTCHeroAttachableAppearanceModifiers model were already flagged by the existing header).

## Names (NAME_FIXES.tsv)
Row name right. The removed `CopyStateFrom` local name is CTCBase::Copy in Ego_r.
