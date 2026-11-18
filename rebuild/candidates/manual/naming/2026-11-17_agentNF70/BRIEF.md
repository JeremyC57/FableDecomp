# Name-fix agent NF70 (2026-11-17, round 7): the follow-up entries queued at nf62-nf66, plus NF64's 14 held rows (95 entries, 174 rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". Round 7 has fixed about 1,650 rows. NF62-NF66 landed (last at 84f2387b5a); this slice is the
follow-up entries they queued. NF68 (engine-side ports) and NF69 (names settled by Ego_d.pdb) run beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf62/README.md` .. `2026-11-17_landing_nf66/README.md` (five landings);
- `rebuild/candidates/manual/naming/2026-11-17_agentNF64/NOTES.md`, `2026-11-17_agentNF65/NOTES.md`, plus the LESSONS.md and
  hdr/README.md files of NF62-NF66;
- `docs/journal/2026-11/EGOD_FINDINGS.md` (new evidence: retail has no ICF and keeps unreferenced COMDATs, so two identical bodies
  at two addresses are always two different decorated names; Ego_d also has the TLC source's object files and named locals).

## Your slice
Files in `$S/w22/agentNF70/`:
- `ENTRIES.tsv`: `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` lines q10724..q10818 (tag `land1117fu`; `q<n>` is the
  0-based line): entry, tag, address, correct symbol, evidence, wrong / current spelling as queued. 95 entries.
- `WORKLIST.tsv`: landed rows that bind an entry address under another spelling (rebuild/oracles/reloc_bindings.tsv at
  3cd153e0c7): entry, target, binder_row, bound_symbol. 101 lines.
- `rows.txt`: the 174 rows you may change (the entries' landed self rows, their binders, and the 14 held rows). All are locked as
  NF70 already.
- `ready/`: **NF64's 14 finished rows held back** because landing them alone would split a caller's binding. NF64's STATUS.tsv /
  NOTES.md (`naming/2026-11-17_agentNF64/`) say which caller each set waits for; those callers are entries in this slice. Re-grade
  each against today's repo (headers changed since), finish its caller, and hand both back together:
  six tree `operator=` rows (0x0045C3CF, 0x0045C79E, 0x0045DD2C, 0x004E99AE, 0x004EC219, 0x004EC33E), 0x00868CA0 with 0x007E2600,
  0x00A90F00, 0x00694020 with 0x008D5000 (also check 0x008D5000's new `?FrameUpdate@CCreatureAction_FireMissileWeapon@@UAEXXZ`
  against its vtable and callers; NF64 did not), 0x00467831, 0x004E9021, and 0x0058148D (its test no longer links: the row's object
  now carries the STLport `erase` bodies, so the test's own stub of `erase(const key&)` is defined twice; fix the test).
  Entry q10804 gives the correct `MAEXXZ` for CCreatureActionBase::FrameUpdate (NF64 rejected the earlier `QAE` claim, q10720).

Two kinds of work, often in the same entry:
1. **Self row:** the row AT the entry address has the wrong own name. Re-author it under the correct spelling (usually by moving it
   onto its shared header) and put it in RENAMES section 1 (address, module, name, mangled_leaf, old_catalog_name, old_leaf,
   evidence). `module` + `name` is the catalog name: give the real class and member (no `Class::` prefix in `name`). For STL
   instances use the module spelling the catalog already uses for that container. For a leaf-only change on a row whose catalog
   name is already right, repeat the old catalog name.
2. **Binder rows:** landed rows that call the entry address under another spelling. Respell them (section 2 of RENAMES: caller
   row, old symbol, new symbol, callee address), taken from before/after compiles.

Two entries are duplicate-definition cleanups, not renames: 0x0081F910 `GetStrongestEngineThemeAt` is defined in the rows
0x0081FA20 / 0x0081FA90 as well as at its own address (rename the 0x0081F910 row to the PDB name: entries q10724 and q10735), and
`CBasicString<char>::operator<` 0x00429950 is defined again in 0x009EFB80. Each body must have one definition: if a row needs the
body visible for code generation, it comes from one place (an opt-in in the shared header), not a copy per row. If that cannot be
done without changing a header's default path, mark the entry `blocked/header` and write the exact change in NOTES.md.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. A row already right is `not-a-binder` (say
why). An entry whose self row is already right and has no wrong binders is `done-already`.

**Verify every claim from two sources** before applying it (PDB name sets, the callee body, vtable slots, the header). Queue claims
come from one agent's evidence; a claim that does not hold goes to REJECTED.tsv with the reason. Do NOT re-probe the whole slice.

**Order of work.** The 14 ready rows and their callers first (they are nearly done), then the entries in queue order. Take each row
to a terminal grade before starting the next. **Budget:** up to 10 grades per row; after 3 failed forms on one row mark it blocked
with the exact compiler error or diff. A row that models a dozen classes locally is blocked/port unless it moves within 10 grades;
name the classes that must move first.

**Containers restart without warning.** Write STATUS.tsv and APPLIED.tsv after EVERY row. The first grade after a restart can time
out while wineserver starts; retry once.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py`). Group rows that share a stand-in or a callee respelling, prove the edit on 2-3, script it,
grade the rest in one vland payload of 20-30 rows, then fix the stragglers by hand.

## Also carry the lessons
These sections of `$S/w22/agentNF44/BRIEF.md` apply unchanged; read them: "Lessons of earlier passes", "Disk", "Round-3 /
Round-2 / Round-1 lessons", "Per entry", "Hard rules", "Lane", "Hand-back". Then the newest LESSONS.md files:
`$S/w22/agentNF67/LESSONS.md`, `$S/w22/agentNF64/LESSONS.md`, `$S/w22/agentNF41/LESSONS.md`. The tool scripts in
`$S/w22/agentNF64/` and `$S/w22/agentNF67/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF68 and NF69 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF70/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF70 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF70/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
