# Naming pass, wave 9 (one agent; a chunk-authoring agent, chunk 575, shares the lane)

Landed: operator=, waves 1-8, LowerBound. Records are in `rebuild/candidates/manual/naming/`, and wave 8 is in
`2026-10-29_wave8/`. Owner decisions still in force:
- Wide text is `unsigned short` (`G`), never `/Zc:wchar_t`; a proposed `_W` spelling lands in the `G` form.
- Names are right the first time: exact PDB spelling, two sources per name, one retail address = one mangled name.

Workspace: `$S/naming/w9/` (S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad).
The prepared input is `$S/naming/w9/QUEUE_566_574.tsv`.

## Scope, in this order (stop at about 500 edited rows)
1. **`QUEUE_566_574.tsv`:** 145 NAME_FIXES rows from chunks 566-574 (columns chunk, address, correct_symbol, evidence,
   wrong_spellings_seen).
   - Merge them into DECISIONS as earlier waves did, and check every spelling against the PDB name lists yourself.
   - Note that `NAME_FIXES_QUEUE.tsv` row 522 (chunk 522) for 0x0099EFB0 is WRONG. The proven spelling is the const&
     return `??4CCharString@@QAEABV0@ABV0@@Z` (chunks 529/531/538/571/574 and both PDBs). Settle 0x0099EFB0 across the
     whole catalog: about 900 landed files declare CCharString::operator=. Put it in its own sub-wave with its own
     recheck, and if it is too big for this wave, scope it and DEFER it with an exact plan.
2. **Wave 8's open decisions:** `2026-10-29_wave8/DECISIONS_W8.tsv` rows whose `w8` column starts `open:` (90 proven
   not reached, 117 identity checks, 60 probable).
   - Do the proven ones first.
   - Identity checks need two sources before editing.
   - Probable rows stay open unless you find the second source.
3. **The five wave 8 `RENAMES.tsv` respell proposals** (00438570, 00405f00, 0051ed80, 0049d810, 008fcf80). Verify each;
   mark the ones to apply APPLIED, with the git-mv target path. The coordinator does the moves.
4. **Cheap DEFERRED.tsv items** from waves 5-8, if budget remains.

## Rules and hand-back
- Follow `briefs/BRIEF_W7.md` and `BRIEF_W8.md`:
  - idempotent scripts plus verified file lists;
  - a composed overlay (`ov_final/rebuild/...`) re-made from the CURRENT repo before the final recheck;
  - BINDINGS.tsv (action/symbol/target/by_row/note);
  - LABELS.tsv, proven names only;
  - DEFERRED.tsv, RENAMES.tsv and DECISIONS_W9.tsv;
  - recheck logs.
- **Recheck:** every changed landed row must stay MATCH/RELOCATION_MATCH + PASS under
  `tools/decomp_pipeline/linux_lane/lane_tools/vland.py --recheck-landed`, with `LANDVERIFY_WORK=$S/naming/w9/lv`. The
  payload builder is `tools/decomp_pipeline/linux_lane/lane_tools/coordinator/mk_recheck_payload.py` (it compares
  against HEAD, so run it on your overlay copy). `bindings_multi_check.py` on your final bindings must print 0.
- **Where you may write:** work only in `$S/naming/w9/`. Never run git. Never write under /home/user/FableDecomp.
- **Processes:** never kill a process; ask the coordinator.
- **Searching:** no recursive find/glob over `/` or the whole scratchpad.
- **Commands:** `timeout` on every compile; never repeat a failing command more than twice with the same arguments.
- **Wrap-up:** leave no background job running when you report.
- **Report** (your final message): rows edited, bindings changed, labels, deferred items with reasons, the 0x0099EFB0
  outcome, recheck totals.
