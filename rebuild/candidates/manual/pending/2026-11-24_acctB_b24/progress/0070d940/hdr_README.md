Overlay for row 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim (B24C6, 2026-11-24). Full copies of the CURRENT repo headers plus
additive #ifdef sections (default branches unchanged); the B24C4 overlay is kept as ../hdr_old.
- engine/CAnimSet.h FABLE_RETAIL_CANIMSET_MESHANIM: GetMeshAnim (0x0070C900, decl), GetFirstMeshAnim (inline), private
  GetInterpolatedAlpha / GetInterpolatedInternalAlpha (inline), GetInterpolatedTime (0x0070C800, decl). Ego_r / FableWin names.
- engine/CAnimateThing.h FABLE_RETAIL_CANIMATETHING_PROGRESSFROMALPHA: GetProgressFromAlpha inline (0x0070BE40 is its COMDAT copy;
  retail calls it before the transition loop and expands it inside), body = the landed row's.
- engine/C3DAnimationTransitionState.h FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE_SETTERS: SetStartState / SetEndState inline
  (`StartState = state;` / `EndState = state;` = landed 0x00710560 / 0x007105A0 COMDAT copies; expanded in the loop at 0x0070DBD6 /
  0x0070DC0C), SetProgress inline (FableWin 0x01E95FF0).
- engine/C3DAnimationStateAlpha.h FABLE_RETAIL_C3DANIMATIONSTATEALPHA_INDEX_CTOR: ??0C3DAnimationStateAlpha@@QAE@KMM@Z (FableWin 0x01E99920).
- engine/C3DAnimationBlendState.h FABLE_RETAIL_C3DANIMATIONBLENDSTATE_ADDCOMBINATIONBLEND: AddCombinationBlend(const C3DAnimationStateAlpha&)
  (Ego_r 0x00B96B60; retail 0x00A3C310).
- engine/C3DAnimationInfo.h FABLE_RETAIL_C3DANIMATIONINFO_ISLOADED: IsLoaded() declaration (out of line 0x00A23F50).
