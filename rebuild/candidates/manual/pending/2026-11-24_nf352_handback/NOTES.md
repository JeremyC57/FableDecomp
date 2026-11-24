# NF352: size-mode CCountedPointer<NUISystem::CComponent> copies (T3 nf340 0x00429C15 / 0x00429C53 / 0x00429C6F) (Account A naming queue, 2026-11-24)

## Result in one paragraph
NF349's per-type size-pragma specialization reaches retail's body for **Reset<CComponent> 0x00429C6F only**. That copy turns EQ in both
rows that call it out of line (0x00605570, 0x00626110): 103 -> 59 B. Nothing else in either object changes. The (T*) constructor
0x00429C15 and the destructor 0x00429C53 cannot reach retail's body this way. As `__declspec(noinline)` (the NF349 form) or as a plain
`inline` specialization (the NF225 form already in this header), each one moves the 0x00626110 and 0x00626750 leaves (all DIFFER;
evidence/probe_grades.txt). They are STATUS skipped. Per the coordinator's update, 0x00626750 is handed back once in b1 on NF350's
approved text alone, proved against repo HEAD. The ReleaseData<CComponent> 0x004291DE size body, a fourth member outside the brief's
three addresses, also works. It is offered as the ruling alternative held/r1 (Q1b). DeleteData<CAction> 0x00429F43 is skipped: the
device keeps the leaf and 0x00428B10, but the copy stays NEQ (12 B v 21).

| Dir | Rows | Verdict | Copies fixed |
|---|---|---|---|
| batches/b1 | 00605570, 00626110, 00626750 | ACCEPT | Reset 0x00429C6F x2 (EQ 59); 00626750 = NF350's MAP_INDEX_DECL_ONLY text (??A 0x005949C3 no longer emitted, owner defines it) |
| held/r1 | 00605570, 00626110 | RULING Q1b (land instead of b1's text for these two rows) | Reset x2 + ReleaseData 0x004291DE x2 (EQ 44) |

## The header change (hdr/engine/CCountedPointer.h; HEAD copy in orig/hdr_CCountedPointer/)
- **Add-only.** One new block after the existing NF225 block's `#endif`, before the include guard's `#endif`. `diff` shows one hunk,
  `538a539,601`, and no existing line changes. The new macro names occur nowhere in the repo (grep).
- **Contents.** Two opt-ins:
  - FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY (used by b1 and r1);
  - FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY (used only by r1; with no user it emits nothing). If Q1b is ruled
    out, the coordinator may drop that `#ifdef` block before landing; b1's objects do not depend on it.
- **Form.** Each opt-in is `template<> __declspec(noinline) inline` (`template<> inline` outside `_MSC_VER`) between
  `#pragma optimize("s", on)` and `#pragma optimize("", on)`. It forward-declares `namespace NUISystem { class CComponent; }` and is
  marked `Fidelity: scaffolding, not retail`. ReleaseData comes first because Reset calls it.
- **Pragma.** Only `("s", on)`, not NF349's `("y", off)` pair: the four owners (0x00429C15, 0x00429C53, 0x00429C6F, 0x004291DE) each set only
  `#pragma optimize("s",on)`. None has a CompilerFlags entry (bounded catalog lookup), so all build at /O2 /Oy /W3.
- **Bodies are the owners' configuration, token for token.** Reset: the default body. ReleaseData: the FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
  body (Data zeroed before Info). The rows' own header configuration is not used.
- **Ctor and dtor blocks were measured and then removed from the final header.** The compiled header (work/hdr4) also carried CTOR
  and DTOR blocks, inactive in every final text. `cpp -P` of all four final / r1 sources is byte-identical with the compiled header and
  with the final header (evidence/hdr/final_vs_compiled_cpp.txt). The final objects were then recompiled with the final header, and
  objsec reads them identical to the probe objects.
- **COMDATs.** The changed COMDATs keep their symbol and selection (2, any). Alignment goes 16 -> 1 (size mode; retail's copies sit at
  odd addresses). Recorded by tools/comdatsel.py.

## The three size-pragma conditions
- **(a) Grade unchanged, leaf identical, only the target COMDATs change.**
  - Scratch grade: H and N are RELOCATION_MATCH for all rows.
  - vland --recheck-landed:
    - HEAD payload: landed text, repo headers.
    - New payload: rows/, with FABLE_HDR_OVERLAY = a full copy of rebuild/include carrying the new header.
    - Result: RELOCATION_MATCH PASS before and after, b1 3/3 and r1 2/2 (batches/b1/evidence/ and held/r1/evidence/ grade_head_v_new.txt).
    - The overlay really applied: vland's kept objects carry Reset 59 (H: 103), and in r1 also ReleaseData 44 (H: 53).
  - objsec352 (stable section keys): b1 changes only the Reset COMDAT, r1 only Reset + ReleaseData. The symbol tables are equal and
    nothing is added or dropped.
  - leafrel: the leaf is data-equal and relocation-equal H v N in every row (evidence/leafrel/).
- **(b) The owners set the same pragmas.** Reset 0x00429C6F
  (rebuild/src/compiled/00/42/CCountedPointerNUISystemCComponent_Reset_00429c6f.cpp) and ReleaseData 0x004291DE
  (.../CCountedPointerNUISystemCComponent_ReleaseData_004291de.cpp) each set `#pragma optimize("s", on)` with
  FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA. Neither has a CompilerFlags entry. The opt-in comment cites both, with their flags.
- **(c) No optimize pragma before the block.** tools/pragscan352.py runs on the preprocessed final TUs (evidence/pragscan_final.txt). It
  finds 0 optimize / auto_inline / inline_depth pragmas before the block, and the specialization count equals the row's opt-in count
  (1/1 in b1, 2/2 in r1). 0x00605570's row-head `#pragma auto_inline(off)` comes after its includes.

## Gates
- **Both pick-any checkers, HEAD v new** (pkcmp346, sides from pk_all.py `--tsv --row-length --leaf` and pickany_vs_retail.py).
  - b1: 0 TURNED_NEQ and 0 NEWLY_NEQ; FIXED Reset in 00605570 and 00626110; 00626750 shows 2 DROPPED (??A, ??8) and nothing turned.
  - r1: FIXED Reset + ReleaseData.
  - The reversed controls FAIL in every row (batches/b1/ and held/r1/ pk_gate_control_reversed.txt).
  - All pk_all headers read source_a=yes (only the DIFFER probe P3 lacked it).
- **0x00428B10 DeleteData<CComponent>** (watched, per the advisor): EQ 11/11 in every accepted object.
- **Drop test:** 0 dropped symbols in 00605570 / 00626110 (b1 and r1).
  - 00626750: 20 dropped. tools/droptest.py output is identical to NF350's on the same text.
  - No reloc_bindings line witnessed by the row names a dropped symbol.
  - The EQ ??8 0x0041C820 is still defined by 0058a0c7, 005949c3 and 00626110. My new 00626110 object still defines it, unchanged.
  - ??A becomes UND, and the landed owner 0x005949C3 defines the exact symbol (NF350 link_p2).
  - NF350's census is copied as evidence/drop_census_00626750_from_NF350.tsv.
- **Includer closure** (evidence/hdr/):
  - tools/closure.py (NF349's tool: quoted and angle includes, includer directory, `../src/compiled` folds) gives a 13,751-row reverse
    closure of engine/CCountedPointer.h.
  - equiv_check over it, HEAD tree v final tree: 0 differing rows (equiv_new.tsv is empty).
  - Planted control: `typedef int nf352_planted_control;` at file scope directly above the new block (plant_CCountedPointer.h).
    - It flags 12,535 files (12,211 rows) of the 13,751-row closure (equiv_plant.tsv). The closure overcounts: some quoted includes
      sit under inactive `#if` arms.
    - Independent cross-check, tools/reachscan.py: for every closure file, the preprocessed text is tested for the CCountedPointer.h-only
      token `CCPPointerInfo(unsigned long refCount`. It finds 12,597 files.
    - Plant minus reach: 0.
    - Reach minus plant: 62 test files. `cpp -M` shows that none of the 62 includes engine/CCountedPointer.h: each pastes a local copy
      of the model into its own text.
    - So the plant flags exactly the true includers, 0 missed (plant_crosscheck.txt).
  - hdrcheck2 was not run. NF349 found it vacuous for a header like this, and equiv_check plus the plant is the proof.

## 0x00626750 (coordinator update)
- The row carries only NF350's change: its b3 text, with FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY and its `// Fidelity: scaffolding, not
  retail` comment. orig/rows is repo HEAD.
- My size-body change does not apply there: every CTOR / DTOR variant, both forms, on HEAD text and on NF350's text, gives DIFFER.
  So NF350's and my changes do not meet in this row. The row gains no NF352 define and needs no merged recheck beyond the one done:
  vland H v N with the new header overlay, RELOCATION_MATCH PASS both.
- Proved here against repo HEAD (compiled at my path, lock NF352): grade, objsec (20 dropped, 0 added, 0 changed), leafrel, both
  checkers with the reversed control, and the drop test.

## DeleteData<CAction> 0x00429F43 (skipped; Q3)
- **Attempt.** One bounded attempt, on 0x00626110 (Prr base): a `template<> __declspec(noinline) inline` DeleteData<NUISystem::CAction>
  size specialization at the end of a SCRATCH copy of NUISystem_CAction.h. It cannot sit in CCountedPointer.h, because CAction must be
  complete there. The scratch header is not handed back.
- **Result.** Grade and leaf kept; 0x00428B10 stays EQ 11/11; only that COMDAT changes (70 -> 12 B). The copy is still NEQ against
  retail's 21 B.
- **What the 12 B is.** The body is `test ecx,ecx; je; push 1; call ??_GCAction@NUISystem@@QAEPAXI@Z; ret`: it calls the scalar
  deleting destructor COMDAT, where retail's 21 B performs the destruction inline.
- **Cause open.** The landed owner row 0x00429F43 (optimize("s", on), the same shared NUISystem_CAction.h, no extra define) is
  landed at 21 B. So the shared model can give retail's body in a size TU. Why this row's TU calls ??_G instead is not established:
  perhaps the speed-mode ??_G / ~CAction COMDATs this TU also emits (NEQ 57) change the inliner's price.
- **Skipped,** per the brief. DELETEDATA_DECL_ONLY was not used.
## Debt left (T3 nf340 lines)
- 0x00429C15 (emitters 00626110, 00626750) and 0x00429C53 (00626110, 00626750; 00605570 not leaf-called) stay NEQ debt. Measured
  above: no size-body form keeps the leaf. Suggested T3 note: "NF352: size-body specialization (noinline or inline) moves both leaves;
  not fixable by the size device."
- 0x00429C6F: closes when b1 (or r1) lands. Its two emitters are fixed.
- 0x00429F43: still debt (see above).
- ReleaseData 0x004291DE NEQ copies in 00605570 / 00626110: unchanged in b1. In b1 the EQ Reset copy now calls that NEQ copy (pk_all
  reach n -> y; it was NEQ at HEAD, so this is not a gate failure). r1 fixes it.
- Owner ports onto the opt-ins (EXTRA_RULES 2026-11-20): 0x00429C6F and 0x004291DE are local-model explicit instantiations. Queue them.

## Rulings needed
- **Q1 (b1 side effect).** In b1 the ReleaseData<CComponent> copy (NEQ 53 v 44) is now reached from the leaf through the EQ Reset copy.
  - pk_all reach goes n -> y in 00605570 and 00626110. At HEAD that copy was NEQ and not reached (00605570), or not leaf-referenced
    (00626110).
  - pkcmp346 passes, because the copy is neither new nor newly NEQ. The NF340 wording ("a leaf-called NEQ copy that is new, or that
    turns NEQ") could still be read against it.
  - Does b1 stand as it is? held/r1 avoids the question by fixing that copy too.
- **Q1b.** May ReleaseData<CComponent> 0x004291DE take the same size-body opt-in (r1)? It is a fourth member outside the brief's three
  addresses. It meets the same three conditions: owner sets optimize("s", on); only the target COMDATs change; pragscan C_OK. It turns
  a pre-existing NEQ copy EQ in both rows. If yes, land r1's texts for 00605570 / 00626110 instead of b1's. If no, land b1 and drop the
  RELEASEDATA block from the header.
- **Q3.** DeleteData<CAction> 0x00429F43. A second attempt would need an opt-in in a second shared header (NUISystem_CAction.h, which
  is where CAction is complete), with its own closure proof, plus finding why this TU calls ??_GCAction. Not started without a ruling.
- **Q2 (disclosure).** Condition (a) says "only that one COMDAT changes". In r1 two COMDATs change, both opted-in targets.

## Consistency checks
- **pk_all leaf.** Every pk_all run passed `--leaf=<the row's // Leaf:>`. In all 27 pk_all files the first `# leaf` line names the row's
  own function (evidence/pk_leaf_check.txt; the one source_a=no file is the DIFFER probe P3). pickany_vs_retail also got `--leaf`.
- **Header copies.** hdr/engine/CCountedPointer.h, batches/b1/hdr, held/r1/hdr and the tree used for the new vland run are
  byte-identical, and orig/ equals repo HEAD (cmp).
- **Staged overlays.** 37 staged CCountedPointer.h overlays exist outside this folder (pending/*, $S/w22/agent*).
  - One diff3-merges cleanly. The other 35 that differ conflict because they are older versions of the header, not because of this
    block: they diverge from HEAD itself.
  - Their only macros missing from HEAD are 2026-11-02 / 11-08 / 11-13 work: QUICK_ACCESS, CALLED_DTOR and the acctB parked
    CTOR_INFO0 / CTOR_TERNARY.
  - No current hand-back stages a CCountedPointer.h change (evidence/hdr/overlay_diff3.txt).
- **NF350's b3 dir.** TABLE_CHANGES.tsv, UNITS.tsv and HEADERS.tsv are header-only (empty); hdr/ is empty. rows/00626750.cpp is
  byte-identical to my copy. Its RESULTS line is reproduced in substance by b1's 00626750 line and evidence.

## Method and rule notes
- **Locks.** 00605570 claimed at 12:01. 00626110 / 00626750 were NF350's at start (11:46 / 11:35). I did not touch them until both
  read `released` (12:02:50), then claimed them as NF352. All released at the end (evidence/locks.log).
- **Prechecks.** Every compiled source and the landed tests passed clang before their compiles: work/pre.log, plus a planted-error
  control that FAILs.
- **One path, one runner.** Every compile ran through ONE full include-tree copy (work/tree), with the header swapped per pass
  (tools/runprobe352.sh, with md5 per pass in work/runprobe_hdr.log), at ONE path per row. All runners were background and sequential.
- **Stopped run.** One background equiv_check plant run (on the superseded 4-block header) was stopped by its own PID and its outputs
  deleted; both equiv runs were redone on the final header.
- No git, no repo writes. Retail bytes appear only in the vland oracle files in work/.

## Files
- batches/b1, held/r1: land_scripted run dirs. Each holds:
  - rows/, rows.txt, RESULTS.tsv, head/payload.json;
  - orig/rows, orig/tests (tests unchanged, so no tests/);
  - hdr/engine/CCountedPointer.h, orig/hdr_CCountedPointer.h, HEADERS.tsv;
  - pk_gate.txt, pk_gate_control_reversed.txt, logs/recheck_{head,new}.log;
  - evidence/ (pk_gate/{H,N}_*, objsec/, leafrel/, droptest_*, pragscan_c.txt, grade_head_v_new.txt).
- evidence/:
  - probes.tsv (per-probe copy verdicts), probe_grades.txt (every probe's grade);
  - pk/, pk_r1/, objsec/, leafrel/, droptest_*.tsv, pragscan_final.txt, vland_overlay_sizes.txt;
  - hdr/ (closure, equiv, plant, reach cross-check), objsec_Adel_00626110.txt, locks.log.
- tools/: runprobe352.sh, pass.py, nf.py (+ROWDIR), pkrun352.py, objsec352.py, comdatsel.py, pragscan352.py, reachscan.py,
  mkbatch352.py, vlandchain352.sh, plus the NF349 / NF347 tools they reuse (leafrel, closure, pkcmp346, droptest).
