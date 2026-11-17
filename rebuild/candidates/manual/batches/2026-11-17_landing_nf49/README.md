# 2026-11-17 landing nf49: name-fix round 7, NF49 (52 rows: definition-manager templates and special-effects callees)

Owner: "Continue the naming fix queue with two agents until complete".

NF49 took 12 entries from both halves: 54 binder rows, from NF41's probe and NF44's binder lists. The entries are:
- `CDefinitionManager::GetDef<T>`: CEngineThemeDef, CMessageEventDef, CCameraManagerSetDef and CCameraModeDef;
- `CDefClassBase::GetSubDef<T>`: CSpecialEffectsDef, CTavernGameSpotTheAdditionDef, CTavernGameCoinGolfDef and
  CTavernGameShoveHaPennyDef;
- `CAIStateGroupBase::PeekDefinitionManager` (two queue lines);
- `CTCSpecialAbilities::CreateParticleEffect`;
- `CSpecialEffectsDef::GetEffectFromString`.

Container restarts interrupted the agent twice. It resumed from its own files each time and ran the final recheck over every
changed row. Brief, worklist and records: `naming/2026-11-17_agentNF49/`.

## Rows (54, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 52 |
| blocked | 2 (rows unchanged, locks released) |

**Blocked rows:**
- **0x008B9E50 (6 grades):** it needs a CDefPointer release form that keeps the handle's stack slot live.
- **0x008F3630 (3 grades):** `NThingShot::CTCShotDefinitionBase` and its def handles must be ported first.

**Partly fixed:** 0x008F7950 CreateDustEffectForCreature. Its GetDef calls and the material call now bind PDB spellings. Its own
name and 12 other callees still use the made-up `NDust008f7950` model, which needs a port of the whole NCombatActionHelpers file
together with 0x008F7BD0. This is queued.

## Entries (APPLIED.tsv)
- **11 done:** e183, e32, e78, e77, e130, e95, e129, nf44:12, nf44:129, e109 and e111.
- **1 blocked:** e110 `GetSubDef<CSpecialEffectsDef>`. 14 of 16 binders are ported; what is left is the 2 blocked rows above.

## Rows and checks
- **Self renames (section 1):** 2 leaf-only changes, 0x0082D3B0 PeekPCameraManagerSetDefinition and 0x00972830 SetupArena; no file
  moves.
- **Section 2:** 89 caller respellings, from relocations before and after each port.
- **Recheck (`logs/recheck_part0..2.log`):** all 52 graded from the repo after landing. All 52 are RELOCATION_MATCH, every test
  PASS.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 129 binding lines and added 83;
  - bindings_multi_check is at its baseline (`NullCoord`).
- **Local model edited:** 0x0063EB20 CWorldMap::DestroyThemeAt edits its pre-existing local CWorldMap model.
  `PeekDefinitionManager` becomes protected and returns `const CGameDefinitionManager&`; `GetStrongestEngineThemeAt` takes
  `long&`. Both now bind their Ego_r spellings. Moving the row onto engine/CWorldMap.h is row debt.

## Headers (opt-ins only; default path unchanged)
- **engine/CAIStateGroupBase.h, `FABLE_RETAIL_CAISTATEGROUPBASE_CAIFILTERBASE_RESTORE_PROTECTED`:** puts the `protected:` label
  back after the `_FINDAGUARD` block. That block's `public:` made the AI action members public (QAE) instead of protected (IAE).
- **engine/CMaterialDef.h, `FABLE_RETAIL_CMATERIALDEF_GETRANDOMSLIDEFOOTPARTICLEEFFECT`:** declares
  `?GetRandomSlideFootParticleEffect@CMaterialDef@@QBEJXZ` (Ego_r).

## Name fixes queued (`land1117fu`)
- 0x007F0300: GetStrongestEngineThemeAt takes `long&`.
- 0x009810B0: PerformActionAndWait is protected; use the new opt-in.
- 0x008F7950: the whole-file NCombatActionHelpers port.
- 0x0069AD10 and 0x0081FA20 / 0x0081FA90: from the first run.

## Rule slips
None reported. After the restart, two grades checked the lane on 0x00917130 with no source change.
