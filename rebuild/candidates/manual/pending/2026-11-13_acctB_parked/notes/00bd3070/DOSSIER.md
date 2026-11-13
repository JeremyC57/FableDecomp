# DOSSIER 0x00BD3070 -- refresh (identity scout pass 2, 2026-11-12, after the bwf1 author's k01)

State: best = $W/v/k01.cpp = $W/differ/00bd3070.cpp, SAME SIZE 11288v11288, 2138/3160 eq, 348 hunks, frame `sub esp,0E4h` v retail 0E0h.
($W/draft_original.cpp is the OLDER c15 and is NOT the differ file any more.) Sections 0-7 below are pass 1; this block supersedes them where it disagrees.

## A. Identity (re-verified, two sources)
`?RenderMeshPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase@@QAEXAAVCEngineSubPrimitiveAnimatedMesh@@W4ERenderPassType@@@Z`
= ghidra_out/egor_pdb_names.tsv (Ego_r 0x00A87E30) + naming_stragglers/FableWin.exe.signatures.tsv (0x02E7E830, 6451 B / 1674 i).
Manifest label right. public non-virtual thiscall `void (CEngineSubPrimitiveAnimatedMesh&, ERenderPassType)`, `ret 8`, no SEH, no /GS,
VC7.1 biased frame `push ebp ; lea ebp,[esp-70h] ; sub esp,0E0h` (not /Oy-). Only caller 0x00BD7199 (RenderSubPrimitive 0x00BD7110, landed).
Still UNLANDED callees (declare, call out of line): 0x00BA9830 GetReflectionTexture, 0x00BD2D90 GetBoneTransformations, 0x00B905A0
PrepareOcclusionTest, 0x00B4FC40 SetupShadowFuncRenderStates, 0x00B47390 SetupShaderSpotLight(const CMatrix3x4&), 0x00B46C80
UpdateGlobalLightingStates, 0x00BEFD90 / 0x00BF2240 / 0x00BF0740 / 0x00BF0B20 / 0x00BF1EE0 outline/overlay state helpers.
0x00B8D090 / 0x00B89070 are landed only as PLACEHOLDER rows (E2_E2_*): PDB names CEngineOcclusionManager::StartOcclusionTest /
CEngineScreenEffectOutlineGlow::SetupDepthBufferTransform (draft spelling right; NAME_FIXES for the placeholders).

## B. The `failed` check: pass-1 point 7.1 was half wrong; NEW hypothesis H1 (untested by the author per NOTES/ATTEMPTS)
Retail layout facts (dis_all.txt):
- 0xBD4E7B `mov al,[ebp+6Fh] ; test ; je 0xBD549A` (the `failed` out-arg bool itself, no copy) sits INSIDE the combined-pass arm, directly
  after that arm's SetTextureFactor shader path (0xBD4E3F..0xBD4E74 `or [eax+3D8h],2`, fall-through). 0xBD4E86..0xBD4F98 = early-return
  block (inline FlushRender loop over [+3CB8h] stages, RestoreDefault..., `jmp 0xBD5C26` epilogue).
- The combined arm's NON-shader TEXTUREFACTOR path is placed AFTER that return block (0xBD4F9D..0xBD5057) and jumps BACK to 0xBD4E7B
  (3 jumps). Z_WRITE (0xBD5139): its shader path `jmp 0xBD4E71` (cross-jumped into the combined arm's last store), its TEXTUREFACTOR path
  jumps to 0xBD4E7B (3 jumps). 9 jumps total land on 0xBD4E71/0xBD4E7B.
- Arms that never set `failed` jump to 0xBD549D (pass 8 0xBD5258, 0xBD53EC, 0xBD5444, 0xBD5449, 0xBD5498); 0xBD549A only reloads
  edi = primitive ([ebp+0Ch]) for the !failed path, then PrepareOcclusionTest.
- Ours (k01): combined shader path `jmp 0xBD51B1` INTO Z_WRITE's copy (reverse cross-jump direction), non-shader path falls through, then
  the `abort_render` test at 0xBD4F72; whole block lands 78 instr later = the -78/+75 and +22/+18 hunks at 0xBD4E71 / 0xBD5292 / 0xBD53FF.
H1: retail has NO abort_render and NO single post-chain check; each arm that can fail ends with its own textual
`if (failed) { FlushRender(); RestoreDefaultPrimitiveParameters(); return; }` (identical text). VC7.1 cross-jumping keeps the FIRST copy
(combined arm) and points every later identical tail at it -- exactly the 0xBD4E7B fan-in; arms without the check fall to the join
(0xBD549D). This also removes the 1-byte bool that plausibly costs the 4-byte frame excess (0E4h v 0E0h). Author's NOTES: "removing it
(single check after the chain) un-threads arms 8/12/13, +48 bytes" -- that tested the single-check form, not H1. Try H1 first.

## C. Frame-slot map, k01 vs retail (scout2/slotmap.py over v/out_k01.txt; 142 of 348 hunks are pure slot renames, 48 slot+reg)
retail -> ours: this +58 -> +48 (18x, first 0xBD3088); primitive +0C -> +30; mesh_primitive +2C -> +24; decal +18 -> +04; scale (+58 of
parent) +30 -> +2C; sphere +24/+28 -> +1C/+20, +1C -> +14 (15x), +20 -> +18; mipmap temps +4C/+48 -> +50/+4C; +50 -> +58 (15x);
material +60, composite_surfaces +44, bank +54, +68 (GFFloatToLongFloor fistp slot) already equal. Negative block -64..-44 shifted by 4
(ours one more 4-byte local at the bottom). Code at the function start (0xBD3184..0xBD3240) is instruction-identical apart from these
slots, so the residue is local allocation order, not logic. The v/ruler sweep (dead `const long` locals) improves hunks 348 -> 339 at
+40 but is diagnostic only.

## D. Switch-arm residue (0xBD3CBB..0xBD3E4C, -67/+78 hunks) is register noise, not source
Retail arms 6 (0xBD3CBB) and 7 (0xBD3D44) each keep their own inline ApplyPalSkinVertexShaderEnv expansion (`cmp eax,5 ; jbe ; mov eax,5`,
VS table this+0x58) and share only from `push edx/ecx` (PS +0x104) -> ApplyPixelShader 0xBD3E17 -> SetEnableFadedShadowFactor tail.
They do not cross-jump the VS part because retail picked edx in arm 6 and eax/ecx in arm 7; ours picked identical registers and merged
it. Fix the slot/register map (B, C) first; do not rewrite arms 6/7. Arm 6 textures[1] = [0x1436E98]+0x2AD4 (GetZeroRGBATexture inline).
Arm 7/10/13 PS constant: lighting mgr ThemeChannels(+3Ch)[CurrentThemeChannel(+E0h)]*0x70+0x30 copied to a 16-byte temp [ebp+34h..40h]
(esi preloaded with [+34h]) then to (+3CCh)[0..3], `or [+3D8h],2` (8 v 11 instr hunks at 0xBD3F6A / 0xBD41E7: scheduling of that copy).

## E. Combined-pass texture factor (0xBD4DDE): pass-1 point 7.2 still stands
R from tint byte [ebp+6Ah], G/B from byte copies [ebp+10h]/[ebp+14h], A from `sub_primitive.Alpha` (byte [+8], movzx), each
`fild ; fmul 1/255 (0x1231724)` via one temp [ebp+40h]. k01 is close but NOT equal (hunks 0xBD4DDE..0xBD4E22): ours loads G from
[ebp+0Ch] and R into ecx first, retail R into edx then Alpha byte, then G [10h], B [14h]; retail also reads material->SelfIllumination via
eax ([ebp+60h]->+24h) where ours uses edi. Operand-order residue: name the intermediates (R,G,B,A) in the order retail loads them.

---- pass-1 dossier follows (point 7.1 superseded by B; everything else still valid) ----

# DOSSIER 0x00BD3070 CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive (identity scout, 2026-11-12)

Deep material already exists and is still correct; this file adds what changed and what to do first.
Full outline: $W/prior/scout/outline.md (block-by-block, frame map, globals, inline table, fistp sites).
Names/headers/members: $W/prior/scout/headers.md. NAME_FIXES candidates: $W/prior/scout/NAME_FIXES_candidates.tsv.

## 0. DO FIRST (best draft is NOT the best source)
- $W/draft_original.cpp = agent4010 cand/c15.cpp = eq 1838. Scout candidate **/opt/fable_lane/w22/agent4010/scout/cand/s03.cpp = eq 2071**
  (+233), under overlay /opt/fable_lane/w22/agent4010/scout/hdr. The diff c15 -> s03 is exactly 3 lines:
  `#define FABLE_RETAIL_C3DMESHMATERIAL_GETDECALID`; declare `C3DPrimitive2* mesh_primitive = sub_primitive.MeshPrimitive;` AFTER
  `primitive = sub_primitive.GetParent()` (retail loads Parent first: 0xBD3184 [78h]+0xC -> [0Ch], then +0x18 -> [2Ch]);
  `material->GetDecalID()` instead of `material->DecalID`.
- $W/hdr (coordinator re-base) is built from agent4010/hdr and LACKS scout/hdr's engine/C3DMeshMaterial.h (GETDECALID +
  SELFILLUM_TRANSPARENT opt-ins: FableWin ?GetDecalID@C3DMeshMaterial@@QBEJXZ 17b/9i) and the
  FABLE_RETAIL_CENGINELOCALLIGHTARRAY_GETCOUNT block in CEngineLightingManager.h. Copy C3DMeshMaterial.h = current
  rebuild/include/engine/C3DMeshMaterial.h + those two #ifdef blocks (diff shown by `diff rebuild/include/engine/C3DMeshMaterial.h
  agent4010/scout/hdr/engine/C3DMeshMaterial.h`: pure additions). s04/s05 (GetCount, GetSelfIllumination/IsTransparent) were neutral (2071).
- 9 STALE overlay headers in $W/hdr (README) must be hand re-based before hand-back: CEngineInternalPrimitiveOcclusionBase.h,
  C3DPrimitive2.h, CEngineScreenEffectOutlineGlow.h, CSurface.h, CIVCountedPointer.h, C3DMeshInfo.h, C2DVector.h, CRenderStateManager.h, CMatrix3x4.h.

## 1. Retail shape (xdis 00bd3070 0x2c28; verified)
- `push ebp ; lea ebp,[esp-70h] ; sub esp,0E0h` = VC7.1's own biased-ebp frame for a big frame (NOT /Oy-; no pragma needed).
  push ebx/esi interleaved with the capture-block ctor, push edi at 0xBD30A1. One epilogue 0xBD5C31 `... pop ebp ; ret 8`.
- thiscall, `ret 8`; no SEH/EH frame, no /GS => `// Flags:` none; no optimize("s"). Same-TU landed siblings carry no Flags line.
- Draft frame is `sub esp,0E4h` (4 bytes too big) -- see sec. 7, `abort_render` is the prime suspect.
- Extent 11288 includes an 18-dword jump table at 0xBD5C40 (zeros for RenderMode 5,14,15; dispatch `dec eax ; jmp [eax*4+..]`, no range
  check) => `default: __assume(0);` and arm order 2,3,4,8,6,7,9,10,11,12,13,1,16,17,18 (draft already has both).
- Two fistp sites (retail_asm, need `// Retail-asm:` lines + per-site table): 0xBD3397 `fistp dword ptr [ebp+68h]` (GFFloatToLongFloor),
  0xBD5808 `fistp dword ptr [edx]` (GFFastFloatToLong inside GetAlphaIncludingDistanceFade). 5x __ftol2 = plain casts.

## 2. Identity (two sources, verified this pass)
- Leaf `?RenderMeshPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase@@QAEXAAVCEngineSubPrimitiveAnimatedMesh@@W4ERenderPassType@@@Z`:
  ghidra_out/egor_pdb_names.tsv 0x00A87E30 + egor_signatures.tsv + FableWin.exe.signatures.tsv 0x02E7E830 (6451 B / 1674 instr).
  Manifest label is RIGHT. `void RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh&, ERenderPassType)`, public, non-const, non-virtual.
- Only caller: 0x00BD7199 in landed rebuild/src/compiled/00/bd/CEnginePrimitiveManagerAnimatedMeshesBase_RenderSubPrimitive_00bd7110.cpp
  (xref.py). Param names descriptive.

## 3. Callees (landed file under rebuild/src/compiled/; PDB spelling; flags)
| addr | landed | PDB name | note |
|---|---|---|---|
| 0x00988020 | 00/98/CShaderRenderManager_ApplyVertexShader_00988020.cpp | ApplyVertexShader(const CVertexShader&, bool) | ok |
| 0x00988140 | 00/98/CShaderRenderManager_ApplyPixelShader_00988140.cpp | ApplyPixelShader(const CPixelShader&) | ok |
| 0x00989A60 | 00/98/CShaderRenderManager_SetUserVertexShaderConstant_00989a60.cpp | SetUserVertexShaderConstant(unsigned long, const C3DVector&, float) | another landed caller spells it SetVertexShaderConstant (NAME_FIXES cand.) |
| 0x00BA9360 | 00/ba/CEnginePrimitiveManagerMeshBase_GetDiffuseTexture_00ba9360.cpp | static fastcall GetDiffuseTexture | ok |
| 0x00BA95D0 | 00/ba/..._GetBumpTexture_00ba95d0.cpp | GetBumpTexture | ok |
| 0x00BA9830 | UNLANDED | GetReflectionTexture(const C3DMeshMaterial*, unsigned long, bool&, const CGraphicDataBank*, bool) (FableWin) | declare |
| 0x00BA9A70 | 00/ba/..._Get2DAlphaMaskTexture_00ba9a70.cpp | Get2DAlphaMaskTexture | ok |
| 0x00BEC950 | 00/be/..._GetOverlaidTextures_00bec950.cpp | protected static fastcall GetOverlaidTextures | ok |
| 0x00BD8480 | 00/bd/CVectorMap_LowerBound_00bd8480.cpp | CVectorMap<pair<long,long>,CEngineTextureDecal,..>::LowerBound | via inline GetTextureDecal |
| 0x00BD2D90 | UNLANDED | CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations() returns const CMatrix4x4* | |
| 0x00BCFB00 | 00/bc/CEnginePrimitiveManagerAnimatedMeshesBase_RenderPrimitiveBlocksIndexBuffer_00bcfb00.cpp (LANDED; outline said U) | private RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2&, const CMatrix4x4*) | via inline RenderPrimitiveBlocks |
| 0x00B905A0 | UNLANDED | CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest(CEngineInternalPrimitiveOcclusionBase&, ERenderPassType, unsigned long&) (IAE) | |
| 0x00B8D090 / 0x00B8D0A0 | 00/b8/E2_E2_00b8d090.cpp / E2_E2_00b8d0a0.cpp = PLACEHOLDER `C00b8d090::M` | CEngineOcclusionManager::StartOcclusionTest() / EndOcclusionTest(unsigned long) (Ego_r 0x00A347A0/B0 + FableWin) | placeholder rows; reloc_bindings say BeginQuery/EndQuery = wrong |
| 0x00B89070 | 00/b8/E2_E2_00b89070.cpp = PLACEHOLDER `C00b89070::M(int)` | CEngineScreenEffectOutlineGlow::SetupDepthBufferTransform(const CMatrix3x4&) (Ego_r 0x00A2FD30) | global 0x01436E3C misnamed g_pWaterRenderer in 00b33010 |
| 0x00B4FC40 | UNLANDED | CEngineShadowRenderer::SetupShadowFuncRenderStates(long, const CMatrix3x4&, const CColorBufferDepthPackParameters&) (nested struct) | landed 00b50510 caller spells it with pointers (NAME_FIXES) |
| 0x00B47310 | 00/b4/CEngineLightingManager_SetupShaderSpotLight_00b47310.cpp | SetupShaderSpotLight() | ok |
| 0x00B47390 | UNLANDED | SetupShaderSpotLight(const CMatrix3x4&) | |
| 0x00B46C80 | UNLANDED | UpdateGlobalLightingStates() | |
| 0x00B44F20 | 00/b4/CEngineLightingManager_SetupShaderPointLights_00b44f20.cpp | SetupShaderPointLights(const CEngineLocalLightArray&) | particle-group caller has PAV form (NAME_FIXES) |
| 0x00BF1030 | 00/bf/CEnginePrimitiveRenderUtil_SetupStencilMask_00bf1030.cpp | SetupStencilMask(const CEngineMeshEffect*, unsigned long, ERenderPassType) | ok |
| 0x00BEFD90 | UNLANDED | SetupOutlineSmoothStencilStates(const CEngineMeshEffectOutline&, float) | |
| 0x00BF01E0 | 00/bf/..._SetupOutlineSmoothAlphaStencilMaskStates_00bf01e0.cpp | (const CEngineMeshEffectOutline&) | ok |
| 0x00BF2240 | UNLANDED | SetupOutlineSmoothFillStates(const ..&, float, bool, long) | |
| 0x00BF0740 / 0x00BF0B20 | UNLANDED | SetupOutlineStencilStates / SetupOutlineFillStates (const ..&, float) | manifest labels 0x00BF0B20 wrongly as Stencil; draft spelling right |
| 0x00BF1EE0 | UNLANDED | SetupOverlayAlphaBlendingStates(unsigned long, CRGBColour) (by value) | |
| 0x00A058C0 / 0x00A05840 | 00/a0/CRenderStateManager_RealiseRenderState / _RestoreCaptureBlock | ok | |
| 0x00A0B140 / 0x009881F0 | 00/a0/CRenderManagerCore_SetWorldTransform / 00/98/CShaderRenderManager_SetWorldTransform | ok | via inline CEnginePrimitiveRenderUtil::SetWorldTransform |
| 0x00BFEA14 free, 0x00BFEA70 __ftol2 | CRT | | |
Draft spellings all agree with the PDB spellings above.

## 4. Members (retail offsets; Ego_r names; all already modelled in $W/hdr headers, no local structs in the draft)
- sub_primitive (engine/CEngineSubPrimitiveBase.h + CEngineSubPrimitiveAnimatedMesh.h, opt-in _ACCESSORS): RenderFlags +4, Alpha +8,
  Parent +0xC, MeshPrimitive +0x18, RenderMode +0x1C.
- parent CEngineInternalPrimitiveAnimatedMesh (_ACCESSORS, _ORIGINAL_CACHE): vslot 32 (+0x80) GetMorphInfo, EnginePrimitiveFlags +0xC bit7,
  AlphaRef +0x28, MeshInfo +0x50, Scale +0x58, TintColour +0x5C, MeshEffect +0x68, ThemeChannelDesc +0x6C, RenderInfo +0xE0
  (Lights +0xE0, Mesh +0xE4, ObjectTransform +0xEC, Distance +0x11C).
- C3DMeshMaterial (0x30): DecalID +0xC (use GetDecalID()), DiffuseMapBankIndex +0x10, SelfIllumination +0x24, Transparent +0x29.
- C3DPrimitive2: BoundingSphere +0xC..+0x18, AverageTextureStretch +0x1C, VertexBuffer +0x30, InitFlags +0x68 (IsDP3Bump).
- this (0x690): PointLightShaderIndex +0x20, VS tables +0x38/+0x58/+0x2A8..+0x30C, TextureOverlayFadeDistandeFactor +0x32C (PDB typo kept).
- lighting mgr: ThemeChannels +0x3C, FadedShadowDiffuseFactorEnabled +0xCE, CurrentThemeChannel +0xE0, StateUpdateFlags +0xE4,
  SelfIlluminationAmbientPercentage +0xE8, ShadowMode +0x4694 (FADEDSHADOW_MODE_CHECK opt-in, already defined in draft).
- Full table + opt-in names: prior/scout/headers.md sec. "Members read" and "Define / include block".

## 5. Visibility
- Same TU / neighbours: RenderPrimitiveBlocks (uncalled OOL copy 0x00BD1EE0 = `jmp 0x00BCFB00`) -> inline forward; GetTextureDecal
  (landed OOL 0x00BD8800, `#pragma auto_inline(off/on)` sibling) -> FORCEINLINE opt-in _GETTEXTUREDECAL_FORCEINLINE + CVECTORMAP_FORCEINLINE_FIND
  (LowerBound stays a call); GetAlphaIncludingDistanceFade (landed OOL 0x00BA7B30) -> _ALPHAFADE opt-in; GetAppropriateMipmap (FableWin
  ?GetAppropriateMipmap@CEnginePrimitiveManagerMeshBase@@SIK... fastcall) expanded inline -> _GETAPPROPRIATEMIPMAP opt-in (draft defines the
  body in-row). GetReflectionTexture / GetBumpTexture called OOL (182/138 B debug) -- keep as calls.
- No STLport map/tree calls; CArray<const CTexture*> dtor expands to `free`.

## 6. Siblings
- 00/bd/..._RenderSubPrimitive_00bd7110.cpp, _CreateInternalPrimitive_00bd6df0.cpp, dtor _00bd7ef0.cpp: no Flags, no pragmas.
- 00/bd/..._GetSupportedTypes_00bd27f0.cpp and CEngineInternalPrimitiveMorphInfo_GetTextureDecal_00bd8800.cpp: `#pragma auto_inline(off/on)` only.
- 00/bc/..._RenderPrimitiveBlocksIndexBuffer_00bcfb00.cpp: SCOPED_PTR / CINDEXBUFFER / CRENDERMANAGERCORE / VSCONSTANTLAYOUT opt-ins.
- Static twin TU 00/ba/CEnginePrimitiveManagerStaticMeshesBase_DisplayMirroredTextures_00ba1b00.cpp: CRGBFLOATCOLOUR,
  CRENDERMANAGERCORE_FORCEINLINE_HELPERS, CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR, ...SHAREDDATA_MEMBERS (FlushRender + SetTextureFactor bodies).
- Matched family: /opt/fable_lane/w22/agent2048/src/00b64fa0.cpp (RenderClouds, capture block, no Flags), agent3015 DrawSilhouette 0x00BA6FF0.

## 7. Likely model errors (draft vs retail)
1. `bool abort_render` (draft l.339, set in 6 arms, tested l.570): retail has no such variable. 0xBD4E7B tests the out-arg bool
   `[ebp+6Fh]` directly (`mov al,[ebp+6Fh] ; test ; je 0xBD549A`); arms that cannot set it (pass 8 0xBD5258, pass 10 non-alpha 0xBD53EC,
   pass 12 0xBD5444, pass 13 0xBD5498, no-pass 0xBD5449) jump straight to 0xBD549D -- compiler jump-threading of a known-false `failed`.
   Write `if (failed) { FlushRender(); RestoreDefaultPrimitiveParameters(); return; }` after the pass chain. Prime suspect for
   `sub esp,0E4h` v 0E0h (4-byte local too many).
2. Combined-pass texture factor (draft l.518-519 `tint.SetA(PeekAlpha()); SetTextureFactor(CRGBFloatColour(tint));`): retail 0xBD4DDE
   reads R from tint byte [ebp+6Ah], G/B from the early byte copies [10h]/[14h], and Alpha straight from `sub_primitive.Alpha` ([78h]+8,
   0xBD4DE5), each `fild ; fmul 1/255` -- no store into tint.A. Try `CRGBFloatColour colour(tint); colour.A = sub_primitive.GetAlphaAsFloat();`
   or a 4-float ctor with `tint.R/255` etc. Region 0xBD4DA3..0xBD4E7B had +33 extra instructions in our build (sd_s00).
3. Start order: Parent before MeshPrimitive (s03 fix), and GetDecalID() accessor (s01 fix) -- not in the draft.
4. Remaining big hunk regions (sd_s00 per-region counts): switch arms 2-13 (78 hunks), pass1/outline 0xBD505C..0xBD549A (30 hunks,
   -54/+50), setup/distance (18). Outline note: pass 1 test is `PeekGraphicInfoFrame(i,0)->GraphicInfo->TransparencyType` (cmp/sbb/and),
   draft uses IsAlpha() -- check its text gives the same. Pass 10 alpha arm shares one `mesh_primitive->IsDP3Bump()` (`setne ; inc` stage count).
5. Tail: retail early-return does FlushRender BEFORE RestoreDefault, normal end AFTER (draft matches). Keep the same `shader_flags` local
   unmodified between the two PrepareForRender expansions (2nd copy reuses frame caches).
