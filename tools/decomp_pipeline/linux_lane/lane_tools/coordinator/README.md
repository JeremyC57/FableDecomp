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

## Cut a chunk from the never-attempted pool (2026-11-01)
`python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/analyze_never.py` re-analyses every known row with no landed
source and no parked attempt (extent scan from `lane_tools/bounds/extent.py`, references, retail-asm signatures, vendor regions,
masked-body twins) and writes `rebuild/candidates/manual/pending/never_attempted/{ANALYSIS,CHUNKS}.tsv` + SUMMARY.md (no bytes).
CHUNKS.tsv is the proposed cut order NA001.. (twins of landed rows first, then 97-256 / 257-512 / 513-1024 / 17-96 / 1025-4096 /
>4096, retry_skipped last; class siblings and pool twins together). To cut NAxxx as chunk N:
`awk -F'\t' -v c=NAxxx 'NR==1||$1==c{print $2"\t"$3"\t"$4"\t"$5}' .../CHUNKS.tsv | sed '1s/.*/address\tmodule\tname\tlength/' > $S/w22/c<N>rows.tsv`,
then steps 2-5 of "Cut a chunk" above (oracle_from_exe.py, chunk<N>.tsv with bytes, mkprompt25.py, CHUNK_BRIEF.md from the
chunk 651 brief incl. its "Increased attempts" and retail-fidelity sections, chunk_history.tsv). Rows whose reason says
"length uncertain" need the extent checked by hand before the oracle (a cold arm after a noreturn call). twin_landed rows: start
from the landed twin's source (the masked bodies are equal; only names / relocations differ). Re-run the analyser after landings.

## Wrong catalog names are fixed at landing (owner, 2026-11-01)
After the labels file is written for a chunk or naming wave, run
`python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/apply_labels.py ghidra_out/labels_<x>.tsv` then
`python3 tools/sync_catalog_names.py`: every proven callee / global / neighbour name goes into `auto-re-candidates.tsv` and
`functions.tsv` immediately (vland only renames the chunk's own rows). A landed row whose own Leaf spelling is wrong is re-synced
from that Leaf, so it stays wrong until its NAME_FIXES_QUEUE row is respelled in the next naming wave -- queue rows are
therefore landed in a naming wave right after every chunk round, not held.

## Agent budget (owner 2026-11-02)
After naming wave 32 lands: three concurrent chunk agents (agentNNN workspaces under $S/w22), no standing naming wave. Each chunk's
NAME_FIXES rows still go to NAME_FIXES_QUEUE.tsv (cNNN prefix) for the naming pass after matching. Land hand-backs one at a time
(land_chunk.sh is serial: it edits build_candidates.ps1 and the catalog). A landing that adds a second spelling for an address respells
the old callers in the same commit (recheck them) or is queued with a PRIORITY note; check reloc_bindings by address, not only with
bindings_multi_check.py.

## Header corrections inside chunks (owner 2026-11-02: "make sure headers are correct while working any chunks if possible")
Every chunk brief carries `CHUNK_BRIEF_headers_section.md` (agentNNN / chunk NNN substituted): an agent fixes a wrong engine header in
`$S/w22/agentNNN/hdr/` and grades with `FABLE_HDR_OVERLAY=<that dir>` (verify_and_land.py searches it before rebuild/include), preferring an
opt-in `FABLE_RETAIL_<CLASS>` branch. At landing: diff each hdr/ file against the repo header it was copied from (merge if the repo moved),
copy it into rebuild/include, list every landed includer (real `#include` lines, transitively) and recheck them all with the chunk's rows
before the commit; a failing includer means the change goes back to opt-in or out.

## Codex pass 11 reservation (owner 2026-11-02)
Pool chunks NA067-NA076 (100 rows, `pending/2026-11-02_codex_pass11/RESERVED.tsv`) are reserved for an outside Codex pass until
2026-11-09: chunk cuts go NA055-NA066, then NA077 onward. Land a Codex return like ChatGPT pass 10 (own oracle, read-only vland regrade, symbol audit).

## Codex passes 12-14 reservation (owner 2026-11-02)
Three more 100-row sections, handed to Codex one at a time as it finishes the previous one: pass 12 = NA077-NA086, pass 13 = NA087-NA096,
pass 14 = NA097-NA106 (released to in-house cuts 2026-11-03, `RELEASED.tsv`) (`pending/2026-11-02_codex_pass{12,13,14}/RESERVED.tsv`), reserved until 2026-11-30. In-house cuts go NA061-NA066,
then NA107 onward. The owner paused in-house chunk launches on 2026-11-02 after chunk 717.
