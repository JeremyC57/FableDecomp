# Chunk 723 round 3 (agent723c) -- notes

Result: 0 matched, 3 DIFFER (all with byte-length parity or close), 0x00B77370 not attempted.
Overlay: $S/w22/agent723c/hdr (built on, not forked). Grade with FABLE_HDR_OVERLAY=$S/w22/agent723c/hdr.

## Rows
- 0x00B70A90 CEngineWaterRenderer::RenderEnvironmentMap -- DIFFER(3567v3567), 8 masked lines. Only real block is the
  UV-V bookmark register swap at 0x00B71724 inside an inlined SetRenderState (documented test/or lane gap; not re-probed).
  Round-3 cleanups: GFClamp01 (landed 0x0070D390 spelling) replaces the invented Clamp; CCountedPointer<CTexture> replaces
  the local texture-pointer stand-in; PeekGraphicBank; callee addresses corrected (new 0x00BFEA1A, delete 0x00BFE9BC,
  _CIacos 0x00BFEBA2 x2, tan is inline fptan, ResetWorldTransform 0x00988290).
- 0x00BC04F0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite (PDB spelling confirmed in Ego_r and FableWin;
  class is the sprite-group manager, takes CEngineSubPrimitiveBase& + ERenderPassType) -- DIFFER(3786v3751), 135 masked
  lines (from 518). Residue: VC7.1 hoists the pass-7 alpha-ref constant 0x10 into ebx because the frame release's
  IsInitialised bit test uses the same 0x10, which spills `this` (~49 lines). Experiment SetAlphaRef(0x11) (not valid
  source) drops to 91, proving the cause; no source spelling, flag (/Oa, /GS) or structural variant moved it. Plus the
  SetRenderState lane gap and minor scheduling (camera base add, tail xor edi,edi, Info/Data zero order).
- 0x00B6EDF0 CEngineWaterRenderer::BuildConstantBuffers -- DIFFER(3144v3144), 14 masked lines, all store scheduling
  (loop-head store/load order, v1/v6/v12/v13). Needs FABLE_RETAIL_C3DTRIANGLE2. ~100 further attempts this round.
- 0x00B77370 -- not attempted (optional, gated on 10+ attempts per row; time went to RenderSingleSprite).

## Header debt
- CGraphicFrameData / CGraphicFrame / CGraphicDataBank / CGraphicDataBankHeader / CGraphicInfo are local models in
  differ/00bc04f0.cpp (no overlay header yet; GetFrame 0x009FF8D0 out of line, Get*Factor inline).
- CEnginePrimitiveManagerParticleGroup has no engine header; local model with SetUpPointLights 0x00BBC0B0 and
  SetupPixelShader 0x00BBCC70. Manager pointer = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[24]
  (GetSupportedTypes 0x00BBCF30 pushes 0x18).
- engine/CEngineWaterSettings.h prints default=DIFF in hdrcheck2. It was inherited unchanged from the round-2 overlay
  (typed members without a retail branch); round 3 did not touch it. The coordinator must move those typed members
  under a FABLE_RETAIL_CENGINEWATERSETTINGS branch (or accept the change) before landing. All other CHANGED headers
  print default=same (work/hdrcheck2.txt).

## Descriptive (non-PDB) names introduced
ERenderPassType enumerators RENDER_PASS_DISPLACEMENT=6 / RENDER_PASS_DISPLACEMENT_ALPHA_TEST=7; CRenderStateManager enums
ALPHA_BLEND_MODE / BLEND_OP / ALPHA_TEST_FUNC / POLY_CULL_MODE enumerator spellings; table names D3DAlphaBlendModeTable
0x01396F68, D3DBlendOpTable 0x01396ED0, D3DAlphaTestFuncTable 0x01396DD8, D3DPolyCullModeTable 0x01396FB0;
g_spriteRenderEnabled_0139b6e4; g_pDisplacementRenderer_01436e38.

## Overlay changes (round 3, hdr/engine/)
New: CEngineInternalPrimitiveSprite.h, CEngineLocalLightArray.h, CEnginePrimitiveThemeChannelDesc.h,
CEngineSubPrimitiveBase.h. Changed (all behind their FABLE_RETAIL_<CLASS> branch, default path unchanged):
C2DVector (AccurateNormalise), C3DBoundingSphere (centre/radius ctor), CBankFile (EntryExists 0x009CC190), CDisplayManager
(CopyBackBufferToTexture 0x009BF5E0), CEngineCamera (typed transforms, LookVector, PeekHFOV/PeekVFOV/PeekLookVector,
PeekInverseNonHomogenousViewTransform -> +0x144), CEngineComponent (PeekRenderEnabled), CEngineLightingManager
(GetLightsInSphereFast 0x00B46B90, members +0xce..+0xe8, four setters __forceinline), CEnginePrimitiveManagerParticleSpriteGroup
(six Apply* shader pickers), CEnginePrimitiveRenderUtil (7 PDB inline helpers), CEngineResourceManager (EGraphicBankNo,
PeekGraphicBank), CEngineScreenEffectDisplacementRenderer (retail branch : CEngineComponent, size 0x1ac), CRenderStateManager
(enums, setters, private static tables), CTexture (GetDimensions 0x009F9DB0), CIDrawEnvironmentParams
(CIDrawEnvironmentTextureSet = 0x18), CRGBFloatColour (__forceinline ctor from CRGBColour), CShaderRenderManager
(SetUserVertexShaderConstants 0x009896D0 + overload addresses 0x00989A60/0x00989AB0/0x00989B50).

## Regrades
- Siblings (round 2's 12): all RELOCATION_MATCH under the final overlay (work/siblings.tsv): 00b77e20 00b75ca0 00b76bc0
  00b36ec0 00b67920 00b6d6d0 00b71880 00bc14e0 00c2a890 006286f0 00b677d0 00bc9800.
- Round-2 differ rows, unchanged: 00b70230 2138v2138 dl 25; 00b761d0 2531v2531 dl 23; 00b77370 2798v2734 dl 158;
  00b751f0 2722v2722 dl 20 with the header stand-ins removed (differ/00b751f0.cpp).

## GOTCHAS one-liners
- FABLE_RETAIL_CARRAY changes register choice and load order in RenderEnvironmentMap; define it.
- VC7.1 folds `1/a*h` to `h/a`, and folds `x*255/255` and `h*0.5*k` unless the steps are split into locals.
- CShaderRenderManager overloads: 0x00989A60 = (K, const C3DVector&, float), 0x00989AB0 = (K, const C4DVector&),
  0x00989B00 = (K, M,M,M,M), 0x00989B50 = (K, const CRGBFloatColour&); ret 0xC/8/0x14/8.
- CIDrawEnvironmentTextureSet is 0x18 bytes.
- CEngineLightingManager small setters must be __forceinline; plain `inline` is called out of line in these big bodies.
- A constant shared with a bit test (0x10 alpha ref vs IsInitialised 0x10) can be hoisted into ebx and spill `this`; not movable from source.
- CEnginePrimitiveRenderUtil PDB inline helpers: FindLocalLights takes the theme-channel desc BY VALUE, which yields retail's build-then-copy double store.
- Use CEngineResourceManager::PeekGraphicBank(static_cast<EGraphicBankNo>(0)) (not GraphicBanks[0]) to get retail's bank load order.
- Point-light index clamp spelled `unsigned long index = lights; if (lights > n - 1) index = n - 1;` matches; ternary/min forms do not.
- In RenderSingleSprite, splitting colour into CRGBFloatColour + C3DVector premultiplied locals gives the 0x30 frame.
- 0x01436E38 is the screen-effect displacement renderer (new 0x1ac + ctor 0x00B86C00 at 0x00B266A4), not engine settings or the shadow renderer.
