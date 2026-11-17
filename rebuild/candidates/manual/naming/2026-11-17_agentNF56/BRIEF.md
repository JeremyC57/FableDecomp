# Name-fix agent NF56 (2026-11-17, round 7): the follow-up queue (61 entries with exact symbols, 95 rows)

Owner: "Continue the naming fix queue with two agents until complete". Round 7 has fixed about 750 caller rows. The original queue
is nearly done: NF53 (engine-side tail, daed7abdfd) and NF54 (game-side first half, d6a18ced4d) are landed, and NF55 (game-side
second half) is running beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf53/README.md` and `2026-11-17_landing_nf54/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF53/NOTES.md` and `2026-11-17_agentNF54/NOTES.md`, plus their LESSONS.md.

## Your slice: the follow-up entries the coordinator queued during round 7
`rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` lines tagged `land1116fu` / `land1117fu` are name fixes that earlier agents
proved but could not apply (outside their worklist, or a self row they could not re-author). You take the 61 whose correct
symbol is an exact decoration. The other 33 (descriptive claims, the 78-row STLport stand-in family, settle-between-two cases) are
in `UNCLEAN.tsv` for a later slice: do not work them.

Files in `$S/w22/agentNF56/`:
- `ENTRIES.tsv`: entry id (`q<line>`, the 0-based line of NAME_FIXES_QUEUE.tsv), address, correct symbol, wrong_binders (the count
  of landed rows that bind that address under another spelling, from rebuild/oracles/reloc_bindings.tsv), evidence, and self_leaf
  (`OK` when the self row's `// Leaf:` already equals the correct symbol, `NOT LANDED`, else what the leaf line says; that column
  is parsed roughly, so read the row).
- `WORKLIST.tsv`: one line per wrong binding (entry, target, correct_symbol, binder_row, bound_symbol). 48 lines, 43 rows.
- `rows.txt`: the 95 rows you may change (the binder rows plus the landed self rows of the entries).

Two kinds of work, often in the same entry:
1. **Self row:** the row AT the entry address has the wrong own name. Re-author it under the correct spelling (usually by moving it
   onto its shared header) and put it in RENAMES section 1 (address, module, name, mangled_leaf, old_catalog_name, old_leaf,
   evidence). `module` + `name` is the catalog name: give the real class and member (no `Class::` prefix in `name`). For a
   leaf-only change on a row whose catalog name is already right, repeat the old catalog name.
2. **Binder rows:** landed rows that call the entry address under another spelling. Respell them (section 2 of RENAMES:
   caller row, old symbol, new symbol, callee address), taken from before/after compiles.
When you rename a self row, every landed caller of it must follow in the same pass if it is in rows.txt; callers outside rows.txt
go to NAME_FIXES.tsv.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. A row already right is `not-a-binder` (say
why). An entry whose self row is already right and has no wrong binders is `done-already`.

**Verify every claim from two sources** before applying it (PDB name sets, the callee body, vtable slots, the header). Queue claims
come from one agent's evidence; a claim that does not hold goes to REJECTED.tsv with the reason. Entries marked `CLAIM` or
`verify` in their evidence need extra care.

**Do NOT re-probe the whole slice.** The worklist comes from reloc_bindings.tsv as of daed7abdfd.

**Budget:**
- After 3 failed forms on one row, mark it blocked with the exact compiler error or diff.
- A row that models a dozen classes locally is blocked/port unless it moves within 10 grades. Name the classes that must move
  first.

**Containers restart without warning (six times so far).** Write STATUS.tsv and APPLIED.tsv after EVERY row. The first grade after
a restart can time out while wineserver starts; retry once.


## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group rows that share a stand-in (for example the CResource list rows) and port them together.
2. Port 2-3 rows of the group by hand, using these steps:
   - include the header, adding an `#ifdef` opt-in in your overlay for any member it lacks;
   - delete the local model;
   - call the real member or template.
   Grade each row until it is RELOCATION_MATCH.
3. Turn the edit into a script, apply it to the rest of the group, and grade them all in one vland payload of 20-30 rows.
4. Fix the stragglers by hand.

## Terminal statuses (per ROW in STATUS.tsv, per entry in APPLIED.tsv)
- **Rows:** `ported` (graded, test PASS), `not-a-binder` (with reason), or `blocked`.
- **blocked** needs an exact reason, the grade count, and the model or header that must be ported first.
  - Example: 0x00B68610's local CRenderStateManager collides with engine/CRenderStateManager.h.
  - A row blocked with 0 grades is not accepted unless it is `authoring` or `library`.
- **Entries:**
  - `done` when every binder row of that entry is ported or not-a-binder;
  - otherwise `blocked` with the rows left and why.
- **Terminal rule:** if a row needs a whole other class ported first (more than 10 grades), mark it blocked/port and name that
  class. It goes to a port list, so do not sink the budget into it.

## Also carry the lessons
These sections of `$S/w22/agentNF44/BRIEF.md` apply unchanged; read them:
- "Lessons of earlier passes";
- "Disk";
- "Round-3 / Round-2 / Round-1 lessons";
- "Per entry";
- "Hard rules";
- "Lane";
- "Hand-back".
NF44's own LESSONS.md and `$S/w22/agentNF41/LESSONS.md` are the newest.

The main lessons, repeated here:
- No new local model of a class that has a shared header. That includes renaming a stand-in to the real class name, and including
  a header under a renaming macro.
- No cast addresses.
- Overlay headers carry `#ifdef` opt-ins only. A body edit must be token-identical without the opt-in, and you must say so in
  hdr/README.md.
- Work only in `$S/w22/agentNF56/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF56 > $S/w22/nflocks/<addr>/owner; true

- NF55 holds its rows as `NF55`; none is in your rows.txt. If a lock you need is NF55's, record the row `deferred: held by NF55`
  and move on.
- Header overlays: NF55 may add opt-ins to the same headers. Keep your macro names specific to the member
  (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF56/)
- rows/;
- hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly:
- rows ported, not-a-binder, blocked and deferred;
- entries done and blocked;
- header opt-ins;
- rule slips.
