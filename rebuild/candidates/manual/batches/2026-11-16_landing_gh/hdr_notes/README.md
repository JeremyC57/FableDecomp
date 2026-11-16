# w24B header overlay (export FABLE_HDR_OVERLAY=<this dir> for every grade / vland)

Every file is the CURRENT repo header (2026-11-16) plus additions only (no line removed; `diff` shows `>` lines only), each behind a
new opt-in; default paths are unchanged. No line used by `FABLE_STATIC_ASSERT(` / `__LINE__` / `FABLE_RETAIL_ASSERT_ARM` moved unpinned:
the only plain asserts that shift are the 21 in engine/CTCMapwho.h (5 lines down), each rewritten as
`FABLE_STATIC_ASSERT_AT(<old line>, ...)`; the other files' asserts after the additions were `_AT` already or there are none.

- engine/CDefinitionManager.h: `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_INLINE` (row 0x00B625E0): the by-class-index
  `GetDef<T>(const CCharString&, long, CDefPointer<const T>*)` as a plain inline body with `SetDynamicCast` on a hit (the
  `..._GETDEF_INDEX_BODY` text, left to the inliner; retail expands the CSkyDef instance in the constructor). Do not combine with
  `..._GETDEF_INDEX_BODY` / `..._PASS22_GETDEF_INDEX_BODY`. The row pairs it with the repo's `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`.
- fable_user_profile.h (inside FABLE_RETAIL_CUSERPROFILEMANAGER):
  - `FABLE_RETAIL_CUSERPROFILEMANAGER_RENDERUIBACKGROUND` (row 0x004075C0): `void RenderUIBackground(float)` and
    `CTexture* PBackBufferTexture` at +0x138, carved out of the opaque tail in both the PASS22 and the default tail (layout
    unchanged, 0x140). Deliberately not named `..._UIBACKGROUND`: that is engine/CUserProfileManager.h's declaration-only opt-in,
    which two landed rows define.
  - `FABLE_RETAIL_CUSERPROFILEMANAGER_LOADGAMESTATE` (row 0x0040BCA0): the protected `void LoadGameState(CAFile&)` declaration
    (Ego_r / FableWin `IAEXAAVCAFile@@@Z`). The row also defines the repo's `..._VIDEODEFAULTS` + `..._PASS22_DEFAULTS` + `..._PASS22_STATE`.
- engine/CGraphicInfo.h: `FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME` (row 0x00542780): the FABLE_FORCEINLINE body of
  `CGraphicDataBank::PeekGraphicInfoFrame(unsigned long, unsigned long) const` (`PeekGraphic(n) ? PeekGraphic(n)->PeekInfoFrame(frame) : 0`),
  the text landed rows 0x00B64550 / 0x00BADB40 carry in-row. Needs `FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC`.
- engine/CCardinalSpline.h: `FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT` (row 0x007DDA30): inline `DeleteAllControlPoints()`
  (FableWin.pdb) and `AppendControlPoint(const C3DVector&)` (Ego_r / FableWin; the landed out-of-line copy 0x0075F980's text);
  `..._APPENDCONTROLPOINT_FORCEINLINE`: per-TU FABLE_FORCEINLINE on AppendControlPoint (QueueSwoop expands all three calls).
- engine/CTCMapwho.h: `FABLE_RETAIL_CTCMAPWHO_SETINMAP` (row 0x007DDA30): inline `void SetInMap(bool)` (FableWin.pdb
  `?SetInMap@CTCMapwho@@QAEX_N@Z`). Under plain FABLE_RETAIL_CTCMAPWHO the header still needs `..._STATTYPE` for its own
  `class CTCMapwho;` forward declaration (pre-existing; the row defines it).
- engine/C3DVector.h: `FABLE_RETAIL_C3DVECTOR_SUBTRACT_X_GROUP` (row 0x007DDA30): `operator-` with only the X difference
  parenthesised (an `#elif` after `..._SUBTRACT_XY_GROUP`; same value, one PAREN tuple).
- NEW engine/CTCCreatureFlyInCircles.h (row 0x007DDA30): hand-written retail class on the retail CTCBase, Ego_r.pdb members at
  retail offsets (0x78; byte-packed, PBurninationEffect +0x3B), Ego_r / FableWin method spellings and access. `EFlyMode` has no
  enumerators (names not in the PDB name lists). The landed CTCCreatureFlyInCircles rows still carry local models of this class.
