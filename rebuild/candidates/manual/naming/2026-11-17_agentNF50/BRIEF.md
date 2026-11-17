# Name-fix agent NF50 (2026-11-17, round 7): STLport red-black tree internals (_Rebalance and _Rb_tree members)

Owner: "Continue the naming fix queue with two agents until complete". Round 7 has fixed about 315 caller rows so far. The latest
landings are 69ed47f1aa (NF47) and c2c0b478b3 (NF48). Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf48/README.md` (the STLport vector family; same method);
- `rebuild/candidates/manual/naming/2026-11-16_agentNF48/NOTES.md` and `LESSONS.md`;
- NF42 / NF44 NOTES on stlport/fable_map.h (`FABLE_RETAIL_STLP_RB_GLOBAL_ROTATE`, `FABLE_RETAIL_STLP_MAP`,
  `..._INDEX_DECL_ONLY`).

## Your slice is a FAMILY, not a tag range
Each agent in round 7 gets a closed family of binder rows. **Every row must end `ported`, `not-a-binder` or `blocked`.**
`carry` is not allowed inside the slice.

NF49 is running beside you on the definition-manager templates. The rows in `$S/w22/agentNF49/rows.txt` are its rows, and none of
them is in your worklist. Never touch an NF49 row; the lock files enforce this.

**Your 9 entries** are in `ENTRIES.tsv`: source half, entry, address, correct symbol, status and the previous agent's note.

**The big one: nf44 entry 39, `?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z` (0x0042971D).** 79 landed rows
bind it under stand-in names.
- Most of them are STLport `_Rb_tree<...>::_M_insert` instances landed under stand-in classes such as
  `CRetailTree_ulongkey_buy0042b5a8::_Insert`. Their header comments carry the retail byte shape and name their Buynode.
- stlport/fable_map.h already declares `static void __cdecl _Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&)` on
  `_Rb_global<bool>` (line 48). Read the opt-ins around it.

**Two steps per row:**
1. **Required: the callee binding.** The call at the retail site must bind the STLport `_Rebalance` symbol above. Do it through
   stlport/fable_map.h (include it and call `std::_Rb_global<bool>::_Rebalance`, or `_Rb_global_inst`), never through a local
   declaration. This is what closes entry 39.
2. **Only where the callers prove it: the row's own name.** If the row is an `_Rb_tree<K,V,...>::_M_insert` instance and its callers
   (or the Buynode, node layout and comparison) prove the exact key and value types, move the row onto the header's real
   `_Rb_tree` / `map` instance. Its self symbol then becomes the STLport `_M_insert` (RENAMES section 1, descriptive-by-library).
   Never invent template arguments. If the instance is not proven, keep the stand-in self name, do step 1 only, and list the row
   in NOTES under "self name still a stand-in" with what is missing.

**The other entries:**
- `_Rb_tree::insert_unique` (map<unsigned long, NUISystem::CUIState>);
- `_Rb_tree::_M_find` (map<CComponent*, CList*>);
- `_Rb_tree::_M_erase` (map<unsigned long, CCharString>);
- `_Rb_tree::find`: map<unsigned long, long> and map<EOnlineScoreboardActivity, ...>;
- `_Rb_tree::clear`: map<CWideString, bool>, two queue lines for one address;
- `_List_base::clear` (list<CCountedPointer<CNavRegionConnection>>).

Verify each claim first, from the callee body, the element types and the callers.

**The worklist is already probed; do NOT re-probe the whole half.** `WORKLIST.tsv` has 99 distinct rows, 101 lines, with columns
entry, target, correct_symbol, binder_row and bound_symbol_at_probe.
- It comes from NF41's probe and NF44's APPLIED binder lists, minus every row landed since and minus NF49's rows.
- Before editing a row, compile its repo copy once with `xrow.sh` to confirm it still binds the wrong spelling. If it no
  longer does, record it `not-a-binder`.
- NF44-half lines have an empty bound_symbol column. Read the binding from your confirming compile.

**Overlaying the STLport headers:** NF41 overlaid the root `#pragma once` fable_string.h and broke other TUs. Overlay `stlport/*.h`
as a full copy plus `#ifdef` opt-ins, and regrade one unrelated includer before you hand back. NF48 did this for
fable_containers.h.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the rows by stand-in shape. Start with the 79 entry-39 `_Rebalance` binders; the `CRetailTree_*::_Insert` rows share one shape.
2. Port 2-3 rows of the group by hand, using these steps:
   - include the header, adding an `#ifdef` opt-in in your overlay for any member it lacks;
   - delete the local model;
   - call the real STLport member (`_Rb_global<bool>::_Rebalance`).
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
- Work only in `$S/w22/agentNF50/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF50 > $S/w22/nflocks/<addr>/owner; true

- NF49 holds its rows as `NF49`. If a lock you need is NF49's, record the row `deferred: held by NF49` and move on.
- Header overlays: NF49 may add opt-ins to engine headers. Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF50/)
- rows/;
- hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per worklist row, with its terminal status);
- APPLIED.tsv (the five entries, plus any extra queue lines you closed);
- REJECTED.tsv, NOTES.md and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly:
- rows ported, not-a-binder, blocked and deferred;
- entries done and blocked;
- header opt-ins;
- rule slips.
