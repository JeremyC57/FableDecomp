# 2026-11-10 landing ao -- chunk 1013b (1 row)

Source: `pending/2026-11-10_linux_hand1013b/`. vland --land RELOCATION_MATCH, test PASS:
- 0x00507650 ?LoadWorldFromBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z (catalog "CWorldMap::LoadFromFile"; manifest respelled), first
  grade. Levers: new shared engine/CTBaseSingleton.h + engine/CEngineManager.h (retail expands the allocating Get: PClass 0x013BA854,
  deleter 0x004361E0 ?DeleteData@?$CCountedPointer@VCEngineManager@@@@SIXPAX@Z), declarations under CRegion / CMapInfo LOADBINARY and
  CWorldMap LOADWORLDFROMBINARY, FABLE_RETAIL_CARRAY and STLport 4.6 fill / fill_n(char*) overloads (FABLE_RETAIL_STLP_FILL_CHAR) so
  vector<char>(n) expands as retail. Overlay 3-way merged onto HEAD (base bea1ff3e9); hdrcheck2 default=same.
- Parked (differ/): the CEngineMap peeks PeekThemeId / PeekThemeBlend / PeekLandscapeHeight (one block placement: the inline
  CEngineWorldMap::GetMap(x, y) negative-coordinate exits), PeekInterpolatedMapNormal (frame slots only), PeekInterpolatedWaterHeight
  (retail expands PeekLandscapeHeight inline and also keeps it out of line -- an inline-budget residue: owner ladder), GetThemeFaceNormalAt
  (one store order), HeightAreaChanged (register choice), CreateLineSegment, Find{Ground,Theme}CollisionPointWithinCell, LoadRegion,
  BuildStaticMapGenerationVisibilityInfo. Opt-ins CEngineMap PEEK, CEngineWorldMap GETMAPXY, CIDrawMap DRAW committed with this landing.
