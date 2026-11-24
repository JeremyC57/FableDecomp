# Landing nf330 b6 (Account A, 2026-11-24): 2 thin-family flat-header rows on 4 new or extended opt-in class models

Agent NF330 took the NF324 Q1 remainder: the 95 rows NF329 did not reach. It ran NF329's route; the rulings are in
`batches/2026-11-24_landing_nf329_b3/README.md`. Hand-back: `pending/2026-11-24_nf330_handback/` (NOTES.md first).
- **Headers first:** `HEADERS.tsv` lists them, each an add-only opt-in.
  - The emitted `??_7` probe matches retail (`PROBES.tsv`).
  - `hdrcheck2` prints default=same.
  - equiv_check plus the planted control: `EQUIV_CHECK.tsv`, `EQUIV_CONTROL.tsv`.
- **Rows:** each moves from its flattened self to the RTTI-placed PDB member. `land_scripted.py --release-t3 nf324` checked staleness
  and rechecked every row on the landed tree, and each graded symbol is the PDB symbol. Their nf324 T3 lines are closed (`T3_CLOSED.tsv`).
- **Includers:** the transitive includer closure is the agent's 83 rows plus the direct includers of every NF330 header. On the tree
  with this batch landed, 157 rows recheck MATCH / RELOCATION_MATCH PASS (`logs/includer_recheck.log`).
- **D1 for the Strafe bases:** at first, StrafeForward's and StrafeBackward's own `??_7` differed from retail at slot 11 (Clone), so b6 was held. NF330 then added the PDB `Clone` declaration (`?Clone@<class>@@UBEPAVCCreatureActionBase@@XZ`, Ego_r + FableWin, RTTI slot 11) to each `_RTTI_DECLS` block.
  - The base's own `??_7` now matches retail: 81 of 81 (Forward), and 80 of 81 plus the slot-0 `??_E` / `??_G` alias (Backward).
  - Each block's `// Needs` line names the full CCombatActionBase branch it requires; on the short model, IsToFaceTarget would land at slot 70.
  - The two rows were regraded on the final headers (MATCH, + libcp), and equiv_check plus the planted control were rerun.
- **NF330 in total:** 81 of 95 rows landed on 55 headers. The 14 skipped-port rows keep their nf324 lines. The landed Clone stand-ins 0x008C0270 / 0x008C02A0 and the CBankFile slot finding are queued as T3 `nf330`.
