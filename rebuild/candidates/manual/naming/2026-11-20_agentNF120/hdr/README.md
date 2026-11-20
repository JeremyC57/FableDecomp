# NF120 overlay headers

## engine/CTCFishing.h
- Copy of rebuild/include/engine/CTCFishing.h (base copy in orig/engine/CTCFishing.h).
- Adds one `#ifdef` opt-in inside the FABLE_RETAIL_CTCFISHING_PASS22 class, after the FABLE_RETAIL_CTCFISHING_DRAW_GETTERS block:
  `FABLE_RETAIL_CTCFISHING_CANCELBUTTONPRESSED` declares `void CancelButtonPressed();` (public, 0x007B0CC0) and the protected
  `void FinishFishing(bool caught);` (0x007B0950), both PDBs. Declarations only, no layout change.
- Without the macro the header is token-identical to the repo copy (the whole addition is inside the `#ifdef`).
- pin_asserts.py: `engine/CTCFishing.h pinned 0` (the FABLE_STATIC_ASSERT_AT lines keep their tags; nothing moved).
- Landed includers (grep of rebuild/src/compiled): 0x00649E8B NPlayerGui::CDrawFishingGame::Draw and 0x007EFF80
  CTCCarriedActionUseFishingRod::IsThingAbleToFish. Neither defines the new macro, so the opt-in changes none of them.
- Used by rows/007b0cc0.cpp. NF118 / NF119 overlays have no CTCFishing.h (checked).

## engine/CTCCreatureModeManager.h
- Copy of rebuild/include/engine/CTCCreatureModeManager.h (base copy in orig/engine/).
- Adds one `#ifdef` opt-in after the FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER block (public section):
  `FABLE_RETAIL_CTCCREATUREMODEMANAGER_REMOVEALLMODES` declares `void RemoveAllModes();` (0x0073AE70, Ego_r.pdb / FableWin.pdb
  ?RemoveAllModes@CTCCreatureModeManager@@QAEXXZ). Declaration only, no layout change.
- Without the macro the header is token-identical to the repo copy. The FABLE_STATIC_ASSERT_AT(106, ...) line keeps its explicit tag
  (it now sits 5 lines lower; the tag, not the line, is what the macro uses).
- pin_asserts.py: `engine/CTCCreatureModeManager.h pinned 0`.
- Landed includers: 157 rows (grep -rlE 'engine/CTCCreatureModeManager.h' rebuild/src/compiled). None defines the new macro (grep), so
  the opt-in changes none of them.
- Used by rows/0073ae70.cpp and rows/00664370.cpp (and the staged work/0073e6a0_ready.cpp, which does not define it).

## engine/CThingCreatureBase.h
- Copy of rebuild/include/engine/CThingCreatureBase.h (base copy in orig/engine/).
- Adds one `#ifdef` opt-in right after the FABLE_RETAIL_CTHINGCREATUREBASE_ONDIE block (retail class head):
  `FABLE_RETAIL_CTHINGCREATUREBASE_ONKILL` declares `public: virtual void OnKill(); private:` (0x00664370, Ego_r.pdb / FableWin.pdb
  ?OnKill@CThingCreatureBase@@UAEXXZ, the CThing slot-12 override). Declaration only; an override of an existing slot, no layout change.
- Without the macro the header is token-identical to the repo copy.
- pin_asserts.py: `total pinned 0`.
- Landed includers: 544 rows (grep -rlE 'engine/CThingCreatureBase.h' rebuild/src/compiled). None defines the new macro, so the opt-in
  changes none of them.
- Used by rows/00664370.cpp.

NF118's overlay (engine/CAIBrain.h, CEnvironmentSpellEffectInterface.h, CTCSpecialAbilities.h) and NF119's (none) touch none of these
three headers (re-read before each addition).
