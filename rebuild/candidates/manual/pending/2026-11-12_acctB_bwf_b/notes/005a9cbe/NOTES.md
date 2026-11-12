## 0x005A9CBE CTCInventoryTrade::ConstructTradeDescriptionWeapon -- RELOCATION_MATCH (bwf1 author, 2026-11-12)
- Made it match: (1) full draft from OUTLINE on the shared headers (0x005C90FC macro block + same-TU noinline GetDef/GetSubDef bodies) graded
  4799v4714; (2) brief form as the if-arm of an if/else instead of an early return (the return duplicated the dtor tail: one extra ReleaseData
  and ~CWideString; also fixed the esi/edi colouring of mgr / zero) -> 4711v4714, all 279 calls equal, only frame slots off; (3) ONE
  `NUISystem::CText* line` local reused for every text line (retail keeps it in ebx / [ebp-20h]); one named local per line shifted the
  whole frame (GetTC key -40 v -3C, the loop's PC/UI temps cross-jumped) -> RELOCATION_MATCH. Strict-headers regrade: RELOCATION_MATCH.
- Headers / flags: optimize("y",off)+("s",on), no /EHsc. Overlay hdr/: CTCInventoryTrade.h FABLE_RETAIL_CTCINVENTORYTRADE_TRADEDESCRIPTION
  (public decl), CTCShop.h FABLE_RETAIL_CTCSHOP_STOCKQUERIES (GetQuantity / GetMaxStock / GetDaysTillRestock). Uses
  FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION for CWideString::operator!=(const CWideString&) (0x0099B0F0; oddly named existing opt-in).
- Names proven wrong: the DOSSIER's "ConvertToCharString" is ?ToCharString@CWideString@@QBE?AVCCharString@@XZ (Ego_r 0x00B02850,
  FableWin 0x02F67B80; fable_string.h already spells it ToCharString). 0x007726B0 is ?GetQuantity@CTCShop@@QBEJJ@Z (Ego_r 0x0077F780,
  FableWin 0x0261E1F0, next to GetMaxStock / GetDaysTillRestock), landed as "CTCShop::GetStockQuantity". Parameter names descriptive
  (selling = arg3, brief = arg4; not in the PDB).
