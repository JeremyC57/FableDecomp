# Hand chunk 277 (C3DMeshObject .. CAIStateGroup_GoToBed): 16 of 40 byte-exact

16 RELOCATION_MATCH (src/ + test/, PASS under g++ and VC7.1), 21 parked in differ/, 2 advisor-landed rows kept in
differ/ (batch a3), 1 skipped.

## What matched (16)

**C3DPrimitive2**
- 00a8fd40 `SaveBinary` -- writes the primitive's vertex/index data to a binary stream. Shape: the `Lock()` results go into
  named locals, so each lock is called before its size is loaded.

**CAC_DummyStatic**
- 00e369b0 `Main` -- the archery-dummy contest script: it waits for hits, scores them with CalculateScoreForHit, and
  plays the reaction animation and sound. Shape: pass string literals as implicit `const char*` -> `const CCharString&`
  conversions. VC builds those temps before evaluating any other argument. Explicit `CCharString("...")` temps are built
  in argument order instead. VC CSEs `angle + 0.25` into ebp itself. Each script-macro wait is a nested loop.

**CAIBrain**
- 0088bcb0 `Update` -- picks the update zone, gates the update on the period, switches to the pending group and copies
  the output. Shape: a forceinline period helper that names the 64-bit creature offset first. The group-name
  CCharString sits in its own `{}` block, so it shares the dead `distance` argument slot.

**CAIScratchpadDef**
- 007ad300 `Transfer` -- persists the scratchpad definition. Shape: explicit `CCharString("")` temps. The int-literal
  default bound to a `const long&` builds its conversion temp before the pushes.

**CAIStateGroupBase**
- 0097dcb0 `AIAction_PerformActionTargetFaceAndWait` -- built on the landed 0x97DF60 model. Shape: a plain inline
  CBrainOutput ctor. VC calls the first use out of line and inlines the per-tick one.
- 0097ef60 `AIAction_ProjectileAttackAndStrafeToThingProximity` and 0097f3a0 `AIAction_ProjectileAttackAndStrafeToPos` --
  Shape: an inline GetInterface plus a same-TU GetSubDef stand-in, a `&Temp(...)` action, and a hand-written release.
- 0097fbe0 `AIAction_RunAwayFrom` -- Shape: the found flag is written through a `bool&` that is also MoveToPos's
  `moving` out-parameter (one shared slot). A named `radiusSquared` gives `fcomp`+`fstp`.
- 009804a0 `AIAction_FleeToThingProximity` -- Shape: the PerformAction result is discarded, a do-while `continue`s after
  MoveToPos, and the creature is named before the EPSILON local.

**CAIStateGroup_BalverineWander / FireflyWander / EnemyCreatureBase**
- 0091bd10 `BalverineWander::Update` -- Shape:
  - memberwise operator- (X, Y, Z order);
  - `!(d<100)` written first keeps both copies;
  - GetInterfaceR with lower_bound inline, length first;
  - GetCreature() called twice so the CCharString temps precede the creature load.
- 00911f80 `FireflyWander::Update` -- Shape: C3DVector difference temps, the night branch first, and GetCreature() twice
  in the light toggle.
- 0091d930 `EnemyCreatureBase::IsInRangeOfHiddenPosition` -- Shape: a user copy ctor on C3DVector, targetPos passed by
  value, and the empty test wrapping the body.

**CAIStateGroup_BanditPlayDice**
- 009285d0 `Act` -- the dice-game turn: it plays the throw animation, rolls, and hands the turn on. Shape: name the
  dice-game pointer after the creature (`CThing* player = GetCreature(); CTCDiceGame* game = DiceGame;`) for both
  CurrentPlayer assignments. That loads the target before the intelligent-pointer temp and fixes the zero/result
  register swap.

**CAIStateGroup_BoastingCrowdMember**
- 0095e900 `Update` -- the crowd member moves to a directed position near the hero and looks at it. Shape:
  - a structured `for(;;)` with first/moving flags reproduces the retail goto layout;
  - an inline GetInterface with a `bool*` flag and a named `CInterfaceMap* map` (the non-const LowerBound for the crowd,
    the const one for navigation);
  - a named `CAIBrain* brain`, so both creature loads read it;
  - the literal `0.0027777f` (0x3B360A13, not 1/360).

**CAIStateGroup_DeedReactionRoot**
- 00960760 `OnReplace` -- Shape: forceinline HasTC; a bit-field SpeechReaction (`x:7, flag:1` gives
  `test cl,cl; jns`); a member Release(); and a named `CWorldObject* world = GetPWorld()` placed before the
  ReleaseDeedSpeech pushes.

**CAIStateGroup_GoToBed**
- 0092c8a0 `FindFreeBanditBed` -- Shape: a named list head, the filter declaration placed where retail allocates it,
  operator= with a named item, and a HasTC-guarded GetTC into an uninitialised pointer.

## Parked for Fable 5.1 (21)

- 00a364e0 C3DMeshObject::GetDummyObjectPositionStatic DIFFER(703v700): the up/forward axis rotations are scheduled
  differently (retail keeps up on the x87 stack and spills forward to its own frame block; frame 0x6c v ours 0x48), and
  the normalise spills reuse the dead index slot | tried: member C3DVector rows v POD float rows (rep movsd matches),
  ctor-arg v memberwise rotate (VC reassociates the sums), null-first CCharString accessors (match) | next: lane gap:
  FPU scheduling
- 00a36a80 C3DMeshObject::GetDummyObjectPositionAnimated DIFFER(745v747): frame 0x4c v 0x78 (retail homes the
  GetPosition temp and the rotated axes in separate blocks); the mesh pointer is loaded before the CRC call; the dummy
  address is a late `add edi,4` v our folded lea | tried: one draft from the 00a364e0 model with the PDB signature
  (mesh vslot 6 animates a local C3DDummyObject) | next: solve 00a364e0 first, then reuse its axis form
- 00a8f770 C3DPrimitive2::ConvertToVBFormatRepeatingMesh DIFFER(617v616): the strength-reduced output pointer is biased
  +0x38 (retail +0x2c); everything else is identical modulo displacements | tried: statement order, ref/ptr/index
  forms, signedness | next: an output-pointer increment written at the loop top
- 00a8f9e0 C3DPrimitive2::UploadToIndexBuffer DIFFER(789v773): fill-loop register assignment (retail: indices in ebx,
  block in esi with this spilled, the bool parameter slot reused as the animated stride counter) and strip/triangle
  count scheduling | tried: max/ternary/if clamp (the if-form usage/pool matches), empty size loop kept | next: lane
  gap: register allocation
- 00a91180 C3DPrimitive2::ConvertToVBFormatAnimated DIFFER(519v519): two residues. The empty DestroyRange tag sits at
  [esp+0x20], shared with the hoisted flags&2; retail puts it in the flags argument slot [esp+0x5c]. The stride
  multiply is `imul edi` then `mov eax,edi`; retail does `mov eax,edi` then `imul eax` | tried:
  - byte vector declared before/after convertedFormat (before fixes the zero-store order);
  - init-list ctor;
  - 3 count forms;
  - tag as a temp, a named local (fixes the zero-byte slot), or a by-value param (worse)

  | next: a different inline dtor nesting for the CVertexShaderInputAnimated arrays
- 00a44f40 C3DSkeletalMorphTarget::CombineMorph DIFFER(516v525): esi/edi swapped (cursor v vector); the bone-morph CRC
  zero store is dead-eliminated; int/float stores are interleaved in the insert arm | tried: 2 forms | next: keep the
  CRC in a named member written before the insert
- 0088bf30 CAIBrain::InitStateGroups DIFFER(536v545): with the advisor's out-only `_M_insert_overflow` stand-in the
  frame matches. Residue: the disabled-name compare bool goes through ebx (retail al then bl), plus table-loop padding
  and the Dirty store | tried: 3 operator== forms on top | next: lane gap: register allocation
- 009810b0 CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage DIFFER(567v521): the by-value CDefPointer param release
  re-reads/materialises (retail `dec [ecx+4]; jne`); the guard=0 store and the nearest-compare copy are scheduled
  earlier | tried: 4 dtor forms | next: the 00979f70 by-value inline helper lever
- 0094f180 CAIStateGroup_BalverineCornered::IsRunnable DIFFER(737v754): callee-saved push placement / shared false
  exit, frame 0x3c v 0x38, the hit-info lea | tried: early return v wrapped body, named events pointer, ctor-arg
  operator- with named dx/dy | next: a single result variable with one exit
- 0093afc0 CAIStateGroup_BanditSitRoundCampfire::OnReplace DIFFER(522v522): only the GFROR13 seed store is scheduled
  before the range-global load | tried: range by value/ref, 3 random forms, a same-TU GFROR13 body, const globals |
  next: lane gap: scheduling
- 00966030 CAIStateGroup_CarryCrateAndFollow::OnReplace DIFFER(597v597): register allocation only (retail this in ebx
  and &PCrate in ebp; ours swapped) | tried: crate ref, LowerBound stand-in on/off, 4 release forms | next: lane gap:
  register allocation
- 00920a00 CAIStateGroup_CreatureSeekEnemyUnderground::Update DIFFER(953v947): frame packing 0x40 v 0x34; a bl=1
  constant in the first block (retail ebx=0); OldPosition is `add esi,0x18` v our folded displacement | tried: mask
  HasTC, forceinline operators/ctor, a Dead bit-field, perceive at the top, vector declaration order, an inline
  distance helper with a scoped velocity | next: lane gap: frame slot sharing
- 00979f70 CAIStateGroup_CreatureTakeCover::AttackTargetThing DIFFER(571v571): only the block placement of the shared
  Terminate block | tried: see track | next: lane gap: shared Terminate block placement
- 009711d0 CAIStateGroup_CreatureUseCoverBase::MoveIntoCover DIFFER(516v516): only an esi/edi swap | tried: decl order,
  for/do, exit forms, pointer IS_THING_ALIVE, self local | next: lane gap: register allocation
- 0096cb40 CAIStateGroup_EatAtTable::GetSeat DIFFER(527v553): the def pointer is in eax (retail ecx); the early exits
  share the main exit (retail keeps a pre-push exit); the seat-claim tails merge | tried: forceinline GetTC, a named
  HasTC(0xB8) thing, a def wrapper | next: duplicate the return-false tails
- 0096cd70 CAIStateGroup_EatAtTable::Update DIFFER(578v578): frame packing 0xbc v 0xc0, def in eax v ecx, a bit-24 byte
  load v shr | tried: bool* flag, copy-ctor register form, failed-flag setne, def copy/helper forms, named string block
  | next: lane gap: frame slot sharing
- 00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath DIFFER(860v860): only the inlined deque push_back
  copy of the 0x30 curve differs. Retail software-pipelines the 12 dwords (the next load comes before the previous
  store, across C3DVector boundaries); ours pairs each load/store | tried: implicit copies (rep movsd), curve copy ctor
  with an init list or body assignments (called out of line), __forceinline curve copy ctors (rep movsd) | next: an
  STLport `_Construct` / `_Copy_Construct` template helper with a by-reference value
- 009130d0 CAIStateGroup_FlyingThingBase::ExecuteFlyingPath DIFFER(976v954): register allocation only. Retail keeps
  this in edi and promotes path._M_start._M_cur to ebp; ours keeps this in ebp and spills the front curve, which moves
  the pop_front block and the isFinal constant | tried (6 drafts): named facing, GetPosition()+GetFacing() (matches),
  named radius (matches the spill), size() in v before the t loop, free null test, curve ref/pointer/front() | next:
  an explicit iterator local for the front curve
- 0094cb40 CAIStateGroup_GameMaster::GetSeat DIFFER(808v792): retail keeps the local list's header node in esi across
  calls (ours reloads it from the list object); the this-spill/frame differences follow | tried: header node built in
  the member v locally, STLport insert(end(), x) push_back shape | next: lane gap: whole-program register caching
- 00918640 CAIStateGroup_Gaze::FindFreeParticularGazePos DIFFER(581v570): the early `return 0` exits keep the vector free
  test (retail folds it); this register; the random's zero-count test is folded | tried: vector decl placement,
  functor local | next: a single exit with a result variable
- 00918b90 CAIStateGroup_Gaze::Update DIFFER(806v802): frame 0x38 v 0x34, with ebp unused (retail caches
  g_FramesPerSecond in ebp), the this-member pointer, a def release null test on the normal path, and the MoveToPos
  argument registers | tried: named angle, ctor-form 2D jitter, creature-minus-pos distance, signed frame compare |
  next: the FPS global read into a named local at the top

Advisor-landed (batch a3, kept in differ/ per the coordinator; STATUS `advisor-landed`):
- 0097ec00 CAIStateGroupBase::AIAction_MoveToPos DIFFER(854v854): only the inlined lower_bound eax copy (lane gap).
- 0097e890 CAIStateGroupBase::AIAction_MoveToPosProximity DIFFER(876v876): the same single gap.

## Skipped

- 0093b1d0 CAIStateGroup_BanditSitRoundCampfire::Update (1011 bytes): not attempted within the budget. It has about 16
  unidentified callees (0x7FEDE0, 0x4C7950, 0xA14540, 0xA141D0, 0x6CBA50, 0x97D580, 0x6A1540, 0x8CC370, 0x97DB70,
  0x662EF0, 0x8D7CA0, 0x99EFB0, 0x8AB380/0x8AB9D0, 0x97D8B0, 0x93AD70), an inline CModeSit-derived `new` with CCharString
  name and CRC, and conditional-destroy flags ([esp+0x20] bits 1/2). The row holds one function.

## Label corrections

Propagated manifest/catalog labels found to be wrong (address: catalog label -> actual):
- 0x693EF0 -> ~CCreatureActionBase
- 0x6A4D00, 0x4C73D0: DoSizeof -> GetPosition (CThingCreatureBase / CThing)
- 0x88B8F0 -> CAIBrain::SetBrainOutput
- 0x8FCFA0, 0x8FCFC0 -> GetPWorld; 0x8FCF40 -> Terminate
- 0x8FCFD0: FinishAction@CCreatureAction_WaspQueenDeath (egor, propagated) -> GetPWorldSeed
- 0x62E900 -> ~CCombatActionBase; 0x87C390 -> SetUser; 0x6A8AF0 -> the gaze free test
- 0x97E270, 0x97E2D0 -> state-group navigation helpers (SetNavigationIgnoreThing / SetNavigationSpeedScale)
- 0xA46B50 -> the morph vector insert
- 0x692620: DrawGetEnvironment -> the action-ID getter
- 0x6A11F0 -> GetThingInCarrySlot
- 0x88CC20 / 0x88D130 -> CVectorMap pair `_M_insert_overflow` / sort
- 0xA14440: GetScaled -> C3DVector::Normalise
- 0x6CB610: operator== -> IsPosNavLineOfSightToPos
- 0x810F40: DrawGetEnvironment -> CEventHitBy hit-info accessor
- 0x8645E0: BeginInputLoading -> CanCreatureSeeCreature
- 0x49D810: DoSizeof -> CWorld random-seed accessor
- 0x49D870: GFGetPreMainMemoryUsed -> GetWorldFrame / GFGetCurrentFrame
- 0x7EE540: IS_THING_ALIVE -> the region accessor
- 0x94B3A0 / 0x8AB320: CFileInfo / QuadricOptimiser names -> CCountedPointer<CModeSitBase> ctor / dtor
- 0x8ABAA0: `_Dest_val` morph -> ~CCreatureAction_SitDown
- 0xAB37B0 / 0xAB3B10: PrepareForLoad / HoverHornetQueen -> CVertexFormat::GetVertexSize / CVertexShaderInput ctor
- 0x792240: SortTreeRecursively -> a std::list destructor
- 0x94D430: AssignCreatureToTask -> the CTCTavernGameCoinGolf game-master setter
- 0x920FF0 / 0x66E630 (FUN_) -> vector<C3DVector>::reserve / push_back
- 0x7448F0: ~CTCSpecialAbilities (egor) -> CTCPerceiveThing::GetExtendedSightRadius
- 0x485F3A: forge-autoname "std::deque::insert" -> vector<unsigned char>::insert(pos, n, value) (the byte-vector resize)
- 0x913D60: forge-autoname "PathBuilder_AppendBezierCurveNode" -> deque<CBezierCurve>::_M_push_back_aux_v
- 0x9137F0: forge-autoname "AllocateCurveBuffer" -> deque<CBezierCurve>::_M_initialize_map
