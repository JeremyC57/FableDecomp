# Prompt: run the byte-matching author lane as workflows (coordinator role)

Paste everything below the line into a fresh Claude Code session on this repository (ultracode / Workflow tool enabled).

---

You are the COORDINATOR of the FableDecomp byte-matching lane. Read `CLAUDE.md`, `docs/HANDOFF.md` and
`tools/decomp_pipeline/linux_lane/lane_tools/workflows/README.md` first. Agents author in scratch; only you verify, land, record,
commit and push (to your designated branch; never open a PR unless asked). `L` below means
`tools/decomp_pipeline/linux_lane/lane_tools`.

## Goal
Take parked rows (functions with a draft that does not byte-match yet) and turn each into a landed row: plain portable C++ that
compiles under VC7.1 to the retail bytes (MATCH / RELOCATION_MATCH), with a behaviour test, on the shared headers, with exact PDB
names. One row is worked until it matches; rows are never dropped silently.

## Roles and models (owner rule)
- Workflow agents are Opus unless Fable is needed. Authoring and row repair need Fable (`model: 'fable'`: both edit code that must
  keep or reach a byte match); scouts, verifiers and naming agents are Opus (a different model also gives a more independent read).
- Agents never run git, never write under the repo, never land. They work in `$S/w22/<wf>/<addr>/` (S = your scratchpad).
- Every Bash call (yours and theirs) exits 0 (`; true`); never pipe a grader into `head` (redirect to a file).

## One-time setup
1. Stand up the Linux lane (`docs/BUILDING.md`, `tools/decomp_pipeline/linux_lane/setup_vc71_wine.sh`); you need the retail
   `Fable.exe` at `$S/dl/game/Fable The Lost Chapters/Fable.exe` (ask the owner for it; never commit it). Lane prefix for every call:
   `export S=<scratchpad>; source $S/wine/env.sh >/dev/null 2>&1; export S; cd <repo>; unset FABLE_HDR_OVERLAY`.
2. The workflow prompts expect these scratch paths; copy them from the repo:
   `$L/fast/{fg.py,vt.py,addmatch.py,cl_watchdog.sh}` and `$L/q.py` -> `$S/tools/`; `$L/xdis.py` -> `$S/w17/`; `$L/xref.py` ->
   `$S/w23/`; `$L/workflows/WF_BRIEF.md` -> `$S/w22/WF_BRIEF.md`; `$L/workflows/{prep_wf.py,combine_wf.py,land_prep.sh,
   land_records.py,wfresult.py,land_naming_prep.sh}` and `$L/coordinator/mk_recheck_extra.py` -> `$S/tools/` (mk_recheck_extra.py is
   called as `$S/mk_recheck_extra.py` in the notes: put a copy there too). `mkdir -p $S/batch $S/land`.
3. Start the watchdog in the background and keep it running: `bash $S/tools/cl_watchdog.sh 300` (orphan Wine `cl.exe` processes
   from timed-out graders otherwise hold a core each for hours).
4. Build the author pool `$S/author_pool_rest.json`: a JSON list of `[length, addr, residue_class, n_local_models, delta,
   draft_path]` from `rebuild/candidates/manual/PARKED_INDEX.tsv`, keeping rows that are unlanded (not in
   `rebuild/oracles/auto-re-candidates.tsv`), unreserved, in the engine range, whose draft exists; smallest first.
5. Reserved rows are off-limits: every `rebuild/candidates/manual/pending/*/RESERVED.tsv` here AND on the other account's branch
   (`git fetch origin <their-branch>`; `$L/coordinator/reserved_check.py` reads both). Check before every cut and every landing.

## The author workflow (`$L/workflows/author_rows.js`)
Per row, as a pipeline (no barrier between rows):
1. **Scout (Opus):** proves name, signature, callees, member offsets, which callee BODIES retail's compiler could see, sibling
   landed rows -> `DOSSIER.md`. No compiles.
2. **Author (Fable, rapid fire):** starts from `differ/<addr>.cpp`, ports the draft onto the shared headers (opt-ins in an overlay
   copy under `hdr/`), and grades with `fg.py` on every tool call (up to three variants per call) until the row matches; writes the
   test (`vt.py` must PASS) and registers with `addmatch.py`. If its budget runs out a fresh author continues from its notes (up
   to three). No grade limit; no switching rows.
3. **Verify (two independent Opus lenses):** mechanical (strict regrade, VC7.1 test, purity, local models, stand-in bodies,
   overlay = repo header + additions, reserved rows) and evidence (name from two sources, callee spellings against relocation
   targets, a test that can fail, faithful claims).
4. **Repair (Fable) once**, then re-verify. Result per row: VERIFIED, MATCH_WITH_OPEN_ISSUES, DIFFER or PIPELINE_ERROR.

Launch one: `python3 $S/tools/prep_wf.py <wfN> 10` (cuts the next ten pool rows into `$S/w22/<wfN>/<addr>/`), then
`python3 $L/chunk_pool/oracle_from_exe.py $S/w22/<wfN>/rows.tsv <wfN>` and write each row's line to `<addr>/oracle.tsv` (scratch
only: never commit oracle bytes of unlanded rows), the reserved check, then
`Workflow({scriptPath: "$L/workflows/author_rows.js", args: <contents of $S/w22/<wfN>/args.json>})`.
On 4 cores: two or three author workflows of ten rows at a time plus at most one naming pass; each workflow runs two agents at
once. Watch `uptime`; if the load is above the agent count, look for stuck `cl.exe`. A ten-row workflow takes one to three hours
and about 50 agents. Start the next workflow as soon as one finishes.

## Landing a finished workflow
1. `python3 $S/tools/wfresult.py <task-output-file> $S/w22/<wfN>` -> status and open issues per row.
2. `$S/tools/land_prep.sh <wfN> <HEAD-at-launch> <addr>...` -> reserved check, one combined payload + oracle, purity / local-model /
   stand-in scans, per-row header overlays merged into `$S/land/<wfN>/M` (superset copy, else three-way merge), `hdrcheck2.py`,
   users of every changed macro. Requirements: no conflict markers; every existing macro and the default path `same` (a `=DIFF`
   macro must be new: zero users). If a default path or an existing opt-in really changes, recheck every landed includer in the
   same landing (`mk_recheck_extra.py` with `EXTRA=<address list>` + `vland.py --recheck-landed`). Two rows adding an opt-in for the
   same inline: compare the bodies and keep ONE macro.
3. Decide per row:
   - VERIFIED and scans clean -> land.
   - Small open issue (a comment claim, an unexplained specifier, a missing "descriptive name" note) -> fix it in the payload, regrade.
   - Real issue (signature against the PDB, test edits the body under test) -> send the row back: one Fable repair agent with the
     exact findings, then two fresh Opus verifiers (evidence lens + an adversarial lens on any new claim), then land.
   - Matches only with another function's UNPROVEN body visible -> hold it back (never a stand-in body), put that callee into the
     next workflow, and when it lands regrade the held row with the landed row's real body copied verbatim.
   - DIFFER -> back to the pool with its notes.
   - Inferred statements (dead code the bytes prove but whose wording is unknown) are fine when behaviour is identical: mark them
     `Inferred:` in the source header with the retail bytes and the ruled-out alternatives.
4. Land: copy `M/` over `rebuild/include`; `python3 $L/chunk_pool/fixmove.py payload.json; python3 $L/naming/keypayload.py
   payload.json; LANDVERIFY_WORK=<dir> LANDVERIFY_STRICT_HEADERS=1 python3 $L/vland.py payload.json oracle.tsv --land > land.log`.
   Every row must print RELOCATION_MATCH / MATCH and PASS. (`NOT_GENUINE` = the purity scanner; it also trips on the bare word
   `naked` in a comment or test local.)
5. Checks: `python3 $L/coordinator/bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` must stay at its baseline;
   `python3 $L/coordinator/funclet_includes_check.py` must report 0 broken.
6. Records: `python3 $S/tools/land_records.py <wfN> <letter> <landed,...> [<not-landed,...>]` writes
   `rebuild/candidates/manual/batches/<date>_landing_<letter>/` and `pending/<date>_linux_hand<WFN>/`; then write both READMEs by
   hand (what landed, verifier findings and what you did, inferred statements, headers, default-path changes and rechecks, checks),
   append the rows' NAME_FIXES to `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` under a new tag, add the lessons to
   `docs/pipeline/GOTCHAS.md` (one line each), one line to `docs/HANDOFF.md`, fix wrong manifest labels of the landed rows.
7. `python3 tools/write_decomp_dashboard.py; python3 tools/update_readme_progress.py` (never hand-edit coverage), commit
   (`land: ...`), push.

## Naming (paused except for the name-fix queue)
When the queue holds about 30 waiting entries (or at once for one symbol bound to two addresses), run
`$L/workflows/name_fix_queue_pass.js` with `args {scratch, agent}` on a prepared `$S/w22/<agent>/` (BRIEF.md, QUEUE.tsv, BUSY.txt =
reserved rows + rows the author workflows hold; copy a previous `naming/<date>_agentNF*/BRIEF.md` and adapt it). It applies the
queue (Opus), an independent verifier regrades every changed row and attacks the hand-back, and one repair pass follows. If the
second verdict still has a blocker, run one more targeted repair agent before landing. Land with `land_naming_prep.sh <agent>
<base>` (overlap, busy rows, new local models, header overlay), copy the overlay headers, `naming/batch_rerow.py` on section 1 of
RENAMES.tsv (addresses with `0x`, name = method only), copy the other rows over their landed sources, funclet check,
`mk_recheck_extra.py` + `vland.py --recheck-landed` for ALL changed rows, `naming/rebind.py`, `bindings_multi_check.py`. Carry
partial entries forward under a new queue tag. Library code taken directly from a library gets no naming work.

## Rules that are never traded for speed
- Plain C++ only in rows and tests: no `__asm`, naked, `_emit`, `register`, bare `__forceinline`; `volatile` only where retail used it.
- Shared headers over local models; header changes are `#ifdef FABLE_RETAIL_*` additions with the default path unchanged; never a
  NEW local model of a class that has `rebuild/include/engine/<Class>.h` (rows and tests).
- Never a stand-in body for another retail function; a callee's REAL (landed, byte-proven) body may sit in a row file.
- Exact PDB names from two sources; one address, one name; made-up names only when found nowhere, descriptive, declared in NOTES.
  A PDB address of a claimed name proves nothing about the retail address being renamed: check the body and the binding table.
- Never commit retail bytes, scratch or `work/`. Agent "test PASS" counts only after the lander's VC7.1 run.
- Do not trust a hand-back, and do not trust a repair: every row is regraded by `vland.py` at landing, and anything an agent
  changed after verification gets verified again.

## What made rows match (read `docs/pipeline/GOTCHAS.md`, the wf1..wf5 lines)
Most "register tie" residues were model errors: port onto the shared headers first, then judge the residue. Next most common:
which callee bodies the retail TU could see (real bodies visible, out of line), the loop form, PDB inline accessors, named locals
that cost no code, and parentheses that move VC7.1's scheduling cut.
