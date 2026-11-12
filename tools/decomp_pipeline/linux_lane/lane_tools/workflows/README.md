# Workflow: author rows (owner opted in with "ultracode", 2026-11-12)

`author_rows.js` runs the byte-matching author lane as a deterministic workflow. For each row (args.rows: addr, module, name,
length, residue_class, pending; args.scratch = $S; the copy first run on 2026-11-12 had $S hard-coded):
1. **Scout** (Opus): proves the row's name, signature, callees, members, which callee bodies retail's compiler could see, and
   sibling landed rows -> `$S/w22/<wf>/<addr>/DOSSIER.md`. No compiles.
2. **Author** (Fable, rapid fire): ports the draft onto the shared headers and grades until the row matches. If an agent's budget
   runs out, a fresh agent continues with its notes (up to 3 per row); a row is never dropped silently.
3. **Verify** (Opus, two independent lenses): mechanical (strict regrade, VC7.1 test, purity, local models, stand-in bodies, overlay,
   formats, reserved rows) and evidence (name from two sources, callee spellings, a test that can fail, faithfulness).
4. **Repair** (Opus) once, then re-verify.
The coordinator still lands (three-way header merge, vland --land, records): the workflow only produces verified hand-backs.

Model rule (owner, 2026-11-12): workflow agents are Opus 5.5 unless Fable 5.2 is needed; authoring is the stage that needs Fable.
Before a run: put each row's draft in `$S/w22/<wf>/<addr>/differ/<addr>.cpp`, cut the oracle with chunk_pool/oracle_from_exe.py,
copy `WF_BRIEF.md` to `$S/w22/WF_BRIEF.md` and lane_tools/fast/*.py to `$S/tools/`. Concurrency is capped at CPUs - 2.

## Landing a finished author workflow (coordinator tools, added 2026-11-12)
- `wfresult.py <task-output> <wf-dir>`: parse the workflow's result list, save RESULT.json, print status / open issues per row.
- `land_prep.sh <wf> <base-commit> addr...`: reserved check, `combine_wf.py` (one payload + oracle), purity / local-model / stand-in
  scans, per-row header overlays merged into `$S/land/<wf>/M` (superset copy, else three-way merge), hdrcheck2, users of changed macros.
- then: copy `M/` over `rebuild/include`, `fixmove.py`, `keypayload.py`, `vland.py --land` (strict headers), bindings / funclet checks.
- `land_records.py <wf> <letter> <landed,...> [<not-landed,...>]`: batch + pending record skeleton (notes, verdicts, labels).
Rows with open issues: fix small findings at landing, or send the row back (one Fable repair agent, then two verifiers) before landing.
A row that matches only with another function's unproven body visible is held back until that function lands.

Other workflows here: `verify_handback_evidence.js` (one Opus evidence verifier per matched row of an agent hand-back; args
scratch, dir, rows) and `name_fix_queue_pass.js` (short naming pass over the name-fix queue: apply, adversarial verify, one repair;
args scratch, agent = the prepared `$S/w22/<agent>/` directory with BRIEF.md, QUEUE.tsv, BUSY.txt).

Run `bash lane_tools/fast/cl_watchdog.sh 300` in the background beside any author lane: it kills Wine compiler processes older than
five minutes (orphans of timed-out graders that otherwise hold a core each for hours).
