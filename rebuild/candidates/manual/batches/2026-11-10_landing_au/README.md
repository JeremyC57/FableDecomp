# 2026-11-10 landing au -- chunk 1016 (6 rows)

Source: `pending/2026-11-10_linux_hand1016/`. vland --land 6 RELOCATION_MATCH, tests PASS:
- 0x00663990 ?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z (a static member, not the catalogued free function;
  same-TU GetSubDef<T> body keeps retail's bare release).
- 0x006A16A0 ?RemoveThingInCarrySlot@CTCCarrying@@QAEPAVCThing@@J_N@Z (returns the removed thing; the bool is unused in retail).
- 0x006D8790 ?AddSuccessfulStrikeOfType@CTCHeroExperience@@AAEXW4EDamageAttribute@@M_N@Z, 124 bytes (the catalog row ran 21 bytes into
  0x006D8810 ?AddSuccessfulWillStrike@CTCHeroExperience@@QAEXM@Z; SPLITS.tsv / BOUNDS_FIX.tsv).
- 0x0085FA00 ?IsToEvadeAllHits@NCreatureMode@@YI_NABVCThing@@@Z, 0x00B67E30 CEngineLandscapeRenderer::RegisterSubPatchForZSorting,
  0x00B84470 CEngineSubPrimitiveRenderer ctor (retail CEngineComponent base; private FableWin GetList(type) keeps the store order).
Headers (3-way merged onto HEAD, base aa0f44807; CEngineSubPrimitiveRenderer.h against chunk 1014's overlay, which the agent started
from; hdrcheck2 default=same, 17 files). CThing.h DRAWABLE slot 26 confirmed by the coordinator: vtable 0x01239ABC slot 26 =
0x004CD1D0 (`ret 4`) and FableWin.pdb ?DrawUpdate@CThing@@UAEXPBVCInterpolationInfo@@@Z. Descriptive name: SubPrimitiveRendererScratchTable
(0x014378C8, NOTES).
Parked (differ/): 0x005EABC0 PositionMesh DIFFER(495v511), 0x009F18F0 CGameEventDataPackage::GetData (register permutation), 0x006668A0,
0x00BCF4F0, 0x0074A4E0, 0x00A55F90, 0x00934E50 (identity settled: the IsOfType / IsNot / In2DRange instance). 0x006CBAC0 not attempted
(header debt in NOTES). Queued land1011as: 15 binding respellings.
