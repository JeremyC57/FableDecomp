# 2026-11-10 landing q -- the PC clothing screen (agentCLO), 1 row

Source: `pending/2026-11-10_linux_handCLO/`. vland --land: RELOCATION_MATCH, test PASS (VC7.1 /Od and g++ -std=c++03).

- 0x005B8743 CTCInventoryClothing::ConstructScreenPC (3,838 bytes; agentSCR's parked skeleton finished). Levers: optimize("s"); same-TU
  GetSubDef<T> / GetDef<T> bodies under auto_inline(off); the ConstructClothingListPC call written in both arms of the worn test; the
  STLport `_List_base(const allocator&)` body visible (allocator temporary in the parent argument slot); `partLists[i][j]` read directly.
- Headers (opt-in, default paths unchanged, hdrcheck2 default=same): engine/CTCInventoryClothing.h FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN
  (NHeroInformationScreens::CBase at +0x130, the six screen methods with PDB access, ClothingItemMap +0x160, ClothingList +0x16C);
  engine/CHeroSuitDef.h FABLE_RETAIL_CHEROSUITDEF_STATGETTYPENAME; engine/CAppearanceModifierDef.h
  FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME.
- Helpers named (labels_landing_2026-11-10q.tsv): CreateMenuDescPC 0x005B3613, InitialiseMannequin 0x005B37F7, SetUpSelectedClothes
  0x005B4795, ConstructTitleString 0x005B6881, ConstructClothingListPC 0x005B6A66 (unlanded), map<EClothingSuitPart,bool> pieces,
  vector<vector<pair<long,long> > > push_back / dtor, CCountedPointer ReleaseData / ShareData instances.
- Test fix at landing: std::snprintf (not in VC7.1) -> sprintf, and the dimming check's colours moved off the x87 rounding boundary
  (200 * 0.9f truncates to 179 under VC7.1 /Od, 180 under SSE).

Queued (`land1011r`): 11 landed spellings the agent proved wrong (0x005BB090 / 0x005BC080 "CActiveFile_OnReadFinished", 0x005BB3D1,
0x005BC237, the CAction / CText / CMenuEntry CCountedPointer ReleaseData labels, 0x00412130, 0x005B37F7 / 0x005B4795 access).
Next: the console twin ConstructScreen 0x005B76D1 has the same body shape.
