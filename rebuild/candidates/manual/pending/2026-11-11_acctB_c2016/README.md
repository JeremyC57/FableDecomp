# 2026-11-11_acctB_c2016 -- acctB hand-back, chunk 2016 (3 reserved rows: 2 matched, 1 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x004CAA00 CThing::ConstructFromLevelScript(const char*) | RELOCATION_MATCH 1730v1730, test PASS | 4 grades. Retail builds a second CStringParser nothing reads; the source keeps it as `unused_parser` |
| 0x0061AD60 CTCInventoryQuests::DrawActiveTestQuests | RELOCATION_MATCH 1631v1631, test PASS | 5 grades. The landed GetTestQuestsToDisplay 0x0061A8A0 body is visible (not inlined) in the TU; the row must include `stlport/fable_containers.h` first (overlay opt-in STLP_DESTROY_AUX_FASTCALL) |
| 0x007D6870 CTCTavernGameCardPairs::ProcessInput (catalog `vfunc_28`; vtable 0x0126F6FC slot 28) | parked, DIFFER 1778v1778, 540 of 571 instructions equal (frame offsets masked) | frame 0x30 vs retail 0x38 (the |dy| copy and `dy` get their own words at [esp+10h] / [esp+24h] in retail); x87 operand order in the inline GetRight 0x007D6BFC, 0x007D6C15..1D, the first Dot 0x007D6C3D..4D and the final Dot 0x007D6ED1..DB; eax/edx swapped in two inline releases 0x007D6F0B / 0x007D6F53. 14 grades (12 in pass 1: two over the pass budget; pass 3 unused); no test yet; `rows/parked/` |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT; four label-only RELOC_NAME warnings on
0x004CAA00, in NAME_FIXES.tsv). Purity and padded-struct greps are clean.

To land: the rows need `hdr/` (`hdr/README.md`). CThing.h / CThingManager.h started from c2008's overlay, not HEAD: merge with
c2008. CInventoryDef.h gains a FULL opt-in (374 more members). CCard.h / CTCTavernGameCardBase.h / CTCTavernGameCardPairs.h
are needed only by the parked row. Rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv:
7 lines; LESSONS.md suggests re-authoring the landed stand-in GetTestQuestsToDisplay 0x0061A8A0 from the body in
rows/0061ad60.cpp. Descriptive names (NOTES.md): `g_bResetUIDsOnLoad` 0x013B8636, TEST_QUEST_GREEN / YELLOW / RED
0x0137D70C..14, static `flash` 0x013B8C84; NInventory::WHITE / GREY / BLACK matched to 0x0137D700..08 by value and order only.
