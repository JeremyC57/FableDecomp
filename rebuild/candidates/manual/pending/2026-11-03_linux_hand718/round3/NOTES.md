# Chunk 718, second pass (agent718b, 2026-11-03)

Builds on the previous agent's `2026-11-03_linux_hand718` records and its `hdr/` overlay (copied here, extended, not forked).

| Row | Result | Attempts |
|---|---|---|
| 0x0089E190 CGameScriptInterface::CameraUseCameraPoint (803) | RELOCATION_MATCH | 4 |
| 0x00487570 CPlayer::DrawFreeCamDebug (1305) | RELOCATION_MATCH | 9 |
| 0x0088EDC0 CGameScriptInterface::CameraDoConversation (1564) | DIFFER(1612v1564), parked `differ/0088edc0.cpp` | 11 |
| 0x008A93A0 CGameScriptInterface::EntityWillTeleportToArea (620) | DIFFER(618v620), unchanged best (previous agent 10 + 3 here) | 3 |

## Levers that worked
- CameraUseCameraPoint: operand order of `startPos + lookDir` decides the operator+ temp order; the four-vector and the
  thing-target CScriptModuleTransitionBetweenPoints ctors (Ego_r overloads); `SetScriptedCameraMode(CCountedPointer<...>)`
  by value; `ConvertSecondsToWF` as an inline static (`(long)(GetConstantFPS() * seconds)`).
- DrawFreeCamDebug: `GFRoundFloat` as `float m = 1; while (places--) m *= 10.0f; float rounded = (float)(long)(v*m); return rounded / m;`
  (a for-loop folds the 1-place multiplier; the one-expression quotient gives `fidivr`); column resets as implicit
  double->float (`pos.X = VIRTUAL_SCREEN_WIDTH * 0.25;`), the explicit cast schedules differently.
- CameraDoConversation: case blocks in source order 2,3,0,1,4,5 give retail's layout; `GetFacingVector()` is slot 70 (+0x118).

## Residues
- 0x0088EDC0: retail cross-jumps cases 0/1/3 and 4/5 at the facing call ("call; push 0; push angle; jmp common") before
  scheduling; VC7.1 here merges only after the scheduler hoists the squared loads. Shared function-scope locals reproduce
  the merge points but add assignment temporaries (frame 0xEC).
- 0x008A93A0: ebx/this vs spilled-thing register colouring; neither suggested lever moves it.

## Header overlay (hdr/, all `default=same` under hdrcheck2)
- `engine/C3DVector.h` FABLE_RETAIL_C3DVECTOR_NORMALISE: `operator-=`, fast-inverse-sqrt `Normalise()` over
  `GFastOneOverSqrtTable`, `Rotate(float, float)` 0x00A14260.
- `engine/CTCPhysicsBase.h` (FABLE_RETAIL_CTCPHYSICSBASE): GetFacingVector() declared before GetFacingVector(float) -- VC7.1
  lays an overload group out in reverse declaration order; retail slot 70 (+0x118) is the no-argument form. No landed row
  calls either overload under this macro.
- `engine/CThing.h` FABLE_RETAIL_CTHING: GetTCGraphicAppearance() (const and non-const).
- `engine/CGameCameraManager.h` FABLE_RETAIL_CGAMECAMERAMANAGER: SetScriptedCameraMode 0x00699E60.
- `engine/CThingManager.h` FABLE_RETAIL_CTHINGMANAGER_PEEK: `const CThing* PeekPThingWithUID(unsigned __int64) const` 0x0051ED60.
- `fable_game.h` FABLE_RETAIL_CMAINGAMECOMPONENT_WF: inline static `ConvertSecondsToWF`.
- New `engine/C2DExtentsF.h`, `engine/CCamera.h` (Ego_r layout, sizeof 0x38, GetHorizontalFOV 0x00A0BE90).
- `engine/CFontBank.h` FABLE_RETAIL_CFONTBANK (polymorphic: dtor, GetStringExtents, GetFontMaxHeight, PrepareToRender, Preload,
  Render; inline GetVFontMaxHeight / GetVStringExtents over GFAXToVX / GFAYToVY).
- `engine/CRenderManager2D.h` FABLE_RETAIL_CRENDERMANAGER2D_TEXT: Draw2DText 0x009DD8F0.
- `engine/CPlayer.h` FABLE_RETAIL_PLAYER && FABLE_RETAIL_CCAMERA: CurrentFreeCamera (+0x40), FreeCamera, OldFreeCamera;
  DrawFreeCamDebug declaration.

## Header debt
CTCCameraPointDefinitionBase / CTCCameraPointScripted / CTCCameraPointScriptedSpline, the NCameraModes script modules and
NGraphicAppearance::CTCGraphicAppearance are declared locally in the sources (no engine header exists for them).
