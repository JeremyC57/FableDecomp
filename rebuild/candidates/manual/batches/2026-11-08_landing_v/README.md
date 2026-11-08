# Landing 2026-11-08v: chunk 867 (PC UI / inventory rows), 4 of 10

All four RELOCATION_MATCH + PASS. Hand-back: `pending/2026-11-08_linux_hand867/` (header table in `hdr_README.md`).

| Row | Notes |
|---|---|
| 0x00427216 NUISystem::CManager::CreateTickSlider | the PC tick-slider bar; payload name carried a comment tail, catalog name and file names corrected at landing |
| 0x005A8C66 CTCInventoryTrade::ConstructActualSpecialInventoryPC | |
| 0x005AF64E CTCInventoryTrade::BuyItem | opt-in FABLE_RETAIL_CTCHERO_TITLE; vland RELOC_CONFLICT GetMainPlayer 0x00449970 vs 0x00449960 in landed 0x007B0880 queued for review |
| 0x005D6748 CTCInventoryStats::ConstructExperienceList | second pass (`payload_second.json`): its test used C++11 `Colour{...}` braces, which VC7.1 rejects (TCC_FAIL); rewritten with a MakeColour helper (g++ and VC7.1 PASS) |

- Headers: ~31 shared headers gain additive `FABLE_RETAIL_*` opt-ins and 12 new headers land (CExpressionDef, CThingObjectDef,
  NGameText_CDataBank, NUISystem_CAction / CActionParamState / CActionParamULONG / CText, CTCActionUseBase, CTCBattleCharge,
  CTCTavernGameMaster, NPlayerGui_CDrawHighlightingTargetingCursor, stlport/fable_algorithm). hdrcheck: default path unchanged for
  every header after the coordinator moved CTCHero.h's two new includes inside its opt-in.
- Parked (4): 0x00590F0B (714 lines, 8 grades), 0x005B01FE SellItemPC (10 lines: bool materialisation + money-sum operand rank),
  0x005F59C7 (327 lines, text-bank local's candidate status), 0x0064BFBA CDrawHighlightingTargetingCursor::Draw (20 bytes of frame
  slots: c2 shares two temp slots where retail has three). Untouched: 0x0048E78E, 0x0064ED4F.
- Names: labels `ghidra_out/labels_landing_2026-11-08v.tsv`; ten respellings queued (land0811v).
