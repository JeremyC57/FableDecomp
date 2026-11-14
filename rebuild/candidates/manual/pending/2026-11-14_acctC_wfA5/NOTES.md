# wfA5 notes

## 0x00B555A0 CEngineWeatherRenderer::Render -- RELOCATION_MATCH, test PASS, addmatch done
- Matched from a hand-written draft (the dossier's outline) on the DEFAULT SetRenderState text: NO SETRENDERSTATE_DESIRED, NO
  CRENDERSTATECAPTUREBLOCK_INLINE (the RenderClouds levers make it differ: DESIRED 11 hunks, DESIRED+BOOKMARK 15, ADDSTATE_DIRTY 11).
  `if (!PeekRenderEnabled()) return;` (FableWin accessor) and direct `RenderEnabled` under CENGINECOMPONENT_PROTECTED both match; Peek kept.
- Scope objects capture, shaders(true, true), layout(1). Opt-ins: CENGINECOMPONENT, CENGINEWEATHERRENDERER(+_BUFFERS, _RENDERRAIN,
  _RENDERSNOW, new _RENDER), CENGINEWEATHERSETTINGS_TAIL, CRENDERMANAGERCORE. No flags. Header: new opt-in in hdr/engine/CEngineWeatherRenderer.h.
- vland (read-only): RELOCATION_MATCH PASS. Warnings: RELOC_CONFLICT D3DAlphaFuncModeTable -> 0x01396DD8 here (symbol + 0x10 for
  ALPHA_TEST_GREATER), 0x01396DE8 in landed 0x00BBC460 (that landed row binds the element address to the symbol name: coordinator check);
  RELOC_NAME D3DTextureUVModeTable 0x01396DC8 labelled g_textureUVModeToD3D (label vs PDB name; ours is the PDB static member name).
- EARLY_RETURN opt-in variants spun cl until timeout (probably an unbalanced #if in that branch): not used.

## 0x00598A1C NUISystem::CFrontEndManager::BuildComponents(bool) (catalog "Init2") -- RELOCATION_MATCH, test PASS, addmatch done
- Name proven: FableWin.pdb ?BuildComponents@CFrontEndManager@NUISystem@@QAEX_N@Z + the sole caller 0x0042EA62 in landed
  CNewFrontendGameComponent::Init2 0x0042E98F. Length 4928 confirmed (code + 17-entry jump table; next function 0x00599D5C).
- Matched at grade ~8 from a hand draft in the twin's (OpenPCOptionsMenu 0x0058AF10) spelling. The two closers: (1) the first-screen tail is
  `if (!m_bQuitFromGame) { if (show) InitScreenStack(map[-1]); else InitScreenStack(map[20]); } else { LoadProfile(..); InitScreenStack(map[0]); }`
  (a ternary key gives neg/sbb; quit-first order lays the arms wrong); (2) the redefine-keys block uses two named locals `accept` then
  `redefiner` (accept alone named sits in ebx; both inline in one call fixes the frame but not the block). FITTED: `redefiner` (made-up name).
- `#pragma optimize("s", on)` (twin), no flags. Opt-ins: twin's set (CCHARSTRING, CWIDESTRING, STLP_LIST, STLP_MAP, CCOUNTEDPOINTER_CALLED_CTOR,
  _RELEASEDATA, CREDEFINERLIST_SETACCEPTBUTTON, CUSERPROFILEMANAGER_DISPLAYMODES + _SAVENAMES (GetProfileName), CDISPLAYMANAGER(+_ANTIALIASING),
  CTEXTURE, CPIXELFORMAT, SCOPED_PTR, C2DBOXF, C2DVECTOR, DEF_CLASS_BASE, CENGINEPERSISTENTPRIMITIVEBASE), CFRONTENDMANAGER_DESTRUCTOR + new _BUILDCOMPONENTS.
- vland: RELOCATION_MATCH PASS; RELOC_NAME warnings only (landed labels GetAnimFilter for GetDisplayModes 0x00406880, SetMember_dword0x18c for
  SetAcceptButton 0x00556570, list ctor/dtor spelled as _List_base by the header) -- the PDB spellings are ours.

## 0x00AA0FA0 C3DAnimationManager::CalcBoneTransformation -- RELOCATION_MATCH, test PASS, addmatch done
- Built from the landed CalcMeshTransformations 0x00A9F540 (helper block verbatim) + ApplyParentTransformations' opt-ins; ~70 grades.
- Closers in order: SetPosition through direct stores / Initialise instead of a C4DVector temporary (-192 bytes), `scale.Initialise(1,1,1)`,
  SWAP(current, other) BEFORE keyframes[j].ToMatrix, `bone_index` named before the keyframe reference (IsBoneEnabled SIB), the parent matrix
  as ONE function-scope aligned local (frame 0x214 and the in-loop store scheduling), and finally SetPosition = `Position.Initialise(position,
  0.0f)` (FableWin-size-supported; closes the BlendWith dot / slerp and x87 AlignedSetMultiply operand orders, which are keyed on the IL of
  the unrolled scaling loop -- dead-local rulers 1..48 never moved them; a diagnostic without the matrix section did).
- Overlay opt-ins: C3DANIMATIONMANAGER_BONETRANSFORM, C3DBONE2_GETNAMECRC, CMATRIX4X4_COPYTO, C3DKEYFRAME_SETPOSITION, new header
  engine/C3DSkeletalMorph.h; also C3DVECTOR_GETSCALED_NAMED_RESULT exists in hdr but is NOT used by any matched row (FableWin 34i v 32i).
- No flags, no pragmas beyond the twins' `#pragma intrinsic(sqrt/fabs)`. Retail-asm lines cite the twins' per-site tables (coordinator: confirm
  that is enough for these new sites, else a per-site table run is needed).

## 0x00AA2110 C3DAnimationManager::CalcBlendedBoneTransformation -- RELOCATION_MATCH, test PASS, addmatch done (successor agent)
- CLOSED (successor): under the twins' shared helper texts (SetPosition = Initialise, plain GetScaled) the row matches with ONE row-local
  spelling: `keyframes[0].ToMatrix(matrix_a, false);` instead of `(*current, false)` (FITTED: same code; c2 record-id keys). A manual
  `CMatrix4x4* previous = current; current = other; other = previous;` instead of SWAP also matches (not used: SWAP has FableWin evidence).
  Both spellings flip the twin 0x00AA0FA0 to 13 hunks, so the twins keep different spellings here. Measured on the way: head named-local
  sweep (64 variants: blend_count / start_time / start_alpha / relative / keyframe refs / index): all 13 (named locals never move these keys);
  scaling loop with `keyframes[j - 1]` indexed twice: BlendWith closes, AlignedSetMultiply stays, remainder-loop schedule changes (6 hunks);
  W-first SetPosition + double index: the reverse. The residue was two independent order bits (BlendWith, AlignedSetMultiply); the matrix
  section's spelling toggles both. ~150 grades total in c4/ (sweeps s1..s6). vland RELOCATION_MATCH PASS. Old differ in tmp/00aa2110.differ_old.cpp.
- (history of the first agent follows)
- Draft = row 3's text with CalcBlendedMeshTransformations 0x00AA0090's blended head; matched at the 2nd grade WITH SetPosition written as
  W-first direct stores (`Position.W = 0.0f; X; Y; Z;`) -- that exact file is kept as tmp/00aa2110.matched_with_Wfirst.cpp.
- Twin conflict (one helper text for both rows): SetPosition x GetScaled sweep (I = Initialise, W = W-first, X = XYZW direct; P = plain,
  N = named-result GetScaled): I/P r3 MATCH r4 13; I/N r3 13 r4 MATCH; W/P r3 13 r4 MATCH; W/N r3 3 r4 12; X/P r3 18 r4 5; X/N r3 8 r4 17.
  Chosen I/P (Initialise has FableWin size support; GetScaled plain is the landed 0x00A89740's text). Residue under I/P: BlendWith Dot order
  (retail Y,Z,W,X; ours X,Y,Z,W, 0x00AA2750..0x00AA2771), slerp products 0x00AA292C..0x00AA296F, x87 AlignedSetMultiply first column
  0x00AA31CA..0x00AA31E7 -- all keyed on the function's IL amount. Tried for row 4 under I/P: scaling statement forms (direct, named scaling
  ref, named position): no match. Next ideas: HasSkeletalMorph text fit (FableWin 24i/72b; ours 26i/78b; candidates 25i/75b tried by fwsize
  only), GetPosition / CopyTo alternatives (both SAME already), row-4-only natural names in the blended head (start / end state refs).
- Later sweep (HasSkeletalMorph texts H2 `Data.Get() != 0 && Data->SkeletalMorph.Get() != 0`, H3 `if (Data.IsNull()) return false; return
  !Data->SkeletalMorph.IsNull();`, H4 `!Data.IsNull() && Data->SkeletalMorph.Get()`): I+orig r3 M / r4 13; I+H3 M / 13; I+H4 13 / M;
  W+H3 13 / M; W+H4 M / 13; H2 breaks both. Every shared-helper lever flips BOTH twins (a parity-like key): the twins need a ROW-LOCAL lever.
  Row-4-only spellings tried without effect: named PeekSkeletalMorph reference, named chain bone in the scaling-target loop, scaling statement
  forms, dead locals; dropping the end_state reference changes code (54 hunks). Next: row-local IL inside the unrolled scaling loop or the
  partial-alpha loop of row 4 only (e.g. `const C3DVector& scaling = scales[j]` was tried: 13), c2 record-id trace (r3010 model.py) if available.

## 0x00B9AC30 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive -- successor: DIFFER(4229v4229) 19 hunks, near-miss (differ/00b9ac30.cpp, work in b9/)
- Successor compile fixes: GetRepeatedMeshDrawDistanceFactor() (+0x88, landed RenderPrimitive's spelling), GetRenderInfo()->Mesh->GetMaterial,
  bank->PeekGraphicInfoFrame(index, 0) (landed 0x00BA4A40 body, defined in the row), new opt-ins CENGINECAMERA_PEEKWINDOW (FableWin
  PeekWindow), CENGINESUBPRIMITIVEMESHBASE_PUBLICMEMBERS, CENGINEVSCONSTANTLAYOUTREPEATEDMESH (retail class branch) +
  CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTREPEATEDMESH (replaces the draft-only REPEATEDMESH_BATCH stand-in, now deleted from hdr);
  existing opt-ins SUBPRIMITIVEBASE_PARENTBASE, RESOURCEMANAGER_GETSTIPLEFADETEXTURE (STIPPLE dropped), CSHADERRENDERMANAGER_INLINE_BLOCKDRAW,
  LIGHTINGMANAGER_SETFADED_SHADOWGUARD (replaces PEEKGLOBALSHADOWMODE), C2DBOXF + C2DBOXF_COPY; `// Flags: /GS` (cookie at 0x00B9AC36).
- Structure closers: duplicated assignment blocks (count == ObjectCount arm and the no-subsections arm, no flag), `++n, ++j` in the copy
  loops, batch loop `for (done...) { remaining = count - done; n = remaining >= batch ? batch : remaining; ... }`, window copied to a local
  `const C2DBoxF window = PeekWindow()` (member-wise copy: fixes frame size 0xC4 and the stipple x87 code).
- Then: C3DPrimitive2 accessors (FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH: GetMaterialIndex / GetVertexBuffer / GetIndexBuffer / GetVertexCount /
  GetTriangleCount) closed the draw loop (late imul) and FlushRender (37 -> 19 hunks). Names: NEngineInternal::GPOcclusionManager / GPLightingManager /
  GPCamera (reloc_bindings PDB names) instead of the RenderPrimitive sibling's g_pEngine*; CClipListElement fields Start / Size (Ego_r).
- Residue (19 hunks, equal length): (1) copy-loop bound re-read `clip.Start + clip.Size` loads Size first (0x00B9AEB5 / 0x00B9B001) + ebx/ebp
  restore order after the inner loop; (2) the inlined GetTexture / TouchResource temps (0x00B9B11E..0x00B9B1C7, 0x00B9B2C4): ours edx/eax where
  retail (and the landed out-of-line 0x00B66C10, same text) eax/ecx -- allocation order; (3) SetRenderState `Flags |= mask` operand order at the
  2nd and 3rd sampler setters (0x00B9B4C3, 0x00B9B534; 1st and 4th right). 0x00B9B26B / 0x00B9BC87 are relocation-only.
  Tried without effect: dead-local rulers 1..20 (24+ adds spill hunks), bound spellings (pointer, indexed, cast, swapped), bank / frame / mesh
  locals, `bool loaded`, texture declared earlier, GetTexture text with GetBank() (kept: = landed text), CGraphicFrameData::GetTexture via
  PTexture.Get() (probe hdr only), wrapping the rest in `if (GetTexture)` (frame +4: worse).

- Name: brief/dossier right (Ego_r 0x00A531E0 + FableWin + vtable 0x012A3E9C slot 5); the workflow task text calling it a constructor is wrong.
- Draft written from the dossier outline over the landed RenderPrimitive 0x00B9A4A0 preamble (its define list), the FlushRender text of the
  landed 0x00BA1B00, and new overlay opt-ins: REPEATEDSTATICMESHES_RENDERSUB (RenderSubPrimitive + 5 private helper decls, the Ego_r
  CSIMDVariableBlock, NEnginePrimitiveManagerRepeatedStaticMeshes::CClipListElement), OCCLUSIONBASE_PREPARETEST, SUBPRIMITIVEREPEATEDSTATICMESH_
  GETPARENT, OCCLUSIONMANAGER_TESTS (Start/EndOcclusionTest), CGRAPHICINFOFRAME_GETTEXTURE_INLINE (the landed 0x00B66C10 text as a forced
  inline), CENGINESUBPRIMITIVEBASE_PEEKRENDERFLAGS (FableWin PeekRenderFlags), CENGINERESOURCEMANAGER_REPEATEDMESH_BATCH (DRAFT stand-in only).
- Compile items left: see the differ file's Grade line. The resource-manager VSConstantLayoutRepeatedMesh needs a retail-offset typing
  (retail reads the ObjectMatricies count at GPResourceManager+0x3034; header layout is the Ego_r 0x30F8 one) -- FableWin accessor
  ?PeekVSConstantLayoutRepeatedMesh@CEngineResourceManager@@QAEABVCEngineVSConstantLayoutRepeatedMesh@@XZ.
- Expected residue classes after it compiles (dossier): /GS frame packing of 0xC4 bytes with heavy slot reuse, copy-loop forms (countdown
  over clip entries, bound re-read), batch-loop pointer strength reduction, faded-shadow bool spelling (`!(flags & 0x200)` zeroed unless
  ShadowMode == 1: SetEnableFadedShadowFactor's own guard), x87 stipple constants (float * then + double 0.015625).
