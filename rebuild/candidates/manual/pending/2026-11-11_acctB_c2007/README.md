# 2026-11-11_acctB_c2007 -- acctB hand-back, chunk 2007 (4 reserved rows, all matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007CF9A0 CTCTavernGameShoveHaPenny::ProcessInput | RELOCATION_MATCH 1434v1434, test PASS | `// Flags: /d2inlT116` + `// Inline-budget:` line: the third std::max<float> site 0x007CFB45 scores 115 against the default 74; T115 still calls it, T116 expands it; no other inline decision in the TU changes -- review at landing |
| 0x006DEEE0 CTCTrophy::GlobalUpdate(bool) (catalog `vfunc_26`; vtable 0x0125FC54 slot 26) | RELOCATION_MATCH 1437v1437, test PASS | uses `FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION` (as landed 0x008A8C60) so the call goes to GetString 0x00415DD0 as in retail |
| 0x006DE8C0 CTCTrophy::OnShowTrophyPerformAction | RELOCATION_MATCH 1460v1460, test PASS | 10 grades (full first pass) |
| 0x00665860 CThingCreatureBase::UpdateAnimation(float) | RELOCATION_MATCH 1405v1405, test PASS | true length 1405 (the 1416 estimate ran into the forwarder at 0x00665DE0) |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 4 wins, no RELOC_IDENTITY; seven RELOC_NAME label warnings, each covered in
NAME_FIXES.tsv / NOTES.md). Purity and padded-struct greps are clean.

To land: the rows need `hdr/` (7 new headers; additive opt-ins in 14 existing headers; `hdr/README.md`). The overlay
includes unchanged copies of chunk 2002's CTCTavernGame.h, CRightHandedSet.h and CInputProcessTavernGame.h
(`2026-11-11_acctB_c2002/hdr`): land or merge c2002's headers first. Open point (also in c2002): the tavern games reach
protected input-process members through a using-declaration; the original access route is unproven. Rebuild the oracle from
Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 19 landed or bound spellings proven wrong (e.g. 0x008302D0
landed as `?Run@T@@QAEXXZ` is CThingAICreature::SetStateGroupEnabled; landed 0x008EF910 UpdateCoinSounds takes bool).
Descriptive global names are listed in NOTES.md.
