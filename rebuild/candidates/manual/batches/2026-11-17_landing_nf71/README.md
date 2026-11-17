# 2026-11-17 landing nf71: name-fix round 7, NF71 (4 rows: blocked ports NF66-NF68 left)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF71 took the 20 rows NF66, NF67 and NF68 had left blocked (the three that wait on an owner decision or a CRenderStateManager.h
change were left out). It ported 4. Brief, list and records: `naming/2026-11-17_agentNF71/`.

## Rows (STATUS.tsv: 20 + 0x0057770A)

| Status | Rows |
|---|---|
| ported | 4: 0x00E12580, 0x008F95D0, 0x00EE2490, 0x006A0150 |
| blocked after 15 grades | 1: 0x004FE630 (DIFFER 453v440, register allocation only: retail keeps the loop counter in ebp) |
| blocked, 0 NF71 grades | 15 (earlier agents' blockers kept; NF71 added findings for 6) |
| deferred | 0x0057770A (held by NF70) |

- **0x00E12580** `NScript::CQ_WaspBossScript::DoMission` (15 grades): NF68's shared-header port matches at `// Flags: /d2inlT60`,
  the smallest budget that does (59 and 58 differ), with an `// Inline-budget:` line. The row no longer uses `#pragma inline_depth`
  regions, which retires its old "Revisit: inline-depth" note.
- **0x008F95D0** `LHNetworkLib::LSocket::SendBlock` (6 grades): the TU frees its local `vector<char>` without the null test
  (`push ecx ; call free` at 0x008F96B8), while the strings next to it keep theirs. The new stlport opt-in
  `FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED` gives that release; the coordinator marked it FITTED in the header comment (owner
  acceptance test of 2026-11-14: same bytes, same behaviour since `free(0)` is a no-op, portable). Local names from Ego_d.pdb.
  Leaf-only self rename to the PDB `vector<char>&` form.
- **0x00EE2490** `NScript::CV_TravellingHeroesScript::CTH_Thunder::Main` (11 grades): the implicit CScriptThing copy, with the
  landed CIsThingAlive idiom (`#pragma inline_depth(0)` round the row-local constructor, marked `// Inline-budget:`); budget flags
  alone never split the two copy sites. Uses FableWin.pdb's `GetMasterData`.
- **0x006A0150** `CCreatureAction_UnsheatheItemFromInventory` constructor (4 grades): a fresh port with all 10 local class models
  deleted; leaf-only self rename to the PDB spelling every landed caller already binds.

## Entries (APPLIED.tsv, 29): 3 done, 26 blocked
- **Done:** nf44:98, nf41:e161, q10502. q10541 now waits only on 0x00EE2A80 (the fable_string.h `operator==` decision).

## Rows and checks
- **Section 1: 2 leaf-only self renames (no file moves). Section 2: 21 respelled bindings.**
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck.log`):** all 4 rows graded from the repo after landing: 4 RELOCATION_MATCH, every test PASS. NF71's own
  final run agrees.
- **Checks:** `naming/rebind.py` removed 21 binding lines and added 6; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (3 opt-ins in 3 files; lines added only)
`stlport/fable_containers.h` (`VECTOR_BASE_DTOR_FREE_UNTESTED`, FITTED), `engine/CV_TravellingHeroesScript.h`
(`THUNDER_GETMASTERDATA`), `engine/CAnimationEntry.h` (`TRANSITION_CTOR`, `??0CAnimationEntry@@QAE@ABV0@J@Z`).

## What the 15 unreached rows need (NF71 NOTES)
- 0x00596CCE / 0x00597006: port 0x005955AB `GetProfileNames` first.
- 0x008C2550, 0x008DDAD0, 0x008E0060, 0x008F3630: NF63's drafts still model classes locally with fake stand-in bodies; they need a
  fresh port, not a missing define.
- Several bodies still call made-up callee names.

## Rule slips
- 14 rows ended with 0 NF71 grades (the "carry each row to a terminal grade" rule).
- A scratch script named `dis.py` shadowed a module import; renamed and rerun.
- One `llvm-pdbutil` call on Ego_d.pdb was denied by the permission system; Ego_d was otherwise read only with `python3 -I`.
