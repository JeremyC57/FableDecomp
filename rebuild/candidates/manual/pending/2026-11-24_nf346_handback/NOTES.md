# NF346: the GetDef<T> REREAD port (T3 nf341 D3), Account A naming queue, 2026-11-24

## What this is
- **Change.** One line per row: `#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`, placed right after the row's
  `#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY`.
  - It is the existing CDefPointer.h opt-in: the retail SetDynamicCast form, which re-reads the source after the release.
  - Owner 0x0066C340; landed 0x0066B370 and batches/2026-11-24_landing_nf341_r2 already use it.
  - No header change. No other line of any row changes (`diff orig/rows rows` is one added line per row; tools/mknew.py).
- **Hand-back.** Three land_scripted run dirs of ACCEPT rows plus one HOLD dir:

| Dir | Rows | Verdict | GetDef copies fixed |
|---|---|---|---|
| batches/b1 | 21 | ACCEPT | 24 |
| batches/b2 | 20 | ACCEPT | 21 |
| batches/b3 | 20 | ACCEPT | 20 |
| batches/p1 | 3 | HOLD_PARTIAL (ruling Q1) | 4 (+3 left) |

  land_scripted lands only ACCEPT, so p1 stays inert until the ruling.
- **Copies.**
  - 65 leaf-called NEQ GetDef<T> copies fixed in 61 rows (ACCEPT).
  - 4 more in the 3 p1 rows, pending Q1.
  - 46 left in the compiled set (see Counts).

## Candidate set and the recount on today's HEAD
- **Source list.** NF341's round-1 HEAD pk_all evidence ($S/w22/agentNF341/evidence/pk/H_*.pkall.tsv): 123 leaf-called NEQ GetDef<T>
  copies in 109 rows. tools/d3list.py reproduces that exactly. With GetSubDef copies counted as well it gives 160 in 110 rows; the extra row,
  0x00607330, has only a GetSubDef copy.
- **Today's HEAD.** tools/gbdefiners.py lists 160 landed GLOBAL_BODY definers, 23 of which already define REREAD.
  - New since NF341's compiled set: only 0x00839940 (Account B, now landed with REREAD).
- **Compiled set: 104 rows.** These are the 109 rows minus:
  - the four nf341_r2 rows;
  - 0x00839940;
  - 0x00821A80, which already defines REREAD;

  plus 0x006628E0 (see Exclusions). Each row was compiled at HEAD and with the define, at one path per row.
- **Recount from today's HEAD objects** (evidence/d3_recount.tsv, tools/count346.py): 115 leaf-called NEQ GetDef copies in the 104 rows.
  - Reconciliation with NF341: 123 - 7 (nf341_r2) - 1 (0x00839940) - 1 (0x00821A80) = 114, + 1 (0x006628E0) = 115.
  - 0x00821A80 was compiled read-only at HEAD (evidence/pk_x/). Its GetDef<CDefClassBase> 0x0042B144 copy is 108 B NEQ with REREAD
    already defined, so it is a copy this define cannot fix.
- **The 22 other clear non-REREAD definers** (160 - 23 REREAD - 104 compiled - 11 non-REREAD holes) were not on NF341's list.
  - Each HEAD object was compiled read-only today and run through both checkers (evidence/pk_rest22, evidence/recount_rest22.tsv).
  - None emits a GetDef copy, so the recount is complete for every clear definer.
- **Not compiled, so not counted:** the 12 stay-off GLOBAL_BODY holes below (11 of them without REREAD).

## Exclusions applied (STATUS `excluded`)
- **Done in nf341_r2:** 0x008F7950, 0x00703150, 0x007F0300, 0x0082D5C0.
- **Account B:** 0x00839940.
  - It has an open B claim, is UNMATCHED `reserved (acctB B24B)`, and is staged in pending/2026-11-24_acctB_b24/rows.
  - B's staged rows are 00402510 00407a70 00407ba0 00839940 00975bb0 00be4fe0 00d04ae0 00d11930 00db3fa0 00de9690 00ed8680 012133c0. None
    is a candidate except 0x00839940.
- **Screen false positives** (tools/screen346.py, the `acctB_b24` text search): 0x00652FA4, 0x006ACA30, 0x0072F170, 0x00730630, 0x00821A80
  and 0x0087F3A0 matched only in comments inside B's header overlays (hdr/<row>/engine/*.h), not as staged rows. They were treated as clear.
- **GLOBAL_BODY holes NF341 did not compile.** Re-screened today, still excluded:
  - T3 column 2: 0x00584B86 (nf307), 0x005D9AA0 (nf302), 0x006AC200 (nf303, also codex RESERVED), 0x007EAC10 / 0x007FC210 (nf303),
    0x00801920 (nf307), 0x0082D2D0 (nf314).
  - T3 column 3: 0x0040C790 (nf297; it already defines REREAD).
  - B claims: 0x007BD2D1 and 0x00859EF0 (claimed); 0x00591B7F and 0x00774D20 (B's `done` is the newest claim, COORDINATION section 3).
- **0x006628E0 is clear today and was compiled.**
  - NF341's screen saw it in T3 column 2. That line is gone.
  - It now appears only in the reason column of nf310 0x006A5AF0 ("0x006628E0 is off limits: self + vtable unit, port").
  - The old column-2 line (nf290 0x006628E0, NF290 entry 2) was closed by the NF342 stale-T3 audit
    (naming/audits/2026-11-24_t3_stale_nf342/T3_CLOSED_STALE.tsv).
  - NF316 Q3: a reason-column mention does not reserve, and that note concerns its self respell, not this define. It lands in b3
    (disclosure, not a ruling request).
- **0x00607330:** no `#define` (GLOBAL_BODY is named only in a comment), and its only NEQ copy is a GetSubDef copy.
- **Codex reservations** (every pending/*codex_pass*/RESERVED.tsv and the *_open dir names) and UNMATCHED_ROWS.tsv: no candidate hit
  except 0x00839940.
- **Locks.** No candidate was locked by another agent (NF345 holds 18 locks, NF344 2; none of ours).

## Method (per row)
1. **Order.** Precheck the landed test (clang -fsyntax-only). 17 unchanged landed tests fail clang and passed VC7.1 `cl /Zs` instead
   (work/pre_zs.log, D1):
   - 14 fail only inside VC7.1 headers (`<cwchar>`, `<functional>`, iostream);
   - 0x0064E978 (50 errors) and 0x007729A0 (9 of its errors) fail on `_assert` calls (no matching function), which D1 lists;
   - 0x006C14D0 fails on an explicit specialization without `template<>`, MSVC-only syntax in an unchanged source (NF300 D1 extension);
   - 0x007729A0 also has 2 parse errors (`typename`, `)`).

   The HEAD source of 0x00859EC0 (recount only) failed clang on the CTCFlourishTarget.h FABLE_STATIC_ASSERT typedef redefinition and
   passed `cl /Zs`. Then claim the lock as NF346. Then precheck the HEAD source and the new source (all 208 pass clang,
   work/pre.log). Then compile in a later call.
2. **Compile.** tools/runboth.sh (pass.py) builds HEAD and new at ONE path, work/one/<addr>/<addr>.cpp, so the `?A0x` hashes agree.
   - Objects: work/obj/H_<a>.obj and work/obj/<a>.obj.
   - Scratch grade: RELOCATION_MATCH for all 104, HEAD and new.
3. **Both checkers** (tools/pkrun346.py): pk_all.py `--tsv --row-length=<catalog length> --leaf=<Leaf>`, and pickany_vs_retail.py.
   - Every pk_all header reads `source_a=yes`; the tool prints SRC_A_MISSING otherwise, and none was printed.
   - tools/pkcmp346.py compares HEAD with new. A crashed or empty checker output counts as MISSING, never as SAME.
4. **objsec** (tools/objsec346.py). COMDATs are keyed by their symbol. Every difference is classified as one of:
   - LEAF;
   - GETDEF_EQ or GETDEF_NEQ (pk_all verdict in new);
   - SUBDEF;
   - HELPER_BODY (a NOADDR `??$SetDynamicCast@` of CDefPointer);
   - HELPER_DROP (a NOADDR SetDynamicCast / `?Assign@?$CDefPointer@`);
   - OTHER.

   OK means only GETDEF_EQ, HELPER_BODY and HELPER_DROP. Output: evidence/objsec/<a>.txt, and each batch has a copy.
5. **Dropped helpers** (evidence/helper_refs_scan.txt, tools/helperscan.py): 33 distinct dropped helpers in the ACCEPT and PARTIAL rows, all
   `?Assign@?$CDefPointer@...`.
   - reloc_bindings.tsv: 0 lines name them.
   - 369 objects scanned: my 104 HEAD and 104 new objects, plus NF341's 161 round-1 HEAD objects. 0 relocations outside their own
     COMDAT name them, and 0 UND entries.
6. **Grade.** vland `--recheck-landed` on the HEAD payload (head/payload.json, the landed text untouched; mkbatch asserts it equals the
   repo file) and on the new payload. Result: <batch>/evidence/grade_head_v_new.txt, logs/recheck_{head,new}.log. The oracle files stay in
   work/.
7. **Tool validation** (evidence/tool_validation.txt). pkcmp346 / objsec346 on NF341 r2's kept objects reproduce its 7 FIXED, GATE PASS,
   the reversed-control FAIL and its objsec classification. A HEAD object against itself reads SAME.

## Results
- **ACCEPT, 61 rows** (b1 / b2 / b3). Every gate passes:
  - grade unchanged, vland RELOCATION_MATCH PASS before and after;
  - both checkers: 0 turned NEQ, 0 newly NEQ, and no leaf-called NEQ GetDef copy left;
  - objsec: leaf identical, and only GetDef copies turning EQ, their NOADDR SetDynamicCast helper bodies, and dropped unreferenced NOADDR
    Assign helpers differ;
  - the per-batch planted reversed control FAILs.
- **HOLD_PARTIAL, 3 rows** (p1): 0x0069ADC0, 0x0082D3B0, 0x008338D0.
  - The define fixes their index-form copies: 4 copies, EQ in both checkers, objsec OK, grade unchanged.
  - Each row also has a leaf-called copy of the other overload, GetDef<T>(const CCharString&, CDefPointer<const T>*): CCameraManagerSetDef
    0x004AB990 (x2) and CBrainDef 0x006D3E80.
  - That copy is 112 B against retail's 104, NEQ at HEAD and byte-identical after (the define does not touch it). This needs ruling Q1.
- **Skipped, 40 rows.** In each, the define changes a GetDef copy but it stays NEQ. Every case is the same:
  - The affected copies are GetDef<CThingBaseDef> 0x0042AF3C (31 copies), <CThingObjectDef> 0x0042B040 (8), <CUIMiscThingsDef> 0x0042B312 (2)
    and <CExpressionDef> 0x005F81BE (1). Each goes from 116 B to 108 B; retail's copies are 98 B.
  - Retail's form is the size-mode body: frame pointer and `and [esi],0`.
  - Of the 18 copies of these four that DO turn EQ (101 -> 98 B), every one is in a row that carries `#pragma optimize("s", on)` (some
    also `("y", off)`). Every skipped row compiles them at plain /O2 /Oy.
  - Reaching retail's form would take a second device, a size-mode region; I did not invent one (brief). Measurement: evidence/objsec/<a>.txt.
  - 0x005E5F30 and 0x006096E0 also change their leaf section with the define.
  - One skipped row, 0x0074E860, also has a GetDef<CObjectFamilyDef> 0x0074F760 copy that the define turns EQ. It is not ported (STATUS).
- **GetSubDef copies.** No GetSubDef COMDAT changes in any of the 104 objects (objsec SUBDEF count 0), so the define does not touch them.
  - 36 leaf-called NEQ GetSubDef copies stay as they were: 10 in ACCEPT rows, 26 in skipped rows.
- **Other debt.** Pre-existing leaf-called NEQ copies in the ACCEPT and PARTIAL rows are unchanged HEAD to new: 85 copy lines, 55 distinct,
  in evidence/debt_neq_copies.tsv. They include LowerBound, STLport vector helpers and ??_GCCharString.

## Counts
| Class | Rows | HEAD NEQ GetDef | Fixed | Left |
|---|---|---|---|---|
| ACCEPT | 61 | 65 | 65 | 0 |
| PARTIAL | 3 | 7 | 4 | 3 |
| skipped | 40 | 43 | 0 | 43 |
| **compiled total** | **104** | **115** | **69** | **46** |

- 65 copies land now and 4 more wait on Q1.
- **Left: 46 copies.**
  - 43 in the skipped rows: 42 size-mode copies (0x0042AF3C x31, 0x0042B040 x8, 0x0042B312 x2, 0x005F81BE x1), plus 0x0074F760 in 0x0074E860,
    which the define would fix but which is not ported with its row. evidence/d3_recount.tsv has the exact per-row list.
  - 3 CCharString-overload copies in p1.
- **Also left, outside the compiled set:**
  - 0x00821A80's GetDef<CDefClassBase> 0x0042B144 (REREAD already on);
  - B's 0x00839940;
  - the 12 holes, uncounted.

## Rulings needed
- **Q1 (p1).** Can a row land when the define fixes some of its leaf-called NEQ GetDef copies, and another GetDef copy (the CCharString
  overload) stays NEQ and byte-unchanged? The NF340 gate treats a pre-existing NEQ copy as debt, but the brief says "a row whose copy stays
  NEQ with the define is skipped". If yes, flip p1's three RESULTS verdicts to ACCEPT. All evidence is already in batches/p1.
- **Q2 (the 40 skipped rows).** Their four GetDef copies need retail's size-mode body. Is a separate unit for them wanted? For example,
  the 2026-11-20 size-pragma scaffolding in CDefinitionManager.h, or per-row `optimize("s")` regions, together with this define. I made no
  attempt (brief: no second device).
- **Disclosure (0x006628E0, in b3; not a ruling request).** NF342's stale-T3 audit closed its column-2 line. Only the nf310 reason column
  still names it ("off limits: self + vtable unit"), and that does not reserve it (NF316 Q3). This change does not touch its self or vtable.
- **No link check was run.** The cover is objsec:
  - the external symbol tables are equal HEAD to new, except the dropped DEF entries of the NOADDR Assign helpers;
  - no UND entry is added;
  - the helper scan finds no reference to any dropped helper.

## Locks
- The 104 compiled rows and 0x00821A80 were locked as NF346.
- The 40 skipped rows and 0x00821A80 were released after measuring.
- The 64 batch rows (b1-b3, p1) are released before hand-back (prefix rule); see the final lock check in work/locks_final.txt.

## Folder layout
- **batches/b1, b2, b3** are the land_scripted run dirs (ACCEPT). **batches/p1** is the Q1 hold. Each holds:
  - RESULTS.tsv, rows.txt, rows/;
  - orig/rows and orig/tests (landed HEAD text);
  - head/payload.json;
  - pk_gate.txt and pk_gate_control_reversed.txt;
  - evidence/pk_gate (H_/N_ pk_all and pvr for each row), evidence/objsec, evidence/grade_head_v_new.txt;
  - logs/recheck_{head,new}.log.
- **rows/ and orig/ at the top level** hold the 64 batch rows. The 40 skipped rows' new and HEAD texts are in skipped_not_for_landing/,
  kept as the measurement source; never apply them. Land only from batches/.
- **evidence/:**
  - pk/: both checkers, HEAD and new, all 104 rows;
  - objsec/: all 104 rows;
  - pk_x/: 0x00821A80 HEAD;
  - pk_rest22/ and recount_rest22.tsv: the 22-row recount;
  - d3_recount.tsv, helper_refs_scan.txt, debt_neq_copies.tsv, tool_validation.txt;
  - fixed_by_copy.tsv: the 69 fixed copies by retail address (T, count, rows; p1 rows marked), for the T3 lines.
- **STATUS.tsv** has one line per candidate and per excluded row.
- **tools/** holds:
  - NF341's tools, retargeted to NF346: nf.py, pass.py, runzs.sh, screen.py and others;
  - new: d3list.py, gbdefiners.py, screen346.py, prelist.py, mknew.py, runboth.sh, pkrun346.py, pkcmp346.py, objsec346.py, eval346.py,
    helperscan.py, count346.py, mkbatch.py, vlandchain.sh, gradecmp.py, mkstatus.py.
- **work/** holds objects, logs and the vland payload / oracle files. The oracle files contain retail bytes: they are not for hand-back.
