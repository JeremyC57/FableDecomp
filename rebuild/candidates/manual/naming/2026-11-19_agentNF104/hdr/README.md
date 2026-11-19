# NF104 header overlay

One overlay header: `engine/CDrawMiniMap.h` (copy of `rebuild/include/engine/CDrawMiniMap.h`; untouched base in `orig/engine/CDrawMiniMap.h`).

## Opt-in
- `FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_UPDATEMARKERS` (nested inside the existing `FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS` block):
  declares `void UpdateMarkers(bool);` on `NPlayerGui::CDrawMiniMap` -- Ego_r.pdb `?UpdateMarkers@CDrawMiniMap@NPlayerGui@@QAEX_N@Z`,
  retail 0x00643E9A (`add ecx,28h ; jmp` 0x0082B7C0 = Ego_r `?UpdateMarkers@CMiniMapDisplay@@QAEX_N@Z`). Declaration only: no member,
  no layout change. Used by row 0x004378C0 (CPlayerGui::UpdateMiniMapMarkers, q4761).

## Assert pin
- `tools/decomp_pipeline/pin_asserts.py` pinned 1 assert: the class-block `FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawMiniMap) == 0x160);`
  (base line 93) is now `FABLE_STATIC_ASSERT_AT(93, ...)`, so its generated name is unchanged by the inserted lines. No assert moved or edited
  otherwise.

## Default path
- Without the new macro the preprocessed text is token-identical to the base (the only other change is the pinned assert, which keeps its
  base line number). With `..._CLASS` alone it is also identical.
- Landed includers of `engine/CDrawMiniMap.h` (`grep -rlE 'CDrawMiniMap.h' rebuild/src/compiled`), none of which define the new macro, so none
  change: 00/64 CDrawMiniMap_ClearPrimitivesIfNecessary_00643de5, NPlayerGuiCDrawMiniMap_Update_0064401c,
  NPlayerGuiCDrawMiniMap_CDrawMiniMap_0064e83f, NPlayerGuiCDrawMiniMap_ToggleCentering_0064e019,
  NPlayerGuiCDrawMiniMap_IsFinishedOpening_00644179; 00/43 CPlayerGui_IsMiniMapDrawing_00437870,
  CPlayerGui_DrawPreparePrimitivesForEngine_00439070.
