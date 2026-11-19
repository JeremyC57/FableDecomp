# hand2c NOTES (2026-11-19)

Tools: tools/g.py <addr> <src> "<idea>" (grade + relocation-masked diff, logs ATTEMPTS.log), tools/vland.sh <addr...> (strict read-only
vland on src/+test/), tools/mkpayload.py. Baseline of every draft: keep/base/summary.tsv (grade, hunks, difflines) + keep/base/<addr>.txt.

## Matched
- 0x006C06D0 CTCBuyableHouse::GetRentMoneyPos -- RELOCATION_MATCH + test PASS (vland strict). Closed by porting the local model onto
  the shared headers (CTCBuyableHouse / CRightHandedSet / C3DVector / C2DVector) with Ego_d's locals (forward copy normalised, offset
  C2DVector from rhs.GetForward() * 0.7f) and the existing C3DVector opt-in NORMALISE_SQUARED_ZXY. New opt-in:
  FABLE_RETAIL_CTCBUYABLEHOUSE_GETRENTMONEYPOS (declaration). NAME_FIX: the landed caller binds a public pointer spelling.

## Open rows
(see STATUS.tsv)
- 0x00F36540 NFractal::CPerlinNoise::GetGaussian2DNoise -- RELOCATION_MATCH + test PASS (vland strict; test also passes on g++ via the
  FABLE_PORTABLE twin). Closed by: shared engine/CPerlinNoise.h, the existing GFFloatToLongFloor helper (no pragma y-off needed: the
  helper's asm already gives the ebp frame), Ego_d's locals, and the PDB inline GFGetInterpolatedValue with its FableWin /Od-proven
  NAMED body: the extra delta tuples move the scheduling-region cut so the epilogue pops precede the two x87 cleanup fstp
  (0x00F36675). Measured with a 40-variant sweep (probe/v00f36540.py): NAMED + one-expression result matches with fade spellings
  F2/F3/F5; picked `(3.0f - (2.0f * v)) * v * v`. Ego_d's v1_x / v1_y are dead (byte-neutral) and left out (no Fidelity line needed).
- 0x00F36010 NFractal::CPerlinNoise::Get2DNoise (catalog: "CPerlinNoise::GetGaussian2DNoise" -- wrong, NAME_FIXES) -- RELOCATION_MATCH
  + test PASS. Ego_d.pdb's CPerlinNoise class record (types.txt 0x232DF) lists private inline const helpers SCurve, Interp(a,b,t),
  LookupIndex(i), LookupGradient(i) (C3DVector BY VALUE: that is retail's 12-byte frame temporaries), LookupScalar(i); added to the
  overlay header under FABLE_RETAIL_CPERLINNOISE_GETNOISE with bodies sized from their Ego_d /Od lengths. Hash order b00, b10, b01,
  b11; gradients C2DVector from LookupGradient; values via C2DVector::Dot(C2DVector(v_x, v_y)).
  Header grouping chosen jointly over the three CPerlinNoise rows: SCurve `t*t*(3.0f-(2.0f*t))` (left-assoc required by the /Od
  length), Interp `a+((b-a)*t)`.
- 0x00F36540 re-done on those helpers (LookupScalar / Interp / SCurve); still RELOCATION_MATCH.

## Open / escalated
- 0x00F361D0 NFractal::CPerlinNoise::Get3DNoise -- DIFFER(873v873), 6 diff lines, all scheduling: retail 0x00F36303/0x00F3630D
  `fmul st(2)` (SCurve(v0_y)'s two products) sit after `mov eax,esi` / `and eax,0xff`, ours before; retail 0x00F3640F `fsub st(1)`
  (result_z0's outer Interp) precedes gradient001's address arithmetic, ours after. Source differ/00f361d0.cpp (statement order v0,
  v1, b, t; gradients/values in 000,100,010,110 order; C3DVector::Dot default). Tried (sweeps, ~300 grades): Dot opt-ins (all worse),
  statement-block orders (8), gradient/value interleavings (18), parenthesised lines (35), helper groupings (SCurve 9 x Interp 6 x
  Lookup 12). The right-assoc SCurve `t*(t*(3-2t))` gets it to 2 lines but contradicts SCurve's /Od length. ESCALATED.
- 0x007E2DF0 CTCWobble::OnInitialActivate -- RELOCATION_MATCH + test PASS. Ported onto engine/CTCWobble.h (PASS22) with a new
  OnInitialActivate declaration opt-in; ParentRHSet assigned straight from `const CRightHandedSet& GetRHSet()` closed the edi/edx copy
  residue; the second virtual is SetRHSet (+0x12C, slot 75) -- the old draft and the landed FrameUpdate call it SetFacingAnglesFromRHSet.
  GFFloatToLongCeil helper; no pragma.
- Coordinator's double-literal candidate for the CPerlinNoise rows: refuted by one grade (VC7.1 keeps qword constants; retail is dword).

## Open, worked
- 0x00AC7EB0 ComputeSelfContraction -- DIFFER 2 lines: `tex_diff *= tex_diff` (in place, Ego_d has no squared local) fixes the tex term;
  left: normal term retail `fld st(1) ; fmul st(2) ; fmul [ecx+78h]` (n*n then *w), ours n*w*n. Tried: 9 spellings of the term, GFSqr,
  cost split, both squared in place, GetMagnitude opt-ins (5). differ/00ac7eb0.cpp.
- 0x008A8260 GetHeroSummonedCreaturesList -- Ego_d locals applied; residue unchanged (private early-exit epilogue). differ/.
- 0x00B947E0 CEngineSceneMap::Update -- RELOCATION_MATCH + test PASS. New header engine/CEngineSceneMap.h from the Ego_d class record,
  CEngineScene accessor opt-in, GFFloatToLongNear helper, GFGetTopBit via the existing FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE opt-in,
  GFLimit, and `GFGetTopBit(new_size > 1 ? new_size : 1)` (a statement clamp splits new_size and the TopBit copy into two registers).
  Ego_r.exe disassembly (0x00A477E0) has the same shape (useful cross-check: ego_r.exe is VC7.1-like code).
- 0x00A62AE0 CPackedUIntArray::CalculateBias -- RELOCATION_MATCH + test PASS (vland strict). Ego_d local names; declaration order
  src_ptr, src_bytes, min_value, src_val, input_bit_mask is the only one of the 24 (after src_ptr) that homes src_val / c1 /
  input_bit_mask in the dead src_array / src_bits / count slots like retail (no `push ecx` frame). Header opt-in
  FABLE_RETAIL_CPACKEDUINTARRAY_CALCULATEBIAS (ReadNBytes inline + BitMasks). NAME_FIXES: BitMasks v landed g_PackedUIntMasks / PackedBitMasks.
- 0x00F361D0 round 2 (coordinator): right-assoc SCurve breaks siblings; scheduler measurement = region cut (81-tuple regions), retail
  has one fewer no-code PAREN tuple in R1 (sim: SCurve(tx)'s inner (2.0f*t) PAREN); include-order permutations no effect. ESCALATE file updated.
- 0x00F361D0 Get3DNoise -- RELOCATION_MATCH + test PASS (vland strict). Closed by the round-3 DAG read: R1 needs SCurve's PARENs after
  (3-2t) and on the whole product (sim enumeration of PAREN placements; uniform across fades), R2 needs one PAREN fewer per Interp.
  Header bodies `(t*t*(3.0f-2.0f*t))` / `(a+(b-a)*t)` match all three users. 0x00F36010 test helper spelling updated to match.
  Landed rows moved to landed/ (commit 039a0c8c02).
- 0x0063F4B0 ForEachInRoughArea<Bind2 nav-layer> -- PORTED onto shared headers (handQ23's 0x0063F7C0 Bind3 port re-instantiated;
  repo FOREACHINROUGHAREA_BODY). Residue: the loop-exit reload order shared with Q19/Q23 (0x008780D0 / 0x0063F7C0); include-order
  and class-definition-order permutations, plus a guarded y loop with hoisted byte offsets (the landed 0x00518130 recipe) as an
  overlay body: identical code. differ/0063f4b0.cpp.
- 0x00AC7EB0: FableWin (later revision) /Od shows `cost = 0; cost += GFSqr(tex)*W1; cost += GFSqr(normal)*W2` (GFSqr = ?GFSqr@@YIMM@Z,
  shared engine/GFWorldDistance.h); those spellings fold or keep the same n*W*n residue. Include permutations: no effect.
- 0x00C40590 (audio library, no PDB): pointer-local declaration orders and an inline member cross product make it worse; not pursued.
- 0x00BE2120 CEngineInternalPrimitive2DViewport::ClearChildPrimitive -- RELOCATION_MATCH + test PASS (vland strict). The hand2c oracle
  row was cut to 97 bytes (the group twin's length); oracle_override.tsv carries the full 106 bytes read from Fable.exe. Port = the
  landed CEngineInternalPrimitiveGroup::ClearChildPrimitive 0x00BC62D0 body over a new 2DViewport opt-in.
- 0x00A5B650 CCachedFile::Copy -- RELOCATION_MATCH + test PASS (vland strict) on first grade of the shared-header port: the prior
  "result temp slot" residue was the local model; the repo's STLport opt-ins (BACK_INSERTER + COPY_GENERIC_INLINE +
  ORIGINAL_COPY_DISPATCH_PASS22 + COPY_NOINLINE, precedent 0x007CDFE0) give the out-of-line __copy call and the tag slot.
- 0x0083DB60 CThingPhysicalSwitch::GetCurrentGraphic -- RELOCATION_MATCH + test PASS (vland strict), first grade. The PDB signature
  (IBEABVCEngineGraphic: protected, returns a reference) and Ego_d's 53-byte /Od body = `return PeekDefinition().XGraphic;` per arm with
  the repo's PEEKDEFINITION_SAMETU body; the draft's pointer return / explicit def pointer was the residue.

# hand3c (2026-11-19) -- successor on the 88 not-plain-C++ rows
## Matched (hand3c)
- 0x00B012E0 NParticleEngine::CPSCRenderMesh::CPSCRenderMesh -- RELOCATION_MATCH + test PASS (vland strict; test also PASS on g++
  FABLE_PORTABLE). Closed by FableWin /Od: the ctor calls ?GFPackFloat@@YIKMKMMK@Z (ecx = bits, edx = 2 decimals) per packed member,
  i.e. the shared engine/GFPackFloat.h, not a local PackParam; with the header's FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT (as the
  landed CPSCRenderMesh rows) the last register swap (SizeFadeMinimum store 0x00B016FD) goes. Strict headers: shared CPSCBase (new
  opt-in FABLE_RETAIL_CPSCBASE_CTOR), CRGBColour, CEnginePrimitiveHandle, CParticleEmitterManager (BULLETTIME_PASS23 + DEFAULTBLEND).
- 0x00B103F0 NParticleEngine::CPSCEmitterGeneric::CPSCEmitterGeneric -- RELOCATION_MATCH + test PASS (vland strict; g++ PASS). Same
  recipe as 0x00B012E0: FableWin /Od 0x02F47CE0 calls GFPackFloat per member; shared GFPackFloat.h + GROUP_PRODUCT closed the
  NonUniformScalingY register swap. Shared CPSCEmitterGeneric.h with new opt-ins CPSCEMITTERGENERIC_CTOR and
  CPARTICLEUPDATEPARAMDESC_CTOR (CPSCBase.h).
- 0x00B1DFD0 NParticleEngine::CPSCDecalRenderer::CPSCDecalRenderer -- RELOCATION_MATCH on the FIRST grade + test PASS (vland strict;
  g++ PASS). Authored from FableWin /Od 0x02F55400 (mem-init in declaration order, GFPackFloat per packed float, then
  alignment / blend mode / blend op in the body) on the shared CPSCDecalRenderer.h; new opt-ins CPSCDECALRENDERER_CTOR and
  CPARTICLEEMITTERMANAGER_DEFAULTALIGNMENT. Lesson: the parked "14 bare fistp" rows of the CPSC* family are GFPackFloat expansions.
- 0x00B05C60 NParticleEngine::CPSCSingleSprite::GetComponentCost -- RELOCATION_MATCH + test PASS (vland strict; g++ PASS). The parked
  `mov al,[esi+93h] ; test al,20h` residue: FableWin /Od shows `if (CrossedSprites) .. else if (FaceMe2D) 2 else 2` -- the second
  test folds away but leaves the byte loaded once (advisor's "second use" hint). Port onto shared CPSCSplineBase / CParticleSystem with
  GFUnpackFloat; the frame needed ScaleParticleDimensions(float&)'s body visible (header-inline COMDAT per PDB object placement, the
  macro-style nested max VC7.1 refuses to inline): new opt-in CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_FLOAT_INLINE. Render cost is
  `poly_count + pixel_count` (the donors' `* poly_cost` = 1 reorders the fadds): Ego_d's poly_cost is from the later revision.
- 0x00C6AC90 CSoundMixer::MixBlockRamped (descriptive; libpk no PDB) -- RELOCATION_MATCH + test PASS (vland strict), first grade. The
  "LTCG custom convention" is VC7.1's private convention for a same-TU static: retail's kernel copy 0x00C6AAE0 is this TU's static
  (5 byte-identical copies, one per thunk TU); the row source carries the landed kernel row's text (shared MixKernelsSIMD.h ramp
  macro) and so grades retail_asm (Retail-asm line cites 0x00C6AB43 pmulhw). Kernel name kept as landed `CUnprovenMixKernel_00c6aae0`
  (one address one name) -- COORDINATOR: that landed name carries the banned "Unproven" placeholder; respell both together if wanted.
- 0x00A9D480 CMatrix4x4::AlignedPreScale -- RELOCATION_MATCH + test PASS (vland strict; g++ FABLE_PORTABLE PASS). FableWin /Od calls
  ??RCMatrix4x4@@QAEAAMHH@Z for each fallback product through a `CMatrix4x4& m` local (Ego_d `m`): `m(r, c) *= scale.X` gives retail's
  operand order for all nine (the parked "x87 commutation lane gap"); SSE block with Ego_d's vect / dest / right, dest assigned first.
  Existing opt-in FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS; no new header.

### 0x00989230 UpdateObjectSpaceTextureTransform (hand3c, escalated)
- FableWin 0x030381B0 + Ego_d: c_transform / range / rows locals; `transform.AlignedSetMultiplyTransposed(WorldMatrix, TextureTransforms[c_transform])`
  (FableWin calls the Transposed forwarder: an inline layer), `rows = range.Count < 4 ? range.Count : 4`, flags as `|= 0x1000; &= ~0x800`.
  Source: differ/00989230.cpp.
- Residue: x87 term order of the inlined AlignedSetMultiply. tools/ord.sh prints the per-column order. Function-level rulers shift column 0 only; the
  c2-unrolled columns 1..3 are invariant to every ruler and every body spelling tried. Escalated at about 85 grades: keep/ESCALATE_00989230.md.

### 0x00A9D750 ApplyBoneMorphScaling (hand3c): RELOCATION_MATCH + PASS
- The last residue was one x87 operand swap at 0x00A9D927. It closed with `if (false) {} else for` on both c_bone loops, matching the
  FableWin 0x03382ED0 `xor ecx,ecx ; je` before each loop (the VC6 for-scope idiom).
- The AlignedPreScale / AlignedPreScaleTransposed bodies moved from the row into the overlay header
  hdr/engine/CMatrix4x4AlignedSSE.h under the opt-in FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES, with the nested _FORCEINLINE opt-in.
  The same text also gives RELOCATION_MATCH out of line for 0x00A9D480 and 0x00A9D580 (diagnostic probes in probe/*_shared.cpp).
- The row now uses the shared C3DAnimationManager.h (MESHTRANSFORMS opt-in) instead of a local class.

### 0x00C1C330 TransformPointThroughMirroredFrame (hand3c): RELOCATION_MATCH + PASS
- The second transform's x87 term order (y first) comes only from STEPWISE accumulation (`r = R1*y; r += ...`). VC7.1 keeps the step
  boundaries. A single expression is reassociated (z first) whatever its written order or parentheses, and dead-local rulers are inert.
- The remaining store-schedule residue (y copy after the z fstp) was solved with the c2 trace (tools/sched t20.py) plus sim.py. A DAG
  search showed that only removing the stack-copy pseudo (0x269) before the final fadd gives retail's tail. The closing
  `r = r + C*v + T` does exactly that, written uniformly for all three components.
- Types: shared engine/C3DVector.h with FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23 (the default copy ctor gives 196 bytes).

### 0x00BEE070 std::__pop_heap<pair<long,CCompositeSurfaceInfo>> (hand3c): RELOCATION_MATCH + PASS
- The explicit instantiation over the shared STLport heap body (FABLE_RETAIL_STLP_HEAP) and the shared CCompositeSurfaceInfo.h
  (PASS22_STATIC_MESH_RENDER) gives the match, like the landed twin 0x00BEEDA0. The draft's local CCountedPointer / pair stand-ins swapped esi and edi.

## Batch 6 (LANDED 2078f8b0cc)
- 00BEE070 std::__pop_heap<pair<long,CCompositeSurfaceInfo>*...>: RELOCATION_MATCH + PASS. Explicit instantiation over the shared STLP_HEAP
  (twin of the landed 00BEEDA0). vland WARN RELOC_NAME on __adjust_heap -> 00bedd70: same function, only the label text differs (demangled form).
- 00664BA0 CThingCreatureBase::GetCurrentMeshRadius: RELOCATION_MATCH + PASS after 3 grades. Shared headers; FableWin 0x01D999E0 order; Ego_d
  locals; GetSubDef<CCreatureDef> body visible but noinline (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE).
- Header bug: CCreatureDef.h with FABLE_RETAIL_CCREATUREDEF_TRANSFER but not FABLE_RETAIL_CDEFSTRINGTABLE makes VC7.1 cl spin forever (bisected).

## Batch 7 (LANDED 4347aa3597)
- 0052EE00 NUISystem::CComponent::GetTargetRenderPosition: MATCH, 2 grades. FableWin 0x0229BB60 shape on NUISystem_CComponent.h + FABLE_RETAIL_C2DVECTOR.
- 007F8C20 CTCAnimatingObject::AddQueuedAnimation: RELOCATION_MATCH, 1 grade. FableWin 0x02674140, sibling 0x007F8F30's opt-ins, NEW engine/CAnimatingObjectDef.h,
  CTCANIMATINGOBJECT_ADDQUEUEDANIMATION.
- 006B2960 CGameNavigatorManagerExternalInterface::GetNavigationInformation: RELOCATION_MATCH, 3 grades. Shared GFFloatToLongNear/Floor (retail_asm
  helpers), block-scoped CCharString("NULL") (Inferred), CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION (slot-1 respelling).
- 00B905A0 CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest: RELOCATION_MATCH, 1 grade. 0x00BB3E80's header set + row-defined inline helpers.

## Batch 8 (LANDED 7030ce2d2e)
- 00B94BF0 CEngineScene::GetMapAt: RELOCATION_MATCH, 2 grades. FableWin 0x02E1D7D0 shape; FABLE_RETAIL_STLP_MAP (visible subscript body, out of
  line) is what lets VC7.1 keep the floor results instead of reloading the escaped map_coords. Opt-ins CENGINESCENE_GETMAPAT and CENGINESCENEMAP_CTOR.
- 00914650 CAIStateGroup_BirdStartled::IsRunnable: RELOCATION_MATCH, 2 grades. FableWin 0x02B8D570 shape; BESTROUGH_BODY drops the
  `pcreature = 0` store. NEW BirdStartled / ScaresBirds headers, DEADCREATURE_STATGETTYPE. The original measures the bird against itself.
- Lesson: an out-param or escaped-address residue (an extra zero store, a reloaded local, a frame home) closes when the same-TU callee's body is visible but kept out of line.

## Batch 9 (LANDED 8a0b6a1f3d)
- 0082A020 CMiniMapDisplay::GetDrawPosition: RELOCATION_MATCH, 40 grades. FableWin 0x027D3990 shape (Ego_d locals). Three levers:
  (1) GetRegionMapExtents kept out of line (CREGIONMINIMAP_EXTENTS_NOINLINE; retail calls 0x0066C6A0); (2) the visible noinline
  GetRelativeMapSize body (RELATIVEMAPSIZE_BODY, FableWin 0x027D3850) fixed renderPos.Y's frame home; (3) five natural brackets moved
  the c2 83-node region cut to between the two final filds (`fild fild mov pop fstp`). 256.0f is a float (retail 0x0125B240; FableWin's
  double gives a qword fdivr). player_gui_def is fetched and unused, as in FableWin. GDummyInt1/2 are the FableWin public names of
  0x013B83B8/0x013B83BC (pending, unlanded drafts call them g_ScreenOriginX / g_MiniMapScreenOriginX; no landed user, no NAME_FIXES row).
- Coordinator: the landed 0x00829C70 GetRelativeMapSize row uses a local model; the RELATIVEMAPSIZE_BODY text is its FableWin body and
  could be its source if it is regraded from the header.

## Batch 10 (LANDED de5ba369a2)
- 00B93B30 CEngineSceneMap::AddPrimitive: RELOCATION_MATCH on the first grade. FableWin 0x02E1C860 statement for statement with the
  inline helpers made real: ConvertToLevel (its temporary copied back is the parked "3-byte temporary at [ebp+0Dh]"), SetGridPos,
  GetGridLevel, CEngineSceneGrid::AddPrimitive -> GetCellCreate()->AddPrimitive. level_size unsigned. Opt-ins CENGINESCENEGRIDPOS_INLINE,
  CENGINEINTERNALPRIMITIVEBASE_SETGRIDPOS, CENGINESCENEMAP_ADDPRIMITIVE. Lesson: the parked frame-slot residue was the missing helper calls.
- 0064D404 NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo: RELOCATION_MATCH. FableWin 0x020E86BF shape on the shared CDrawQuestInfo.h (the
  parked "fistp lane gap" label was wrong: no asm in retail; plain C++). The last residue (height in [ebp-4] vs retail's st(0) across
  the GetElementByIndex call) closed with GetElementByIndex's real body as an ordinary definition earlier in the TU under
  #pragma auto_inline(off): an in-class noinline COMDAT body does NOT give c2 the register/FP summary (measured on a 10-line probe:
  COMDAT -> spill, earlier strong definition -> st(0) kept). Only optimize("s", on): with optimize("y", off) the helper grades
  DIFFER(35v32) (frame pointer) at 0x00644FC5; without it, MATCH. NAME_FIXES: 0x00437870 is CPlayerGui::IsMiniMapDrawing.
- 00829C70 / 00829CE0 re-ports (coordinator-approved for 00829C70; 00829CE0 found by the helper self-check): the header bodies graded at
  their own addresses (both RELOCATION_MATCH), rows are the header text emitted through anonymous-namespace scaffolding (0x00B9C110
  pattern). vland --recheck-landed: both PASS.
- Helper self-check (coordinator lesson): 00B93B30's inline helpers have no out-of-line retail copies (no labels); 0064D404's row-defined
  GetElementByIndex MATCHes 0x00644FC5.

## Batch 11 (READY)
- 006CC800 CTCCreatureNavigation::FrameUpdate: RELOCATION_MATCH. FableWin 0x02479E80 statement shape on shared headers (CTCCreatureNavigation,
  CThingCreatureBase, CThingSearchTools CONSTPROC, CThingFilter_IsNot, the anonymous FrameUpdateThing the landed 0x006CE800 declares),
  GFFloatToLongCeil (Retail-asm), scale 3.0f. vland found the stagger bound __aullrem: retail calls _allrem, so the unique ID is cast to
  __int64 (Inferred placement). Test value chosen exact under x87 extended precision (0.6f gave 55 under VC7.1, 54 under g++).
- 009C38C0 CFileInstaller::CActiveFile::OnWriteFinished: RELOCATION_MATCH. Plain C++ (the parked "LTCG esi convention" is VC7.1's own
  convention for a noinline static: declared __fastcall it gets the esi result and `ret 4`; a plain static is caller-cleaned).
  The last residue (eax/ecx/edx rotated by one from 0x009C392B) was the inline SetDirtyDiscError text: FableWin's
  `Get().ExitThread = Get().ExitThread || on;` matches; the landed `if (...) error = true;` text does not here. FableWin's text also
  matches 0x009C3530 and 0x009C4440 (measured). GetMarkerFileName grades RELOCATION_MATCH at 0x009C33A0 from this source.
- 00644FC5 re-port (coordinator-approved): MATCH, pass pattern `00644fc5_TEST PASS`.

- 00B53F60 CEngineWeatherRenderer::FrameStart (worked, deferred): retail keeps `this` in ecx across UpdateBoxPositions 0x00B52990
  because the whole same-TU chain never writes ecx: UpdateBoxPositions tail-calls BuildBoxPositionMatrix 0x00B51AD0 (Ego_r private;
  ~970 bytes, unlanded, not in the catalog oracle), and that body only reads ecx (checked: every ecx use is `[ecx+..]`). A probe with
  UpdateBoxPositions defined earlier (auto_inline off) but BuildBoxPositionMatrix only declared still homes `this` in esi (418v411).
  Closing it needs both bodies defined earlier in the row's TU (ordinary definitions), i.e. reconstructing 0x00B51AD0 first.

- 00A4D770 C3DAnimation2::CalcLocalTransformationsRelative (worked, OPEN, ~42 grades): DIFFER(420v420), one instruction:
  retail 0x00A4D8F1 `lea edi,[edx+ebx]` (dest + iv) v ours `[ebx+edx]`. Source: FableWin 0x03227CA0 shape (ConvertTimeToFrames;
  loop: C3DKeyframe kf_rel; GetKeyframeInterpolateFixPos(c_seq, kf_rel, frame, fraction); dest[c_seq] = *mesh.GetBoneKeyframe(c_seq)),
  the landed 0x00A4D920 helper texts verbatim, FixPos FABLE_FORCEINLINE (Ego_d params bone_index/dest/frame/fraction).
  Lever found: ConvertTimeToFrames' body must be visible (sibling `inline` text) or VC7.1 copies `fraction` per iteration (438v420).
  Tried without effect on the lea: 5 copy spellings (index, pointer add, reversed index, ref local, memcpy), 4 loop spellings, inline
  depth (no FixPos / sequence ref local), SetPosition + C3DKeyframe CTOR opt-ins, CalcLocalTransformations defined ahead (TU
  context), body after the row, /Oa /GS /EHsc, the QFE-4035 toolset (same), mesh-before-animation include order. Ego_r 0x00BA2EB0
  (other build, double constants) has OUR order. Diagnostic: with the ConvertTimeToFrames call replaced by plain stores the order
  becomes base-first (with c_seq/iv registers swapped), so the call / address-taken frame+fraction drive it. Overlays kept:
  C3DAnimation2.h FABLE_RETAIL_C3DANIMATION2_CALCLOCALRELATIVE, C3DMesh2.h FABLE_RETAIL_C3DMESH2_GETBONEKEYFRAME.

## Batch 12 (in progress)
- 0066CD50 CRegionMinimap::BuildRegionMinimapTGA: RELOCATION_MATCH, vland strict PASS (`0066cd50_TEST PASS`). FableWin 0x021886B0
  statement shape on the shared CRegionMinimap.h (new opt-in BUILDTGA types the surface and the two vectors), Ego_d.pdb locals.
  Retail's two size copies are FableWin's two GetRegionMinimapSize() reads (tga_width, tga_height). The parked "loop frame" residue
  was the old local-struct model. Last residue (a register rotation eax/edx/ecx from 0x0066CD98, unmoved by 5 row respellings) closed
  with the inline getters' own text: FableWin / Ego_d `long idx = size * pos.Y + pos.X; return RegionMinimapCellList[idx].eTheme;`
  (the named local, not the folded subscript). Interpolate is FableWin's text with retail's float 256.0f.
  NAME_FIXES: 0x0066DC50 is CRegionMinimap::CreateRegionMinimapTGAFile (FableWin 0x02188B80 body = the landed call sequence).
- 00B41A80 CEngineMap::PeekWaterHeight: RELOCATION_MATCH first grade, vland strict PASS (`00b41a80_TEST PASS`). FableWin 0x02D5DD80 is
  two statements (`depth = PeekWaterDepth(x, y); return depth > 0.001f ? PeekLandscapeHeight(x, y) + depth : 0.0f;`); the parked
  draft's hand-expanded neighbour lookup is the header's PEEK_BODIES PeekLandscapeHeight. Needs the TU's `/d2inlT576` (as the landed
  0x00B40BD0; without it PeekLandscapeHeight stays a call, DIFFER(68v424)). New opt-in CEngineMap.h PEEKWATERHEIGHT (declaration).
- 00699E60 CGameCameraManager::SetScriptedCameraMode: RELOCATION_MATCH, vland strict PASS (`00699e60_TEST PASS`). FableWin 0x021F9950
  statement shape on the FULL CGameCameraManager.h (new opt-in SCRIPTEDCONTROL) with FableWin's private SwitchIntoScriptedControl
  (0x022002F0) as a plain inline body (Ego_r 0x006C1080 expands it too); new header NCameraModes_CScriptedControl.h. Two devices:
  CCOUNTEDPOINTER_RESET_NOINLINE (existing NF62 opt-in: retail calls Reset 0x0069C2F0 but keeps the local's Info in edi across
  SetCameraMode; graded RELOCATION_MATCH at 0x0069C2F0 from this source) and `// Flags: /d2inlT150` with an `// Inline-budget:`
  line (default budget calls SwitchIntoScriptedControl, DIFFER(290v431); smallest N 150, 149 fails; EXTRA_RULES 2026-11-08
  preference 2; a FABLE_FORCEINLINE on it also matches but is not evidenced). The parked "dead argument slot" residue was the
  missing visible Reset body. NAME_FIXES: 0x00697C60 is CGameCameraManager::UpdateCleanupFlags (catalog: CMapInfo_00697c60::Clear).

## Batch 13 (READY): SetDirtyDiscError unification + GetMarkerFileName (coordinator decision on b12)
- One text everywhere: FableWin.pdb 0x0304C9B0 `Get().ExitThread = Get().ExitThread || on;` with Ego_d names `on` / `cs`.
  Respelled: 009C3530 (also moved off its local class onto engine/CFileInstaller.h + CCriticalSectionLock.h, which renames its
  static to the PDB `DirtyDisc`; on the local class with plain `bool ExitThread` the new text is DIFFER(79v72): the volatile member
  of the shared header is what makes it match), 009C37A0, 009C3820, 009C4440 (/d2inlT120 unchanged), 009C38C0 (names only),
  009C3590 / 009C3660 (names only, local classes). All seven RELOCATION_MATCH + PASS under vland --recheck-landed, except that strict
  headers reports HEADER_POLICY for 009C3590 / 009C3660 (their landed local CAFile / CTBaseSingleton / CActiveFile models: pre-existing,
  not caused by this change; non-strict q.py RELOCATION_MATCH). Porting those two onto the shared header needs the FableWin shapes
  (GetMinTransferSize inline, Ego_d locals active_file / pathname / flags / file_size ...): a separate job.
- 009C33A0 GetMarkerFileName, new row: RELOCATION_MATCH + `009c33a0_TEST PASS`. Extent: 315 bytes, int3 padding before (after the
  `ret 4` of 0x009C3380) and after `ret 4` at 0x009C34D8, next function 0x009C34E0; single caller 0x009C39C2 (OnWriteFinished).
  Row TU is the OnWriteFinished TU (file static beside its caller, word-wrap statics precedent).
- NAME_FIXES: 013CA814 = ?DirtyDisc@CFileInstaller@@1_NA (Ego_r + FableWin publics); landed Pause 0x009C31E0 spells it
  g_installerNoPauseWait.
- b13 LANDED by the coordinator (8 rows + Pause 0x009C31E0 respelled to CFileInstaller::DirtyDisc; stale DirtyDiscError binding dropped).
- QUEUE (END, after the not-plain-C++ rows): port 0x009C3590 StartReadingFile / 0x009C3660 StartWritingFile onto the shared
  engine/CFileInstaller.h (FableWin 0x030495F0 / 0x030499D0 shapes; Ego_d active_file, pathname, flags, high_file_size, file_size /
  output_size, distance_to_move); hand back as a normal READY batch (coordinator approved 2026-11-19).

## Batch 14 (in progress)
- 007727E0 CTCShop::GetCurrentBasicPrice: RELOCATION_MATCH + `007727e0_TEST PASS` (strict vland), 26 grades. FableWin 0x0261FE90
  statement shape (reduction = ShopDef->PriceReductionFractionPerItem * (GFFloatToLongCeil(Quantity) + add): FableWin evaluates
  ShopDef-> first). Closing lever: the map<long, CShopItem> const find body visible but out of line (existing opt-in
  FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE; the landed 0x00777170 spelling, and this TU's copy grades MATCH at 0x00777170).
  Seeing that find only reads the key, VC7.1 copies `def` into ebx and reuses [ebp+8] for the ceil result / reduction, as retail.
  GETDEF_GLOBAL_BODY + GETSUBDEF_BODY are also needed (455 / 475 without). This TU's copies of those templates DIFFER at the
  retail COMDATs: 0x0042AF3C and 0x0059350C come from /Os objects (their landed rows carry `#pragma optimize("s", on)`), and
  0x005130A0 (an /O2 object) calls the protected non-const GetPDefFromGlobalIndex 0x009ADA10, while the shared GETSUBDEF_BODY
  calls the public const overload 0x009AD9E0. So the shared body is not retail's GetSubDef text. That is pre-existing, and 121
  landed rows use the opt-in: reported, not changed. No header change.
- 006B75A0 CGameMap::Resize: RELOCATION_MATCH + `006b75a0_TEST PASS` (strict vland), 12 grades. FableWin 0x024A0160 statement shape
  with Ego_d names. Levers: STLP_VECTOR_C (operator= body seen, called out of line; the default is the implicit member-wise copy)
  and /Gr (the game-map TU's free template COMDATs are __fastcall; the row matches without it, but the same-TU operator= 0x006B7E20
  needs it). New opt-in FABLE_RETAIL_CGAMEMAP_RESIZE in engine/CGameMap.h: Resize declaration, CGameMapCell::Clear declaration
  (0x006B7490, FableWin's mem_fun_ref target), and the cell's data members private as in FableWin LF_FIELDLIST 0x533C9. The class
  is then no aggregate, so vector(size_type)'s `_Tp()` writes nothing, as at 0x006B7D60. Same-TU COMDATs graded from the row source:
  0x006B7E20, 0x006B7D60, 0x006B7CC0, 0x006B7C10, 0x006B7B70 and 0x006B79A0 all MATCH / RELOCATION_MATCH. NAME_FIXES: 0x006B7490
  is CGameMapCell::Clear, not "Reset".
- 007DF770 CTCWallMountEffects::FrameUpdate (catalog vfunc_9): RELOCATION_MATCH + `007df770_TEST PASS` (strict vland), 7 grades.
  This rewrites the parked local-model draft on shared headers, following FableWin 0x02766430 statement for statement. New header
  engine/CTCWallMountEffects.h (Ego_r layout, FableWin enum EWallMountEffectsConfirmationType). New opt-in
  FABLE_RETAIL_CCREATUREACTION_USEWALLMOUNT_CLASS in the generated engine/CCreatureAction_UseWallMount.h: the polymorphic class on
  CCreatureActionBase (ctor (CThingCreatureBase&, CThing&) 0x00843610, virtual dtor, Initialise / PerformAction / Clone).
  Existing opt-ins that closed it: CINTELLIGENTPOINTER_ASSIGN (operator= = GetPItem + SetPItem; without it the implicit member-wise
  copy is 468v437) and IS_THING_ALIVE_NOINLINE (retail calls 0x006961F0; the emitted copy grades RELOCATION_MATCH there). The
  vland RELOC_NAME warning: retail 0x007DF380 is an ICF-shared destructor body (catalog _Destval<...CRegisteredMusicEntry>,
  labelled Dtor_CCreatureActionBaseBase); this row binds ??1CCreatureAction_UseWallMount@@UAE@XZ there.

## Batch 15 (in progress)
- 00780D80 CTCFlammable::FrameUpdate: RELOCATION_MATCH + `00780d80_TEST PASS` (strict vland), 31 grades. Rewrote the parked local-model
  draft (it had a fake same-TU GetSubDef body) on shared headers, FableWin 0x01F51180 shape. Ego_d names. New headers:
  engine/NFire_CFirePoint.h (Ego_r layout, inline IsOnFire / IsBurning from FableWin) and engine/CFlammableDef.h (Ego_r members
  +0x28..+0x34). New opt-in FABLE_RETAIL_CTCFLAMMABLE_FRAMEUPDATE in engine/CTCFlammable.h: the rest of the Ego_r members, typed
  PDef, inline IsOnFire, FrameUpdate and ExtinguishFire. The prior draft's one residue (where the conversion temporary is homed) was
  the inline budget: retail calls CDefString::operator CCharString 0x00415DD0 out of line with its body seen. /d2inlT59, the
  smallest of the matching window 59..66, with an Inline-budget line. The emitted conversion copy grades RELOCATION_MATCH at
  0x00415DD0. GetSubDef<CEntitySoundDef> 0x0065F604 DIFFERs from the shared GETSUBDEF_BODY, the same pre-existing gap as b14.
  Slot 59 is CThingPhysical::ModifyHealth (retail vtable 0x01244F7C), via CTHING_ORIGINAL_VTABLE.

## Queue note (coordinator, after b14 landed)
- END of queue ports onto engine/CFileInstaller.h now: 0x009C3590, 0x009C3660 AND 0x009C31E0 Pause (its local model emits
  ?DirtyDisc@CFileInstaller@@2_NA; PDB/shared header have @@1_NA). Remove the 2_NA binding in the hand-back.
- Header overlays: base on the current repo copy; new opt-in blocks must not move existing FABLE_STATIC_ASSERT lines (or pin with _AT).

### 00439bb0 CPlayerGui::UpdateMoney -- RELOCATION_MATCH + PASS (batch 15, 4 grades)
- Rewritten from the prior local-model draft (parked DIFFER, frame slots +4, tail order) onto shared headers: CPlayerGui.h
  (CPLAYERGUI + CTOR + MEMBERS + PEEKGUIDEF + new UPDATEMONEY), CPlayerGuiDef.h CLASS (MoneyIncrease/DecreaseTextColour +0x4C4/+0x4C8,
  SecondsBeforeStatsBackBarMoves / SecondsToShowStatsBars +0x4E8/+0x4EC), NPlayerGui_CStatBar.h (new CHANGETEXTS), CTCHeroStats BRIBE +
  GETMONEY, CThing GETTC_EXPANDED, IS_THING_ALIVE (inline), CTCSoundPlayer, CPlayer (PLAYER).
- Statement shape: FableWin 0x01A814F0, Ego_d local names. Closing lever: the uninitialised branch as three member-order stores
  (retail +0xC/+0x10/+0x14, each re-reading PMoney); the chained assignment stores +0x14 first.
- GFLimitLower defined in the row (FableWin ??$GFLimitLower@JH@@YIXAAJABH@Z; same body as CAnimSet.h's). Retail-asm line cites the
  GFFloatToLongNear fistp at 0x00439C3D/41 (vland NOT_GENUINE without it).
- Leaf ?UpdateMoney@CPlayerGui@@AAEXXZ (private; PDB) -- matches the 9 existing references.

- b15 LANDED (coordinator): 00780d80, 00439bb0; DELTA_NOTIONAL_ZERO NAME_FIXES added by coordinator; 0x00415DD0 warning accepted.

### 00621a20 CTCInventoryQuests::InitialiseQuestCardList -- RELOCATION_MATCH + PASS (batch 16)
- Rewritten from the parked local-model draft (600v600, register colouring) on shared headers, FableWin 0x01F7D6C0 shape with Ego_d
  names: ResetCategoryList (inline Categories.clear()), IsInventoryScreen (CBase slot 3) -> CQuestManager active cards; else
  GetIsEnabled (slot 75) -> GetBuildingThing -> GetTC(TCI_HERO_CENTRE) -> GetQuestCards; QuestCardCount; PeekTC<const CTCQuestCard>
  -> GetPDef()->InventoryCategory -> AddCategory; GetCategoryWithIndex(0)->SetAsSelected(true).
- Closing levers: FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL (same TU as 0x0061AD60) and FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
  (not INLINE_LOWERBOUND: GetTC's non-const LowerBound stays a call). First grade of the shared-header text was already 558v600 with
  the structure right.
- Gotcha: forgetting FABLE_RETAIL_CDEFSTRINGTABLE made CDefString.h hit its #error and cl.exe spin for 13+ min under Wine (killed my
  own processes). Added tools/zs.py (front-end /Zs with a 150 s watchdog) to catch these fast.
- Helper __copy<CCategory*> RELOCATION_MATCHes at 0x00625820. 0x005C2A8E (27 bytes, ~CCategory out of line) is another TU's COMDAT
  copy; ours (dtor expanded) is discarded. Its catalog label (_Destroy_range<pair<unsigned long,CAnimationEntry>> /
  ForEachElem0x2c...) and 0x005C2739's (~CCreatureAction_FireMissileWeapon) look wrong for a CCategory range destroy -- not proven
  with two sources, so no NAME_FIXES row.
- New opt-ins (overlays on today's repo copies): CTCInventoryQuests.h RESETCATEGORYLIST, NInventory_CTCInventoryBase.h ADDCATEGORY,
  CCategory.h SETASSELECTED.

### 00d8a240 CQ_HerosOldHouseScript::WatchForBooty -- OPEN (29 grades), site-map residue
- Rewritten on shared headers (CLevelScriptBase / CGameScriptInterfaceBase / CScriptThing / CCountedPointer; new opt-in
  FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_WATCHFORBOOTY in hdr/engine/CQ_HerosOldHouseScript.h with the three unpinned asserts pinned at
  their old lines 74/75/77). src/00d8a240.cpp = FableWin 0x037BA020 form (for(;;NSF), all returns via TRetVoid().Return()).
- Method (use for 00d6c830 too): judge by WHICH returns expand ~CScriptThing, not by size. Retail: S1 loop-top T (+bottom) expanded,
  S2 `!loaded` T expanded (own block 0x00D8A3B0), S3 inner-wait T / S4 after-wait T / S5 dig T -> one 0x004AA840 call block, S6 final
  -> call.
- Results (variants in keep/variants_00d8a240/):
  FableWin form default 662: S1,S3,S6 expanded (S2 cross-jumps into S6's copy, S5 into S1's), S4 call.
  + CCOUNTEDPOINTER_RELEASEDATA: same. Without TRetVoid: same. /d2inlT10..100 non-monotonic (487..662), never the retail map;
  at 45/46 the deep sites get ~CScriptThing expanded but ~CCountedPointer called (a partial inline retail never shows).
  Prior do/while form (top/!loaded `break`) 610 (b): end-of-function expansion + S3 expanded, S2 merged into exit.
  (d) b with S2 `return`: 610, S2 call, S3 expanded. (g) b with S1 `return`, S2 break: 610, S1 call, S2 (end) + S3 expanded.
- Pattern: VC7.1 here always expands the natural scope-end copy and then the innermost-loop return (S3) first; retail never
  expands S3. Unresolved lane gap; keep OPEN, revisit with a fresh idea (e.g. inner wait as a separate scope/helper) or escalate
  at ~100 grades.

### 00a770b0 CNavQuadTree::GetNodeAt -- RELOCATION_MATCH + PASS (batch 17)
- The prior parked text (527v533: area box / BlockedNode store order) rewritten to the FableWin 0x03293760 statements with its local
  names: box = C2DBoxF(MapPos, C2DExtentsF(MapWidth, MapHeight)) (C2DBOXF_EXTENTS) + ContainsPoint (C2DBOXF_CONTAINSPOINT_INLINE),
  map_space_pos, x / y / index via GFFloatToLongFloor, node = NavigationLayers[nav_layer].Nodes[index], blocked_node =
  GetBlockedNode() (new opt-in), Position.Initialise(...), NavLayer then QuadTreeLevel. First grade 533v533 with only the 16.0
  constant differing: FableWin's double 0.5 -> `fadd qword`; retail adds a float 16.0f -> written `MAX_NODE_SIZE * 0.5f`.
- ONE_OVER_MAX_NODE_SIZE is the PDB name (FableWin / Ego_r S_LDATA32, guarded in this function). MAX_NODE_SIZE (32.0f) is inferred
  from it (no symbol in either PDB) -- marked descriptive in the row.
- Retail-asm line now cites the three fistp addresses (0x00A771A6 / 0x00A771DD / 0x00A77214); the prior cited the filds.

### 00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable -- OPEN (14 grades), 3-instruction scheduling residue
- Source `src/00986210.cpp` (= keep/variants_00986210/v4_evidence.cpp): FableWin 0x02C76EB0 shape -- `if (type != HUMAN_CHILD) { ... return
  ret_pcreature != NULL; } return false;` (FableWin `je` to the shared `xor al,al`), ptc_perception then ret_pcreature (Ego_d order),
  magnitude of `GetCreature().GetPos() - ThingReactingTo->GetPos()` (FableWin: reacting GetPos is the ??G argument, creature the this).
  Evidence-based forms (byte-inert here): C3DVECTOR_INITIALISE_SUB (FableWin ??G 0x019072B0 is 34i), C2DBOXI_RADIUS_INITIALIZERS
  (FableWin ctor 0x01ADDBB0). New nested opt-in FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_CALLINGCHILDREN (hdr overlay,
  declaration only, after HeroImitationPlayUpdate; no assert moves).
- Frame lever found: without the 0x009871C0 template body in the TU the frame is 0x68 v 0x5c (CFilter_Bind4 temp allocated last
  instead of over the dead difference vector; ret_pcreature in the top slot). With the helper row's text (and its define set) visible
  ahead of the function, every slot matches retail (frame 0x5c, Bind4 at -0x2c over the vector, ret -0xc, ptc -8, ceil -4).
- Remaining: 0x00986374..0x0098637A, the In2DRange #1 copy -- retail loads the spilled sight radius (`mov ecx,[ebp-0x1c]`) between
  the Y load and the Y store; ours loads it after `mov eax,[eax+8]`. Inert so far: declaration order, ret scope, operator- form,
  box ctor form, ORIGINAL_COPY_FORMAL copy ctor. Next levers to try: same-TU sibling bodies in retail .obj order (the Bind3
  GetPFirstThing 0x00986E30 / GetPBestThing 0x00986D90 instances), the In2DRange filter construction order.

### 00b328c0 CEnginePrimitiveRenderer::LoadNewScene -- RELOCATION_MATCH + PASS (batch 18, 4 grades)
- FableWin 0x02DA8F90 shape with Ego_d locals on the shared headers: CEnginePrimitiveRenderer (+ MEMBERS), SharedData RENDERSETTINGS,
  CEngineCamera POSITION, CEngineShadowRenderer MEMBERS + PEEKSCENEDATA, CShadowRendererSceneData COPY, CEngineLightingManager FRAMESTART,
  CEngine PEEKVIDEOOPTIONSDEF, CEngineVideoOptionsDef PASS22_FIELDS, GFGetInterpolatedValue, GFFloatToLongFloor (Retail-asm line).
- Closing lever: FABLE_RETAIL_CARRAY_INDEX_ASSERT (FableWin's CArray<CEngineScene*>::operator[] 0x02CF4750 asserts index < size);
  without it the element load is hoisted above the pushed arguments (515v517).
- New opt-ins (hdr/, b18): CENGINEPRIMITIVERENDERER_LOADNEWSCENE (the override declaration), CENGINESCENE_PRELOADPRIMITIVES (three
  overload declarations 0x00B92F80 / 0x00B92E50 / 0x00B930A0). Stand-ins in the row (no header exists): CEngineComponentParams,
  CEngineComponentLoadNewSceneParams, CEngineVideoOptions (Ego_r member names; sizes asserted).
- NAME_FIXES: 0x01436EB0 = ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B (four wrong spellings in reloc_bindings);
  0x01436E60 label respell to ?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A (vland RELOC_NAME warning).

### 00a38c20 CCRCSymbolMap::LoadBinary -- RELOCATION_MATCH + PASS (batch 19, 1 grade)
- FableWin 0x031DB670 statement for statement with Ego_d locals on shared headers: CDiskFileWin32 (inline dtor), CArray<unsigned char>(n)
  (FABLE_RETAIL_CARRAY vector(n) form) + GETBYTESIZE, CMemoryDataInputStream / CDataInputStream inline ReadULONG / ReadSLONG,
  CVectorMap : CArray<pair> (resize 0x00A39AE0 landed). Element store `SLONGMap[c1] = std::pair<long,long>(var1, var2)` (FableWin
  converts a pair<long,long> into the map's pair<unsigned long,long>). The prior's "constant 0 / 4 register caching lane gap" came
  from the local model; the shared inline stream read gives retail's esi / ebp caching as is. Stand-in CCRCSymbolMap (no header).

### 00b1d550 CPSCDecalRenderer::DecalCollides -- RELOCATION_MATCH + PASS (batch 20, 2 grades)
- FableWin 0x02F567F0 statement for statement with Ego_d locals; the landed PreparePrimitives 0x00B1F200 define set, GFPackFloat
  (GFUnpackFloat x2, Retail-asm line for the fistp at 0x00B1D5A9 / 0x00B1D609), GFGetInterpolatedValue, std::min, GFSqr
  (C3DVECTOR_NORMALISE_SQUARE_HELPER). Leaf is the obj's STLport decoration (`ABU?$_List_iterator@...U?$_Nonconst_traits...`).
- New opt-ins: CDECAL_ISACTIVE_MAXLIFE (FableWin's IsActive 0x02F56430 is `MaxLife > -1 && Active`; the header's PREPARE body is
  `Active` only and the landed PreparePrimitives spells the MaxLife test by hand), CDECAL_GETSIZEFACTOR_NOARG (FableWin
  ?GetSizeFactor@CDecal@NParticleEngine@@QBEMXZ = SizeFactor[0]), CPSCDECALRENDERER_DECALCOLLIDES (protected declaration).

### 00b1d790 CPSCDecalRenderer::PoolDecal -- RELOCATION_MATCH + PASS (batch 21, 6 grades)
- FableWin 0x02F56D80 statement for statement with Ego_d locals; CDecal accessors from FableWin bodies (new opt-in CDECAL_POOLING:
  GetGraphicFrame / SetGraphicFrame / GetAlphaFactor() / IncrementSizeFactor / IncrementAlphaFactor / Deactivate; SetMaxLife via the
  existing CDECAL_UPDATE), CPSCDECALRENDERER_POOLDECAL declaration. Both pooling tests of life and frame read PoolIncreaseLifeEnable
  (retail and FableWin agree).
- Closing lever: the DecalCollides 0x00B1D550 body in the TU ahead of the row (same .obj in retail). Without it: frame 0x1C v 0x14
  (param-slot reuse differs) and the IncrementSizeFactor divide is hoisted (880v876). Declaration order / const / if-else spellings
  of the clamp were inert. Same lesson as 00986210 (helper template body visible changed slot packing).

### 00b1dc70 CPSCDecalRenderer::AddToBoundingSphere -- RELOCATION_MATCH + PASS (batch 22, 1 grade)
- FableWin 0x02F55EB0 statement for statement with Ego_d locals (trail_pos2 kept: declared, unused, as Ego_d records). PreparePrimitives
  idioms: row-local inline PeekGraphic / GetWidth / GetHeight, ScaleParticleDimensions body noinline (the folded 0x00AF8F00; q.py MATCH
  at its address from this TU), GetTickRate / GetGraphicBank local declarations. DELTA_NOTIONAL_ZERO extern (vland RELOC_NAME warns on
  the old label g_NormaliseEpsilon; NAME_FIXES row already filed by the coordinator in b15).
- Inferred line: factor = 1.0f only when (Flags & 0x40) && (Flags & 0) (FableWin literal masks; folds to 0.5f).
- Test note: vland compiles tests with VC7.1 -- `fmod(float, double)` is ambiguous there (TCC_FAIL); cast to double.

### 00c40590 CCodaListener::Commit -- RELOCATION_MATCH + PASS (batch 23, 87 grades total, closed in 1 once identified)
- Identity: the 0x00C3xxxx-0x00C4xxxx "audio library" is Sensaura GameCODA 2.10 (SDK fetched at $S/sdk/gamecoda/tree/CODA, BUILDING.md).
  Searching CODAApi.lib for the row's `test [esi+48h],20000h` / `and eax,38000h` bytes finds member .\Release\CodaListener.obj,
  ?Commit@CCodaListener@@QAEXXZ, 192 bytes; tools/codaobj.py prints its relocations (m_ffCommit, Normalize x2, the two float pools,
  OrthogonalRight). The Dbg object gives the statement shape (assert m_vFront != m_vTop, Dot, the +-0.05 test, both arms call
  OrthogonalRight: m_vTop.OrthogonalRight(m_vRight, m_vFront) / m_vRight.OrthogonalRight(m_vFront, m_vTop)).
- Closing lever: the SDK Toolkit twin class CCtkVector (Toolkit/include/CtkVector.h) has the inline OrthogonalRight / Dot bodies; with
  them as header inlines VC7.1 expands the first call and calls the COMDAT copy at the second at the default budget (no flags), and
  the six x87 products load in retail's order. 80 grades of operand-order sweeps on a made-up model (spelling permutations, helper
  roles, pointer locals, dummy-parameter symbol shifts) never closed it: VC7.1's commutative order there tracked the inline function's
  parameter structure, not spelling.
- Other rows in this range: check CODAApi.lib FIRST (`tools/codaobj.py <lib> <symbol>`, or a byte search) -- names, statement
  shape and header-inline bodies come straight from the SDK.
- NAME_FIXES: 00c40590 Commit, 00c402e0 ?Normalize@CCodaVector@@QAEMXZ (landed as CPKVector3_00c402e0::Normalise), 00c40370
  ?OrthogonalRight@CCodaVector@@QAEXABV1@0@Z (landed as C3DVector::SetCrossProduct), 0143bc24 ?m_ffCommit@CCodaListener@@0IA.

## Wrap-up state (owner order 2026-11-19): touched rows that did NOT land
Each row: best grade | best source | what was tried | residue | best next idea.
- 00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable | DIFFER (461v461, 14 grades) | src/00986210.cpp (= keep/variants_00986210/
  v4_evidence.cpp; v1..v3 alongside) | FableWin 0x02C76EB0 shape on shared headers, the 0x009871C0 template body in the TU (fixed the frame),
  declaration-order / ret-scope / operator- / box-ctor / copy-ctor spellings | 0x00986374..0x0098637A: retail loads the spilled sight radius
  `mov ecx,[ebp-0x1c]` between the Y load and the Y store of the first In2DRange copy | next: same-TU sibling bodies in retail .obj order
  (Bind3 GetPFirstThing 0x00986E30 / GetPBestThing 0x00986D90), In2DRange filter construction order, a bracket (OWNER RULE 2026-11-19).
  Overlay: hdr/engine/CAIStateGroup_OpinionReactionRoot.h (CALLINGCHILDREN).
- 00d8a240 CQ_HerosOldHouseScript::WatchForBooty | DIFFER (610v610 best do/while form; FableWin form 662v610; 29 grades) | src/00d8a240.cpp
  (FableWin form), variants in keep/variants_00d8a240/ | site map of the ~CScriptThing expansions (see section above) | retail expands at
  the loop-top and `!loaded` returns, calls 0x004AA840 at inner-wait / after-wait / dig returns; VC7.1 always expands the inner-wait return |
  next: the inner wait as its own scope / helper; check Aeon's Lua port of the quest for statement shape. Overlay: hdr/engine/
  CQ_HerosOldHouseScript.h (WATCHFORBOOTY, three asserts pinned).
- 00a4d770 C3DAnimation2::CalcLocalTransformationsRelative | DIFFER (420v420, 42 grades) | src/00a4d770.cpp | see the 00A4D770 entry above |
  0x00A4D8F1 `lea edi,[edx+ebx]` v ours `[ebx+edx]` | next: brackets on the dest index expression (owner rule 2026-11-19), the for-scope
  `if (0) {} else for` idiom (0x00A9D750 lesson), FableWin xor/je check. Overlays: hdr/engine/C3DAnimation2.h, C3DMesh2.h.
- 00989230 (escalated, ~88 grades): differ/00989230.cpp, keep/ESCALATE_00989230.md. x87 column order follows the inline-layer structure.
- 0063f4b0 (DIFFER 769, 30 grades): differ/0063f4b0.cpp; loop-exit reload order at 0x0063F6F4 / 0x0063F755 (Q19/Q23 family gap).
- 00ac7eb0 (DIFFER 215, 66 grades): differ/00ac7eb0.cpp; two-line n*n*w operand order. Next: brackets (owner rule), FableWin for-scope idiom.
- 008a8260 (DIFFER 456v466, 3 grades): differ/008a8260.cpp; retail keeps a private early-exit epilogue at 0x008A8428, ours merges it.
- Baseline-only rows (1 grade each, drafts in ref/<addr>/draft.cpp): every other OPEN line in STATUS.tsv.
- Not started (coordinator, end of queue): port 0x009C31E0 Pause with 0x009C3590 / 0x009C3660 onto engine/CFileInstaller.h (FableWin
  0x030495F0 / 0x030499D0) and remove the ?DirtyDisc@CFileInstaller@@2_NA binding (the PDB has @@1_NA).
