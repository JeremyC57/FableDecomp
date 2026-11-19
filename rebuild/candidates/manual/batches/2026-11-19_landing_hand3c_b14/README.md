# Landing hand3c b14 (2026-11-19): three not-plain-C++ rows

All three rows are RELOCATION_MATCH + `<addr>_TEST PASS` under strict vland.

**0x007727E0 CTCShop::GetCurrentBasicPrice** (FableWin 0x0261FE90 shape)
- Closed by the existing STLP_TREE_CONSTFIND_NOINLINE opt-in: the map find body is visible but called out of line, so `def` stays in ebx.
  This TU's copy grades MATCH at 0x00777170.
- Finding (not acted on): the shared GETSUBDEF_BODY calls the public const GetPDefFromGlobalIndex 0x009AD9E0, but retail GetSubDef<CShopDef>
  0x005130A0 calls the protected non-const 0x009ADA10. 121 landed rows use that opt-in.

**0x006B75A0 CGameMap::Resize** (/Gr; FableWin 0x024A0160 shape, Ego_d names)
- New opt-in FABLE_RETAIL_CGAMEMAP_RESIZE: the Resize and CGameMapCell::Clear declarations, with the cell members private per the
  FableWin field list. That makes the class a non-aggregate, so vector(size_type)'s `_Tp()` writes nothing, as retail 0x006B7D60 does.
- Same-TU COMDATs 0x006B7E20, 0x006B7D60, 0x006B7CC0, 0x006B7C10, 0x006B7B70 and 0x006B79A0 all match from this file.

**0x007DF770 CTCWallMountEffects::FrameUpdate** (catalog vfunc_9; FableWin 0x02766430 shape)
- New engine/CTCWallMountEffects.h.
- New opt-in FABLE_RETAIL_CCREATUREACTION_USEWALLMOUNT_CLASS in CCreatureAction_UseWallMount.h; its three moved asserts were pinned at landing.
- vland warns RELOC_NAME on the ICF-shared dtor 0x007DF380. It is not a failure.

**Checks**
- Header-user recheck of CGameMap.h and CCreatureAction_UseWallMount.h: 7 of 7 good (`logs/recheck_header_users.txt`).
- NAME_FIXES: 0x006B7490 is ?Clear@CGameMapCell@@QAEXXZ (the landed 0x006B7AF0 comment calls it "Reset"); 0x007DF770 is
  ?FrameUpdate@CTCWallMountEffects@@UAEXXZ.
