# Hand chunk 2045 (CEnginePrimitiveManagerParticleDecalGroup): 1 of 1 byte-exact

## What matched (1)
- 0x00BB8EF0 CEnginePrimitiveManagerParticleDecalGroup::RenderSubPrimitive -- RELOCATION_MATCH 1941v1941 (`// Flags: /d2inlT189`), test PASS
  (g++ tall.sh + VC7.1 tall.py), read-only vland RELOCATION_MATCH + behaviour PASS. Source `src/00bb8ef0.cpp`, test `test/00bb8ef0.cpp`,
  overlay `hdr/` (hdr/README.md).
  - What it does: if the manager is enabled and the sub-primitive's parent group holds decals and is a landscape decal group: fetch frame 0
    of the group's decal-bank graphic (PeekGraphicBank(0)->PeekGraphic(DecalBankIndex)->GetFrame(frame, 0, 0, 0), return if not available);
    capture block + shaders on; texture stage 0 = the frame texture; U / V clamp; SetupBlendOpState(BlendOp);
    premultiplied = SetupAlphaBlendingStatesFromBlendMode(BlendMode); point lights from the particle-group manager
    (GFDebugPDynamicCast(g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(24))->SetUpPointLights()) when Flags & 0x80, else
    SetSelfIlluminationAmbient(100); UpdateGlobalLightingStates; PrepareForRender(1); vertex-shader constant layout 3; if LandscapeDecal
    RenderLandscapeDecalGroup(group, premultiplied, lights) (the mesh-decal branch is compiled out on PC); RestoreDefaultPrimitiveParameters;
    the scoped objects restore layout, shader enables and the capture block, and the frame reference is released. `pass` is not read.
  - Shape: the first draft (probe/v1.cpp, written from the parked sibling 0x00BBE090 RenderSingleSpriteTrail and the landed 0x00BBC460)
    differed only by three inline decisions plus one store: with /d2inlT189 it was 531 of 534 instructions; declaring `CGraphicFrame frame;`
    before the graphic lookup moved retail's `mov [esp+20h],edi` (0x00BB8F44, FrameData zeroed in the dead `sub` home before the bank read)
    and gave all 534 instructions (probe/v2a.cpp; v2b with the lookup chained into the GetFrame call is identical).

### Inline split (why the flag; measured with tools/inl.sh = agent2003 / 2037 c2 trace)
- Default threshold 74, cost rule (c2.dll 0x1078D1AA; score = c1 * (1/f + 6) / 7): retail expands CGraphicDataBank::PeekGraphic (c1 55,
  f 0.125, score 110), CRenderStateScopedCaptureBlock() (c1 38, f 0.0625, score 119) and ~CScopedShadersEnable (c1 60, f 0.0625, score 188);
  the plain build calls them (1766v1941). Every other inline site is already expanded at 74 and stays so at 189 (trace v2a with the flag:
  50 decisions, 0 refused).
- /d2inlT189 is the smallest N (/d2inlT188 -> 1884v1941, the destructor stays a call).
- Plain C++ tried first: the frequencies come from the four exit tests (enable, `size() > 0`, `LandscapeDecal`, GetFrame), each halving;
  ~CScopedShadersEnable needs f >= 0.38 at the end of the function, so no if / return spelling reaches it (probe/v3.cpp: early returns +
  `if (GetFrame) { ... }`: same three refusals, and different code, 1875v1941).
- Budget-rule check (agent4004's finding: callees not yet compiled get c2's budget rule): defining PeekGraphic after the row (probe/e1.cpp)
  still takes the cost rule -- a non-inline function is compiled after its inline callees; making the row an inline virtual emitted through
  a constructor (probe/e2.cpp, diagnostic only) moves every site to the budget rule but then refuses list::size, ~CGraphicFrameData, the
  capture ctor, the shader-enable setters and ~CScopedShadersEnable: not retail either.
- The scoped helpers keep plain `inline` (retail has uncalled out-of-line copies: ??0CScopedShadersEnable 0x009D8AD0, ??1 0x009D8B40).
  The two CEnginePrimitiveRenderUtil blend helpers must be FABLE_FORCEINLINE: as plain inline SetupBlendOpState scores 484 (c1 154) and
  SetupAlphaBlendingStatesFromBlendMode has c1 693, over c2's hard 450 cost cap (probe/v4p.cpp: 1296v1941).

## Names and evidence
- Row: Ego_r.pdb + FableWin.pdb ?RenderSubPrimitive@CEnginePrimitiveManagerParticleDecalGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z;
  RTTI vtable 0x012A5998 slot 5 (rtti-vtables.tsv; the only reference to 0x00BB8EF0 is 0x012A59AC). Leaf confirmed with objdump -t.
- FableWin.pdb per-TU COMDAT list (donor 0x02EABCA0 RenderSubPrimitive -> SetupAlphaBlendingStatesFromBlendMode 0x02EAC0C0 -> SetupBlendOpState
  0x02EAC160 -> RenderLandscapeDecalGroup 0x02EAC1D0 ... list::size 0x02EAD750 ... GFDebugPDynamicCast<CEnginePrimitiveManager*,
  CEnginePrimitiveManagerParticleGroup*> 0x02EAEFB0) names the helpers the body uses; RenderMeshDecalGroup is a 0x10-byte stub there
  (the second `LandscapeDecal` test at 0x00BB951E with no else call is the compiled-out mesh path).
- Callees / globals: PDB spellings through the shared headers (LABELS.tsv; NAME_FIXES.tsv lists the wrong spellings landed rows bind for
  GetFrame, Uninitialise, RestoreCaptureBlock, the four shader enables and SetVertexShaderConstantLayout).
  Established data spellings (no PDB data symbols on this machine): NEngineInternal::GPResourceManager 0x01436E98, GPRenderManager
  0x01436E18, GPShaderRenderManager 0x01436E14, g_pEnginePrimitiveTypeInfo 0x01436E84 (26 landed rows; one row binds g_pPrimitiveTypeInfo),
  g_pEngineLightingManager 0x01436E9C.
- Inline bodies proven against retail's uncalled out-of-line copies (free probes, tools/qx.py): PeekGraphic = 0x0041C1B0 MATCH;
  distance<list<CEnginePrimitiveHandle>::const_iterator> (list::size via FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE) = 0x00BB9770 MATCH.
  CScopedShadersEnable ctor / dtor copies 0x009D8AD0 / 0x009D8B40 agree with the header bodies by inspection (constructors / destructors
  cannot have their address taken, so not compiled against).
- Descriptive (no PDB locals on this machine): parameter names `sub`, `pass` (as the landed siblings), locals `group`, `frame`, `info`,
  `capture`, `shaders`, `layout`, `premultiplied_alpha`, `point_light_count` (c2038's parameter names). No made-up function / class / global.
- PrepareForRender(1): bit 0 set (no ResetWorldTransform), bits 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400 clear (no camera / lighting
  / bone / transform updaters in retail 0x00BB946A..0x00BB9508); bit 2 and bits above 0x400 are not observable, 1 is the minimal value.
- GetPrimitiveManager(24): retail `mov ecx,[[0x01436E84]+70h]` = PrimitiveManagers[24] (Ego_r PrimitiveManagers[47] at +0x10).

## Header debt / review at landing
- hdr/: chunk 2038's overlay (unchanged) + chunk 2045 additions (hdr/README.md table): one declaration in the decal-group typed branch, the
  FableWin GetParentBase / PeekParentBase opt-in, CGraphicInfo GetFrame declaration + PeekGraphic body opt-ins, the c2037 CGraphicFrame.h
  copy, a new CEnginePrimitiveManagerParticleGroup.h. Chunk 2038's row 0x00BB8750 regraded under this overlay: RELOCATION_MATCH.
- SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode bodies are in the row (engine/CEnginePrimitiveRenderUtil.h says the including row
  defines them); the parked hand742 0x00BBE090 writes the same bodies -- one shared text if both land.
- GFDebugPDynamicCast defined row-locally with engine/CThing.h's exact text (landed precedent 0x00984D90 and nine others).
- vland WARN RELOC_NAME: 0x01396DC8 `?D3DTextureUVModeTable@CRenderStateManager@@0PAKA` (the header's PDB-DATA spelling) v label
  'g_textureUVModeToD3D' (chunk 723's descriptive name) -- label debt, not a row problem.
- Landed local models the new shared headers could replace later: 0x00BCB800 SpriteTrails::RenderSubPrimitive (local CGraphicInfo / frame /
  render core), 0x00BBC460 / 0x00BBC0B0 (local particle-group manager models).

## Commands (resume)
- Grade: `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2045/hdr; bash gall.sh` (honours the Flags line).
- Free probes: `python3 tools/cc.py <src> <tag> '?RenderSubPrimitive@' --flags /d2inlT189` (compile only) then
  `python3 tools/md.py 00bb8ef0 '?RenderSubPrimitive@' --obj tmp/cc_<tag>/x.obj -n 0` (relocation-masked diff);
  `bash tools/inl.sh <src> <tag> [flags]` (c2 inline trace -> tools/inl/<tag>.txt); `QLEN=0x.. python3 tools/qx.py <addr> <src> <leaf>`.
- Measurements: grade 1 RELOCATION_MATCH (src), grade 2 RELOCATION_MATCH (comment-only change), chunk 2038 regrade RELOCATION_MATCH.
  Probes on top: 13 compile-only builds (v1 1766, v1t 1941 531/534, v2a / v2b 534/534, v2a188 1884, v3 1875, v3g, v4 534/534, v4p 1296, v4d
  534/534, e2 1756, e1 trace only), 6 inline traces (v1, e1, e2, v3, v2a@189, v4p@189), 2 out-of-line COMDAT proofs, tall.sh / tall.py, one
  read-only vland.

## Parked for Fable 5.1 (0)
## Skipped
- none
## Label corrections
- NAME_FIXES.tsv (8 callee addresses). Catalog: 0x00BB8EF0's reservation estimate (2659) included the decal TU's COMDAT run; the oracle's
  1941 is right (SPLITS.tsv).
