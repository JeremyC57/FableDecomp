# 2026-11-19 landing nf104: naming round 10, PRIORITY table slice 1 (42 rows)

Records: `naming/2026-11-19_agentNF104/` (selfcheck 0 problems). 40 entries from `naming/PRIORITY_RECONCILE_2026-11-19.tsv`: 24 done, 2 done-already
(q6239, q6543), 2 rejected (q4777: 0x00429160 is ReleaseData, 0x00429B72 is `jmp 0x00429160`; q4671: superseded by the CDefString.h
re-pointing), 2 blocked, 10 open (multi-row families; NF108 has them).

## Rows (42): 8 MATCH + 34 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
17 self renames (RENAMES_applied.tsv, 14 file moves) and 33 caller respells; in every done entry every exe-scan referencing row binds the claim.
Header: engine/CDrawMiniMap.h `FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_UPDATEMARKERS` (declaration only, nested in the `_CLASS` block; pin_asserts
pinned 1 assert at base line 93; assert_names_check 0 mismatches).

## Decisions and blocked entries
- **q4194 (0x004046B0) rejected as posed (advisor decision, owner delegated):** 63 landed rows bind the folded STLport destructor of
  list<CStringParserCommentBoundaries> as `~list` (47) or `~_List_base` (16). Both are valid names for the folded body; rows on the shared
  headers get `~_List_base` from the compiler, local models hand-declare `~list`. No direction is enforced; the local-model rows are queued as
  low-priority ports (GOTCHAS nf104 line).
- q4413: 0x008ED590 sits on rebuild/include/candidates/object_family_compiled.h (GFROR13 declared YIKK); NAME_FIXES carries `?GFROR13@@YIJK@Z`.

## Checks
bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Recheck of 1,267 rows (CDrawMiniMap.h includers + every landed row the exe
scan finds referencing one of the 32 respelled targets): all good (`logs/includer_recheck.txt`). No-new-split gate with the "before" set taken
from the 42 rows compiled at HEAD (new: `gate.py eval ... p_before.json`): 0 created (`logs/split_gate.txt`). The seven earlier gate verdicts
that depended on agent-reported old spellings (nf100 / nf101 / nf106) were re-checked against parent-commit sources: all were existing splits.
