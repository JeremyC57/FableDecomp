# Landing nf339 b1 / b2 (Account A, 2026-11-24): T3 "self respell" lines: 6 rows changed, 138 stale lines closed

Agent NF339 re-read the 227 T3_PORT_QUEUE lines whose unit is "self respell". Hand-back: `pending/2026-11-24_nf339_handback/` (NOTES.md
first); tools are in `naming_batch/agent_tools/nf339/`.

STATUS: 138 done-already (stale), 5 done (b1), 1 done (b2), 60 skipped-port, 8 skipped-held, 15 rejected, 1 not attempted.
- **b1 (5 rows, no header change):** landed with `--release-t3 nf334,binders1`; 4 T3 lines are closed (`T3_CLOSED.tsv`).
  - 0x00AF59A0: `DestroyDecals` becomes `DeleteAllDecals`.
  - 0x0069A920: `PopulateSwitchList` / `PopulateListFromData` become the private `InitManualCameraList` / `InitSpecialCameraList`.
  - 0x004D297B: the constructor call moves from `CThing*` to `CThing&`.
  - Unit 0x008C63C0 + 0x008C6430: `WhackGround` goes from QAE to the private AAE, with `friend class <derived>` in each local base model (NF305 Q5).
  - Per row: HEAD v new at one path, sections identical under the rename, symbol sets equal, graded symbol unchanged, RELOCATION_MATCH PASS.
  - Table: 5 deletes; each old spelling's only witness was a moved row, and a census found no other landed reference.
  - Two test mirrors were respelled.
- **b2 (1 row):** 0x008800C0 `CCreatureAction_StabbedToDeath::Clone` now uses the landed opt-in class model (NF322, 0x0087E830 / 0x0087FE50).
  - cloneproof: the Clone COMDAT is identical under the rename, the emitted vftable equals retail 0x01277B74 (70 / 70 slots), the deleting
    destructor equals retail, and libcp is the only `.drectve` change (NF318 Q1).
  - Landed with `--release-t3 access3,nf317,nf320,nf323`; all four of its lines close (D6).
- **Stale lines (D1, accepted):** 138 self-respell lines are closed without a landing (`T3_CLOSED_STALE.tsv`). Later landings had already
  fixed those rows (nf317 / nf318 / nf321 / nf323 / nf326 / nf327, none run with `--release-t3`). For the 135 self lines, the agent compiled
  each row at HEAD and showed that it defines and grades the PDB symbol. For the 3 binder lines, the binder objects bind the PDB spelling.
  Other-tag lines for the same addresses stay open; three of those addresses are still open in the tracker for other reasons, among them a
  binder follow-up at 0x009F5E10.
- **D2:** `land_scripted.py --release-t3` now releases an address with several queue lines when EVERY line naming it has a released tag and
  the address in column 2. All those lines close with the row, which is how D6 closed.
- **D3:** 0x008F1720 EndRound lands only with the nf288 unit that names it, not alone.
- **D4:** the three `_PASS` alias rows wait for the NF322 Q1 header-unit route (guard widening). They stay on their T3 lines.
- **D5, confirmed:** where the return width retail uses wins over the PDB return type, the line is rejected (NF317 Q4), not skipped-port.
- **Note:** `rebuild/oracles/self_symbols.tsv` (03:05) is out of date; it still shows the old symbol for 111 of the stale rows. Regenerate it
  before anyone trusts it.
