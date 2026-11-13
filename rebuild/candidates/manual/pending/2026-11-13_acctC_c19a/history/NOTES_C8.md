# agentC8 NOTES (w22 acctC lane; continues agentC6 / C3 / C2). Tools: tools/ (C6's, paths re-pointed to agentC8) + new:
# tools/om.py (O-meter: binary search of the late-ruler count that loses the next 'a'), tools/mm.sh (variant -> grade + O-meter + L-meter,
# log probe/mm.log), tools/odis.py (disassemble symbols of an obj), probe/scan.sh (ruler scan).

## RESULT (agentC8): 0x00BB30A0 RELOCATION_MATCH (3538v3538) + test PASS + vland PASS; sibling 0x00BB3E80 re-fitted, RELOCATION_MATCH + PASS + vland PASS.
Hand-back: src/00bb30a0.cpp, src/00bb3e80.cpp (supersedes agentC6's), test/00bb30a0.cpp, test/00bb3e80.cpp (C6's, unchanged), payload.json / oracle.tsv
(both rows), hdr/ (+2 opt-ins, hdr/README.md). What made it match (FINAL = "solution B" below):
- SHARED texts (both rows, one TU): FlushRender `PD3DDevice->SetStreamSource(0, 0, 0, 0)` (was SelectVertexBuffer(0, 0): same bytes, ~34
  fewer records; THE lever for the 0x00BB33B7 test/or order), SetTextureFactor pixel path `C4DVector(colour)`, FABLE_RETAIL_CRGBFLOATCOLOUR_
  INITIALISE_CTOR, FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX, new FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET and
  FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER (FableWin sizes for the last three: SAME).
- Row 2 text: GetCurrent2DAlphaMaskInfo with a named viewport-manager pointer (fwsize 16i/46b = FableWin SAME), tint as
  `CRGBColour tint(primitive.TintColour); tint.A = alpha; CRGBFloatColour tint_colour(tint);`, `const C4DVector& factors`, named `mesh` and
  `self_illumination`, FABLE_RETAIL_ASSERT(cloth_primitive.GetParticleProgram()). FITTED as a set for the fog load order (0x00BB3D28): low edge of
  an 8-wide window (tolerates +6 rulers, none fewer). Unused row-1 helpers (GetAppropriateMipmap, PeekGraphic, AddOcclusionTest, GFGetTopBitInByte)
  dropped from row 2: GetAppropriateMipmap names GFFloatToLongFloor (an asm helper) and made purity grade the row naked_asm.
- Row 1: + FABLE_RETAIL_ASSERT(cloth_primitive.GetParticleProgram()) and FABLE_RETAIL_ASSERT(primitive.MeshEffect.Get()) in the first outline
  arm (11 asserts, FITTED; window: k=0 only of rulers 0/2).
- Alternative also matching both rows ("solution D", probe/candD_keep_r*.cpp + probe/hdr_candD): ATV without named shader refs instead of
  C4DVector(colour)+GetParticleProgram Get(); not chosen (FableWin's ATV 68i is closer to the named-ref text 67i than 61i).
## (history) 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive -- was IN PROGRESS, DIFFER(3538v3538)
Residue (unchanged): expansion 8 = SetAlphaTestEnable(false) in SetupAlphaBlendingStatesFromRenderFlags' `else` of (render_flags & 2)
(0x00BB33B7: retail `test eax,esi ; or esi,eax` = 'a', ours 'b'); expansion 23 = SetFogEnable(false) (0x00BB3D28: retail loads the state
mask first = 'M').
Measurements (agentC8):
- Late AND early rulers (`const ERenderPassType ruler_K = pass;`) are equivalent (global count). Base: a-count 7 for k in [0,27], 6 for
  [28,~91], 5 from ~93; L(fog)=M for k in 7..15, 40..48, 72..90 (not strictly periodic). Large k (300..900) cycles the O band and changes code.
- Deletion diagnostics (code-breaking, probe/d): ATV call removed -> a0=8 (T8=32), Lmeter 11; bail-out FlushRender removed -> a0=9 (T9=31);
  SetUserVertexShaderConstant removed -> a0=9 (T9=5), Lmeter 12; SetSelfIlluminationAmbient removed -> a0=7 T7=30, Lmeter 14.
  => O and L are separate axes; structural changes (calls / address-taken temps) move them much more than named locals.
- Pure unit weights (Ometer/Lmeter, base 28/7): ToC3DVector / C4DVector& factors / named C3DVector / named CRGBFloatColour +1;
  named CRGBColour tint 0; lights as pointer 0; composite_surfaces ternary 0; preloading outside its block 0 (code changes);
  ATV without the three named `shader` refs -3 (31/10); ATV clamp removed 0; ATV single call through a shader pointer -1 (code changes);
  FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE 0; FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR +4.
- SetupAlpha arm reorders (`!(flags & 2)` first, `pass != 1` first) change the code layout: retail's layout is the current one.
- FableWin evidence: the inline helpers listed after Render2DClothPrimitive include GetDiffuseTexture / GetBlockOnLoad / Get2DAlphaMaskTexture /
  GetSatRGBATexture (inline in FableWin; out of line 0x00BA9360 / 0x00BA9A70 in retail) and GetAveragePatchSize (not in retail's code);
  FableWin's row is 239i v ours 195i (+44: less than one debug assert, so the later revision added code; no assert count follows).

## 0x00B783F0 CEngineWaterRenderer::Render -- UNTOUCHED

### agentC8 progress on 0x00BB30A0 (probe/base2.cpp)
- Ruler scan k=92..420 (probe/scan1.out): the a-count drops every 32 rulers at k = 28, 92, 124, 156, 188, 220 (T7=28; the step at 60 is
  skipped), so T8 = -4 (expansion 8 is 4 units short); L(fog)=M windows are [7,15], [40,48], [72,90], [104,120] -> [-25,-17] below.
  => remove R in [17,25] pure units (C6's estimate confirmed) with T9 (~-36) still <= 0.
- base2 = src + three code-identical changes (all shared with row 0x00BB3E80): ATV without the named `shader` refs (-3),
  SetTextureFactor's pixel path `SetPixelShaderConstant(2, C4DVector(colour))` (-4; FableWin has ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z),
  FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX (existing repo opt-in; fwsize SetPixelShaderConstant 28i/79b = FableWin SAME; -1).
  base2: O RIGHT (a0=8, T8=4), L still C: Lmeter 15 -> 9..17 more units to remove (or an L-only structural lever).
- FableWin: ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z 12i/28b = `{ InitialiseFromColour(colour); }` (?InitialiseFromColour@C4DVector 23i/68b).
- 0-unit on base2: two_sided const, &textures[0], textures block closing before the last FlushRender; two_sided before the lighting calls changes code.
- FABLE_RETAIL_ASSERT dead arms weigh 0 units in this row (6 positions measured).
- MATCH FOUND (agentC8): K3 = src + FlushRender SetStreamSource direct (F1) + FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR + tint copy (CRGBColour tint(primitive.TintColour); tint.A = alpha) + const C4DVector& factors -> RELOCATION_MATCH (probe/K3_match.cpp). K2 (= F1+fltinit+TB+GetParticleProgram Get()) also matches (probe/K2_match.cpp + probe/hdr_K2_keep).
- Row 2 matches found (all RELOCATION_MATCH, code identical otherwise): FlushRender with `PD3DDevice->SetStreamSource(0, 0, 0, 0)` instead of
  the inline SelectVertexBuffer(0, 0) (F1: O-only lever, ~34 O-units, 1 L) + FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR (FI, existing opt-in)
  + tint copy `CRGBColour tint(primitive.TintColour); tint.A = sub_primitive.PeekAlpha(); SetTextureFactor(CRGBFloatColour(tint));` (TB) +
  one of {`const C4DVector& factors` (UR), GetParticleProgram via ParticleProgram.Get() (P1, FableWin 10i), named mesh (A5), named
  self_illumination (A6)}. L window 8 wide (K3 = F1+FI+TB+UR tolerates +6/-1 rulers), O window ~22.
- BUT F1 + FI are shared with the matched sibling 0x00BB3E80 (one text per helper): row 1 with F1+FI = DIFFER (O one extra 'a',
  L two M's). Row-1 measurements (tools/r1w.sh, probe/r1w.log): each of its 9 FABLE_RETAIL_ASSERTs weighs exactly one O step (32 units)
  and ~2 L; `mipmap_level < 32` assert weighs 0; TB in row 1 +4 L; P1 +1 L/+2 O; ATV direct -4..-6 L; C4DVector(colour) -4 L/-4 O.
  Row 1 with F1+FI needs one more assert (O) and L +24..26 or -6..-8. Joint combo search running (tools/combo.py, probe/items_r1.py /
  items_r2.py, logs probe/combo_r1.log / combo_r2.log, overlay probe/hdr_new with two new opt-ins: FABLE_RETAIL_C3DCLOTHPRIMITIVE_
  GETPARTICLEPROGRAM_GET and FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER).
- GetPixelShader2DDiffuse as a switch = FableWin 19i (instruction count SAME; ours if-form 16i) but changes the register allocation
  (ebx/ebp swap of graphic_bank / composite_surfaces): not adopted. FableWin is a VC8/VC9 build (_Iterator012 / _Container_base12 STL):
  byte counts and conversion-heavy helper sizes differ from VC7.1; instruction counts of plain accessors are still a good hint.
- Behaviour test written: test/00bb30a0.cpp (host model, sibling style).
- Joint fit (two rows, one text per shared helper). Combo logs probe/combo_r1*.log / combo_r2*.log (tools/combo.py, items_*.py, mkv.py).
  Row 1 + F1 + FI matches with C4DVector(colour) (C1) + 2 more asserts (ASP = `FABLE_RETAIL_ASSERT(cloth_primitive.GetParticleProgram())`
  before the program deref, ASE = MeshEffect in the first outline arm, ASB = PeekParentBase before SetupAlpha), e.g.
  C1+PSI+P1+Q1M+ASP+ASE. Row 2 with C1 needs 5 more L-units than with the sibling-free set (each named local / GetCurrent2DAlphaMaskInfo
  named manager (FableWin SAME 16i/46b) / ASP2 / named float tint = +1): C1+PSI+P1+Q1M+TB+UR+A5+A6+ASP2+GCa+FT predicted.
  Row 2 without C1: PSI+P1+Q1M+TB+UR+A5 (or +A6, or TB+A5+A6), PSI+P1+TB+UR, P1+Q1M+TB+UR all match; row 1 without C1 is 2..4 L-units short
  (asserts are O-steps with ~0..2 L; late asserts OH / SR give the O step with 0 L).

## 0x00B783F0 CEngineWaterRenderer::Render -- DIFFER(9343v9343), ported, residue unchanged (agentC8, ~70 grades)
- differ/00b783f0.cpp = acctB's best draft ported onto TODAY's headers: shared engine/CEngineComponentRenderParams.h (the draft had local
  structs; InterpolationInfo.BulletTimeCurrent.WFInterpolate with FABLE_RETAIL_CENGINECOMPONENTPARAMS_INTERPOLATIONINFO), the repo opt-ins
  that replaced acctB's stale ones (_CREATEDYNAMICTEXTURES, FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE for SetTextureBumpEnvMatrix,
  FABLE_RETAIL_CENGINESKYRENDERER_ENVMAP, FABLE_RETAIL_CTEXTURE_WATERDYNAMIC) and one NEW overlay opt-in FABLE_RETAIL_CENGINEWATERRENDERER_RENDER
  (hdr/engine/CEngineWaterRenderer.h: RenderBackground / RenderIceBackground / RenderSeaNonReflectiveForeground / SetCommonRenderStates decls;
  93 static asserts pinned). SetCommonRenderStates' FORCEINLINE body is in the row. Size / frame / calls equal; residue = acctB's:
  L quartet (lo.py positions 21-24: ours M, retail C) at 0x00B7945D/0x00B794C1/0x00B79525/0x00B79589, exp3 pair (63-64: ours C, retail M)
  0x00B7A5D0/0x00B7A635, O at 0x00B79938 (position 32), and a store scheduled at 0x00B7913A v 0x00B79125.
- tools/wscan.py (rulers after `float time`, prints only the differing L/O positions): symbol rulers 0..64 never flip the quartet; +4..+6
  fix the exp3 pair; from +12 other O positions break; 26..34 and 60..64 change the code (8947 bytes).
- Lesson from 0x00BB30A0 to try next: the L order of a block depends on the records created BEFORE it (forward), O on the whole count; the
  biggest code-neutral levers are inline helpers called with constant arguments whose branch folds (FlushRender's SelectVertexBuffer(0, 0)
  in the cloth rows). Here the region before the quartet holds four AttachTextureToStage(i, 0) (dead `texture ? ... : 0`), Restore(),
  the envmap block and SetFogEnable(false): acctB measured that removing all four attaches flips the quartet. Look for the retail text of
  that region with fewer folded records (e.g. a helper that releases the stages) -- per block, metered with wscan.py.
- agentC8 fix on the water row: FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_RESTORE_OPEN_FIRST (hdr/engine/CRenderStateCaptureBlock.h): retail clears
  Open BEFORE calling RestoreCaptureBlock (0x00B79125 store, then call 0x00B7912A) although the block address escaped -> the source order
  is store-first (as the landed INLINE_DTOR text). Fixes the store-scheduling hunk (ndiff 115 -> 114); the quartet / exp3 pair / O pair stay.
  Grades on this row: ~70 (wscan probes included). Not tried yet: per-block metering of IL changes in the Restore..exp2 region; the
  exp3 pair is fixed by +4..+6 symbols (a natural +4..+6 lever must not move the quartet, which symbols never move).
