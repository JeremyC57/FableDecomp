# 2026-11-10 landing w -- chunk 1004, the weapons / clothing leftovers (49 rows)

Source: `pending/2026-11-10_linux_hand1004/`. vland --land: 47 MATCH + 2 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x005C90FC CTCInventoryWeapons::ConstructWeaponDescriptionContainer and 0x005CACB9 RefreshWeaponDescriptions (RELOCATION_MATCH):
  `if (IsLiveGUIMode()) X.Reset(Create("PC_..")); else X.Reset(Create("UI_.."));` (a ternary does not merge the arms), a nested `if` for
  the viewport cast, visible STLport map / erase bodies and CDeletion bodies. With these the CTCInventoryWeapons TU has 16 of 18 rows.
- Split of the 0x005B9641 row (BOUNDS_FIX.tsv): DrawPreparePrimitivesForRendering ends at 0x005B9BF0 (0x5AF bytes); 0x005B9BF0..0x005B9D8B
  are 47 contiguous leaf helpers with no caller, jump or address reference anywhere in Fable.exe, so their owners stay unproven: each is a
  `CUnproven<Kind>_<addr>` class shaped from its bytes (EXTRA_RULES), all MATCH. 47 manifest starts added.
- Headers (opt-in, default paths unchanged; CTCHeroExperience.h merged 3-way with chunk 1005's MODIFYLEVEL): new CTCMultiStrike.h,
  CTCMultiArrow.h, CCreatureAction_UnsheatheItemFromInventory.h, CHeroAbilityStatPackage.h; opt-ins CWIDESTRING_NULLSTRING,
  CPLAYERGUI_LIVEGUI, CTCHEROEXPERIENCE_GETPHEROCOMBATDEF, CHEROCOMBATDEF_HEAVYWEAPONS and others (hdr/README.md).

Parked (differ/): 0x005CA68E SetupActiveWeaponsOnCarrier DIFFER(1538v1535) (register choice: c2prio gives `this` ebx over the 0x48
constant); 0x005C71AA DIFFER(680v680) (two-argument setup order; over budget); 0x005B3E4E GlobalUpdate (needs animation headers);
0x005B9641 DrawPreparePrimitivesForRendering (needs CUIMiscThingsDef / CRightHandedSet / C3DOrientationInterpolator / morphed-mesh headers).
Queued `land1011x`: name fixes incl. 0x013BCA24 ?NullString@CWideString@@2V1@A (8 invented spellings) and 0x0042D5B1 map<ulong,CUIState>::operator[].
