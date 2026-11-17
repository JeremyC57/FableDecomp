# e03 NOTES (egod lane) -- summary (details per row below)
- MATCHED (6, all RELOCATION_MATCH + test PASS + read-only vland): 0x00B4B8B0, 0x00753E90, 0x008E5D40, 0x00B81820, 0x00920DC0, 0x00867D20.
- OPEN (4, differ/ + notes): 0x00B9D730 (429v429, one x87 operand order), 0x00A90260 (498v496, ebx/edi colouring),
  0x00919240 (508v508, two entry instructions), 0x007E0F20 (531v537, presence-word load form + one frame word).
# e03 NOTES (egod lane)

## 0x00B4B8B0 CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace -- RELOCATION_MATCH (8 grades)
- FITTED spelling (pvec.Initialise; FableWin / Ego_d /Od size 376b, this text 282b). Matched with Ego_d's locals (shadow_light_vector copy, pvec, shadow_world_x / _y as `^` products, local_world_to_shadow via the
  12-float ctor + PostScale(C3DVector)): our /Od frame puts every local at Ego_d's offset. What closed the old x87 residue
  (`fld st(2) ; fmul [L.Z]`): `pvec.Initialise(0,1,0)` / `(1,0,0)` in the two arms (member-wise stores gave the old residue; a
  ternary / assigned temporaries put pvec in memory). The result-copy scheduling needs the existing shared grouped cross text
  (FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP, FITTED in the repo; default cross leaves the copy set-up hoisted).
- Headers: overlay opt-in FABLE_RETAIL_CENGINESHADOWRENDERER_CALCLOCALWORLD (declaration). Flags: none. Names all PDB (two sources).
- FableWin /Od size of the row is 376b/137i, ours 290b/98i: the original has ~39 more debug instructions (maybe asserts); not needed.

## 0x00B9D730 CEnginePrimitiveManagerStaticMeshesBase::IsInsideFustrum -- OPEN DIFFER(429v429) (pass 1, ~30 grades)
- Best source differ/00b9d730.cpp: shared headers only (sibling RenderPrimitive 0x00BA1140's include block, pruned), Ego_d locals
  center / radius, `mesh_prim->GetBoundingSphere().GetCenter()` + `primitive.ObjectTransform.TransformPoint(center)`,
  `GetBoundingSphere().GetRadius() * primitive.Scale`, `CEnginePrimitiveRenderUtil::GetCurrentCamera().ClipSphere(center, radius)`.
  Overlay opt-in FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_ISINSIDEFUSTRUM (private declaration). The whole body
  matches except ONE operand order: retail 0x00B9D7A4 `fld [ecx+18h] ; mov ecx,[01436EA0h] ; fmul [eax+58h]` (Radius first),
  ours `fld [eax+58h] ; fmul [ecx+18h]`.
- Needed (evidence): the by-value GetBoundingSphere() copy for the centre (without it the frame / loads change, 451 bytes).
- Tried, no change at the site: radius before / after centre; `*=`; scale at the ClipSphere call; operands swapped; direct
  `BoundingSphere.Radius` / `.GetRadius()`; user sphere copy ctor (C3DBOUNDINGSPHERE_COPY); dead-local ruler k=1..7 (not
  record-count keyed by outer names); GPCamera direct (fwsize 35i/100b v FableWin 35i/101b; ours with GetCurrentCamera 36i/101b);
  Ego_d's own 2-arg ClipSphere text (locals zdist / vec, 262 debug bytes; candidate texts in tmp/csA.txt, `peekg` variant is
  93i/262b v FableWin 91i/262b) + FableWin-sized operator- / Dot texts.
- Note: retail RenderPrimitive 0x00BA1169 also loads the radius first (`fld [eax+34h] ; fmul [esi+58h]`) and the landed
  siblings 0x00B9DBD0 / 0x00B9DC60 reached radius-first only with a `double` local (not Ego_d's float). Next idea: a c2 probe of
  which of two memory factors is loaded first (param order / address-temp age), then look for the IL shape that gives it.

## 0x00753E90 CTCLook::LookAtPos -- RELOCATION_MATCH (12 grades)
- Re-ported from the old local-model draft onto the shared engine/CTCLook.h (the defines block of the landed sibling
  0x00752E00 + CTCBASE_COMPONENT / CMAINGAMECOMPONENT_WORLDFRAME / CTCCREATURENAVIGATION_{ISNAVIGATING,NAVIGABLE}) with Ego_d's
  locals (dir_to_look_in; block ptc_creature_nav + `bool naving = GetTC(&nav) && nav->IsCurrentlyNavigating()`): the old
  frame / edi-v-ebp residues vanished at the first grade.
- What closed the rest: Ego_d tc_look.obj's same-TU bodies defined after the row in Ego_d order -- SetHeadTurnState (with
  `TimeToStopLooking = -1;` LAST: store-first hoists it above the head-state test, retail sinks it to the join 0x00753F7E; the
  body grades MATCH at 0x007517B0 either way), GFGetAngleYZTo 0x00754E70 (wrap written out like the shared XYTo text),
  IsOverriddenByParent 0x00751A70, SetRecoveryAngles (expanded), UpdateLookTimer 0x00751AD0 (the row calls UpdateLookTimer()) --
  plus the existing shared opt-in FABLE_RETAIL_GFANGLES_ANGLEXYTO_BODY (with GFGetAngleXYTo visible retail keeps the parent thing
  in edi across the two angle calls).
- Flags: none. Header overlay: none. vland: RELOCATION_MATCH + non-blocking WARN RELOC_NAME (0x0129BA3C bound
  ?DELTA_NOTIONAL_ZERO@@3MB, an old label table says g_NormaliseEpsilon; already recorded in labels_landing_2026-11-15ga.tsv).
- Text note for the coordinator: the landed out-of-line SetHeadTurnState 0x007517B0 source writes the TimeToStopLooking store
  first; this row needs it last (both texts grade MATCH out of line). One text per helper: prefer store-last.

## 0x008E5D40 CTCPhysicsStandard::Move -- RELOCATION_MATCH (14 grades)
- Re-ported onto the shared CTCPhysicsStandard.h / CTCPhysicsBase.h (the landed twin CTCPhysicsControlled::Move 0x00725C40's
  defines) with Ego_d's locals (area, preserve_ke, current_pos, speed, thing_list).
- fwsize with FW_EXTRA=/EHsc (the row has a destructible vector) was the key: with FableWin's inline accessors IsMoving() /
  GetVelocity() / GetPhysicsDef() the row text is FableWin-size (120i/379b SAME); without /EHsc the comparison is meaningless.
- Frame (thing_list v current_pos slots swapped, the old residue): the second moving test as an early return after
  PerformStepBasedMovement (adds a destructor reference to thing_list). NOTE: that text is 124i at /Od /EHsc v FableWin 120i
  (FableWin is a later build) and Ego_d debug length 379 = the if-block form: FITTED (chosen by the retail frame). The if-block form with an 11i allocator::deallocate (`free(p)`, FableWin-size) was probed: frame still swapped.
- x87 orders: new overlay opt-ins MAGNITUDE_X_THEN_YZ (FableWin-size SAME) and NORMALISE_LOCALS_YZX (47i v FableWin 48i), both
  FITTED orders. Flags: none. vland clean.
- Header debt: Ego_d's EPhysicsInteractionType (PHYSICS_PRESERVE_KE = 128, PHYSICS_STANDARD_COLLIDE_WITH_IMPASSABLE_LANDSCAPE =
  512, ...) is not in a shared header; the row writes the values with the names in comments.

## 0x00B81820 CEngineSimulationManager::FrameStart -- [SUPERSEDED: see the pass-2 section below] DEFERRED to pass 2 (0 grades; port plan below)
- The draft is a local model; a landable source needs shared-header work first: CEngineSimulationManager.h's retail branch is
  opaque (members not modelled; the generated default has the right Ego_r offsets: lists +0x10/+0x14/+0x18, WindNoiseTime
  +0x1028, Zoom +0x1068, the pending counts +0x106C..+0x1074 -- FrameStart is a CEngineComponent virtual so `this` = object+4,
  hence retail's [ebp+0Ch]); no header exists for CEnginePrimitiveSimulationInfo / CEngineSimulationBase.
- Ego_d facts: function-scope `CMovableResourceMemoryPool* old_cloth_pool` (the global at 0x013D2D70 is the cloth memory pool,
  saved / set to the world's pool +0x368 / restored), three blocks `{ list iterator itr; { CEnginePrimitiveSimulationInfo& sim =
  **itr; ... } }`; the inner loop is the same-TU CEnginePrimitiveSimulationInfo::RunSimulation (retail out-of-line copy
  0x00B80B40, uncalled; Ego_d locals itr / sim) expanded; CEngine::GetNonInterpolatedWorldTime (20 debug bytes) is first used
  here (the double at [0x01436EA4]+0x98).

## 0x00920DC0 CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace -- RELOCATION_MATCH (3 grades)
- Matched at the first compilable port onto shared headers (CAIStateGroupBase MEMBERS/GETCREATURE, CTCPhysicsBase, CThing,
  CIntelligentPointer + COMPARE_NULL_INT for the shared IS_THING_ALIVE<T>, CRightHandedSet CTOR_INLINE, C3DVector ANGLE_BODIES
  GetAngleXY, CCreatureAction_RiseUp over CCreatureActionBase FULL_MEMBERS) with Ego_d's block-scope `direction` and the tail as the
  qualified base call CAIStateGroup_CreatureIdleUnderground::OnReplace() whose body (same TU, Ego_d order) is defined after the row
  and grades RELOCATION_MATCH at 0x00920960. Physics calls through GetCreature().GetPhysicsTC() virtuals ResetPosition /
  SetFacingAngleXY / ResetRHSet. Flags: none. Header overlay: none (C3DVector overlay default paths unchanged).
- Local class declarations (no shared header exists): CAIStateGroup_CreatureIdleUnderground : CAIStateGroupBase and
  CAIStateGroup_CreatureSeekEnemyUnderground : CAIStateGroup_CreatureIdleUnderground { PEnemy +0x14; RisePos +0x1C } (Ego_r names).

## 0x00A90260 C3DPrimitive2::CreateDegenerateTriangleIndexBuffer -- OPEN DIFFER(498v496) (pass 1, 8 grades)
- differ/00a90260.cpp is the Ego_d-faithful port (shared C3DPrimitive2.h with the landed UploadToIndexBuffer 0x00A8F9E0 defines +
  CARRAY_SIZE_CTOR_OUTOFLINE): CArray<unsigned long> index_counts, a function-scope boost::scoped_ptr<CIndexBuffer>
  new_index_buffer (reset(new), InitIndexBuffer(*new_index_buffer, new_byte_size, InitFlags), swapped into IndexBuffer at the end
  so its destructor deletes the old buffer -- the retail 0x00A90423..0x00A90437 tail), four blocks each with its own c_block /
  block_info& (+ count, next_block_info& in the first, src_offset / src_indices / new_byte_size / dest_indices / dest_offset in the
  copy block). Whole structure right; residue = register colouring from 0x00A90271 on: retail total_indices edi / stride + new
  buffer ebx, ours the reverse, which also turns two `cmp byte [reg+9],0` flag tests into `mov bl ; test bl,bl`.
- The old draft (ref draft.cpp, 496v496) has a single function-scope `i` and a raw new_buffer pointer, contradicting Ego_d.
- Tried: scoped_ptr declared first / after index_counts, new_byte_size before the new, FableWin accessors GetStaticBlockCount /
  GetStaticBlock (fwsize SAME each; row 251i v FableWin 275i at /Od /EHsc -- ~24 instructions of the original text unknown).
  Next: c2 priority trace of total_indices v the new-buffer temp; InitIndexBuffer FableWin 27i v ours 25i (text off by 2i).

## 0x00919240 CAIStateGroup_LookAtInterestingThings::FindInterestingThing -- OPEN DIFFER(508v508) (pass 1, 5 grades)
- The draft (already shared headers) has exactly Ego_d's locals (objects, total_interest, chosen_interest; two blocks {n;
  {interesting}}). Residue: two entry instructions -- retail `xor ebp,ebp ; mov [esp+10h],0B3h ; mov ecx,[eax+18h] ; push edi`
  (filter temporary stored before the creature load), ours `mov ecx,[eax+18h]` first and the store after `push edi`.
- Tried: CAIBrain creature pointer form (no CREATURE_REFERENCE), PBrain->GetCreature(), a named search-tools reference
  (diagnostic, worse), the template body hidden (diagnostic: 540 bytes, so the visible same-TU body is required). Next: the
  retail order looks like a may-alias edge between the filter store and the creature load -- look for a source shape where the
  filter's address is taken before the box argument is built (e.g. the CThingFilter_HasTC ctor out of line / a filter copy inside
  the template body), or the scheduler-region cut (c2 sched replay).

## 0x00867D20 CCombatAction_GenericResponseToExplosion::FinishAction -- [SUPERSEDED: see the pass-2 section below] DEFERRED (0 grades; needs a full port)
- The draft is a local model (local CThingCreatureBase / CCreatureActionBase / action stand-ins). Ego_d locals for the port:
  function scope `long anim_index`, `bool flip`, `CTCCreatureModeManager* tc_modes`; block { `const C3DAnimationInfo* info`,
  `CCountedPointer<C3DAnimationStats const> stats` }; block { `const CThingCreatureDef& creature_def` } (the stabbed-to-death test
  reads the creature def through a named reference). Twin: 0x00867B40 (same residue per the draft notes).

## 0x007E0F20 CTCAICreatureWillPowerIndicator::FrameUpdate -- [SUPERSEDED: see the pass-2 section below] DEFERRED (0 grades; needs a full port)
- The draft is a local model with stand-in bodies (GetSubDef / GetEffectFromString / GetTCDriver stubs): not landable.
- Ego_d locals: `CTCCombat* ptc_combat` (interface 0x30 is CTCCombat, not CTCMelee; IsInMelee 0x00733190 is its call); block {
  `CDefPointer<CSpecialEffectsDef const> pspecial_fx`, `long fx_index`; block { `CRightHandedSet rhs`, `CTCCarrying* ptc_carrying`,
  `C3DVector weapon_pos`; { `CThing* pweapon`; { `CThing* pemitter`; { `CTCDParticleEmitter* ptcd_particle_emitter` }}}}}.
  Retail 0x007E0FCD..0x007E1013: `GetSubDef(&pspecial_fx) && pspecial_fx->GetEffectFromString(CCharString("POWER_UP_INDICATOR"),
  fx_index)` with the conditional CCharString destruction flag at [esp+10h].

## Pass-2 plan (after pass 1)
- Protect matches: row 3 test must check the YZ recovery angle; row 3 GFGetAngleYZTo text = landed 0x00754E70 text; row 4 early
  return relabelled FITTED (FableWin / Ego_d debug size 379 = the if-block form) + one deallocate-11i probe; NAME_FIXES 0x00753B40.
- Ports in order 5, 8, 10 (Ego_d debug length = FableWin /Od size: targets 807 / 2408 / 1196, fwsize FW_EXTRA=/EHsc when a local
  has a destructor). Then row 7 (text 786b v 869b: close with fwsize), row 9 (fwsize v 1467), row 2 last.

## 0x00B81820 CEngineSimulationManager::FrameStart -- RELOCATION_MATCH (pass 2, 4 grades; 3 were include fixes)
- Matched at the first compiling grade once ported: member model of the retail class (overlay opt-in
  FABLE_RETAIL_CENGINESIMULATIONMANAGER_MEMBERS: Ego_r member names/types at the generated offsets, three STLport lists, FrameStart
  override), new overlay headers engine/CEngineSimulationBase.h (virtual slot layout from the retail vtables, RunSimulation slot 6)
  and engine/CEnginePrimitiveSimulationInfo.h (CIVCountedPointeeBase + Simulations CArray at +8), the same-TU RunSimulation body
  (Ego_d locals itr / sim; MATCH at 0x00B80B40), Ego_d's `old_cloth_pool` + three `{ itr { sim } }` blocks, `while (count) { ...;
  --count; }`, and the FableWin inline accessors (new opt-ins FABLE_RETAIL_CENGINE_GETNONINTERPOLATEDWORLDTIME,
  FABLE_RETAIL_CMESHDATABANK_GETCLOTHMEMORYPOOL, FABLE_RETAIL_EMESHBANKNO_ENUMERATORS; existing CENGINERESOURCEMANAGER_GETMESHBANK).
- vland: RELOCATION_MATCH + WARN RELOC_NAME (0x01436EA0 bound ?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A, an old label says
  g_pEngineCamera; non-blocking).
- fwsize notes: GetClothMemoryPool FableWin 10i/25b v this text 9i/20b (FableWin reads a typed CCountedPointer member, the
  flattened header has the raw Data word); RunSimulation /Od 208i v FableWin 83i (release bytes MATCH; the /Od gap is STL debug
  shape, not examined).

## 0x00867D20 CCombatAction_GenericResponseToExplosion::FinishAction -- RELOCATION_MATCH (pass 2, 1 grade)
- Written from the LANDED twin CCombatAction_GenericStrikeResponseKnockdown::FinishAction 0x00867B40 (its include block, local
  CCreatureAction_BeStunned / ...KnockdownRecover classes and IsKnockdownToStun declaration), with Ego_d's names (anim_index, flip,
  tc_modes, { info, stats }, { creature_def } as a block-scope `const CThingCreatureDef&` in the no-health arm) and this row's
  differences read off retail (mode test through GetCurrentMode().GetType(), trailing RemoveMode(0x42)). First grade matched: the
  old "allocation gap" was the local model. Overlay opt-in FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETOEXPLOSION_FINISHACTION.

## 0x007E0F20 CTCAICreatureWillPowerIndicator::FrameUpdate -- OPEN DIFFER(531v537) (pass 2, 7 grades)
- differ/007e0f20.cpp is the first landable-shaped port (the old draft used stand-in bodies): shared CThing / CTCBase /
  CIntelligentPointer (CTOR / ASSIGN out of line) / IS_THING_ALIVE (NOINLINE: retail calls the <CIntelligentPointer<CThing>> and
  <CThing*> COMDATs) / CDefClassBase GetSubDef<CSpecialEffectsDef> / CSpecialEffectsDef / CTCCombat ISINMELEE / CTCCarrying /
  CTCDParticleEmitter FULL (Create(long, const C3DVector&, bool), AttachToThing(CThing&, long, const CCharString&, long, float)) /
  CThing GETTCDRIVER; local class CTCAICreatureWillPowerIndicator : CTCBase with Ego_r members (no shared header). Ego_d locals:
  ptc_combat, { pspecial_fx, fx_index, { ptc_carrying, weapon_pos, { pweapon, { pemitter, { ptcd_particle_emitter }}}}}; Ego_d's
  `CRightHandedSet rhs` is NOT written (no trace in retail; the 0x20 frame has no room for it).
- Residue: (1) retail `mov ecx,[eax+24h] ; xor ebx,ebx ; test ecx,10000h ; mov [esp+0Ch],ebx` (0x007E0F2A) v ours `xor ebx ; mov
  [esp+0Ch],ebx ; test dword [eax+24h],10000h`; (2) the GetTC key temporary shares fx_index's frame word in retail ([esp+18h]),
  ours gets its own ([esp+1Ch]) -> 4-byte renumbering of the later temps. Tried: ptc_combat with / without `= 0`, three separate
  returns, the whole body inside `if (GetTC && IsInMelee && !PoweredUp)`, HASTC_MATERIALISED (worse, 543). Next: the `Timer <=`
  test spelling (fild/fild compare: probably a frames-per-second helper), a named rhs (Fidelity) as a packer probe, the
  IsInMelee / PoweredUp order.

## Coordinator decisions (e03)
- Two new overlay headers (not opt-in gated: the classes had no header): engine/CParticleContext.h, engine/CEngineSimulationBase.h,
  plus engine/CEnginePrimitiveSimulationInfo.h (Ego_d class records; see hdr/README.md).
- Same-TU helper texts that differ from LANDED local-model rows of the same address (all grade MATCH / RELOCATION_MATCH at their own
  addresses from my files): RunSimulation 0x00B80B40 (landed: local CSimulation_00b80b40 / SSimulationEntry types),
  IdleUnderground::OnReplace 0x00920960 (landed: local N_0062e900 model), SetHeadTurnState 0x007517B0 (TimeToStopLooking store last
  v first), GFGetAngleYZTo 0x00754E70 (wrap written out v the landed forced-inline made-up WrapAngle).
- The FableWin.pdb accessor opt-ins I added are named after FableWin entries (GetNonInterpolatedWorldTime, GetClothMemoryPool,
  GetVelocity / IsMoving / GetPhysicsDef).
- 0x013D2D70 is spelled with its bound name ?g_ParticleMemoryPool@@3PAVCMovableResourceMemoryPool@@A; Ego_d calls the saved value
  old_cloth_pool (the global's PDB data name is not in the name sets).
- Header debt: EPhysicsInteractionType (Ego_d enumerators) not in a shared header (row 0x008E5D40 writes 0x80 / 0x200).
- vland warnings (non-blocking): RELOC_NAME 0x0129BA3C (DELTA_NOTIONAL_ZERO v label g_NormaliseEpsilon), RELOC_NAME 0x01436EA0
  (GPCamera v label g_pEngineCamera), RELOC_ALIAS __CIfmod -> MSVCR71 _CIfmod thunk.
