# Chunk 2035 overlay headers (opt-in only; default paths unchanged)

- engine/CGameCameraBase.h (copy of rebuild/include/engine/CGameCameraBase.h): nested opt-in `FABLE_RETAIL_CGAMECAMERABASE_UPDATE`
  inside the existing `FABLE_RETAIL_CGAMECAMERABASE_RENDER` branch: declarations of `virtual void Update()` (public) and the private
  `float GetRotationAngle(const C3DVector&, const C3DVector&)`, `bool IsLandscapeCollision()`, `bool IsThingCollision()`.
  Evidence: Ego_r.pdb + FableWin.pdb spellings ?Update@CGameCameraBase@@UAEXXZ, ?GetRotationAngle@CGameCameraBase@@AAEMABVC3DVector@@0@Z,
  ?IsLandscapeCollision@CGameCameraBase@@AAE_NXZ, ?IsThingCollision@CGameCameraBase@@AAE_NXZ; retail addresses 0x006FDD60 (vtable
  0x01264A8C slot 0), 0x006FD4F0, 0x006FD6C0, 0x006FDA20 (Ego_r donor order = retail order). No layout change.
- engine/CWorldMap.h (copy of rebuild/include/engine/CWorldMap.h): opt-in `FABLE_RETAIL_CWORLDMAP_FINDLASTUNBLOCKEDGROUND` after the
  LINEOFSIGHT block: `bool FindLastUnblockedGroundPositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const`
  (Ego_r.pdb / FableWin.pdb QBE spelling, retail 0x0063D960, the existing reloc_bindings spelling; called at 0x006FE382). Declaration
  only, no layout change. The row includes this overlay copy first (before fable_string.h) so the repo copy cannot win.
