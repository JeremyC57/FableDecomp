# NF458 hand-back (2026-11-26): the next stale binding-line candidates (A3 continuation)

NF458 took NF453's 490 `NEXT_CANDIDATES` lines under rulings r453a–e. It refreshed the census at HEAD 5288bedbe6 (42 rows had drifted) and recompiled all 8,330 relevant rows. `NOTES.md` covers the method and the resume point.

## Applied (`TABLE_CHANGES.tsv`)
- **107 deletes:** in 106 the live spelling is already in the table at that target; 1 is a second line covered by a replace.
- **105 replaces** (stale → live, r453b).
- **25 rewitnesses.**
- **Closing check:** `EVCHECK.tsv` 237/237.
- **Coordinator checks:**
  - the repo table was byte-equal to NF458's base;
  - apply 0 misses;
  - bindings_multi 1;
  - descriptive_check 2,022 lines, 0 FAIL.
- **Tracker** (against the NF457 state): **+57 done**, exactly as NF458 predicted.
  - 23 split_unknown → agree_std, 19 split_fixable → agree_pdb, 11 standin → agree_std, 2 standin → agree_pdb, 1 + 1 agree_unverified → done.
- **Worse-looking moves:** 3 split_unknown → standin (0x00C2C7B0, 0x00C705B0, 0x00C70F80). No object uses the deleted spelling there any more, so only the landed stand-in is left. At 0x00C705B0 / 0x00C70F80 the deleted `SVoiceTap` read better than the live `SUnprovenImpulse52`, so that is queued as a respell.
- **Limit on the libpk vtable names:** several standin → agree moves are libpk vftables named from landed code, which has no RTTI. Their agree_std is only as good as that code's names.

## Held (`HELD_LINES.tsv`, 253)
| reason | lines |
|---|---:|
| Not stale: the old witness still binds the spelling | 96 |
| r453d: a referencing row has no re-derivable relocations | 87 |
| Witness binds nothing at the target | 41 |
| Anonymous-namespace hash in the live spelling | 9 |
| Two table lines for one spelling and target | 8 |
| Other | 12 |

## Open questions (not decided; the lines stay held)
- **r458a:** 62 of the 87 r453d holds are blocked only by referencing rows that grade MATCH and whose compiled function has no relocations at all. Should those rows count as re-derived?
- **r458b:** 8 targets have two table lines for the same (spelling, target) with different witnesses. `apply_table` keys on (spelling, target), so it cannot drop one of them. Add a witness-keyed delete, or leave them?

## Queued
- NAME_FIXES_QUEUE nf458: 31 PDB respell lines, 1 identity check (IsPlayingVirtual at 0x00C20F40 vs 0x00C207A0), and the SVoiceTap respell candidate.
