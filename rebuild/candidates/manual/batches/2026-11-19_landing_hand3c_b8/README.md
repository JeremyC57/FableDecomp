# Landing hand3c b8 (2026-11-19): CEngineScene::GetMapAt (0x00B94BF0), CAIStateGroup_BirdStartled::IsRunnable (0x00914650)

vland --land, strict headers: RELOCATION_MATCH + PASS for both (2 grades each). All 23 landed users of the changed headers rechecked.
- GetMapAt: FableWin 0x02E1D7D0 shape -- GFFloatToLongFloor x2 (shared retail_asm helper), MapList[y][x], a new CEngineSceneMap over the cell box
  when the slot is null. FABLE_RETAIL_STLP_MAP keeps the subscripts out of line with visible bodies (without it 311v311 frame slots).
  Opt-ins: CEngineScene.h GETMAPAT (MapList typed as Ego_r's nested map; declaration), CEngineSceneMap.h CTOR (CBaseClassNonCopyable base per
  RTTI; inline ctor calling Initialise 0x00B949A0).
- IsRunnable: FableWin 0x02B8D570 shape with Ego_d pnav / area / pcreature; keeps the original's bug of measuring the bird against itself (FableWin and
  retail both pass the creature's PeekPos() twice); DIST_CREATURES_SCARE_BIRDS 5.0f folded. FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY drops the
  `pcreature = 0` store as retail. NEW headers CAIStateGroup_BirdStartled.h, CThingFilter_ScaresBirds.h; CThingDeadCreature.h STATGETTYPE.
Name fixes (`NAME_FIXES.tsv`): 0x00B97C10 (descriptive `std_map_find_or_insert`) is the inner map's operator[]; 0x0129CBA4 is ?NAVIGATOR_TYPE_A_STAR@@3KB
(value 3) -- landed rows use three made-up names for it (g_NavigatorTypeDefault, g_groundWanderMode, g_flyingMode).
