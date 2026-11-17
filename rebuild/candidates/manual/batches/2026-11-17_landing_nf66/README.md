# 2026-11-17 landing nf66: name-fix round 7, NF66 (7 rows: game-side ports NF62 did not reach)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF66 took the 18 game-side rows NF62 had left blocked (one draft grade each, or a real attempt that stopped short). Brief, list and
records: `naming/2026-11-17_agentNF66/`.

## Rows (STATUS.tsv: 18 rows + 0x0053BF20)

| Status | Rows |
|---|---|
| ported | 7: the NUISystem CList group 0x0053AD07 / 0x0053822B / 0x0053B321 and 0x0053BF20, plus 0x006666A0, 0x004B11A0, 0x00694430 |
| blocked after real attempts | 4 |
| blocked, one probe grade only | 8 |

- **0x0053BF20 was outside rows.txt.** It is a sixth binder of q10462 / q10472 that NF66 found while checking those entries. It
  had no lock, and NF66 ported it in one grade with the 0x0053AD07 recipe. It also now binds 0x0053CFC2 under q10461's spelling.
- **Blocked after real attempts:**
  - 0x0075A530 (12 grades, DIFFER 610 v 611): one x87 operand-order byte. A probe showed the fix needs one extra by-value return
    between `GetBoundingBox().GetCenter()` and the local, which no PDB member provides. That needs a decision on a descriptive
    `CPhysicsMesh::GetBoundingBoxCentre` opt-in.
  - 0x004FE630 (7 grades, DIFFER 451 v 440): the text compare is solved. Only register allocation is left: retail keeps `i` in ebp,
    this build keeps `sections` there.
  - 0x00596CCE / 0x00597006 (2 grades each): both need 0x005955AB `GetProfileNames` ported and its body visible in their file.
- **One probe grade only:** 0x004091C0, 0x00416953, 0x0041B550, 0x00549B20, 0x0057770A, 0x005778CA, 0x00599E3F and 0x006D53C0. Each
  STATUS line gives the compile errors and what the port still needs.

## Entries (APPLIED.tsv, 22): 5 done, 17 blocked
- **Done:** q10445, q10462, q10472, nf41:e81 and nf41:e47, each checked against every other landed binder of its target.

## Rows and checks
- **Section 1: 4 self renames, all leaf-only (CList AddScrollingChildAndSetOffsets / RemoveChild /
  AddScrollingChildDynamicallyAndSetOffsets, CThingCreatureBase::WorldUpdate); no file moved. Section 2: 38 respelled bindings.**
- **Funclets:** none include a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck.log`):** all 7 rows graded from the repo after landing: 7 RELOCATION_MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 30 binding lines and added 15; bindings_multi_check is at its baseline (`NullCoord`).
- One grade of 0x004B11A0 bound the non-const `LowerBound` at the const address 0x004365B0. NF66's binding comparison caught it, and
  the landed form binds the const one.

## Headers (opt-ins in 10 files; lines added only)
NUISystem_CList.h, CThingCreatureBase.h (2), CThingGameObject.h, CTCPhysicsBase.h, NThingShot_CTCShotDefinitionBase.h, CQuestManager.h,
CAnimSet.h, CTCCarrying.h, CFrontEndManager.h and CUserProfileManager.h. The last two serve only the blocked drafts of
0x00596CCE / 0x00597006. Details: `naming/2026-11-17_agentNF66/hdr/README.md`.

## Name fixes queued (`land1117fu`, 5)
0x006666A0's base callers 0x006AD9D0 / 0x00834760 (still QAE through local models), the STLport `find` forwarders 0x0053C91C /
0x0053C8F8, and the descriptive setter 0x00662450. 0x00693250 has an Ego_r anonymous-namespace name whose hash changes per file, so
it can not be matched exactly. 0x0042ABCA (header binds `~_List_base<CComponent*>`, the self row is `~list`) is in NOTES only: NF66
could not prove which is right.

## Rule slips
- One Bash call exited 1 (a `diff` without `; true`).
- 0x0053BF20 was ported without being in rows.txt (it had no lock, so nothing collided).
