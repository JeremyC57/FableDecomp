# NF440 hand-back (2026-11-25): CCountedPointer best-guess names

`NOTES.md` has NF440's method and every gate. `evidence/` holds the predictor output, the gate results and the link logs.

## Landed: 14 rows (`batches/2026-11-24_landing_nf440_nf440_acc1`, `..._acc2`)
These rows are self respells. Each one moves a `CCountedPointer` stand-in self (`CUnprovenPointee_<va>` or `CCountedPointer_<va>`) onto the name the binding table already binds at that address from a landed caller.
- **acc1 (5 rows):** 0x00535EBB, 0x00578A8D, 0x007044D0, 0x008AA8E0, 0x00CDD4F0. Two of them are file renames, and four Module fields move.
- **acc2 (9 rows):** NF428's hold_r1 `Reset` rows: 0x0051CEA0, 0x00675060, 0x006750D0, 0x00771B60, 0x008AB2B0, 0x00A44170, 0x00AC38A0, 0x00B37B90, 0x00BB64A0.
- **Registry:** all 14 rows go into `rebuild/oracles/descriptive_names.tsv` as `one-source:caller-binding`. R-NF428-1 counts a caller binding as one source, so these names stay apart from proven.
- **Coordinator checks at landing:**
  - header-free, so no equiv rerun (owner speed rule);
  - `descriptive_check.py`: 14 lines, 0 FAIL;
  - tracker per address: 14 rows go split_fixable → agree_descriptive, and nothing else changes. Proven stays at 34,391, and the best-guess total is 34,405.
  - bindings_multi 1 (NullCoord), funclets 0 broken, s units 0 failed.

## R-NF440-1 (advisor decision, owner delegated, 2026-11-25): granted (a) and (b)
**(a) Gate change.** `PDB_NAME_ELSEWHERE` is gone.
- The rule names one PDB name set as a valid basis.
- The name sets carry FableWin / Ego_r addresses, so a name being in a set says nothing about where it lives in retail.
- `SYMBOL_ELSEWHERE` (bound or defined at another retail address) is the real clash test.

`ADDRESS_PROVEN` now fires only when the address carries a PDB name other than the listed one.

**(b) Tracker change.** `naming_status.py` checks the registry before PDB membership, so a guessed PDB name grades agree_descriptive and never counts as proven. A registry-named target is never T1, so caller respells do not treat a guess as a known target.

Self-test: a PDB-named registry line at 0x00401067 took that address out of proven (34,391 → 34,390) and graded it agree_descriptive. The best-guess total did not move. The line was then reverted.

## Landed later the same day: 170 rows (`batches/..._nf440_bg1p` .. `_bg4p`, 50 / 50 / 50 / 20)
NF440's emitter pilot cleared all of them.
- **Emitters:** NF440 compiled 4,909 candidate rows whose include tree spells `CCountedPointer<T>`. Every object that defines a guessed symbol was run through pk_all against its batch's addresses.
- **Result:** no reached NEQ, and no relocation maps a guessed name to another address.
- **Debt:** 145 lines of unreached NEQ copies, in `bgNp/DEBT.tsv` (per-row tables in `bgNp/emit_table.tsv`). The nf440 T3 family line covers them.
- **Registry:** 170 lines, basis `one-source:tu-order`.
- **Accuracy (NF440's leave-one-out figures):**
  - T is about 90% right unanchored (19 of 21) and 18 of 18 anchored;
  - `~` / ReleaseData is about 81–83% right;
  - so about one name in four is wrong somewhere.
- **Coordinator checks at landing:**
  - header-free, so no equiv rerun;
  - registry check: 184 lines, 0 FAIL;
  - tracker per address: 170 standin T3 → agree_descriptive, and nothing else moves;
  - proven stays at 34,391; the best-guess total is 34,575 (53.99%);
  - bindings_multi 1 (NullCoord), funclets 0 broken, s units 0 failed.

## Earlier hold, now resolved: 170 rows (`nf440_bg1..bg4`)
These are layout-predictor guesses. They are blocked by emitters outside NF428's 151 objects.
- Binding a name to an address makes every landed object that emits that name comparable to the address.
- **Next step:**
  1. Grep `rebuild/src`, `rebuild/tests` and `rebuild/include` for each T inside `CCountedPointer<`. Rows whose T appears nowhere else can land.
  2. For the rest, compile the emitters and run pk at the guessed addresses, with bg1 as the pilot.
  3. A reached NEQ holds the row. An unreached NEQ is debt tagged to its registry line.
- **Accuracy (NF440's own leave-one-out figures):**
  - T is about 90% right for unanchored rows;
  - the `~` / ReleaseData choice is about 80% right;
  - so about one name in four is wrong somewhere. The rule allows that, and the registry keeps every guess correctable.
- `evidence/exposed_debt.tsv` (12 lines) and acc2's six unreached copies are debt tagged to their registry lines.
