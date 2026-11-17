# 2026-11-17 landing nf68: name-fix round 7, NF68 (12 rows: engine-side ports at or above 0x00970000)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF68 took 18 engine-side rows that NF63 / NF65 (and NF60, for 0x00E12580 / 0x00EDA8C0) had left blocked. It ported 12. Brief, list
and records: `naming/2026-11-17_agentNF68/`.

## Rows (STATUS.tsv, 18)

| Status | Rows |
|---|---|
| ported | 12: 0x009790F0, 0x00979F70, 0x0097A630, 0x009CEAE0, 0x009F6890, 0x00A25BD0, 0x00A42980, 0x00A79A30, 0x00AD85D0, 0x00B32AD0, 0x00B7FA20, 0x00EDA8C0 |
| blocked | 6 (each with at least one grade of a real port on the shared headers) |

- **0x00EDA8C0** (`NScript::CV_TalentlessBardScript::ProcessSongsAge`) matches with `// Flags: /d2inlT59`, the smallest inline budget
  that matches (57 and 58 still differ). The row carries an `// Inline-budget:` line with the values tried, as the owner's
  2026-11-14 acceptance test asks.
- 0x00A42980 was `_global::CIVCountedPointer_Assign_00a42980`; it is `CLipSyncDataBankEntry::BeginLoading`. 0x00AD85D0 and
  0x009F6890 are now the STLport `vector::_M_fill_insert` / `operator=` instances.
- **Blocked:**
  - 0x00B76BC0 (9 grades) and 0x00BB8090 (10): the operand order of the render-state bookmark test/or at one or two sites; every
    form inside the shared headers was tried.
  - 0x00E12580 (12): NF60's draft without its `#pragma inline_depth` regions is RELOCATION_MATCH under `/d2inlT62`, so the inline
    budget is what this row needs; the port onto NScript_CQ_WaspBossScript.h reaches DIFFER(1446v1446) at `/d2inlT100`.
  - 0x00EE2490 (8, best DIFFER 1321v1334): retail calls the CScriptThing copy out of line for one use and inline for another, and
    no header form gives both in one TU.
  - 0x00B9E950 (2) and 0x00BD1F70 (1): the row classes have no shared header; a full rewrite was not reached.

## Entries (APPLIED.tsv): 9 done, 9 blocked
- **Done:** nf41:e102, nf41:e177, nf44:16, nf44:83, q10459, q10475, nf48:land1113fy:e28, nf48:land1011fk:e1, nf46:34.
- q10475 counts as done: its only binder 0x009F6890 is ported and binds the STLport spelling `std::_Destroy`; the queued
  placeholder spelling is in REJECTED.tsv and the landed self row 0x009F6720 is queued for that respelling.
- **Blocked:** nf41:e120, nf41:e188, nf44:70, nf44:71, nf44:73 (0x00BD1F70), q10478 (0x00B9E950), nf41:e161 (0x00EE2490),
  q10541 (0x00E12580, 0x00EE2A80), nf45:29 (0x00B76BC0, 0x00BB8090 and four rows outside the slice).

## Rows and checks
- **Section 1: 9 self renames (6 file moves). Section 2: 73 respelled bindings.** Module spellings checked against the catalog's
  sibling rows of the same container instance.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 12 rows graded from the repo after landing: 12 RELOCATION_MATCH, every test PASS.
  NF68's own final run agrees.
- **Checks:** `naming/rebind.py` removed 72 binding lines and added 35; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (opt-ins in 16 files; default path unchanged)
CBankFile.h (3), CEngineWaterRenderer.h, fable_containers.h (7 STLport noinline / declaration opt-ins; one existing `#ifndef` line
widened to `#if !defined(A) && !defined(B)`, rebased on NF67's remove_if opt-in), CEnginePrimitiveRenderer.h, CShaderRenderManager.h,
CEngineLightingManager.h, CLipSyncDataBank.h (retail class `CLipSyncDataBank : CBankFileAsync`), CPhysicsMesh.h, CBankFileAsync.h,
CCreatureAction_ProjectileCrouch.h (retail layout), CAIStateGroup_CreatureUseCoverBase.h (3), CAIStateGroup_CreatureAdvanceInCover.h,
CAIStateGroup_CreatureTakeCover.h, CTCPerceiveThing.h, and two that serve only blocked drafts (NScript_CQ_WaspBossScript.h,
CV_TravellingHeroesScript.h). Details: `naming/2026-11-17_agentNF68/hdr/README.md`.

## Coordinator header work requested
- CBankFileAsync.h: 0x009D58D0 is `GetAsyncEntry` in both PDBs; the existing `CREATEENTRY` opt-in declares a made-up name, and the
  landed 0x00AF1360 / 0x00B42530 bind it (queued with the 0x009D58D0 name fix).
- CRenderStateManager.h: a setter form that is faithful per site (0x00B76BC0, 0x00BB8090).
- CBankFile.h: `CRuntimeData` should be a `class`, as both PDBs spell it.
- Promote the retail-class opt-ins of CLipSyncDataBank and CCreatureAction_ProjectileCrouch to the default path.
- CScriptThing copy forms for the NScript TUs (0x00EE2490).

## Name fixes queued (`land1117fu`, 20)
NF68's NAME_FIXES.tsv (19: STLport helpers its ports now bind, `GetAsyncEntry`, `InitialiseGlobalLights`, the protected cover-state
setters, `~CCreatureActionBase`, and others), plus the 0x00E13310 one-address-two-names conflict (EndMission in the self row and
header, WaspMenace in a binder; both made up).

## Rule slips
- Two Bash calls exited 1 (trailing `grep` with no match). One `rm -rf` was refused by the safety check and rerun in a guarded form.
- A `head -40` in NF68's grade script cut one relocation dump (0x0097A630); section 2 was regenerated from a full dump.
- 0x009D58D0 was first spelled CreateEntry and ProjectileCrouch's FacePos first placed at +0x118; both fixed and regraded.
- vland logs for five rows were written into the repo's ignored `work/` directory (`work/v_<addr>.log`, a default log path); the
  coordinator moved them out. Nothing was committed.
