# wfA4 NOTES (acctC lane, 2026-11-14)
Tools: tools/g1.sh <addr> <leaf> <src> [k] [lines] (q.py + frame-masked sd.py), tools/win.py q.txt R|O lo hi, tools/inl.sh (c2 inline trace, FUNC=?Name@).
Overlay: hdr/ = agentC22 overlay + FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS (see hdr/README.md).

## 0x00B77370 CEngineWaterRenderer::RenderSeaReflectiveForeground -- DIFFER(2798v2734), differ/00b77370.cpp
- Name: Ego_r 0x00A25300 + FableWin 0x02D4F430 ?RenderSeaReflectiveForeground@CEngineWaterRenderer@@AAEXM@Z; helpers FableWin-only
  SetSeaReflectiveForegroundTexturesAndUVModes / SetupSeaReflectiveForegroundShaderConstantsAndFog (FORCEINLINE, no retail copy).
- Re-authored on shared headers (RenderSnow 0x00B54E30 idioms: CRenderStateScopedCaptureBlock, PeekGraphicBank->PeekGraphicInfoFrame->GetFrame,
  CGraphicFrame GETTEXTURE/DTOR_FORCEINLINE). Everything matches up to 0x00B77660 (capture block, both bank lookups, first GetFrame).
- Residue = one inline decision: retail calls ??_GCGraphicFrameData (0x0041BDF0, push 1) for the surf frame's final release at 0x00B77AE9;
  we expand it (trace: every ??_G site c1 42 f 0.25 sc 60 < 74; a+g 4850 at the edge release (expanded in retail too) v 5078 at the surf
  release). A caller-size (a+g > 9000) flip would need +3922..4150 tuples before the surf site -- far beyond dead-assert rulers.
  Follow-ons: frame 0x48 v 0x44 (edge frame gets a stack home), ebx/[esp+10] homes, SetRenderState test/or operand order.
- Tried: RELEASEDATA nested dtor (FableWin ~CGraphicFrame 9i = one call: evidenced) -> same 2798; both frames declared up front -> same.
- fwsize: GetFrame ours 202i v FW 146i (/EHsc), SetSeaReflective.. 125i v 269i, Render.. 104i v 107i, Setup.. 138i v 139i.

- (successor) Structure read from retail: the first-fetched frame's data lives in ebx (0x00B77660 `mov ebx,[esi+1Ch]`), is released first
  WITH a null test and expanded (0x00B77A97..0x00B77ADC); the GetTexture'd frame (no null test, c2 dropped it after the deref) is released
  second through `push 1 ; call ??_GCGraphicFrameData` (0x00B77AE7). The GetFrame failure exits (0x00B77557/0x00B7759E/0x00B776E3) jump straight
  to the capture-block restore. Ours expands both releases, so the `test byte [..+1Ah],10h` constant is CSE'd into bl (`mov bl,10h`), which
  costs ebx and pushes the first frame to [esp+10h] (frame 0x48 v 0x44): all follow-ons come from the one inline decision.
  Tried: without _RELEASEDATA (2798, same); without _DTOR_FORCEINLINE+_RELEASEDATA (plain `~CGraphicFrame(){UnInitialise();}`: 2606, the
  destructor goes out of line). Header note (CGraphicFrame.h, _UPLOADFRAME): VC7.1 expands ??_G only at the top level -- so the second release
  is probably reached through one more NON-forced inline layer than the first in the original (not found yet). Probed: Release() or ReleaseData() plain inline (probe/rsm/CGraphicFrame.h.probe): both go out of line everywhere (2606).
## 0x00B751F0 CEngineWaterRenderer::RenderIceForeground -- DIFFER(2722v2722), differ/00b751f0.cpp
- Name: Ego_r 0x00A23180 + FableWin 0x02D4CF10; helpers SetIceForegroundTexturesAndUVModes(bool) (FW 0x02D4D210), SetupIceForegroundShaderConstantsAndFog
  (FW 0x02D4D690; landed out-of-line copy 0x00B7F3B0, same text), GetIceForeground*VertexShader (Ego_r/FW, out of line 0x00B6DF50/90/D0).
- Re-authored on shared headers (same idioms as row 1; shadow params = FableWin ctor CColorBufferDepthPackParameters(4,5,3) from
  FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS; lights through CEngineLightingManager _MEMBERS). All instructions match except the
  bookmark `test`/`or` operand order in the six SetTextureUVModeU/V SetRenderState expansions (sites 0x00B754D5, 0x00B75542, 0x00B755AF,
  0x00B7561C, 0x00B75689 + one more); the [4]/[8] lines are masked D3DTextureUVModeTable+4/+8 relocations (fine).
- Measured (all FITTED probes, none kept): SETRENDERSTATE_DESIRED / _BOOKMARK / ADDSTATE_DIRTY alone and combined: no change at k=0.
  Dead FABLE_RETAIL_ASSERT count sweep k=0..90 (cond IceBumpMap / this): 14 hunks k<8, 12 @8-10, 11 @11-12, 9 @13-15, 7 @16-21 (sites 3-6
  fixed, sites 1-2 still wrong and the SetSpecularEnable site 0x00B759EB load order breaks), then 13..22 rising. Assert POSITION is
  irrelevant (start / between sites / after the loop all give 7 at k=16). Opt-ins shift the window by ~3 asserts. No count closes all 7.
  => the residue is not a pure whole-body count; something per-site (sites 1-2 = stage 0) differs. SetupFresnelPixelShaderConstants helper
  (FW name, listed after SetupIce..): code-identical, no change.
- fwsize (/EHsc): Render 160i v FW 166i, SetIce.. 102i v 242i (+140 ~ 2 asserts), SetupIce.. 103i v 95i, ThemeChannelDesc ctor 14i v 13i.

- (successor) Per-expansion dead asserts (FableWin evidence: ?SetRenderState 213i v our text 45i ~ two ASSERT arms, ?AddStateToUpdateList
  108i v 23i ~ one; probe opt-ins in probe/rsm/, NOT kept): every per-expansion assert acts as a PURE SHIFT of the whole-body count by 7
  (one per expansion, 7 expansions): sr=1 k = sr=0 k+7, aa=1 k = aa=0 k+7. Body count sweep k=0..20 (with 0..2 per-expansion) gives the
  wrong-site sets in order {123456} {12345} {1234S} {123S} {12S} {16S} {456S} {23456S} {123456S} (S = SetSpecularEnable 0x00B759EB): each site
  is right in a window of ~10 asserts and the windows are staggered ~2.6 asserts per site (S..1 ~ 20) -> NO total count fits all 7.
  The residue is structural: per-site record spacing (records per SetTextureUVMode expansion, or records between site 6 and S) differs.
  Also tried: new probe opt-in SETRENDERSTATE_CURRENT_FIRST (`(CurrentBookmarkMask & state.BookmarkMask)`, the landed twin 0x00B77E20's
  SetState text): code-identical. Overlay CRenderStateManager.h removed again (no change kept).
## 0x00BE0EB0 CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive -- DIFFER(2889v2889), differ/00be0eb0.cpp
- UPDATE (later in the run): camera ported to the shared engine/CEngineCamera.h (FABLE_RETAIL_CENGINECOMPONENT + _CENGINECAMERA_POSITION +
  _SCREENTOWORLD, NEngineInternal::GPCamera 0x01436EA0) with the repo opt-in FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS (exactly the draft's
  fitted X-row parenthesisation) and the scale through FableWin ?TransformDistScreenToViewHorizontal (float scale = 100.0f; ...). FableWin
  evidence: TransformViewToWorld 12i = one call (draft's element-wise local text 64i), ScreenXYLinearZToView 27i = one expression (draft's
  split 31i). That fixes the early `mov ebx,1`. Residue with 10 FITTED dead asserts (k window 10..12): x87 product order 0x00BE0F02 (retail
  (X-WCX)*Z*1/hw) and 0x00BE0FA0 (retail z*1/hw*100/hvs; ours 1/hw/hvs*z*100: c2 reassociates), test/or site 0x00BE1620. Without the asserts
  20 fg hunks. Source-order opt-ins for both camera texts (z first) are code-identical (c2 reassociates by record key): tried and removed.
  The particle-group classes remain local models (landed sibling 0x00BBC460 idiom); CEngineInternalPrimitive2DParticleGroup has no header.
- (first pass) the earlier draft as-is:
- Earlier draft compiles unchanged against today's headers (local layout models via the #define-rename trick, the same idiom as the LANDED
  sibling CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive 0x00BBC460). Residue: `mov ebx,1` one x87 instruction early
  (0x00BE0FE4, inside the PostScale run) and the bookmark test/or operand order at 0x00BE1620 and 0x00BE16C6 (opposite senses).
- Dead-assert count sweep k=0..30: k=10..12 fixes 0x00BE16C6 only; 0x00BE1620 and the mov ebx,1 stay. DESIRED / DIRTY opt-ins: same curve.
- Not done: port off the local models onto CEngineCamera SCREENTOWORLD / CEngineInternalPrimitiveParticleGroup headers (would be needed
  anyway for strict shared-header mode; a new header for CEngineInternalPrimitive2DParticleGroup is required).

- (successor) FableWin size evidence for the ten dead asserts: fwsize FW_EXTRA=/EHsc on this source = 394i v FableWin
  ?RenderSubPrimitive@CEnginePrimitiveManager2DParticleGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z 400i (d +6); without them 214i
  (each dead arm 18i at /Od; 9 -> 376i, 11 -> 412i). So k=10 is the size-closest count (still INFERRED conditions; cond `group`/`pass`/`this`/
  `sub.Parent`/`group != 0` all give the same 13 hunks at k=10). fwsize: TransformScreenXYLinearZToWorld is NOT in FableWin -> differ now calls
  TransformScreenXYLinearZToView + TransformViewToWorld (both SAME 27i / 12i; code-identical). Also tried, no change: depth before pos, camera
  reference first, GPCamera-> direct, depth re-read after the transform, TransformDist(100.0f, scale=depth) role swap, probe camera texts
  (Y row first: worse 26; X `(X-WCX)*Z*1/hw`: identical; local z: 15).
## 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter -- DIFFER(2923v2916), differ/00b5a3f0.cpp
- Earlier draft (already on shared headers) compiles unchanged; + existing overlay opt-in FABLE_RETAIL_C4DVECTOR_INITIALISEFROMCOLOUR
  (FableWin ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z 12i/28b = one call; code-identical here).
- Residue: third SetPixelShaderConstant (index 7) 0x00B5A68C: retail `or dword ptr [eax+3D8h],ecx` (and no early load of inverse.X
  until after `add edx,80h`); ours `mov edi,[eax+3D8h] / mov esi,[esp+50h] / or edi,ecx / mov [..],edi` (+7 bytes, then the loop
  alignment nop at 0x00B5ACBF disappears). Sites 1-2 are load/or/store with prefetch in retail too; site 4 is RMW in both.
- Tried: dead-assert counts 0..30 (no change), inverse as a call-site temporary (20 hunks), declared after grey (27), ApplyVertexShader
  first (31), inverse = C4DVector(1,1,1,1) - grey (default 7 / NAMED_ADDSUB 7), C4DVECTOR_NAMED_RESULT (same 2923).
  New probe opt-in CRGBFLOATCOLOUR_SCALE_NAMED_RESULT (FW ??DCRGBFloatColour@@QBE?AV0@M@Z 38i/101b; probe 39i/103b) bloats to 3060:
  removed again (overlay CRGBFloatColour.h identical to agentC22's).

## 0x00BB2540 CEngineClothRenderer::RenderPolygons -- DIFFER(2726v2905), differ/00bb2540.cpp (earlier draft, regraded only)
- Name: Ego_r/FableWin ?RenderPolygons@CEngineClothRenderer@@AAEX...; declared by the overlay's FABLE_RETAIL_CENGINECLOTHRENDERER_RENDER.
- Regraded unchanged: 93 frame-masked hunks, 179 bytes short. Not ported this run (breadth first). Port plan: shared CEngineClothRenderer
  (_MEMBERS/_RENDER), CParticleProgram (_RENDERACCESS), CParticleSimulation, a new header for CEngineParticleSimulationRenderData (Ego_r
  ??0 0x00A6C310 / IsBusy / ??_G; retail ctor 0x00BB63A0, size 0x70), GPPrimitiveRendererSharedData 0x01436E78 with the overlay's
  _DYNAMICVB members. Retail facts seen: init loop counter in the dead argument slot [esp+0x84], loop walks the new pointer (+4, stride 0x34),
  failure exit 0x00BB2E9E; vertex loop unrolled by 4 (`cmp [esp+7Ch],4` at 0x00BB2730).

## 0x00BAA870 CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList -- DIFFER(4575v4600), differ/00baa870.cpp (successor)
- Ported the acctB draft onto today's headers by re-applying the stale opt-ins as NEW overlay blocks (agentC22's probe copies, merged by hand):
  CEngineCamera _ZSPRITE_ACCESSORS, CMatrix3x4 _POSTSCALE_XYZ, CPreTransposedBoneMatrix _SET, CEngineLocalDetailGenerator _ZSPRITE (default pad kept
  in #else), CEnginePrimitiveRendererSharedData _ZSPRITE (#ifdef before _MIPMAPSIZEMULTIPLIER, superset incl. MipmapSizeMultiplier +0xb0),
  CEngineInternalPrimitiveRepeatedZSprite _TYPED (the __vftable assert wrapped #ifndef _TYPED), CEnginePrimitiveManagerRepeatedZSprites _TYPED.
  Stale C3DMESHINFO_LODS is today's repo opt-in FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL (same text); CSCOPEDSHADERSENABLE_FORCEINLINE_DTOR is today's
  FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE. Grade reproduces the acctB best: DIFFER(4575v4600).
- Evidence-based fixes: the two faded-shadow sets are the PDB body (repo opt-ins _SETFADED_SHADOWGUARD + _SHADOWGUARD_DIRECT, call with `true`;
  retail 0x00BAA951 `cmp [ecx+4694h],esi ; mov al,1 ; je ; xor al,al ; cmp al,[ecx+0CEh]`) instead of the draft's hand-written guard +
  stale ENABLE_FIRST opt-in (fixes 0x00BAA95D / 0x00BAAA4B). FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB (FableWin ??G 34i): 45 -> 39 frame-masked hunks.
- Probed, not kept: new overlay opt-ins C3DVECTOR_INITIALISE_ADD / _SCALE / _DIV (FableWin 34i/31i/35i; our texts 34i/31i/34i at /Od): each
  alone or together worse (44..58 hunks); SCALE_NAMED_RESULT worse.
- Remaining residue (address order): PostScale x column keeps x (retail `fld [E41] ; fmul st(1)` + `fstp st(0)`, ours `fmul [E41]` consumes);
  rep movsd of the inverse copy position; frame-slot ORDER (ours sphere radius at [esp+0x90] v retail [esp+0x84] -> disp8/disp32 sizes, +0xc
  bytes before 0x00BAAB36); bbox copy order 0x00BAB045; SetRenderState sites 0x00BAB67E..0x00BAB75C (load order); 0x00BAB9FE.. pads.
  The acctB residue list (ref/00baa870/notes/NOTES.md "Residue list") still applies item by item.
- Globals respelled to the lane NEngineInternal::GP* convention (GPPrimitiveRenderer 0x01436E80, GPCamera 0x01436EA0, GPOcclusionManager
  0x01436E5C; code-identical, 39 hunks). Still the draft spellings (no lane GP* name found): g_pLocalDetailGenerator 0x01436E88,
  g_pEngineBillboardGenerator 0x01436E4C. Frame-slot residue detail: retail control0 at [esp+88h..90h] (disp32) where ours sits at
  [esp+78h..80h] (disp8): the length deficit (-25) is mostly encoding size from slot ORDER (acctB residue 9).

## Header overlay changes this run
- engine/CEngineWaterRenderer.h: NEW FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS (decls only). Asserts there are all _AT-pinned, none moved
  semantically. No other overlay header changed (CRGBFloatColour.h / CEngineCamera.h probe opt-ins were removed; both files are byte-identical
  to agentC22's again). Rows of other batches that include CEngineWaterRenderer.h are unaffected (new #ifdef only).
- (late) FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX (FableWin SetPixelShaderConstant 28i = the CArray subscript; fwsize showed ours 29i)
  added to differ/00b5a3f0.cpp and differ/00b751f0.cpp: code-identical there (same grades). In 0x00B77370 it keeps 2798 but scatters the
  register residue (64 fg hunks v 51), so that differ stays without it -- one more sign row 1's residue is the inline split, not text.
- (successor) Overlay additions, all additive under NEW opt-ins, listed in hdr/README.md: CEngineCamera _ZSPRITE_ACCESSORS, CMatrix3x4 _POSTSCALE_XYZ,
  CPreTransposedBoneMatrix _SET (new copy), CEngineLocalDetailGenerator _ZSPRITE (new copy), CEnginePrimitiveRendererSharedData _ZSPRITE,
  CEngineInternalPrimitiveRepeatedZSprite _TYPED (new copy), CEnginePrimitiveManagerRepeatedZSprites _TYPED (new copy). Asserts pinned (_AT).
  All five differ sources regraded after the overlay edits: unchanged grades (2798 / 2722 / 2889 / 2923 / 4575). No row of this batch matched,
  so nothing in payload.json / oracle.tsv (files not created).
