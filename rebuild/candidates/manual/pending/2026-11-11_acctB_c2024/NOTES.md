# Hand chunk 2024 (CEngineScreenEffectOutlineGlow .. CEngineShadowRenderer): 1 of 2 byte-exact

## What matched (1)
- **0x00B891B0 CEngineScreenEffectOutlineGlow::CEngineScreenEffectOutlineGlow(const CEngineInit&)** -- RELOCATION_MATCH 1819v1819, test PASS
  (g++ and VC7.1). The constructor: CEngineComponent base, the OldRenderTarget member built in place (CBaseClass(), vtable 0x0129C800,
  two CSurface), three CVertexShader / three CPixelShader member ctors, the capture block's inline `Open = false`; then the body:
  OutlineGlowScaleFactor 0.333, an A8R8G8B8 CPixelFormat, a render-target scratch page with one 256x256 and two 128x128 textures,
  the three screen quads (1/512 and 1/256 half-texel offsets), the colour loop, the shader look-ups (the SIMPLE copy-outline pixel
  shader when InitFlags & 0x40) and the default parameters. Shape: the sibling glow / displacement constructors' shape (one statement
  per vertex field, quad by quad, positions then UVs; the 12 colours as one `for (i < 4)` loop); OutlineTexture cleared between the
  first and second AllocateTexture, the blur pointers after the third; the pixel-shader pair as an if / else (the SIMPLE arm's second
  look-up cross-jumps into the else arm at 0x00B896E2); the white override colour through `CRGBColour::Initialise(255,255,255,255)`
  (a `= CRGBColour(...)` temporary builds it on the frame and copies a dword: the only diff of the first grade).

## Parked for Fable 5.1 (1)
- 00b4fc40 CEngineShadowRenderer::SetupShadowFuncRenderStates DIFFER(1837v1837): x87 term order of the inlined PostMultiply's column-0 sum
  only -- retail 0x00B5023F..0x00B5025E `M2*m20 + m30*M3 + m10*M1 + m00*M0`, ours `M2*m20 + m00*M0 + m30*M3 + m10*M1` (the
  `fld [ebp+8Ch] ; fmul [eax-18h] ; faddp` pair sits two terms early); 454 of 457 instructions equal, same length, every register,
  frame slot, bookmark test/or block and relocation equal | tried (16 grades): direct members for the shadow-buffer texture / technique /
  depth texture (all three, two pairs: changes the induction base and all four columns), switch on the technique, if-wrap of the early
  return, named technique local, named fade C4DVector, direct `ShadowMode`, `const CMatrix4x4&` scene-matrix reference, default ctor +
  `Initialise`, matrix declared at the top, AttachTextureToStage through CTexture::GetD3DTexture; out-of-line screening of six PostMultiply
  bodies against retail's own COMDAT 0x00B507E0 (only the landed body matches); FABLE_FORCEINLINE vs /d2inlT314 (identical) | next: find
  the c2 key of the canonical 4-term sum order (it reacts only to the PeekRenderingTechnique() accessor in the technique condition, and
  in fragments to whole control-flow units: setters with internal ifs, early returns, a call in an if; not to symbol counts, dead locals,
  inline levels in argument positions, the inline helpers' early-return forms or other functions in the TU), then pick the natural
  spelling that yields retail's key. Pass 3 was not used (my context budget, not the grade budget, ran low): 10 grades remain for it.
  The parked source (`differ/00b4fc40.cpp`, flags `/d2inlT314` with an Inline-budget line) and its test (`differ/00b4fc40.test.cpp`,
  PASS under g++ and VC7.1) are ready for a retry.

## Skipped
- None. Both rows start at a real function start and end at their `ret` (no cold code, no second function): 0x00B891B0 ends `ret 4`
  at 0x00B898C8 (next function 0x00B898D0 the landed destructor); 0x00B4FC40 ends `ret 0Ch` at 0x00B5036A (next 0x00B50370 the
  constructor). The oracle lengths are right (no SPLITS.tsv).

## Names and evidence
- Row names (both proven, two sources each): see LABELS.tsv. The catalog labels were right.
- `CEngineShadowRenderer::CColorBufferDepthPackParameters` is a nested struct (U-keyed: `ABUCColorBufferDepthPackParameters@1@`), and the
  3-argument overload takes const references (`ABVCMatrix3x4@@`): the landed 0x00B50510 source spells a local pointer signature that
  binds a wrong symbol for 0x00B4FC40 (NAME_FIXES.tsv).
- 0x00B89080 is FinishWithDynamicTextures (Ego_r donor order and size, body), landed as the descriptive ReleaseTextures (NAME_FIXES.tsv).
- CMatrix4x4: retail's COMDATs prove `CMatrix4x4(const CMatrix3x4&) { Initialise(m); }` (ctor 0x00B50AE0 returns this, Initialise
  0x00B50770 does not), matching FableWin's separate ??0 / ?Initialise symbols.
- Globals: 0x01436E98 as `g_pResourceManager` (row 1; the bound spelling the sibling glow constructor 0x00B5F090 uses). **Unresolved**:
  0x01436E98 has several landed spellings (`NEngineInternal::GPResourceManager` in 0x00BADB40 / 0x00B2D9F0 / chunk 2018, `g_pResourceManager`,
  `g_pEngineData`); the NEngineInternal::GP* family (GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14, GPEngine, GPCamera...)
  suggests `NEngineInternal::GPResourceManager` is the consistent choice -- the coordinator should pick one (no PDB data symbol either way).
  Row 2 uses NEngineInternal::GPRenderManager / GPShaderRenderManager (established), `g_pEngineShadowRenderer` 0x01436E60 (bound) and
  `g_pEngineLightingManager` 0x01436E9C (settled per the CEngineLightingManager header); by the same family argument their real names may
  be NEngineInternal::GPShadowRenderer / GPLightingManager (unproven).
- Descriptive names: the EShadowRenderingTechnique enumerators (the PDB keeps only the tag; hand727's spellings reused). No other made-up name.

## Header debt
- engine/CEngineScreenEffectOutlineGlow.h and engine/CEngineShadowRenderer.h: the retail branches gained member opt-ins (hdr/README.md);
  the landed OutlineGlow rows (0x00B89080, 0x00B898D0, 0x00BEAAA0/0x00BEB4D0 propagated junk) and 0x00B50510 still use local models.
- engine/CRenderStateCaptureBlock.h: the FableWin-only inline constructor is an opt-in; every in-place `Open = false` store (0x00B86C00 too)
  wants it.
- The shadow TU's COMDAT run (0x00B4B7B0..0x00B4B826, 0x00B50590..0x00B50B40) lists the inline accessors the TU uses (LABELS.tsv).
