# NF443 hand-back (2026-11-25): T1 list B caller respells, round 2

`NOTES.md` has NF443's method and gates; `RESIDUE.tsv` (1,136 lines) lists what is left, and `tools/` holds the generator (`rebind443.py`, NF438's rebind438 extended).

## Landed: a1, a2, a4, a5, c1 (238 caller rows)
- **What changed:** caller-only respells. Each row's own self is unchanged, and no header is touched.
- **Generator extensions:**
  - protected and private PDB members declared with a friend line (NF438 q1);
  - a free stand-in becomes a static member when the calling convention is the same;
  - const-away casts;
  - c1 edits the model of the CCombatSequenceBase ctor at 0x008B0AB0 so it takes references.
- **NF443's gates per row:**
  - vland HEAD = new;
  - tests PASS;
  - objgate ALL_EQ;
  - name_gate no worse;
  - pk shows 0 new NEQ;
  - the /FORCE link adds no duplicates;
  - xrcheck resolves every respelt callee to its target.
- **Table:** the delete and add lines are witnessed in the same batch. The cross-batch rewitness lines of a1, a2 and a5 were applied with apply_table.
- **Debt:** 160 rows gained a bodiless class (r3), booked as one header-debt line (`T3.txt`).

## r1 (coordinator, 2026-11-25): declined; a3 held
In a3, five tests keep the stand-in stub and add a PDB-named stub that forwards to it. That keeps the old spelling defined beside the PDB name, which breaks one name per address. NF438 q2 already set the form: the stub is renamed with the row, as in a1.

a3 (49 rows) is held as a whole, because a delete witnessed by one row could otherwise drop a binding a held row still uses. NF443 is switching the five tests and re-gating.

## Held rows (12, in `*_hold`)
| Rows | Reason |
|---:|---|
| 6 | change the code (long to float / bool conversions) |
| 4 | worse on name_gate (the bodiless class adds SHARED_HEADER_AVAILABLE) |
| 1 | 0x00698D10, on the `?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ` clash between 0x00789CC0 (self) and 0x007CBF50 (bound via row 0x007BDCFC) |
| 1 | 0x009D0380, whose callee 0x009CFBC0 is Codex-reserved |

## Round 2 (landed): a3 (49 rows) and d1 (21 rows)
- **a3:** the five tests now use the a1 stub form (r1 resolved). NF443 re-gated a3 on the new HEAD.
- **d1:** the 12 g4_bin_5 rows plus the 13 leftover CCombatSequenceBase ctor rows. Its delete closes 0x008B0AB0.
- **Tracker:** 29 addresses go split_fixable → agree_pdb, none worse. Proven count: 34,506.
- **Name fixes (`round2/NAME_FIXES.tsv`), sent back to NF443 as one unit batch.** The three addresses all carry the catalog name `CTCDivineWrath::GetDamageMultiplier`; the fix renames them together with their callers 0x00698D10, 0x007BDCFC and 0x00868B40:
  - 0x0078E7B0 → GetDamageMultiplier;
  - 0x007CBF50 → `CTCMultiArrow::GetPerArrowDamageMultiplier`;
  - 0x00789CC0 → `CTCHeroRangedAttacks::GetProjectileWeaponFirstPersonTargetingModeZoomFOV`.
- **r2 (coordinator): granted.** An enum-only `engine/EOracleMinigameInput.h` (EGameAction.h form), included by CTCOracleMinigame.h inside its retail-class branch and by row 0x00691680, shipped as a `_hdr` batch.
