# acctC workflow hand-back wfA5 (unworked rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00598A1C | NUISystem::CFrontEndManager::BuildComponents (catalog: Init2) | 4,928 | RELOCATION_MATCH | PASS |
| 0x00AA2110 | C3DAnimationManager::CalcBlendedBoneTransformation | 4,556 | RELOCATION_MATCH (retail_asm via shared headers) | PASS |
| 0x00AA0FA0 | C3DAnimationManager::CalcBoneTransformation | 4,460 | RELOCATION_MATCH (retail_asm via shared headers) | PASS |
| 0x00B555A0 | CEngineWeatherRenderer::Render | 3,832 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). None had an earlier draft: all hand-authored.
REVIEW ITEMS:
- 0x00B555A0: WARN RELOC_CONFLICT — `CRenderStateManager::D3DAlphaFuncModeTable` resolves to 0x01396DD8 here and 0x01396DE8 in
  landed 0x00BBC460; both spell SetAlphaTestFunc(ALPHA_TEST_GREATER) (= base + 0x10), so one of the two recorded the element
  address instead of the base: the lander decides which binding is right.
- The two animation rows use the existing shared retail-asm helpers (GFFloatToLongNear, CMatrix4x4AlignedSSE), cited by
  address; FITTED: the GFFastArcCos named table values, and in 0x00AA2110 the first chain matrix spelt
  `keyframes[0].ToMatrix(matrix_a, false)`.
- 0x00598A1C: `#pragma optimize("s", on)` as its landed twin 0x0058AF10; local name `redefiner` is made up (descriptive);
  four RELOC_NAME label warnings (e.g. 0x00406880) in coordverify.log.
`open/`: 00b9ac30 CEnginePrimitiveManagerRepeatedStaticMeshes ctor (draft, a few compile items left).
