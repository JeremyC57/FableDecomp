# NF349: size-mode GetDef<T> copies in the 40 rows NF346 skipped (Account A naming queue, 2026-11-24)

## Result in one paragraph
The 2026-11-20 size-pragma scaffolding works for all four copies, in the landed per-type form. The brief's literal text (the pragma pair
wrapped around the GETDEF_GLOBAL_BODY template) does nothing, as the header's CEntitySoundDef block already says: measured on all 40 rows,
every copy stays 108 B. The working text is four add-only nested opt-ins in engine/CDefinitionManager.h. Each is a `template<> __declspec(noinline) inline`
explicit specialization of GetDef<T>(long, CDefPointer<const T>*) with the GETDEF_GLOBAL_BODY body token for token, between
`optimize("y", off)` / `optimize("s", on)` and `optimize("", on)`. Each row gets the NF346 REREAD define plus the opt-in(s) for its type(s).

| Dir | Rows | Verdict | Size-mode copies fixed |
|---|---|---|---|
| batches/b1 | 19 | ACCEPT | 20 (0x0042AF3C x12, 0x0042B040 x6, 0x0042B312 x1, 0x005F81BE x1) + 0x0074F760 (REREAD alone) |
| batches/b2 | 19 | ACCEPT | 19 (0x0042AF3C x19) |
| held/x1 | 2 (0x005E5F30, 0x006096E0) | HOLD_BRIEF, out of every batch | 3 measured |

Copies: 42 of 42 size-mode copies turn 98 B EQ under both checkers (0x0042AF3C x31, 0x0042B040 x8, 0x0042B312 x2, 0x005F81BE x1).
39 of them are in b1 / b2, and 3 are in the two held rows. 0x0074F760 GetDef<CObjectFamilyDef> in 0x0074E860 turns EQ with REREAD alone and lands in b1.
Nothing is left in the 38 batch rows: 0 leaf-called NEQ GetDef copies remain under either checker.

## The header change (hdr/engine/CDefinitionManager.h; HEAD copy in orig/hdr_CDefinitionManager/)
- **Shape.** One new `#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY` block inserted right after the existing one. It is pure addition: `diff` shows one hunk, `235a236,352`, and no existing line changes.
  - Inside are four nested opt-ins:
    - FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGBASEDEF_SIZE_BODY
    - _CTHINGOBJECTDEF_SIZE_BODY
    - _CUIMISCTHINGSDEF_SIZE_BODY
    - _CEXPRESSIONDEF_SIZE_BODY
  - Each opt-in forward-declares its type with the class key and namespace of its engine header (`class`, and NUISystem for CUIMiscThingsDef), and carries its own pragma pair.
  - A shared guard raises `#error` when any of them is defined without FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD. The 98 B form needs the re-reading SetDynamicCast; NF346 measured the REREAD-less size-mode copy at 101 B in its `optimize("s")` rows; not re-measured here.
  - The block is marked `Fidelity: scaffolding, not retail` and follows the landed precedents (GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY, _CCREATUREDEF_SIZE_BODY, ENGINE_GETDEF_PASS27), with the `_MSC_VER` guard of the CEntitySoundDef form.
- **Why per type, not a wrap.** VC7.1 takes a template instance's optimize mode at the end of the TU. The W variant
  (work/var/<a>_W.cpp + work/hdrW: the template definition wrapped in the pair) leaves all 43 copies byte-identical to REREAD-only
  (evidence/sizes_HRWN.txt: W = R for every copy).
- **Symbol spelling unchanged.** The specialization emits the same COMDAT symbol with the same COMDAT selection. objsec REREAD-only v new
  (evidence/objsec_RN/) shows only the opted-in GetDef<T> COMDATs changing, and equal symbol tables.
- **Comment edit after the compiles.** One comment line was edited after the objects were built (it now cites the owners' flags /O2 /Oy /W3).
  - `cpp -fpreprocessed` output of the compiled header and the final header is identical (md5 782f1f45...).
  - The line count is the same.
  - The final header is the one used for the vland "new" recheck and the final equiv_check.

## The three conditions, per row
- **(a) The row's own body grades the same, and only the target COMDATs change.**
  - The scratch grade is RELOCATION_MATCH for H, R and N.
  - vland --recheck-landed is the same HEAD v new: batches/*/evidence/grade_head_v_new.txt. HEAD uses the repo headers; new uses `FABLE_HDR_OVERLAY` = a full copy of rebuild/include carrying the new header.
  - objsec H v new is OK: the leaf is identical, and only GETDEF_EQ, NOADDR SetDynamicCast helper bodies and dropped NOADDR Assign helpers differ (the NF346 classes).
  - objsec REREAD-only v new: only the opted-in GetDef<T> COMDATs differ.
  - tools/leafrel.py also reads each leaf data-equal and relocation-equal H v N, keyed by stable section keys: evidence/leafrel/accept38_HvN.txt, 38/38.
- **(b) The owner row sets the same pragmas itself.** All four owners set `#pragma optimize("y", off)` and `#pragma optimize("s", on)` at row head.
  None has a CompilerFlags entry, so each builds at /O2 /Oy /W3. That is the 2026-11-21 refinement: the pair restores the owner's mode, and the opt-in comment cites the owner files and their flags. The owners:
  - 0x0042AF3C rebuild/src/compiled/00/42/CDefinitionManager_GetDefT_0042af3c.cpp: a local model with the stand-in name GetDef_0042af3c and no Leaf line.
  - 0x0042B040 .../CDefinitionManager_GetDefCThingObjectDef_0042b040.cpp: a local model, a strong `template<>` specialization.
  - 0x0042B312 .../CDefinitionManager_GetDef_0042b312.cpp: a local model, an explicit instantiation.
  - 0x005F81BE .../00/5f/CDefinitionManager_GetDef_005f81be.cpp: the shared-header GETDEF_GLOBAL_BODY template plus REREAD.

  Only 0x005F81BE proves that the shared-header text plus REREAD in size mode gives the 98 B body by itself. The other three prove the pragmas
  and the retail body; the new objects then show it. Rule note: the 0x0042B040 owner defines the same symbol strongly. Those COMDAT copies
  already existed at HEAD, so this is not a new kind of conflict. The owners' ports onto the header are queued below.
- **(c) No row-head optimize pragma comes before the include.** No row source carries an optimize / auto_inline / inline_depth pragma.
  tools/pragscan.py on the PREPROCESSED new TU finds 0 optimize pragmas before the first NF349 block in all 40 rows (evidence/pragscan_c.txt),
  and the specialization count equals the row's opt-in count. 0x006D0630 also uses fable_map.h's STLP_TREE_FIND_LONG_LONG_SIZE_BODY; that
  block comes after ours and resets to the command line itself.

## Method
1. **Screen.** tools/screen.py and screen346.py found all 40 clear. The `acctB_b24` text hits on 0x006ACA30 / 0x0072F170 / 0x00730630 are
   comments in B's header overlays (as NF346 found). B's staged pending/2026-11-24_acctB_b24/hdr/00402510/engine/CDefinitionManager.h adds a
   STARTUP_SETTERS opt-in in another region (add-only, no overlap: `diff3 -m` of ours / HEAD / B's overlay gives 0 conflicts,
   work/merge_result_diff3.h).
   The nf340 T3 lines 0x0042B040 / 0x0042B312 are the debt lines of these very copies (see "Queue" below). NF347 / NF348 held no locks; NF344's two locks are not ours.
2. **Order.**
   - Precheck the landed tests with clang. 10 fail only in VC7.1 headers (`<cwchar>`, `<functional>`, iostream), or on MSVC-only syntax in unchanged
     sources (0x006C14D0 explicit specialization, 0x007729A0 `_assert` / parse); all 10 then passed `cl /Zs` (D1 / NF300; work/pre_zs.log).
     0x006ACA30 passed with -fno-wchar.
   - Lock as NF349.
   - Precheck H / R / W / N with clang against the same include tree each compile uses (work/pre.log, 160/160 PASS).
   - Compile, in a later call.
3. **One tree, one path.** Every compile goes through ONE full copy of rebuild/include (work/tree), whose CDefinitionManager.h is swapped
   per pass (tools/runprobe.sh), at one path per row (work/one/<a>/). Two traps found on the way:
   - **Includer lookup.** A thin overlay holding only engine/CDefinitionManager.h can lose to the repo copy through MSVC's
     includer-directory lookup. Only a full tree copy is safe.
   - **Concurrent runners.** Two runners compiling the same row at the same path clobber each other's source. My first probe did this
     and was discarded and redone sequentially.

   The planted `#error` control was dropped because wine cl hangs on a hit #error (GOTCHAS agentNF24): it was killed by PID. The proof that
   the overlay was used is N v R: only the new header reads the new macros, and every target copy changed 108 -> 98 B.
4. **Checkers.** pk_all.py (`--tsv --row-length=<catalog> --leaf`) and pickany_vs_retail.py run on H, R, W and N (evidence/pk/).
   - All 160 pk_all headers read source_a=yes, and no pvr Traceback.
   - pkcmp346 H v N: 0 TURNED_NEQ, 0 NEWLY_NEQ in both checkers. b1 GATE PASS with 42 FIXED lines, b2 GATE PASS with 38 (both checkers).
   - The reversed planted control FAILs in each batch (pk_gate_control_reversed.txt).
5. **Header includer closure** (evidence/hdr/).
   - tools/closure.py builds the reverse include closure of engine/CDefinitionManager.h: a BFS over rebuild/include plus every catalog row
     source and test, following quoted and angle includes resolved against the includer's directory and rebuild/include, `../src/compiled`
     folds included. It finds 1,210 rows.
   - equiv_check.py, HEAD tree v new tree, over those rows: 0 differing (rerun with the final header: evidence/hdr/equiv_final_new.tsv is empty).
   - Planted control: `typedef int nf349_planted_control;` inside the EXISTING GETDEF_GLOBAL_BODY block, outside the new blocks. It flags
     exactly 160 rows (160 src + 6 test lines). These are exactly the 160 closure rows whose source or test `#define`s GETDEF_GLOBAL_BODY:
     cross-checked, 0 missed, 0 extra.
   - hdrcheck2.py also ran: all `=same`. It ALSO reads the plant as `=same`, because it defines one macro at a time and never reaches the
     GLOBAL_BODY region. So it is vacuous for this header; equiv_check is the proof.
   - No existing macro changes. The new macro names occur nowhere in the repo before this change (grep).
6. **Grade.** vland --recheck-landed, HEAD payload (landed text, repo headers) v new payload (rows/, full-tree overlay with the new header):
   19/19 and 19/19 RELOCATION_MATCH PASS before and after (held/x1: 2/2). The WARN sets are identical HEAD v new.
   - **The overlay really applied in vland.** The objects vland kept carry the size-mode targets at 98 B in new and 116 B in HEAD:
     b1 20/20, b2 19/19, x1 3/3.
7. **Dropped helpers** (the stand-in for a link check, as NF346). 22 distinct NOADDR `?Assign@?$CDefPointer@...` COMDATs drop H v N.
   - reloc_bindings.tsv names none of them.
   - 731 objects scanned: mine (H / R / W / N), NF346's 231 and NF341's 340. 0 relocations outside their own COMDAT name them, and there
     are 0 UND entries (evidence/helper_refs_scan.txt, tools/helperscan.py).
   - The external symbol tables are otherwise equal (objsec).
8. **Final-header precheck.** After the comment edit, all 40 new sources were prechecked again with clang against the tree carrying the
   final header (work/runprobe_finalpre.out).

## The two held rows (held/x1; brief: out of every batch, report separately)
- **0x005E5F30 and 0x006096E0.** With REREAD plus the opt-ins, their three copies turn 98 B EQ (0x0042B040 x2, 0x0042B312 x1), and the grade stays
  RELOCATION_MATCH. objsec346 reads LEAF=1 for H v R (REREAD alone, NF346's finding) and for H v N.
- **The LEAF verdict is an artifact.** tools/leafrel.py keys every relocation target by a stable section key (a COMDAT's symbol, or the
  section name plus its ordinal) instead of objsec346's raw section index. With that key the leaf is data-equal AND relocation-equal H v R,
  H v N and R v N (evidence/leafrel/005e5f30.txt, 006096e0.txt). leafrel's negative controls flag (evidence/leafrel/negative_controls.txt): a target GetDef COMDAT H v N and R v N, and a planted relocation-target swap. What objsec346 sees is `$L` labels whose section index shifted when the
  NOADDR Assign helper COMDATs were dropped: jump-table and EH-label relocations, same targets.
- **Ruling Q2 below.** If ruled in, held/x1 has the full evidence (pk_gate, objsec, objsec_RN, leafrel) and only needs its verdict flipped to
  ACCEPT: its vland recheck is done (held/x1/evidence/grade_head_v_new.txt, 2/2 RELOCATION_MATCH PASS, same).

## Queue / debt (for the coordinator)
- **T3 nf340 lines.** 0x0042B040 and 0x0042B312 name 0x005E4BE0 as their only emitter, which b1 fixes. Their 0x005E5F30 copies are still NEQ at HEAD:
  NF346 listed them in the skipped set, but the nf340 lines do not list them. The lines can close when b1 lands and x1 is ruled; otherwise keep them.
- **Owner ports onto the header** (EXTRA_RULES 2026-11-20: "queue the owner's port onto the header"): 0x0042AF3C, 0x0042B040 and 0x0042B312
  are local models. 0x0042AF3C still has the stand-in self name GetDef_0042af3c and no Leaf line; the PDB name GetDef<CThingBaseDef> is bound at
  0x0042AF3C in reloc_bindings.tsv (witness 0x007CBDC0). Porting each onto these opt-ins would remove the strong / stand-in definitions.
- **Scaffolding debt.** The four opt-ins are scaffolding debt by rule, like the other size bodies.
- **GetSubDef copies.** NEQ GetSubDef copies are untouched; per row they are in work/eval.tsv column remain_subdef: 26 lines over the 40 rows,
  unchanged H v N.

## Rulings needed
- **Q1 (form).** The brief described the opt-in as "wrapping the GETDEF_GLOBAL_BODY definition in optimize pragmas". That text was measured
  and does nothing. The landed form of the same rule (per-type explicit specialization, as GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY /
  ENGINE_GETDEF_PASS27) is what works, with one macro per type. Please confirm that this is the 2026-11-20 rule's device and not a new one.
- **Q2 (held/x1).** May 0x005E5F30 and 0x006096E0 join on the stable-key leafrel proof, now that objsec346's LEAF verdict is shown to be
  section-index renumbering? This applies to NF346's REREAD-only text as well. It also suggests objsec346 should key `$L` labels by a stable
  section key (tools/leafrel.py shows how).
- **HEADERS.tsv** uses the landed `class verdict macro notes` columns (as batches/2026-11-24_landing_nf337_b3); the header diff is in hdr/ v orig/hdr_CDefinitionManager.h.
- **Q3 (disclosure, not a request).** The comment line in the header was edited after the compiles; the comment-stripped text is identical (above).
- **Disclosure (rule slip).** The first merge test used `git merge-file` on scratch copies in work/. It touched no repository, but it is a git
  command, which the brief forbids. I deleted its output and redid the test with `diff3 -m`. No other git command was run.

## Files
- batches/b1, batches/b2: land_scripted run dirs. Each holds:
  - rows/, orig/rows, orig/tests, rows.txt, RESULTS.tsv, head/payload.json;
  - hdr/engine/CDefinitionManager.h, orig/hdr_CDefinitionManager.h, HEADERS.tsv;
  - pk_gate.txt and pk_gate_control_reversed.txt;
  - evidence/pk_gate, objsec, objsec_RN, pragscan_c.txt, grade_head_v_new.txt, and logs/recheck_{head,new}.log.

  Both carry the same header. It lands once, with or before the first batch.
- held/x1: the same layout, verdict HOLD_BRIEF.
- rows/ and orig/: all 40 new texts and their HEAD texts. Land only from batches/.
- evidence/:
  - pk/: H_, R_, W_ and N_ for both checkers;
  - objsec/ (H v N), objsec_RN/ (R v N), objsec_HR/ (the two held rows, H v R), helper_refs_scan.txt;
  - leafrel/, sizes_HRWN.txt, pragscan_c.txt;
  - hdr/: hdrcheck2.txt and hdrcheck2_plant.txt, plus the equiv_check outputs and closure list.
- tools/: NF346's tools retargeted to NF349, plus new ones:
  - mknew349.py, runprobe.sh (one-tree swap runner), presrc.sh, pkrun349.py, gdsum.py, objsec349.sh;
  - eval349.py, pragscan.py, closure.py, leafrel.py;
  - mkbatch349.py, vlandchain349.sh, mkstatus349.py.
- work/: objects, logs, trees and the vland payload / oracle files. The oracle files hold retail bytes: not for hand-back.
