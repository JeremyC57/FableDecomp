# 2026-11-19 landing nf96b: naming round 8, NF96 part 2 (27 rows + 1 rebind)

NF96 part 2 applied the advisor's same-TU strong-definition rule (`batches/2026-11-19_landing_nf96a/README.md`) and worked its follow-ups;
it stopped for context with 4 entries open (q11463, q11464, q11466, q11467; resume point in NOTES). Records:
`naming/2026-11-19_agentNF96_part2/` (selfcheck.log: 0 problems).

## Entries (part 2): 8 done, 3 done-already, 1 blocked, 4 open
- **q11204 / q11230 (done):** NF91's held owner port of GetCategoryIndexForTradeType 0x0059B6F4 lands (it still grades on the current
  headers); the four same-TU copies in 0x0059C6B0 / 0x0059C6DD / 0x0059C710 / 0x0059C747 meet the three conditions (PDB spelling, the
  owner's body verbatim, the Fidelity line). Body diffs: `logs/q11204_bodydiff.txt`.
- **GetCardDef (q11339 closure):** the owner 0x008F0B20 was itself a stand-in (non-const spelling on local models) and is now ported; the
  copies in 0x008F0B80 / 0BD0 / 0EB0 / 10B0 carry the owner body verbatim and the Fidelity line; 0x008F2280 (the defect: the PDB symbol with
  a stand-in body) is ported onto the shared headers and its own leaf is now the PDB `UAEXXZ`. Diffs: `logs/getcarddef_bodydiff.txt`.
- **done:** q11454..q11457 (Account B's CEngineTextureMorph vector claims, verified), q11458, q11462 (label-table fix, queued).
  **done-already:** q11453 (0x006CA950 already bound the right name; this landing's recheck rebinds it, so the stale `?AddMorph@CTextureMorphs`
  line is gone from reloc_bindings.tsv), q11459, q11461. **blocked:** q11460 (0x006EDA80: its local CThingCreatureBase does not derive from
  CThing, a header port).

## Rows (27): 7 self renames (RENAMES_applied.tsv, 4 file moves) + 20 respellings / Fidelity-line copies
Recheck from the repo after landing (`logs/recheck.log`, 28 rows with 0x006CA950): RELOCATION_MATCH / MATCH, every test PASS. One
`RELOC_CONFLICT _mult_to_stay_off_table` warning on 0x008F0BD0: a TU-local table (retail has one copy per TU, 0x0138927C and 0x01383C98;
the name is in no repo header or source), the same class as the documented `_$E2` warning. Two rows changed again after earlier round-8
landings (0x0059C747: the Fidelity line only; 0x008F0B80: the owner body verbatim); both diffs checked against the repo files.

## Headers: NInventory_CTCInventoryBase.h `FABLE_RETAIL_NINVENTORY_CITEM_REMOVEITEM` (token-identical without it; 0 asserts moved).
Includer recheck (84 rows: its includers + catalog-only renames): 84 of 84 good. `logs/includer_recheck.txt`.

## Checks
Synced with FullDecomp-B first (B's hand3c WatchForBooty landing, its CMatrix4x4 batch snapshot). `naming/rebind.py`: removed 54, added 48;
`bindings_multi_check.py` at baseline; funclets 1,355 checked, 0 broken; selfcheck 0 problems.

## Rule slips (NOTES): a released lock briefly overwritten and restored; two released locks taken by overwrite, not mkdir; read-only calls
without env.sh; two scratch files outside the workspace (deleted); one LINK_FAIL before a test's old symbol was respelled.
