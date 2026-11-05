# Plan phase 6 (2026-11-05 late): 0x0041E5F2 respell and the CThing header wave

This README covers `2026-11-05_plan_p6_w1` (wave 1, plus the phase's tools, APPLY.md, NEXT.md, strict_classes.txt and idxpost/)
and `2026-11-05_plan_p6_w2` (wave 2). One plan agent did the work; the coordinator applied and regraded it.

| Wave | Directory | Changed and passing | Deferred |
|---|---|---|---|
| w1, 0x0041E5F2 `?GetInstance@CManager@NUISystem@@SIPAV12@XZ` | `plan_p6_w1` | 27 | 0 |
| w2, CThing onto engine/CThing.h | `plan_p6_w2` | 1,759 | 2,431 |

- **Header:** engine/CThing.h gains the GetTC / PeekTC template bodies behind three new sub-macros
  (FABLE_RETAIL_CTHING_GETTC_EXPANDED, _STATEACCESS, _SCANINCLUSION_OUTOFLINE). hdrcheck2 reads `default=same` and every existing
  macro `same`.
- **Applied** with `apply_w35c.py` (w1 then w2). 0x0059F304 was rewritten by both waves and w2's copy predates w1's respell, so it
  was taken out of w2 (DEFERRED, reason `coordinator`) and keeps the w1 text. BINDINGS: 40 retired, 31 added;
  `bindings_multi_check.py` 0. Queue lines 62 / 568 / 579 / 701 closed, 649 closed as wrong. Label 0x0041D198 =
  NUISystem::CManager::SetMeshBank (`ghidra_out/labels_plan_p6_2026-11-05.tsv`).
- **Coordinator regrade from the working tree** (`logs/regrade_*`): the 1,786 changed rows plus the 220 other landed sources whose
  includes reach engine/CThing.h, 2,006 in all. gate.py 2,006 / 2,006 PASS; vland `--recheck-landed` 2,006 / 2,006 PASS
  (46 MATCH, the rest RELOCATION_MATCH), every behaviour test PASS.
- **Not done:** waves 3 (CIntelligentPointer leftovers) and 4 (CTCBase, CThingCreatureBase). `NEXT.md` is the phase 7 brief: the
  CThing deferral classes (header_lacks 754, local_std 567, CDefPointer closure 460, ...), and `w2/CALLERS.tsv` lists 62
  old-spelling binders of PDB-spelled targets still to respell.
- **Tools** (`scripts/`, rebuilt this phase because the phase 1-5 tools were lost with their scratch directory): hdrmig6.py (the
  rewriter), gatewave.py, vlandwave.py, conscheck.py, handback.py, idxpost.py, objdiff.py, wavelib.py and the surveys. They read
  `S` from their own text: set it to the current lane scratch before use.
- **idxpost/** (from the b7d036105 index): MULTINAME.tsv (14,994 Fable conflicts, 80 Fable ICF, 1,346 vendor conflicts) and
  QUEUE_STATUS.tsv (satisfied 33, partial 2,866, open 652, unparseable 1,597). queue_mark.py has not been run on it yet.
