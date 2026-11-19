# hand4a header overlay (one line per opt-in)
(empty) Everything earlier is merged into the repo by the coordinator: stlport/fable_containers.h PAIR_LESS_RIGHT_BYVALUE (b1),
engine/CTCInGameMenu.h PCSETQUIT (b2), engine/C3DVector.h ACCURATE_MAGNITUDE_PAREN_RETURN + DIVASSIGN_RECIPROCAL_PAREN and the three
NCameraModes_CScriptModule*.h headers (b3). The history of each is in NOTES.md and batches/b1..b3/hdr.
- engine/NCameraModes_CBase.h (copy of today's repo + 2 opt-ins, CYLINDER section only):
  FABLE_RETAIL_NCAMERAMODES_CBASE_SETPOSITION_INLINE makes SetPosition / SetObservedPos inline (`GoTo.Position = p` / `LookAt.Position = p`;
  FableWin debug 0x02200060 / 0x0283A030). FABLE_RETAIL_NCAMERAMODES_CBASE_VIEWOCCLUDED_INLINE makes IsCameraViewPosOccluded inline
  (`return ViewOccluded;`, FableWin 0x021FE230). For row 0x0087BD70. Default path unchanged.
- engine/CGameCameraManager.h (copy of today's repo + 1 opt-in, LOOKAROUND section): FABLE_RETAIL_CGAMECAMERAMANAGER_WORLDHEIGHT_GETTERS
  adds public inline GetMinWorldHeight / GetMaxWorldHeight (FableWin 0x029C7E80 / 0x029C7EA0). Default path unchanged.
- engine/NCameraModes_CRotateAround.h (NEW): CRotateAround : CBase (CYLINDER) with Ego_r members at their retail offsets (0x24 lower:
  StringLengthXY +0xA4 .. Theta +0xC4), sizeof 0xC8 (as the landed OnInit 0x0087BF40 asserts); virtual declarations from the Ego_r /
  FableWin name set.
