# Chunk 509 (agent509, 2026-10-27/28): 17 of 20 single functions landed (256-512 bytes), 1 held

First chunk of single functions of 256-512 bytes (20 rows, address order, 0x00401067..0x00635D70). One Opus 5.5 agent at
high effort; a container restart stopped the first agent after 8 drafts, and the resumed agent regraded them.

- **Landed:** 17 rows with `vland.py --land` (all RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY):
  WinMainCRTStartup (0x00401067, /O1), CWorldMap::IsSectionToHaveNavigationMap, CPlayer::UninitModeUseProjectileWeapon,
  std::__introsort_loop<CTestQuest> and twelve more STLport instantiations, and others (NOTES.md).
- **Held (1, pending/2026-10-27_linux_hand509/held/):** 0x0062CF90 CWorld::EACreatureBlock matches (RELOCATION_MATCH,
  test PASS) but calls 0x0043DBD0 as `IS_THING_ALIVE<CThingPlayerCreature*>`, while landed 0x0064A568 binds that symbol
  to the byte-identical twin 0x0043DBF0 (marked unproven there). FableWin.pdb names at least ten pointer instantiations of
  IS_THING_ALIVE (CThing*, const CThing*, CThingPlayerCreature*, const CThingPlayerCreature*, CThingCreatureBase*, ...),
  so the retail twins near 0x0043DBD0 need an identity pass before either binding lands.
- **Parked (1, pending/.../differ/):** 0x004BC1A0 NGraphicAppearance::DrawRHSet (397v406): retail gives each of three
  CEnginePrimitiveLine temporaries its own 0x40 frame slot; VC7.1 shares one.
- **Skipped (1):** 0x005239CD is the middle of the real function at 0x005239C0 (379 bytes; reads ebx it never set);
  the catalog row needs re-cutting to 0x005239C0.
- **Identities:** 5 proven, 14 probable (the STLport shapes are exact; element types from stride and callers),
  1 descriptive.

Label corrections (NOTES.md): 0x0078BEF0 is CTCBerserk::CanBerserkBlock; 0x007331E0 / 0x007337E0 CTCCombat::IsBlocking /
SetBlocking; 0x00630DD0 std::_Destroy<CUIStateDef*>; 0x00401377 the CRT `_setargv` stub; 0x00430300
~CParentDefClassBase; 0x0043211D that vector's `_M_clear`. Review: STLport `_M_fill_insert` access is QAE in some landed
bindings and IAE in others; header debt for the component and camera classes in 0x0062CF90 / 0x00635D70 and CResource
in 0x00632FF0.
