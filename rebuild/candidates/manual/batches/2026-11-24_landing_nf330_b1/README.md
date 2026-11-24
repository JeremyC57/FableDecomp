# Landing nf330 b1 (Account A, 2026-11-24): 20 thin-family flat-header rows on 17 new or extended opt-in class models

Agent NF330 took the NF324 Q1 remainder: the 95 rows NF329 did not reach. It ran NF329's route; the rulings are in
`batches/2026-11-24_landing_nf329_b3/README.md`. Hand-back: `pending/2026-11-24_nf330_handback/` (NOTES.md first).
- **Headers first:** `HEADERS.tsv` lists them, each an add-only opt-in.
  - The emitted `??_7` probe matches retail (`PROBES.tsv`).
  - `hdrcheck2` prints default=same.
  - equiv_check plus the planted control: `EQUIV_CHECK.tsv`, `EQUIV_CONTROL.tsv`.
- **Rows:** each moves from its flattened self to the RTTI-placed PDB member. `land_scripted.py --release-t3 nf324` checked staleness
  and rechecked every row on the landed tree, and each graded symbol is the PDB symbol. Their nf324 T3 lines are closed (`T3_CLOSED.tsv`).
- **Includers:** the transitive includer closure is the agent's 83 rows plus the direct includers of every NF330 header. On the tree
  with this batch landed, 99 rows recheck MATCH / RELOCATION_MATCH PASS (`logs/includer_recheck.log`).
- **Classes:** def classes (CAIScratchpadDef, CActionUseDef, CBonusItemDef, ...) on their RTTI bases.
