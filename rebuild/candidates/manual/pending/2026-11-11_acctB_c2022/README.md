# 2026-11-11_acctB_c2022 -- acctB hand-back, chunk 2022 (2 reserved rows: 1 matched, 1 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007D3820 CTCTavernGameCoinGolf::ProcessInput (catalog `vfunc_28`; vtable 0x0126F49C slot 28) | RELOCATION_MATCH 1883v1883, test PASS (g++ and VC7.1) | `// Flags: /d2inlT87` + `// Inline-budget:` line: the XAngle clamp's std::max / min site scores 86 against 74 (static frequency 1/32, cost 16); 87 is the smallest value that expands it and no other decision in the TU changes -- review at landing. The TU also defines the landed GetCamera 0x007D3560 and GetSubDef<CTavernGameCoinGolfDef> 0x007D4BD0: grade only the ProcessInput leaf |
| 0x007F26B0 CTCLightningOrb::Initialise | parked, DIFFER 1793v1802 | all 512 instructions equal with frame offsets masked; only the frame differs (`sub esp,78h` vs retail 8Ch; retail slot map in STATUS.tsv / NOTES.md). 3 grades only (the agent's context ran out); source + passing test in `rows/parked/` |

Coordinator re-verification of the matched row (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade,
both test runs, read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT; label-only RELOC_NAME on 0x00687D00 and
0x00A0D390, RELOC_ALIAS on __CIfmod). Purity and padded-struct greps are clean.

To land: the row needs `hdr/` (5 new headers; opt-ins in 5 copied repo headers; the c2007 overlay files unchanged;
`hdr/README.md`): land c2002's and c2007's headers first. Rebuild the oracle from Fable.exe (no retail bytes are in this
folder). NAME_FIXES.tsv: 12 entries (e.g. the three CTCLightningNode setters 0x007F39A0 / 0x007F39F0 / 0x007F3A40 landed as
`unattributed AssignTarget_*`; 0x007F23D0 is CTCLightningOrb::OnInitialActivate, landed as LoadDefAndFlagAppearance).
