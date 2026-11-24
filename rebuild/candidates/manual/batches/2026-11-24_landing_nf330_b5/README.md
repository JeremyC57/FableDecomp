# Landing nf330 b5 (Account A, 2026-11-24): 4 thin-family flat-header rows on 3 new or extended opt-in class models

Agent NF330 took the NF324 Q1 remainder: the 95 rows NF329 did not reach. It ran NF329's route; the rulings are in
`batches/2026-11-24_landing_nf329_b3/README.md`. Hand-back: `pending/2026-11-24_nf330_handback/` (NOTES.md first).
- **Headers first:** `HEADERS.tsv` lists them, each an add-only opt-in.
  - The emitted `??_7` probe matches retail (`PROBES.tsv`).
  - `hdrcheck2` prints default=same.
  - equiv_check plus the planted control: `EQUIV_CHECK.tsv`, `EQUIV_CONTROL.tsv`.
- **Rows:** each moves from its flattened self to the RTTI-placed PDB member. `land_scripted.py --release-t3 nf324` checked staleness
  and rechecked every row on the landed tree, and each graded symbol is the PDB symbol. Their nf324 T3 lines are closed (`T3_CLOSED.tsv`).
- **Includers:** the transitive includer closure is the agent's 83 rows plus the direct includers of every NF330 header. On the tree
  with this batch landed, 155 rows recheck MATCH / RELOCATION_MATCH PASS (`logs/includer_recheck.log`).
- **D1 (accepted for b5):** CAIStateGroup_Gaze.h gains a `FABLE_RETAIL_CAISTATEGROUP_GAZE_RTTI_DECLS` block inside the landed base model, which the b5 rows define (the NF324 b5 / b6 form; NF318 Q2 is about unproven switches). The base's own emitted `??_7` matches retail on all 16 slots (`PROBES.tsv`).
- **b6 is held.** The StrafeForward / StrafeBackward base blocks prove slots 12 and 73, but their own `??_7` differs at slot 11 (Clone). NF330's alt adds the PDB Clone declaration; it needs a regrade of the two b6 rows first. The Clone stand-in rows 0x008C0270 and 0x008C02A0 and the CBankFile 21-against-20-slot finding are queued as T3 `nf330`.
