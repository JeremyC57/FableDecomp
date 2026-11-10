# 2026-11-10 landing ap -- chunk 1013c (1 row) + one re-authored binder

Source: `pending/2026-11-10_linux_hand1013c/`. vland --land RELOCATION_MATCH, test PASS:
- 0x0063AB90 ?GetThemeFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z (582 bytes; the chunk oracle row was cut at 601 and re-read
  from the exe). Lever: the first lookup's C2DCoordI is a named local in its own `{}` scope filled member by member, which puts retail's
  X store between the two `_ftol` calls (0x0063ABBD). No header change.

Re-authored (vland --recheck-landed RELOCATION_MATCH, new self-contained test PASS):
- 0x00890090 CGameScriptInterface::SetEnvironmentThemeWeightExternals, now the PDB member
  ?SetEnvironmentThemeWeightExternals@CGameScriptInterface@@UBEXABVCCharString@@M@Z on the shared headers. The callee 0x006B3600 was bound
  as the invented `?Do@Inner@@QAEXHHHH@Z`; it is ?SetThemeWeight@CEnvironment@@QAEXJJJM@Z (Ego_r + FableWin name sets; chunk 1013c's
  LoadRegion draft calls it twice with (channel, 0, theme, 1.0f)). New opt-in FABLE_RETAIL_CENVIRONMENT_SETTHEMEWEIGHT in
  engine/CEnvironment.h (declaration only). reloc_bindings respelled by hand (recheck does not write bindings).

Parked (differ/, measured residues in the pending NOTES.md): the CEngineMap peeks PeekThemeId / PeekThemeBlend / PeekLandscapeHeight
(one block placement), PeekInterpolatedMapNormal (frame slot creation order; its oracle row is cut at 816 bytes, real body 528),
PeekInterpolatedWaterHeight (/d2inlT301 expands the inline callee; one kept frame slot left), Find{Ground,Theme}CollisionPointWithinCell
(scheduling), HeightAreaChanged (register choice), CreateLineSegment, LoadRegion and BuildStaticMapGenerationVisibilityInfo (SRC_FAIL
against current headers; header debt listed in NOTES).
