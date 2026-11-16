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

## 0x00BAA870 -- opus pass 3 (successor, 2026-11-15): 80 -> 42 real hunks (rh.py), DIFFER(4600v4600); differ/00baa870_p5.cpp
- MEASURED with wfB6/E10 tools/lk (c2 operand-sort comparator log, copied to tools/lk): the E-first / vector-first product order is
  keyed on c2 record ids (key = id*32 mod 65536, higher first). Our texture_transform fields had main-body records (ids 16..31,
  224..235) because the E4k rows were hand-written; +4/+8 fields first touched through an inline reference/`this` get LATE-pass ids
  (1865..2031 here; retail's wrap past 2048 -> lowest keys). Retail: E11 (+0) normal id, other E fields late -> E-first everywhere
  except the x terms (vector first), exactly retail's pattern.
- Closer (EVIDENCED, sibling idiom RenderBillboard 0x00B8B780 wfB4 src: view copy + SetTranslateOnly(0,0,0) + matrix helpers with the
  zero terms folded by VC7.1): `texture_transform.SetTranslateOnly(0.0f, 0.0f, 0.0f); texture_transform.PreTranslate(-sphere.Center);`
  (NEW overlay opt-in FABLE_RETAIL_CMATRIX3X4_PRETRANSLATE, FableWin ?PreTranslate@CMatrix3x4@@QAEXABVC3DVector@@@Z 165b/61i; both
  texts _ACCUMULATE / default give identical bytes) + all four texture-space points through the two-arg
  TransformPoint(source, destination) (FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS, FableWin 164b/61i).
  Also added (unused now): FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST (FableWin 146b/55i two-arg TransformVector): with
  SetTranslateOnly(translation) it leaves a frame temp (112 hunks) -- not retail.
- Record wrap: 5..8 dead asserts at entry (device d) push the late-pass records past 2048 (sites osd/csf/cosf/sideZ need it): 42 hunks.
  Without asserts 65. Asserts are a ruler here (count to be minimised / placed after the structure is final).

## 0x00B77370 -- wfA4 successor-3 pass (2026-11-15, ~170 grades + 3 comparator traces + 2 c2prio traces): DIFFER(2721v2734), 33 rh / 27 sd hunks (was 35/28), differ/00b77370.cpp
- NEW best: `CGraphicFrame edge_alpha;` declared right after `CGraphicFrame surf;` at the helper top (FITTED; behaviour identical: the empty
  frame's release is a no-op on the first-fetch failure path). Fixes the x87 product order of Settings.BumpMatFactor * WaterBumpFactor at
  0x00B77AEE and 0x00B77BE2 (retail `fld [ebp+2DCh] ; fmul [ebp+5C0h]`; textual operand order alone is neutral) and the test/or of bookmark site 3
  (0x00B77903); moves the ??_G split window from 18 to 16..17 dead PeekGraphic asserts (16 kept: smallest). Other declaration orders (maps hoisted,
  edge before surf, maps before frames, `CGraphicFrame surf, edge_alpha;`): equal or worse.
- Names: the made-up local `Clamp` replaced by the PDB GFLimit<float,float,float> (engine/GFLimit.h, FableWin ??$GFLimit@MMM@@YIXAAMABM1@Z) --
  byte-neutral; siblings' FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX added (neutral). Previous best kept as differ/00b77370_prevClamp.cpp.
- Remaining residue (address order): (1) 2nd GetFrame BL cross-jump (0x00B7768A..0x00B776E3, 7 hunks, the 13-byte length gap); (2) bookmark load
  order at all six SetTextureUVMode sites + test/or at sites 4-6 (0x00B77826..0x00B77A54); (3) ApplyVertexShader / ApplyPixelShader address temps
  (retail lea eax / lea ecx, ours lea edx / lea eax; 0x00B77B19).
- (1) measured with c2prio --trace --blocks: pre-RA block order is [failure BL (blk 50)] [new-data path (51-58)] [surf release join (59-66)], i.e.
  the same as retail; the difference is the const-0 split: ours piece #172 (allowed ...si.p -> esi) covers only the join/release (esi live in
  51-58), the new path's zeros stay immediate pieces (#183/#28/#86, spill 0), so after allocation the two `push 0 ; push 0 ; call BL` tails are
  identical and c2 cross-jumps them. Retail's piece spans blocks 51-66 (esi excluded -> ebp), hence `push ebp` and no merge. `this` = ebp prio 8,
  not live in that region (ebp allowed for #172). Not moved by: helper restructurings (nested ifs, bool result, inner block for edge_alpha =
  2734v2734 but 59 hunks, edge released early), `if (!helper()) return;`, map locals named/unnamed/unsigned, TOUCHFRAME / GETBANK_DIRECT /
  CURRENTFRAME_DIRECT GetFrame opt-ins (window moves to 17), assert mixes.
- (2) comparator trace (k/run.sh, copied from wfE6; first-pass AND operand keys, 6 sites): lo keys 0x3c9f.. (step 0x60 = 3 records per setter, free
  list), hi keys 0x7437 / 0x8c57.. (high-water mark); ours lo < hi at all six sites (C first). With n=0 asserts: lo 0x7f9e, hi 0xcf36 (dead asserts
  LOWER the ids: freed records reused); +20 trivial asserts at the row start: +0x2000 on both. Rulers that do NOT move the sites: bare `long x;`
  locals (allocate nothing -- wfB6 lesson), INITIALISED dead locals k=1..16 (`long fidelityN = WaterForegroundPolyCount;`), trivial asserts t=1..28
  at the row start (they do move g: t>=8 flips the split at n=18), the five SetRenderState opt-in combinations, asserts in the helper.
  The UV run is the function's LAST setter run (ids from the free list: wfE6 lesson) -- count rulers cannot reach it; only symbol-population
  changes (like the edge_alpha declaration) moved a site.
- FableWin /Od /EHsc sizes (fwsize): Render 76i v FW 107i (+31: not the loop form -- index loops 77i), SetSea.. helper 125i v 269i (~2 asserts),
  PeekGraphic 399i (16-18 dead asserts) v 170i (~2), GetFrame 202i v 146i (our header text without TOUCHFRAME; the TOUCHFRAME text is FW's).
- Natural asserts tried in the helper (ASSERT(surf.GetTexture()) / ASSERT(edge_alpha.GetTexture()) after each fetch): add ~no g; window unchanged.
## 0x00B751F0 -- wfA4 successor-3 pass (2026-11-15, ~40 grades + 1 comparator trace): still parked, differ unchanged
- Comparator trace: 7 first-pass AND key pairs (6 UV sites + S), load order right everywhere in the no-assert source; the residue is the second-pass
  test/or bit only (sites 1-6 wrong without asserts). Initialised dead locals k=1..16 at the row top: no change at all. TOUCHFRAME (+GETBANK_DIRECT
  +CURRENTFRAME_DIRECT, the matched same-TU twin 0x00B70A90's GetFrame set): identical at t=0; with t asserts the same staggered pattern
  (t=8: 1-5 wrong, 12: 1-3+S, 14: 1-2+S, 16: 1,6,S, 18: 4-6+S) -- shifted, not aligned. `ice_texture` local before / inlined: no fix.
- (pass 3, later) window centre sums as `window.TLX + window.BRX` / `TLY + BRY` (retail adds BRX onto TLX: 0x00BAB332 `fld st(0) ; fadd st(2)`):
  window site fixed (36). FITTED parens `((TLX + BRX) * 0.015625f) + 0.015625` (both centre args) fix the 0xc call ecx load: 33 at 8 asserts.
  Tried, no better: GetMagnitude (any C3DVector text; fixes the half-extent squares 0x00BAB0BF but flips sideZ + one RS site: 39), Normalise
  texts (PAREN_SQUARES_*, LOCALS_XYZ, SQUARE_HELPER, default, GROUPED_SUM, UNPAREN: 43..45), up via one/two-arg TransformVector of (0,1,0)
  (47 best over the full cycle), one-arg TransformPoint on copies for the four tex points (54..), other_side = -side / up = -up (length
  changes), const& sphere (49), control sums swapped (worse), window accessors GetTLX()/GetBRX() (44), k = 1..7 unused float locals x asserts 4..9 (no gain).
- Measured facts for the successor: dead asserts are a GLOBAL id shift (same result wherever placed; period ~158 asserts); relative record
  order is fixed by the spelling only. c2 allocates records in 32-id blocks from a free list (low recycled blocks for the late tex
  destination fields: keys 91..117 here), so the remaining sites need the exact record population, not a count.
  tools: tools/baa_exp.py (patch-file experiments graded over an assert window), tools/baa_grid.py, tools/lk/cmpn.py (named comparator leaves).
- (pass 3, later) `C3DVector bottom(0,0,0);` declared BEFORE `top` (FITTED, record order): 31 hunks at 8 asserts (fixes the bottom Y row
  0x00BAAD75 and the sideY schedule). Combinatorial sweep Normalise text x GetMagnitude x parens x asserts 0..16: nothing below 33 before this.
  Declaration permutations of bottom/top/side/other_side running (w/b8/e7.out).
- (pass 3, end) FINAL best differ/00baa870.cpp = 31 rh real hunks at 8 dead asserts (re-graded from the file). Remaining TRUE residue
  (address order): k fst 0x00BAACA0 (fixed by any non-LOCALS Normalise text, which breaks others: structural, Normalise text unknown);
  control_tex / control1_tex Y-component products 0x00BAAFD3 / 0x00BAAFFB / 0x00BAB048 / 0x00BAB059 (retail: tex+4 before the factor,
  tex+8 after it -- no single wrap explains both side_tex and top_tex, so the tex destination records differ structurally); half-extent
  squares 0x00BAB0BF (GetMagnitude fixes it, flips sideZ + one RS); constants 3-5 schedule 0x00BAB260 / 0x00BAB273; 0xb call ecx
  load 0x00BAB334 v 0x00BAB383; SetRenderState test/or 0x00BAB67E. The rest of rh's list (0x00BAA9BE/D8/F2, 0x00BAAC6E, 0x00BAB11F,
  0x00BAB1C3, 0x00BAB515, 0x00BABA0F/33) are rh.py artefacts (immediates >= 0x400000 masked on one side; relocation addends) and
  0x00BAB93A is alignment padding from the 2-byte offset of earlier hunks. No test written (row not matched).
- Also tried in pass 3 without gain: declaration permutations of bottom/top/side/other_side (23), PostScale(C3DVector), inverse copy /
  one_on_radius moved earlier, control vectors through operator+=, factor declaration order / operand order, C3DVector operator texts
  (INITIALISE_SCALE / SCALE_NAMED_RESULT / SCALE_X_PAREN / INITIALISE_ADD / ADD_XY_GROUP), copy-initialised top/bottom, grouped tex decls.

## 0x00BAA870 -- opus pass 4 (2026-11-15): 31 -> 15 true hunks; differ/00baa870_p6.cpp (work copy, see final entry)
- EVIDENCED by the lk record trace (tools/lk on w/r6/base.cpp): control_tex / control1_tex Y products are keyed on side_factor/top_factor
  (ids 449/450) v the tex .Y records (374..382, created at the distance statements). Retail needs factor keys in (117, 375): the factor
  variables must exist BEFORE the tex .Y reads, i.e. the weights are computed INTO the factor variables and normalised in place:
  `float top_factor = (float)pow(fabsf(bottom_tex.Y - top_tex.Y) * VerticalControlPointWeightingFactor, ControlPointWeightingPower);
   float side_factor = (float)pow(fabsf(side_tex.Y - other_side_tex.Y), ControlPointWeightingPower);
   float one_on_total = 1.0f / (top_factor + side_factor); side_factor *= one_on_total; top_factor *= one_on_total;`
  -> all four control_tex sites fixed at once. The `fst [esp+18]` distance store stays (it is _fabsf's double->float, no named distance).
- half-extent squares fixed by `half_extent.GetMagnitude()` with FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED (repo opt-in, same family
  sibling 0x00BA5260 / agentC2 0x00BB3E80): needs a re-sweep of the ruler (5 asserts + 3 dead `float unusedN = sprite.MinDistance;`).
- (pass 4) FINAL differ/00baa870.cpp = 15 true hunks (rh real 25), 4600v4600, ruler a=5 dead asserts + k=3 `float fidelityN = sprite.MinDistance;`
  (device c, FITTED count; dead locals are hoisted: position never matters, only the count -- swept every statement position, identical).
  Previous best kept as differ/00baa870_p5b.cpp (31 rh). Base without ruler: w/r6/G.cpp. Tools: tools/x7.py (patch file x asserts x k grid,
  prints true-hunk addresses), tools/x8.py (per-position dead-local sweep), tools/cmpdump.py (comparator log dump with leaf names).
- MEASURED product key rule (exact on this row's trace): leaf key = id mod 2048; product key = (32*f + 64*s + 48) mod 65536 (+2 flag bits),
  f = the higher-key leaf, s = the other; sum terms emitted in DESCENDING key order (flat sort).
- side_tex Y row (0x00BAAE27, retail x,z,y; ours z,y,x): side+0 = id 1059, other_side+0 = 1068 (late), texture_transform Y-row E12/E22/E32 =
  2002/2003/2004 (block 1984..2015). With side/other_side fixed, retail's orders (side x,z,y AND other_side z,y,x) need E12 in [1959,1976]:
  exactly the E record block one 32-id block earlier (1952 -> E12 = 1970). Global rulers cannot do it (relative order invariant); needs one
  fewer record block allocated between the side block (1056) and the E block (record blocks 1248/1312/1408/1632 + interleaved gaps).
  Tried without gain: control vectors via += (worse, length), IsNonZero dead asserts at 3 positions (hoisted, global), side spellings
  (copy-init / direct-init / `side = up; side *= r` / -up direct-init), up via TransformVector (one/two-arg, in place: worse 24+).
- k fst (0x00BAACA0): product X*one_on_magnitude: up+0 = 790, one_on_magnitude = 792 (late, consecutive after squared): retail needs up+0
  key > one_on_magnitude key; impossible with consecutive ids unless the wrap -- the up+0 record must be created later than the Normalise
  local (structure unknown). Normalise texts (default, LOCALS_XYZ, PAREN_SQUARES_*, SQUARE_HELPER, UNPAREN) fix it but break the squares
  order 0x00BAC3C (LOCALS is retail's). FableWin Normalise 48i v LOCALS 47i is a compiler artefact: every float-returning helper (pow,
  GFFastOneOverSqrt, Normalise) is +1i/+3b in FableWin.
- const 5 / 0xb ecx are scheduling residues invariant over the whole (a,k) window; tried: C3DVector(sphere.Center)/r, named centre local
  (breaks frame), operator/ texts DIV/DIV2/DIV3/DIV4 (length), 0xb args as /64.0f, unparenthesised, (float) casts, -GetHeight()*k.
- (pass 4, end) Also no gain: up normalised as a temporary (`C3DVector up = C3DVector(...).Normalise();`: length 4590) or member-wise
  assigned (23+), k = 6..14 fidelity locals at a = 4/5 (k=6 equal 15, more worse / length). Parked again as near_miss: 6 clusters left,
  each needs a record-population fact not yet found (see the side_tex block-window analysis above). Next idea for a successor: find
  which late-pass record block between 1056 and 1984 retail does NOT allocate (e.g. trace the block allocator calls, not just records).
## 0x00B751F0 -- wfA4 pass 4 (2026-11-15): quantitative model of the test/or residue (k/or2.py; 6 comparator traces)
- The test/or bit is the SECOND comparator sort (k/*.json.rows pairs (592,0x20040002)x2; k/or2.py prints (key>>5)&2047). Per site two
  operand keys A (step 72 per UV site) and B (step ~38); the site is RIGHT iff A mod 2048 < B mod 2048 (validated: the model reproduces the
  whole observed assert-sweep sequence {123456}{12345}{1234S}{123S}{12S}{16S}{456S}{23456S}{123456S}; tools/b75model.py <d>).
  Baseline (differ, i0): A1=1600, B1=1510 (d = A1-B1 = 90); S: A=216(+2048), B=1890.
- Dead asserts (top or end, identical): UNIFORM +16 per assert on both A and B (8 asserts: +128 each) -> they slide the pattern but never
  change d. Named-local / code-neutral rewrites (bank / info locals, decl order, area(ctor), default:, C4DVector named, early return) give
  the IDENTICAL sweep pattern (not even a shift). The twin's GetFrame + SetRenderState opt-in set (gfsr) shifts it by ~2 asserts only.
- Closing needs d in [381, 1879] mod 2048 (brute force over (A1,B1) with the measured per-site steps and S offsets): i.e. d must change by
  +291..+1789; we sit in the failing band (-259..+291). Diagnostics (not candidates): removing the loop -> d=352 (+262), removing the
  lighting/specular/loop -> d=502, removing the GetFrame fetch -> d=354 (+264). So d is set by LIVE IL of the whole function (code-identical
  respellings cannot reach it); a real source difference of a few hundred live records is still unknown.
- Also measured this pass (k=0 identity + k=18 pattern; baseline k18 = {1,5,6,S}): 132 unused opt-ins of the included headers one at a time
  (tools/b75opt.sh, tmp/b75opt18b.out): none changes d (CARRAY / TOUCHFRAME / PEEKGRAPHIC-if-form only shift by ~1 assert; RELEASE_ISNULL /
  RELEASE_MEMBER / GETFRAME_EARLYRETURN change code); early-return SetRenderState / AddStateToUpdateList texts (probe/b75er, PROBE_ER1/2):
  identical; single-call PeekGraphicInfoFrame (named info): -33 bytes (retail really computes PeekGraphic twice); index loops: +67 bytes;
  local renames: identical. Parked again: the residue is d (live-IL difference of >= ~260 records), not a count; no device in the
  owner's list can reach it (asserts / k locals are uniform shifts or nothing).
## 0x00B77370 -- wfA4 pass 4 (2026-11-15, ~300 grades mostly scripted + 2 comparator traces): still DIFFER(2721v2734) 33 rh, differ unchanged
- Same quantitative model as 0x00B751F0 (k/or2.py on k/s0.json.rows): second-sort keys per UV site A1=1118 (step 72), B1=1190 (step ~38);
  right iff A<B mod 2048 -> sites 1-3 right, 4-6 wrong (= the graded residue). d = A1-B1 = -72; the six sites close only for d in
  [193,1913] (mod 2048), i.e. d must DROP by >= 63. Uniform shifts (dead / trivial asserts) never close it.
- Load order (first sort, k/s0.ids): lo 13375.. / hi 26583..34135 (16-bit keys), ours lo<hi at all six, retail the reverse: closable by a
  UNIFORM shift of 38953..51680 key units (1217..1615 ids). Trivial row-start asserts shift ~+12.8 ids each but also raise g (the ??_G
  split window: n PeekGraphic asserts x t trivial: n16 t0-12, n12 t48-56, n8 t96-104; tools/b77grid.sh, tmp/b77grid.out): no grid point
  fixes the load order (s896 trace: +544 ids only); best n12 t48 = 32 rh (site 6 test/or) -- 60 asserts for one hunk, not kept.
- DIAGNOSTIC ONLY (not a device): k `__assume(member != const)` statements at the row start lower d (~-1.5 per statement on 0x00B751F0's
  trace) -- on this row k=60 fixes the test/or of sites 4-6 exactly as the model predicts (rh 31). So the missing source fact is LIVE IL
  that leaves no code (pass-1 records), present in retail and absent from ours, in BOTH water rows (0x00B751F0 needs the opposite sign or
  +291). Lead for the coordinator: Lionhead's release ASSERT may be `__assume(x)` (landed CPersistContext rows use `__assume(0)` for
  NODEFAULT / ASSERT(false)); fable_assert.h models only the dead-arm form. Not adopted here (needs an owner decision + evidence).
- Opt-in sweep (106 unused opt-ins of the GetFrame / counted-pointer / render-state headers, tools/b77opt.sh, tmp/b77opt.out): none better
  (all neutral or 36..88 hunks). SetRenderState opt-in combinations (DESIRED/BOOKMARK/DIRTY, 5 legal combos): identical.
- (0x00B751F0, pass 4, end) Counter-evidence for an `__assume`-form release ASSERT inside the render-state setters: one
  `__assume(StateStackSize < 1024)` / `__assume(StateUpdateListSize < 256)` anywhere in SetRenderState / AddStateToUpdateList (probe/b75as,
  PROBE_A0..A4) changes the code (2713v2722, early region 0x00B752BF..0x00B753C7) -- so not there. Row-level `__assume(member != c)` keeps
  2722 and moves d by ~-1.5 each (tmp/cv/as*.cpp; traces k/as8, k/as32): far too weak (needs ~170+). No header change made this pass
  (hdr/ untouched; probes live in probe/b75er, probe/b75as only).
## 0x00BAA870 -- opus pass 5 (successor, 2026-11-15): 15 -> 7 true hunks, DIFFER(4600v4600); differ/00baa870.cpp (prev best: differ/00baa870_p6c.cpp)
- k fst 0x00BAACA0: NEW overlay opt-in FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY (LOCALS sum Z,X,Y from the members, no x/y/z copies;
  FITTED text choice among 30 swept sum forms/orders: member forms fix k fst AND keep the squares order; the copies were the cause).
  With it the old FITTED `bottom before top` is no longer needed (natural top, bottom order is now right).
- const 5 schedule 0x00BAB260/73: measured with the scheduler region dump (tools/sch.sh = wfB3 sched2/t15): the 81-tuple region cut fell
  between const 5's `fld inv ; fld center.x` and its fmul (region 31 = GetTransitionFade call .. const 4 + 1-2 flds). One more 0x269 PAREN
  tuple in region 31 fixes it: `cos((camera.PeekHFOV() * 0.0795774683f) * TWO_PI)` (FITTED grouping; `(fade.FadeBeginDistance - (r * 0.01f))`
  or `((cos(..)) * ..)` are equivalent; parens around a whole call ARGUMENT make no tuple).
- 0xb ecx load 0x00BAB383: GetWidth/GetHeight with the existing overlay opt-in FABLE_RETAIL_C2DBOXF_EXTENT_NAMED (EVIDENCED: FableWin 13i/30b
  v 11i one-expression body) -- the earlier source never enabled it. The 0xb centre-arg FITTED parens still needed for the 0xc ecx load
  (b00; one parenthesised arg suffices for 0xc but then 0xb ecx returns: keep both).
- Ruler re-swept: a=0..170 at k=0 (period 158, only a=4..5 / 161..162 near), then a=3..5 x k=0..8: best a=4, k=3..6 (k=3 kept) = 7.
- Residue: side_tex Y row term order 0x00BAAE27 (retail E12*sx, E32*sz, E22*sy; ours E32, E12, E22), RS test/or UVModeU 0x00BAB67E
  (site 4 of 7) + FilterMin 0x00BAB7C5 (site 7). Integer fidelity locals behave exactly like float ones (same grid).
- No gain: up via TransformVector (one/two-arg + up.Z = 0, any ruler: 25+), side/other_side respellings (length changes except
  direct-init), named fade_begin local, parens on the 0xb width/height args (no tuple).
- (pass 5, measured models of the 2 remaining residues; traces w/n/B7.lk.json, w/n/B8.lk.json, k/b8.json.rows)
  * side_tex rows: product key = (32*max + 64*min + 48) mod 65536 of the two leaf keys (id mod 2048), terms emitted in DESCENDING key
    order (verified on the side Y and Z rows of B7). Leaves: E12/E22/E32 = 2033/2034/2035, E13/E23/E33 = 2039..2041 (late pass),
    side+0 = 1056 (main stream), side+4/+8 = 33/35 (late, wrapped). Brute force: both rows right only for side+0 in [1029,1031] (or E
    shifted by -51..-53); no uniform shift (asserts / k locals) works except isolated points whose assert counts change length.
    The main record stream runs ...960..991, then 1056.. (ids 992..1055 are taken by another allocation before side): retail's side+0
    sits at offset 5..7 of block 1024, i.e. retail's main stream did NOT skip block 1024 there. Variant B8 (FITTED extra paren
    `(GetDimentions().Z * 0.5f)` in top, a=4 k=2) puts side+0 at 991 (one record fewer) -> Y row right, Z row wrong: also 7 hunks.
  * RS test/or: second-pass OR operand keys (k/or2.py on k/run.sh output) for the 9 setters (AlphaTest, AlphaRef, AlphaFunc,
    Specular, Fog, UVU, UVV, Mag, Min): A = 1402 + 72*i, B = 1586 + ~38*i; right iff A mod 2048 < B mod 2048. Ours pattern 111111000,
    retail 111110001. Brute force: needs the A-B offset changed by +19..+49 (with a uniform shift s in 27..118) or -127..-237;
    never at x = 0, so no ruler can fix it (same class as the two water rows): a live-IL difference before the first setter.
  * Tried without gain this pass: parens anywhere between PreTranslate and side (paren tuples move region cuts, not records), top
    spellings (named top_z, bounds reference, mesh_info->, top.Z assignment, /2.0f), GetCenter texts (PROBE1-3, default = neutral:
    CENTER_MIN_FIRST is no longer needed), textures[] placement / member-wise init, render-manager local (length), mixed assert kinds
    (`sprite.ObjectCount` costs the same ids as `sprite.Mesh`), integer fidelity locals (= float ones).
## 0x00B751F0 -- MATCHED (wfA4 successor-4, 2026-11-15, ~70 grades): src/00b751f0.cpp + test/00b751f0.cpp, RELOCATION_MATCH, test PASS (mutant without the fresnel clamp FAILs), read-only vland PASS
- What closed it: FIVE unused `CGraphicFrame` locals declared right after `CRenderStateScopedCaptureBlock capture;` (owner device c, `// Fidelity:` line,
  marked FITTED). Behaviour identical (empty frame: ctor stores 0, release is a no-op; no byte emitted). Sweep: row top k=1 {2345S}... k=4 {1}, k=5/6/7 MATCH;
  inside the helper after `ice`: k=1 {36S}, k=2 {6S}, k=3..5 {S}, k=6 MATCH, k=7 all six wrong; with k=6 in the helper dead asserts 0..8 keep the match.
- Why: the pass-4 model (keys A/B, d must move by hundreds) is right, and the lever is the constructor / destructor IL of CLASS-typed locals (the
  inline release chain of each frame is expanded, then folded away): it moves d where scalar locals, initialised dead locals, dead asserts, inline
  layering (SetupFresnelPixelShaderConstants helper: identical sweep), helper definition order, TU content (the whole row duplicated above/below: identical),
  /Zi and /Z7 (identical) cannot. Also measured: SetupIce.. plain / `inline` goes out of line (inline trace sz 589, c0 586 >> T 74), so FORCEINLINE is
  right; named CColorBufferDepthPackParameters / CEnginePrimitiveThemeChannelDesc locals: identical sweep.
- Coordinator decision: five unused class-typed locals as device (c) (the owner's list names "k unused named locals"; these are CGraphicFrame, not
  scalars). Plausible original: leftover frame declarations of textures the helper once fetched from the graphic bank (unproven).
- vland WARN RELOC_NAME only (PDB spellings GPShadowRenderer / GPLightingManager / D3DTextureUVModeTable v made-up labels): NAME_FIXES.tsv.
- (pass 5, end) Also no gain (each over a=3..4 x k=2..4): SetRenderState opt-ins DESIRED (4586) / BOOKMARK (= 7) / ADDSTATE_DIRTY (4586)
  / EARLY_RETURN (cl spins: incompatible with this include set), matrix texts PRETRANSLATE_ACCUMULATE / TRANSFORMPOINT_LTR (= 7),
  COPY_SUMS (29), POSTSCALE3 / no POSTSCALE_XYZ / no SETTRANSLATEONLY_XYZ (do not compile), mesh_info local dropped (= 7, neutral),
  const sphere reference (20), `> 0` LOD test (8), u_min operand order (9), unsigned GetMesh flags (= 7).
  FINAL: differ/00baa870.cpp = B7 (7 true hunks: side_tex Y row 0x00BAAE27/3B/45 + RS test/or 0x00BAB67E/82 + 0x00BAB7C5/C9).
  Equal alternative w/n/B8.cpp (extra FITTED paren in top, k=2): side Y right, Z row wrong (0x00BAAE50..60). Tools added: tools/pq.sh
  (grade + true hunks), tools/vg.py / vgg.py (variant files, optionally over a ruler grid), tools/x9.py / x11.py (ruler grids),
  tools/sch.sh + tools/sched2 (scheduler region DAG dump, from wfB3). No behaviour test (row not matched).
## 0x00B77370 -- wfA4 successor-4 (2026-11-15, ~110 grades + 2 inline traces): DIFFER(2721v2734) 30 rh / 24 sd (was 33/27), differ/00b77370.cpp (prev: differ/00b77370_prev16.cpp)
- NEW: k unused `CGraphicFrame` locals right after the capture block (the lever that closed 0x00B751F0): with the 16 PeekGraphic asserts, k=1..2 -> 32,
  k=3 -> 31, k=4..10 -> 30 (test/or of UV sites 4-6 0x00B7797A / 0x00B779E7 / 0x00B77A54 fixed), k=12 31, k>=14 worse, k>=40 moves the ??_G split
  (2686). The frames do not move g (split window still n=16..17). In the HELPER (next to edge_alpha) the frames change the inline decisions (2686/2622/2593).
- Grid n PeekGraphic asserts x k frames x t trivial row asserts: best n12 k4 t56 = 29 rh (68 asserts, not kept); n8: split lost at all t.
- Still open: (1) BL cross-jump 0x00B7768A..0x00B776E3; (2) bookmark LOAD order at all six UV sites (retail loads [state+8] before [mgr+2814h]; frames up to
  k=32 flip only site 3 and break sites 1-2's test/or); (3) ApplyVertexShader / ApplyPixelShader address temps (retail lea eax / lea ecx-before-this).
- Neutral with the frames: CCountedPointer<CTexture> / CIVCountedPointer<CFrameData> unused locals m=1..6, SetRenderState opt-in combos (DESIRED / BOOKMARK /
  DIRTY), TOUCHFRAME GetFrame text (FW-exact, split window drops to <=15: n15 = 34 rh), /Zi (identical). Frequency lever ruled out: every ??_G site has
  internal f 0.25 whatever the caller nesting (caller f clamps), so no early-return / loop shape can split them.
## 0x00BAA870 -- pass 6 (wfA4 last-resort pass, 2026-11-15)
- Class-typed unused locals (the 0x00B751F0 closer): C3DVector k=1..7 (no RS effect), CGraphicFrame g=1..7 at capture / textures / top /
  inside the mesh-change block, and g=1..7 x a=0..8 x k=0..7 (504 grades, tmp/a870/gfgrid.log): best 9, never below the 7 of B7.
- MEASURED (k/run.sh + k/or2.py, A = per-setter key step 72, B = step 38; base B7 A0=1400 B0=1584): each float fidelity local moves
  A by +21.3 and B by 0; one CGraphicFrame local moves A +64, B +110 (A-B -46: the wrong direction); removing the EndOcclusionTest
  statement (after the setters) moves neither. RS right iff 76 <= dA < 148, dB < 160, 19 <= dA-dB <= 49 (relative to B7).
## 0x00B77370 -- wfA4 successor-5 (2026-11-15, ~120 grades + 3 c2prio traces): DIFFER(2721v2734) 18 rh / 12 sd (was 30/24), differ/00b77370.cpp (prev: differ/00b77370_prev30.cpp)
- NEW (closes the bookmark LOAD order at all six UV sites): N dead `FABLE_RETAIL_ASSERT(SeaBumpMap != 0);` at the TOP of the helper (before the
  fetches = records allocated before the UV setter expansions). Window at k=4 frames: 78..~94 (77 none; 100..108 break the test/or; 110..124 good
  again; 130+ none); k=5: 84..90, 114..124; k=6/7 need more. The COUNT of statements matters, not their size (`a && b` needs ~74, `this != 0` folds).
  Asserts in PeekGraphic (x4 expansions) move g and lose the ??_G split (2622/2593). Tools: tools/b77370_t.sh (template B0.tpl/T1.tpl: n PG asserts,
  k frames, hm helper asserts), tools/b77370_bits.py (per-site load-order + test/or bits), tools/b77370_pieces.py (c2prio --blocks json -> piece blocks).
- The landed same-TU twin CreateDynamicTextures 0x00B72180's opt-in set (TOUCHFRAME, GETBANK_DIRECT, CURRENTFRAME_DIRECT, CFRAMEDATA_ISLOADING(_ISNULL),
  CIVCOUNTEDPOINTER_GET, TOUCHTEXTURE_GET, RELEASE_ISNULL, RELEASE_COUNT, SETRENDERSTATE_DESIRED, CAPTUREBLOCK_INLINE): same 18 rh at n=15, hm=120
  (TOUCHFRAME moves the split window to n=15). GETFRAME_EARLYRETURN: wrong layout (21).
- ROOT CAUSE of everything left (BL cross-jump 0x00B7768A..0x00B776E3 = the 13 bytes, temp rotation 0x00B776C8 edx, ApplyVertex/PixelShader lea
  eax/ecx 0x00B77B19/0x00B77B25): c2's const-0 live-range SPLIT in the 2nd GetFrame's failure paths. c2prio --blocks (tmp/b77m/B0.json): ours makes
  piece #175 = {new path 51..58} (spill 0, immediates) and #87 = {not-init BL 50, 58 live, release 59..66} -> split -> #170 {59..66} -> esi; retail
  has ONE piece {new path from `call new` .. release} in ebp (esi is busy in the new path) and block 50 immediates. Pieces are formed from the EARLIEST
  referencing block: ours numbers the not-init BL block (end of GetFrame's then-branch) before the new-data path (else-branch).
  PROOF: a probe GetFrame text with the outer test reversed (`if (Data.IsNull()) { new path } else { ... }`, probe/b77nf, NOT adopted) gives
  exactly retail's second-GetFrame code (xor ebp,ebp after call new, cmp eax,ebp, push ebp x2, jmp back to `cmp ebx,ebp`, no cross-jump) -- but
  lays the new path out inline in both GetFrames (25 rh) and breaks three landed rows (RenderSnow 0x00B54E30, RenderOuterSky 0x00B65A20,
  CreateDynamicTextures 0x00B72180 DIFFER) while 0x0041C260 / 0x009FF8D0 still match. So retail numbers the new path first in THIS row only, with
  the original layout: the source fact is in the caller context (not found). Not moved by: helper inner block for the tail, `bool ok` for the
  2nd fetch, nested-if / single-return shapes (worse), unused frames between the fetches (inline decisions change: 2686).

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).
- (pass 6) a=0..24 x k=0..8 grid (tmp/a870/ak.log): only a=4 k=3..6 = 7; a=7 k=5/6 = 8 (RS site UVU only + half-extent squares). Asserts
  beyond a=4 break the early x87 regions (side_tex / PreTranslate) long before the RS window opens.
- (pass 6) SetRenderState opt-ins DESIRED/BOOKMARK/ADDSTATE_DIRTY, all 7 combinations x a=0..12 x k=0..7 (728 grades, tmp/a870/rs*.log):
  never better than 7, RS never fully right. Enum names for the setter args (ALPHA_TEST_GREATER, TEXTURE_UV_WRAP, TEXTURE_FILTER_POINT,
  as the landed siblings spell them) are byte-neutral and kept. TransformPoint operand order inside SUMS is neutral (flat sort);
  the default += text is out of line, a forceinline += probe breaks 58.
- (pass 6) CGraphicFrame g=4 (a=3, k=2..4) is the FIRST point where all nine RS sites are right (RS model satisfied), but the
  PreTranslate/PostScale x87 region 0x00BAAB00..0x00BAABD4 and side_tex break (19 hunks, 4586 bytes).
- (pass 6) Class locals in a SCOPED block `{ T unused; }` (position anywhere after the x87 code: identical among all setter positions)
  act differently from function-scope ones. MEASURED: one CGraphicFrame block -> A +90, B +110 (function scope: +64/+110); float
  fidelity locals k=3..5 give identical keys (k0->k3 = +64 on A, a step function, not +21 per local). Block-type sweep (1 object):
  C3DBoundingSphere / CEngineFadeDistance / CMatrix3x4 / C2DBoxF / C3DVector / CPreTransposedBoneMatrix = neutral (7),
  CCountedPointer<const C3DMesh2> emits its out-of-line dtor (no), CIVCountedPointer<CTexture> = 5 (side_tex Y row + RS UVU only).
- (pass 6) NEW BEST differ/00baa870.cpp = 5 true hunks (prev 7-hunk best kept as differ/00baa870_p7b7.cpp): one unused
  `CIVCountedPointer<CTexture> unused_texture;` (device c, class-typed, FITTED, `// Fidelity:` line) inside the
  `if (PeekZSpriteGenerationEnabled())` block fixes SetTextureFilterModeMin 0x00BAB7C5. Trace k/a870iv1: A 1486+72i, B 1680+38i
  (A +86, B +96 v B7). The same local right after the GetMesh return test / inside the !IsLoaded return block / in a `{}` before
  the setters also gives 5; before the GetMesh return test (dtor copied onto that exit) 12; at function top 9; two objects worse.
  152 one/two-object placements x {CIVCountedPointer, CGraphicFrame} (tmp/a870/pp1.log): RS never fully right with early intact.
  Setter args now use the enum names (neutral).
- Remaining: (1) side_tex Y row (unchanged model: side+0 must be 1029..1031); (2) SetTextureUVModeU test/or 0x00BAB67E: needs
  A-B +29..59 with A shift < 62. Every class object tried LOWERS A-B (gf fn -46, gf blk -20, iv blk -10, box copy removal lowers
  it too); scalar locals beyond 3 and every scalar kind (double, bool, pointer, reference, long, uchar, class copies of
  sphere / window / matrix / fade) are neutral. No known lever raises A-B.
- (pass 6, end) Also neutral: visible noinline CPreTransposedBoneMatrix::Set (landed text) and VISIBLE_CALLED_DTOR for ~CCountedPointer,
  flattening SelectVertexBuffer into DrawBuffersIndexed, texture_transform / fade direct-init. Box as a const reference or an unused
  box copy move RS the wrong way. No header change this pass (probe overlay tmp/a870/ov only, not for hand-back). Tools:
  tools/a870grid.py (asserts x fidelity x class locals [+EXTRA defines, NBLK/TBLK scoped block]), a870pp.py (placement pairs),
  a870blk.py, a870cls.py, a870mv.py, a870an.py; logs tmp/a870/*.log; traces k/a870*.json.rows.

## 0x00B77370 -- wfA4 successor-6 (2026-11-16, ~150 grades): RELOCATION_MATCH, test PASS, read-only vland PASS, addmatch done; src/00b77370.cpp (prev best differ/00b77370_prev18.cpp)
- What closed it: the HELPER SHAPE, not a ruler. The edge GetFrame's failure must share the first GetFrame's `return false;` statement, and that
  shared exit must come BEFORE the success body in source order; the edge map read stays a separate statement after the first fetch:
      long surf_map = env->WaterSurfMap;
      if (<surf GetFrame>) { long edge_map = env->WaterEdgeAlphaMap; if (<edge GetFrame>) goto frames_loaded; }
      return false;
  frames_loaded:  <attach / UV setters>; return true;
  (goto = FITTED; precedent: landed goto-to-shared-tail rows 0x0075D9A0 / 0x00847C90.)
- Measured shape table (k=4, 78 helper asserts unless noted; marker = retail `xor ebp,ebp` x2 + `push ebp` x2 in the edge GetFrame failure paths):
  base `if (!A) return false; ...; if (!B) return false;` and X11 `if (A) { if (!B) return false; body }`: piece WRONG at every n (BL cross-jump).
  Nested `if (A) { if (B) { body; return true; } } return false;`, `if (A && B) {...}`, X13 `if (!A) return false; if (B) {...} return false;`:
  piece RIGHT while all ??_G expanded, but the shared exit after the body inverts the ??_GCGraphicFrameData call order (the failure-path release
  goes to a call before the surf final release): lengths 2804 / 2738 / 2670 / 2622, never 2734.
  `if (!A || !B) return false;` (edge read inline): piece right, 2734 at n=19, 9 hunks: the GP-chain temp is evaluated first at 0x00B774DD
  (ecx/edx swap; c1 evaluates the call-containing object chain before the plain arg). Bool-flag forms (`ok = A; if (ok) {..; ok = B;}`): piece wrong.
- Device windows with the goto shape: PeekGraphic asserts n=19 only (18 -> 2804, 20 -> 2688); helper asserts 70..86 at k=4..6 (94 breaks);
  k=0..3 never match (10..24 hunks). Smallest chosen: n=19, hm=70, k=4. Inferred / Fidelity lines rewritten with these counts.
- vland: RELOCATION_MATCH PASS; only RELOC_NAME warnings (D3DTextureUVModeTable labelled g_textureUVModeToD3D; ??_GCGraphicFrameData label).
- Test: model test (sibling 0x00B751F0 style) with surf-missing / edge-missing (surf released, edge never fetched when surf missing) / bound
  cases; mutant without the edge fetch FAILS.
- (succ-6 addendum) helper /Od 107i (fwsize, goto shape, no asserts) v FableWin 269i; n=19 window re-measured at hm=70 (18 -> 2804, 20 -> 2688). payload re-addmatched after the comment fix.
## 0x00BAA870 -- pass 7 (wfA4 successor-7, 2026-11-16, ~45 grades): still DIFFER(4600v4600), 5 true hunks; parked again
- differ/00baa870.cpp now carries two byte-neutral PDB spellings (prev kept as differ/00baa870_p8pre.cpp): the 9-arg DrawBuffersIndexed
  definition takes `const CTexture* textures[]` (FableWin ?DrawBuffersIndexed@CShaderRenderManager@@QAEXQAPBVCTexture@@KAAVCVertexBufferWin32@@...:
  Q = array parameter; our header decl `const CTexture**` mangles PAPBV) and FABLE_RETAIL_CENGINECAMERADESC_STRUCT (FableWin
  ?PeekCameraState@CEngineCamera@@QAEABUCEngineCameraDesc@@XZ, struct key). The header decl of the 9-arg overload still says `const CTexture**`
  (coordinator: an additive opt-in on CShaderRenderManager.h if wanted; not needed for bytes).
- fwsize (/EHsc, GetTransitionFade removed so the row is measured alone): row 820i/3251b with the devices, 729i/2911b without them, v FableWin
  845i/3250b (+116i over the device-free text: about 6-7 debug asserts of ~17i; our 4 dead asserts cost 19i each at /Od). This does not contradict device d
  at 4..7 (FableWin sizes are unreliable on x87 code; the gap is not a whole number of asserts): do not cite it on the Inferred line.
- No gain (each on the current best): named-result C3DVector operators INITIALISE_ADD / _SCALE / _DIV / SCALE_NAMED_RESULT (FableWin 34i/31i/35i)
  change the length (4590 / 4584 / 4616 / 4584; ADD+SCALE+DIV together 4622) -- MEASURED only: INITIALISE_SCALE alone puts the side_tex Y row in
  retail's term order (E12, E32, E22) but rotates PreTranslate and complements all nine bookmark test/or bits; a SCALE x asserts 0..14 x
  fidelity 0..7 grid (tmp/a870/p7scale.log, first 40 of 120 points read before hand-back) best 18 hunks at 4600 (side_tex Y row still wrong there); dead asserts placed right before the setters
  (n=1..2: 8 hunks, 3: 14, >=4: length 4586) -- they act globally, not from their position; float fidelity count re-swept k=0..8 at the old
  place and right before the setters (identical: k=2..3 = 5, k=4..6 break the early x87 row 0x00BAAD75, k=8 length): the effect is global;
  const GetAsCArray path in PrepareForRender (probe overlay); named texture local in the 9-arg DrawBuffersIndexed loop (its /Od size 45i/121b
  is closest to FableWin 46i/121b, but byte-neutral in the row); box hoisted before top (3 placements: length 4665..4717); top through
  mesh_info (4584); scoped block around the whole render section; if/else instead of the !IsLoaded early return (neutral / 4610);
  query declared at the top, bool occlusion_test (length), textures declared then assigned before the setters; TWO_PI as literal / non-static
  / extern const.
- Side finding for the landed same-TU row 0x00BAA460: its `Clamp` template is a made-up name (FableWin has no ??$Clamp); FableWin has
  ??$GFLimitReturn@M@@YIABMABM00@Z (39i/99b, `const float& __fastcall GFLimitReturn(const float&, const float&, const float&)`), a candidate
  for the real helper (one source only, NOT proven; GetTransitionFade /Od is 40i v FableWin 59i, so its debug text also carries ~19i more).
- Key-model caveat (for a successor using it): with the ids written in pass 5 (E12/E22/E32 = 2033..2035, side+0 = 1056, side+4/+8 = 33/35),
  the written product formula gives E32, E22, E12, which is neither ours (E32, E12, E22) nor retail's (E12, E32, E22): re-check the formula
  against w/n/B7.lk.json before trusting the "side+0 in 1029..1031" window.
