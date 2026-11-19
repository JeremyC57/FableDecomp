# Landing hand3c b12 (2026-11-19): 3 rows

vland --land, strict headers: RELOCATION_MATCH + PASS for all three.
- 0x0066CD50 CRegionMinimap::BuildRegionMinimapTGA: FableWin 0x021886B0 with Ego_d locals; the last register rotation closed on the inline getters' named local
  `long idx = size * pos.Y + pos.X;` (FableWin + Ego_d). Opt-ins: CRegionMinimap.h BUILDTGA, CSurface.h SETPIXEL_COLOUR, CRGBColour.h INTERPOLATE (256.0f float),
  C2DCoordI.h INITIALISE.
- 0x00B41A80 CEngineMap::PeekWaterHeight: FableWin's two statements on PEEK_BODIES; Flags /d2inlT576 (the same TU flag landed 0x00B40BD0 carries; without it
  DIFFER(68v424)). CEngineMap.h PEEKWATERHEIGHT declaration.
- 0x00699E60 CGameCameraManager::SetScriptedCameraMode: FableWin 0x021F9950 on the FULL header; private SwitchIntoScriptedControl inline (FableWin 0x022002F0, Ego_r
  expands it); NEW NCameraModes_CScriptedControl.h (0xE8, Ego_r members); CGameCameraManager.h SCRIPTEDCONTROL; existing CCOUNTEDPOINTER_RESET_NOINLINE (this source
  grades RELOCATION_MATCH at 0x0069C2F0 too); `// Inline-budget:` /d2inlT150 (smallest N; 149 fails).
Name fixes: 0x0066DC50 ?CreateRegionMinimapTGAFile@CRegionMinimap@@..; 0x00697C60 ?UpdateCleanupFlags@CGameCameraManager@@AAEXXZ.
Header-user recheck: run with the batch's follow-up commit.
