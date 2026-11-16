# wfA6 NOTES (acctC lane, author: wfA6)
Result: 0 of 5 rows matched; all five graded and parked (see STATUS.tsv). Overlay additions in hdr/README.md (wfA6 section).
Tools written: tools/qs.sh (grade to tmp/<addr>.ret/.our), dd.sh, mdiff.sh (frame-masked diff), fmap.py (frame-offset map),
terms.py (x87 term order per store), sweep.sh (one line per variant), calls.sh <addr> <leaf> (leaf call targets).

## 0x00ADE080 C3DBoundingBox::InitialiseFromOBB -- parked, differ/00ade080.cpp, DIFFER(2733v2735)
- Name: Ego_r.pdb 0x00BCFDA0 + FableWin.pdb 0x03313830 ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z. Extent
  0x00ADE080..0x00ADEB2F (ret 8 at 0x00ADEB2C) = 2735, correct (the old draft's "carries InitialiseFromSphere" note was wrong for
  this oracle: InitialiseFromSphere starts after 0x00ADEB2F).
- SSE arm: 0x00ADE329..0x00ADE3B1 is original inline asm (pointer local homed + re-read `mov [esp+24h],eax ; mov eax,[esp+24h]`,
  fixed xmm0..xmm5 schedule, loads folded through eax). Isolated as NEW shared helper hdr/engine/C3DBoundingBoxOBBSSE.h
  GF_SSE_BOX_FROM_OBB_CORNERS(corners, centre, minimum, maximum) with a FABLE_PORTABLE twin; the SSE arm is now instruction- and
  frame-identical to retail (only reloc masks differ). Dual-path test NOT yet written (needed before landing).
- What fixed the frame (0xF4 -> 0xE4, every frame word now maps): the x87 arm spelled as temporaries
  `MinCoord.LimitUpper(centre - v0); MaxCoord.LimitLower(centre - v0);` (retail re-stores the CSE'd centre-v0 into the same temp
  slot for the second call, 0x00ADE6FB..0x00ADE703) instead of a named `p`; `MaxCoord = centre + v0; MinCoord = MaxCoord;`.
  Also FABLE_RETAIL_CMATRIX3X4 (forceinline TransformPoint: retail has one call only, 0x00A5CB90) and
  CMatrix4x4::InitialiseTransposed (FableWin name; new opt-in FABLE_RETAIL_CMATRIX4X4_INITIALISETRANSPOSED, FABLE_FORCEINLINE:
  plain inline was called out of line, retail expands it).
- Residue (EVIDENCED as term order only): x87 operand order in the four TransformVector expansions (retail v0 X row x*E11,y*E21,z*E31;
  v1..v3 X rows x*E11,z*E31,y*E21; Y rows y*E22,z*E32,x*E12), the TransformPoint X row (retail x*E11,z*E31,y*E21,+E41) and the
  x87-arm GetCenter add (retail Max.X first). Tried (no change or worse): v1..v3 copy-init, `half` + v0..v3 ctor, centre declared
  early, v0 direct-init, `*= 0.5f`, `/ 2.0f`, if/else instead of early return, &corners[0], p declared before min/max, const half,
  v0 assigned after declaration, GROUPED_X TransformVector, ToC3DVector vs explicit C3DVector (explicit is better: 2733 v 2739).
- Later in the run (75 grades): FITTED grouped bodies FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_GROUPED_XZY and
  ..._TRANSFORMPOINT_GROUPED_XZY took the masked diff 77 -> 43 (all X / Z rows of v1..v3 and the centre X row now equal). Measured:
  VC7.1 reorders flat sums and the inner pair of a grouped sum whatever the source order (yy/zz declaration order, E-first products,
  inner pair spelled either way: identical bytes); only the explicit grouping survives. Left: Y rows (retail z*E3k first, ours y*E2k
  first; v0 Y row matches) and the v0 X row ((x+y)+z in retail).
- Caution on the fitted grouping: retail's v0 X row is (x+y)+z and its v1..v3 X rows (x+z)+y, which no single grouped body can give;
  so the original TransformVector is most likely the flat landed body and the real residue is VC7.1's canonical reordering of a flat
  sum, which differs between retail and our build in a consistent "rotated" way (flat body: ours v0 (y+x)+z, v1 (x+y)+z). Not
  moved by: yy/zz/xx declaration order, E-first products, FABLE_FORCEINLINE TransformVector, vector construction spellings. The
  GROUPED_XZY opt-ins are kept as the best measured state (43 v 77 masked lines) and marked FITTED; drop them if a flat-body
  lever is found.
- Helper test: test/test_C3DBoundingBoxOBBSSE.cpp (for rebuild/tests/portable/): portable path PASS (g++ -DFABLE_PORTABLE); the
  asm + twin dual path PASS under VC7.1 (vt.py run of a copy with FABLE_DUAL_PATH_TEST and an `00ade080_TEST PASS` line).
- No behaviour test for the row itself yet (the row is not matched).
- Next (older): named-local sweeps (lesson C40 0x00BD2D90: reassociation keyed on symbol numbers), e.g. names of SSE-arm locals, a
  `C3DVector half` in the x87 arm feeding v0..v3, centre/GetCenter spelled via MinCoord+MaxCoord; TransformVector body variants
  as a new opt-in (fitted).

- Successor (wfA6b, 2026-11-14, ~50 more grades, probes in probe/r1): (1) dead-local rulers (k=1..32 `float dz=obb.MinCoord.X;`,
  k=1..12 `const C3DBoundingBox& dz=obb;`, at function top or at the x87-arm start: identical results) on the FLAT body never give
  retail's orders (termmis >= 12) -- expected: a uniform record-id shift can only ROTATE the cyclic order of an add chain's keys, and
  retail's v1..v3 X row (x,z,y) is not a rotation of ours (x,y,z); so the RELATIVE record ids differ. (2) Proven: the transform's
  field records are shared by both arms: reading m in row-major order inside InitialiseTransposed (probe/h1 overlay) changes the
  x87-arm orders (flat termmis 12 -> 11) but breaks the SSE arm's load/store order (retail's = today's text). (3) Arm order swapped
  (`if (!g_SSEAvailable) { x87 } else { SSE }`, or x87 + early return): x87 laid out first, 487 masked lines -- not retail.
  Open lead: the creation order of the E-field / vector-field records (not the source order of the sums) decides the orders; a
  spelling that keeps the SSE copy order but creates the E records in another order (or the v1..v3 field records in another order)
  is the remaining lever. Kept parked.

## 0x00C79FD0 ResampleStereoMMX -- parked, differ/00c79fd0.cpp, DIFFER(2884v2884), 151 difflines (register colouring only)
- What it is: NOT hand asm and NOT a vendor .lib object: compiler-scheduled MMX intrinsics through the Intel ivec.h class library
  model (M64 / Is32vec2 / Is16vec4 + mul_add / unpack_* helpers), the fourth of the libpk resampler family whose three siblings
  are landed as compiled C++ rows with the same class text: ResampleMonoMMX 0x00C78A10, ResampleMonoRampMMX 0x00C79440,
  ResampleStereoRampMMX 0x00C7AB20 (and the out-of-line ivec copies 0x00C78590..0x00C78970 of this TU). The catalog label
  Audio_ConvolveFilter_WithGain is wrong: the row is ?ResampleStereoMMX@@YAXPBFKKKPAJ1G@Z, the spelling bound in
  rebuild/oracles/reloc_bindings.tsv and declared by the landed callers MixVoiceFadeIn 0x00C70240 / MixVoiceFadeOut 0x00C702E0 /
  MixVoice 0x00C70390 (descriptive: libpk has no PDB). Grade family: genuine (plain C++ + intrinsics), Flags /GS like the siblings.
- Residue: in all four switch cases the last s3 add: retail `paddd mm0,mm1 ; movq mm1,mm0` (s3 in mm1, s1 / pan reloads in
  mm2), ours `movq mm2,mm0 ; paddd mm2,mm1` (s3 in mm2, reloads in mm1). Equal length, equal slots.
- Tried this run (all identical swap or worse): s0..s3 declared at loop top; one reused `a` for both halves; a and b both computed
  before the outputs (worse, 2892); the preceding TU functions (0x00C78A10, 0x00C79440 landed bodies) placed above the row (TU
  numbering probe: no change). The earlier lane's 54 grades (pan/output spellings, scopes, raw intrinsics, += chains, loop forms,
  /d2inl probes) are listed in ref/00c79fd0/draft.cpp. Parked as a register-colouring residue per WF_RULES.

## 0x00AE3C30 CParticleProgram::ReadFromFile -- parked, differ/00ae3c30.cpp, DIFFER(2278v2310)
- Name: Ego_r.pdb 0x00BF2EE0 + FableWin.pdb 0x033351A0 ?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z.
- Ported the chunk-850 draft onto today's headers: overlay opt-ins FABLE_RETAIL_CPARTICLEPROGRAM_READ (typed members + AllocateArrays /
  ReadGroups / ReadFromFile decls; needs today's FABLE_RETAIL_CPARTICLEPROGRAM_RENDERACCESS), FABLE_RETAIL_CDATAINPUTSTREAM_COPYTO
  (decl), FABLE_RETAIL_C3DCLOTHRENDERVERTEX_CTOR (FableWin ??0C3DClothRenderVertex@@QAE@XZ empty ctor: retail never writes the
  resize(n) temp; removed our `mov [esp+2Ch],0`); today's FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVE (Clear) and
  FABLE_RETAIL_CPARTICLESIMULATION_READFROMFILE replace the stale CLEAR / READ names. All 40 calls now equal retail's, in order.
- Residue: register allocation in the second half (from 0x00AE40CB, after `pool` dies): retail enregisters constant 0 in ebx (used
  by every inline CDataInputStream::Read range check, both long() resize temps 0x00AE418B / 0x00AE4275, the loop test 0x00AE4423)
  and reloads `this` from [esp+10h] at each member access; ours CSEs `this` into ebx (0x00AE416F) and uses immediates. Frame 0xC v
  0x10 follows (retail's never-written C3DTriangle2 temp shares this's slot).
- Tried (no byte change at all): loop counter declared at function top / before the loop / initialised early; resize(n)
  specialisations given visible noinline bodies instead of declarations. Earlier lane tried the fill-temp hoist.
- Next: find the IL fact that makes 0 a CSE candidate in retail (a zero-valued named local or member read spanning 0x00AE40CB..
  0x00AE4423?), or a different spelling of the CArray reads (Read(GetAsCArray(), size()*sizeof) through a CArray member helper).

## 0x00BD5C90 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer -- parked (draft not landable), differ/00bd5c90.cpp
- Name proof: vtable slot 8 (word 11 of the vtable at 0x012A78DC: RTTI COL 0x0135DDDC, slot 5 = ?RenderSubPrimitive@
  CEnginePrimitiveManagerAnimatedMeshesBase, slot 4 = FrameEnd) = RenderPrimitiveToShadowBuffer in rebuild/include/engine/
  CEnginePrimitiveManager.h's retail slot order; Ego_r.pdb 0x00A8AB30 / FableWin.pdb 0x02E81680
  ?RenderPrimitiveToShadowBuffer@CEnginePrimitiveManagerAnimatedMeshesBase@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@@Z;
  `ret 4`, one reference argument. The batch task line calling it "constructor" is wrong (the ctor is the landed 0x00BCFF10).
- The earlier lane's draft regrades DIFFER(2935v2935) (170 diff lines) on today's headers, but carries ~25 local class models where
  shared headers exist; it needs a full port to engine/CShadowClipVolume.h, CShadowRendererSceneData.h, CEngineShadowRenderer.h,
  CEnginePrimitiveManagerAnimatedMeshesBase.h, CEngineInternalPrimitiveAnimatedMesh.h etc. (idioms: landed DrawSilhouette 0x00BA6FF0
  opt-in set) before residue work means anything. Not started this run (context budget spent on rows 1-3).

- Successor (wfA6b): FULL PORT DONE onto shared headers (no local models, no /d2inl flag), reusing row 0x00BA6240's opt-in set:
  differ/00bd5c90.cpp grades DIFFER(2948v2935), all 32 direct calls equal retail in order, frame 0x2D4 = retail, 76 masked /
  197 unmasked diff lines. New header work: CEngineInternalPrimitiveAnimatedMesh.h opt-in _RENDERINFO (SetupMesh decl + FableWin
  GetRenderInfo; needs _ORIGINAL_CACHE), new headers engine/CEngineInternalPrimitiveMorphedAnimatedMesh.h (Morph +0x128) and
  engine/CEngineInternalPrimitive2DMorphedAnimatedMesh.h (Morph +0x150), CEngineInternalPrimitiveMorphInfo moved into its own
  engine/CEngineInternalPrimitiveMorphInfo.h (CEngineRenderInfoStaticMesh.h redefines CEngineRenderInfoMeshBase, so the static-mesh
  header cannot share a TU with the animated one). What moved it: the morph branches written without a shared reference (retail
  has two GetHiddenMaterials expansions); the C4DVector constant in its own { } block (frame 0x2E4 -> 0x2D4, retail puts it at
  +0x30 over a dead slot); FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO (virtual slot 33 = +0x84 for
  CalcInterpolatedObjectTransform, as the landed 0x00B39080); FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR (centre transform, FITTED:
  PARTICLE_POINT / ZYX_SUMS give the same bytes); named `float mesh_radius` for SetupMesh (retail pushes both args from frame
  homes); radius as `GetBoundingSphere().Radius + GetBoundingSphere().GetCenter().GetMagnitude()` (retail loads Radius first and
  pops a leftover copy).
  Residue: (1) `add eax,28h` -- ours forms &Center once and addresses x/y/z off it, retail addresses MeshInfo+2Ch.. directly (13
  extra bytes with the two loop-alignment pads it shifts); GetMagnitude text variants (XY_THEN_Z, SQUAREDMAGNITUDE_XYZ/ZYX) do not
  remove it; (2) the inline SetRenderState expansions of the stage-0 filter/UV calls swap eax/esi in `test`/`or`; (3) the constant
  block pushes its address after the stores in retail; (4) a 0x2814/+8 load pair order in an inline SetRenderState.

## 0x00BA6240 CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer -- parked (draft not landable), differ/00ba6240.cpp
- Name proof: vtable slot 8 (word 11 of the vtable at 0x012A50DC: COL 0x0135C964, slot 5 = ?RenderSubPrimitive@
  CEnginePrimitiveManagerStaticMeshesBase); Ego_r.pdb 0x00A5D4E0 / FableWin.pdb 0x02E9FE30 same mangled name as the catalog.
- The draft no longer compiles: its FABLE_RETAIL_CMATRIX3X4_MULVECTOR (inline operator* with translation) is gone from today's
  header, and it contradicts today's FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR (operator* out of line at 0x00A56530, 3x3 part
  only). Respelled as in-place TransformPoint (ZYX_SUMS) it grades DIFFER(3035v2937). Also carries ~20 local class models and stand-in
  bodies (SetupFirstTextures is now landed at 0x00B9D670): needs the same full port as 0x00BD5C90.

- Successor (wfA6b): FULL PORT DONE onto shared headers (no local class models, no /d2inl flag): differ/00ba6240.cpp grades
  DIFFER(2937v2937), all 32 direct calls equal retail in order; 72 diff lines with frame offsets + relocations masked
  (tools/mdiff2.sh), 207 unmasked. Header work (hdr/README.md, wfA6b section): agentC22 render headers copied whole (checked purely
  additive vs today's repo with tools/addonly.py); new opt-ins CSHADOWCLIPVOLUME_CLIPSPHERE (FableWin ClipSphere(const C3DVector&,
  float), range test through offset.Dot(offset) -- FITTED among 6 spellings), CENGINESHADOWRENDERER_PEEKMINWCLAMP,
  CENGINEINTERNALPRIMITIVEMORPHINFO_HIDDENMATERIALS (typed set + CArray, FableWin GetHiddenMaterials, FABLE_FORCEINLINE),
  CENGINEPRIMITIVERENDERERSHAREDDATA_SHADOWBUFFERSHADERS (nested in _SILHOUETTES), CENGINEPRIMITIVERENDERUTIL_SHADOWBUFFERSHADERS
  (FableWin getters), CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE (ctor; no out-of-line copy anywhere), CGRAPHICINFO_GETWIDTH merged
  from agentC22; re-used CSCOPEDSHADERSENABLE_FORCEINLINE, C3DTEXTURESTAGES_CTOR (agentC13 text). Same-TU inline bodies in the row
  (DrawSilhouette / DisplayMirroredTextures / 0x00BB3E80 idioms): GetAppropriateLOD, GetAppropriateMipmap, FlushRender, PeekGraphic,
  SelectVertexBuffer, CalcDistanceFromCamera (FABLE_FORCEINLINE: plain inline was called), RenderUtil getters.
  What moved it: named `float distance` (2921 -> 2934, as DrawSilhouette); Dot range test; C3DTextureStages declared BEFORE the
  mipmap local (2934 -> 2937 = retail length).
  Residue (v15): (1) frame: our `bool used_blank_textures` sits at ebp-1 (retail -0x2D, after the loop offset), shifting radius /
  hidden (retail -4) and the stages block by 4; declaration position (function top, after mesh, before loop, loop top) does not move
  it. (2) retail drops the ctor's dead DiffuseTextureStage = -1 store (stores only Bump -1 + blank 0 before `jne`); ours keeps it
  (`or ecx,-1` + two stores). Tried: `Diffuse = 0` before the if (no else): -1 killed but the 0 stays before the branch (retail has it
  in the else arm); separate if/else; ternary on a named bool: worse. (3) the C4DVector constant: retail copies MinWClamp with
  integer moves (assignments spelling; better than the 4-float ctor). (4) small scheduling (a +0x28D0 load, this/material slots).
- (0x00BA6240, later) EVIDENCED fix of residue (2): retail reads the shadow technique BEFORE the C3DTextureStages ctor (a named
  `technique` local ahead of the declaration): the ctor's dead Diffuse = -1 store then disappears exactly as in retail (an
  address-taken local's store cannot pass a pointer load, so the load must come first). differ/00ba6240.cpp is that version
  (DIFFER 2937v2937, 81 masked / 211 unmasked lines: the zero register is esi in ours (immediate byte store) v edx in retail, and the
  depth-pack texture pointer is `lea ecx,[edx+32Ch] ; test` v retail `add ecx,32Ch`); differ/00ba6240_alt_v15.cpp is the lower-count
  but structurally wrong version (72 / 207). Not moved by: bool v enum named local, const, mipmap before/after, a named
  CEngineShadowRenderer& for both reads.
- (0x00BD5C90, later) A named `const C3DMeshInfo* mesh_info = primitive.MeshInfo;` for the radius and centre reads removed the
  &Center CSE: differ/00bd5c90.cpp now DIFFER(2933v2935), 79 masked / 144 unmasked lines (radius, centre transform, frame all equal).
  Left: the inline SetRenderState bookmark code of the stage-0 SetTextureFilterModeMag/Min + UV calls (retail `test eax,esi ;
  or esi,eax ; mov [edx+8],esi`, ours esi/eax swapped -- the RenderClouds-style record-id tie; fitted CRENDERSTATEMANAGER_
  SETRENDERSTATE_DESIRED (+_BOOKMARK) variants measured worse: 91 / 83), the scene-data copy push order, one plane-loop load order.
  (CRENDERSTATEMANAGER_EARLY_RETURN does not compile on its own here -- cl spun to the timeout; the sweep was stopped. ADDSTATE_DIRTY
  not measured.)

## 0x00BA6240 -- MATCHED (wfA6c, 2026-11-15): src/00ba6240.cpp + test/00ba6240.cpp, RELOCATION_MATCH, vt PASS, read-only vland RELOCATION_MATCH PASS
- What closed it (in order, each graded): (1) the used_blank_textures bool in its own { } round the SetupStaticBlocks call (frame packer:
  ebp-2Dh as retail; 81 -> 45 masked lines). (2) EVIDENCED level change: the bodies of the callees that receive &texture_stages defined
  above the row (SetupFirstTextures 0x00B9D670, RenderMeshPrimitiveBlocksIndexBuffer 0x00B9E5B0, SetupStaticBlocksBlankTextures 0x00BA4A40,
  SetupStaticBlocks 0x00BA50A0, + GetDiffuseTexture/GetBumpTexture/CGraphicInfoFrame::GetTexture so SetupStaticBlocks itself matches):
  text copied from the wfB3 TU file rebuild/candidates/manual/pending/2026-11-14_acctC_wfB3open/tu_00ba2350_00ba5260.cpp; each grades
  RELOCATION_MATCH at its own address in this file. Probes showed all three direct callees must be visible (any one missing = no change).
  This fixed the technique-block register choice (add ecx,32Ch) -> 28 lines. (3) `unsigned long mipmap = 0;` before the stages (FITTED):
  2938 -> 2937, shared zero dl. (4) UseBlankTextures = false before the mipmap call (FITTED, store 0x00BA696F). (5) constant as
  `const float min_w_clamp = ...; SetUserVertexShaderConstant(0, C4DVector(128.0f, min_w_clamp, 0.0f, 0.0f));` (FITTED: load first,
  integer copy, X..W stored after the pushes; a named C4DVector or a stand-in visible SetUserVertexShaderConstant body are the only
  other ways; the 4 overloads are .cpp functions in Ego_r's CShaderRenderManager run 0x00AE62B0.., so no visible body in retail).
- Globals respelled to the PDB names (WellingtonGame PDB globals list): NEngineInternal::GPShadowRenderer 0x01436E60, GPCamera 0x01436EA0,
  GPPrimitiveRendererSharedData 0x01436E78, GPPrimitiveRenderer 0x01436E80 (was g_pEngineShadowRenderer / g_pEngineCamera / g_p...).
- Header: copied wfB3's engine/N3DPrimitive_CStaticBlock.h (agentC17 opt-in FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS_BLOCKDRAW, asserts
  pinned with FABLE_STATIC_ASSERT_AT) into hdr/. vland warnings only: RELOC_NAME (labels), RELOC_CONFLICT D3DAlphaFuncModeTable 01396DD8
  here v 01396DE8 in landed 00BBC460 (folded constant index into the table; not an identity error).
- Retail-asm line added (GFFloatToLongFloor at 0x00BA6A6F, shared helper): purity grade retail_asm (no asm in the row).
- Test: host model (DrawSilhouette-style) of early-outs, per-primitive skips, technique 2 stage/constant, transparent/opaque arms.

## 0x00BD5C90 -- wfA6c progress (in work)
- Constant block: the 0x00BA6240 spelling (`const float min_w_clamp = ...; SetUserVertexShaderConstant(0, C4DVector(128.0f, min_w_clamp, 0.0f, 0.0f));`)
  fixes the constant hunk but re-keys the radius GetMagnitude x87 order (B1, m=91).
- k-lever: plain unused `int x;` locals are inert (front end drops them); `int unusedN = 0;` re-keys c2 records and only the COUNT
  matters (position / type inert). B1 + k=6: length 2935 = retail, m=60 (left: scene-data push interleave, plane-loop x87 order,
  4 SetRenderState test/or swaps, 0x2814/+8 load order). B1 + TRANSFORMPOINT_SUMS + k=9: 2935, m=46 (left: TransformPoint row-1
  product order, 2 test/or swaps, load order).
- Render-state record devices (shared FITTED opt-ins CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED / _BOOKMARK / ADDSTATE_DIRTY) x k x
  TransformPoint opt-in, ~200 grades (tools/genbd.py makes the variants): best B1 + TRANSFORMPOINT_SUMS + DBX + k=9..12 = 2935v2935,
  m=42 (left: TransformPoint X-row product order (retail (E11*x + E31*z) + E21*y), and the test/or swap of the first two stage-0
  filter setters 0x00BD6239.. (the last two now equal)). GROUPED_XZY is worse with DBX (m=72).
- LTR + DBX + k=9..11: 2935v2935, m=40 (TransformPoint X row now equal; left: the scene-data copy push interleave 0x00BD5DD0.. and the
  2 stage-0 filter test/or swaps). k position is irrelevant (function top, technique block, before the loop: same results by count).
- GetDiffuseTexture/GetBumpTexture bodies visible above the row (they are probably header-inline COMDATs: 0x00BA9360 is called from
  every render TU and sits after the static-mesh TU): no byte change here (probe tmp/bd/D1.cpp).
- k windows (LTR + DBX): k=9..11 fixes the mipmap x87 + TransformPoint but leaves 2 test/or + push; k=17/19/20 fixes all four test/or
  but breaks the mipmap x87 (2933) -- residues right in DIFFERENT k windows = missing / extra IL spread over the expanded helpers
  (wfB6 lesson), not a single tie. PARTICLE_POINT = LTR. Sweeping the default TransformPoint (the text 0x00BA6240 and 0x00BA5260
  match with) next.

## 0x00AE3C30 -- wfA6c
- k-lever (`int unusedN = 0;` k=1..6): byte-identical (2278v2310): the this-v-zero ebx choice is allocator priority, not a record tie.
- Divergence starts 0x00AE40BE (IndexedTextureCoords read after the out-of-line resize(n) 0x00AE7B10): retail `mov ebx,[esi]` (begin
  in a callee-saved temp) then `xor ebx,ebx` (zero candidate for the rest); everything before is identical except frame offsets.
- Default TransformPoint (the text 0x00BA6240 / 0x00BA5260 match with) x {none, D, X, DBX} x k=0..20: best m=60 (worse than LTR):
  so the remaining residues are not only record ties; some IL of this row (or of a same-TU callee body that retail had visible: candidates
  RenderPrimitiveBlocksIndexBuffer 0x00BCFB00, SetupMesh 0x00BCF2E0, GetBoneTransformations 0x00BD2D90 -- landed with local models,
  not ported here) differs. Best kept: differ/00bd5c90.cpp = LTR + DBX + k=9 (2935v2935, m=40). Test test/00bd5c90.cpp written (host
  model, not run against a match). Parked for this run (~330 grades).

## 0x00BD5C90 -- wfA6d (successor, 2026-11-15)
- Measured: residue (a) (scene-data push schedule) is NOT moved by k (0..30) with LTR/DBX; it is fixed by an explicit X-row tree
  ((E11x+E31z)+E21y)+E41 (probe opt-in TRANSFORMPOINT_PROBE1 in hdr/engine/CMatrix3x4.h, probe only): with PROBE1 + DBX + k=17/20 the
  centre transform, the push and all four stage-0 test/or are equal; only the mipmap TransformPoint X row is left (2933v2935, m=37).
- Inference (EVIDENCED by trees): retail's centre rows are ((E1kx+E3kz)+E2ky) trees; the mipmap rows (this row and the matched static
  row 0x00BA6240, both contexts there) are default-text trees E1kx+(E2ky+E3kz). No single 1-arg TransformPoint text can give both, so the
  centre is most likely NOT the 1-arg TransformPoint: 2-arg TransformPoint(source, dest) (FableWin has it), with default 1-arg for the
  mipmap. Tried 2-arg texts SOURCE_DEST_SUMS (=SDPROBE3, operand order inert), SDPROBE1 (default 2-arg text, forceinline), SDPROBE4
  (grouped), x {N, D, DBX} x k: best m=52 (push still wrong). `.Center` source instead of GetCenter(): worse (m=70..90).
- Named-local / expression shifts between the residues (distance inline, render_info unnamed, scene_data named, mesh_radius inline,
  type named, min_w_clamp inline, mesh_info dropped) x k=0..20: none better than 40.
- Same-TU evidence (new): IsInsideFrustum@CEngineInternalPrimitiveAnimatedMesh 0x00BCF040 (same TU, same CalcInterpolatedObjectTransform
  + MeshInfo+0x28 centre) transforms its centre with DEFAULT-text trees ((E3kz+E2ky)+E1kx). This row's centre trees are
  ((E1kx+E3kz)+E2ky) with `fld Ejk ; fmul st(i)` products. So the centre here is spelled differently from IsInsideFrustum's
  (not the 1-arg default TransformPoint): open question which helper/text. Tried as candidates (all with the default 1-arg for the
  mipmap): 2-arg SOURCE_DEST_SUMS, default 2-arg text forceinline (SDPROBE1), all-rows-accumulated x,z,y (SDPROBE2, the 2-arg
  default's own Y/Z style), grouped (SDPROBE4), COPY_SUMS, each x {N,D,DB,DBX,X,DX} x k in {0,9,10,13,17,20}: best m=52..55; the
  2-arg forms emit `fld st(i) ; fmul Ejk` products (retail `fld Ejk ; fmul st(i)`) and leave the push wrong. Probe opt-ins were
  removed again (hdr/engine/CMatrix3x4.h restored byte-identical; 0x00BA6240 regraded RELOCATION_MATCH).
- ~290 grades this run. Parked again; best stays differ/00bd5c90.cpp (2935v2935, m=40).

## 0x00AE3C30 -- wfA6d
- New facts: retail's `this` is a pure memory variable (reloaded from [esp+10h] everywhere) and the never-written C3DTriangle2
  resize(n) fill temp is packed ON this's home ([esp+10h], `lea ecx,[esp+10h]` at 0x00AE3D5D); ours packs the temp at +14h over the
  pair temp and makes `this` a register candidate (ebx from 0x00AE416F). Frame 0x10 v 0xC follows. So the open question is why c2
  treats `this` as memory-only in retail.
- Tried (byte-identical): k = 10/20/40 unused locals; explicit `resize(n, C3DTriangle2())`; named `C3DTriangle2 blank` (at top, in a
  block, before the call). /Op identical; /Oa, /Ow much worse (no sibling carries them). Parked (~10 grades).

## 0x00C79FD0 -- wfA6d
- Tried (byte-identical): k=1..7 unused `__m64 unusedN = _mm_setzero_si64();` (MMX-class records); ivec.h's
  `operator=(const M64&)` members on Is32vec2 / I32vec2 / Is16vec4 / I16vec4. The s3 colour (mm1 retail v mm2 ours) is the register
  the final paddd result takes (retail: into the partial-sum reg then copied to p1's dead reg). Parked (~10 grades).

## 0x00BD5C90 -- wfA6e (2026-11-15, successor): m=40 -> 28 (22 of the 28 masked lines are relocation noise; real residue = 1 push schedule)
- NEW lever: two dead release asserts (hdr/fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM; agentC6 measured the same residue class on
  0x00BB3E80) fix both stage-0 filter-setter test/or swaps at k=9..13 (a=1..8 all similar; count-only); they flip the hidden-materials
  SIB (`cmp byte [eax+ecx]` retail v ours [ecx+eax]); a named `const unsigned long material_index = mesh_primitive.GetMaterialIndex();`
  for the two uses fixes that. differ/00bd5c90.cpp = LTR + DBX + k=9 + 2 asserts + material_index: DIFFER(2935v2935), m=28.
- Left: scene-data copy push (0x00BD5DBB..) scheduled two x87 instructions early in retail. Measured with asserts: all 108 X/Y/Z row-tree
  combos (flat F, default D, ((x,z),y) T, ((x,y),z) P) at k=10/12: X=F push late, X=T push one early (and mipmap X breaks), X=D push
  exact but centre X order wrong; Y/Z trees inert for the push. 2-arg centre TransformPoint (accumulated / copies / flat) x {SUMS, LTR}:
  m>=44. Named sphere copy: worse. Assert expressions with float / class temporaries: none below 30.

## 0x00BD5C90 -- MATCHED (wfA6e, 2026-11-15): src/00bd5c90.cpp + test/00bd5c90.cpp, RELOCATION_MATCH, vt PASS, read-only vland RELOCATION_MATCH PASS, addmatch done
- What closed it (each graded, in order): (1) two dead FABLE_RETAIL_ASSERTs (hdr/fable_assert.h, dead arm) -> both stage-0 filter-setter
  bookmark test/or equal (m 40 -> 30); (2) a named `material_index` for GetMaterial + hidden_materials (the asserts had flipped the SIB at
  0x00BD6379) -> 28; (3) the radius spelled `((Radius + |Center|) * Scale) + BoundingRadiusFix` (natural grouping paren; same tree; its
  no-code PAREN tuple moves the scene-data copy push 0x00BD5DBB into the Z-row sum as in retail -- MATCHING_PLAYBOOK scheduler rule)
  -> RELOCATION_MATCH. `+ (BoundingRadiusFix)` paren works too; parens elsewhere in the radius expression do not.
- Minimal set measured: k=9 exactly (k=0..8 differ), both asserts (one assert: test/or back), material_index needed, DBX opt-ins still
  needed (none / D / DB / X / DX / BX / B all differ). TRANSFORMPOINT_LTR = FableWin ?TransformPoint text (61i/168b SAME, fwsize).
- Retail-asm evidence line for the shared GFFloatToLongFloor (0x00BD65D1..) added so purity = retail_asm (vland said NOT_GENUINE without it).
- Header: hdr/fable_assert.h added (copy of agentC6's shared header via wfE3; new file, additive; 0x00BA6240 does not include it, regraded
  RELOCATION_MATCH in the same vland run).

## 0x00C79FD0 -- wfA6e (2026-11-15)
- c2prio (tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py, --flags /GS; trace in tmp/c79/c2p/base.txt): case 0 s1 = web #37
  prio 168 key 198 (split, memory), s3 = web #40 prio 168 key 118 -> reg60 (mm2). Retail = s3 coloured BEFORE s1 (s3 takes reg59 = mm1,
  the s1 reload piece mm2). Equal prio, key = number of the last tuple writing the web (tuples numbered per block from the last tuple
  back, so the earlier def wins): with one straight-line loop body and s1 defined before s3, ours can only colour s1 first. Retail
  therefore had either a higher prio for s3 (more references / another block) or a block boundary between the two defs.
- Measured inert (byte-identical, m=147): k = 1..20 `int unusedN = 0;`; 1..8 dead FABLE_RETAIL_ASSERTs (pointer and int conditions);
  renaming s3 (7 names); a separate copy `Is32vec2 t = s3;` for b; reusing s1 / s3 / s0+s2 as the a / b results (webs are rebuilt).
- Parked (~40 grades). Open: find a no-code IL block boundary or an extra s3 reference that the original source had.

## 0x00AE3C30 -- wfA6f (2026-11-15, successor; ~25 grades + 3 c2 traces)
- Read c2's own allocation (tools/ae3c30_c2prio.py = C41 c2prio_f.py; traces tmp/ae3/prio1*.txt): first-run priorities `this` -298
  (split: esi #8, edi #7, ebx #46) v `const 0` -322 (refs 90, "split, immediate"). In the third split pass every const-0 piece is
  SKIPPED with spill -1 ("not worth a register"), then this-piece #46 (prio -77, allowed only ebx) takes ebx = ours from
  0x00AE416F. Retail's const-0 piece over 0x00AE40CB..0x00AE441E (loop guard) must have spill > 0 (it gets ebx; the loop counter
  takes ebx after). So the open fact is the COST of the const-0 piece (sum w*cost over its blocks), not a key tie: k locals cannot move it.
- Frame (EVIDENCED): C3DTriangle2 is 6 bytes; retail frame = [+0 this home shared with the 8-byte never-written fill temp]
  [+8 pair<CCharString,ulong> temp]; ours = [+0 this spill] [+4 fill temp over the pair temp]. A consequence of `this` being a
  spilled candidate in ours (pieces), packer runs after allocation.
- Tried, all inert (2278v2310, 127..131 masked lines): FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE (+ _VALUE_TYPE) with
  `ExportParticles.Insert(name, index)` (byte-identical; the pair dies before Dirty either way); same-TU ReadGroups 0x00AE3A60 body
  visible above the row (probe-only opt-in FABLE_RETAIL_C3DGROUP2_READGROUPS: GetVertexBlends / Resize decls, kept in
  probe/ae3h/engine/C3DGroup2.h, NOT in hdr/ since inert; source tmp/ae3/v3.cpp); probe overlay probe/ae3h also has a Read body
  through IsValidPosition(__int64) (the landed ReadGroups reader's spelling): inert; resize(n) via
  FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_OUTOFLINE (noinline body) + explicit two-arg resize(n, T()) for Tris / Vertices / long;
  named count locals per resize (all / first three / last three: 127 best, no allocation change); named float for AveragePatchSize;
  named NonSimCount local; signed loop counter / ReadSLONG count; an empty inline `T* const&` toucher of `this` (inert; a throwing
  one changes code). None produces `xor ebx,ebx`.
- Next (untried): find what makes a const-0 block cost +1 v -1 in c2 (0x10782473 pass: cost of a constant reference) -- e.g. trace
  one tiny probe TU with `mov [mem],0` / 64-bit compare / test shapes; then look for the IL shape in the second half that retail had
  (candidate: the 64-bit range check spelled through IsValidPosition(__int64) as the landed ReadGroups local reader does, which
  changes the compare tuples against 0 without changing bytes).

## 0x00ADE080 -- wfA6f (2026-11-15, successor; ~95 grades)
- Real residue of differ/00ade080.cpp with relocation noise ALSO masked (tools/ade080_md.sh: `[0]` v `[ADDR]`, call/jump targets):
  17 lines, not 43 (26 of the 43 were relocation-mask noise). Left: (1) v0 X row association, retail (x*E11 + y*E21) + z*E31 at
  0x00ADE485..0x00ADE497, ours (x*E11 + z*E31) + y*E21 from the FITTED GROUPED_XZY body (v1..v3 need that grouping); (2) centre
  TransformPoint Y row inner pair, retail z*E32 computed first (`fld st(2) ; fmul [eax+1Ch] ; fxch st(4) ; fmul [eax+10h] ;
  faddp st(4)`, 0x00ADE624..), ours y*E22 first (one fxch fewer = the 2 bytes); (3) `lea ecx,[esi+0Ch] ; mov ebx,ecx` scheduled
  13 instructions earlier in ours (0x00ADE60C / 0x00ADE612 in retail).
- FableWin COMDAT order after InitialiseFromOBB (0x03313830): ??4C4DVector@@QAEABV0@ABVC3DVector@@@Z, then
  AlignedTransformTransposedAssumeW / TransformTransposedAssumeW: the original SSE arm uses C4DVector::operator=(const C3DVector&).
  Measured: `C4DVector centre; centre = obb.GetCenter();` (FABLE_RETAIL_C4DVECTOR_ASSIGN3D) is byte-identical to the ctor form
  (kept the ctor); corners through operator= is worse (2743, 272 lines).
- Sweeps (all >= 17, none better): k unused locals 1..19 (int / float) at function top, x87-arm start, before centre (period ~6;
  k=13..18 give equal length 2735 but 40..44 lines: other x87 orders rotate); dead release asserts (FABLE_RETAIL_ASSERT_DEAD_ARM)
  n=1..6 x k=0..7: an assert shifts numbering like k (the fields it names do not matter: E31,E21,E11 v E11,E21,E31 identical), best
  24; flat TransformVector / TransformPoint with k=0..11: 51..66 lines. Shared-symbol spellings (one `half` for both arms, half
  transformed in place, `half` + v0 copy): 190..295 lines.
- Conclusion: the remaining orders are relative c2 record/key orders (not a uniform shift), so devices (c)/(d) cannot close it.
  Parked. Best stays differ/00ade080.cpp (unchanged).

## 0x00C79FD0 -- wfA6g (2026-11-15, successor; ~41 grades + 3 c2prio traces; parked again, best unchanged differ/00c79fd0.cpp)
- Decision re-measured (c2prio --trace --blocks, tmp/c79n/base_trace.txt): in the third-pass re-sort of case 0 the s1 reload piece
  #167 (prio 168, key 198 copied from s1 #37) sorts before s3 #40 (prio 168, key 118) and takes reg59 = mm1, so s3 gets reg60 = mm2.
  Retail = s3 first. Case 2 (K 15): s1 pieces 184 v s3 180, so there it is the PRIORITY, not the key, that decides.
- Priority arithmetic from the trace (MMX class, loop block, w 2): K = 7 non-window candidates (s0..s3, a, b, pan) + q window-quad
  CSE temps (q = 7 in case 0, 8 in cases 1-3). s3 keeps its first-pass prio 2*K*6 = 12K; the s1 piece is recomputed with
  K' = K + q (the quad pieces): 2*K'*4 = 8(K+q). s3 colours first only if 12K > 8(K+q), i.e. (non-quad candidates) c > q:
  retail's IL would need about two more non-split MMX candidates in each loop block (or s3 with a 4th reference). Retail's code
  shows no other multi-use MMX value, so the source fact is still unknown.
- Mono sibling evidence: retail ResampleMonoMMX 0x00C78C4C uses exactly our stereo pattern (`movq mm2,mm0; paddd mm2,mm1`, s3 = mm2,
  s1 reload mm1): with the same sums, only the stereo outputs (a/b used twice, pan) make retail colour s3 first.
- Tried this run, all byte-identical (2884v2884, 49 hunks) unless noted: the REAL VS2003 ivec.h (sdk/vs2003/include/ivec.h, full class
  library instead of the local model; tmp/c79n/iv1.cpp); copy of s1 (and s0) placed after s3 (`Is32vec2 s1 = t1;`); `s3 +=` last term,
  full `+=` chain for s3, `s1 +=` last term; named single-use intermediates (fold sum w/x, pan product pa, Is16vec4 temp) -- K stays
  14 (traced); `a >>= 1` / `b >>= 1` in place before the pan output; `(void)&s1` / pointer / reference to s1; s3 or s1 or all four
  declared at function scope (s1/all: frame-slot changes only, 122/163 hunks); label / goto before the a-fold (no RA block boundary,
  K unchanged, scheduling hunk added); sums in an array `Is32vec2 s[4]` (scalar-replaced, slots differ, 2852); sum-order
  permutations (emitted order follows source: 2892); named window quads (3252); inline fold helper by const ref (2940; by value
  does not compile: C2719 aligned parameter).
- Open: a natural source construct that adds MMX candidates (or a 4th s3 reference) in each loop block without code. Not a k-locals /
  assert / inline-flag residue (those re-key symbols, not tuple numbers or K).

## 0x00ADE080 -- wfA6g (2026-11-15, third agent; ~25 grades + 3 linearizer traces)
- Linearizer trace (tools/00ade080_lk: wfB6 lk.py comparator log of 0x1076A394 + an.py naming leaves; traces tmp/00ade080_lk/*.json):
  the x87-arm products are (IND E, local) with keys hash = hE + 64*id + 48, hE = 0x6E + 2*offset (E11 is a count-1 IND, its product
  count 3, always sorted apart); leaves are the TransformVector locals xx/yy/zz (ids 498..509, one batch for all four expansions, in
  declaration order) in EVERY row, also with field spellings (v.Y / v.Z in the rows: inert, c2 canonicalises them to the locals).
- Model result: with one leaf per component, no id assignment (wraps included) gives retail's v1..v3 Y and Z orders together (the key
  difference of the (y,z) pair is 0x18 + 64(z-y) in both rows), AND the emitted order is not the key order: declaring zz,yy,xx
  (ids z<y<x) flips the Y-pair keys but ours still emits y*E22 first (it only changed which values stay on the x87 stack: 31 lines).
  So the remaining x87 orders are decided by the x87 stack code generator (stack state), not by the linearizer keys; record-id
  devices (k locals, asserts) and leaf spellings cannot reach them. Declaration orders XZY / ZXY: identical (17).
- Also tried (worse or inert): 2-arg TransformVector(half, v0) (frame +247 lines), in-place TransformVector(half, half) (41/21),
  2-arg TransformPoint(obb.GetCenter(), centre) with SOURCE_DEST_SUMS (37) / COPY_SUMS (40); fwsize on the best source: the
  C3DVector operator+/-/* (+6i), GetCenter (+2i), LimitLower/Upper (+3i), InitialiseTransposed (+4i/-8b = fldz/fld1 constant
  stores) differ from FableWin, all float-code helpers (FableWin's compiler differs on float code), TransformVector / TransformPoint
  / C4DVector ctors SAME. SCALE_NAMED_RESULT operator* 29i v FableWin 31i (not closer to proof).
- PROGRESS (wfA6g): helper-local DECLARATION ORDER is the lever for the x87-stack orders (rulers / k / asserts are not): TransformPoint
  with locals declared z, x, y and the FableWin-sized flat LTR text (new opt-in FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZXY_LOCALS in
  hdr/engine/CMatrix3x4.h, FITTED order) -> DIFFER(2735v2735), 14 real lines (was 2733v2735, 17): centre loads and centre X / Y rows
  now equal retail. differ/00ade080.cpp updated (previous best kept as tmp/00ade080_differ_prev_wfA6f.cpp). 0x00BA6240 / 0x00BD5C90
  regraded RELOCATION_MATCH after the header change.
- Left (14): v0 X row (4 lines), lea/mov ebx schedule (5), centre Z row (5: retail y,z,x; ours z,x,y = a rotation).
  Sweeps on the new state, all >= 14: 6 TransformPoint declaration orders x 15 bodies (probe/00ade080_h, TPORD_* x TPB_*: ZXY or ZYX
  with several bodies tie at 14); TransformVector declaration orders x {grouped, flat} (grouped XZY/ZXY 14, others 28; flat 40..58);
  live-initialised rulers `float rulerN = obb.MinCoord.X;` k=1..13 at x87 start / before centre (14 at k=0,6,7,8; never fixes Z or v0);
  C3DVector SUBTRACT/ADD_XY_GROUP, SCALE_X_PAREN, SCALE_NAMED_RESULT (58..282, worse: they touch the SSE arm); MinCoord/MaxCoord init
  spellings (`MinCoord = MaxCoord = centre + v0` identical; others worse); redundant parens in the TV body (inert).
  NOTE: the TVB_* field-spelling probes of the first half of this run were void (a `#el#if` typo) and were re-run after the fix: inert.
- More on the 14-line state (wfA6g): a TransformVector X row spelled flat / (x+z)+y / (x+y)+z with grouped Y/Z rows: all 14 (the X row
  form is irrelevant once Y/Z are grouped); x 6 declaration orders: >= 14; outer spellings (v1..v3 reversed, copy-and-negate,
  `half` then v0..v3 ctors, v0 declared last, one-line v1,v2,v3): 14..251. The v0 X row stays (x+z)+y in every variant: in ours all four
  TransformVector expansions get their locals in one record batch; retail's v0 differs from v1..v3, so its v0 expansion is probably
  different text or a different expansion time (open). Probe overlay probe/00ade080_h (TPORD_* / TPB_* / TVORD_* / TVB_* / TVB_MACRO
  switches on a copy of CMatrix3x4.h WITHOUT the ZXY_LOCALS opt-in: emulate it with GROUPED_XZY + TPORD_ZXY + TPB_1) is kept for reuse.

## 0x00AE3C30 -- wfA6g (third agent; 7 grades, no progress)
- c2prio first-run trace re-read: const 0 (#1, allowed only ebx) loses in pass 1 because pool (#9, -175, ebx over lines 47..56) holds
  ebx; in pass 2 the big const-0 piece #138 (-270) again finds ebx taken by pool, so it must overlap pool's range in ours; in retail
  the const-0 piece that gets ebx starts after AllocateArrays (0x00AE40CB). The cost-1 blocks of const 0 are 134 (lines 61-62) and 157
  (63-64), the two long() fill-temp stores, plus 219; a piece needs both (spill = sum - 1 > 0). Open fact: the piece boundaries.
- Lexical scopes around the second-half statements (each pair / all reads / loop + count / from IndexedTextureCoords on): inert
  (2278v2310, 131 real lines, no `xor ebx,ebx`).
- Decomposition of the 14 (wfA6g): TransformPoint local declaration order decides three things at once: load order (xx declared
  before yy -> retail's `fld [28h] ; fld st(2)`), Y-row pair (z first in ZXY / ZYX, y first in XYZ / XZY) and Z-row order (right in
  XYZ / XZY / ZYX, wrong in ZXY). No order gives all three: XZY = loads + Z right, Y wrong (17); ZXY = loads + Y right, Z wrong (14);
  ZYX = Y + Z right, loads wrong (14). const / non-const mixes of the three locals (8 variants x 2 bodies): inert (14). Next idea for a
  successor: a body whose rows read the LOCALS in a different order per row is inert (c2 canonicalises); look instead at what retail
  had ON the x87 stack before the transform (GetCenter's text decides which of x / y / z stay in registers: retail stores x to
  [esp+28h] and keeps y, z) -- e.g. GetCenter / operator+ / operator* texts that leave a different value in memory.

## 0x00ADE080 -- wfA6h (2026-11-15, fourth agent)
- MEASURED (wfB1 c2 list-scheduler trace, tools/00ade080_sc/sc.sh + rc.sh, read-only use of $S/w22/wfB1/tools/sched): the x87 arm is
  one block cut into 81-tuple scheduling regions (83 nodes with the 2 dummies). The `lea ecx,[esi+0Ch] ; mov ebx,ecx` residue is the
  REGION BOUNDARY: lea has no predecessor, so it issues in the first cycle of its region. Ours: region 5 starts at GetCenter's
  `fadd [ecx+8]` (0x00ADE5DB); retail's starts 12 tuples later (after `fld st(3)` of the TP X row, 0x00ADE60A). The 12 tuples are
  no-code op-0x269 "grouping barrier" tuples: one per explicitly grouped sum (parens OR a split temp `t = a+b; v = t+c;`), i.e. one
  per row of the FITTED TRANSFORMVECTOR_GROUPED_XZY body x 4 expansions. So retail's TransformVector is FLAT (no grouping), as the
  landed default text: with GROUPED_XZY removed the lea hunk vanishes (verified), 40 real lines left, all x87 term orders.
- So the 14-line state was a local optimum on a false text: every earlier "outer spelling" sweep at the grouped state measured
  nothing (the grouping pinned the orders). With the flat body the orders DO move with outer spellings (v1..v3 construction,
  `v0 *= 0.5f`, member-wise stores): sweep on the flat base, probe/00ade080_p (cg.py / cs.sh: v0 x v1..v3 x centre x TP combos).
- TV declaration order (6) is inert on the flat body's term orders; k unused int/float locals 1..11 at 6 positions on the 14-line
  state: period 8, position-independent, never better than 14 (device c does not apply); k 1..16 on the flat base: 44..57.
- New overlay opt-in FABLE_RETAIL_C3DBOUNDINGBOX_GETDIAGONAL (FableWin GetDiagonal 22i/58b, GetDimentions 15i/35b): inert at the
  14-line state.
- FLAT-TV family (all with the landed default flat TransformVector, i.e. GROUPED_XZY dropped; lea / mov ebx EQUAL in all):
  240-combo sweep (probe/00ade080_p/sweep1.txt: v0 {(M-m)*0.5f, `v0 = M-m; v0 *= 0.5f`, GetDiagonal, /2.0f, GetDimentions*0.5f,
  GetDimentions + *=} x v1..v3 {ctor, copy+negate, member-wise, copy+assign, three copies} x centre {4 spellings} x TP {ZXY, LTR}):
  best = `v0 *= 0.5f` + member-wise v1..v3 stores + GetCenter + TP ZXY_LOCALS: DIFFER(2735v2735), 20 real lines, ALL x87 term order:
  v0 X row (retail x,y,z ; ours y,z,x), v1..v3 X rows (retail x,z,y ; ours y,z,x), v1..v3 Y rows (retail z,y,x ; ours y,z,x); every
  Z row and the whole centre TransformPoint equal. Saved as differ/00ade080_flatTV.cpp (grades with today's hdr/, no probe macros).
  differ/00ade080.cpp (14 lines) is kept, but it rests on the FITTED grouped TV body that the region boundary disproves; a successor
  should continue from differ/00ade080_flatTV.cpp.
- Measured on the flat-TV state: k unused locals 1..32 have PERIOD 32 (k=32 == k=0) and only move the Z-row z/x wrap boundary across
  the vectors (v3 at k=8, v2 at 12, v0 at 16); the X / Y rows' y-before-z NEVER changes for any k: the product keys step by 2048 per id
  (mod 65536), so the y/z pair's residue mod 2048 is fixed by the leaf kind (TV local copy xx/yy v the vN.Z field record that c2
  propagates for zz) -- rulers / asserts cannot reach it. The construction spelling of v1..v3 does (ctor: z last everywhere; member-
  wise: y,z,x everywhere; v1 v v2/v3 differ under ctor/Initialise), so the open fact is the leaf IDENTITY of z (and x) in retail.
- Inert on the flat-TV best: TV declaration order (6) and non-const / one-line / src-copy locals; member-wise store order (6 perms x
  vector-/component-major); separate v1..v3 declarations; v1..v3 declared before v0; SSE-arm `half` spellings (GetDiagonal,
  GetDimentions, `*=`). Worse: arrays `C3DVector v[4]` with or without a loop (c2 CSEs products across vectors: 2651..2854 bytes);
  2-arg TransformVector(const C3DVector&, C3DVector&) with temporaries (CSE, 2672..2686) or (v, v) (same as member-wise).
- Grades this run: ~200 (sweeps included).

## 0x00C79FD0 -- wfA6i (2026-11-15, fifth agent; ~30 grades + 3 c2prio traces; parked again, best unchanged differ/00c79fd0.cpp)
- Re-traced (tools/00c79fd0_tr.sh; tmp/c79z/*.tr): loop blocks K = 14 / 14 / 15 / 14 (cases 0..3; case 1 and 3 have only 7 shared
  window quads, case 2 has 8: in[0] shared by s0/s1 AND in[7] by s2/s3). Run-2 re-sort: cases 0, 1, 3 are an exact PRIORITY TIE
  (s3 168 = s1 reload piece 168; the piece's key, copied from s1, is larger because s1 is defined earlier, so the piece takes mm1);
  case 2 is priority-decided (piece 184 v s3 180). Piece K' = (s3, a, b) + 2q window (parent + piece) + 3 sum pieces + 1 pan piece.
  So retail needs either two more non-split MMX candidates per loop block (c >= 9), or a 4th s3 reference (cost 8), or a block
  boundary between the s1 and s3 definitions (keys count up block by block) PLUS a K change for case 2. No k-locals / assert /
  inline-flag device reaches that (they re-key symbols, not tuple keys or K).
- Tried, all byte-identical (2884v2884, m2=147) unless noted: CSE'd fold with no named a/b (fold written twice per output);
  `a >>= 14` after the plain fold; `a >>= 1` in place before the pan output (traced: K unchanged, the >>1 web is not a candidate);
  a named per-iteration pan copy `Is16vec4 p = pan` (traced: copy-propagated, K unchanged); pan as a short[4] array read through
  `*(const Is16vec4*)`, as a pointer, as a reference; free helpers non-inline (our obj also emits the out-of-line ivec copies, like
  retail's 0x00C78590..0x00C788D0); `I32vec2` a/b; a named `h = a >> 1` reused for b; s3 (or every sum) started from its last
  product (`s3 = p7; s3 = <6 terms> + s3`); every sum declared then assigned; `first[0]/[1]` indexed stores (2892/2916 worse);
  named output loads `x0 = *first; *first++ = x0 + a` (3156, worse); b computed before a's stores (2892); a Filter7(in+k, c+n)
  inline helper (1248 out of line; forced inline 3060).

## 0x00AE3C30 -- wfA6i (2026-11-15, fifth agent; ~20 grades + 1 c2prio trace; parked, differ/00ae3c30.cpp UPDATED, m2 135 -> 133)
- NEW EVIDENCED fix: the exported-particle insert is `ExportParticles.Insert(name, index)` (FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE +
  FABLE_RETAIL_CVECTORMAP_SORT, both existing overlay opt-ins; replaces FABLE_RETAIL_CVECTORMAP_INSERT + Insert(pair)). Retail destroys
  the pair temporary BEFORE setting Dirty (0x00AE44D4 `lea ecx,[esp+18h] ; call ~CCharString` then 0x00AE44E1 `mov byte [edi+0Dh],1`),
  so the pair is a temporary inside Insert(key, value); with Insert(pair) ours sets Dirty first. FableWin.pdb has only
  ?Insert@?$CVectorMap@VCCharString@@K..@@QAEXABVCCharString@@ABK@Z (0x03066C10) and fwsize gives Update = `if (Dirty) Sort();`
  (13i SAME with _SORT). make_pair v value_type (INSERT_VALUE_TYPE): identical bytes; kept the default (make_pair).
- Still open: (1) this-v-const-0 in ebx (unchanged; c2 trace tmp/ae3z/base.tr: run 1 the no_export temp #57 takes ebx at lines 67-68,
  const 0 #1 fails and splits; its piece #138 overlaps pool (#9, ebx in run 2) and splits again into pieces with spill -1, so the
  this-piece #46 takes ebx); (2) loop schedule: retail stores pair.second (`mov [esp+1Ch],eax`, 0x00AE4498) before the push_back
  `cmp esi,[edi+8]`, ours after; (3) sort call: retail `mov ecx,[edi] ; xor edx,edx ; mov dl,[edi+0Ch] ; push edx ; mov edx,[edi+4]`,
  ours (and the out-of-line Sort 0x009B91D0) `mov edx,[edi+4] ; mov ecx,[edi] ; xor eax,eax ; mov al,..`; (4) site 2 (RenderQuads)
  retail reloads `this` before the count (0x00AE3E4D), ours after. (2)-(4) are schedule/allocation orders, not new calls.
- Frame fact: retail frame is [+0 this home = never-written C3DTriangle2 fill temp (6 bytes, rounded to 8)] [+8 pair]; +4 is never
  addressed, i.e. the fill temp occupies +0..+7 and `this`'s home sits inside it (a never-written temp is never live, so it packs at 0).
- Tried, inert (2278v2310, m2 133..137): loop forms (`c = n; c != 0; --c`, `while (n--)`, `c > 0`); Insert(name, in.ReadULONG());
  `CCharString name(...)`; const name / index; parens around the Read sizes and the AllocateArrays argument; sizeof(T) * size().

## 0x00C79FD0 -- wfA6j (2026-11-15, sixth agent) -- BREAKTHROUGH m2 147 -> 23
- The colouring residue closes with the CSE form of the >>14: `Is32vec2 a = unpack_low(s0, s1) + unpack_high(s0, s1);
  *first++ += a >> 14; *second++ += mul_add(Is16vec4((a >> 14) >> 1), pan) >> 14;` (same for b). `a >> 14` becomes a CSE temp
  candidate (+1 MMX candidate per output pair = +2 K per loop block, exactly what the wfA6g/i priority arithmetic required).
  Source: tmp/c79z/c79v1.cpp (tails/c79v1). Only a or only b in this form: m2 53; a named `x = a >> 14`: copy-propagated, 147.
- Left (m2 23): one schedule slot in b's second output (`pmaddwd mm0,mm2` before `mov edi,esi` in retail cases 0/1; `psrad mm0,1`
  before `mov edi,esi` in cases 2/3) + switch-table relocation noise.

## 0x00ADE080 -- wfA6j (2026-11-15, fifth agent on this row; ~110 grades + 2 traces)
- METRIC FIX: tools/ade080_md.sh masks [esp+N], so it hides frame errors. New tools/00ade080_um.sh (frame offsets NOT masked) and
  tools/00ade080_g2.sh (grade + masked + unmasked; HDR made absolute: a relative HDR path makes cl spin to the 300 s timeout and
  the scripts then print the STALE previous diff -- several "inert" results of a relative-HDR run are void). Measured:
  differ/00ade080_flatTV.cpp is 20 masked but 158 unmasked: `v0 = M - m; v0 *= 0.5f` homes v0 at esp+28h (retail esp+34h, and the
  Z difference in a separate temp esp+18h, 0x00ADE424). `C3DVector v0 = (obb.MaxCoord - obb.MinCoord) * 0.5f;` gives retail's frame.
  So the wfA6h flat-TV sweep ranked a frame-broken spelling first.
- NEW BEST (frame right): differ/00ade080_flat2.cpp = v0 `(M - m) * 0.5f`, member-wise v1..v3 stores (X,Y,Z vector-major), new
  overlay opt-in FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_XY_COPIES (flat rows, copies of x/y only, z read from v.Z; FITTED),
  TP ZXY_LOCALS: DIFFER(2735v2735), 20 real / 44 unmasked lines (the 44 are the same hunks counted per line, no frame error).
  Left, all x87 term order: v0 X row (retail x,y,z; ours y,z,x), v1..v3 X rows (retail x,z,y; ours y,z,x), v1..v3 Y rows inner
  pair (retail z,y,x; ours y,z,x). Z rows, centre loads/TP/GetCenter, construction stores, lea/mov ebx and frame equal.
  Ctor v1..v3 + flat 3-copy = 40; ctor + XY_COPIES = 34 (fixes GetCenter add order + centre TP Z row only, a numbering effect:
  zz declared but unused = 30, k=29..32 on 3-copy never reach 20).
- Term-order facts (terms.py, v1 rows): ctor / Initialise (inline param path): X x,y,z ; Y y,x,z ; Z y,x,z (z always last).
  Member-wise (any store order, v- or c-major): every row y,z,x (one uniform leaf order, E offset irrelevant). Retail's orders
  differ per row (Y z,y,x v Z y,z,x), i.e. in retail the y and z product keys are close enough for the E offset to decide.
  Store order Z-first (mZXYv) gives Z rows right and Y y,z,x but breaks the store order (scheduler keeps source store order).
- Inert / worse this run (frame-right bases): k unused locals (uninitialised: no effect at all; `int unusedN = 0;` at top, between
  v3 and the transforms, before centre: identical per k -- locals get ids at one point, position-independent; only Z-row wraps
  move, X/Y never); dead release asserts n=1..4 before v0 / before TransformVector(v0) (+6 bytes, only Z rows move); TV body
  source orders (E-first, retail order: identical bytes); 2-arg TransformVector(v, v) with 3-copy / XY-copy bodies (= 1-arg);
  DATA_UNION matrix layout (inert); v0 via GetDiagonal / GetDimentions*0.5f / Peek accessors (inert); copy+negate / copy+assign /
  v3 from v2 (61..87); assignment from ctor temporaries (207); `half` + ctors (257..269); TV declaration orders (worse or v1 inert).
- Linearizer trace of the ctor+XY_COPIES state (tmp/ade_lk/t1.json, an.py start 2340): x/y leaves are the TV copies (ids 500/501
  for v1), z leaf the v1.Z field record id 1055 (= 31 mod 1024; key 0x9e+0x30+64*id); the product keys alone do not predict the
  emitted order (same conclusion as wfA6g). Anniversary PDB has no InitialiseFromOBB (publics: only C3DBoundingBox ctor and
  CalculateMatrix), so no local names from there.
- wfA6j addendum (METRIC, important): ade080_md.sh also masks every small hex constant (`[eax + K]`), so two rows with the same
  instruction shapes but different E offsets (different x87 term order) count as EQUAL. The honest residue count is
  tools/00ade080_um.sh (only 5-8 digit addresses / [0] / call-jump targets masked). Re-ranked by `um`: old grouped differ
  00ade080.cpp = 28; flatTV.cpp = 158 (frame); flat2.cpp = 44; grouped XZY body WITH x/y copies only (v.Z in the rows; probe:
  probe/ade_s/c_g2.cpp with HDR=probe/ade_s/hdr, TVQ macros) = 22: left only the v0 X row (y/z swap), the v1..v3 Y-row inner
  pair (retail z*E32 first) and the lea/mov ebx region boundary (the 12 grouping tuples). Inner-pair source order inside the
  parens is inert (3 variants), partial groupings (X/Y/Z subsets) 28..56, grouping through an inline helper function is NOT a
  grouping (reassociated freely, no 0x269 tuple, lea right, orders lost: 62).
- Also inert on flat2: v1..v3 declaration order / declared before v0, x87 arm in else-branch or own block, __forceinline TV,
  v0 through the 2-arg overload with v1..v3 through the 1-arg (20..26). TV through a result temporary (`v = C3DVector(...)`):
  2457 bytes (c2 then CSEs the products across the sign-flipped vectors; retail's in-place copies prevent that).
- Open for a successor: the x87 code generator's choice of the inner pair and its operand order for a flat 3-term sum whose
  leaves are a TV copy (x, y) and a field (z). Retail's per-row variation (Y z,y,x v Z y,z,x) needs y/z keys within one id;
  ours have y = copy (id ~500) and z = field (id ~1055). A comparator-RESULT trace (the lk tool logs only arguments) would
  settle the sort direction; that is new tooling, not done.
- wfA6j last measurement: k = 1..8 initialised int locals on the grouped+XY-copies probe (c_g2): the v0 X row and the Y-row inner
  pairs never move (only extra hunks appear: SSE-arm lines 13/15 at k=3..5, centre add at k=5..7); device (c) does not reach them.
  Matched rows 0x00BA6240 / 0x00BD5C90 regraded RELOCATION_MATCH after the XY_COPIES overlay addition. Grades this run ~150.
- Scheduler analysis of the m2-23 residue (tools/sched = hand892 t13/sim, paths fixed; DAG dumps tmp/c79s/v1d.json region 2,
  v1e.json region 3): case-0 loop body = 3 scheduling regions 81+81+6 tuples (168 instructions, no no-code tuples); retail's
  order fits the same cuts (prefix sets equal at 81 and 162; only positions 158/159 swapped). sim.py reproduces ours from the DAG;
  in region 2 `mov edi,esi` (#77) and `pmaddwd` (#80) tie on prio 3 and idx breaks it. Only one IL order of the b tail gives
  retail: [psrad1, pmaddwd, mov(second copy), add esi, psrad14] (pert2.py / pert3.py; the a tail is insensitive). Ours is
  [mov, add, psrad1, pmaddwd, psrad14]. c2's IL order is canonical (z3 with h named before the first output: identical DAG
  order): MMX expression temps are placed right before their use, the post-increment copy at statement start.
- Inert on the m2-23 residue (all byte-identical): parens anywhere in the sums / fold / tail; k = 1..7 int locals at 5 positions;
  1..3 dead asserts at 3 positions; declaration orders of pan/first/second/in/c; loop forms (++in in the header, i++);
  named u / t / h for the pan product or shift (before or after the first output), reference / pointer copies of *second++,
  `>>=` in place, raw intrinsics for psradi / pmaddwd, I32vec2 pointers, real VS2003 ivec.h, one reused a for b, s0/s2 reuse.
  Worse: labels in the loop body (49..55), free operator+= (2820), `*second += ..; ++second` (2868, no copy), pan-first mul_add (85).
## 0x00C79FD0 -- MATCHED (wfA6j, 2026-11-15): src/00c79fd0.cpp + test/00c79fd0.cpp, RELOCATION_MATCH, vt PASS (mutant
## dropping the >>1 fails), read-only vland RELOCATION_MATCH PASS, addmatch done (module _global, flags /GS)
- What closed it, in order: (1) the CSE form of the >>14 (m2 147 -> 23, colouring); (2) the pan product assigned to a DECLARED
  local (`Is32vec2 u; ... u = mul_add(Is16vec4((a >> 14) >> 1), pan); *second++ += u >> 14;`): an assignment to an existing object
  is not forwarded into the += tree the way a copy-initialised single-use local is, so the pmaddwd stays before the output2
  post-increment copy in c2's IL order and wins the region-2 scheduler tie (`pmaddwd mm0,mm2` before `mov edi,esi`) -> RELOCATION_MATCH.
  Hand-back spelling: `Is32vec2 a, b, u, v;` at the top of each loop body, then assignments (C style). Equivalent matches measured:
  u,v at function scope (fs1), only v at function scope (fs2), a,b,u,v at function scope (fs3), u,v at loop top (fs4),
  `Is32vec2 v; v = ...` in place (fs5). Without the CSE form (a = fold >> 14 named) the declared products do not match (2912/2928).
- Both are FITTED (marked in the source header); no header change, no device (k locals / asserts / flags) needed.

## 0x00AE3C30 -- wfA6j (2026-11-15, sixth agent; ~25 grades; parked, differ/00ae3c30.cpp UPDATED, real 129 -> 127)
- NEW (FITTED): a declared-then-assigned count `unsigned long n; ... n = in.ReadULONG(); X.resize(n);` fixes the first hunk (retail
  reloads `this` before the count at 0x00AE3E4D, RenderQuads site). The RenderQuads site alone is enough (e1); the differ uses it at all
  six resize sites for a uniform style (d3, same 127). `unsigned long n = in.ReadULONG();` (copy-init) does not do it (wfA6f).
- Still open: const 0 in ebx over 0x00AE40CB..0x00AE441E (ours: `this` piece #46). New observation: at the IndexedTextureCoords site
  retail loads _M_start into ebx for size() (`mov ebx,[esi] ; mov eax,[esi+4] ; sub eax,ebx`, 0x00AE40BE) right before `xor ebx,ebx`,
  while every other size() (retail and ours) folds it (`sub eax,[esi]`): at that point retail had a free ebx that a temp took, i.e. the
  `this` piece did not hold ebx there.
- Inert this run (127..129): declared-then-assigned no_export / index / NonSimCount / AveragePatchSize (float) / BezierEnable (bool);
  n reused for NonSimCount, no_export, the CopyTo size. Worse: a declared `long size` for the three second-half Read sizes (2286, 181).
- Not tried (cost): the landed same-TU ReadVectorArray 0x00AE1730 body above the row (its landed file uses local class models, a port
  onto the shared headers is needed first; wfB5 lesson says visible same-TU callees change capture / reload decisions).
- Also inert (127): loop counter declared before the loop / at function top / `c = 0` before the read; `resize(n, 0)` and
  `resize(n, 0L)` for ParticleIndices / VertexIndices.

## 0x00ADE080 -- wfA6k (2026-11-15, sixth agent on this row; ~150 grades + 3 lk traces) -- QUANTITATIVE MODEL of the TV term order
- Tools: tools/00ade080_rows.py (per-vector per-row x87 term order, retail ~ ours, from tmp/00ade080.ret/.our after a g2 grade);
  tools/00ade080_lk/grp.py (groups of the linearizer's product sorts from an lk.py trace, with each product's (IND key * leaf));
  tools/00ade080_model.py (the model below + (D,d) search). Probe dir probe/ade_t (gen.py/t.sh/g.sh/cv.sh/sv.sh: TV body / construction /
  SSE-arm variants on a copy of probe/ade_s/hdr with an ITQ macro for InitialiseTransposed). Traces tmp/ade_t/{base,i1,zxy}.json.
- MODEL (fits every measured variant): the LAST linearizer pass (lk trace index > 4300) sorts each TV row's 3 products by
  key = 64*id(leaf) + Ek + 0x30 (mod 0x10000); c2's comparator 0x1076A394 is a plain unsigned key compare and the EMITTED order is
  DESCENDING key order. Ek: X row x 0x4008 (E11), y 0x8008 (E21), z 0x4008 (E31); Y/Z rows x 0xc008, y 0x8008, z 0x4008.
  Leaves: X row {Fx,Fy,Fz} (v's FIELD records: all three read from memory before the first store), Y row {Cx,Fy,Fz}, Z row {Cx,Cy,Fz}
  (Cx/Cy = the TV copies xx/yy, inline locals ids ~460-476). Field records of v0..v3 are ids ~180-231, created in pairs per member-wise
  store statement (src/dest order flips with a negation), e.g. base: v0.X 180 v1.X 181 .. v0.Y 184 v1.Y 185 .. v0.Z 188 v1.Z 189,
  v2 190/224/226, v3 227/229/231; z-first stores (class B, `v1.Z=..; v1.X=..; v1.Y=..` per vector, probe c_pvZXY um 40 < 44):
  v1 Fz 183 Fx 185 Fy 189. Model output = measured rows for class A (all yzx) and class B (X yxz, Y/Z yzx), and the k-local effect
  (k unused locals shift ONLY the C ids, never the F ids: so k never touches X rows; Z rows flip at the predicted d).
- CONSEQUENCES: (1) X rows depend only on the F ids mod 1024: retail's x-first X rows need y's key to wrap (Fy mod 1024 in 512..767)
  with Fx > Fz, i.e. ~330-540 more records created before v0..v3's field records than ours (class B + D 324..538 and C shift d 271..485
  gives v1..v3 = retail in the model); no store order / k / assert can do that. (2) v0 can NOT be matched with F leaves for y/z in
  any configuration (its y/z keys would have to be ~256 ids apart): in retail v0's y and z are registers in EVERY row (X row: x from
  [34], y st4, z st3; Y/Z rows all registers), i.e. retail's v0 leaves for y/z are copies / scalars, not v0 field records -- ours
  are F records 184/188. So retail's v0 is not a field-record struct at the TV (different construction of v0 or of the TV call).
- SSE-arm text DOES move the x87 rows (new lever): InitialiseTransposed column order (probe c_i1) turns every X row into zxy; row
  permutations of InitialiseTransposed move only Z rows; removing it changes everything. Other SSE-arm spellings (GetDiagonal, ASSIGN3D
  centre, declaration positions of matrix/min/max, const half) are inert. Interpretation: records created before the F records.
- Inert (TV body level, all confirmed with rows.py): Data[][] matrix access, C3DVector copy in the TV, yy-before-xx copies, copies
  declared between rows, field reads where valid (T1..T3), comma declarations; result-temp body = CSE (2678).
- wfA6k part 2 (Ek is NOT fixed): the E-IND keys of the last pass are 0x10008 + 0x4000*n, n in 0..3 per matrix entry, and n follows the
  order in which the transform's entries are FIRST referenced (InitialiseTransposed in the SSE arm): base n = E11 1, E21 2, E31 1,
  E12 3, E22 2, E32 1, E13 3, E23 2, E33 1; column-order InitialiseTransposed (probe c_i1, trace tmp/ade_t/i1.json) gives E11 1, E21 0,
  E31 1, E12 2, E22 3, E32 0, E13 1, E23 2, E33 3 (that is why the SSE arm moves the x87 rows). Model search over n (4^9) with our F/C
  ids: 2000 assignments (class A) / 800 (class B) make v1..v3 = retail (e.g. A: E11 1, E21 0, E31 0, E12 0, E22 2, E32 2, ...); NONE makes
  v0 = retail together with v1..v3 (v0's y/z leaves must not be F records, see above). Data[][] spellings of InitialiseTransposed
  (source or dest) = E names (identical records, inert); loops change the SSE bytes (2739).
- v0 = retail rows (xyz yzx yzx) WAS reached by constructions that read v0 through the copy constructor: probe c_n3
  (`C3DVector v1 = v0; v1.Y = -v0.Y; C3DVector v2 = -v0; v2.Z = v0.Z; v2.Y = v0.Y; C3DVector v3 = -v0; v3.Z = v0.Z;`) gives v0 xyz yzx yzx,
  v1 xyz yxz yxz, v2/v3 xyz yzx yzx, 2733 bytes (um 53). The per-vector form table (probe fsw.sh/forms.py, 7 forms: member-wise XYZ /
  ZXY, ctor, Initialise, copy+assign, copy+negate, -v0+fix): member-wise yzx yzx yzx, z-first yxz yzx yzx, ctor/Initialise/copy forms
  xyz yxz yxz, -v0+fix xyz yzx yzx; NO form gives retail's v1..v3 (xzy zyx yzx): that needs the z product above the y product in both
  the X and Y rows, i.e. different E keys (n) or the y field id wrapped (F ids 512..767 mod 1024).
- Inert: the landed 0x00A5CB90 AlignedTransformTransposedAssumeW body visible as an inline above the row (FableWin COMDAT order says it
  is a header inline first used in this TU; probe v_atv.cpp: identical bytes, called, rows unchanged).
- New best FLAT source: differ/00ade080_flat3.cpp (flat2 + z-first member-wise stores) DIFFER(2735v2735) real 18 / um 40 (flat2: 44).
  The grouped probe probe/ade_s/c_g2.cpp (um 22) stays the lowest count but rests on a grouping the region boundary disproves.
- Next for a successor: find the source fact that gives retail's E-key n pattern (the entries' first-reference order: which statement
  of the SSE arm references transform first and in what order -- e.g. a CMatrix4x4 constructor / Initialise path that reads
  the 3x4 in another order but stores in retail's order) and, separately, why retail's v0 y/z are not field records (v0 read
  through a copy constructor does it in c_n3). Use tools/00ade080_model.py to test a hypothesis before grading (an lk trace = 60 s).

## 0x00AE3C30 -- wfA6l (2026-11-15, seventh agent; ~45 grades + 3 c2prio traces; parked, differ/00ae3c30.cpp unchanged, real 127)
- CORRECTED MECHANISM (trace tmp/ae3k/base_tb.txt = c2prio --trace --blocks on differ/00ae3c30.cpp; JSON with block addresses
  tmp/ae3k/base.json, reader tmp/ae3c30_blk.py): the decisive step is NOT a spill skip. In the 4th colouring pass the const-0 piece
  #103 = blocks 112..201 (IndexedTextureCoords Read .. loop guard, i.e. exactly retail's xor-ebx range 0x00AE40CB..0x00AE441E; it
  holds both long() fill-store blocks 134/157, spill > 0) has prio -106 and the `this` piece #47 = blocks 134..199 has prio -77
  (block sums reproduce both numbers exactly), so #47 takes ebx first and #103 is split into the spill -1 bits the earlier notes saw.
- Why: the pass-2 split of the `this` web (#110, after pool takes ebx) made pieces #47 {refs 134,157,179,189,199}, #8 {refs 100,101,
  111} = blocks 99..111 (esi) and #7 (first half, edi). The IndexedTextureCoords `add esi,0C4h` ref (block 111) joined the esi piece
  #8 (ours keeps `this` in esi across the count read: val scratch edi at 0x00AE4071, no reload). Retail reloads `this` at 0x00AE4096
  and uses esi as the val scratch, so in retail ref 111 was NOT in the esi piece; if it starts / joins the second-half piece instead,
  that piece is blocks 111..199 = prio about -119 < -106 and the const-0 piece wins ebx = every remaining retail hunk. So the open fact
  is the split pass's assignment of ref 111 (c2.dll 0x1078B2AA / regions 0x1078B51C), not priorities or keys: still uninstrumented.
- Block-address data: blocks are allocated per statement group in separate heap runs (first half 0x1D8..0x1E0xxxxx, blocks 83-102
  0x1AB5xxxx, 103-123 0x1B49xxxx, 124-144 0x1B46xxxx, ...); a dummy function before the row (d1..d40) moves addresses a little but
  not their order and is byte-inert, so the "seed by block address" idea of acctB r3011 does not reach it here.
- Inert this run (2278v2310, real 127, no `xor ebx,ebx`): new overlay-probe opt-in FABLE_RETAIL_CARRAY_GETBYTESIZE (FableWin
  ?GetByteSize@?$CArray@{VC3DTriangle2,VC3DQuad2,VC3DClothRenderVertex,VC2DVector,J}@@@@QBEIXZ are all COMDATs of the reader TU,
  so the original may read `in.Read(X.GetAsCArray(), X.GetByteSize())`; byte-identical, kept only in probe/ae3c30_gb/engine/CArray.h,
  NOT in hdr/); named `version` + 1..10 dead FABLE_RETAIL_ASSERT(version == 333 / <= 333) (device d); IndexedTextureCoords count as
  direct / own local / declared-then-assigned / second declared local; AllocateArrays test as `== false` / named bool / braces;
  ReadVectorArray with int 0 / in braces; GetAsCArray named pointer; sizeof first. Worse: `if (AllocateArrays(..)) {rest}` (2282, 143);
  a `CArray<C2DVector>&` reference for the site (2270, 140..142: the reference lives across the count read in esi).
- fwsize (FW_EXTRA=/EHsc) on the differ: ReadFromFile 237i v FableWin 311i (+74i, asserts or extra helper calls in the debug build;
  undecided); every helper SAME except vector<C2DVector>::size (FableWin 11i v 12i: FableWin's later STL) and Insert (-2i).
- Next for a successor: instrument c2's split pass (log each piece's seed reference and region) on this row; then look for the IL
  fact that keeps ref 111 out of the esi piece (candidates: what occupies esi/edi over blocks 103-110 at pass-2 time in retail).

## 0x00ADE080 -- wfA6m (2026-11-15, seventh agent on this row) -- BREAKTHROUGH: um 40 -> 4 (probe), all four TV vectors = retail
- NEW LEVER (measured): the SSE arm's InitialiseTransposed through a LOCAL POINTER (`const CMatrix3x4* pt = &transform;
  matrix.InitialiseTransposed(*pt);`) keeps the SSE arm byte-identical (sse=0) but gives the x87 arm its OWN IND records for the
  transform entries (created in x87 order instead of the SSE arm's IT order). With that: (a) construction of v1..v3 by CONSTRUCTOR
  `C3DVector v1(v0.X, -v0.Y, v0.Z); ...` (stores still X,Y,Z) gives v0 AND v1..v3 X rows = retail (xyz / xzy); (b) TV body with
  x/y copies and Y/Z rows grouped `xx*E1k+(yy*E2k+v.Z*E3k)` gives every Y/Z row = retail. TP GROUPED_XZY then fixes the centre.
  Probe differ/00ade080_wip_m.cpp (ProbeTV free helper = body probe): DIFFER(2735v2735) um=4, left ONLY the lea/mov ebx region
  boundary (ours line 377, retail 385): each grouped TV row x 4 expansions = 4 no-code tuples before the boundary (flat Y/Z -> lea 385
  exact, orders lost). So retail's TV is flat and Y/Z orders must come from another source fact (sweep running: flat bodies x copies).
- Dead asserts naming only transform entries (any position) and unused locals initialised from transform entries: NO effect on any
  TV row (dead / unused reads create no IND record). Asserts with class temporaries at the x87 start shift ids in bulk (n=20..40
  rotate X rows) but FableWin /Od row size (400i v ours 398i at plain /Od) leaves no room for them (one dead arm = 25i at /Od).
- Records reset per function (a dummy function defined above the row: identical). Flags /Oa /Ow /G5 /G6 /GB /QIfist /Ob1 /GS inert,
  /G7 +3 lines, /Op /arch:SSE[2] wreck the row.
- Product-key model (verified on traces tmp/00ade080_n/al1.json, z3.json; probe/00ade080_n/m2.py): k(leaf)=32*id, k(IND)=0x4000*n+8
  (n = IND id mod 4 in effect), operands sorted descending, product key = k1 + 2*k2 + 0x30 mod 2^16, row emitted in descending key.

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).
- wfA6m state 2 (honest best, saved as differ/00ade080.cpp): DEFAULT TransformVector text (3 copies, flat: the landed header text,
  no TV opt-in; XY_COPIES no longer needed), TRANSFORMPOINT_LTR, SSE arm `const CMatrix3x4& t = transform; matrix.InitialiseTransposed(t);`
  (a reference local works like the pointer; an internal pointer inside the IT body works too), v1..v3 `C3DVector v1(v0.X, -v0.Y, v0.Z);`.
  DIFFER(2735v2735) real 34 / um 34, lea/mov ebx region EXACT (385), SSE arm exact. Every X and Y row of v0..v3 and of the centre TP =
  retail. Left: the Z ROWS of all five transforms (TV v0..v3 + TP): retail y,z,x, ours x,z,y.
- Z-row analysis (lk trace tmp/00ade080_n/s3c.json + m2.py): Z-row IND residues are (E13,E23,E33) n = (1,0,0); every Z row becomes
  retail with (3,2,2) (= +2 on the three Z-row INDs only) or (0,2,2)/(0,1,2)/(1,2,3)/(3,1,2)/(3,2,3); X (1,1,1) and Y (3,2,2) must stay.
  Not moved by: named locals defined between the Y and Z rows of the TV body (locals do not shift IND ids), k unused locals anywhere
  (allocated at function entry; they only touch the SSE arm), declaration placement of the copies, construction spellings.
  Grouping the Z row (x+(y+z)) gives the right Z orders but adds 4 region tuples (lea 381). Next: find what sets the per-row IND
  "n" (a counter: X row 1, Y row 2 with the copy term +1, Z row 0 with one term +1) -- e.g. trace the IND key assignment pass.
- wfA6m alias table (probe/00ade080_n/ali.py; SSE IT / TV(v0) / TV(v1..v3) / TP each through `transform` or a reference local):
  SSE through `transform` = INDs shared with the SSE arm (x87 aliases then inert); SSE through `t` + TV(v0) through `transform`
  (1000/1001/1010/1011) = the best state; TV(v0) through an x87 reference `m` (11xx) re-orders X/Y rows (worse). 1100 x 100 flat
  bodies: v0 X row never right.
- Also measured this run (all on the wfA6m best): Z-row grouping (parens / split temp / accumulate) fixes v0 Z only and costs 4
  region tuples; Initialise(...) TV body = default; v0 spellings (ctor of three differences, `*=`, half + copy) worse; C3DVector
  copy-ctor body form (CTCTARGETINGAI_ATTACK text) inert; IT statement swaps move rows but break SSE load order (retail IT order =
  our base order, proven by the load sequence 0x00ADE16C..); random IT permutations never keep sse=0.
- Probe tools (probe/00ade080_n): fl.sh <src> [flags] (grade + lea line + SSE-arm count + rows), tv.sh (TransformVector body as a
  free ProbeTV helper on base $B), tvf.py / itf.py / ali.py / cv.py / so.py generators, fgen.py / igen.py / xgen.py body grammars,
  m2.py key model, lkr.sh (lk trace + compact TV product table), score.py. Logs: fl.log (400 flat bodies), xc.log, ig.log, big.log.
- Grades this run: ~330 (+5 lk traces). Status: NEAR-MISS, differ/00ade080.cpp (34) is the honest best; differ/00ade080_wip_m.cpp
  (ProbeTV grouped Y/Z body, um 4) shows the Z-row target but is disproven by the region boundary and uses a probe helper.

## 0x00AE3C30 -- wfA6n (2026-11-15, eighth agent) -- frame residue found and fixed (hidden by the esp-offset masking)
- NEW (measured, EVIDENCED): retail's frame is 0x10 and ours was 0xC (masked by the metric: every esp displacement shifts by 4). Retail
  packs the RenderTris resize(n) C3DTriangle2 temporary into the `this` home slot (0x00AE3D5D `lea ecx,[esp+10h]`), pair at frame 8.
  Defining FABLE_RETAIL_STLP_VECTOR (the existing opt-in that makes STLport's _M_fill_insert body visible, as every retail TU that
  calls resize() saw it; precedent 0x00AC2BE0) gives frame 0x10 with the temporary at [esp+10h] = retail. Allocation unchanged (real 127).
- Forced-priority what-if (C2_FORCEPAIR="47:-77=-200", diagnostic only, tmp/ae3n/f1): with the `this` piece #47 forced below the
  const-0 piece, the ONLY differences from retail left were (a) the frame size (now fixed, above) and (b) the missing `this` reload
  at the IndexedTextureCoords site 0x00AE4096 (ref 111 still in the esi piece #8). Everything else, including the pair-store /
  sort-argument schedule hunks, follows from the const-0-in-ebx decision. So two source facts remain: ref 111's piece and the prio.
- Inert this run: C3DTriangle2 empty user ctor opt-in (FableWin ??0C3DTriangle2@@QAE@XZ 0x02E67070; FABLE_RETAIL_C3DTRIANGLE2_CTOR
  added to hdr/engine/C3DTriangle2.h, not used), explicit `resize(n, C3DTriangle2())`, named fill local (function scope: frame 0x14),
  FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE, sibling read pattern `data / long bytes / if (bytes > 0)` (worse, 2286..2334).
- wfA6n BREAKTHROUGH (probe, real 127 -> 50, `xor ebx,ebx` present, 11 this reloads = retail): the split boundary is decided by
  edi occupancy in block 111 (the IndexedTextureCoords join block). Split rule measured on the traces (tools/ae3n_pieces.py,
  tools/ae3n_occ.py over tmp/ae3k/base.json): a `this` piece grows from one referencing block to the next only if some register of
  its set is free (block-level: a coloured candidate REFERENCED in the block occupies its register) in every block up to and
  including the next referencing block. #7 stops at block 20 because block 31 (RenderTris) references #126 esi + #127 edi; #8
  reaches block 111 because only esi (#133) is referenced there -- the GetAsCArray result (edi, #41) is first defined in the
  ternary's arm block 112. With GetAsCArray written as assign-then-branch (`T* p = _M_start; if (!(size()>0)) p = GetZeroSizeArray();
  return p;`, probe/ae3n_ga engine/CArray.h GA_A / GA_K2 / GA_G1 / GA_G2 / GA_G5 all = real 50) edi is defined in block 111, #8 stops at 101,
  the const-0 piece wins ebx and every second-half hunk closes. Left (DIFFER(2302v2310)): at the 4 sar-type GetAsCArray sites retail
  loads _M_start into eax and copies `mov edi,eax` before `ja` (the plain ternary's colouring), the select form coalesces it
  (`mov edi,[esi]`, start temp copy-forwarded into p): 8 bytes. NB qs2.sh / 00ae3c30_ag.sh read the overlay from $HDR, not from
  FABLE_HDR_OVERLAY (earlier probe-overlay grades may have used hdr/ silently). fwsize: FableWin GetAsCArray<C2DVector> 19i/52b
  = the ternary with `&front()` (ours START 19i/49b; select forms 18i/47b).
- wfA6n NEAR MATCH (probe differ/00ae3c30_wip_n.cpp, DIFFER(2310v2310) real 12, hdr/ unchanged = header ternary GetAsCArray): only
  the IndexedTextureCoords site spelled as a select in the row (`C2DVector* data = X.begin(); if (!(X.size() > 0)) data =
  reinterpret_cast<C2DVector*>(&X); in.Read(data, X.size() * sizeof(C2DVector));`), every other site keeps GetAsCArray. Left: the
  6 lines of that select (0x00AE40AC..0x00AE40B8): retail `mov eax,[esi] ; mov ecx,[esi+4] ; sub ecx,eax ; sar ecx,3 ; test ; mov
  edi,eax`, ours `mov edi,[esi] ; mov eax,[esi+4] ; mov ebx,edi ; sub eax,ebx ; sar ; test`.
- wfA6n state: differ/00ae3c30.cpp = the near match above (DIFFER(2310v2310) real 12; same as differ/00ae3c30_wip_n.cpp + header
  comment). Inert on it: k = 1..7 unused int locals; bool/count computed first (w2, x2, z1, z2), named start local (x1, x4), data
  declared at function top (u1, u2), front()/begin() spellings (w5), C-cast zero pointer (w6). Worse: start reused for the byte count
  (v1 2318: start becomes a separate candidate -- `mov ecx,[esi] ... mov edi,ecx` shape right, registers/join wrong), data = zero
  first (z3), every ternary (y1..y3, u3: real 141, split lost). Next: find the IL fact that keeps retail's _M_start load a separate
  candidate in a select-before-branch (candidate: the join's size() start load CSE'd with it -- retail's `mov ebx,[esi]` would then
  be its rematerialised second piece).
- wfA6n last measurements (all on the differ, HDR=hdr): `data = size() > 0 ? data : zero` after `data = begin()` (q1) = real 12 too (a
  ternary whose true arm is a no-op keeps the def before the branch); void* / unsigned char* / const data (t1v..t3v) = 12; `count`
  reused for the byte count (b1) 2302 (no recompute at the join, wrong). Probe overlay probe/ae3n_ga (CArray.h GA_* texts, GetByteSize)
  is measurement only, NOT for hand-back. Grades this run ~110 (+5 c2prio traces in tmp/ae3n/).
- wfA6n decisions for the coordinator: (1) FABLE_RETAIL_STLP_VECTOR (existing opt-in, landed precedent 0x00AC2BE0) now defined in the
  row: EVIDENCED by the frame (0x10, temp at [esp+10h]); (2) the IndexedTextureCoords data select in the row is FITTED (only that site;
  a select GetAsCArray in the shared header would coalesce the other four sites and contradict FableWin's 19i ternary); (3) new inert
  opt-in FABLE_RETAIL_C3DTRIANGLE2_CTOR in hdr/engine/C3DTriangle2.h (announced in hdr/README.md, used by no source).

## 0x00ADE080 -- wfA6o (2026-11-16, eighth agent on this row; ~40 grades + 1 scheduler trace) -- re-parked, best unchanged
- Grader sanity: fl.sh grades are real (a comment edit rewrites .our; flipping TRANSFORMPOINT_LTR moves 34 -> 44). Probe dir probe/ade_o
  (fw.sh = fwsize of row/IT/GetCenter; tv.sh = TV Y/Z body probe on overlay probe/ade_o/h5; h2/h3 = INPLACE_ASSUMEW probe overlays).
- NEW EVIDENCE (FableWin COMDATs right after ?InitialiseFromOBB 0x03313830): ??4C4DVector@@QAEABV0@ABVC3DVector@@@Z (0x03313EC0),
  ?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPAVC4DVector@@KM@Z (0x03313EF0, 40b/18i = 3-arg IN-PLACE wrapper) and
  ?TransformTransposedAssumeW@CMatrix4x4@@QBEXAAVC4DVector@@M@Z (0x03313F30, 36b/16i = 1-arg in-place wrapper): the original SSE arm
  reads `centre = obb.GetCenter(); matrix.TransformTransposedAssumeW(centre, 1.0f); matrix.AlignedTransformTransposedAssumeW(corners, 4, 0.0f);`.
  Probe opt-in (NOT in hdr/; probe/ade_o/h3/engine/CMatrix4x4.h, name FABLE_RETAIL_CMATRIX4X4_INPLACE_ASSUMEW): the two wrappers forwarding
  to the 2-arg TTAW / 4-arg Aligned. Plain inline: VC7.1 calls BOTH out of line (2580 bytes, sse=190: inline budget of this big row);
  FABLE_FORCEINLINE: byte-identical to the current best (34) with or without `t`, with or without ASSIGN3D. Inert for the residue, but it
  is the EVIDENCED SSE-arm text; a lander may adopt it (needs FORCEINLINE: precedent lesson 3 of this file).
- FableWin is a VC9/VC10 build (std::forward, _Debug_range, tr1::integral_constant COMDATs next to the row): its /Od float-constant code
  differs from VC7.1 (InitialiseTransposed 63i/195b v FW 67i/187b = exactly four `fldz/fld1 ; fstp` constant stores; GetCenter 20i/53b v
  22i/58b = exactly one x87-pushed 0.5f). CORRECTION: every measured VC9/10-v-VC7.1 instruction delta is >= 0 (bytes are not), so FW's
  400i is an UPPER BOUND on the original's VC7.1 /Od count, minus >= 2i per nonzero float constant the row passes (and +2i per 0.0f if
  VC9 pushes it through fldz). Ours 404i (`t`) / 402i (no `t`) is too much text by >= 8i.
- Region arithmetic re-checked (scheduler trace tmp/00ade080_sc/sc_cur.json): x87-arm regions 83/83/67, NO 0x269 tuple; tuples = emitted
  instructions incl. fxch; retail and ours both have 163 instructions (incl. fxch) before the `lea ecx,[esi+0Ch]`. Grouping any TV Z row
  would put 4 no-code tuples before the boundary: retail has none, so retail's TV is flat (confirms wfA6h).
- Measured inert/worse this run: Z-row term order permutations in the TV body (`yy*E23+zz*E33+xx*E13`, `zz*E33+...`, E-first) = byte-
  identical (records are NOT created in source walk order); x87 arm as an array `C3DVector v[4] = {half, ...}` = 271 (not scalar-replaced,
  frame moves); IT constant-row spellings (chained, int, Data[3][k], reordered) = identical /Od size (no VC7.1 spelling gives FW's fldz).
- Without `t` (SSE arm IT through `transform`, the INDs shared with IT): ctor v1..v3 = 55; MEMBER-WISE v1..v3 (`v1.X = v0.X; v1.Y = -v0.Y;
  v1.Z = v0.Z;` ...) = real 45 with ALL FOUR TV Z ROWS = retail (yzx), but every TV X/Y row is yzx for all four vectors (IND keys dominate
  the leaves: leaf-independent; retail's X/Y rows differ between v0 and v1..v3, so retail's x87 IND keys are leaf-comparable, i.e. not the
  IT-created ones). Store orders XZY/YXZ/YZX/ZXY/component-major: 43..59, none better. cpneg 79, cpas 93, assign-from-temp 218.
- Status: NEAR-MISS, re-parked (no measured progress in ~40 grades). Best stays differ/00ade080.cpp (real 34 / um 34).

## 0x00AE3C30 -- wfA6o (2026-11-16, ninth agent; ~95 grades + 1 c2prio listing; re-parked, differ/00ae3c30.cpp updated, real 12)
- EVIDENCED: the five sar-type retail GetAsCArray sites (RenderQuads 0x00AE3E63..6F, RenderVertices 0x00AE3F45..51, ITC 0x00AE40AC..B8,
  ParticleIndices 0x00AE419A..A6, VertexIndices 0x00AE4284..90; RenderTris divides by 6) are the same sequence (`mov eax,[esi] ;
  mov ecx,[esi+4] ; sub ; sar ; test ; mov edi,eax ; ja ; mov edi,esi`), so the original ITC site is most likely the plain
  GetAsCArray() ternary; none of the ~25 select spellings tried (this run + wfA6n) gave the last 6 lines (the def absorbs the start load).
  Ternary base = tmp/ae3o/g1.cpp (real 127, 6 this reloads, no `xor ebx,ebx`, first divergence = val scratch edi v esi at 0x00AE4071).
- Inert on the TERNARY base (all exactly real 127 / reloads 6): k = 1..7 unused int locals; dead FABLE_RETAIL_ASSERT x1/2/4 at function
  top, after AllocateArrays, after ReadVectorArray, after the ITC resize; ReadVectorArray body (landed text, noinline and plain) above
  the row; visible noinline resize(n) bodies for C2DVector / all three specializations; resize(in.ReadULONG()); block-scoped count;
  a CArray<C2DVector>& after the count (140); WriteToFile-style `const CArray<T>* const& array` view before / after / around (127..140);
  named data local; GetAsCArray default &front() text (127); `long bytes` first (145); NonSim total local (133) / bool ok (127); /Oa (worse).
- Inert / worse on the SELECT base: condition-first bool / count (+ dead assert reuse to block forward-substitution); start named + dead
  asserts; `data = zero; if (size()>0) data = begin()` (real 14: start becomes its own load in ebx, branch inverted, finish loaded first);
  start reused in the count (2318); `<=` / `0 >=` / `0 <` (12), `== 0` / `!size()` (13: test 0xfffffff8; jne).
- HYPOTHESIS, unmeasured (select residue): the load goes to whichever of {data, CSE start temp} comes first in IL; every C spelling with data's
  def before the branch puts begin() first (a single-use condition temp is forward-substituted into the branch), so retail's order
  (start temp loaded by the size, data = copy after the test) needs the ternary, whose def sits in the arm block (and loses the split).
- Hand-back improvement: FABLE_RETAIL_CARRAY_GETASCARRAY_START removed from the differ (default text, = FableWin GetAsCArray 19i), same bytes.
- FableWin TU facts (fablewin_pdb_names.tsv 0x0333D440..0x0333E9A0): the particle TU instantiates GetAsCArray, PeekAsCArray (const) and
  GetByteSize for C3DTriangle2 / C3DQuad2 / C3DClothRenderVertex / C2DVector / long: the reader probably spelled the byte count
  GetByteSize() (byte-identical here per wfA6l; header opt-in not added).
- Also inert on the ternary: an AllocateArrays body (header members; pool calls through two probe externs, measurement only) above
  the row, noinline (127). ReadGroups body not tried (called after the ITC site, no row local passed to it).
- Status: NEAR-MISS, parked. Coordinator: the 6-line residue is the select's (FITTED) start-load ownership; no natural closer found.
- wfA6o bounded /Od-window check (probe/ade_o/g_*.cpp, fw.sh then grade): v0 = obb.GetDiagonal() and SSE half = obb.GetDiagonal()
  (FABLE_RETAIL_C3DBOUNDINGBOX_GETDIAGONAL, FW 22i) = BYTE-IDENTICAL to the best (34) and -16i at /Od (388i with `t`): EVIDENCED shrink.
  With the FW-COMDAT SSE arm too (INPLACE_ASSUMEW FORCEINLINE wrappers + `centre = obb.GetCenter()` via ASSIGN3D): g_fwt.cpp (with `t`)
  385i = identical bytes (34); g_fw.cpp (no `t`) 383i = 55 (IND-shared state). Window fit: 383 + 2 (1.0f) + 10 (five 0.0f) = 395 <= 400;
  the no-`t` ctor-centre g_gdbn 386 + 14 = 400 exactly. Corner locals `{ C3DVector p = centre - v; Min.LimitUpper(p); Max.LimitLower(p); }`
  = 2578 / 378 (and 376i, below the window); member-wise v1..v3 under `t` = 65. No Z row moved in any of them. probe/ade_o/g_fwt.cpp is
  the most-evidenced equal-best source (needs the INPLACE_ASSUMEW opt-in, probe overlay probe/ade_o/h3, not in hdr/).

## 0x00AE3C30 -- wfA6s (2026-11-16, successor tenth agent)
- Inert: CParticleProgram::ReadGroups (0x00AE3A60, retail neighbour directly above the row = same TU) defined above the row with the
  landed body text (probe/ae3s/t_rg.cpp ternary base: 2278v2310, 60 hunks as before; probe/ae3s/s_rg.cpp select base: 2310v2310, the
  same single 6-line hunk). Needed a probe-only opt-in FABLE_RETAIL_C3DGROUP2_BLENDS (GetVertexBlends 0x00AC4080 / Resize 0x00AC41D0)
  in probe/ae3s/h/engine/C3DGroup2.h; NOT in hdr/.
- wfA6s select-side measurements (tools/ae3s_mk.py replaces the ITC block of the differ; tools/ae3s_g.sh grades + prints our site):
  the residue is WHICH candidate owns the _M_start load, and it is the CSE first occurrence: in every select spelling the first
  occurrence is data's own assignment, so c2's CSE temp for the condition / join start is a COPY of data (`mov edi,[esi] ; mov ebx,edi`,
  the temp spans to the join, whose `mov ebx,[esi]` is its rematerialised piece). With the def of data in the arm (z3 / p6: `data = zero;
  if (size() > 0) data = begin()`, or the ternary) the temp owns the load (`mov ebx,[esi]` / `mov eax,[esi]`) as in retail. Statement order
  does not change it: count first + named start + data = start (p5), comma `data = (size(), begin())` (p4) = the same 12 lines, i.e. c2
  forward-substitutes / copy-propagates before CSE. Ternary with a pre-assigned data (p1 `data = zero; data = c ? begin() : data`, p2 with
  zero in both) = real 141 (jbe/jmp junk, split lost); zero first + nested GetAsCArray (p3) = 2326. z3 / p6 (real 14) are the closest
  "temp owns the load" forms; left there: arm order (zero hoisted, `jbe` + `mov edi,ebx`) and the temp unsplit in ebx (retail: eax, then
  the join piece in ebx), finish loaded first.
- Retail fact noted: the five sibling count reads all reload `this` from [esp+10h] after the read; for RenderQuads / RenderVertices /
  ParticleIndices / VertexIndices the previous site's Read ends in `rep movs` (esi/edi pinned), for ITC it follows the ReadVectorArray
  call (this still in esi) -- that is why ITC is the only site where the `this` piece can run on in ours.
- Status: re-parked, differ/00ae3c30.cpp unchanged (select, real 12). ~12 grades this run.

## 0x00ADE080 -- wfA6p (2026-11-16, ninth agent on this row) -- the IND key is a VALUE-NUMBER id (c2 code read + 4 traces)
- c2 code (13.10.3077): node key 0x1075d388 -> 0x1075d54e (dispatch by node class byte +8). IND (op 0x252, class 6, path
  0x1076c720): key = (fold(+0x30) + 8 + (key(address child +0x34) << 8)) & 0xffff. Leaf (class 2, path 0x1075df16): if the record's
  +0x14 (defining expr) is set -> key of that expr; else if record kind byte == 3 -> id << 6 (0x1076c28d); else fold(id) << 5.
  So an E entry's IND key = 0x4000 * (address-temp id mod 4) + 8. E11's address is the `transform` leaf itself (id 2, fold path)
  -> key 0x4008 always (n pinned at 1).
- The address temps are kind-3 records created by c2's value numbering (0x107636xx walk -> 0x10761623 hash lookup -> new entry
  0x10761516 -> temp record 0x107615bc): one new id per NEW unique expression (op, vn1, vn2, type) in IL walk order (ADD 0x278,
  IND 0x252, 0x264 load, MUL 0x27a, NEG 0x268). Trace of the best (tools/00ade080_lk/wvn.py + vnshow.py, tmp/c2re/vn1.json):
  v0 halves 1260-62, negations -v0.Y/-v0.X 1263/64, then TV(v0): X row E31 addr 1265, E21 1269, add, E11 (3 VNs), add (13 VNs);
  Y row E32 1278, E22 1282, add, E12 1287, add (14); Z row E33 1292, E23 1296, add, E13 1301, add (14). TV(v1..v3) and TP reuse them.
- Model (tools/00ade080_vn.py, reproduces ours exactly): rows (E1k,E2k,E3k) = (s+1, s, s) per row start s (E11 = 1); retail needs
  s_X in {0,1}, s_Y in {2,3}, s_Z = 2. With 13/14/14, s_Z = s_X + 3 (mod 4) for EVERY flat or grouped 3-term TV body walked first
  (term order / grouping / copies never change the per-row count; copies are leaves, not VNs): the `t` state CANNOT reach retail
  with these leaf ids. That is why ~1500 grades of TV-body / construction sweeps never fixed the Z rows. A bulk shift (asserts,
  k locals, anything before TV(v0)) moves all three starts together and cannot help either.
- Tools: tools/00ade080_lk/lk2.py (+lk2_gdb.py: comparator log + IND address trees), indf.py (final-pass IND -> address record),
  recr.py (records by id), wp.py (hw write watchpoint on record fields), wvn.py (+wvn_gdb.py: every new VN with its IL expression;
  args lo hi = id window), vnshow.py. c2 disassembly: tmp/c2re/c2.dis (objdump of $FABLE_VC71/bin/c2.dll).
- wfA6p no-`t` state (tmp/00ade080_nt.cpp = differ minus `t`, DIFFER(2729v2735) real 55; traces tmp/c2re/vnnt.json, nt.json): the SSE
  arm's InitialiseTransposed numbers E11's load (1192/93) then one address+IND+load per entry in IT order E21,E31,E41,E12,...,E43
  (stride 3: ids 1194..1224); the x87 TVs create NO address/load VNs (reuse all, even across the if-arms). Final IND classes
  X (1,2,1), Y (3,2,1), Z (3,2,1); tools/00ade080_vn2.py (no-t leaf ids) reproduces the grade's rows exactly. Allowed triples
  (E1k,E2k,E3k) are the same in both states: X {(1,0,0),(1,1,0),(1,1,1)}, Y {(0,2,2),(0,3,3),(1,3,3),(3,2,2),(3,3,3)},
  Z {(0,1,2),(0,2,2),(0,2,3),(1,2,3),(3,1,2),(3,2,2),(3,2,3)}. IT stride 3 gives E2k - E3k = 1 in every row; Y needs 0: no bulk
  shift of the IT ids works (best 4/12). So BOTH pure states are structurally impossible with today's leaf ids.
- wfA6p mixed search (tools/00ade080_vn3.py, `t`-state walk, entries in set P pre-existing = 1 VN instead of 4, own class): the
  smallest P that fixes all 12 TV rows has TWO entries, base unchanged (s_X = 1); e.g. {E32,E33} classes (3,3), {E12,E33} (0,2),
  {E33,E13} (2,0), {E22,E12} (2,3), {E32,E12} (3,1) -- each of these is ALSO what an IT-stride numbering gives for that pair
  (b = 0/3/3/2/0), i.e. retail behaves as if exactly two Y/Z-row entries were loaded through `transform` before TV(v0) and the
  rest were fresh. No source structure that produces that is known (IT reads all twelve through one reference; dead reads create
  no VN). Leaf-id alternative ruled out for xx/yy (would need copy record ids ~1022..1200 and then breaks the Y rows).
- Status: NEAR-MISS, re-parked with the model. Best stays differ/00ade080.cpp (real 34 / um 34), unchanged. ~12 grades + 9 traces.

## 0x00ADE080 -- wfA6q (2026-11-16, tenth agent on this row; ~45 grades + 1 VN trace) -- count-space model VALIDATED; re-parked
- NEW TOOL tools/00ade080_vncnt.py: count-space model (per-row VN walk E3k addr+3, E2k addr+3, ADD, E1k (E11: 3), ADD; extra VNs at
  positions 0..5; base b). Ours = b 1, 13/14/14 -> classes X(1,1,1) Y(3,2,2) Z(1,0,0) (8/12, = the notes). 12/12 with b 1 and ONE extra
  VN per TV(v0) row at any position after the E2k address (positions 2..5), or X+1/Y+1 only, or Y+2 at the row end, or extras at row
  START Y+1/Z+1 (0,1,1) / Z+2 (0,0,2); also b 0 with +1/row. This is a different target from wfA6p's "two pre-loaded entries".
- VALIDATED by an artificial probe (tmp/00ade080_q/a1.cpp: ProbeTV default 3-copy body with `+0.0f` at the end of each row;
  `*1.0f`, `-0.0f` identical): ALL FOUR TV Z ROWS = retail (yzx), every X/Y row kept, real 14 / um 14, length 2735. VN trace
  (tmp/00ade080_q/vn_a1.json, window 1240..1440): the extra VN is op 0x269 (unary, 269(row sum)) at the end of each row. Left in a1:
  lea/mov ebx boundary (ours line 373, retail 385: the 0x269s are the no-code tuples wfA6o's region arithmetic excludes), GetCenter
  add order (Max.X first in retail), TP Z row (ours zxy). So retail has one extra VN per TV(v0) row that is NOT a tuple (deleted before
  tuple generation) -- the natural source of it is still unknown.
- Natural candidates graded (none moved a Z row): `(float)((double)sum)` (no VN, lea 374), `const C3DVector c = v;` copy body (= 34),
  `const float xx=v.X, yy=v.Y, zz=v.Z;` one declaration (= 34), `v = C3DVector(rows...)` / `C3DVector r(rows); v = r;` (2825, frame),
  accumulate in a float `r` / `v.X += ...` (2737), temporaries nx/ny/nz then stores (real 30 / um 30, Z rows unchanged, temporaries'
  order inert = VN walk follows the stores; body larger than FableWin's 55i: measurement only, NOT a candidate). TV row ORDER
  permutations (r2..r6) and centre/TP before the TVs (q1/q2) are structural breaks (store order / length): do not revisit.
- FableWin /Od: default TV text is 55i/150b SAME (fwsize), so the TV body text is fixed; the extra VN must come from the context of
  TV(v0) (what v / transform are when TV(v0) is expanded), not from a different body.
- Devices measured: dead asserts (fable_assert.h if(0) form) at the x87 start n = 1,2,3 and function top n = 1,2: byte-identical (34);
  n = 5,7 only break the SSE arm (68). Uninitialised unused locals (8, 32) inert; initialised ones emit code. Model: a bulk shift cannot
  fix the Z rows (Z start = X start + 3 with 13/14/14). Devices c/d CLOSED for this row.
- No-`t` state joint search (vn2 model, b0 = IT base, uniform leaf shift d): 12/12 at b0 = 1 (ours b0 = 2) with ALL leaf ids +14..+51;
  no device found that shifts leaf ids alone (x87-arm asserts change rows and length: 2729 -> 2735, real 68). Open lead.
- Status: NEAR-MISS, re-parked. Best stays differ/00ade080.cpp (real 34 / um 34), unchanged. Probes in tmp/00ade080_q/.

## 0x00AE3C30 -- wfA6t (2026-11-16, eleventh agent; ~55 grades; re-parked, differ/00ae3c30.cpp unchanged = select, real 12)
- CORRECTION of wfA6o's premise ("five sar-type sites byte-identical => ITC is the plain ternary"): only the COND parts are identical.
  The JOIN (byte-count recompute) differs: RenderQuads / RenderVertices / ParticleIndices / VertexIndices read `mov eax,[esi+4] ; sub
  eax,[esi]` (no start candidate), ITC reads `mov ebx,[esi] ; mov eax,[esi+4] ; sub eax,ebx` (start in a register = a CSE temp's join
  piece, as in every select / zero-first form). So the ITC source is NOT the same statement as the other four sites.
- Measured, all on tools/ae3t_g.sh (grade + markers valesi = `mov esi,[eax]` count (retail 9), thisrel = `mov esi,[esp+10h]` (8),
  xorebx (1)) and tools/ae3t_site.sh (prints our ITC site); variant generators tools/ae3t_mk.py (dead asserts), tools/ae3t_mk2.py
  (replace count read .. ITC block), tools/ae3s_mk.py (ITC block body):
  * Dead asserts per site (FABLE_RETAIL_ASSERT(X.size() == n) after each of the 6 resize(n) / after each Read / both / ITC only) on
    the ternary base: byte-identical (real 127). Device d inert here (if(0) arms with no class temporaries leave no IL trace).
  * CParticleSimulation::ReadFromFile 0x00AE1BC0 (pending differ, hand321/Q24; NOT landed) visible above the row: inert (127).
  * Row-level ternary / if-else (`size() > 0 ? begin() : zero`, `&front()`, `&X[0]`, start local, in the Read arg, if/else): all real
    141 with `jbe ; jmp ; mov edi,esi` (the true-arm copy coalesces with the load). Only the INLINE GetAsCArray ternary gives the clean
    `mov eax,[esi] .. mov edi,eax ; ja ; mov edi,esi` shape.
  * A reference / pointer to the array (`CArray<C2DVector>& tc = X;` before / after the resize, in a block, pointer): real 140, the
    whole count is CSE'd at the join (no recompute) -- so a shared base makes c2 reuse the cond count, which retail does not.
  * NEW FAMILY U4 (differ/00ae3c30_wip_t.cpp, real 10, 2318v2310): `C2DVector* data = X.GetAsCArray(); in.Read(data, (X.end() -
    X.begin()) * sizeof(C2DVector));` gives the retail cond shape AND const 0 in ebx (all second-half hunks close) with the ternary,
    because the join's end() is read this-relative (`mov edx,[esp+10h] ; mov eax,[edx+0C8h]`: a single-use inline `this` is folded
    into the address; size() uses its `this` twice and keeps esi) -- a `this` reference after the ITC flips the const-0 / this
    priority. Left: +8 bytes (that reload) and the esi `this` piece still runs through the ITC count read (val edi, no reload at
    0x00AE4096). Inert on U4: resize(in.ReadULONG()), own count local, k = 1..3 unused locals, `&front()`, const end() cast,
    named finish, bytes local, sizeof first; `-(begin() - end())` 14; un-named data (U2) 153.
  * Select + U4 join (SJ1: `data = begin(); if (!(size() > 0)) data = zero; Read(data, (end() - begin()) * 8)`): OWNERSHIP FLIPS to
    the temp (`mov ecx,[esi] ; .. ; mov edi,ecx ; ja ; mov edi,esi`, join reuses ecx), real 20 / 2318 (end() this-relative again,
    plus a `mov edx,ecx` copy). So the join's start expression decides who owns the start load: a row-level begin() at the join makes
    the temp own it; size() at the join (param-based start) leaves it with data. Zero-first + U4 join (SJ2) 18.
  * Also inert / worse: `n = X.size()` before or after data (X1 / X3 = the select's 12 exactly: not forward substitution, c2 picks the
    user variable as the load owner whatever the IL order), `n = size(); data = GetAsCArray()` (127), `bytes` first (S1 16: temp
    owns via the byte count, but no join recompute), SGI-style size() `end() - begin()` probe overlay (263, discarded).
- Open fact (for a successor): a join expression that reads start through a row-level begin()-like CSE (to give the temp the load,
  SJ1) but finish through size()'s esi base (no this reload), or the U4 family plus whatever ends the esi `this` piece before ITC.
  Probe files: probe/ae3t/*.cpp (U*, SJ*, T*, X*, V*, K*), probe overlay probe/ae3t/h (PROBE_SGI_SIZE, measurement only).
- Status: NEAR-MISS, re-parked. Hand-back best stays differ/00ae3c30.cpp (select, DIFFER(2310v2310) real 12, equal length).
- wfA6t addendum: SJ3 (join `(begin() + size() - begin())`) 18, SJ4 (cond `end() - begin() > 0`, join size(): end() this-relative,
  `test eax,0FFFFFFF8h ; jg`) 17, SJ5 (join `(char*)end() - (char*)begin()`, temp owns the load but end() this-relative) 22.
  Total this run ~58 grades; stop rule reached (no family closes; best hand-back unchanged).
- wfA6q addendum: op 0x269 IS an explicit float cast: `v.X = (float)(row)` on every TV row = the `+0.0f` probe exactly (d1, real 14).
  Casts on the X and Y rows only (e2, saved as differ/00ade080_cast.cpp, PROBE helper, FITTED): every TV row = retail, real 14, lea 377
  (8 cast tuples before the boundary; 12 casts -> 373; so one line per no-code tuple). Casts on Y/Z only (e4) = 42. XY_COPIES + casts
  (e1/e3) = 51 (z leaf changes break X/Y). Casts in TP rows (ProbeTP, LTR text, X/Y or all rows): inert (no tuples, no row change).
  Residue of e2: GetCenter add order, lea/mov ebx 8 lines early, TP Z row (zxy v retail yzx). Next lever if anyone continues: an
  extra VN per TV(v0) row that does NOT survive to tuple generation, or 8 fewer no-code tuples elsewhere before the lea (TV / TP / ctor
  copies are no-code tuples); the casts themselves are fitted text a programmer would not write (not a hand-back candidate).
- wfA6q CORRECTIONS to the lines above (read these as authoritative): (1) "non-tuple VN" is a HYPOTHESIS: the only extra VN produced
  (the cast, op 0x269) IS a tuple. Measured: one extra VN per TV row (or X+1/Y+1) fixes every TV row. The count-space fits still open
  side by side: this one, wfA6p's two pre-loaded Y/Z entries, and no-`t` with b0 = 1 plus a uniform leaf shift of +14..+51.
  (2) Only the lea is tied to the tuple count (373 with 12 casts, 377 with 8). GetCenter flipped although the extra VN total is
  0 mod 4, and TP Z was already wrong at 34: both UNEXPLAINED. (3) The cast text was not compiled at /Od under the name
  TransformVector, so "FableWin size fixes the TV body" is unproven for casts (VC7.1 drops a float->float cast at /Od, probably 55i).
  (4) Casts on entry loads (`yy*(float)m.E21`, `xx*(float)m.E11` in the X/Y rows; f1/f2) are dropped: no VN, byte-identical (34).
  Grades this run: ~57 + 1 VN trace.

## 0x00AE3C30 -- wfA6u (2026-11-16, twelfth agent; ~6 grades + 3 c2prio listings; re-parked, differ/00ae3c30.cpp unchanged = select, real 12)
- PREMISE SETTLED (no compile): wfA6t's "the ITC join differs, so ITC is a different statement" is VOID. wfA6n's forced-priority run on the
  plain-ternary base (tmp/ae3n/f1/o.n lines 430-450) emits retail's ITC site exactly: cond `mov eax,[esi] ; mov ecx,[esi+4] ; sub ; sar ;
  test ; mov edi,eax ; ja ; mov edi,esi` AND join `mov ebx,[esi] ; mov eax,[esi+4] ; sub eax,ebx ; sar ; shl ; xor ebx,ebx ; cmp eax,ebx`.
  So the start-in-ebx join is an allocation consequence of the const-0 ebx piece starting there, not a different source. c2prio datum
  agreeing: the cond start temp is 3 refs in eax in both the ternary base (tmp/ae3u/g1.prio, #128) and U4 (tmp/ae3u/u4.prio, #157), i.e.
  it never reaches the join in either; U4's join `mov ebx,[esi]` is a separate load, not a CSE piece.
- NEW THIRD SOURCE (Anniversary 360 Test build, weak: edited after TLC; method of wfB5/probe/anniv): ?ReadFromFile@CParticleProgram is
  module 154 (Unity_lib_particle_simulationEtAl, file bbblibrary/cpp/lib_particle_simulation.cpp), 0005:0091D0F0, 3736 bytes, lines
  962..1056 (probe/00ae3c30_anniv/m154_lines.txt, m154_syms.txt). Each array is ReadULONG / resize / Read on three lines (RT 974-976,
  RQ 983-985, RV 993-995, ITC 1010-1012, PI 1020-1022, VI 1030-1032) plus a 360-only byte-swap loop (2 lines). The ITC and RQ PPC
  sequences are instruction-identical (resize; cond loads start+finish, `mr r4,start`, `bne`, `mr r4,&X`; join reloads finish, REUSES
  start) => ITC is the same statement as the other sites: plain `in.Read(X.GetAsCArray(), X.size() * sizeof(T))`. Params are named
  `is` / `alloc`; the only stack local is `name` (CCharString); n is reused as the swap-loop count (a named function-scope count local).
- So the evidenced target is the ternary GetAsCArray at ALL five sites (tmp/ae3o/g1.cpp, real 127); the select at ITC (differ, real 12)
  is a FITTED stand-in for an allocation outcome. The one open fact: with the ternary text, what ends the esi `this` piece before
  0x00AE4096 (equivalently: const-0 piece outranks the late `this` piece; wfA6n's forced what-if shows everything else then follows).
- FableWin particle-TU COMDATs (0x0333E010..0x0333E9A0): per type GetAsCArray (19i/52b), PeekAsCArray const (19i/52b), GetByteSize
  const (10i/22b = `return size() * sizeof(T);`). GetByteSize is byte-identical to size()*sizeof here (wfA6l); no opt-in added.
  FableWin vector<C2DVector>::size 11i/24b comes from another TU (0x0195B404), not the particle TU: ignore.
- Inert this run (ternary base, probe overlays probe/00ae3c30_u/h{a,b,c}, NOT in hdr/): a dead FABLE_RETAIL_ASSERT inside
  vector::front() (FableWin front() is 29i v ours 9i: the only helper text inside the ternary's true arm) with `!empty()`,
  `size() > 0`, `_M_start != _M_finish`: all real 127, valesi 8 / thisrel 5 / xorebx 0 (= unchanged). Asserts without class
  temporaries leave no IL trace (as wfE3 found).
- Closed families (do not reopen): select spellings (~25: the user variable always owns the start load), U4 / SJ* (unequal length or
  ownership/end() wrong), k locals and row-level asserts on both bases (inert), device b (calls equal: not an inline residue).
- Status: NEAR-MISS, re-parked. Coordinator decision needed: accept that the evidenced text is ternary-everywhere (real 127) and the
  hand-back select (real 12) is FITTED; the row needs a source fact that changes the late `this` / const-0 piece priority.

## 0x00ADE080 -- wfA6r (2026-11-16, eleventh agent on this row; ~100 grades + 4 VN traces) -- re-parked; GetCenter ruler found (device c)
- Anniversary 360 PDB: no InitialiseFromOBB procedure (one TPI method-list hit only; C3DBoundingBox procs = ctor, CalculateMatrix): the
  line-table tip cannot apply. FableWin.exe itself is not on this machine (signatures.tsv holds sizes/hashes only).
- /Od count v FableWin 400i (fwsize, absolute src path; probe/ade_o/fw.sh breaks on relative paths): best 404i; EVIDENCED SSE text
  (`C4DVector centre; centre = obb.GetCenter();` ASSIGN3D + in-place wrappers TTAW(centre, 1.0f) / ATTAW(corners, 4, 0.0f), probe opt-in
  FABLE_RETAIL_CMATRIX4X4_INPLACE_ASSUMEW in probe/ade_o/h3) with `t` 401i, without `t` 399i (/EHsc identical). So the evidenced, t-less
  text sits inside the FW bound (400 minus constant corrections); `t` is the only item that pushes past it. Evidenced text is
  BYTE-IDENTICAL in the t-state (34) and in the no-t state (55): saved as differ/00ade080_evid.cpp (grade with HDR=probe/ade_o/h3).
- GetCenter X add (x87 arm, 0x00ADE5CD Max.X first in retail; only X flips, Y/Z right) is a k-local RULER (period 8; which record is responsible is UNPROVEN: in g2 the temp moved L608 -> L609 with no flip, consistent with but not proof of a temp-id key). VN traces (wvn.py; tmp/00ade080_x/
  vn_best.txt, vn_g2.txt, vn_n1.txt) show the x87 GetCenter's Max loads go through a fresh inline `&obb.MaxCoord` temp (best L608, SSE arm
  L302; Min.X is the `_obb` leaf (id 3) itself, address offset 0, VN 1157 shared by both arms). The VN ADD record operand order = emitted
  order on 4 data points (SSE 1158 ADD(Max,Min); x87 best 1353 ADD(Min,Max) = Min first; x87 k5 1353 ADD(Max,Min) and n1 1327 ADD(Max,Min) = Max first). k unused `int uK = 0;` at function top (period 8): k = 5,6,7,13,14,15 -> real 30
  (GetCenter fixed, nothing else moves); k = 3,4,11,12 fix GetCenter but move 4 other lines (34); k = 2,10 -> 38. Saved
  differ/00ade080_k5.cpp (device c, FITTED, `// Fidelity:`), NOT the hand-back: the TV Z rows / TP rows are still open, and devices only
  count once the structure is right. evid + k5 = 30 too. My simple key formula (id<<6 class v obb) did not fit both arms: read the
  comparator (lk2) before modelling it.
- Negative (all graded this run): GetCenter texts on the cast probe (CENTER_MIN_FIRST opt-in, explicit (Min+Max)/(Max+Min)*0.5f: 14,
  inert; centre declared at x87 top 163); 2-arg TransformPoint(obb.GetCenter(), centre) SOURCE_DEST_SUMS / named source / COPY_SUMS on the
  cast probe 28/28/36 at 2729; no-`t` + cast 59; evidenced SSE + cast (t) 14; GetDiagonal for v0 (34, identical), `C3DVector centre(...)`
  (34), `centre; centre = GetCenter()` (2779); k sweep on no-t evidenced n1 k = 1..55 odd: 59..130, never < 55; k sweep on the cast probe
  k = 1..8: 14..26 (k5 = 16: GetCenter fixed but TV(v3) Z row breaks); TP texts on cast / cast+k5 (none, PARTICLE_POINT, SUMS, ZYX_SUMS,
  ZXY_LOCALS, GROUPED_XZY): 14..25, never better; XY_COPIES + k = 0..7: 55..69 at 2733.
- Device c is closed for the TV Z rows only (bulk shift: VN record ids shift with k too, so s_Z - s_X is invariant); uninitialised
  locals are inert, INITIALISED ones move GetCenter. Grades this run ~100 (+4 VN traces).
- Status: NEAR-MISS, re-parked. Hand-back best stays differ/00ade080.cpp (real 34). Probes: differ/00ade080_k5.cpp (30, device c),
  differ/00ade080_evid.cpp (34, evidenced SSE text, probe opt-in), differ/00ade080_cast.cpp (14, FITTED casts). Next lever for a successor:
  the TV Z rows still need the extra VN per TV(v0) row (wfA6q); with that found, add k=5 (or its equivalent) for GetCenter and re-measure TP Z.
