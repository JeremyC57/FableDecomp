# Plan phase 5 (2026-11-05): 4,128 rows moved onto shared headers

This README covers the four `2026-11-05_plan_p5_*` directories, w1-w4. Each holds that wave's PASS, DEFERRED, NOTES, BINDINGS
and QUEUE_LINES.

| Wave | Directory | Changed and passing | Deferred |
|---|---|---|---|
| w1, CCountedPointer | `plan_p5_w1` | 2,436 (2,313 RELOCATION_MATCH + 123 MATCH); 73 callers respelled | 650 |
| w2, CScriptThing remainder | `plan_p5_w2` | 150 (138 + 12) | 218 |
| w3, CIntelligentPointer (packed retail branch) | `plan_p5_w3` | 1,488 (1,469 + 19); 14 respells | 2,196 |
| w4, value-class leftovers | `plan_p5_w4` | 54 | 1,609 |

- **The agent's own grading:** every changed row passed gate.py and vland `--recheck-landed`, with the same grade and parity as
  its landed source.
- **Headers:** no shared header changed.
- **Applied** with `apply_w35c.py`, in the order w1 to w4 (114 rows sit in more than one wave). The BINDINGS ADD/RETIRE lines are
  applied to `reloc_bindings.tsv`: 26 added, 26 retired.
- **The coordinator's regrade** (gate + vland `--recheck-landed` on every row) was running when this was committed; its result
  is recorded in a follow-up commit. The w1 gate had already passed all 2,436 rows.
- **Phase 6:** `NEXT_PHASE6.md` lists what is left of steps 3-6.
