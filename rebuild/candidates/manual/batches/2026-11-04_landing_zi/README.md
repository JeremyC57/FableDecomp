# Landing 2026-11-04zi: chunk 772 (8 never-graded Fable bodies from chunks 767/770), 3 landed

## Result
- **Landed 3 of 8** (`logs/land.log`). All are RELOCATION_MATCH + PASS, graded read-only on the merged tree first (`logs/new2.log`):
  - 0x00E11670 `NScript::CQ_WaspBossScript::CWaspVictim::Main` (13 grades)
  - 0x00E137B0 `NScript::CQ_WaspBossScript::Outro` (7 grades; the catalog had the autoname DoMission_WaspMenaceOutro)
  - 0x00E62D60 `NScript::CV_ChickenKickingScript::CreateSpectators` (3 grades)
  - There are no helpers, asm or pragmas.
- **Parked 3** (`pending/2026-11-04_linux_hand772/differ/`, tests pass). All three are the open "out-of-line callee inside an
  expanded inline" family: retail expands a container or ctor inline and calls ~CScriptThing / the CScriptThing copy / ~CTimer out of
  line inside it. Plan step 4 (the CScriptThing family) owns them.
  - 0x00EE8390 OakVale: `vector<CScriptThing>::resize`, 9 grades.
  - 0x00E1F7E0 CTraderToEscort::Init: the CSpawnedFunc1 `Par1` copy, 6 grades. It stopped at the shared blocker.
  - 0x00D29880 CHookCoastTeleporter::Main: static-frequency inline split, 10 grades. `/d2inlT` was used as a diagnostic only.
- **Not attempted:** 0x0078C900 CTCBerserk::InitAbility and 0x00828710 CRegionConnectionGraph::InitialiseGraph. Their flow and callee
  maps are in the chunk NOTES.md.

## Headers (2 changed, new sub-macros, `default=same`)
- `FABLE_RETAIL_CQ_WASPBOSSSCRIPT_ENTITIES` in `NScript_CQ_WaspBossScript.h`.
- `FABLE_RETAIL_CCHARSTRING_CONCAT_STRING` in `fable_string.h`: the free `operator+(const CCharString&, const CCharString&)`
  0x0099F570.
- `ppcmp.py`: all 3,557 includers read the same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zi.tsv` (11).
- **NAME_FIXES (7) queued under `land0411zi`:**
  - 0x00E20210 is `CQR_EscortTraderScript::WatchForTraderHit`, which landed callers bind as `WatchForCutscene@CQ_WaspBossScript`;
  - 0x0099F570 placeholder `sub_99f570` is `??H@YI?AVCCharString@@ABV0@0@Z`;
  - 0x00CDBFB0 is the cast-map `~map`.
- **Checks:** `bindings_multi_check.py` prints 0. `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Coverage
46,864 genuine of 49,568; 5,854,520 function bytes (57.28%).
