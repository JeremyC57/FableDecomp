# NF457 hand-back (2026-11-26): creature-action ctor respells

`NOTES.md` covers the method and the resume point; `RULINGS_NEEDED.tsv` holds the agent's questions.

## Landed: 38 rows (+26 proven, +6 best guesses)
| batch | rows | what |
|---|---:|---|
| b5a_creature_ctor_selfonly | 26 | one-argument `(CThingCreatureBase*)` → `&` ctors with no retail caller (xref: no call, jmp or absolute reference); 2 are FableWin-only best guesses |
| b5b_creature_ctor_inline_units | 9 (4 units) | 4 FableWin-only ctors with the 5 rows that model them in-class; each construction call becomes `X(*p)` |
| b6_buyhouse_unit | 3 | NF456's held unit: ctor 0x0072E690 → `(CThing&)`, factory 0x004D321B onto its PDB `?Alloc@…`, and 0x0072F040's in-class ctor; 1 table replace |

- **Gates (per NF457):** vland RELOCATION_MATCH both sides, identical WARN sets, objproof ALL_EQ under the intended renames, link 0 new duplicates.
- **Coordinator checks:**
  - bindings_multi: 1;
  - funclets: 1,704 checked, 0 broken;
  - s units: 159 checked, 0 failed;
  - descriptive_check: 2,022 lines, 0 FAIL.
  - Tracker: 25 agree_unverified + 1 split_fixable → agree_pdb, 6 → agree_descriptive, and nothing worse.
- **The brief's premise was wrong:** the "one landed caller" ctors in NF456's census have no retail caller at all.

## Rulings (coordinator, on standing precedent)
- **r457a: granted.** 0x0085BBA0 is CCreatureActionHeroRunningStop's ctor, not Decapitation's: it stores RTTI vftable 0x012767EC and both PDBs have the name. The batch carries no catalog rename, so it lands with the next agent together with one.
- **r457b: declined as built.** The particle-group header port adds `/DEFAULTLIB:"libcp"` to the row objects, which was declined in r450a. Redo it with a containers-free view (r451a) and move 0x00BBA140 onto the header as well. The PAU→PAV rekey of invented selves is fine (r456a).
- **r457c: proposal accepted for the queue.** QuestCard 0x007026A0 is the ctor's home; 0x00702A50 should become a thin row on it.
- **r457d: accepted as existing debt (r452b).** In b5b and b6, each caller's in-class ctor model is emitted as a COMDAT duplicate. That already happens at HEAD; the respell only renames it.
- **r457e: held.** 0x008B9150 would add a new duplicate DEF.

All open items are on the T3 nf457 family line; the rows are under COORD_HOLD_NF457.
