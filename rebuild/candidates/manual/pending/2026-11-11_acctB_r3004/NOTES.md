# Hand chunk 3004 (CEngineShadowRenderer .. C3DAnimation2): 1 of 2 byte-exact (near-miss retry pass)

Both rows start at real function starts and end at their own `ret` (checked by the prior chunks 2024 / 2014 and again here: 0x00B4FC40
ends `ret 0Ch` at 0x00B5036A, 0x00A4E300 ends `ret 0Ch` at 0x00A4E950); the oracle lengths are right, no SPLITS.tsv. Every grade and
trace used `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3004/hdr` (the merge of the two prior overlays, no change of mine: hdr/README.md).

## What matched (1)
- **CEngineShadowRenderer::SetupShadowFuncRenderStates** 0x00B4FC40
  `?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABVCMatrix3x4@@ABUCColorBufferDepthPackParameters@1@@Z` -- RELOCATION_MATCH
  (1837 v 1837), test PASS under g++ and VC7.1, vland (read-only) RELOCATION_MATCH + PASS, every relocation target = retail (tools/rel.py).
  Source: chunk 2024's parked body (flags `/d2inlT314`, Inline-budget line) with the five accessor results named:
  `CTexture& shadow_buffer_texture = GetShadowBufferTexture();`, `EShadowRenderingTechnique technique = PeekRenderingTechnique();`,
  `CTexture& depth_packing_texture = g_pEngineShadowRenderer->GetDepthPackingTexture();`,
  `const CMatrix4x4& world_to_shadow = SceneData.WorldToShadowBufferMatrixTextureSpace;`,
  `ELightingManagerShadowMode shadow_mode = g_pEngineLightingManager->PeekGlobalShadowMode();`.
  How it was found (all measurement free, 3 grades after the baseline check):
  1. The residue (column 0 of the expanded PostMultiply: retail `M2*m20 + m30*M3 + m10*M1 + m00*M0`, ours `M2*m20 + m00*M0 + ...`) was
     reproduced in a fragment (probe/f/f0.cpp + tools/fr.sh). There the order of the three other columns moves with every IL unit before
     the loop (period 4), and column 0 alone moves with the number of NAMED SYMBOLS of the function: dead `long d = k;` locals give a period-8
     rotation of the m00*M0 term's position and operand order and change nothing else (379 bytes every time).
  2. On the full row (c2 trace runs that keep the object, tools/tord.sh; no retail diff): k dead locals anywhere in the outer function body
     (entry, either branch, before / after the matrix, inside the fade block) give k=0 2'031, 1-2 2'0'31, 3-4 02'31, 5-7 2'310 (= retail),
     8 2'031 again; the rest of the loop and the length stay retail's. Locals of inline bodies expanded earlier (an extra wrapper around
     AttachTextureToStage, a ctor without the Initialise level, dead locals in Initialise / PostMultiply) do not count; argument
     temporaries do not count; another function in the TU does not count.
  3. Natural, instruction-neutral spellings that add outer symbols: named accessor results (references / enum locals are forwarded:
     no instruction, no frame slot). Named C4DVector constants also move the order but each takes a frame slot (graded once: frame 0x90 ->
     0xA0, 423 / 457 equal). Five accessor names (one per accessor the body calls: the FableWin COMDAT run after this function is exactly
     GetShadowBufferTexture, GetDepthPackingTexture, PeekGlobalShadowMode) gave retail's order with length and frame unchanged in the trace,
     and RELOCATION_MATCH on the grade.
  The names are descriptive (no PDB local names exist); any 5..7 such forwarded names give the same bytes -- the choice of these five
  (every accessor result named, the house style of the rest of the body) is mine.

## Parked for Fable 5.1 (1)
- 00a4e300 C3DAnimation2::GetDeltaMovement DIFFER(1609v1619): product sums E21..E33 (retail stores 0x00A4E766, 0x00A4E78B, 0x00A4E7B0,
  0x00A4E7CE, 0x00A4E7F0, 0x00A4E810) retail ((t0+t1)+t2), ours ((t2+t0)+t1) with t2 the start-matrix a11-family term, plus one product
  operand swap at 0x00A4E766 / 0x00A4E7CE; translation 0x00A4E90A / 0x00A4E90D term order; start ToMatrix E11..E13 kept on the x87 stack in
  retail (0x00A4E567 `fxch st(1) ; fstp [esp+8] ; fstp [esp+18h]`); frame 0x100 v 0x104 (13 store-order issues by tools/t2.sh) | tried
  this pass: the outer-symbol-count lever that closed 0x00B4FC40 (dead locals k=1..7: no effect at all), declaration order of the two
  matrices / two keyframes (no effect) | next: chunk 2014's own lead -- the read order of the start-matrix field candidates in c2prio's ref
  events (the offset-0 field shares the matrix symbol's candidate id, which is why the a11 term is the odd one out), then Transpose /
  ToMatrix / SetMultiplyNoTranslation nesting spellings that move that id. The source in differ/ is chunk 2014's, unchanged (its test
  differ/00a4e300.test.cpp passes under g++; it was not re-run under VC7.1 in this pass).

## Skipped
- None.

## Names and evidence
- Rows: as proven by chunks 2024 / 2014 (LABELS.tsv, NAME_FIXES.tsv carried over for 0x00B4FC40 and its relatives).
- Descriptive (made-up) names in 0x00B4FC40: the five locals above and the EShadowRenderingTechnique enumerators (chunk 2024 / hand727);
  ELightingManagerShadowMode's enumerators are not recovered, so the shadow-mode test stays `== 1`.
- Globals unchanged from chunk 2024 (NEngineInternal::GPRenderManager / GPShaderRenderManager, g_pEngineShadowRenderer 0x01436E60,
  g_pEngineLightingManager 0x01436E9C); the 0x01436E60 / 0x01436E9C family question stays open (see LESSONS.md).

## Label corrections
- None new (see NAME_FIXES.tsv from chunk 2024).
