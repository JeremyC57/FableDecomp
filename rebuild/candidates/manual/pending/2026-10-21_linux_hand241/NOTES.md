# Hand chunk 241 (CAIStateGroup_Wander .. CCRCSymbolMap): 13 of 40 byte-exact

All 13 matched rows grade MATCH/RELOCATION_MATCH (`grades.txt`) and their behaviour tests print `<addr>_TEST PASS` under
g++ (`tests_gpp.txt`) and VC7.1 (`tests_vc.txt`). 17 rows are parked in `differ/` (best source, `// Grade:` line 2),
10 rows are skipped (1 lane gap, 9 not attempted within the budget).

## What matched (13)
- CAIStateGroup_Wander::Update 0x00911710 -- wander near the creature: the landed ChickenWander model (GetTC ternary,
  5.0f radius, `if (Random(100) >= 50) return; idles = Random(2); if (idles == 0) return;` then the idle loop).
- CAIStateGroup_WhisperPotionCombat::IsRunnableLeaf 0x0096aad0 -- boss-phase switch; the frame-stamp/range check lives in a
  `__forceinline` helper so VC7.1 pushes/pops ebx inside the late branch only (NEW shape lesson, reused for CAS_Ambusher).
  `(float)delta > (float)g_FrameRate * 10.0f`; retail layout PTarget +0x14, TrapThings +0x1c (12 bytes), FrameLastRun +0x28.
- CAIStateGroup_WhisperRetreat::Update 0x0096b600 -- phase switch (cases 0 and 2; case 2 has its own Terminate+return);
  IsThingCarryingWeapon __fastcall, UnsheatheMeleeWeapon, CTCStealth::SetAsInDanger, JumpToSafety/UseWill.
- CAS_Ambusher::Main 0x00e2fd30 -- ally with the trickster, wait for the hero, shout, idle: body after the early
  `if (IsDone()) return;` in a `__forceinline RunAmbush()`; `const CCharString&` bound to the def string (no copy);
  `~CScriptThing()` declared out of line so the trickster is destroyed by call at both exits.
- CAS_Buddy::Init 0x00e2bfa0 -- by-value CScriptThing arguments built in the call area with the inline copy ctor (two
  vtable stores + AddRef); pack(1) script layout.
- CASoundBank::InitSymbols 0x00a01950 -- SplitPath / operator+ __fastcall / DisplayProgress with implicit "" conversions;
  `PSymbolMap.Reset(new CCRCSymbolMap)`; LoadBinary / LoadFromFile / SaveBinary fallbacks.
- CActionFollowCreature::OnCleanup 0x007158f0 -- FinishAction model; `bool` parameter gives `xor edx,edx; mov dl`; the
  GetTC ternary form (component straight into ecx) vs the out-param form decides `mov ecx,[eax+4]`.
- CActionOverheadTalkToThing::Update 0x00717450 -- block-scoped `{ long chars ... }` / `{ CWideString text ... }` share one
  frame slot; `Max(0L, chars)` by const ref gives the `lea` select with `jg`.
- CEnvironmentThemeDefVector::operator= 0x00472eda -- byte copy of the landed COpinionDeedReactionDefVector::operator= model
  (`#pragma optimize("s", on)`, named `__false_type __tag`, __forceinline __copy / __uninitialized_copy, `_Destroy` ->
  `__destroy_aux` __fastcall) with the 0xc4-byte element.
- CBanditCronies::Main 0x00e03810 -- the 16-byte `CIsThingAlive filter(Me); RegisterFilter(filter);` scope object of the
  landed CHTEPrisoner::Main (vtable 0x012C32F0, CEntityScriptBase::SetPredicate 0x00F35B10), `NewScriptFrame; if (IsDone())
  return;` before the rotated distance wait loop, the sound as a CCharString LOCAL (operator CCharString temp + copy ctor)
  destroyed after the final `do frame while (!IsDone())`.
- CArcheryCompetitionSignpost::Main 0x00cdec40 -- implicit const char* -> CCharString temporaries; the state dispatch is
  an if / else-if chain in the order 0, 2, 1 over a NAMED copy of the shown state (re-reading the member re-loads it);
  rotated `while (!IsDone()) { ...; NewScriptFrame(); }`.
- CBlendedParticleEffectSet::TransferBinaryIn 0x0046944c -- size-optimised def TU; four inlined CDataInputStream reads
  through `__forceinline long ReadLong() { long v; Read(&v,4); return v; }` / `ReadFloat()` helpers: the returned local is
  homed in the dead stream-argument slot and loaded BEFORE `this` (a plain `value` local + member store loads `this` first).
- NCameraModes::CBase::UpdateFixLookDirection 0x0084cdf0 -- bool local in bl, `(float)cos(angle * 2pi_double)` homed in the
  angle slot, dot product written `dir.Z * flat.Z + flat.X * dir.X + flat.Y * dir.Y` (first product's operand order
  matters), clamp chain, `float turn = (float)acos(dot) * INV_2PI; flat.Rotate(axis, turn * sign);` (the named local fixes
  the multiply order; `acos * c * sign` in one expression multiplies by sign first).

## Parked for Fable 5.1 (17)
- 00911a90 CAIStateGroup_WanderAroundInitialPos::Update DIFFER(391v391): esi/edi swapped in the first half (retail creature esi, interface map edi), second half identical | tried: 5 lookup/ordering forms | next: lane gap: register colouring
- 00947280 CAIStateGroup_WanderAroundShops::IsRunnable DIFFER(346v346): retail keeps this in edi and forms &PrettyThing as `lea esi,[edi+1Ch]` in both arms; ours `add edi,1Ch` | tried: member / reference / pointer / byte-array forms | next: lane gap: register colouring (&member hoisting)
- 00926060 CAIStateGroup_WifeGiveYouGift::Update DIFFER(387v386): retail keeps the speech wait loop top-tested (one PlayAISpeech call, `je` back, private Terminate+epilogue); the lane inverts it | tried: while, for(;;), do/while+goto, goto-retry, reassign, inline helper x2 | next: lane gap: loop inversion of a top-tested while
- 009426d0 CAIStateGroup_WorkAtSchool::PlayLoop DIFFER(328v322): the shared `xor al,al` false exit sits right after the first action test in retail; the lane puts it after the final `sete al` exit | tried: 4 exit/branch forms | next: lane gap: false-exit block placement
- 00944300 CAIStateGroup_WorkInWarehouse::Update DIFFER(344v354): retail hoists &PCrate into ebp for the loop (this spilled); the lane re-forms `lea [ebp+1Ch]` per use | tried: member, reference, byte-array cast | next: lane gap: register colouring (&member hoisting)
- 008718b0 CActionEventAddObject::Dispatch DIFFER(341v341): this esi/edi swap and player/objectDef ebp roles (ebp pushed inside the slot>0 branch in retail) | tried: 3 lookup forms incl. the __forceinline-helper sink | next: lane gap: register colouring
- 00713d50 CActionFollowCreature::SetAsSneaking DIFFER(395v395): scratch-register scheduling in the sneaking-on arm (creature through eax, bitset word in ecx) | tried: direct, accessor, named local | next: lane gap: scratch scheduling
- 004e6f74 CTextureMorphEntryVector::operator= DIFFER(290v333): retail spills x._M_start/_M_finish to frame slots and keeps the element size in esi (POD element) | tried: landed CEngineGraphicVector POD model, nested struct, begin()/end() | next: find a landed POD operator= with the spill shape (size-optimised TU?)
- 004e72c8 CSkeletalMorphEntryVector::operator= DIFFER(273v327): same POD operator= spill/element-size pattern | tried: same 3 forms | next: as 004e6f74
- 0070d1a0 CAnimSet::Init DIFFER(331v331): the `__true_type` tag temporaries are homed in the live alpha argument slot in retail (ours: data/animator slots); CurrentAlpha/LastAlpha stores scheduled apart | tried: short field, unsigned short field (temp fixed), STLport __copy_trivial (fixed), named tag local | next: the retail tag may be a zero-size global/static; try `LastAlpha = CurrentAlpha` ordering variants
- 007fd8d0 CApplyImpulseIfCloseToCombatLine::operator() DIFFER(321v321): the GetTC component load `mov eax,[eax+4]` vs ours edx/ecx in the two flag setters | tried: inline GetPos copy, named local via reference, direct member copy, user copy ctor (frame now identical) | next: lane gap: register colouring
- 00d167c0 CBH_Hostage1::Init DIFFER(340v340): Me copy read via [edi+0x10]/[edi+0xc] in retail, ours via an ebp alias | tried: CScriptThing Me, CScriptThingStored Me | next: lane gap: register colouring
- 00d16bb0 CBH_Hostage2::Init DIFFER(333v332): this in esi (retail) vs edi | tried: same 2 forms | next: lane gap: register colouring
- 00e38f80 CAttackingAssassin::Init DIFFER(377v376): new result kept in edi (retail) vs ebp | tried: same 2 forms | next: lane gap: register colouring
- 00d910b0 CBanditInRoomSample::Main DIFFER(369v369): the near-hero IsDone exit: retail hoists the g_pGameScriptInterface load and jumps into the shared DeregisterTimer tail | tried: do/while, while+break, while+explicit return, break for the remove branch | next: lane gap: exit tail merging
- 00f1b690 CArenaSpawn::Main DIFFER(361v361): retail merges the inner wait loop's return with the ArenaState return (second DeregisterTimer copy) and the top test with the loop fallthrough; ours merges the inner loop's return into the first copy | tried: while, named effect local, do/while, goto label | next: lane gap: exit tail merging
- 009122a0 CBezierCurve::GetPointOnCurve DIFFER(358v390): the 3 P1 t (1-t)^2 term: retail reloads (1-t) before the P1*3 products and spills (1-t)^2 into the dead t slot; ours keeps it on the x87 stack | tried: NRVO operators, ctor operators, named (1-t) local, by-reference scalar factor | next: try `P1 * 3.0f * t * (1-t) * (1-t)` with an explicit `float u2` temp placed mid-expression, or a Pow helper

## Skipped (10)
- 00dfca90 CBanditExtra::Main -- lane gap: the shout count is `Round(size/3 * ...)` with the bare dword `fistp` inline-asm
  rounding (GOTCHAS wave 25); the rest is an ordinary entity Main (IsDistanceBetweenThingsOver 0x00CBE3EA,
  GetSquaredDistanceBetweenThings 0x00CBE512, AddLineToConversation, parent 0x00DFC320).
- 0070c330 CAnimSet::UpdateTimePassed, 0070d390 CAnimateThing::UpdateCombinationAnimList, 00c72af0
  CAudioSource::InitializeProperties, 00646dbd CAutoPickupBar::DrawStatBarOnLeft, 009d06f0 CBankFile::OpenReadOnly,
  009d56c0 CBankFileAsync::OpenReadOnly, 009d61f0 ~CBankFileAsyncData, 00a38e50 CCRCSymbolMap::ParseBuffer, 00a39010
  CCRCSymbolMap::LoadFromFile -- not attempted within the budget (each needs a deep file/audio/anim model with no landed
  sibling); untouched for the next pass.

## Label corrections
- 0x00F35B10 is CEntityScriptBase::SetPredicate / RegisterFilter (manifest "CWorldMap::AllocateMapThingUID" propagated);
  0x00F35B30 IsDone ("CCreatureAction_TrollWhackGroundBase::Initialise" propagated).
- 0x00A14440 is C3DVector::Normalise (manifest "GetScaled"); 0x00A13C60 C3DVector::Rotate(axis, float) confirmed.
- 0x00710980 / 0x00710F70 are vector<short> / vector<CTransitionBlendAlpha>::_M_insert_overflow (manifest
  "Vector_Reallocate_*"); 0x00473023 / 0x004734B7 are CEnvironmentThemeDefVector clear / _M_allocate_and_copy
  ("CActiveFile::OnReadFinished" propagated); 0x00472EDA is CEnvironmentThemeDefVector::operator= ("AssignEnvironmentThemes").
- 0x008FCF40 CAIStateGroupBase::Terminate (not CXMVPlaybackInfo), 0x008FCFC0 GetPWorld, 0x0049D870 CWorld::GetWorldFrame,
  0x0049D810 CWorld::GetPRandomSeed, 0x007AED10 CTCBoss::GetBossPhase, 0x00946F90 / 0x00947110 WanderAroundShops shop /
  pretty-thing pickers, 0x007F6160 targeting list-clear thunk, 0x007EE540 GFGetPVillageInterface, 0x0065A053 CTCVillage
  crate-task completion (not GetItemDescription), 0x00658EA0 GetBestCrateTaskForThing, 0x00713290 CActionTalkBase current
  line text getter, 0x00A38500 CCRCSymbolMap ctor, 0x00A01AA0 CCountedPointer::Reset, 0x00CB7E50 parent script Add,
  0x00449700 GetMainPlayerNumber, 0x00487DC0 GetHeroThing.
- PDB headers: CAIStateGroup_WhisperPotionCombat is 4 high after TrapThings and misses PTarget +0x14; CActionTalkBase
  offsets are 4 lower after Messages; CAnimSet is 4 high after Anims and 8 high after InternalAlphas.
