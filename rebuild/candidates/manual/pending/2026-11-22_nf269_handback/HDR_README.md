# NF269 overlay (2026-11-22)

## candidates/outline_glow_compiled.h (base: orig/candidates/outline_glow_compiled.h = the repo copy at the start of this session)
- New opt-in `FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER` (rows 0x00BEAAA0 / 0x00BEB4D0 and their tests define it): includes the
  retail branch of engine/CEngineLandscapeMemoryManager.h (with the landed `_DELETEVERTEXBUFFER` declaration opt-in) and
  engine/CEngineLandscapeRenderer.h (`_GPLANDSCAPERENDERER`) in place of the header's stand-ins `extern fable_u8* DAT_01436ea8`,
  `class LandscapeLayerList { InsertNode(int) }` and its local two-member `CEngineLandscapeMemoryManager`, and types the +0x10 member
  `field_10` as `CVertexBufferWin32*` (it is CWaterPatchMesh::VertexBuffer, freed through DeleteVertexBuffer 0x00BDA0D0).
- Existing lines are only wrapped (`#ifdef` / `#else` / `#endif`); the default path is token-identical.
- pin_asserts.py pinned 3 asserts (the three `FABLE_STATIC_ASSERT` lines below the class, now `FABLE_STATIC_ASSERT_AT(33 / 34 / 35, ...)`, their
  old line numbers).
- Proof: equiv_check over the closure of the header (eqrows.py: 1,374 rows, the two includers plus every `../src/compiled/` includer),
  repo tree v a copy with this overlay: 0 rows differ (evidence/equiv.tsv empty). Control: the same copy with a token planted in the
  default path: both includers (src + test) differ (evidence/equiv_ctl.tsv).
- Landed includers: only rows 0x00BEAAA0 / 0x00BEB4D0 and their two tests (grep of rebuild/src, rebuild/tests, rebuild/include);
  candidates/00beaaa0.hpp also models LandscapeLayerList but nothing includes it. The opt-in changes no other includer.
- No other naming agent overlays this header (NF268's entries are elsewhere).
