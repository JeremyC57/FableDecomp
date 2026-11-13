# acctC hand-back c19a (agentC19, 2026-11-13) — supersedes c8a and c17a

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00BB3E80 | CEngineClothRenderer::RenderClothPrimitive | 4,540 | RELOCATION_MATCH | PASS |
| 0x00BB30A0 | CEngineClothRenderer::Render2DClothPrimitive | 3,538 | RELOCATION_MATCH | PASS |
| 0x00B9E5B0 | CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer (bonus, not reserved) | 402 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.

What changed against c8a (cloth rows): the fitted `FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB` is no longer used (repo default
`operator-`); `GetAppropriateMipmap` is the FableWin-backed text (81 instructions at /Od, as FableWin); globals carry PDB
spellings (`NEngineInternal::GP*`). `hdrdiff/C3DVector.h.diff` still shows the two unused `*_SUB` opt-ins of the overlay:
do not apply them at landing. The c8a review items still stand (`history/README_c8a.md`): the dead release asserts
(`fable_assert.h`, counts fitted: 11 and 1) and the FlushRender spelling.

Name corrections (two sources, `NAME_FIXES.tsv`): 0x01436E80 `NEngineInternal::GPPrimitiveRenderer`, 0x01436E78
`NEngineInternal::GPPrimitiveRendererSharedData` (landed users spell 0x01436E80 `g_pEnginePrimitiveRenderer`), 0x01436E60
`GPShadowRenderer`, 0x01436E84 `GPPrimitiveTypeInfo`, 0x0143B990 `GTopBitInByteTable`.

Open, in `open_*/` (successor agentC20):
- 0x00BA2350 SetupRenderModeShadersAndConstants: DIFFER(9516v9632). Blocks the whole TU.
- 0x00BA5260 RenderMeshPrimitive and 0x00BA50A0 SetupStaticBlocks: RELOCATION_MATCH inside the TU, but the TU needs the real
  0x00BA2350 body, so they land with it. The TU's shared-header GetDiffuseTexture 0x00BA9360 / GetBumpTexture 0x00BA95D0
  texts match at their own addresses and are replacement candidates for the landed local-model files.
- 0x00B783F0 water Render: DIFFER(9343v9343), three residues.
