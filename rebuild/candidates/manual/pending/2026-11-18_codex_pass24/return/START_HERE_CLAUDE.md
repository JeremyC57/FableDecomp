# Start here for Claude — Ego_d pass 24

Review the 28 verified functions first. They cover 21,463 of 97,034 assigned retail bytes (22.12%). The other 72 rows are parked with their selected sources and passing behavioral fixtures. Every assigned address is present. All 100 final translation units compiled with MSVC 7.1 RTM, compiler version 13.10.3077. All 100 final fixtures built and ran under both VC7.1 and 64-bit GNU C++03.

| Pool | Assigned | Verified | Retail bytes verified | Retail bytes assigned |
| --- | ---: | ---: | ---: | ---: |
| egod_open | 68 | 22 | 18,315 | 77,827 |
| egod_parked | 32 | 6 | 3,148 | 19,207 |

## Verify and land

1. Check SHA256SUMS from the extracted directory. Apply the contextual changes in hdr/PATCHES.json to a local copy of the package headers as described in hdr/README.md. The full snapshots give an independent comparison.
2. Use STATUS.tsv as the ledger. For each verified row compile the exact selected source with cl 13.10.3077, base flags /O2 /Oy /W3 and the per-row extras. Confirm the exact COFF leaf in LABELS.tsv using objdump -t.
3. Use the original PE with SHA-1 39fd416ba23a91a89713ec80c789f57ddb22e2c8 and the assigned extent. Compare with only actual COFF relocation operands masked. Independently check the resolved symbols, addresses, addends and literal sizes. The 28 verified rows have 701 audited relocation sites. Replaying the bound values reproduced each entire retail slice. The resolved SHA-256 per verified leaf is in VERIFICATION.json.
4. Build each standalone test under VC7.1 /Od /W3 /GR- without /EHsc and under g++ -std=c++03. Each executable must exit zero and print its lowercase address followed by _TEST PASS. Tests use fixtures for dependencies. The row notes document coverage and any modeled helpers.
5. Review NAME_FIXES.tsv before binding callers. LABELS.tsv identifies the leaf actually emitted for each submitted source. BOUNDS_FIX.tsv has no entries because no assigned extent was changed. Keep the 72 parked rows in review. A passing behavioral fixture or equal function length alone cannot establish a retail match.

## Continue the parked rows

PARKED.md orders all 72 by the number of strict instruction difference hunks. Each row directory contains the closest selected submission under the current header and naming rules, its source SHA-256, tests, PROBES.md and NAMING_EVIDENCE.md. FINAL_OPERANDS.md records instruction-aligned relocation candidates for parked rows. Manual corroboration in RELOCATIONS.md or NAMING_EVIDENCE.md takes precedence over an uncertain automatic pairing. The six closest rows by this instruction metric are 0066a9e0, 00ae3c30, 00b0d360, 006e89b0, 00c52c40, 00f3abf0.

The `attempts` column records the row author's source-strategy count. GRADE_HISTORY.tsv adds all 2,976 recorded compiler comparisons across the batch, including older candidates, failed builds and unchanged verification repeats. Historical masked-equality entries are provisional. Only STATUS.tsv and the final reconstruction receipts carry the verified disposition. Some corrected shared-header drafts supersede closer historical draft sizes because the historical code used the wrong class, call binding or behavior.

This handback contains source and review evidence. Run the full repository neighbour audit again on the landing checkout, particularly if any shared headers have moved since the input package.
