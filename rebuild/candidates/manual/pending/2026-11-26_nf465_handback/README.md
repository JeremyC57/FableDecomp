# NF465 hand-back (2026-11-26): scripted bulk run over the 392 unheld T1 targets

The pool was every split_fixable T1 target not held by T3_PORT_QUEUE or a lock. `POOL_HOLDS.tsv` has a verdict per target, and `NOTES_agent.md` gives the yield per shape and the resume point.

## Landed: d1_dataglobals, 9 rows, +2 proven
- **0x013750B0 `?IMERenderAlpha@@3EA`:** 4 rows. **0x013B8A88 `?HeroSwapInProgress@NThingManager@@3_NA`:** 5 rows. Both names are Ego_d-only.
  - Each row's file-scope extern stand-in is renamed, in the row and in its test. The table loses 4 stale lines.
- **Gates:**
  - full caller census (retail references): 104 rows, 0 BAD;
  - vland RELOCATION_MATCH PASS on both sides; WARN sets identical;
  - objproof ALL_EQ, `.drectve` included; pk 0 NEQ;
  - link: 0 duplicates; unresolved 34 → 32.
- **Coordinator checks:** bindings_multi 1; funclets 1,704, 0 broken; s units 159, 0 failed; descriptive_check 2,030, 0 FAIL.
- **Tracker:** exactly the two targets moved from split_fixable to agree_pdb.

## Finding: the scripted call-site rename pool is exhausted
- No target could be done by table edits alone; every one still has a live non-PDB binder in a compiled object.
- 235 of the 392 need ports:
  - 197 `??_7` targets whose binders are local class models;
  - 38 data targets that are class static members.
- 92 are generator residue: rebind443 made 10 rows on 6 targets, ctor449 and dtor449 made nothing.
- The remaining naming lever is ports onto the shared headers. That is the same work as the short-vftable fix (NF461, r459a), so the two are one job.

## Rulings
- **R465a: granted.** Respell the 0x01436E24 Ghidra label `g_pEngineEnvironmentDef` to the PDB name `?GPEnvironmentDef@NEngineInternal@@3PBVCEnvironmentDef@@B`, in the same commit as its 9 respelt binder rows (r463c precedent; `NAME_FIXES.tsv`). The rows were not all kept in `held/`, so this is queued for regeneration.
- **R465b: declined.** A bodiless local class for a class that has a shared header is a local model (r456b / r460d). The 10 rebind443 rows stay held.
- **R465c: not lifted.** The VIRTUAL_SCREEN_WIDTH / HEIGHT owner gate stays until the owner says otherwise.
- **R465d: declined.** It is the D1 proposal, and the owner chose to keep the T3 hold. The 28 cos-table / 0x01436E74 rows land with their T3 lines' own ports.
