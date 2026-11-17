# Name-fix agent NF55 (2026-11-17, round 7): the game-side tail of the queue, second half (39 small entries, 70 rows)

Owner: "Continue the naming fix queue with two agents until complete". Round 7 has fixed about 570 caller rows so far. The latest
landings are 99125bceeb (NF51) and 3a2abf8762 (NF52); NF54 (the first half) has handed
back and the coordinator is landing it now. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf52/README.md` and `2026-11-17_landing_nf51/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF52/NOTES.md`, plus the LESSONS.md of NF51 and NF52;
- `$S/w22/agentNF54/NOTES.md` and `$S/w22/agentNF54/LESSONS.md` (your sibling half, same kind of rows).

## Your slice: the second half of the game-side tail (targets below 0x00800000)
The big families are done. What is left is a long tail of small entries, cut by code area:
- NF53 (running beside you) has the engine side, targets at or above 0x00800000.
- The game side was split by entry and target address into two halves: NF54 had the first half (done, being landed) and you have the second.

`WORKLIST.tsv` holds every wrong binding in your rows, including lines from entries in the second half. Fix every wrong binding
in each of your rows.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed.

The rows in `$S/w22/agentNF53/rows.txt` belong to NF53, and none of them is in your worklist. Never touch an NF53 row; the lock
files enforce this. Do not touch rows outside your worklist either: the first half's rows were NF54's and are being landed.

**Your 39 entries** are in `ENTRIES.tsv`: source half, entry, address, correct symbol, status and the previous agent's note. Each
has 1-12 binder rows. Typical claims:
- CInputProcessGameBase::IsEventGameAction / IsGameActionEventInQueue;
- CGameTimeManager::IsTimeBetween (e105: NF54 did its first-half binders; the rest are yours);
- `CArray<pair<unsigned long, C3DVector>>::push_back`, `CVectorMap::LowerBound`, `vector<pair<long,CCharString>>::erase`;
- CPersistContext::Transfer<CFaction> / Transfer<CIntelligentPointer<CThing>>;
- two `std::_Rb_tree<long,...>::find` entries marked BINDING CONFLICT with 0x00B2EB20 (e221 / e195): verify which instance each
  binder really calls before respelling anything;
- CThingCreatureBase, CTCBoss, CTCSpecialAbilities, CTCVillage, CTCShopKeeper, CTCTalk, CTCCombat, CTCPhysicsStandard and the
  CThingSearchTools templates.

An entry may also have had binder rows in NF54's half (e105 and nf44:1 are examples). Judge the entry over your rows plus NF54's
APPLIED.tsv: mark it `done` only when both halves are done, else `blocked` naming the rows left.

**Work entry by entry, in ENTRIES order:**
1. **Verify the claim** from two sources: PDB name sets, the callee body, vtable slots, and the landed callee row's own leaf.
   - Many of these were queued from one author's evidence. A claim that does not hold goes to REJECTED.tsv with the reason.
   - `CLAIM` / `probable` / `BINDING CONFLICT` notes need extra care.
2. **Confirm** with `xrow.sh` on the repo copy that the row still binds the wrong spelling; otherwise mark it `not-a-binder`.
3. **Respell or port:** include the shared header (adding an `#ifdef` opt-in in your overlay if a member is missing), delete the
   local stand-in, and grade.
4. **Self rows:** when the callee's own landed row is the one that is wrong, rename it in RENAMES section 1, and move its landed
   callers in the same pass.

**The worklist is already probed; do NOT re-probe the whole half.** `WORKLIST.tsv` has 70 distinct rows, 79 lines, with columns
entry, target, correct_symbol, binder_row and bound_symbol_at_probe.
- It comes from NF41's probe and NF44's APPLIED binder lists, minus every row landed since, minus NF53's rows and minus NF54's.
- NF44-half lines have an empty bound_symbol column. Read the binding from your confirming compile.

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
- Work only in `$S/w22/agentNF55/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF55 > $S/w22/nflocks/<addr>/owner; true

- NF53 holds its rows as `NF53`. If a lock you need is NF53's, record the row `deferred: held by NF53` and move on.
- NF54's header opt-ins (`$S/w22/agentNF54/hdr/README.md`: NGameText_CDataBank.h, CPersistTraits.h, CWorld.h,
  boost/scoped_ptr.h, CWorldMap.h, CAnimInit.h, stlport/fable_containers.h) land in the repo during your run. If you overlay any
  of those headers, copy the repo version again before your final recheck and re-add your opt-ins to it.
- Header overlays: NF53 may add opt-ins to engine headers. Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF55/)
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
