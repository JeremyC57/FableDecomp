# NF438 hand-back (2026-11-25): T1 caller rebinds, list A

**Landed: 161 caller rows** in `batches/2026-11-24_landing_nf438_{q1,q2,q3,r1,r2}`, landed in order.
- They make 167 callee rebinds onto 83 targets: 58 close fully and 25 partly.
- No header changes. Each row's own self is unchanged.
- The generator is new: `tools/rebind438.py`, driven by libclang.

**Gates:**
- vland: same grade on both sides, PASS;
- objgate: ALL_EQ, with the UND delta exactly old -> new;
- pk: 0 new NEQ;
- /FORCE: duplicates unchanged;
- every new name is in both PDB name sets and is defined by the target's own self object.

## Coordinator fixes at landing
- **Cross-batch table lines.** The lander applies a table line only when its witness landed in the same batch. 8 lines whose final action was a rewitness to a still-binding row outside the batch, or a delete witnessed by a row in another batch, were applied by hand: `batches/2026-11-24_landing_nf438_r2/TABLE_CHANGES_CROSSBATCH.tsv`.
- **Self symbols.** NF438's SELF_SYMBOLS.tsv has two columns (address, self_symbol). Merging it stripped the status column of 161 lines, and the tracker dropped 20 proven addresses to unmeasured. The lines were restored:
  - lines whose symbol was unchanged were restored from the committed file;
  - 43 new lines were built from the batch grade and the row's `// Leaf:`.

  `apply_ss.py` now reads columns by header and refuses a file without a status column.
- **Tracker, full per-address diff against the commit before:** 85 split_fixable -> agree_pdb, nothing else changed (NF438 and NF439 together).

## Rulings (coordinator, 2026-11-25)
- **q1: granted.** A bodiless declaration may declare a protected / private PDB member, so the call binds IAE / AAE, and give the calling local class or function access with a `friend` line. It adds no layout and no body, and the debt is booked as r3's. This covers 205 residue lines.
- **q2: granted.** A HEAD test that links the row object and stubs the callee under the stand-in name has its stub renamed with the row, callee only; name_gate --test must be no worse. This covers the 9 rows held for test LINK_FAIL.

## Held
16 rows (`*_hold/`):
- 9 under q2;
- 4 targeting 0x009BEF80, whose own row needs the `?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z` name fix;
- 3 with objgate not equal.

## Residue (`RESIDUE.tsv`, 1,795 lines over 826 targets)
| Reason | Lines |
|---|---:|
| special members and operators | 654 |
| no caller found | 255 |
| row screens | 253 |
| protected / private (now q1) | 205 |
| template scopes (GetSubDef<T>, CCountedPointer<T>) | 102 |

112 rows gained a bodiless local class (r3); that is booked as header debt.
