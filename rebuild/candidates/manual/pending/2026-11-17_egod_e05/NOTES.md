# e05 egod lane notes

## 0x006AD700 CThingPlayerCreature::ControlledMove -- RELOCATION_MATCH (22 grades incl. 6 diagnostic), test PASS, vland RELOCATION_MATCH PASS
- Port: include / opt-in set of the landed same-TU SetActionForMelee 0x006AABE0 (CWorld::GetGameCamera -> CAIGameCameraBase slot 10
  GetCameraManager(const CGameCameraManager**) const, CGameCameraManager::GetCamera().PeekWorldPos()), PeekTC(&ptc_hit_loc), Ego_d locals
  verbatim. The first compiling grade already closed the old 110-grade x87 residue (the projection `fld [esp+18h] ; fmul st(2)`):
  the old draft's identities were wrong (the "lock target" is the camera manager, `cam_pos` = camera position; ComputeAcceleration is
  UpdateWalkingControlForces; the action virtuals are IsPreventingNormalMovement / IsClearingMovementVector).
- Three facts closed the rest: (1) existing FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23 (retail squares Y then X, 0x006AD803);
  (2) the same-TU ApplyImpulseVelocity holds the PhysicsTC test (retail's `lea eax,[esi+180h]` before the test, 0x006AD8FD) -- FITTED
  text `if (GetPhysicsTC()) GetPhysicsTC()->ApplyControlledImpulse(impulse); else ImpulseVelocity += impulse;` (/Od 26i/76b v FableWin
  27i/78b); (3) the last residue (X operand order `fld [eax] ; fadd [esi+1F0h]` at 0x006AD910) was one c2 record short (dead-local
  ruler k=1 anywhere, also inside ApplyImpulseVelocity, matched; k=2 too): PeekPCurrentAction through CCountedPointer::Get (FableWin
  10i/25b = that text, new opt-in FABLE_RETAIL_CTHINGCREATUREBASE_PEEKPCURRENTACTION_GET) supplied it.
- Literal 8 = Ego_d NCameraModes::CAMERA_MODE_SCRIPTED_CONTROL: no repo header has EMode's enumerators (coordinator: add the enum
  body to a shared header and respell). vland WARN RELOC_NAME: ?DELTA_NOTIONAL_ZERO@@3MB at 0x0129BA3C is labelled g_NormaliseEpsilon
  (the shared header / landed rows use DELTA_NOTIONAL_ZERO; label debt, not a row issue).
- Names proven (two sources each): ControlledMove, UpdateWalkingControlForces, ApplyImpulseVelocity (Ego_r + FableWin + Ego_d),
  GetPCombatFirstHitLocationPosInside const (Ego_r + retail jmp thunk), GetCameraModeType (landed + Ego_r). No descriptive names.

## 0x00798940 CTCForcePushed::FrameUpdate -- RELOCATION_MATCH (31 grades incl. 9 diagnostic), test PASS, vland RELOCATION_MATCH PASS
- The draft was an unworked local model. New shared header engine/CTCForcePushed.h from the Ego_d class record (no header existed);
  CCreatureAction_BeingForcePushed given its real base CCombatActionBase (opt-in); Ego_d locals and scopes. First compiling grade:
  DIFFER(615v615) with two residues.
- Residue 1, store order `mov [esp+14h],ebx` (pspecial_effects) before `mov [esp+24h],ebx` (effect) at 0x00798A44: the handle is declared
  before `long effect = 0` (FITTED). Ego_d's debug offsets do NOT give declaration order (checked: e04's matched 0x0057AEB4 declares
  pbase_def first, Ego_d puts it at the highest offset; ptc_inventory_weapons/text_bank reversed too).
- Residue 2, X operand order of expected_position (`fld [esi+2Ch] ; fadd [edi+18h]`, 0x007989E2): dead-local ruler needed +2 records
  before it; tried named-result operator- / operator+, GetAngleXY through GET_ROUNDED_ANGLE(GFArcTan) (FableWin 19i size; needs a forced
  inline), GFRoot, named GetSquaredMagnitude: all inert. GFSqr (FableWin ?GFSqr@@YIMM@Z) for `DistanceBeingPushed` squared closed it;
  GFSqr for both bounds also matches (kept: one spelling for both).
- `pcreature->GetTypeFlags() & 2` = "parent is a physical thing" (landed 0x00641020 / 0x0085FA00 spell it the same); the physical
  reference through GFDebugDynamicCast<CThingCreatureBase,CThingPhysical> (Ego_d / FableWin instantiation; not needed for the bytes).
- vland WARN RELOC_ALIAS __CIfmod -> the MSVCR71 import thunk 0x00BFEB28 (expected for fmod).
- Names proven: FrameUpdate, SetUpForcePush, FinishBeingForcePushed, FinalCleanup (Ego_r + Ego_d), CCreatureAction_BeingForcePushed ctor
  (Ego_r), GetEffectFromString / CreateParticleEffect / GetSubDef (bindings + PDB). Physics virtuals at +0x88 / +0x108 are the shared
  header's SetForcedToMovePos / SetFacingAngleXY (slot positions proven by the byte match).

## 0x007B6810 CTCCrateStack::FillStack -- RELOCATION_MATCH (6 grades), test PASS, vland RELOCATION_MATCH PASS
- Ported onto shared headers (CThingObject::Create, CTCBase PeekPlayerManager / GetPWorldSeed, GFFloatRandom out of line, CIntelligentPointer
  ASSIGN, GetAngleXY ANGLE_BODIES) with Ego_d's names; the old "param-home slot reuse" residue vanished at the first compile. The one
  remaining hunk (fsub 0.0625 scheduled ahead of the marker_dir load, 0x007B6884) closed with `adder = GFFloatRandom(..); adder -= RANGE / 2;`
  (FITTED; the second crate written the same way, also matches).
- CRATE_HEIGHT (0.6f, writable .data 0x0138366C, only reference 0x007B697E) is a DESCRIPTIVE name: no PDB / string / FSE name found.
  vland WARN RELOC_ALIAS __CIfmod (expected).

## 0x0089CE70 CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay -- OPEN, DIFFER(614v616) (13 grades), differ/0089ce70.cpp
- New level: Ego_d says the row is a 31-byte wrapper like its four landed siblings; retail is CanThingBe_Sensed_ByOtherThing(thing,
  other_thing, 5) EXPANDED. Defining the Sensed body (codex pass23 draft of the parked 0x0089C910) in the file above the wrapper makes
  VC7.1 inline it here exactly as retail does: 19 hunks plain, 3 with the draft's /d2inlS100 (one is an fg relocation artefact).
- Residue: the cone test's pop form (`fcomp st(1) ; fnstsw ax ; fstp st(0)`, 0x0089D096 / 0x0089D09A, ours `fcompp`), 2 bytes. Same as
  0x0089C910's "x87 compare/pop pair". Tried: GFCos, dot > cos, Ego_d's ptc_physics block local (before / after Normalise), if-return.
- Coordinator: a pair job with 0x0089C910 (fit Sensed's helper texts by fwsize first; the /d2inlS100 flag needs replacing).

## 0x00681E60 CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool) -- OPEN (pass 1), differ/00681e60.cpp = RELOCATION_MATCH on LOCAL MODELS
- Identity: the long overload (Ego_d parameters personality_def / replace_existing_override; Ego_r ?SetPersonalityOverride@CTCCreatureOpinionOfHero@@QAEXJ_N@Z).
  Ego_d's egod_tu maps the name to 0x006820E0, which is the CCharString overload (landed); this row is 0x00681E60 (0x006820E0 calls it).
- With Ego_d's structure the draft's last SIB residue (0x00681FDE `lea ecx,[eax+ecx+10h]`) closed at the first grade: wife attitudes
  disabled by `for (long c1 = 13; c1 < 18; ++c1) AttitudeMask[c1] = false;` in both branches (Ego_d c1 blocks; enumerators
  OPINION_ATTITUDE_TYPE_WIFE_FIRST 13 .. OPINION_ATTITUDE_TYPE_LAST 18), found_attitude after it, end_it/it pairs per block, and the old
  personality's TimeScalingRunOut written twice (no reference local -- Ego_d lists none).
- Not landable yet: every class is the draft's local model (incl. a local std::vector/map/list/fill). Pass 2: port onto
  CTCCreatureOpinionOfHero.h (SERIALISE branch types AttitudeMask / TransientOffsets), COpinionPersonalityDef.h (needs an opt-in typing
  AttitudeEnabled as std::map<EOpinionAttitudeType,bool>), CDefinitionManager / CDefString / CDefPointer, stlport fill / vector / map / list.
  The local-model text is the oracle for the statement shape. The else branch's fill is expanded (rep stos) while the then branch calls
  std::fill<bool*,bool> 0x00685430: expect a per-site inline decision.

## 0x00A1CC30 CNavigatorAStar::ConstructFinalRoute -- OPEN (pair job), differ/00a1cc30.cpp (2 grades)
- Still RELOCATION_MATCH today with the stand-in body of GetIntersectionPointsBetweenNodes 0x00A1A7E0, now with Ego_d's names (goal_state,
  const float navigator_radius, previous_state, next_state, intersection_points; no `diameter` local). Not landable until 0x00A1A7E0's
  real body (Ego_d debug 2490, same TU, listed right before the row) matches at its own address and the classes are ported onto
  engine/CNavigatorAStar.h. Coordinator: pair with 0x00A1A7E0.

## 0x00A1BA10 CNavigatorAStar::GetRandomNavigablePos(const C3DVector&, float, const C2DVector&, float) const -- OPEN, differ = the old draft (0 grades)
- Ego_d does NOT help this row: Ego_d's only GetRandomNavigablePos record has parameters (centre_pos, dist_to_keep_within,
  pos_is_navigator_pos) and a 1628-byte random-position body -- a later revision. The TLC signature is Ego_r's
  ?GetRandomNavigablePos@CNavigatorAStar@@UBE?AVC3DVector@@ABV2@MABVC2DVector@@M@Z (pos, distance, direction, spread).
- Name fix for the draft's callee: 0x00A1AD40 is ?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z (Ego_r name
  set; Ego_d tc order puts GetPosInDirection(start_pos, direction, distance_to_keep_within) right after GetRandomNavigablePos), not the
  draft's descriptive GetNavigablePosInDirection (not landed, so no NAME_FIXES entry). Residue unchanged from the draft (mirrored fadd
  operands of ahead + g_NavHeightTestOffset, 0x00A1BB07); still local models.

## 0x0070E2A0 CAnimateThing::CalcCachedAnims -- OPEN, not graded (pass-1 analysis only)
- Real callees from Ego_r: 0x0070D940 = ?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z
  (retail pushes 0.0f / 1.0f, &DelayedAnimQueue.front() (queue +0x60 head node +8), this = &CurrentAnim at +0, hidden result);
  0x0078DEF0 Ego_r ?GetPSkeletalMorph@CTCSkeletalMorph@@QBE?BV?$CCountedPointer@VC3DSkeletalMorph@@@@XZ (retail 0x00786870, by value);
  StartAnimState / EndAnimState = result (C3DAnimationBlendState::operator= expanded), then SetSkeletalMorph 0x00A3C350 on both when
  PeekTC(&ptc_appearance_morph) (interface 0x78) and the morph pointer is non-null (setne on a by-value temporary).
- Ego_d: function-scope `const CTCSkeletalMorph* ptc_appearance_morph`; a block with `long c1` (retail shows no loop -- a check whether a
  loop of SetSkeletalMorph or a debug-only validation loop; compare with fwsize: Ego_d debug length 1097).
- Port needs: CAnimateThing.h opt-in (CalcCachedAnims / CalcInterpolatedMeshAnim declarations), a new CTCSkeletalMorph header (none in
  engine/ or _quarantine/), C3DAnimationBlendState SHAREDDATA / TYPED_DATA + operator= text, SetSkeletalMorph declaration. Same-TU
  landed sibling UpdateCachedAnims 0x0070E520 is on local models.

## 0x008BF280 CCreatureAction_GenericFireProjectileWeapon::PerformAction -- OPEN, not graded
- Ego_d scopes: { ptc_weapon (CTCWeapon*), pcarried_thing (CThing*), ptc_carry (const CTCCarrying*) { shot_damage_multiplier (float),
  global_index (long), ptc_animating_object (CTCAnimatingObject*), shot_accuracy_percentage (float) { pshot { pTCEvents } } } }.
  The draft (local models) has `accuracy` and `shotIndex` = shot_accuracy_percentage / global_index; one of the draft's two literal
  1.0f arguments is probably shot_damage_multiplier. Old residue: weapon in esi (retail edi) and the accuracy float sharing the int
  slot -- Ego_d's separate float local is the first thing to try once ported.

## 0x00805600 CTCTargetingPlayer::FrameUpdate -- OPEN, not graded
- Ego_d: function scope pgp_int (const CGamePlayerInterface*), z_targeting (bool), ptargeted_thing (const CThing*), test_slong (long);
  block { lose_target (bool), ptc_targeted (const CTCTargeted*) }. test_slong = the frame stagger: retail loads the 64-bit UniqueID
  (+0x18/+0x1C, high word stored dead) and adds GetWorldFrame (`(frame + id) % 4`). Const pointers mean const method overloads
  (IsActive / IsGameActionEventInQueue / IsZTargetable / const LowerBound).
- Blockers recorded by the earlier pass: engine/CTCTargetingPlayer.h is wrong past +0x68 for retail (two 16-byte PDB vectors are 12
  bytes), callee names 0x0085EE10 / 0x004379D0 unproven. Start from the FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST rows
  0x00804020 / 0x00803D60 (landed, same class, shared headers).
- 0x0089CE70 pass 2 (2 grades): the GFCos text (FableWin ?GFCos@@YINM@Z) expands but moves the dot product AHEAD of the GetFOV call
  (frame 0x20 v 0x18, 36 hunks) -- not the original spelling at this site. 0x0089C910 has no oracle in this batch (cannot grade the
  Sensed body at its own address here).

## Summary (e05)
- Matched (RELOCATION_MATCH, test PASS, vland PASS): 0x006AD700, 0x00798940, 0x007B6810.
- Open: 0x0089CE70 (DIFFER 614v616, pair with 0x0089C910), 0x00681E60 (matches on local models only; port pending), 0x00A1CC30 (matches
  only with a stand-in callee body; pair with 0x00A1A7E0), 0x00A1BA10 (Ego_d record is another revision), 0x0070E2A0 / 0x008BF280 /
  0x00805600 (not graded: ports onto shared headers needed; analysis above).

## Final adjustments (after the advisor review)
- 0x00798940: FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE dropped -- the row matches with the shared GetSubDef body merely visible
  (VC7.1 keeps 0x00669580 a call by itself): no inline control in the row. 0x00669580 has no oracle in this batch (not graded at its
  own address). CTCForcePushed.h parameter names now Ego_d's (distance_to_move, thing_causing_the_force_push, hit_in_back); ctor
  (CThing&) resolved from Ego_d arglist 0x9D53. All three rows regraded RELOCATION_MATCH after this last header edit; vland WINS 3.
- 0x007B6810: locals moved to function scope as Ego_d records them, with an early `if (!height) return;` (RELOCATION_MATCH).
- 0x006AD700: the ApplyImpulseVelocity text is FITTED (26i/76b at /Od v FableWin 27i/78b) -- coordinator to decide; the
  PeekPCurrentAction `.Get()` text is proven by size (10i/25b = FableWin).
