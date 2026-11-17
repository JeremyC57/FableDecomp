# Name-fix agent NF60 (2026-11-17, round 7): `pGlobalScriptDef` binders (2 entries, 205 rows)

Owner: "Continue the naming fix queue with two agents until complete" and "Keep going until queue is done". Round 7 has fixed
about 1,000 rows. NF59 (the follow-ups queued at nf55-nf57) is running beside you; NF58 (STLport tree stand-ins) is being landed.

## Your slice: `pGlobalScriptDef` (queue entries q10541 / q10542, 205 rows)
Ego_r.pdb and FableWin.pdb make the global at 0x0143E90C `CDefPointer<const CScriptDef> pGlobalScriptDef` (unqualified, class
type: they carry its `dynamic initializer` and `atexit destructor`). `rebuild/include/engine/fable_script_def_global.h` declares it
so, mangled `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A`. NF57 settled it (q10429) and moved 4 rows onto that header
(0x00D15A40, 0x00E7B970, 0x00F07B80, 0x0122AE7E: include fable_script_def_global.h with FABLE_RETAIL_DEF_CLASS_BASE /
CDEFSTRINGTABLE, drop the local CScriptDef). About 154 landed rows still bind `?pGlobalScriptDef@@3PAVCScriptDef@@A` (a plain
pointer), and others declare it locally under other spellings. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf57/README.md` and `naming/2026-11-17_agentNF57/NOTES.md`, LESSONS.md;
- the four NF57 rows above (their landed files) as the pattern.

Files in `$S/w22/agentNF60/`:
- `rows.txt`: the 205 landed rows that mention `pGlobalScriptDef` but do not include engine/fable_script_def_global.h (NF57's
  158-row list plus a source grep). Some may already bind the right spelling through a local `extern` of the same type; they are
  `not-a-binder` only if their emitted symbol is already exact (check by compiling), otherwise move them onto the header too.
- `HELD.txt`: empty today.
- q10542: 0x01224C99 is the `_$E1` (dynamic initializer) of pGlobalScriptDef. If it is a landed row, catalogue it in the
  `` `dynamic initializer for 'pGlobalScriptDef'' `` form (as 0x0122AE7E's atexit destructor is) and make it come from the same
  definition; if not landed, say so in APPLIED.tsv.

Per row: replace the local declaration (and any local CScriptDef / CDefPointer model that exists only for it) with
`#include "engine/fable_script_def_global.h"` and the macros it needs, then use `pGlobalScriptDef` through CDefPointer's
interface (`->`, `.Get()` or whatever the header provides; read engine/CDefPointer.h). The code bytes must stay identical: a
`CScriptDef*` read of a CDefPointer whose first member is the pointer may need the header's accessor that loads that member.
Record each row's old and new binding in RENAMES section 2 (caller row, old symbol, new symbol, 0x0143E90C). There are no self
renames unless q10542 needs one.

**This is a transformer slice.** Group the rows by the shape of their local declaration (grep it), prove the rewrite by hand on
2-3 rows of a group, script it, and grade the group in payloads of 20-30 rows. A row that models a dozen classes locally and does
not move within the budget is `blocked/port`, naming those classes.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed.

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
- Work only in `$S/w22/agentNF60/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF60 > $S/w22/nflocks/<addr>/owner; true

- NF59 holds its rows as `NF59`; none is in your rows.txt. If a lock you need is held by another agent, record the row
  `deferred: held by <owner>` and move on.
- Header overlays: NF59 may add opt-ins to the same headers. Keep your macro names specific to the member
  (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF60/)
- rows/;
- hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (two lines, q10541 and q10542: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly:
- rows ported, not-a-binder, blocked and deferred;
- entries done and blocked;
- header opt-ins;
- rule slips.
