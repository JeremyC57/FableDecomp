# 2026-11-17 landing nf79: name-fix round 7, NF79 (47 rows: the nf76-nf77 follow-ups, q10956..q10974)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", "add another agent to speed
up naming queue completion", then "Decrease to 2 agents at a time once next agent finishes".

NF79 took the 19 entries queued at the nf76 and nf77 landings and their binders. Brief, list and records: `naming/2026-11-17_agentNF79/`.

## Rows (STATUS.tsv, 63 = 60 locked + 3 claimed through the lock; 0x0059A238 was held by NF78 and is not among them, its two wrong spellings are queued)

| Status | Rows |
|---|---|
| ported | 47 |
| not-a-binder | 13 |
| blocked | 3: 0x00A25E70 (DIFFER 747 v 755 on the shared `CCountedPointer<const C3DMesh2>`), 0x00B82100, 0x00B90270 (STATUS gives the forms and errors) |

Among the ported rows:
- **q10969:** the data symbol 0x013CA818 is `?PClass@?$CTBaseSingleton@VCFileInstaller@@@@0V?$CCountedPointer@VCFileInstaller@@@@A` (Ego_d.pdb
  PUB32 and the shared header's member); its binders, bound as Instance, PInstance, g_pointer_013ca818 or PClass, now agree.
- **q10973:** 0x01436E9C is `NEngineInternal::GPLightingManager`; the stale Ghidra label `g_pEngineLightingManager` is queued.
- **q10970:** 0x0057770A, the twin of 0x005778CA, is ported as `CTCInventoryExperience::CloseInventoryInternal` (was `vfunc_89`).
- **q10974:** all five addresses named from two sources; 0x00A23E40 / 0x00A256F0 / 0x00A36EC0 / 0x00BEB540 landed, 0x00A25E70 blocked.
- Self renames off stand-ins, e.g. 0x0087C390 `CTCSinglePersonOccupiable::SetOccupier` (was `_unattributed::AssignTarget_0087c390`), 0x009D52B0
  `CBankFileAsyncEntry::ReadDataNonAsync` (was catalogued `CEditTransactionBase::ActionSetEngineTheme`), 0x0066E799
  `NHeroInformationScreens::CManager::SelectScreenOfType`, 0x00558E10 `NUISystem::CDraggableInto::GetDataFromDef` (was `Initialize`), the
  STLport deque `back` / `pop_back` (were `CScreenStack::Top` / `Pop`).

## Entries (APPLIED.tsv): 18 done, 1 blocked (q10974: 0x00A25E70)

## Rows and checks
- **Section 1: 24 self renames (34 file moves).** Coordinator correction: the deque rows 0x0059B039 / 0x0059AEE5 take the catalog spelling of
  their sibling deque rows (`std::deque<NUISystem::CComponent *, std::allocator<NUISystem::CComponent *>>`).
- **Headers (13 opt-ins in 12 overlays, one new header `engine/NTCScriptedControl_CActionDoCreatureAction.h`):** listed in
  `naming/2026-11-17_agentNF79/hdr/README.md`. NF79's base copies were identical to the repo. 2 asserts pinned (CTCScriptedControl.h,
  NUISystem_CDraggableInto.h); `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65. The one non-additive edit turns NF76's
  `#if` in stlport/fable_deque.h into an `#elif` (token-identical with no opt-in defined, per hdr/README.md).
- **Existing local models edited in place, no new ones** (NOTES "In-place edits"): 0x00B76BC0, 0x00BE1BA0, 0x00A256F0 and 0x0093AD70 changed a
  pre-existing local model only as far as the PDB spelling needed; a full port of each is still open (NOTES lists the classes).
- **Tests:** 0x005952C3, 0x008916A0 and 0x00BEB540 got new tests, because the old ones linked the stand-in names. The new fixtures declare
  the real classes only as link stubs for the row's imports. Each test keeps its PassPattern.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 47 rows graded from the repo after landing: 45 RELOCATION_MATCH and 2 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 121 binding lines and added 63; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 27)
NF79's NAME_FIXES.tsv (26: the GPLightingManager Ghidra label, STLport helper names for 0x0059AC41 / 0x0059AD35 / 0x0059A8F3 (the PDBs only
carry Dinkumware names: NOTES), 0x0059A238's two spellings and others), plus the three blocked ports.

## Rule slips
- Three Bash calls exited non-zero (a helper named dis.py shadowed the standard library; two `ls` of missing files).
- q10970 was ported before the q10971..q10974 respellings, against the brief's order.
- 0x00A25E70 took 6 grades before being blocked, past the three-failed-forms limit.
- Two compiles ran through q.py directly to keep the object for disassembly (not in grades.log).
