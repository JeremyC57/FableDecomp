# NF464 hand-back (2026-11-26): GetPreferredPos unit, WatchFor* callbacks, EnumerateScene binder

`NOTES_agent.md` is the agent's own notes and resume point. `QUEUE_RESULTS.tsv` and `RULINGS_NEEDED.tsv` are its queue verdicts and its questions.

## Landed: 6 rows, +5 proven
- **bQ, 0x006EA770:** the callee 0x006E89B0 is respelt from ComputeAttackVector to the PDB name `GetAttackerPlacementVector` (NF463's respell, gated again by NF464).
  - The T3 nf287 line for this address was released (`REL=nf287`, ruling r464d).
  - Gates: RELOCATION_MATCH PASS on both sides; WARN 11 → 10; objproof ALL_EQ; `.drectve` unchanged; 0 link duplicates.
- **bW (landed as `bWa`): 0x00D16DC0, 0x00E38F80, 0x00F083A0, 0x00F08A30, 0x00F08C30.**
  - The WatchFor* callback parameter is respelt from the local `CScriptThingRuntime` to `CScriptThing` (`engine/CScriptThing.h`). The three AmbushTraders binders land as one unit.
  - Table: 3 replace, 1 delete.
  - Landing note: the first try held 3 rows on the landed-self-symbol check, because the batch's reasons lacked the lander's `; target <addr>` callee-only marker. `bWa` is the same batch with that marker added; the recheck grades were already RELOCATION_MATCH PASS.
- **Tracker:** 0x006E89B0, 0x00D17090, 0x00E39140 and 0x00F08590 moved from split_fixable to agree_pdb; 0x00D16FD0 moved from agree_unverified to agree_pdb. Nothing got worse.
- **Checks:** bindings_multi 1; funclets 1,704, 0 broken; s units 159, 0 failed; descriptive_check 2,030, 0 FAIL.

## Rulings (coordinator, on precedent)
- **r464d: granted.** The nf287 subject (the `CCombatPositionMethods` stand-in spelling) is gone from every source and HEAD object, and all 8 retail callers bind the PDB name.
  - 0x0093DCD0 still reaches the name through a local `CCombatManager` declaration, which is HEAD debt (r460d forbids new ones). It is queued as T3 nf464 0x0093dcd0.
- **r464c: granted.** The parameter-only respell is accepted. The two-view script models are existing debt (r457d).
  - The PDB `WatchForDecapitation` spelling is now referenced by a row while 0x00D16FD0 has no row of its own. The unresolved total fell from 403 to 400.
- **r464a: granted on condition.** The extra pick-any `??1CEngineSceneEnumerator` COMDAT is the one 0x00BACA20 already emits at HEAD, at no retail address.
  - bE (0x00B321B0) carries a `CEngineScene.h` opt-in, so it lands in the next header round with one equiv_check for the round (speed-up D2).
- **r464b: held.** 0x00B320A0's inline enumerator would add a `??0` beside the strong landed constructor: a new duplicate under r457e. Queued as T3 nf464 0x00b320a0.

## Queued
- NAME_FIXES_QUEUE nf464: 0x00BACA20's constructor spelling (its caller 0x00B321B0 needs the STLport map type).
- About 9 more WatchFor* callbacks of the same shape (see NOTES_agent.md) for the next bulk pass.
