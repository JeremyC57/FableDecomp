# Name-fix agent NF72 (2026-11-17, round 7): the follow-up entries queued at nf67-nf69 (51 entries, 83 rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". Round 7 has fixed about 1,750 rows. NF67, NF68 and NF69 landed (last at 144339cc2d); this slice is
the follow-up entries they queued. NF70 (follow-ups queued at nf62-nf66) and NF71 (blocked ports) run beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf67/README.md`, `_nf68/README.md` and `_nf69/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF67/`, `_agentNF68/` and `_agentNF69/`: NOTES.md, LESSONS.md, hdr/README.md;
- `docs/journal/2026-11/EGOD_FINDINGS.md` (retail has no ICF and keeps unreferenced COMDATs, so two identical bodies at two addresses
  are always two different decorated names; Ego_d has the TLC source's object files in `ghidra_out/egod_carved_functions.tsv`).

## Your slice
Files in `$S/w22/agentNF72/`:
- `ENTRIES.tsv`: `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` lines q10859..q10909 (tag `land1117fu`; `q<n>` is the
  0-based line): entry, tag, address, correct symbol, evidence, wrong / current spelling as queued. 51 entries.
- `WORKLIST.tsv`: landed rows that bind an entry address under another spelling (rebuild/oracles/reloc_bindings.tsv at
  144339cc2d): entry, target, binder_row, bound_symbol. 40 lines.
- `rows.txt`: the 83 rows you may change (the entries' landed self rows and their binders). All are locked as NF72 already.
  0x00E12580 is held by NF71 (HELD.txt): record it `deferred: held by NF71`.

Entries that are more than a rename:
- 0x008B9F20 CCreatureAction_TeleportIn::Initialise: port it onto the shared headers with the same text as its landed twin
  TeleportOut 0x008B9E50 (landing nf67).
- 0x009D58D0 `GetAsyncEntry`: engine/CBankFileAsync.h's existing `CREATEENTRY` opt-in declares a made-up name that the landed rows
  0x00AF1360 / 0x00B42530 bind. Add the PDB-named declaration as a new opt-in, move those rows onto it, and say in NOTES.md that
  the old opt-in can be removed once nothing uses it (the coordinator removes it).
- 0x00E13310: the landed self row and header say EndMission, a landed binder says WaspMenace; both are made up. Look for the
  original name (PDBs, strings, FSE, Ego_d tables) and use one name everywhere. If none is found, keep the better descriptive name,
  use it in every row, and mark it descriptive in NOTES.md.
- 0x0047EEE4: port the row onto engine/NGameText_CDataBank.h (CGroupEntry derives CResource since nf69; its local
  `CGroupEntry : CRefCounted`, CWideString, CCharString, CDataBank and CManager models must go). Up to 15 grades for this row.

Two kinds of work, often in the same entry:
1. **Self row:** the row AT the entry address has the wrong own name. Re-author it under the correct spelling (usually by moving it
   onto its shared header) and put it in RENAMES section 1 (address, module, name, mangled_leaf, old_catalog_name, old_leaf,
   evidence). `module` + `name` is the catalog name: give the real class and member (no `Class::` prefix in `name`). For STL
   instances use the module spelling the catalog already uses for that container. For a leaf-only change on a row whose catalog
   name is already right, repeat the old catalog name.
2. **Binder rows:** landed rows that call the entry address under another spelling. Respell them (section 2 of RENAMES: caller
   row, old symbol, new symbol, callee address), taken from before/after compiles.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. A row already right is `not-a-binder` (say
why). An entry whose self row is already right and has no wrong binders is `done-already`.

**Verify every claim from two sources** before applying it (PDB name sets, the callee body, vtable slots, the header). Queue claims
come from one agent's evidence; a claim that does not hold goes to REJECTED.tsv with the reason. Do NOT re-probe the whole slice.

**Order of work.** The entries in queue order. Take each row
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
`$S/w22/agentNF69/LESSONS.md`, `$S/w22/agentNF68/LESSONS.md`, `$S/w22/agentNF67/LESSONS.md`. The tool scripts in
`$S/w22/agentNF68/` and `$S/w22/agentNF69/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF70 and NF71 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF72/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF72 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF72/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
