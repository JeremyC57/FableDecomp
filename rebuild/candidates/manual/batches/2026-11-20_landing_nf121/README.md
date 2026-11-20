# 2026-11-20 landing nf121: the CTCCarrying family (13 rows)

Records: `naming/2026-11-20_agentNF121/` (selfcheck 0 problems). 6 entries: 3 done (q4470 0x006A1490, q4471 0x006A1120, q4472
0x006A19B0), 2 done-already (q4473 0x006A11F0, q4984 0x006A1540), 1 deferred (q4469 0x006A1060: its last binder 0x006A1E00 belongs to NF122,
which has the two-line recipe). 58 rows examined: 13 ported, 44 not-a-binder, 1 deferred.

## Rows (13): 13 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Self leaf respells (RENAMES s1, catalog names unchanged, no file moved): 0x006A1060 IsCarryingThing and 0x006A1490
  IsCarriedThingInPrimarySlot take `const CThing&` and are const; 0x006A1120 GetCarriedThing is const; 0x006A19B0 RemoveThing takes `CThing&`.
  Ego_r.pdb + FableWin.pdb publics carry only these spellings; the bytes are the same (the reference is the pushed pointer).
- 9 binders respelled in place inside their existing local CTCCarrying models (round-9 rule): 0x005C3A95, 0x006647F0, 0x006648D0,
  0x0069F9E0, 0x0078E9C0, 0x00793EC0, 0x008BA3C0, 0x008C6A50, 0x009647C0. No new local model, no header change, no flags.
- Advisor rule (owner delegated): existing splits may shrink, none may be created. 0x006A1060 goes from 3 spellings to 2 (only
  0x006A1E00 keeps the old QAE_NPAV form); the claim was already bound by 9 rows, so no spelling is new. The self rename is kept.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. Referencing rows: 28 good / 0 bad. No-new-split gate 0 (`logs/split_gate.txt`:
0x006A1060 existing, other = 0x006A1E00). Table spelling sets: 0x006A1060 3 -> 1, 0x006A1120 2 -> 1, 0x006A1490 2 -> 1; none grew.
NAME_FIXES queued: 0x006A1060 (row 0x006A1E00 recipe), 0x006A10A0 IsCarryingThing(long) const (0x006648D0 binds the non-const form).
