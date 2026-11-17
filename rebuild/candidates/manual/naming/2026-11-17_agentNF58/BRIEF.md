# Name-fix agent NF58 (2026-11-17, round 7): the STLport tree / list stand-in self names (76 self rows, 163 rows)

Owner: "Continue the naming fix queue with two agents until complete" and "Keep going until queue is done". Round 7 has fixed
about 800 caller rows; the original queue is landed (NF53, NF54, NF55 at 3912175f64). NF56's follow-up slice is being landed by
the coordinator, and NF57 (the follow-up entries that need a name settled) is running beside you.

## Your slice: the STLport tree / list stand-in family (queue entry q10437, 76 self rows, 163 rows)
NF50 fixed the `_Rb_global<bool>::_Rebalance` binding in 79 rows, but 78 of them kept stand-in SELF names, such as
`CRetailTree_longkey_buy004e4fef::Insert`, `CRbTree::_M_insert`, `CThingMap64::Insert`, `list_T_00a81470::~list_T_00a81470` or
`CRetailTree_erase00630d90::~CRetailTree_erase00630d90`. CLAUDE.md bars such placeholders in landed code. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf50/README.md` ("How e39 was ported");
- `rebuild/candidates/manual/naming/2026-11-17_agentNF50/NOTES.md` (section "Self name still a stand-in" is your list) and
  LESSONS.md, and NF50's step-2 rows: the 10 rows it already renamed to the STLport `_M_insert` via explicit instantiations of
  stlport/fable_map.h's `_M_insert` under `FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY` (grep the landed rows for that macro).

Files in `$S/w22/agentNF58/`:
- `ENTRIES.tsv`: one line per self row (76): row, its current stand-in self symbol, its landed callers (from NF50's table).
- `rows.txt`: the 163 rows you may change (the 76 self rows that are landed plus their landed callers).
- `EXCLUDED.txt`: rows you may NOT touch: 0x0076A110 / 0x00769980 belong to NF57 (q10438); 0x0069D520 is held by another agent
  (record its entry `deferred: held`, unless its lock is gone when you reach it; then claim it and work it).

Per self row:
1. **Prove the instance type from two sources.** The caller is usually `insert_unique` / `insert_equal` / `operator[]` of a map or
   set that is a PDB member (Ego_r.pdb / FableWin.pdb struct layouts in ghidra_out/struct_layouts_egor.tsv and the PDB name sets
   for `_Rb_tree<...>` instances), so the key / value types come from that member's declared type and the caller's own class. The
   node size and the copy of the value in `_M_create_node` must agree with that value type.
2. **Port the caller first** (it binds the stand-in today) onto the real container from stlport/fable_map.h /
   stlport/fable_containers.h and the class's shared header, then **re-author the self row** as the explicit instantiation of the
   STLport member (`std::_Rb_tree<K, std::pair<const K, V>, std::_Select1st<...>, std::less<K>, std::allocator<...> >::_M_insert`,
   `std::list<T>::~list` / `_List_base<T>::clear`, ...). STLport spellings are descriptive-by-library: template arguments only
   where callers or PDB members prove them.
3. RENAMES section 1 for the self row: module = the instance class (e.g. `std::_Rb_tree<long, std::pair<const long, CFoo>, ...>`,
   as NF50's ten step-2 rows were catalogued), name = the member. Section 2: every caller's respelled binding.
4. If the caller can not move within the budget (it models a dozen classes locally), both rows are `blocked/port` with the class
   chain named. Do not rename a self row whose callers still bind the stand-in spelling unless those callers move in the same pass
   (a self rename without its callers splits the binding).

Groups: most rows are the ~45 `CRetailTree_{long,ulong}key_buyNNNN::Insert` rows (long or unsigned long keys; each has a `buyNNNN`
node-buy helper named after its own address). Port 2-3 of them by hand, then script the rest (see "Method").

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. A caller that already binds correctly and
needs no change is `not-a-binder` (say why). Entries: `done` when the self row and all its callers are ported or not-a-binder.

**Do NOT re-probe the whole slice.** The list comes from NF50's NOTES.md table (as of 3912175f64).

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
- Work only in `$S/w22/agentNF58/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF58 > $S/w22/nflocks/<addr>/owner; true

- NF57 holds its rows as `NF57`; none is in your rows.txt. If any lock you need is held by another agent, record the row
  `deferred: held by <owner>` and move on.
- Header overlays: NF57 may add opt-ins to the same headers. Keep your macro names specific to the member
  (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF58/)
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
