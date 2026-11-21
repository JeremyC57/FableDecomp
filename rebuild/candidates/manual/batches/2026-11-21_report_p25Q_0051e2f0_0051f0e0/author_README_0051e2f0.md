# 0x0051E2F0 CThingManager::ActivateThings -- RE-PORT onto shared headers, READY (p25Q, Account B, 2026-11-21)

Land as a `fix:` commit (replaces rebuild/src/compiled/00/51/CThingManager_ActivateThings_0051e2f0.cpp and its test), BEFORE 0x00521CF0,
whose same-TU `// Fidelity:` copy of this row is this file's body verbatim.

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x0051E2F0 | CThingManager::ActivateThings(const vector<CThing*>& activate_these_things) | 395 | RELOCATION_MATCH (no flags) | PASS (links the REAL row object; 7 scenarios, call log against an independent model) | 17 / 17 KILLED |

Strict check (`vland.log`): `LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=hdr vland.py payload.json oracle.tsv --recheck-landed`
-> RELOCATION_MATCH, behaviour PASS, no warnings. (`--recheck-landed` because the address is already landed.)
`src.diff` = landed file -> re-port.

## Text
- Shared headers only: CThingManager (World +0x24 via GETWORLD), CThing (ORIGINAL_VTABLE slots 8 OnCreate / 9 InitialActivate /
  10 OnInitialActivate; IsDead; new ENTITYINITLEVEL opt-in), CClass::GetType, CWorld::IsSerialisingAboutToLoadHeroState (landed opt-in).
- Ego_d's locals (activate_these_things, one const_iterator `it` and one `pthing` per loop, `phero`) and statements; the source comment
  decodes the Ego_d line sizes 2054..2116 (progress calls with a string temporary, 8-B GetEntityInitLevel calls, 5-B `continue`s, 7-B
  `phero = NULL`, 6-B `phero = pthing`, 2-B `else`).
- Dropped: the landed local CThing / vector / CMainGameComponent models, the local virtual names (Initialise / OnCreate / Activate) and the
  made-up `ShowProgress` helper (no PDB names it; Ego_d calls NProgressDisplay::DisplayProgress directly at each progress line).

## Labels
- `// Inferred:` the held-back hero's three-term condition as one `if` (Ego_d spreads it over lines 2108 / 2110, split not decoded; retail
  tests the three terms in that order).
- No Fidelity copies, brackets, flags or inline-budget controls.

## Header changes (hdr/, hdr.diff) -- strictly add-only, pin_asserts: 0 pinned
- engine/CThing.h, NEW opt-in FABLE_RETAIL_CTHING_ENTITYINITLEVEL: Ego_d nested enum CThing::EEntityInitLevel (NONE 0 .. DONE 3) and the
  private const inline accessor GetEntityInitLevel() (FableWin.pdb ?GetEntityInitLevel@CThing@@ABE?AW4EEntityInitLevel@1@XZ, `ABE` =
  private), plus `friend class CThingManager;` -- not invented: the accessor is private in the PDB and Ego_d's ActivateThings calls it
  (lines 2059 / 2075 / 2093), so the original CThing befriended CThingManager. No layout change.
- engine/CThingManager.h, NEW opt-in FABLE_RETAIL_THING_TYPE_IDS: Ego_d's unnamed thing-type enum (THING_TYPE_NULL 0 .. THING_TYPE_DRIVER
  15, MAX_NO_THING_TYPES 16).
- engine/CThingManager.h, NEW opt-in FABLE_RETAIL_CTHINGMANAGER_LOADMAINPLAYERCREATURESTATE (shared with the two other batches):
  declarations of LoadMainPlayerCreatureState, ActivateThings (when WORLDLOAD does not declare it), private CopyHeroToHero, and
  NThingManager::HeroSwapInProgress (0x013B8A88). Ego_d parameter names; declarations only.
- Newly flagged rows: none known (no other landed row defines a local EEntityInitLevel / THING_TYPE enum).

## Names (NAME_FIXES.tsv)
Row name right. Spellings removed with the local models are listed. Observed, not in this row: four other landed rows bind four different
names to 0x009E9F40 (NProgressDisplay::DisplayProgress in both PDBs) -- listed for the coordinator.
