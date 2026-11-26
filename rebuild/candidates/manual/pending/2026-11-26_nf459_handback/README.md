# NF459 hand-back (2026-11-26): HeroRunningStop identity, creature-action ctor units, particle-group view

`NOTES.md` covers the method and resume point; `RULINGS_NEEDED.tsv` holds the agent's questions; `ties_b9.txt` has the name ties.

## Landed: 26 rows (+10 proven, +4 best guesses)
- **b7: 0x0085BBA0 is CCreatureActionHeroRunningStop's ctor** (r457a), now `??0CCreatureActionHeroRunningStop@@QAE@AAVCThingCreatureBase@@@Z`.
  - The catalog Module change was applied first, then the file rename. `functions.tsv` was updated by `apply_renames`.
  - Its NF457 queue line is marked done.
- **b9: 25 rows in 12 units**, `(CThingCreatureBase*)` → `&` ctors with their callers.
  - The Tentacle unit: 0x008CF990 / 0x008CFA10 with caller 0x008CF7D0.
  - 11 one-argument ctors, each with its caller.
  - 12 table replaces.
  - 4 FableWin-only best guesses: 0x00690DB0, 0x008E43A0, 0x008E4420, 0x008CFA10.
- **Coordinator checks:**
  - bindings_multi: 1;
  - funclets: 1,704 checked, 0 broken;
  - s units: 159 checked, 0 failed;
  - descriptive_check: 2,026 lines, 0 FAIL.
  - Tracker: 10 agree_unverified → agree_pdb and 4 → agree_descriptive, with nothing worse.

## Rulings (advisor decision, owner delegated)
- **r459a: short COMDAT vftables. Existing debt; b7 lands.** The coordinator read the objects:
  - the ctor rows on local models emit `??_7<Class>@@6B@` as a 4-byte COMDAT with selection 2 (pick any);
  - the Clone / deleting-dtor rows emit the full 324 bytes under the same name.
  - So a real link may keep the short table. This is logged in FINDINGS_LOG, the fix (the shared-header port) is on the T3 nf459 line, and the count across all landed objects is still to be measured.
- **r459c: the Tentacle unit lands.** `xref` shows the only retail call to 0x008CF990 is at 0x008CF87C, inside 0x008CF7D0. 0x008CF700 inlines an invented one-argument ctor, which is queued as port debt.
- **r459b: 0x00BBA140 keeps its local model.** A research item asks which part of `<new>` emits `/DEFAULTLIB:"libcp"`, and whether a guard in `fable_containers.h` could suppress it. That would change every container row, so the owner decides.
- **r459d: the b8 containers-free particle-group view is granted, on three conditions:**
  1. equiv shows nothing differing outside the batch;
  2. the opaque members keep their PDB names, offsets and sizes;
  3. the destructor takes slot 0, and an `#error` fires on conflicting macros.

  b8 lands in a separate commit once the equiv check passes.

## b8 landed (after equiv)
7 rows: 0x00BBA140 (destructor QAE→UAE, still on its local model per r459b) and the six particle-group deleting destructors on the new containers-free view; equiv: 57,160 rows checked, only the six batch deleting destructors differ, 0 outside (`EQUIV_CHECK_b8.txt`); +1 proven (0x00BBA140).
