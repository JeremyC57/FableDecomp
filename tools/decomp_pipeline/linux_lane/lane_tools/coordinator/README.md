# Coordinator kit (Linux lane, 2026-10-29)

The scripts the coordinator session used to cut chunks, land agent hand-backs and apply naming waves. All run from the
repo root with `S` exported to the lane scratch dir (the one holding `wine/env.sh`, `dl/game/.../Fable.exe`, `w22/`,
`w17/xdis.py`): `export S=<scratch>; source $S/wine/env.sh`.

## Cut a chunk (rows from a pool, e.g. rebuild/candidates/manual/pending/2026-10-29_chatgpt_pass9/UNTOUCHED_RESERVED.tsv)
1. Write `$S/w22/c<N>rows.tsv` (address, module, name, length). Check each extent first: padding (cc/90) before and
   after, no int3 / several rets inside, first instruction a prologue (chunk 567: 9 of 12 released rows were fragments).
2. `python3 ../chunk_pool/oracle_from_exe.py $S/w22/c<N>rows.tsv chunk<N>` -> `$S/batch/chunk<N>_oracle.tsv` (scratch only,
   retail bytes never committed); build `$S/w22/chunk<N>.tsv` (rows + bytes column) from it.
3. `python3 ../chunk_pool/mkprompt25.py <N>` -> `$S/w22/agent<N>/` (scripts, dis_all.txt, PROMPT.md); write
   `CHUNK_BRIEF.md` (custom header + the "Read first" tail of `CHUNK_BRIEF_example_569.md`, numbers swapped).
4. Append the rows to `rebuild/candidates/manual/chunk_history.tsv`, commit "docs: queue chunk N".
5. Agent prompt: read CHUNK_BRIEF.md then PROMPT.md; no git, no writes under the repo, no killing processes.

## Land a chunk hand-back
`bash land_chunk.sh <N>` (strip existing rows, fixmove, keypayload, asm grep, `vland.py --land`), then
`python3 records.py <N>` (batch records, pending differ/, proven labels; if it finds 0 proven, write the label file from
NOTES.md by hand), copy NAME_FIXES.tsv into the batch and append it (chunk column first) to
`rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv`, write the batch README, a FINDINGS_LOG line and GOTCHAS lines,
then `tools/write_decomp_dashboard.py` and `tools/update_readme_progress.py`, commit, push.

## Apply a naming wave (agent workspace W with ov_final/, BINDINGS.tsv, LABELS.tsv, RENAMES.tsv)
1. Recompose the overlay against the current tree (`$W/tools/compose<k>.py $W/ov_now`) and `diff -r` it with the agent's
   final overlay; then `cp -r $W/ov_final/rebuild/. rebuild/`; apply RENAMES marked APPLIED with `git mv` and fix
   Source/TestSource in `rebuild/build_candidates.ps1`.
2. `python3 apply_bindings.py $W/BINDINGS.tsv`; `python3 bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` must print 0.
3. Correct catalog names (`auto-re-candidates.tsv` column 2) from LABELS.tsv; copy LABELS.tsv to
   `ghidra_out/labels_naming_wave<k>_<date>.tsv`.
4. `python3 mk_recheck_payload.py <out>` (every changed landed row vs HEAD) and a full
   `vland.py <out>_payload.json <out>_oracle.tsv --recheck-landed`: every row MATCH/RELOCATION_MATCH + PASS before commit.
5. Records to `rebuild/candidates/manual/naming/<date>_wave<k>/` (README, DECISIONS, BINDINGS, LABELS, DEFERRED, RENAMES,
   scripts/, recheck.log). Naming briefs used so far: `rebuild/candidates/manual/naming/briefs/`.
