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
- (wfA4 opus-5.5 pass, 2026-11-15, ~25 grades) Inline controls measured: (1) scoped #pragma inline_depth(N) round "CGraphicFrame surf;":
  N=0/1: ctor+dtor (FORCEINLINE) called out of line (3006); N>=2: no change. Round "return true;" N=0/1: both ~CGraphicFrame called (2673);
  N=2..4 no change. => a region only limits calls in ITS OWN statements; ??_G sits in Release's header body (depth 5 from the row:
  helper 1, ~CGraphicFrame 2, ReleaseData 3, Release 4, ??_G 5). Global #pragma inline_depth(N) before the includes: N=4 calls all
  three final ??_G, N=5 calls ~CGraphicFrameData, N=6 calls ~CCountedPointer<CTexture>, N>=7 = default. No depth splits the two sites.
  (2) /d2inlT cannot (all 11 ??_G sites sc 60). (3) caller size: trace g (global growth counter 0x10882B58) = 4821 at the edge release,
  5049 at the surf release, a(??_G)=29; flip needs a+g>9000. Dead FABLE_RETAIL_ASSERTs before "return true;": k=930..980 reproduce
  retail's split exactly (edge expanded, surf push 1; call ??_G), but ~4 tuples/assert also re-colours the whole row (2694v2734,
  102 structural hunks from the prologue). Not a credible device (950 asserts) and not closing. (4) explicit surf.UnInitialise()
  (alone / both / edge in an inner block): UnInitialise itself goes out of line (2814..2816).
  Also seen: retail has 6 BeginLoading calls (3 per GetFrame expansion), ours 5 (2nd GetFrame's failure-path BL merged): a
  cross-jump that follows the register state, probably a follow-on of the release decision.
  Open lead: retail g at the edge release must be ~8780..8970 (ours 4821): something ~4000 tuples bigger BEFORE the releases in the
  original (FableWin SetSeaReflectiveForegroundTexturesAndUVModes is 269i /Od v ours 125i: +144i of unknown source text).
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
- (wfA4 opus-5.5 pass, 2026-11-15, ~75 grades) New devices measured: k=1..7 unused named locals (top): no change. Dead asserts
  ONE PER UV-MODE CALL inside SetIceForegroundTexturesAndUVModes (FableWin helper 242i v ours 102i, +140i ~ 6-8 asserts: the
  per-site hypothesis), n=1..6 per site (cond `this`; cond `shadows` extends its liveness and changes the prologue, +15 bytes):
  n=3 -> only sites 1-2 + SetSpecularEnable 0x00B759EB wrong (5 real hunks), n=4 -> sites 4-6 + S. Grid per-site n=2/3 x top
  k=1..10: same staggered sets, best 5 real (sites 1-2 + S). Asserts placed AFTER the last UV call still flip the earlier sites
  (so the decision is not "records before the site"), and asserts before the capture block = at the helper start (identical
  results): the order depends on the total count and on the spacing between sites only. S is right only at totals <= ~6.
  Verdict unchanged: structural; no count / distribution found.
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
- (wfA4 opus-5.5 pass, 2026-11-15, ~70 grades) 13 -> 4 real hunks (fg with relocation-identical pairs dropped: tools/fgc.sh).
  * 0x00BE0FA0 FIXED: new overlay opt-in FABLE_RETAIL_CENGINECAMERA_DISTH_NAMED = the repo's _MESH2D_ORIGINAL text of
    TransformDistScreenToViewHorizontal (const float scaled = 1/hw*distance*z; distance = scaled/hvs) on the FORCEINLINE branch
    (_MESH2D_ORIGINAL itself makes the camera helpers plain inline -> TransformViewToWorld out of line, 2777). FITTED v FableWin size
    (this text 18i /Od v FableWin 16i; x87 helper sizes are unreliable per WF_RULES).
  * 0x00BE0F02 FIXED: new overlay opt-in FABLE_RETAIL_CENGINECAMERA_SXY_XNAMED (X row: const float x=(X-WCX)*Z; X=x*1/hw; Y row
    unchanged). Source order of the one-expression text is irrelevant (p1..p3 identical: c2 sorts by record key); only a named
    intermediate pins it. FITTED (29i v FableWin 27i).
  * Left: `mov ebx,1` (the CSE'd true constant) scheduled two x87 instructions early in the PostScale run (0x00BE0FDE v 0x00BE0FE4);
    bookmark test/or at 0x00BE1620 (SetRenderState in the `if (WriteZBuffer)` arm). Dead-assert count sweep 0..24 on the new
    baseline: 0x00BE1620 wrong at every count (11/12 best: only it + mov ebx); second-position asserts (after CurrentBSRadius) 1..8,
    k=1..7 unused locals (top / after depth / inside the arm): no fix (k>=4/5 breaks x87 sites). if (!W) arm swap: 2601 (layout).
    pos / depth / camera spellings (copy-ctor, assign, depth from group, camera ref first): worse or equal.
  * Before landing: CEnginePrimitiveManager2DParticleGroup HAS a repo header (engine/CEnginePrimitiveManager2DParticleGroup.h,
    _MANAGER opt-in) and CEngineInternalPrimitiveParticleGroup.h exists: the local models / #define rename must be ported
    (strict-header rule); CEngineInternalPrimitive2DParticleGroup needs a new overlay header.
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

- (wfA4 opus-5.5 pass, 2026-11-15, ~15 grades) MEASURED CAUSE: a diagnostic `ProbeExpose(&inverse)` call (not kept) makes site 3
  `or [eax+3D8h],ecx` exactly as retail (the inverse.X load can no longer be hoisted above the UpdateFlags store), exposing `offset`
  does not. => in retail `inverse` is address-exposed (or otherwise may-alias the store); ours is not. Spellings tried for a natural
  exposure, none moves it: k=1..7 unused locals (no change at all), inverse.Initialise(..), inverse = inverse - grey,
  inverse = C4DVector(..), const C4DVector& inverse = C4DVector(..) / = C4DVector(1,1,1,1) - grey, const C4DVector inverse.
  The diagnostic exposure also moves inverse's frame slot (to [esp+30h]), which retail does not: the real mechanism keeps the slot.
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
- (wfA4 successor, 2026-11-15) RELOCATION_MATCH reached with local models still in place (probe/be/sw/b09.cpp = differ/00be0eb0.cpp):
  TransformPoint through the repo opt-in FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR (instead of _SUMS; mov ebx 2 -> 1 slot), camera
  helpers called as `NEngineInternal::GPCamera->...` with NO `camera` reference local (fixes the X-row product order 0x00BE0F02 without
  SXY_XNAMED and puts mov ebx at 0x00BE0FE4), DISTH_NAMED kept (FITTED), dead asserts 7..9 match (0..6: 0x00BE16C6; 10..12: 0x00BE1620).
  Measured: x87 parenthesisation shifts the integer-instruction schedule one slot per paren level (named == parenthesised).
  Next: port the local class models to shared headers (strict-header rule) and re-sweep the assert count.
- (wfA4 successor) MATCHED: src/00be0eb0.cpp RELOCATION_MATCH + test PASS + addmatch + read-only vland PASS (WARNs only: RELOC_NAME
  GPCamera v label g_pEngineCamera, D3DTextureUVModeTable v g_textureUVModeToD3D; RELOC_CONFLICT D3DAlphaFuncModeTable 0x01396DD8 here v
  0x01396DE8 in landed 0x00BBC460 -- an addend difference of the same table, coordinator to check the sibling's spelling).
  Ported off every local model: shared CEngineSubPrimitiveBase.h (_PARENTBASE: sub.GetParentBase()), CEngineInternalPrimitiveParticleGroup.h
  (STLport list: const_iterator loop + (CEngineInternalPrimitiveBase&)it->GetData(), the landed MeshGroup idiom), new opt-ins
  CEnginePrimitiveManagerParticleGroup _RENDER / CEnginePrimitiveManager2DParticleGroup _RENDER, new header CEngineInternalPrimitive2DParticleGroup.h.
  The port moved the dead-assert window from 7..9 to 6..8; 8 chosen (closest to FableWin's 400i debug size inside the window).
  Overlay opt-in FABLE_RETAIL_CENGINECAMERA_SXY_XNAMED removed (unused now).
- (wfA4 successor, 0x00B5A3F0, ~14 grades) No change from: engine/GFLimit.h instead of the in-row GFLimit copy (kept: identity), contrast
  through GFLimit (worse, 7), direct-init scale/offset, named CRGBFloatColour temps, C4DVector(inverse) copy at the call, inner block
  round offset..inverse, PSCONST_INDEX off (default begin()+index), CRGBFLOATCOLOUR_CTOR4_INITIALISE, globals respelled to the lane
  names NEngineInternal::GPDisplayManager (0x01436E1C) / GPCamera (0x01436EA0) (kept: identity, code-identical), CTexture* backBuffer.
  fwsize: ??DCRGBFloatColour (operator*(float)) FW 38i/101b v ours 30i (named result 39i/103b; earlier agent: named result bloats the row).
- (wfA4 successor, 0x00B751F0, ~16 grades) differ now uses the PDB helper GFLimit (FableWin ??$GFLimit@MMM@@YIXAAMABM1@Z; engine/GFLimit.h)
  for the fresnel clamp instead of the in-row `Clamp` (byte-neutral). fwsize (/EHsc): FableWin's inline callees carry debug asserts
  (PeekGraphicBank +60i, PeekGraphic +149i, PeekGraphicInfoFrame +83i, both GetTexture +82/+87i, ~CGraphicFrameData +75i, helper +140i).
  Placed dead asserts there (in-row PeekGraphic n=0..3 x PeekGraphicInfoFrame m=0..3; 2 helper asserts at 10 position pairs): the same
  staggered total-count pattern as before (sites fixed from the last one backwards, specular 0x00B759EB breaks at 3+). Not closing.
- (wfA4 successor, 0x00B77370) /d2inlS2/4/5/6: byte-identical (65 hunks) -- the caller-size cap a+g>9000 is not the /d2inlS knob here.
- (wfA4 successor, 0x00BAA870) retail holds GPCamera in ebx from 0x00BAAAA1 (the `camera` reference local is right); GPCamera-> direct
  is code-identical; 19 real hunks by tools/fgc.sh. Not worked further.

## 0x00B5A3F0 -- MATCHED (wfA4 opus pass, 2026-11-15): src/00b5a3f0.cpp + test/00b5a3f0.cpp, RELOCATION_MATCH, test PASS, vland PASS
- Closer: `C4DVector offset = C4DVector(Settings.Tint * (brightness + 0.5f));` instead of `brightness += 0.5f;` + `Tint * brightness`.
  Same bytes everywhere else; the site-3 `UpdateFlags |= 2` (0x00B5A68C) becomes retail's RMW. Mechanism: the flags temps (c2 ids
  114..117, 3 refs each) are NOT global candidates (c2prio shows them dropped), so their registers come from the local allocator over
  the whole block from the brightness clamp join (0x00B5A56A) to the alpha clamp (calls are not block ends); one tuple less (no
  store back to `brightness`) changes that block's local picks: site 3 temp no longer edi, the inverse.X prefetch cannot hoist, peephole fuses.
- The "address-exposed inverse" theory of the earlier pass was wrong: exposure moves inverse to a private slot (+0x10 frame), retail
  shares inverse's slot with the Tint*contrast temporary, so retail's inverse is NOT exposed.
- vland needed `// Retail-asm:` (0x00B5A70F/0x00B5A713 fld/fistp of GFFloatToLongNear) or the row reads NOT_GENUINE.
- Names: unchanged from the earlier draft (Ego_r/FableWin ?ApplyColourFilter@CEngineScreenEffectColourFilter@@AAEXXZ; lane global names as landed rows).

## 0x00B77370 -- wfA4 opus pass (2026-11-15, ~75 grades + 3 inline traces + 1 c2prio): near_miss DIFFER(2721v2734), 30 hunks, differ/00b77370.cpp
- Measured the inline rule in c2.dll: 0x1078D1E5 `mov ecx,[0x10882B58] (g) ; add ecx,a ; cmp ecx,2328h (9000, hard-coded immediate, no switch)`;
  past it a caller-size penalty pen ~ (a+g)/216 is added to the score (landed twin CreateDynamicTextures 0x00B72180: its final 10 ??_G sites
  have pen 42..55, sc 120..138 > T 74 = called; ours sc 60). g is per function (a copy of the row defined before/after it in the TU: no change).
  /d2inlcrmax<N> exists in c2 but is not this limit (no effect). Our final releases sit at g 5305 / 5597; retail needs g in ~[8680, 8970] at the first.
- Device found: N dead `FABLE_RETAIL_ASSERT(index < GraphicList.size());` in the in-row PeekGraphic (4 expansions; ~53 g per dead expansion because
  the inline size() inside the dead arm is expanded before the arm is deleted). N=18 gives retail's split exactly (17: none called, 19: both).
  Asserts with no inline call in the condition add ~nothing to g; asserts in PeekGraphicInfoFrame with `PeekGraphic(graphic) != 0` overshoot
  non-linearly (nested inline decisions). FableWin evidence for asserts: PeekGraphic +149i (~2), so 18 is FITTED (device d, count stated).
- Correction of an earlier note: in retail ebx holds the SECOND-fetched (edge) frame data (0x00B77660 is inside the 2nd GetFrame) and stage 3
  takes [esp+10] = surf, so the draft's AttachTextureToStage(3, surf.GetTexture()) and declaration order (surf, then edge) are RIGHT
  (edge-texture / edge-first variants regraded worse once the split is in).
- Residue with the split: (1) the 2nd GetFrame's new-CFrameData path: ours tail-merges its BeginLoading with the failure path's (5 BL v retail 6);
  retail keeps them apart because its const-0 piece there is in ebp (`xor ebp,ebp` 0x00B77693 / 0x00B776EF; `push ebp ; push ebp`), ours
  in esi (c2prio: const 0 split esi #2, esi #172, 6 immediate pieces; this = ebp whole function). (2) SetRenderState bookmark `mov eax,[eax+2814h]`
  position and test/or order at 0x00B77829..0x00B77A50 (record counts). Neutral: k=1..7 unused locals, 1..10 trivial asserts at the row start,
  the CreateDynamicTextures frame opt-in set, GETFRAME_EARLYRETURN, helpers plain inline (go out of line).
## 0x00B751F0 -- wfA4 opus pass: N dead PeekGraphic asserts (the B77370 device) give the same best family: N=5 -> 7 hunks (5 real: sites 1-2 + S
  0x00B759EB); grid N=4..6 x 0..22 body asserts: no better. Not closing; parked again.

## 0x00BAA870 -- opus pass 2 (2026-11-15): DIFFER(4600v4600), differ/00baa870.cpp (previous best kept as differ/00baa870_wC.cpp twin)
- Closer of the length / frame residue (EVIDENCED): retail copies the camera window BY VALUE. efm.py frame map of the old draft put retail's
  window_right home at frame -180 = sphere+8 (the BRX field of a 16-byte object sharing the sphere slot once the sphere is dead), i.e.
  `C2DBoxF window = GPCamera->PeekCameraState().Window;` (member-wise C2DBOXF_COPY: only BRX gets a frame home, the rest stay on x87)
  and the constant 0xb from window.GetWidth() / GetHeight() / BRX+TLX / BRY+TLY. Result: 4600v4600 and the efm frame map has NO differing
  symbol (was: sphere/other_side, shaders/layout, query/side_tex/top_tex, control_tex/other_side_tex all swapped -> disp8/disp32 = -25 bytes).
  Implicit copy (no C2DBOXF_COPY) = 4616 (block copy). PeekWindow() accessor: identical bytes.
- NOTE sd.py masks frame displacements, so it also hides operand-order flips between two frame operands; tools/rh.py (frame UNmasked,
  relocations + call / jump targets masked) and tools/sites.py (per-site retail sequence present?) are the measures now (80 real hunks).
- Remaining residue, address order: PostScale E41 keep (0x00BAAB75) + rep movsd / mov edx,17Ch positions; k fst/fstp (0x00BAACA0);
  centre z add order (0x00BAACBC); then a UNIFORM product-load-order flip in all texture-space transforms (bottom_tex 0x00BAAD4B, top_tex
  0x00BAADA9.., side_tex 0x00BAAE00.., other_side_tex 0x00BAAE77..): retail loads the matrix element first (`fld E3k ; fmul v`), ours the
  vector component; term orders of the TransformPoint rows also differ. Textual operand order has no effect (record-keyed). Tried without
  effect on those sites: dead-local rulers `float u = sprite.MinDistance;` k=1..256 at entry (moves other sites only: osd, c34/c45, rs),
  zero-init / reference rulers (no effect at all), CMATRIX3X4_DATA_UNION, top declared before texture_transform, INITIALISE_ADD/SCALE/DIV
  operator texts (SCALE alone: 4590, 69 hunks; DIV: 4619+), TransformVector+SetTranslateOnly for the E4k rows (+48 bytes: xx copies spill),
  two-arg TransformPoint (SOURCE_DEST_SUMS) for bottom/top_tex (+48 with the above). Next idea: find which records make E-fields "late"
  in retail (an inline helper that reads the matrix through `this` after the vectors are named), or trace c2 with c2dbg.

## 0x00BB2540 CEngineClothRenderer::RenderPolygons -- MATCHED (wfA4 opus pass 2, 2026-11-15): src/00bb2540.cpp + test/00bb2540.cpp
- RELOCATION_MATCH 2905v2905, vt.py PASS, read-only vland PASS (one RELOC_NAME warning: GPLightingManager labelled g_pEngineLightingManager).
  No flags, no devices. ~25 grades.
- Re-authored the old local-model draft (2726v2905) on shared headers; the steps that closed it, in order:
  (1) port: CEngineClothRenderer _MEMBERS/_RENDER, CParticleProgram _RENDERACCESS + NEW _TEXCOORDACCESS, CParticleSimulation NEW
      _MUTABLE_RENDERDATA, NEW header CEngineParticleSimulationRenderData.h, shared data _MEMBERS + _DYNAMICVB with the FableWin
      GetDynamicVB* and NEW GetDynamicIB* accessors, CTVertexNormTex1 vertices (FVF 0x112) via CGenericVertex SetPosition / SetNormal
      (_INLINE_SETTERS) + NEW _INLINE_SETU1V1, corner accessors GetA..GetD -> 2838v2905;
  (2) EVIDENCED vertex loop: both render-vertex indices read into named locals first (retail 0x00BB2770 spills the texture index), a
      local `positions = PositionBuffer` hoisted (retail edx), VertexNormals re-read per vertex, ToC3DVector -> 2889v2905, 35 hunks;
  (3) Lock(flags, 0, 0) for both buffers (retail pushes flags last); positions declared between PeekRenderVertices and
      PeekIndexedTextureCoords (FITTED: keeps `this` in ebp) -> 26 hunks;
  (4) winding as ONE 2-byte object `unsigned char winding[2] = { 0, 1 }` (retail packs both bytes into the dead render_tris argument slot
      [esp+0x84]/[esp+0x85]; two named bytes take two slots) swapped with FableWin's SWAP<unsigned char> -> 2905v2905, 16 hunks;
  (5) quad loop indexes quads[q] (FITTED: retail biases the quad pointer by +2, 0x00BB2CE9) -> 8 hunks; SWAP before the base update
      (FITTED: retail reads winding[0] before the add) -> RELOCATION_MATCH.
- Lesson: two byte locals that retail stores at adjacent bytes of ONE frame / argument slot are one 2-byte object (array or struct).
- (opus pass 2, later) differ/00baa870.cpp now also spells the box extents with the FableWin ?GetDimentions@C3DBoundingBox (NEW overlay
  opt-in FABLE_RETAIL_C3DBOUNDINGBOX_GETDIMENTIONS, fwsize 15i/35b = FableWin SAME): code-identical (80 real hunks), more natural.
  fwsize of every matrix helper the row expands = FableWin (PostScale 67i, PostTranslate 22i, one-arg TransformPoint 61i); off: Normalise
  47i v 48i (no Normalise opt-in gives 48i), GFFastOneOverSqrt 29i v 30i (x87, unreliable), GetWidth/GetHeight 11i v 13i (EXTENT_NAMED
  12i: 81 hunks), GetCenter 20i v 22i (PROBE1 `Min + GetDimentions()*0.5f` = 22i but +96 bytes in the row: not it).
- More tries on the uniform E-first product order (none moved top/side/bot sites): two-arg TransformPoint for all four tex transforms
  (SOURCE_DEST_SUMS = FableWin 61i text: 86 hunks; COPY_SUMS: 73 hunks; default text is called out of line), bottom/top only (81/87),
  dead float rulers after the E4k rows k=8..224 (61..81 hunks, other sites only), dead FABLE_RETAIL_ASSERT(sphere.Radius > 0.0f) x1..16
  there (61..71), CENGINEFADEDISTANCE_INLINE_CTOR off (identical). Conclusion: the E-vs-vector load order in the four texture-space
  transforms is NOT a record-count tie (no ruler / assert count at any position flips it); something structural about how retail reaches
  texture_transform there is still unknown. Park reason: ~95 grades this pass, no measured progress after the frame fix.
