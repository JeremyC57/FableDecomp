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
Descriptive names used: g_spriteRenderEnabled_0139b6e4 (0x0139B6E4; no PDB / string / FSE / EgoCore name), as in the landed
g_particleSpriteRenderEnabled_0139b6e0. Established (landed) spellings kept: g_pEngineLightingManager 0x01436E9C, g_pEngineCamera /
NEngineInternal::GPCamera 0x01436EA0, g_pEnginePrimitiveTypeInfo 0x01436E84, g_pDisplacementRenderer_01436e38.
