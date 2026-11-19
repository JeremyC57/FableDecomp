# Landing hand3c b18 (2026-11-19): CEnginePrimitiveRenderer::LoadNewScene (0x00B328C0)

**Result:** strict vland --land gives RELOCATION_MATCH + `00b328c0_TEST PASS`; no flags.

**Source:** FableWin 0x02DA8F90 statement for statement, with Ego_d locals.
- Shared headers only.
- `// Retail-asm:` line for the GFFloatToLongFloor fistp at 0x00B3292A.

**What closed it:** FABLE_RETAIL_CARRAY_INDEX_ASSERT. FableWin's CArray operator[] opens with the index ASSERT, and without it the element load is hoisted
(515v517).

**New opt-ins** (pure additions; asserts already `_AT`-pinned):
- CEnginePrimitiveRenderer.h LOADNEWSCENE: the virtual override declaration.
- CEngineScene.h PRELOADPRIMITIVES: three overloads, 0x00B92F80 / 0x00B92E50 / 0x00B930A0.

**Row-local stand-ins:** CEngineComponentParams, CEngineComponentLoadNewSceneParams and CEngineVideoOptions have no shared header yet. They use Ego_r member names,
and their sizes are asserted.

**NAME_FIXES:**
- 0x01436EB0 = ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B, replacing four wrong reloc spellings;
- 0x01436E60 = ?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A.

**Header-user recheck:** 72 / 72 good, run together with hand4e b3.
