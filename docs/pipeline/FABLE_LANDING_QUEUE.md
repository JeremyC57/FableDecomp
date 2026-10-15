# Fable 5.1 landing queue (wave 25 onward)

**Owner's rule (2026-10-15):** while the session runs on Opus 5.5, as many sub-agents as the lane allows (16 at once)
hand-author the class-labelled chunk pool. They grade with the lane compiler while authoring, but the **final compiler
check, the landing, and every diff that keeps failing wait for a Fable 5.1 session.** This file is that session's
complete to-do list. Nothing listed here is in the catalog yet. The generated table at the bottom is rewritten by
`python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/queue_status.py` after every staging or landing.

Where the work is:
- **Staged batches** live in `rebuild/candidates/manual/pending/<name>/` (committed, so they survive a container reset):
  `payload.json` (sources + tests of the rows the agent graded MATCH/RELOCATION_MATCH), `oracle.tsv` (those rows'
  retail bytes, the same per-function rows a landed batch commits), `src/`, `test/`, `differ/` (best source of each
  parked row), `NOTES.md` (becomes the batch README), `STATUS.tsv` (one line per chunk row), `rows.tsv` (address,
  class, name, length of all 40 rows; no bytes).
- The agents' full working dirs are `$S/w22/agentNN/` (scratch, lost on a container reset; nothing in them is needed
  that is not staged).
- Scripts: `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/` -- `stage_pending.sh` (Opus side),
  `land_pending.sh` (Fable side), `oracle_from_exe.py`, `queue_status.py`, `mkprompt25.py` + `agent_scripts/` +
  `brief_tools_rules.md` (how the fleet's prompts and helper scripts were built).

## Step 0 -- bring the lane up (Fable)
1. Same container: `export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad;
   source $S/wine/env.sh; export S=...; cd /home/user/FableDecomp`, then prime Wine once:
   `timeout 120 wine cmd /c exit >/dev/null 2>&1 </dev/null` (a cold prefix makes the first grade hang for minutes).
   Prove it: `python3 $S/tools/q.py 00415b80 rebuild/src/compiled/00/41/SNavigationLayerDefaults00415b80_SNavigationLayerDefaults00415b80_00415b80.cpp` -> MATCH.
2. Fresh container: follow `HANDOFF_FOR_NEXT_SESSION.md` section 4 (game, DDK compiler, Wine prefix, scratch tools),
   then for each pending batch whose parked rows you will retry:
   `python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/oracle_from_exe.py rebuild/candidates/manual/pending/<name>/rows.tsv <name>`
   (writes `$S/batch/<name>_oracle.tsv` so `q.py` can grade those addresses; never commit that file).
3. `git pull origin FullDecomp` first: the Opus session keeps pushing staged batches.

## Step 1 -- final compiler check + landing of every STAGED batch (Fable)
For each table row whose state is `STAGED`, oldest first, one at a time (the lander is sequential):
```
bash tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/land_pending.sh <name>
```
`verify_and_land.py --land` inside it is the final check: it recompiles every source with the retail flags (plus the
pragma/flag sweep), compares bytes, compiles and runs every behaviour test with cl.exe under Wine, rejects
`__asm`/`naked`/`_emit`, and only then copies winners into `rebuild/src/compiled/`, `rebuild/tests/`, the catalog
and the oracle. Then:
- Compare `WINS: N` with the table's "matched" count. A shortfall is printed as `DIFFER`/`FAIL` lines: copy each
  refused source from `rebuild/candidates/manual/batches/<name>/` into `pending/<name>/differ/<addr>.cpp`, add a
  hand-over line for it under that batch's "Parked" section in `pending/<name>/NOTES.md`, and fix the batch README
  (`batches/<name>/README.md`) count line.
- The script trims `pending/<name>/` to `differ/`, `rows.tsv`, `NOTES.md`, `STATUS.tsv` (the open work).
- Spot-check 2-3 landed sources per batch for header comments, real member names and the shared engine headers
  (CLAUDE.md "Shared headers"); fix in place if needed (re-run the lander on that batch dir to re-grade).
After all staged batches: `python3 tools/decomp_pipeline/families/funclets.py . <date>_linux_funclets38` (land that dir
the same way if it found anything), then `python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/queue_status.py`.

## Step 2 -- the hard diffs (Fable)
Every parked row below carries the authoring agent's hand-over line: `DIFFER(ours v retail): what differs | tried:
forms tried | next: idea or "lane gap"`. For each one:
1. Grade the parked source: `timeout 200 python3 $S/tools/q.py <addr> rebuild/candidates/manual/pending/<name>/differ/<addr>.cpp [leaf] [--flags ...]`
   and `python3 $S/w18/udiff.py <(...) | grep -v "^ "` for the differing lines. (Needs the oracle rows of step 0.2
   on a fresh container; on the same container `$S/batch/chunkNN_oracle.tsv` already has them.)
2. At most 4 attempts per row (the loop-prevention rule). Rows marked "lane gap" are known DDK-4035-vs-retail
   codegen gaps (GOTCHAS 24k-24p lane-gap lists): give them one attempt at most, then leave them for the Windows
   RTM/QFE re-grade (`docs/pipeline/QFE4035_COMPILER_GATE.md`).
3. A row that matches: write its behaviour test, put source + test into a new dir `$S/w22/fretryNN/{src,test}` with the
   helper scripts (`cp tools/.../agent_scripts/*` and replace `__S__`/`agentNN`/`chunkNN`), run `tall.py`, `finish.py`
   (it reads `$S/w22/chunkNN.tsv`; point it at `pending/<name>/rows.tsv` + the step-0.2 oracle if the chunk TSV is
   gone), stage it as `<date>_linux_fable_retryNN` with `stage_pending.sh` and land it with `land_pending.sh`.
4. Delete the fixed row's `differ/` source and hand-over line; when a batch's `differ/` is empty remove its pending dir.

## Step 3 -- docs after each landed part (Fable)
GOTCHAS: one `- Wave 25x (...)` line built from the batches' NOTES "What matched" lessons; journal
`docs/journal/2026-10/LINUX_CONTAINER_LANE_2026-10-13.md` (new part section above the previous one); `docs/HANDOFF.md`
and `docs/ROADMAP.md` counts; `python3 tools/write_decomp_dashboard.py && python3 tools/update_readme_progress.py`;
commit `land: ...` as BuffJesus with the session's Co-Authored-By / Claude-Session lines; `git push -u origin FullDecomp`.

## Also open from the Fable sessions (optional retries)
Parked rows of the Fable-authored chunks 59 and 60 are listed with evidence in
`rebuild/candidates/manual/batches/2026-10-14_linux_hand59/README.md` and `..._hand60/README.md` (sources in
`$S/w22/agent59/parked/`, `$S/w22/agent60/parked/`; scratch only).

## Generated status
<!-- queue:begin -->
| pending batch | chunk | matched (to land) | parked | skipped | state |
|---|---|---|---|---|---|
| `2026-10-14_linux_hand58` | Hand chunk 58 (CMessageEventFilter_IsOriginatedByThing .. CMouseCursor): 34 of 40 byte-exact | 34 | 6 | 0 | STAGED: land it |

Totals: **34 matched rows staged for landing**, **6 parked rows** for the Fable retry pass.

### Parked rows per batch (hand-over lines written by the authoring agents)

#### `2026-10-14_linux_hand58` (6 parked; sources in `rebuild/candidates/manual/pending/2026-10-14_linux_hand58/differ/`)
- 00ac4ec0 GetChunkInfo: the lane reads the hidden result pointer before the sprintf and copies `this` (explicit / implicit CCharString conversion, an inline accessor, optimize("s"), all under /GS).
- 004afdd0 (labelled FindMostRecentMessage): the SGI `__find` value temporary -- retail homes it in the dead `key` slot (`mov eax,[esp+10h] ; mov [esp+10h],eax`, shared with the empty tag), the lane gives it a `push ecx` frame slot; the rest of the body matches.
- 0082a340 SetMarkerGraphic: only the hidden-result slot of `__find_if` (retail the dead `graphicIndex` slot shared with the tag, the lane the dead `thing` slot).
- 0082a390 GetNonAutoMarkers: prologue only -- retail pushes ebx/esi at the entry, the lane shrink-wraps them into the non-empty branch; the loop (malloc, placement test, link) is byte-identical.
- 00741d60 CModeExplosiveForcePushed `??_G`: the lane calls the implicit ~CAnimFilterOfGroup and ~CModeBase out of line where retail inlines them; verified in the lane's object that an implicit destructor stores only the BASE vtable (so a user `__forceinline` destructor adds a store retail lacks); `novtable` and the inline pragmas do not help.
- 006af2d0 IsOriginatorAlive::operator(): the inlined GetPOriginator<CThing> constant-false check (`mov [esp+8],eax ; xor al,al ; jne`) that the lane folds (`return false`, `thing != thing`, `(unsigned long)thing < 0` through a `CThing* const&` tried) -- the same gap as the parked 0x006AF4E0.

<!-- queue:end -->
