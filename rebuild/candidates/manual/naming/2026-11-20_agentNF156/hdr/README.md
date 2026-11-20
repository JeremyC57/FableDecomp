# NF156 header overlay

## fable_ime.h
- Base: the repo working-tree rebuild/include/fable_ime.h (copy in orig/fable_ime.h, byte-equal at copy time).
- Added opt-in (add-only, declaration only): `FABLE_RETAIL_CIME_FINALISESTRING` inside the `FABLE_RETAIL_CIME` NUISystem::CIME class, right
  after the existing `FABLE_RETAIL_CIME_ISIMEACTIVE` block: `void FinaliseString(bool commit);` (Ego_r.pdb S_PUB32 + FableWin.pdb
  ?FinaliseString@CIME@NUISystem@@QAEX_N@Z, 0x00404B50). Used by rows/00851920.cpp.
- Default path token-identical: without the macro the preprocessed text is the repo header's (the block is the only addition).
- pin_asserts.py pinned 1: `FABLE_STATIC_ASSERT(sizeof(NUISystem::CIME) == 0x54)` -> `FABLE_STATIC_ASSERT_AT(94, ...)` (the line it had in the
  repo header, now line 99). The IMEUI_APPEARANCE assert (line 41) is above the insertion and did not move.
- No other agent overlays fable_ime.h (checked NF150..NF155, NF157, NF158 hdr/).
- Landed includers (grep -rlE 'fable_ime.h' rebuild/src/compiled): none of them defines FABLE_RETAIL_CIME_FINALISESTRING, so the overlay changes none of
  them: NUISystemCEditBox_KeyPressed_00540320.cpp, NUISystemCIME_LoadIMESettings_00405350.cpp, CIME_Initialise_00405650.cpp,
  CIME_LoadIMESettings_00405350.cpp, NUISystemCIME_Shutdown_004051c0.cpp, global_dynamicinitializerforgIME_01219e90.cpp.
