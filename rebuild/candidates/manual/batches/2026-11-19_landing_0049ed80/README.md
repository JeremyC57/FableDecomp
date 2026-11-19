# Landing 0049ed80 (2026-11-19, hand2b): CWorld::ConsoleSetFreeCameraLookVector

vland --land, strict headers: RELOCATION_MATCH + PASS (14 look vectors incl. the vertical fallback and both sides of the epsilon; 7/7 mutants).
FableWin donor shape (0x01B527F0 + locals) on shared headers: `C3DVector side_vector(look_vect.Y, -look_vect.X, 0.0f)`, GetMagnitude() <
DELTA_NOTIONAL_ZERO -> Initialise(1,0,0), up_vect = side_vector ^ look_vect, Normalise, SetView. Existing opt-in
FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22 gives retail's cross-product x87 order (XZ / XYZ groupings also match; X, XY,
ORIGINAL_FORMAL, NAMED_RESULT do not). New opt-ins: FABLE_RETAIL_CWORLD_CONSOLESETFREECAMERALOOKVECTOR (protected static __fastcall decl),
FABLE_RETAIL_CPLAYER_GETCURRENTFREECAMERA (FableWin inline). CGenericVar has no engine header: the row defines the Ego_r 28-byte layout locally
(queued: a shared engine/CGenericVar.h). Naming: 0x0129BA3C landed as g_NormaliseEpsilon is ?DELTA_NOTIONAL_ZERO@@3MB (`NAME_FIXES.tsv`).
