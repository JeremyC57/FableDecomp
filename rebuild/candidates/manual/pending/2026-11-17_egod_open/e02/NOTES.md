# e02 notes (egod lane, 2026-11-17)

## 0x006E89B0 CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector -- OPEN, DIFFER(344v344), pass 1 ~36 grades
- Best: differ/006e89b0.cpp. Ported from the local-model draft onto shared headers (CDefendingCombatantInfo.h WHEEL + PRONG +
  new overlay opt-in PLACEMENTVECTOR), Ego_d names (pring, prong, pref_dist in the if block, inner_ring_rad / outer_ring_rad at
  function scope), same-TU bodies in Ego_d order (anon GetPreferredCombatDistanceForAttacker with separation / distance /
  combat_def / far_range / middle_range; GetAttackerProngVector with pprong; GetRingRadius with dist / c1 / pringu). The else path
  residue of the draft closed (named outer_ring_rad). Left: ONE scheduling slot in the first-path epilogue: retail 0x006E8A79
  `pop edi ; fmul st(1) ; pop esi`, ours `pop edi ; pop esi ; fmul st(1)`.
- Tried (no change): distance+separation order, `distance += separation`, `/ 2.0f`, if/else distance (300 bytes), const floats,
  prong *= (354), named ret (354), explicit C3DVector(..) (342), paren tuples at 6 sites, operator* text opt-ins (INITIALISE_SCALE /
  NAMED_RESULT 334, X_PAREN same), C3DVector ctor opt-ins, dead asserts (pring / separation / pref_dist), visible noinline
  stubs of GetProngVector / GetAttackerProng, top-of-function declarations (350 / 374), same-TU CCombatRing::IsOuterRing
  (Ego_r 0x00711110; `!IsOuterRing(def)` gives identical bytes; outer-first arm order swaps the layout).
- Next: c2 scheduler replay (hand892 tools/sched) of the first-path exit block to find what tuple orders fmul ahead of pop esi.
  fwsize: FableWin GetRadius is 71i (an assert inside the inline; shared body, not changed).

## 0x00843310 CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended -- RELOCATION_MATCH (1 grade), test PASS, vland RELOCATION_MATCH
- What matched: shared headers (FinishAction 0x00847520's EXPRESSION_FRAME header set, without CINTELLIGENTPOINTER_CTOR_OUTOFLINE, plus
  CCREATUREACTIONBASE_ANIMCTOR / _SETANIMATION) and Ego_d's facts: the PDB parameter list (sound_id 6th -> SoundEffectID +0xC4,
  frames_running 7th -> FrameCount +0xBC) and NO named locals: the body is `if (PPerformToThing.GetPItem() && AnimationToOtherLoop != "")
  SetAnimation(GetRandomAnimFromName(*PCreature, AnimationToOtherLoop), -1, 0); else if (AnimationLoop != "") SetAnimation(... AnimationLoop
  ...)` and VC7.1 cross-jumps the two calls (the draft's animName local and early return were the residue).
- Headers / flags: none new; no flags.
- Names: row (Ego_r + FableWin), GetRandomAnimFromName 0x006924B0 (Ego_r 0x00518E00 + FableWin 0x01DB7950), all proven.

## 0x00933DB0 CAIStateGroup_MinionKidAnnoy::Update -- RELOCATION_MATCH (8 grades), test PASS, vland RELOCATION_MATCH
- What matched: ported from the local-model draft onto the shared headers of the landed sibling TagTarget 0x009341C0, with Ego_d's
  scopes (MAX_RANGE; block { ret, nav_route_calculated, { attack_range } }); the kick result assigned back to `ret` (tested after the
  temporary's destructors); `GetCreature().GetPhysicsTC()->Radius + PTarget->...->GetRadius()` (RADIUS_ACCESS); and the 2D range test
  as the shared inline GFWorld2DDistanceSmallerThan(creature pos, target pos, MAX_RANGE, false) (engine/GFWorldDistance.h; both PDBs),
  which alone gives retail's X-then-Y differences (every C3DVector / C2DVector subtract or GetSquaredMagnitude spelling and opt-in
  computed Y first).
- Headers / flags: existing opt-ins only (CINTELLIGENTPOINTER_CONVERT_INLINE, _COMPARE_NULL_INT, CTCPHYSICSBASE_RADIUS_ACCESS, the
  TagTarget set); no flags.

## 0x00AC7CB0 QuadricOptimiserInternals::COptimisedPrimitive::GetFaceNormal -- RELOCATION_MATCH (1 grade), test PASS, vland RELOCATION_MATCH
- What matched: Ego_d locals vertex_a / vertex_b / vertex_c / normal only (no edge locals: `normal = (c - a) ^ (b - a)` in one
  expression) + the C3DVector opt-in set and /Gr of the landed same-TU sibling Initialise 0x00ACA830 (NORMALISE_NAMED_SQUARED,
  SUBTRACT_NAMED_RESULT, CROSS_XZ_GROUP, MAGNITUDE_SQRTF). No flags (the sibling's /Gr gives the same bytes). No header changes.

## 0x00986ED0 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby ctor -- RELOCATION_MATCH (~45 grades incl. diagnostics), test PASS, vland RELOCATION_MATCH
- What matched (the residue was the x87 operand order of the inlined GetRight cross products, 0x00986F9A..0x00986FCA): three source
  facts that each add inline layers / records: the template `creature.PeekTC(&ptc)` (not PeekTC(id, &ptc)) with FableWin's PeekTC text
  FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN (instead of GETTC_EXPANDED); `MinCosine = GFCos(0.125f)` (the retail double constant
  0.78539818525314331 is exactly 0.125 * (double)6.2831855f, the shared GFCos text); and a grouped cross text (any of CROSS_X / XY / XZ /
  XYZ / ORIGINAL_GROUPED; CROSS_XYZ_GROUP chosen, the most used). Also CTCENEMY_QUERIES + CTCPHYSICSBASE_STATTYPE for
  StatGetInterfaceType. A k-dead-locals diagnostic (k=1 or 6 matched) proved it was a record-count tie before the real spellings were
  found; no dead locals are delivered.
- Headers / flags: existing opt-ins only; no flags.

## 0x00B9E120 CEngineInternalPrimitive2DStaticMesh::Initialise -- RELOCATION_MATCH (7 grades), test PASS, vland RELOCATION_MATCH
- What matched: the landed twin CEngineInternalPrimitive2DAnimatedMesh::Initialise 0x00BCF4F0 (same Ego_d const-float names) written in
  this row's own statement order, on a real class model (new overlay opt-in FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH) instead of
  the draft's reinterpret_cast model; camera helpers from the shared FABLE_FORCEINLINE branch + FABLE_RETAIL_CENGINECAMERA_DISTH_NAMED
  (no flag). With the twin's _MESH2D_ORIGINAL texts it needs /d2inlT144 (T143 leaves a call): avoided.
- Header: overlay engine/CEngineInternalPrimitive2DStaticMesh.h (new opt-in; the generated block's 21 plain asserts pinned with
  pin_asserts.py; one new assert in the opt-in branch).
- Name note: 0x01436EA0 is NEngineInternal::GPCamera (twin row + this row read camera fields +0xC0..+0x110 through it); the landed
  CEnginePrimitiveRenderer::Render 0x00B33010 binds it as ?g_pLightingManager_01436ea0@@3PAVCEngineLightingManager@@A (NAME_FIXES).

## 0x00ACD960 QuadricOptimiserInternals::COptimisedPrimitive::Optimise -- OPEN, DIFFER(383v383) register-tie, pass 1 ~18 grades
- Best: differ/00acd960.cpp (+ overlay opt-ins FABLE_RETAIL_STLP_MULTIMAP_EMPTY, FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION_ISILLEGAL; /Gr).
  Ported onto the shared headers (COptimisedPrimitive METHODS, CVertexPairContraction, fable_map multimap + TREE_ERASE_BODY) with the
  real names (0x00AC8360 = CVertexPairContraction::IsIllegal: Ego_r 0x00C61350 follows CleanUp 0x00C61090 and retail 0x00AC8360 calls
  CleanUp 0x00AC7B00 first; the draft's "Initialize"/"SimplifyByVertexPairs" were wrong), Ego_d locals, and the same-TU UpdateCost +
  PopFrontContraction bodies after the row: the instruction stream is now retail's exactly (inline erase at the first PopFront, called
  erase 0x00AD3C30 at the second) up to register naming.
- Residue (c2prio_op.py, probe/tools): colouring order y(erase node, 100, esi) > map ptr (91, edi) > Data (86, esi) > Info (72, ebp) >
  this (-7, ebx). Retail: Info esi, Data/y edi, this ebx, map ebp -> Info must outrank y and `this` must outrank the map pointer.
- Tried (no change): CCOUNTEDPOINTER_RELEASEDATA, ORIGINAL_COMPLETE_MEMBERS_PASS22, `contraction = *pair_itr->second` (28 hunks),
  split iterator declaration, break vs return, negated arms, else-if chain (338), no /Gr, visible stub bodies of IsIllegal /
  DetectFoldOver / PerformContraction (387).
- Next: c2prio --blocks on Info / y / map to find the block weights; try the visible real bodies of DetectFoldOver 0x00ACD900 and
  PerformContraction 0x00ACD840 (not landed: pair-landing case) and the multimap erase COMDAT 0x00AD3C30.

## 0x0086B9D0 CRandomAppearanceMorph::GetRandomSkeletalMorph -- RELOCATION_MATCH (13 grades), test PASS, vland RELOCATION_MATCH
- What matched: Ego_d's locals and scopes (keys, it_group, group, upper_bound, it; no_morphs = the inline CVectorMap::Count) and the
  three CVectorMap<long, CSkeletalMorph> members Ego_d instantiates in this TU, visible and out of line: the const LowerBound (default
  noinline body), the const UpperBound (new overlay FABLE_RETAIL_CVECTORMAP_UPPERBOUND_BODY, a plain out-of-class template body -- no
  noinline needed, VC7.1 keeps it out of line itself; fixed the map register and the reload of
  *seed after the calls) and GetKeyList (new overlay FABLE_RETAIL_CVECTORMAP_GETKEYLIST_BODY, Ego_d names key_list / no_key / last_key / it:
  fixed `this` in ebp and upper_bound in the frame). Each body graded at its own address: LowerBound MATCH 0x0086A930, UpperBound MATCH
  0x0086A980, GetKeyList RELOCATION_MATCH 0x0086D240. A visible AddSkeletalMorphToMorphSet stub changed nothing (not used).
- Landing note: the row object also emits the LowerBound / UpperBound / GetKeyList COMDATs of CVectorMap<long, CSkeletalMorph>.
- Flags: none.

## 0x009FAF70 CTexture::GenerateMipmapsWithGPU -- RELOCATION_MATCH (5 grades), test PASS, vland RELOCATION_MATCH
- What matched: ported from the local-model draft onto the shared headers of the landed same-TU sibling BltToRenderTargetWithGPU
  0x009FA780, Ego_d locals (cb, display_manager, old_viewport = GetViewport(), old_rt = GetRenderTarget(), rt; mm in the loop) and the
  same-TU bodies Ego_d lists before the row: GetNoLevels (MATCH 0x009F9DF0) and GetSurfaceLevel (RELOCATION_MATCH 0x009F9E00), both
  inlined here. CSurface needs COPYFROMMEMORY (inline virtual dtor: the vtable re-stores before each Uninitialise).
- Header: new overlay opt-in FABLE_RETAIL_CSURFACE_TEXTUREMIPMAP (CSurface copy ctor + InitialiseFromTextureMipmap declarations). No flags.
- Landing note: the row object also defines CTexture::GetNoLevels / GetSurfaceLevel (same-TU bodies).

## 0x00B50370 CEngineShadowRenderer::CEngineShadowRenderer -- OPEN, DIFFER(406v406) constant-register / scheduling, pass 1 ~10 grades
- Best: differ/00b50370.cpp: ported from the local-model draft onto engine/CEngineShadowRenderer.h MEMBERS + overlay opt-ins
  FABLE_RETAIL_CSHADOWCLIPVOLUME_CTOR (FableWin inline default ctor, fwsize SAME) and _INITIALISETEXTURES (Ego_r ?InitialiseTextures@..@@AAE_NXZ,
  private bool), CRENDERSTATECAPTUREBLOCK_INLINE_CTOR; UV scaler through C3DVector::Initialise (member stores 4 hunks, temporary 443 bytes).
  Everything up to the body's constants matches. Left: retail gives 1.0 / 8.0 / 0.1 three registers at once (eax / ecx / edx, loads
  hoisted, stores in source order); ours reuses eax and the scheduler pairs each constant's stores.
- Measured: moving `UserVideoOptionShadowDistanceScale = 1.0f` before the 8.0 stores gives retail's eax / ecx assignment (live-range
  overlap) but the scheduler then reorders the stores (o1). fwsize: C3DBoundingSphere() and CRenderTarget() texts differ from FableWin
  (22i v 15i, 28i v 18i) -- candidate inline texts that change tuple / record counts before the body.
- Next: fit the C3DBoundingSphere / CRenderTarget default-ctor texts to FableWin sizes (overlay opt-ins), then the statement order.

## 0x0086BE40 CRandomAppearanceMorph::GetRandomBodyParts -- RELOCATION_MATCH (~14 grades), test PASS (e01's test), vland RELOCATION_MATCH; COORDINATOR DECISION (inline control)
- e01's source minus the TU-wide FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE, plus the existing FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
  (STLport _vector.c overflow bodies visible, noinline) and `// Flags: /d2inlT49` with an Inline-budget line (measured: T49 and T50 match, T46-48 and T54-55 differ).
  Under exactly these settings: the row RELOCATION_MATCH, the same-TU helper GetRandomTextureMorphs RELOCATION_MATCH at its own address
  0x0086B810 (it was DIFFER(191v230) under e01's PUSHBACK_NOINLINE; plain inline without OVERFLOW_OUTOFLINE gives 364v230), and the TU's
  other row 0x0086B9D0 also RELOCATION_MATCH. No natural spelling found without the flag: at default thresholds VC7.1 expands
  push_back<CMeshTextureSet> (retail calls 0x0086E7C0, whose own body expands the non-POD overflow), at <= 48 it calls GFRandom.
- Coordinator: two inline-steering devices (one existing opt-in + one per-row /d2inlT). If not acceptable, the row stays parked with
  this source as the best (differ-equivalent) and the e01 hunks.

### Pass-2 audit fixes (advisor)
- 0x0086B9D0: the two new CVectorMap bodies are plain out-of-class template members (no __declspec(noinline)): row, UpperBound MATCH
  0x0086A980 and GetKeyList RELOCATION_MATCH 0x0086D240 unchanged; also RELOCATION_MATCH under row 11's TU settings (/d2inlT49 +
  OVERFLOW_OUTOFLINE). Full vland after the overlay change: all 8 RELOCATION_MATCH.
- 0x00AC7CB0: graded without /Gr: RELOCATION_MATCH; the Flags line dropped.
- 0x0086BE40 disclosure: our object's push_back COMDATs are NOT retail's 0x0086E7C0 (retail: ?push_back@?$CArray<CMeshTextureSet> 187 bytes,
  vector push_back + non-POD overflow expanded, copy through vector<CMorphSetTextureMorph> copy ctor 0x0086B2A0). Ours: CArray::push_back
  a 5-byte jmp, vector::push_back 69 bytes calling the noinline overflow (DIFFER(5v187) / DIFFER(69v187) at 0x0086E7C0). Only the row and
  the helper 0x0086B810 match.

### Pass 2 on the open rows
- 0x00ACD960 Optimise: DetectFoldOver(const CVertexPairContraction&) written for real (Ego_d c_sub loop over SubContractions calling the
  private overload DetectFoldOver(const CSubContraction&, const C3DVector&) 0x00ACD0E0, new overlay opt-in
  FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_DETECTFOLDOVER_SUB): RELOCATION_MATCH at 0x00ACD900, row unchanged (30 hunks, same esi/edi/ebp
  rotation). fwsize: FableWin's ~CCountedPointer<CVertexPairContraction> is 9i/19b (= `{ ReleaseData(); }`, the RELEASEDATA text; graded
  in pass 1, no change), its copy ctor / operator-> / operator* carry asserts (+13..+17i). Not tried: IsIllegal's and PerformContraction's
  real bodies (IsIllegal before the row in Ego_d order: index_list / c_sub / sub locals, 279 debug bytes; PerformContraction 611 debug
  bytes after it, 0x00ACD840 not landed).
- 0x006E89B0: INITIALISE_SCALE (FableWin-sized operator* text) re-read: it makes both returns build a named result and VC7.1 cross-jumps
  their tails (334 bytes, `jmp 0x6E8AE6`); not retail. Default text stays; residue unchanged (one pop slot).
- 0x00B50370: FableWin's C3DBoundingSphere() is 22i/48b; texts tried at /Od: `{ Center = C3DVector(0,0,0); Radius = 0; }` 24i/67b,
  `{ Center.Initialise(0,0,0); Radius = 0; }` 17i/46b, `: Center(C3DVector(0,0,0))` 15i/38b, member stores 18i/61b -- none fits; the
  ctor's own Ego_d S_FRAMEPROC frame is 0x104 bytes with no named locals (debug-only temporaries; FableWin /Od 611i v ours 95i).

- 0x0086BE40 call identity: the row object calls ?push_back@?$vector@VCMeshTextureSet@..@std@@QAEXABVCMeshTextureSet@..@@Z (objdump -r; the
  CArray::push_back wrapper is expanded), i.e. 0x0086E7C0 is bound as vector<CMeshTextureSet>::push_back, as fable_containers.h already
  names it (one-source; retail's 187-byte body expands the non-POD overflow, ours calls it -- the COMDAT itself is not retail's). Retail's
  0x0086CA20 (the helper's overflow call) is the per-element (__false_type) overflow (null-tested 8-byte element copies), the same
  instantiation our build calls, so the helper's masked call identity is consistent.
- Unused overlay opt-in: FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_ISOUTERRING (row 1 trial only; no source uses it -- drop or keep for the row).
