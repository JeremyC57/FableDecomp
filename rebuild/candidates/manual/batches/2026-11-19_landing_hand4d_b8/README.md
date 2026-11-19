# Landing hand4d b8 (2026-11-19): C3DPrimitive2::ConvertToVBFormatStaticBump (0x00A8F1B0)

**Result:** strict vland --land gives RELOCATION_MATCH + PASS; no flags. The test killed 7 of 7 mutants.

**IDENTIFY:** the catalog label TransformVertexBufferWithNormals_LoopUnrolled is wrong, and so is the local name in the landed caller 0x00A92740. Both are in
NAME_FIXES.tsv.

**Body:** FableWin's text, using:
- the VC6 for-scope idiom;
- Data->Vertices[c_vert];
- operator^ and Dot;
- Ego_d local names.

One `// Inferred:` line, for ORIGINAL_GROUPED_CROSS_PASS22: it clears the fnstsw / fstp st(0) swap that Codex had parked on.

**Headers:**
- New opt-in FABLE_RETAIL_C3DPRIMITIVE2_CONVERTSTATICBUMP in C3DPrimitive2.h (all asserts there are `_AT`-pinned).
- New engine/CTVertexNormTex1BumpBase.h and engine/CTVertexNormTex1Bump.h (Ego_r layout, 0x30 bytes).

**Header-user recheck:** C3DPrimitive2.h, 27 / 27 good.
