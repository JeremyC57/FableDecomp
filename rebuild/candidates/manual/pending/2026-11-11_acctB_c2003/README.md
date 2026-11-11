# 2026-11-11_acctB_c2003 -- acctB hand-back, chunk 2003 (4 reserved rows)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00E10E60 NScript::CQ_WaspBossScript::CWaspChaseWoman::Main | RELOCATION_MATCH 1306v1306, test PASS | 14 grades (two passes); `// Inline-budget:` line: a folded `&& true` on the route-update arm (GOTCHAS chunk H40f device), measured with the c2 inline trace -- review at landing |
| 0x0051F480 NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSectionInEntityScript | RELOCATION_MATCH 1270v1270, test PASS | |
| 0x00598463 NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile | RELOCATION_MATCH 1335v1335, test PASS | size TU; shared headers only |
| 0x00D2988B "CHookCoastTeleporter::Main" | SKIP | reserved address is a fragment: the function starts at 0x00D29880, 1319 bytes (SPLITS.tsv) |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 3 wins, no RELOC_IDENTITY; five RELOC_NAME label warnings on 0x00598463, explained in
NOTES.md / NAME_FIXES.tsv). Purity and padded-struct greps are clean (the word "register" appears only in prose comments; the
coordinator respelled one `wchar_t` in a comment of 00598463.cpp as `unsigned short`, comment only, row regraded after).

To land: the rows need the five header copies in `hdr/` (each adds one new opt-in branch; `hdr/README.md`). `payload.json`
holds the three matched rows; rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv lists the
landed spellings proven wrong; LESSONS.md "Review at landing" notes that the landed file
`rebuild/src/compiled/00/9b/CStringParser_FindKeyword_009b9ba0.cpp` holds an unrelated body.

## Fragments in the acctB reservation (coordinator scan, 2026-11-11)
Five reserved addresses lie inside an earlier function (previous catalog start + extent scan covers them); no compiled function
can match them, and the true starts are not in RESERVED.tsv, so acctB skips them:
0x0074BC30 (in 0x0074BC20), 0x00D2988B (in 0x00D29880), 0x00BD71CC (in 0x00BD71B0), 0x00BA115C (in 0x00BA1140),
0x00BC3F3D (in 0x00BC3F30).
