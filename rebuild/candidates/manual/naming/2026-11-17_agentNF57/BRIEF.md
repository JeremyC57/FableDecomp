# Name-fix agent NF57 (2026-11-17, round 7): the follow-up entries that need a name settled (32 entries, 79 rows)

Owner: "Continue the naming fix queue with two agents until complete". Round 7 has fixed about 800 caller rows. The original
queue is landed: NF53 (engine-side tail), NF54 and NF55 (game-side tail, 3912175f64). NF56 (the 61 follow-up entries with exact
symbols) is running beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf53/README.md`, `2026-11-17_landing_nf54/README.md` and
  `2026-11-17_landing_nf55/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF53/NOTES.md`, `2026-11-17_agentNF55/NOTES.md`, plus their LESSONS.md.

## Your slice: the follow-up entries whose correct name is not yet an exact decoration
`rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` lines tagged `land1116fu` / `land1117fu` are name fixes earlier agents
queued. NF56 has the 61 with exact symbols. You take the other 32: descriptive claims (`(unknown: ...)`), `(check: ...)` /
`(verify ...)` claims, and `(settle: A vs B)` cases. The 78-row STLport stand-in family (q10437) is NOT yours; it is a later slice.

Files in `$S/w22/agentNF57/`:
- `ENTRIES.tsv`: entry id (`q<line>`, the 0-based line of NAME_FIXES_QUEUE.tsv), tag, address(es), the claim, the evidence, and the
  self-leaf / note column as queued.
- `WORKLIST.tsv`: every landed binding of the entry addresses in rebuild/oracles/reloc_bindings.tsv (entry, target, binder_row,
  bound_symbol), whatever spelling it uses. Some already bind the right symbol; once you settle the name, those are `not-a-binder`.
- `rows.txt`: the 79 rows you may change (the landed self rows at the entry addresses, every binder row above, and callers the
  evidence names).
- `HELD_BY_NF56.txt`: 2 rows (0x0053BF20, 0x0053CFC2) your entries touch that NF56 holds. Record them `deferred: held by NF56`
  unless the lock is gone when you reach them (NF56 may finish first; then claim the lock as usual and work them).

Per entry:
1. **Settle the name first, from two sources** (Ego_r.pdb / FableWin.pdb name sets, the Ego_r source order around the address, the
   vtable slot, the callee body, strings, libpk headers, landed callers). Write the decision and both sources in APPLIED.tsv.
   - `settle: A vs B`: pick one with evidence, or reject both.
   - `unknown` / descriptive: the PDBs may not name it (libpk ships no PDB). A made-up name is allowed only when the original is
     found nowhere (PDBs, strings, FSE manifest refs/fse_api_manifest.json, EgoCore, sibling repos). It must say what the
     function or class does, be used for that address everywhere, and be marked descriptive in NOTES.md. No `Unproven`,
     `VirtualNN_<addr>` or address-suffixed placeholder classes in landed code.
   - A claim that does not hold goes to REJECTED.tsv with the reason.
2. **Self row:** re-author the row at the address under the settled spelling (usually on its shared header) and put it in RENAMES
   section 1 (address, module, name, mangled_leaf, old_catalog_name, old_leaf, evidence). `module` + `name` is the catalog name:
   the real class and member, with no `Class::` prefix in `name`. For a leaf-only change on a row whose catalog name is already
   right, repeat the old catalog name. Never make a descriptive stand-in class the catalog name when a better catalog name exists.
3. **Binder rows:** respell every landed caller in rows.txt to the settled spelling (section 2 of RENAMES: caller row, old symbol,
   new symbol, callee address), taken from before/after compiles. Callers outside rows.txt go to NAME_FIXES.tsv.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. A row already right is `not-a-binder` (say
why). An entry whose self row is already right and has no wrong binders is `done-already`.

**Do NOT re-probe the whole slice.** The worklist comes from reloc_bindings.tsv as of 3912175f64.

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
- Work only in `$S/w22/agentNF57/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF57 > $S/w22/nflocks/<addr>/owner; true

- NF56 holds its rows as `NF56`; only the two in HELD_BY_NF56.txt touch your entries, and they are not in rows.txt. If a lock you
  need is NF56's, record the row `deferred: held by NF56` and move on.
- Header overlays: NF56 may add opt-ins to the same headers. Keep your macro names specific to the member
  (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF57/)
- rows/;
- hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: the settled name with its two sources, then done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly:
- rows ported, not-a-binder, blocked and deferred;
- entries done and blocked;
- header opt-ins;
- rule slips.
