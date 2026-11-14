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
