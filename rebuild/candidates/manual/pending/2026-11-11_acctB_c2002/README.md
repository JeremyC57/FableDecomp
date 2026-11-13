# 2026-11-11_acctB_c2002 -- acctB hand-back, chunk 2002 (4 reserved rows)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007B90F0 CTCTavernTable::OnInitialActivate | RELOCATION_MATCH 1257v1257, test PASS (g++ and VC7.1) | `rows/007b90f0.cpp` |
| 0x007D8570 CTCTavernGameSpotTheAddition::ProcessInput | RELOCATION_MATCH 1340v1340, test PASS (g++ and VC7.1) | `rows/007d8570.cpp`; jump table 0x007D8A9C inside the row |
| 0x006224C0 CTCInventoryQuests::OpenInventoryPC | parked, DIFFER 1335v1338 | `rows/parked/`; pair-landing case with 0x0061E6D0 (see STATUS.tsv) |
| 0x0074BC30 "CTCCreatureGenerator::DrawDebugInfo" | SKIP | reserved address is a fragment: the function starts at 0x0074BC20 (SPLITS.tsv) |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/grades.txt`, `tests_gxx.txt`,
`tests_vc.txt`, `vland.log` (read-only vland: both rows RELOCATION_MATCH + PASS; two RELOC_NAME label warnings on
0x00A0D390 and 0x00687D00, both in NAME_FIXES.tsv). Purity grep (`__asm`, `naked`, `_emit`, `register`, `volatile`,
bare `__forceinline`, `wchar_t`) and the padded-struct grep are clean on rows and tests.

To land: the rows need the headers in `hdr/` (all changes to existing headers are opt-in branches; `hdr/README.md`).
`payload.json` holds the two matched rows; rebuild the oracle from Fable.exe (no retail bytes are in this folder).
Layout: `rows/<addr>.cpp` + `rows/<addr>.test.cpp`, `rows/parked/`, `hdr/`, STATUS.tsv, NAME_FIXES.tsv, LABELS.tsv,
LESSONS.md, NOTES.md, SPLITS.tsv.

## Lane note for the coordinator
`tools/decomp_pipeline/linux_lane/setup_vc71_wine.sh` under git 2.43: `git checkout HEAD -- .` ignores the sparse-checkout
list and starts fetching the whole WinDDK tree blob by blob. The files the script needs arrive first; stopping the checkout
and re-running the script (it skips the clone once `bin/x86/cl.exe` exists) completes the lane.
