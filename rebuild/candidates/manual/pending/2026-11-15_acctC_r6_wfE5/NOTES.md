# wfE5 notes (acctC lane, render rows)

## 0x00BBAC60 CEnginePrimitiveManagerDecal::RenderLandscapeDecal -- RELOCATION_MATCH, test PASS, addmatch, vland clean
What made it match (from the chunk-888 draft, DIFFER 3823v3743):
1. Shared-header port (local CGraphicFrame / CGraphicInfo / CTexturePointer / bank / resource-manager / CTVertexNormTex1 / CEngineLocalLightArray
   models replaced by the 0x00BB8EF0 opt-in set: CGRAPHICDATABANK_PEEKGRAPHIC, CGRAPHICINFO_GETFRAME, CGRAPHICFRAME(_CTOR/_GETTEXTURE/_DTOR_FORCEINLINE),
   CBANKFILE_ENTRYQUERY, CENGINELIGHTINGMANAGER_MEMBERS) -- code-identical.
2. SetTextureBorderColour expanded (new overlay opt-in CRENDERSTATEMANAGER_BORDERCOLOUR, FABLE_FORCEINLINE; plain inline stays called).
3. SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode written as the landed sibling 0x00BB8EF0 does (code-identical here).
4. The pixel-shader constant built in BOTH arms (`if (premultiply) { CRGBFloatColour colour(decal.Colour); colour.R *= colour.A; ...
   SetPixelShaderConstant(2, C4DVector(colour)); } else SetPixelShaderConstant(2, C4DVector(CRGBFloatColour(decal.Colour)));`): retail repeats
   the byte->float conversion at 0x00BBAF5B and 0x00BBB394 and shares the store tail by a jump (0x00BBB400). 3823 -> 3743, equal length.
5. Last residue: SIB base/index order of the index stores in both index-loop arms (12 instructions, 0x00BBB213.. / 0x00BBB40B..). A no-byte
   DIAG ruler at the end (24+ DiagRuler3) closes it = a symbol-record threshold. Dead release asserts (FABLE_RETAIL_ASSERT, overlay
   fable_assert.h): 2..6 match, 1 / 8 do not; 3 chosen = FableWin estimate ((861 - 597) / 87). Positions FITTED (vertex == vertex_count,
   index == triangle_count * 3, lights). SetRenderState variants (DESIRED / BOOKMARK / DIRTY), k unused locals, helper-text opt-ins
   (INITIALISEFROMCOLOUR, PSCONST_INDEX, INITIALISE_CTOR) do not move it.
Local class models: CEngineInternalPrimitiveDecal / CEnginePrimitiveManagerDecal (no repo header; landed 0x00BBA290 / 0x00BBA340 model locally).
No flags. vland WARN only: D3DTextureUVModeTable 0x01396DC8 labelled 'g_textureUVModeToD3D' in the label set (PDB spelling is the class static).

## 0x00BC04F0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite -- RELOCATION_MATCH, test PASS, addmatch, vland clean
Re-authored on the landed same-TU sibling 0x00BBF4A0 (RenderSubPrimitive): its opt-in set, helper texts (SetupBlendOpState /
SetupAlphaBlendingStatesFromBlendMode FABLE_FORCEINLINE, ApplyVertexShader / ApplyPixelShader / GetCurrentCamera inline, GFFastSin,
the header's Apply*VertexShader helpers, ConstantBlock references c0..c2) and statement shapes; the stale chunk-723c draft only for the
single-sprite parts. Steps: port (3626v3751) -> /d2inlT79 (PeekGraphic + capture-block ctor expanded; smallest N, T78 = 3710) ->
PeekRenderEnabled (+8, retail CEngineScreenEffectDisplacementRenderer branch + CENGINECOMPONENT_PROTECTED) -> the draft's alpha_scale /
premultiplied C3DVector colour (frame 0x20 -> 0x30, 27 hunks) -> c2.W = 0.0f (retail 0x00BC0FA4; equal length, one test/or left) ->
SETRENDERSTATE_DESIRED + _BOOKMARK (FITTED, existing landed texts; DESIRED + ADDSTATE_DIRTY also matches; dead asserts 1..6 do not).
The sibling 0x00BBF4A0 does not match with those two texts (checked): a per-row record-count device. New overlay header
CEngineInternalPrimitiveSprite.h; new nested opt-in _SETUPLOCALLIGHTS. g_spriteRenderEnabled_0139b6e4 is DESCRIPTIVE (no name found;
same pattern as the landed g_particleSpriteRenderEnabled_0139b6e0). vland WARN RELOC_CONFLICT: D3DAlphaFuncModeTable resolves to
0x01396DD8 here and 0x01396DE8 in landed 0x00BBC460 (same SetAlphaTestFunc(ALPHA_TEST_GREATER) text as landed 0x00BBF4A0) -- coordinator.

## 0x00BCA950 CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail -- DIFFER(3757v3749), 27 structural hunks (parked)
Port: the stale chunk draft's local tree filer / pool / class models replaced by the 0x00BCC800 prelude (same opt-ins, FlushRender /
DrawBuffersNonIndexed / PeekGraphic-free helpers, GetDynamicVB* bodies), shared SpriteTrail headers (needs `class CEnginePrimitive;` before
the header and FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS, else the derived class adds a second vptr: size assert 180 fails),
NEngineInternal::GPCamera with PeekNonHomogenousViewTransform / PeekInverseNonHomogenousViewTransform, and the sibling's
`vertex0 = vertices++; vertex1 = vertices++; SetPosition / SetDiffuseColour / SetU1V1` (103 -> 27 hunks). Residue = register picks (see
differ line 2): rematerialised points-1, ecx/edx swaps, index*3 scheduling, tangent pointer registers; size gap = alignment padding.
Tried: DIAG ruler 8/24/48 (no move), vertex_count from (points - 1), segments declared after length (no move).
Successor: 27 -> 9 hunks. `long vertex_count = segments * 16 + 2;` moved BEFORE `CMatrix3x4 transform = trail.WorldMat;` (retail computes it between the
return test and the rep movsd, 0x00BCAA4F..0x00BCAA6F) -> 11; `long first_step = 0;` declared before current / previous (retail stores [ebp+64h] = 0
first, 0x00BCAF41; the inner `for (long step = first_step; ...)` copy `mov ebx,[ebp+64h] ... mov [ebp+64h],ebx` at 0x00BCAFD6 needs the separate
variable: an outer `step` loses it) -> 9. Left: CSE (ours `mov eax,ecx`) v rematerialised `lea eax,[ebx-1]` 0x00BCAA4F; tangent .Y/.Z pointer IVs at
0x00BCB215; three alignment pads. No move from k=1..6 unused locals, Min/loop spellings, segments position, vertex_count type/const/order, direction spellings.
Third author: regraded unchanged (DIFFER(3757v3749)); masked diff = the CSE hunk (ours `mov eax,ecx` v retail `lea eax,[ebx-1]` 0x00BCAA4F: ecx
still holds segments from the Min operand `lea ecx,[ebx-1]` 0x00BCAA09, retail rematerialises), the tangent-row pointer / x87 operand order at
0x00BCB178 / 0x00BCB212..0x00BCB256, and the pads (the 1-byte shorter mov shifts the loop alignment: fixing the CSE hunk should fix the pads).
No genuinely new measured idea; parked on sight per WF_RULES (pure CSE / register residue, equal calls).
Fourth author (~70 grades + 2 c2prio traces): 9 -> 6 hunks, DIFFER(3746v3749). `C3DVector direction = tangents[current].Interpolate(tangents[previous], t);`
(FableWin ?Interpolate@C3DVector@@QBE?BV1@ABV1@M@Z donor 0x01D09580, body `*this+(vector-*this)*interp`; new overlay opt-in FABLE_RETAIL_C3DVECTOR_INTERPOLATE,
same text as the CPARTICLE_SPRITE block, which cannot be used because it also switches CMatrix3x4::TransformPoint to LTR) closed the tangent IV hunk
(0x00BCB215) -- EVIDENCED by the PDB function + bytes. Left: one allocator decision. c2prio: `segments` (id 20, prio -317) is split; its 2-ref piece
(def 0x00BCAA09 / use in vertex_count 0x00BCAA4F) gets ecx at prio 20 (allowed .cd.i..), the loop-guard piece is dropped (memory [ebp+58]). Retail
re-emits `lea eax,[ebx-1]` at the use (C39 lesson: c2 re-emits `lea [base+const]` for a dropped piece of p = base+const while base is live). Equal length
probe: `(short)segments` gives 3749 with only that hunk (pads follow the length). No move: 13 arithmetic forms (c2 reassociates all, even points*16-14
and trail_size-(skip+1)), segments / vertex_count at 5 positions, const / int / unsigned, k=1..7 unused locals plain and initialised at 3 positions,
1..8 dead asserts at 4 positions (>=5 moves other code), the six C3DVector operator opt-ins. Next idea: find what makes retail's def and the
vertex_count use separate 1-ref pieces (piece regions = forward reach within liveness, MATCHING_PLAYBOOK "Split pieces").

## 0x00BAE2D0 CEnginePrimitiveManager2DSprites::RenderSubPrimitive -- RELOCATION_MATCH, test PASS, addmatch, vland clean (successor)
Hand-authored from the disassembly and the acctB OUTLINE (ref/00bae2d0/notes) in the idioms of the matched 0x00BB30A0 (Render2DClothPrimitive:
CLOTH2D helper texts, SetTextureFactor, GetCurrent2DAlphaMaskInfo) -- first grade 5878v5892, 72 hunks. Steps: `unsigned long alpha = sub.PeekAlpha()`
per arm (retail reads Alpha once into ebx and converts it as unsigned long: fild + 2^32 fix-up 0x00BAED0B) -> 5892v5892, 4 hunks; clock textures
as an initializer (assignment [1] then [0] swaps the loads at 0x00BAEDB6) -> 2 hunks (one test/or order at 0x00BAF62D in the sprite ADDITIVE
SetSourceAlphaBlendMode); SETRENDERSTATE_DESIRED / _BOOKMARK / DIRTY / OUTLINE_INLINE no move; a named `EEnginePrimitiveBlendMode blend_mode`
per arm (FITTED; sprite arm only also matches; using it for every later use does not) -> RELOCATION_MATCH. GetPixelShader2DDiffuse(Only) as the
ternary (FableWin SAME 19i/58b). Setup2DDiffuseShading /Od 44i v FableWin 51i (x87; not closed by eight text variants; release bytes match).
NOTE for the coordinator: retail's GetPixelShader2DDiffuse is `mode == ALPHA ? PixelShader2DDiffuse : PixelShader2DDiffuseAdditiveAlpha`
(0x00BAED68 `cmp ecx,2`); the matched c19a 0x00BB30A0 defines it as ADDITIVE||ADDSMOOTH (only ever called with ALPHA there, so unconstrained).
Class model settled for the three 2DSprites rows: new nested opt-in FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_MEMBERS (inside _MANAGER:
render virtuals RenderSubPrimitive / Render2DPrimitive overrides, protected virtual RenderClockPrim slot 13 / RenderSpritePrim slot 14, typed
Ego_r members), new headers CEngineSubPrimitive2DSprite.h / CEngineSubPrimitive2DClockSprite.h / CTVertexColTex2.h (Ego_r layouts).
Globals: NEngineInternal::GPRenderManager / GPShaderRenderManager / GPPrimitiveRendererSharedData / GPPrimitiveTypeInfo (Anniversary publics).

## Open rows (not matched)
- 0x00BB0970 RenderSpritePrim / 0x00BAFA00 RenderClockPrim (2DSprites): re-ported only (overlay FORMATQUERIES / ISLINEAR; GetWidth /
  GetHeight are called out of line 0x004372A0 / 0x004372C0, GetPixelFormat 0x009F9D80). Both drafts still carry local models of the frame
  classes and of the manager; CEnginePrimitiveManager2DSprites.h has only a flat model (no RenderSpritePrim / RenderClockPrim /
  RenderSubPrimitive members): the class model must be settled once (a new opt-in on the shared header) before any of the three can land.
  Retail uvs evidence for RenderSpritePrim: GetTextureQuadUVs into a temp at [esp+3Ch], `rep movsd` x8 into uvs [esp+64h]
  (U[4] then V[4]), du on U1/U2, dv on V2/V3, offsets U0,V0,U1,V1,U2,V2,U3,V3; the last V3 add is commuted (`fld offY ; fadd st1`).
- Fourth author (2026-11-15): 0x00BB0970 / 0x00BAFA00 not re-authored this pass (budget went to the two near-misses); both still need a full hand re-author on the 0x00BAE2D0 class model per the map below -- do not port the stale drafts (138 hunks, local frame / bank / resource models).
### Successor's map for re-authoring 0x00BB0970 RenderSpritePrim on shared headers (class model now settled; not yet written)
- Frame fetch = the expanded CGraphicInfoFrame::GetFrame of the landed RenderSnow 0x00B54E30 / RenderOuterSky 0x00B65A20 / RenderClouds 0x00B64FA0
  (opt-ins CGRAPHICINFOFRAME_GETFRAME, CRESOURCE_TOUCHRESOURCE, CIVCOUNTEDPOINTER_RESET_ADOPT, CMOVABLERESOURCE, CGRAPHICFRAME(+_CTOR/_GETTEXTURE/
  _DTOR_FORCEINLINE), STATICBLOCKS_TEXTURE_HELPERS, CRESOURCELIST; row-defined PeekGraphic (inline) + PeekGraphicInfoFrame (FABLE_FORCEINLINE),
  RenderSnow's texts): retail 0x00BB0A80..0x00BB0C4B (bank = GPResourceManager->PeekGraphicBank(0) at +0x954, CGraphicInfo 0x2c, frames +0x28 x16)
  and again for the alpha mask 0x00BB0E83..0x00BB10D6 (mask bank index from GetCurrent2DAlphaMaskInfo, frame 0).
- Clip region = GetCurrent2DAlphaMaskInfo (0x01436E84 ->+0x94 ->+0x0C: AlphaMaskBankIndex +4, AlphaMaskBank +0, MaskObjectsOutsideAlphaMask +0x28,
  AlphaMaskClipBox +0x18); copy 0x00BAE2D0's helper texts. Viewport = GPDisplayManager (0x01436E1C) +0x1F4 C2DBoxI; GetRenderTargetDimensions 0x009BEDC0.
- Interpolation factor = CEngineInternalPrimitiveBase::CurrentInterpolationInfo.InterpolationSetList[InterpolationSet].WFInterpolate
  ([set*12 + 0x0143B934]); the _CLASS branch of CEngineInternalPrimitiveBase.h lacks GetCurrentInterpolate (needs a nested opt-in under
  _CURRENTINTERPOLATIONINFO). GFFastCos/Sin over 0x013CD550 (GFFastCos.h, GFFASTCOS_FORCEINLINE as in 0x00BC04F0).
- Needed new opt-ins: CEngineInternalPrimitive2DSprite.h typed retail class over the polymorphic base (C2DBoxF ClipBox +0x48, EEngine2DLayer Layer,
  BankIndex, Frame, StartAngle, EndAngle, CRGBColour Colour +0x6C, CTexture* Texture +0x70, bool Filter, C2DVector UVOffsetsStart +0x75 /
  UVOffsetsEnd +0x7D (packed), WrapTexture +0x85, BlendMode +0x88); CGraphicFrameData::GetTextureQuadUVs() const 0x009FC810 (out of line) +
  Width/Height (+0x14/+0x16); CEngineScreenEffectDisplacementRenderer::GetEnable2DDisplacement (FableWin lists it right after RenderSpritePrim;
  0x01436E38 +0xEE); CEnginePrimitiveRenderer2D Layer2DToDistance 0x00B4A6B0 / Layer2DDistanceToZ 0x00B4A6C0 / Layer2DToSortDistance 0x00B4A6A0
  (FableWin inline Layer2DToZ listed after RenderClockPrim) and Enable2DLayer[] +0x320 [third author: header has only Layer2DToDistance under
  _LAYER2DTODISTANCE; Layer2DToSortDistance 0x00B4A6A0 is bound in reloc_bindings, 0x00B4A6C0 is unbound -> needs a new opt-in declaring both;
  retail call list also has 0x009FF6B0 after each BeginLoading (frame copy, out of line) and 0x009DE140 at the end (debug box draw);
  GetTextureQuadUVs is `?GetTextureQuadUVs@CGraphicFrameData@@QBE?AUCTextureQuadUVs@@XZ` (hidden struct result)]; CEngineSubPrimitiveRenderer ADDTYPED 0x00B84720 (0x01436E74);
  CEngineSubPrimitiveBase::operator new 0x00BEB540; ctor 0x00BACE50.
- 0x00BC8A60 RenderLine (third author): RE-AUTHORED on shared headers, differ/00bc8a60.cpp, DIFFER(3394v3416), 46 masked hunks (was 136).
  Model: 0x00BCC800 prelude (GetFrame / PeekGraphicInfoFrame / CGraphicFrame opt-ins) + decal row's SetupAlphaBlendingStatesFromBlendMode text;
  CScopedShadersEnable (shared header, 0x013BC470 = CShaderRenderManager::SingletonInstance) with FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
  (retail expands the dtor at both exits 0x00BC8C1B / 0x00BC970F); NEngineInternal::GPCamera PeekNonHomogenousViewTransform / PeekInverse...;
  new nested overlay opt-in FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM_CAMERASTATE (PeekCameraState().NearZPlane, camera +0x6c);
  C2DVector IsNonZero / Normalise 0x00A14540 (C2DVECTOR_NORMALISE); TRANSFORMPOINT_SUMS (66 v 68 for LTR / ZYX); static local index table
  `static unsigned short indices[6] = {0,1,3,0,3,2}` (0x0139BAC8, .data, one xref); local class models (no shared header; landed siblings do the same).
  What closed hunks (EVIDENCED by bytes): blend form `alpha = line.Colour.A; if (!Setup..(mode)) brightness = 0xff; else brightness = alpha;`
  (retail: movzx ebp before the switch, default arm `mov eax,0xff ; jmp`, case tail `mov eax,ebp` 0x00BC9310); `bool textured = BankIndex != 0`
  after it (setne cl 0x00BC9317); ApplyPixelShader as if/else (cross-jumped call, push in each arm 0x00BC9390..); clip as
  `start += (end - start) * ((near - start.Z) / (end.Z - start.Z))` (fst + fdiv st(1) 0x00BC8C56); `C2DVector perp(dir.Y, -dir.X)` (fchs 0x00BC8DCA).
  Left: frame 0xd0 v 0xc4 (retail reuses start's slots for corner2 [0x00BC8E04]: slot packing), corner Z int copies scheduled early in retail
  (0x00BC8D93..), x87 term order of the transform rows, near-compare fnstsw schedule (0x00BC8C01), GetFrame: retail keeps the
  Frame-not-initialised BeginLoading separate (0x00BC9733), ours cross-jumps it with the new-CFrameData one (store order 0x00BC976C).
  Tried, worse: explicit switch (108), ternaries (114), corners copy-then-adjust (61), end-first declaration (61), near first (56).
  Frame analysis (the 12-byte gap drives most remaining hunks): retail stores corner2 into start's own slots ([esp+0Ch..14h] after
  `push esi`, 0x00BC8E04 / 0x00BC8E0E / 0x00BC8DA9) and copies start.Z / end.Z into the corners' Z slots as integer moves early
  (0x00BC8D93..0x00BC8DBE); ours copy-propagates corner0.Z = start.Z (zz read from start.Z's slot), keeping start live, so no slot sharing,
  frame 0xd0, and the x87 row order follows (retail X row starts with the corner's own Z slot). Corners declared up front and assigned from
  C3DVector temporaries (x1) give frame 0xc0 and 41 hunks but +65 bytes of copies; member assignments (x3/x4) change nothing. Next idea:
  find the construct that makes corner0.Z a real copy (inline helper with C3DVector out-params for the clip / corner build, or the
  TransformPoint(const C3DVector&, C3DVector&) overload on temporaries), then re-sweep TRANSFORMPOINT variants.
- 0x00BC8A60 RenderLine (fourth author, ~45 grades): DIFFER(3409v3416), 25 struct hunks (from 96 unmasked; frame now 0xc4 = retail).
  Closed (EVIDENCED by bytes): corners as `C3DVector offset(perp.X, perp.Y, 0.0f); corner0 = start - offset; corner1 = start + offset;
  corner2 = end - offset; corner3 = end + offset;` -- retail copies start.Z / end.Z into the corner Z slots as int moves (0x00BC8D93..
  0x00BC8DBE) because Z = start.Z -/+ 0.0f folds after copy propagation; start's slots then hold corner2 (0x00BC8E04) and the frame shrinks
  by one vector (3394 -> 3417 bytes, 96 -> 29 hunks). `dir *= line.Width * 0.5f;` (C2DVector::operator*=(float)) for the two scalings
  (retail consumes the half width with `fmul [esp+38h]` 0x00BC8DBA) -> 25.
  Left: (a) x87 commutative order: start / end Y rows of TransformPoint (retail E22*yy first, 0x00BC8B1A / 0x00BC8BAC), corner0 transform
  (0x00BC8E56..0x00BC8E96), `start.X + offset.X` loaded start-first in retail (0x00BC8DE2, 0x00BC8E12); (b) near-compare fnstsw schedule
  0x00BC8C01; (c) const 0 in ebp from the textured test in ours (`cmp edi,ebp` v retail `test edi,edi` 0x00BC9315; retail loads ebp=0 at
  0x00BC9432); (d) GetFrame tail: retail keeps the Frame-not-initialised `push ebp ; push ebp ; call BeginLoading ; jmp` block separate
  (0x00BC9733) and schedules the Reset path as `push ; push ; mov ecx,edi ; mov [esi+8],edi ; call` (0x00BC976C); ours tail-merges the two
  BeginLoading calls = the 7-byte gap. RenderSnow 0x00B54FE2 retail has ours' store-first order (immediate pushes there).
  No move: TransformPoint LTR / PARTICLE_POINT / ZYX / default (SUMS best), C3DVector ADD/SUB groups, offset names (9), offset forms (4),
  textured spellings (5), k=1..32 initialised unused locals, dead asserts 1..12 (float condition: worse).
Descriptive names used: g_spriteRenderEnabled_0139b6e4 (0x0139B6E4; no PDB / string / FSE / EgoCore name), as in the landed
g_particleSpriteRenderEnabled_0139b6e0. Established (landed) spellings kept: g_pEngineLightingManager 0x01436E9C, g_pEngineCamera /
NEngineInternal::GPCamera 0x01436EA0, g_pEnginePrimitiveTypeInfo 0x01436E84, g_pDisplacementRenderer_01436e38.

## 5th author (wfE5 re-run, 2026-11-15) on 0x00BCA950 / 0x00BC8A60 -- both re-parked, differ/ sources unchanged (no variant beat them)
- 0x00BCA950 (~45 grades + 2 c2prio traces, tools/c2/c2prio_op.py copied from hand892): still DIFFER(3746v3749), 6 hunks = the one
  remat decision at 0x00BCAA4F. Measured: the `trail.MaxLength - 2` temp of block 6 is NOT a colouring candidate (block-local: it takes
  whatever the global `segments` piece leaves -- edx when the piece is ecx, ecx when the piece is edx). `segments` (#20) fails in round 1
  (allowed only ebx = points) and is split; the def + vertex_count piece (2 refs) survives and gets ecx (prio 20, allowed .cd.i..). Retail's
  `lea eax,[ebx-1]` needs that piece to be split again / dropped (1-ref pieces), i.e. no free register over its region. x87 blocks 7-9
  have K = 0 (no integer candidate), so their live-through cost is 0: the priority cannot be lowered there.
  No move (all grade 6 hunks or worse): float max_length / remaining named (6/11), if-form Min (7, x87 changes), unsigned / long
  max_segments, segments after the return / before the loop / loop guard `points - 1`, `points - 1.0f`, vertex_count after the transform
  copy (22: the piece then takes edx because rep movsd forbids ecx / edi, still `mov eax,edx`), after the tangents ctor (memory reload),
  vertex_count as `points*16-14`, `(points*8-7)*2`, `(trail_size-skip-1)*16+2`, `2*(segments*8+1)` (c2 canonicalises all to the
  CSE'd segments<<4+2), nested `if (!(length < D)) {..}` / `if (points >= 2) {..}` instead of early returns (identical), reusing `skip`
  as segments (8), vertex_count spelled inline at both uses (17), the Lionhead const-reference Min / Max templates of C2DLineF.h (9..24,
  reference temporaries go to memory). Next idea (not done): instrument c2's split pass (0x1078B2AA / region 0x1078B51C) to print each
  piece's block set and forbidden registers, then find which statement in retail closes ecx / edx / edi over block 10.
- 0x00BC8A60 (~45 grades): still DIFFER(3409v3416), 25 hunks. Measured: the x87 Y-row order of the start / end TransformPoint (retail
  E22*yy first, ours E12*xx first -- the Z row comes out yy-first in BOTH, so it is not the SUMS text) and the corner adds do not move with
  1..8 unused float / long locals (bare or initialised from line.BankIndex / line.Width) at the top, nor with 1..256 live-initialised float
  rulers at the top or right before `offset` (k >= 16 only grows the frame: 3417 bytes, 32+ hunks): not a record-id tie here. Worse:
  named `const CMatrix3x4& view` / `inverse_view` references (36..53), the TransformPoint(const C3DVector&, C3DVector&) overload with
  default / SOURCE_DEST_SUMS / COPY_SUMS (115+). GetFrame tail: FABLE_RETAIL_CIVCOUNTEDPOINTER_CASTTOBASE (FableWin's CFrameData Reset
  form) and RESET_FORCEINLINE give identical bytes; GETFRAME_EARLYRETURN 27. The tail-merge is a list-scheduler order in the Reset block
  (retail `push ebp ; push ebp ; mov ecx,edi ; mov [esi+8],edi ; call`, ours store-first) -- the same instructions in both, so a DAG / tuple
  order question (hand892 sched/ tools would measure it; not run).
- 0x00BC8A60 (5th author, scheduler trace with wfB1's sched tools copied to tools/sched/, paths repointed): in OUR build the shared
  `push ebp ; push ebp ; call BeginLoading ; jmp` is already its own scheduling region (6 nodes) -- the two BeginLoading calls are tail-merged
  BEFORE scheduling, so the residue is the IL-level merge, not the scheduler. Retail keeps path 1 (Frame not initialised, ecx = Data.Object
  from the test 0x00BC95D3) and path 2 (new CFrameData: pushes, then `mov ecx,edi ; mov [esi+8],edi`) apart. Probe (overlay copy in
  tmp/00bc8a60_hdr only, NOT in hdr/): `CFrameData* data = new CFrameData(this); Data.Reset(data); data->BeginLoading(..)` gives identical
  bytes (copy propagation unifies the `this` operands). Next idea: whatever makes path 2's call tuple differ from path 1's at IL level
  (its `this` evaluated after the argument pushes).

## 0x00BB0970 RenderSpritePrim -- RELOCATION_MATCH (wfE5 6th author, 2026-11-15; ~60 grades)
- src/00bb0970.cpp + test/00bb0970.cpp (PASS), addmatch + read-only vland clean (only RELOC_NAME warnings for the catalog labels of 0x004372A0 /
  0x004372C0). No flags, no pragma. Re-authored from the disassembly on shared headers (the stale draft's body was a guide only; it had two real
  errors: the half-texel nudge is 0.01f (0x012A5428), not 0.5f, and vertex 2/3 are BL/BR with UVs 3/2).
- Headers: 0x00BAE2D0's class model + RenderSnow's GetFrame set + new overlay opt-ins (hdr/README.md, '6th author' block): CGRAPHICFRAMEDATA_TEXTUREQUADUVS,
  CENGINEINTERNALPRIMITIVE2DSPRITE, CENGINEPRIMITIVERENDERER2D_LAYER2DTOZ (FABLE_FORCEINLINE Layer2DToZ: plain inline was called out of line),
  CENGINESCREENEFFECTDISPLACEMENTRENDERER_GETENABLE2DDISPLACEMENT, CRENDERMANAGER2D_DRAW2DBOXOUTLINE; copied verbatim: wfB3's
  CENGINERESOURCEMANAGER_SATRGBATEXTURE, wfE7's CENGINESUBPRIMITIVEBASE_SETTERS_ALPHAFLOAT. SharedData via _RENDERSETTINGS + _SILHOUETTES
  (Show2DBoundingRect +0x25C); the sub-primitive renderer needs _ADD + _ADDTYPED. The source needs a `// Retail-asm:` line (GFFloatToLongNear
  is a shared retail-asm helper; without it purity.classify says naked_asm and vland reports NOT_GENUINE).
- What closed it (measured with wfB1's c2 scheduler tracer, wk/bb0970/sc.sh): the block 0x00BB10EC..0x00BB1379 is ONE c2 scheduling region
  (calls do not cut it) split into 83-node windows; retail's window edges fall after the first corner fstp (0x00BB1219) and after the last
  corner store (0x00BB1327). Moving the edges = changing the IL node count of the statements before them: ClipBox.GetWidth()/GetHeight()
  (needs FABLE_RETAIL_C2DBOXF) removed the two float-rounding nodes of `(BRX - TLX) * 0.5f`, and the clip-space conversion written as two
  statements per coordinate added the nodes that put the colour code into the last window. FITTED items are listed in the source header.
- Also FITTED: the UV temporary as `{ CTextureQuadUVs quad = ...; uvs = quad; }` (copy from lea esi,[esp+3Ch]; plain `uvs = call()` copies
  from the returned eax; a function-scope named copy costs its own 32-byte slot, frame 0x84); DiffuseColour after U1/V1 per vertex.

## 0x00BAFA00 RenderClockPrim -- near miss DIFFER(3936v3946), 16 masked hunks, ONE residue (wfE5 6th author, ~75 grades)
- differ/00bafa00.cpp: re-authored on shared headers with the 0x00BB0970 model (same opt-ins) + new overlay opt-in
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DCLOCKSPRITE (hdr/README.md). The stale draft had the 0.5f nudge (retail 0.01f, 0x012A5428 x4) and
  the type ternary; everything else of its statement structure was right. Clamp = landed GFLimitReturn text (Ego_r/FableWin
  ??$GFLimitReturn@M@@YIABMABM00@Z); the second clamp (fraction) is an explicit ternary (immediate stores 0x00BB0721..0x00BB0746).
- Closed (EVIDENCED by bytes unless FITTED): two GetCurrent2DAlphaMaskInfo calls (retail folds +0xC into displacements, 0x00BAFA6D);
  then-arm of the index loop as plain post-increments (inc eax ; inc eax, 0x00BB0824); Colour / Colour2 stores before the textures
  (0x00BB086F); `to` declared before `from` (segment in edx, next in eax, 0x00BB0786; FITTED); clip-space conversion as two statements per
  coordinate for all five points + direct `(BRX - TLX) * 0.5f` half extents (window edge: `mov ecx,13Ch` at 0x00BB04EE; FITTED, the
  0x00BB0970 lesson); `// Flags: /d2inlS18` (GFLimitReturn is called at S17 and below, any /d2inlT has no effect; inline-budget device b).
- LEFT (10 bytes): retail keeps constant 0 in edi from the index loop to the Draw2DBoxOutline push and spills the AddSubPrimitive type
  local to [ebp+74h]; ours gives edi to the local. c2prio (wk/bafa00/prio_v5.txt, trace_v5.txt): the local (prio 16) is coloured before
  the split pass that creates the loop zero piece (prio 20). Moving the local's def before the loop flips it (proves the mechanism) but
  moves code. Tried, no move: if/else / ternary / `type` reassigned / init-then-if / declaration position / reversed polarity / two calls
  (longer) / k = 1..7 unused locals / 1..3 dead FABLE_RETAIL_ASSERTs / named zero index / loop variable types / colour ctor forms.
  Next: find what retail's IL had that triggers that split pass earlier (trace the split triggers in c2, or a source form where the
  local's web spans the loop without code motion).
## 7th author (wfE5 successor, 2026-11-15) on 0x00BCA950 / 0x00BC8A60 -- both re-parked, differ/ sources unchanged (~35 grades + 2 c2 traces)
- 0x00BCA950: piece lineage read from the c2prio --json trace (tmp/a950/base.json): segments #20 (refs at def / vertex_count / loop entry /
  latch) -> split -> #170 -> {#284 1-ref dropped spill -10 (a loop-guard ref), #202} -> #202 split -> #241 = def + vertex_count (2 refs,
  ecx). Retail's `lea ecx,[ebx-1] ; mov [ebp+58h],ecx` (def) + `lea eax,[ebx-1]` (use) is exactly what two 1-ref dropped pieces would give
  (def computed into a scratch and stored, use rematerialised from live ebx), i.e. in retail the split of #202 must START at the
  vertex_count block (or the def block must have had no free register when #202 was split). Block shares (--blocks): x87 blocks 7-9 K=0.
  New no-move / worse grades: segments declared before `start` (79 hunks; + vertex_count after the transform copy: 68, equal length,
  memory reload `mov eax,[ebp+20h]` not a remat), vertex_count after the transform copy alone (29), `(points - 1) * 16 + 2` with segments
  kept / segments defined after the length test / both (byte-identical: copy propagation), `fraction` before `start` (9 hunks, equal
  length 3749 only because an fst becomes fstp+fld; the remat hunk is unchanged), fraction clamp before `start` (16). Next idea is the same
  as the 5th author's: read c2's split pass (0x1078B2AA, hot/cold-split code) for the start-block rule of a piece; not done.
- 0x00BC8A60: the x87 orders are insensitive to every surface spelling tried: `offset + start` v `start + offset`, offset built from
  `dir.Y, -dir.X` without perp, `0` v `0.0f`, k = 1..3 unused floats after `end`, after screen_end, after dir (all byte-identical, 26
  hunks), C3DVector ADD_XY_GROUP (26) / SUBTRACT_XY_GROUP (27) / NAMED_RESULT_SUB / INITIALISE_SUB (80). Probe header (tmp/8a60_hdr only,
  NOT in hdr/): a TransformPoint with the Y row spelled `E22*yy+E12*xx+...` or Z `E23*yy+E13*xx+...` is byte-identical (c2 canonicalises
  commutative sums: source term order is irrelevant); declaring `yy` before `xx` in the body moves code (50 hunks) but keeps the Y row
  xx-first. So the Y-row order is not term order nor xx/yy symbol order; parked.
## 8th author (wfE5 successor, 2026-11-15) on 0x00BAFA00 -- re-parked, differ/00bafa00.cpp unchanged (~14 grades + 7 c2 traces)
- MECHANISM PROVEN with a diagnostic forced-priority what-if (C20 c2pf/c2prio.py, C2_FORCEID, obj graded by tmp/bafa00/objcmp.py;
  tmp/bafa00/wi.sh): forcing `list` (#92) to priority <= 8 gives RELOCATION_MATCH of the WHOLE row (92=8, 4, 1, -200 all match;
  92=11 / 13 do not; following 91=17 does not). So the only fact missing is a source whose `list` priority is <= ~10 (ours: 16).
  Sequence (json trace tmp/bafa00/base.json, tree.py / lin.py): `following` (prio 14) taking edx splits the const-0 piece; the loop+tail
  zero piece it yields has prio 11 and takes edi only if `list` has not taken edi first (list prio 16 > 14 > 11 in ours).
- Why the source cannot reach it with the retail block structure (c2prio --blocks): list = then block 203 (K 2: list + const 5) +4,
  else block 204 (K 1) +2, use block 205 (K 5: list, sub, this, prim, const 0 cost 0) +10 = 16, no live-through block. Even K = 1 in
  every block gives >= 8+... (min 14 with the use block's 4 variable candidates), so retail must have had list live through extra
  blocks (or fewer candidates in the call block) that leave no code. Not found.
- New no-move grades (all byte-identical to base unless noted): ternary as the AddSubPrimitive argument; float `distance` local for
  Layer2DToSortDistance (3940, worse); `type` reassigned on today's base (byte-identical: c2 makes the same copy web); init-then-if;
  list reusing `segment` / `next` (identical: candidates are per web, not per symbol); selection moved before Colour / SetAlpha /
  SetRenderFlags (stores move after the test: worse); `following` as unsigned short / short (identical); `unsigned short i` (3944, worse).
- Next idea for a successor: a retail-plausible construct between the list defs and the call that creates c2 blocks but no code
  (list must lose >= 6 to live-through penalties), or a call-block shape with fewer candidates; the forced-prio harness grades any
  hypothesis in ~40 s.

## 9th author (wfE5 successor, 2026-11-15) on 0x00BCA950 / 0x00BC8A60 -- both re-parked, differ/ sources unchanged (~85 grades + 6 what-ifs)
Re-measured with the owner's last-resort devices (WF_RULES acceptance test) in mind; none moves the residues.
- 0x00BCA950 (still DIFFER(3746v3749), 6 struct hunks = the remat at 0x00BCAA4F + pads):
  - Dead release asserts (FABLE_RETAIL_ASSERT_DEAD_ARM, fable_assert.h, condition `trail.Width > 0.0f`): k = 1, 2, 4 at 12 positions
    (top of body .. inner loop .. before SetTextureUVModeU) are byte-identical (6 hunks); k >= 5 at ANY position before the final block
    gives 22 hunks (position-independent = a record-count effect: x87 orders flip in the tangent loop 0x00BCAB4E and the
    TransformPoint rows 0x00BCB261..0x00BCB2F7; the remat hunk stays); k 16 / 24 / 32 -> 26 / 30 / 59. So device (d) cannot reach it,
    and the remat decision is invariant to record ids (device (c) likewise, as the 4th author measured with k = 1..7 locals).
  - No `segments` variable at all (Min on `points - 1 - fraction`, loop `segment < points - 1`, vertex_count `(points - 1) * 16 + 2`
    or `points * 16 - 14`), and `float segments`: byte-identical 6 hunks (c2 value-numbers every form into one temp).
  - Forced-priority what-ifs (wfcommon C20 c2pf/c2prio.py; tmp/00bca950_wi.sh): C2_FORCEUID 241 / 202 / 170 (the piece lineage) and
    C2_FORCEID 20 = -1000 / -10 / 100 all byte-identical to base -- pieces made by the split pass are not inserted through the
    forcing hook (0x10782B26), and forcing the parent did not register either (log shows no `forced` event); a what-if that reaches
    split pieces needs a hook in the split pass (0x1078B2AA), not built (BRIEF: no compiler-hook tooling).
  - Reasoning recorded: the def+vc piece's priority is a sum of positive w*K*cost terms (the x87 blocks between have K = 0), so it
    can never be <= 0; retail's remat therefore needs the piece NOT TO EXIST (def and vc in different pieces, or vc in the dropped
    loop-ref piece), i.e. a different split region, not a different priority. Inline control does not apply (no inline residue).
- 0x00BC8A60 (still DIFFER(3409v3416), 25 struct hunks):
  - Dead release asserts k = 1..32 at the top of the body: all worse (k 1-3 / 7-12: 38 hunks, k 4-6 / 14-20: 44, 24: 48, 28-32: 60+;
    length 3417 from an alignment pad and corner1..3 x87 order flips); the start / end Y-row orders 0x00BC8B1A / 0x00BC8BAC never move.
  - GetFrame tail evidence: the same header text (FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME body, flags 0) gives three different
    path-B shapes in matched / landed rows: RenderClouds 0x00B64FA0 `push ebx ; mov [esi+8],edi ; push ebx ; mov ecx,edi ; jmp <path-A
    call>` (call-only cross-jump, 0x00B65A17), CreateDynamicTextures 0x00B72180 `mov [esi+8],eax ; mov ecx,eax ; push ebx ; push ebx ;
    call` (store first, like ours, 0x00B7233B), RenderOuterSky 0x00B65D3D `xor ebx,ebx ; push ebx ; push ebx ; mov ecx,edi ; call`.
    So the residue is per-context scheduling / cross-jumping, not the header text; a duplicated tail in the header would contradict
    the FableWin /Od size of GetFrame (146 instructions), so it is not an acceptable fitted text.
  - x87 Y-row order analysed: retail evaluates the stack-resident yy side first (`fmul E22` on st0), ours loads xx first then fxch; the
    Z row (yy from memory) is yy-first in both. Not record ids (asserts / locals / rulers), not term order (7th author). Open.

## 10th author (wfE5 successor, 2026-11-15) on 0x00BAFA00 RenderClockPrim -- MATCHED (RELOCATION_MATCH, test PASS, addmatch, vland clean)
- src/00bafa00.cpp + test/00bafa00.cpp; flags /d2inlS18 (Inline-budget line on line 2: GFLimitReturn expanded, S17 leaves it called);
  same overlay opt-ins as the 6th author's differ source (no header change this pass). differ/00bafa00.cpp removed (superseded).
- What closed it: the index loop's `indices[index++] = following;` written in BOTH arms (each arm writes its whole fan entry) instead
  of once after the if/else. Retail's single copy at 0x00BB085F (`inc eax ; mov [esi+eax*2+0F8h],dx ; inc eax`) is c2's cross-jump of the
  two arm tails, done after register allocation; at allocation time the loop latch then holds only `++i ; i < 6` (K 2, not 5), so the
  zero constant piece that spans the loop arms to the Draw2DBoxOutline push loses less live-through priority (latch -4 instead of -10)
  and gains in the arms (K +1 each), rises above the AddSubPrimitive type local (16) and takes edi; the local spills to [ebp+74h] with the
  retail if/else stores. One edit, first grade (tmp/bafa00/bafa00_d1.cpp).
- How it was found: c2prio --blocks of the base (tmp/bafa00/blkall.txt) shows K and the live-through penalties per block; the type local
  cannot drop below 14 with the retail block shape (8th author), so the other side was worked: which blocks cost the zero piece its
  priority. The latch block 200 (w 2, K 5: following, i, index, sub, const 6) was the largest no-reference penalty that a source form
  could remove without changing code -- a statement that retail shows as a cross-jumped tail.
- Also measured this pass (all DIFFER, no move): `const ESubPrimitiveType& list = cond ? DISP : type` / a pointer to the local (c2
  removes the address-taking: byte-identical); flag test spelled `!= 0` (identical), via a bool local (3940), via `>> 10 & 1` (18 hunks);
  early `ESubPrimitiveType list = type;` + `if (flags) list = DISP;` before the index loop (15 hunks: the zero does take edi, the local is
  spilled into type's home [ebp+7Ch] with a self-copy before the loop -- a second confirmation of the mechanism), at the top (174), before
  the angle (100), after the loop (24); dead early inits (const or type) + full if/else: eliminated, identical.
- Forced-priority what-ifs this pass: list #92 = 10 and 9 also give RELOCATION_MATCH (threshold is < 11 = the zero piece's priority).
- vland warnings (not errors): RELOC_NAME GetWidth / GetHeight (as 0x00BB0970, NAME_FIXES rows exist); RELOC_ALIAS __CIfmod -> the
  MSVCR71 import thunk; RELOC_CONFLICT ??0CEngineSubPrimitive2DClockSprite@@QAE@PAVCEngineInternalPrimitive2DClockSprite@@@Z -> 0x00BB1780
  here, 0x00686800 in landed 0x004D2DDB: the landed CTCUndeadSoul row misnames the CTCBase constructor 0x00686800 with this class's
  constructor name (NAME_FIXES.tsv row added); 0x00BB1780 is the real one (called right after operator new of the 0x138-byte clock
  sub-primitive at 0x00BB053E, stores the clock pointer at +0Ch and vtable 0x012A55AC).

## 11th author (wfE5 successor, 2026-11-15) on 0x00BCA950 / 0x00BC8A60 -- both re-parked, differ/ sources unchanged (~50 grades)
- 0x00BCA950 (still DIFFER(3746v3749), 6 struct hunks = the `segments` remat at 0x00BCAA4F + pads). New measurements (tmp/00bca950_va/vb/vc.py,
  driver tmp/00bca950_drv.py): the def / copy structure is invisible to c2 -- Min on `(points - 1) - fraction` with `segments` defined after
  the return or before the loop, `(float)segments - fraction`, `segments = points; --segments;`, a named `float segf`, a named
  `float remaining = segments - fraction`: all byte-identical (6). Outer loop as `while`, as `if (..) do {} while (++segment < segments)`,
  `segment` declared before the for: byte-identical. Tangent loop as while / if+do: worse (34 / 39, 3778 / 3794). vc after the transform
  copy re-measured: 22 (piece takes edx).
  New lead, NOT verified: c2prio trace tmp/a950/base.json shows `forward 241` -- the def+vc 2-ref piece goes through c2s
  (line above cut by a shell quote; it continues:) copy-forwarding check (0x107820B7, drop pass). That check returns at once unless the
  candidate +0x3C node has opcode 1, then needs the conditions at 0x1078C249 (node +0x2C -> +0x10 set, [node]+9 bit 0, node+0x28 byte
  2 or 6, 0x10796DAD / 0x1078006C non-zero). Retail's `lea eax,[ebx-1]` at the vc use is exactly what a successful forward of
  `segments := points - 1` into its only other use would emit (no piece left, the use recomputed from live ebx); the 9th author showed
  the piece cannot be dropped by priority. Next step for a successor with hook tooling: break at 0x1078C249 for #241, log which
  condition fails, then find the source fact that makes it pass. ~17 grades this pass.
- 0x00BC8A60 (still DIFFER(3409v3416), 25 struct hunks; ~30 grades). New measurements: `textured` spelled `> 0`, `? true : false`, `!!`,
  implicit bool, `0 !=`: byte-identical; `textured` after `level` 32, before the if 43, before alpha 111, no variable 49 -- the const-0
  piece keeps the textured block in all (ours `xor ebp,ebp ; cmp edi,ebp`, retail `test edi,edi` 0x00BC9315, ebp = 0 only at 0x00BC9432).
  Dead release asserts (FABLE_RETAIL_ASSERT_DEAD_ARM) before offset / perp / inv_start / near_plane, k = 1, 2, 4, 8: position-independent,
  38 / 44 hunks, 3417 bytes (adds corner1..3 x87 flips and a pad; the Y-row and start+offset orders never move). GetFrame tail:
  RELEASE_MEMBER and ISNULL+DTOR_DIRECT byte-identical, RELEASE_FORCEINLINE / INNER / default 56; a probe overlay with `T* Object` in
  CIVCountedPointer (tmp/00bc8a60_phdr, probe only, not in hdr/) byte-identical. The landed out-of-line GetFrame 0x0041C260 (real
  argument registers) has retail's `push ; push ; mov ecx,edi ; mov [esi+8],edi ; call` with the same statement order as ours
  (`Data = p;` then `Data->BeginLoading`), so the order is a per-context scheduling outcome, as the 9th author concluded.
- 0x00BCA950, 11th author, forwarding lead MEASURED (tools/c2/c2fwd.py = c2prio_op.py + 9 log hooks inside 0x107820B7 / 0x1078C249;
  json tmp/00bca950_fwd.json, 45 s): of 71 forwarding checks, 64 stop at the first test (candidate +0x3C node opcode != 1). #241
  (segments, def+vc piece) has opcode 18 (as two `_nodeKey` candidates), so forwarding is never attempted for it. Opcode 1 is the
  copy form (`_this` from the parameter, `_previous` = the result of the inline Max), i.e. a copy that survives copy propagation
  because its source is a multi-def temporary; such candidates then go through the further tests (c1..c5). Retail's straight
  `lea ecx,[ebx-1] ; mov [ebp+58h],ecx` def does not look like such a copy, so the forwarding lead is WEAK: a successful forward in
  retail would need segments defined from a multi-def temporary that compiles to the same single lea. Not pursued further; row
  re-parked. Opcode histogram of the 71 checks: 618 x40, 50 x8, 1 x7, 45 x7, 30 x4, 18 x3, 33 x2.
