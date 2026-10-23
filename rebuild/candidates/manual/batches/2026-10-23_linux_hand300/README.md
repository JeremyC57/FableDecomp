# Hand chunk 300 (CTCBuyableHouse .. CTCDrainLife): 11 of 40 byte-exact

## What matched (11)
All 11 are RELOCATION_MATCH under gall.sh, and each test prints PASS under g++ (tall.sh) and VC7.1 (tall.py).
- 0075d320 CTCCameraPointScripted::OnSerialseLeaf
- 0078e9c0 CTCDivineWrath::UnsheatheWeapon
- 00793ec0 CTCUnholyPower::UnsheatheWeapon (catalog says CTCDivineWrath; byte-identical twin of 0078e9c0)
- 007306c0 CTCDoor::OnInitialActivate
- 00684210 CTCCreatureOpinionOfHero::ReportAllCrimes
- 007796b0 CTCDRegionExit::FrameUpdate
- 0074a260 CTCCreatureGenerator::GetUndeadGenerationPoint
- 0074ace0 CTCCreatureGenerator::UpdateUndead (rpair ok)
- 0080d0b0 CTCDExplosion::CreateStaticExplosion (rpair ok)
- 00784260 CTCCreatureGroupBoastingCrowd::EnterCrowdState
- 007846b0 CTCCreatureGroupBoastingCrowd::OnHeroLeaveBoastingArea (EnterCrowdState is defined in the same TU; /O2 inlines only the constant-9 call, as retail does)

## Parked for Fable 5.1 (14)
- 006c14d0 CTCBuyableHouse::OnInitialActivate DIFFER(734v734): scheduling only; retail loads the AddTC `this` before the name pushes, and the 0x21 lookup key uses a different register | tried: 6 variants (decl order, scoped temps, inline AddTC) | next: hoist ParentThing into a named local before the CCharString temp
- 0075c670 CTCCameraPointDefinitionBase::OnSerialise DIFFER(715v716): only the CoordAxis copy; retail reads Up/Forward at folded [esi+0x22]/[esi+0x2e] and writes through ebp | tried: 16 variants (copy forms, member-wise, references) | next: model CoordAxis as a by-value CRightHandedSet member with a compiler-generated operator=
- 0078faf0 CTCDivineWrath::FinalCleanup DIFFER(586v586): register swap in the HitEffects loop (iterator edi, &it->Effect esi in retail) | tried: 12 loop forms (index, pointer, while-advance) | next: declare the effect reference before the iterator test
- 00795090 CTCUnholyPower::FinalCleanup DIFFER(586v586): same body as 0078faf0 (CTCUnholyPower copy) | tried: as 0078faf0 | next: whatever fixes 0078faf0 fixes this one
- 00705d40 CTCCarryable::SetAsCarriedBy DIFFER(522v526): retail hoists ParentThing into ebx for the map-number set and looks up the 0x75 OnThingCarried component before its argument | tried: 7 single-expression variants (all shrink the frame) | next: a named component local declared before the map-number statement
- 006a1f80 CTCCarrying::PutThingInCarrySlot DIFFER(664v660): frame 8 vs 0xc; the second find stores its copy into the slot home | tried: scoped locals, shared iterator | next: a second named iterator local for the second find
- 0074ec50 CTCContainerRewardHero::CreateItemPickupContainer DIFFER(572v564): empty() compare operand order, shrink-wrapped ebp push, vector dtor caching | tried: 7 variants (implicit const char* strings, empty() forms) | next: model the vector as a nested struct with empty() (the 0074ace0 lever)
- 006e0230 CTCDParticleEmitter::UpdateEmitterCameraAttachment DIFFER(frame 0x7c v 0x98): orientation copy store-sinking and the camera-location temps layout | tried: 7 variants (literal 4.0f, inline GetCameraLocation wrapper) | next: by-value CCameraLocation returned into a named local, then member copies
- 006dffb0 CTCDParticleEmitter::UpdateEmitterThingAttachment DIFFER(635v633): one hunk; the second orientation vcall must be the wrapper form, but a wrapper in both arms is cross-jumped | tried: 3 variants | next: a distinct wrapper per arm (different inline functions)
- 00683770 CTCCreatureOpinionOfHero::GetHeadPendingDeed DIFFER(876v902): deed/log esi<->edi swap, shrink-wrapped pushes, CDefPointer release copy form | tried: 8 variants (bool chain, conditional temp flag, push_front via insert) | next: an explicit CDefPointer release helper, and declaring the log before the deed
- 007b6810 CTCCrateStack::FillStack DIFFER(615v615): param-home slot reuse only (crate in position's arg slot, twist2 in direction's) | tried: 11 grades (shared crate var, scoping, decl order, inline facing helper) | next: reassign the by-value parameters themselves as locals
- 007dda30 CTCCreatureFlyInCircles::QueueSwoop DIFFER(642v642): the push_back placement copy interleaves loads; the empty tag temps are homed in end's slot | tried: copy-ctor forms, member operator+ | next: an implicit copy ctor through a by-value insert helper
- 0066fc90 CTCCoopSpirit::UpdateScreenRestrictingForces DIFFER(655v655): movsx register for the player number and force-ctor load order | tried: 11 grades (inline GetPlayerNumber, member-assigned force, inline GetHero) | next: a (angle, x, y) constructor argument order for the force vector
- 006e1620 CTCDParticleEmitter::FrameUpdate DIFFER(608v608): only the GetPosition() copy into the local; retail interleaves load X, load Y, store X, while ours stores each component right after loading it | tried: 9 grades (`DieOffTimer >= 0` fixed the jl; POD C3DVector, assignment, user copy ctor, by-value accessor, member-wise ctor, parent local, no emitter local, named interpolator) | next: the scheduling-window lever (add or remove IR earlier in the block), or a CThing::GetPosition model returning by value

## Skipped
- 0075d580 CTCCameraPointScriptedSpline::UpdateCameraParams: 16 callees, inline spline/interpolator math with no model.
- 007b57c0 CTCCarriedActionUseActivateQuest::vfunc_28: 23 distinct unmodelled callees.
- 007ef200 CTCCarriedActionUseExpression::vfunc_28: 15 callees plus vcalls, unmodelled.
- 006a16a0 CTCCarrying::RemoveThingInCarrySlot: inline slot-vector erase and CCountedPointer/def lookups, 18 callees.
- 006a1a00 CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation: CCountedPointer slot defs, CCharString temps, GetDummyObject and interpolation, 17 callees.
- 007348c0 CTCCombat::IsToBlock: 23 distinct callees. CTCCombat.h is 0xc off retail here (AlwaysBlockThings at +0x50, Blocking at +0x63).
- 00735fd0 CTCCombat::AttemptToStartParticle: 18 callees, particle def/CCharString lookups unmodelled.
- 0074e860 CTCContainerRewardHero::GetRewardItems: 24 callees, STL reward vectors unmodelled.
- 0074af50 CTCCreatureGenerator::CreateCreaturesUndead: 29 distinct unmodelled callees.
- 006cbac0 CTCCreatureNavigation::GetNextNavPosOnRoute: 11-case jump table over creature speed vslots, plus 3 navigator vcalls with 10-12 arguments. The retail layout is 4 bytes off Ego_r after PossibleNavigators.
- 006cbe80 CTCCreatureNavigation::GetNearestFleePositionBehindCover: bind2nd/ptr_fun cover search (0x6CD3B0, predicate 0x6CCF50), inline fast rsqrt and map-bounds clamps.
- 006823d0 CTCCreatureOpinionOfHero::FrameUpdate: opaque +0x344 layout, a transient list and __aullrem.
- 0080c6d0 CTCDExplosion::UpdateDynamicHitThings and 0080cd40 CTCDExplosion::UpdateHitThings: STL-heavy hit filters (0x80EC50, 0x80E670, 0x80E8E0).
- 007c36a0 CTCDrainLife::FrameUpdate: an inline list, a filter object and a search; 13 callees unmodelled.

## Label corrections
- 00793ec0 and 00795090 are CTCUnholyPower::UnsheatheWeapon / FinalCleanup (vtable 0x0126BFDC), not CTCDivineWrath.
- 0x006A23E0 is not COptimisedPrimitive::RemapMorphs (the label is propagated).
- 0x00686D30 is CTCBase::GetWorldMap, not GetDisplayEngine. Likewise 0x00686DA0 is CTCBase::GetPRandomSeed, not CViewBase::GetDisplayEngine.
- 0x0049D870 is the static GetWorldFrame (GFGetCurrentFrame), not GFGetPreMainMemoryUsed.
- Catalog row 007846b0 (len 624) covers OnHeroLeaveBoastingArea only up to 0x0078491F; 0x00784920 is the already-landed OnHeroBoast.
