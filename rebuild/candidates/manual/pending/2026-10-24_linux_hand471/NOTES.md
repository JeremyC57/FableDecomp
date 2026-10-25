# Hand chunk 471 (NHeroInformationScreens::CBase .. _Push_heap): 22 of 40 byte-exact

All 22 are RELOCATION_MATCH under gall.sh. Every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).
calls.py shows every call target equal to retail's.

## What matched (22)

- **STLport heap algorithms (12 rows).** The catalog's Dinkumware `_Adjust_heap` / `_Push_heap` / `_Pop_heap` spellings
  are STLport 4.6 `__adjust_heap` / `__push_heap` / `__pop_heap` templates in `_STL`.
  - **`__adjust_heap` rows:**
    - 004ba180: quest persistence;
    - 009b8220: def-class registry, with an unsigned key (`jae`);
    - 009e6db0: symbol map, CCompareString;
    - 00a33a70: pair<unsigned long,C3DMeshVolume>;
    - 00a71220: pair<long,C3DMeshVolume>;
    - 00a71370: pair<CCharString,C3DMeshVolume>;
    - 00af3290: registered PSC table;
    - 00cbb3b0: active-entity scripts by an unsigned __int64 key;
    - 0080ad90: targeting candidates.
  - **`__push_heap` rows:** 00a708e0 and 00bed880.
  - **`__pop_heap`:** 0080b210.
  - **Shape:**
    - `topIndex` lives in the `push ecx` slot (or `first` and `topIndex` in the `sub esp,8` slots).
    - The compare is `comp(first[second], first[second-1])`.
    - `(hole - 1) / 2` is signed int division (`cdq ; sub ; sar`).
    - In 00cbb3b0, val's count block stays in esi because no out-of-line call sees val.
  - **0080ad90 needed a same-TU `__declspec(noinline)` stand-in body for the comparator's operator().**
    - The stand-in does not capture `this`.
    - Only then does VC7.1 software-pipeline the by-value comparator copy the way retail does.
- **STLport `copy_if` / `__find_if` (007b7140, plus 007f7ef0 parked).**
  - copy_if appends the crate-stack markers of a type to a vector through a back_insert_iterator.
  - Shape:
    - the by-value predicate is `__forceinline`;
    - `GetPItem` is its own statement before the `GetSubDef(&def)` push;
    - same-TU noinline stand-ins for GetSubDef and `_M_insert_overflow` keep the def holder in the dead `dest` slot.
- **NUISystem::CSpellContainer::AddChildDynamically (00545f00).**
  - Copies the per-state position and colour of the child in the slot, then inserts at begin() + index.
  - Shape: the state map is held in a local; the right-hand side is evaluated before the new child's operator[].
- **NUISystem::CSwappingStateComponent::Update (00547380).**
  - Cycles the swap states once each state's dwell time runs out.
  - Shape:
    - the pair predicate is a 4-byte by-value functor going to the out-of-line `__find_if`;
    - the empty iterator tag needs a user default constructor (otherwise `mov byte [esp+14h],0`);
    - the next index is `index < size()-1 ? index+1 : 0`.
- **NTCScriptedControl::CActionMoveTo::Update (00714900).**
  - Moves the scripted creature towards a position or a target thing.
  - Shape:
    - the action-point proximity ternary is a float local (x87 `fld`/`fstp`, not integer moves);
    - the nav result is read before the unrelated Active store;
    - squared length written X*X+Y*Y+Z*Z over the output member is emitted Z, Y, X;
    - the target thing's position comes through an inline reference accessor.
- **NTCScriptedControl::CActionTalkBase::OnCleanup (00714e00).**
  - Ends a talk: un-look, restore the conversation attitude, and let mode 27 linger.
  - Shape: the parent thing is a local before the second GetThing() compare; the linger value is converted before GetCurrentMode() is re-fetched.
- **NTCScriptedControl::CActionPickUpGenericBox::Update (007153b0).**
  - Hands the creature a pick-up-generic-box action and finishes when it is done.
  - Shape:
    - the action is a temporary bound to SetCurrentAction's const reference;
    - there are same-TU GetSubDef / LowerBound stand-ins, following the model of the landed PickUpGoods::PerformAction 0x00848730;
    - the box is taken into a local before GetSubDef.
- **libpk PKCTRArray::ResizeAllocation (00c32950, /EHsc).**
  - Shape: memcpy is inlined (`rep movsd` + `rep movsb`); the capacity sum is formed after the free.
- **libpk PKCBufferedReader (00c4a1a0 GetAvailableReadSize, 00c4ac70 ReturnBuffer, /EHsc).**
  - Every check is the libpk assertion macro: a block-scoped log line goes to the sink, then the trace temporary.
  - In 00c4a1a0 the alignment test is unsigned (`div`) and the overrun tests are signed.
  - In 00c4ac70 the ring walk re-reads the slot count, and the walk ends by setting the index to the count, not by `break`.
- **libpk CReverbProcessor::ProcessInput (00c69c60, descriptive name).**
  - Feeds one block into the reverb delay line:
    - fades out the tail on switch-off, then resets both gains;
    - runs the input gain and the in-place line gain (blend or apply), split at the wrap;
    - fades in on switch-on, then advances the write position.
  - Shape: `(float)n` is written at every use in a ramp. VC7.1 then shares one conversion slot and keeps two `fdiv`s; a float local becomes a reciprocal multiply.

## Parked for Fable 5.1 (18)

- 00b84470 CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer DIFFER(270v270): VC7.1 loads eax=2 once and groups the six RenderPass=2 stores (lists 0,1,11,12,2,3); retail keeps source order and clears eax after the float stores | tried: source-order stores under RTM and QFE 4035 | next: lane gap: store scheduling (compiler build)
- 00b17bd0 NParticleEngine::CPSCAttractor::InterpolateTo DIFFER(930v926): retail converts the lerped fall-off with a bare `fstp ; fld ; fistp dword` helper; the C++ cast calls __ftol2, moves this to esi and spills the 1-t local | tried: cast reference body (1 grade) | next: retail_asm candidate: minimal `__asm fistp` body with a `// Retail-asm:` line, as in the landed GetRandomRadius 0x00B102C0
- 00b1bc80 NParticleEngine::CPSCDecalRenderer::GetComponentCost DIFFER(261v280): the two-decimal rounding is a bare `fld ; fistp dword [esp] ; fild` in retail; C++ casts call __ftol2 | tried: cast reference body (1 grade) | next: retail_asm candidate: minimal `__asm fistp` with a `// Retail-asm:` line
- 00b131e0 NParticleEngine::CPSCEmitterGeneric::DatabaseCopy DIFFER(850v839): retail keeps each destination bit-field word in a register across the same-base stores and forwards the zero point's constants; VC7.1 reloads the word for every field | tried: pointer-local source with field order from the masks; __true_type user ctor + signed GetSize + hoisted zero point (best) | next: lane gap: store-to-load forwarding
- 00b17410 NParticleEngine::CPSCOrbit::CollectUpdateParameters DIFFER(459v459): retail gives the second descriptor temporary its own slot (`sub esp,28h`); VC7.1 reuses one descriptor slot (`sub esp,1Ch`) | tried: one reused key local, three key locals, block scopes | next: lane gap: frame-slot packing
- 00b094b0 NParticleEngine::CPSCSingleSprite::Initialise DIFFER(729v732): retail builds the crossed-sprite loop's zero C3DVector temporary once in the preheader; VC7.1 rebuilds it inside the loop with the argument pushes | tried: NeedsPrim helper + trail store order from a compile-only search (1 grade) | next: lane gap: loop-invariant hoisting
- 00650e98 NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick::DrawAt DIFFER(987v987): the caption X sum loads offset.X before pos.X (retail pos.X first), and the tick sprite's four zero UV stores land after the box stores (retail before) | tried: 12 caption operand/alias/operator forms, member-wise C2DBoxF operator=, named GuiDef local, reference sprite-ctor parameters, 120+ compile-only scorer variants (1 grade) | next: named UV C2DVector locals declared before the box assignment; else lane gap: x87 operand / store scheduling
- 00712900 NTCScriptedControl::CActionPlayAnimation::Update DIFFER(362v362): the played-path creature read is `mov eax,[esi+4] ; mov ecx,[eax+4]`; retail has `mov ecx,[esi+4] ; mov ecx,[ecx+4]` | tried: creature local + not-played-first block order, inline GetCreature accessor in the new-expressions, nested queue/set-current form | next: write that one test as `PTCScript->ParentThing->IsPerformingAnyAction()`
- 007175d0 NTCScriptedControl::CActionSummonerLightningAttackTarget::Update DIFFER(318v318): retail gives the attack-start block's creature ebx (pushed at the head); VC7.1 reuses ebp and shrink-wraps the ebx push into that block | tried: orb action as a temporary bound to a const reference, swapped declaration order in the second block, finished check as a forceinline helper | next: lane gap: register allocation
- 00717710 NTCScriptedControl::CActionTalkBase::OnActivate DIFFER(522v522): CModeConversation argument registers (retail ecx=attitude, edx=parent; ours edx/ecx) and the final target block (retail edx/eax/edx + `lea eax`; ours eax/edx/eax + `lea ecx`) | tried: map and creature offset fixes, new-expression argument locals + tail, single-condition tail, RTM and QFE | next: lane gap: register allocation
- 007bcc40 NThingShot::CTCShotDefinitionDragonFireball::InitMovement DIFFER(807v807): the target position gets the delta's frame slot (speed/time words swapped), the velocity's squared length sums Y,Z,X (retail X,Y,Z), and the radius scaling loads the factor first | tried: six length-sum orders, direct-member lengths, IS_THING_ALIVE by const&, forced-inline GetRandomInRange, named accuracy / RH-set / acceleration locals, permuted lead-block declarations, block-scoped delta (1 grade + frame-insensitive compile-only scorer) | next: lane gap: frame-slot assignment + x87 operand canonicalisation
- 007bee40 NThingShot::CTCShotDefinitionThrow::UpdateOrientation DIFFER(344v344): retail squares forward as Z,X,Y (ours Z,Y,X), and the up vector's Y term is forward.Z * side.X (ours side.X * forward.Z) | tried: forceinline vector helpers, member-wise displacement, retail operand order for the cross term and the forward length, RTM and QFE | next: lane gap: x87 operand canonicalisation
- 0055c6f0 NUISystem::CMouseCursor::ProcessEvent DIFFER(276v276): VC7.1 overlays the four GetRenderTargetDimensions temporaries and the movement local in one 8-byte slot (`sub esp,10h`); retail gives each its own slot (`sub esp,30h`) | tried: inline dims helper + movement reference local, inline display-manager accessor + front-end pointer local, inline dtor on the extents temporary, flag probes, RTM and QFE | next: lane gap: frame-slot packing
- 0053e9a0 NUISystem::CScrollingViewport::ProcessEvent DIFFER(587v578): retail fetches the GetPosition cursor before the GetSize one in each dead far-corner pair and keeps mouse.X/Y in ebx/ebp; ours fetches both cursors first and spills; retail's frame is 0x7C in first-use order, ours 0x8C | tried: free operator+ and member operator+ for the dead cursor corner | next: fetch each GetPosition() cursor into a named local before the GetSize() call; else lane gap: frame layout
- 0054e6f0 NUISystem::CViewport::Draw DIFFER(690v644): far-corner Y (retail `fld pos.Y ; fadd st(1) ; fstp st(0)`, ours `fadd [pos.Y]`), screen-size slots split, and the primitive store order (retail copies the alpha-mask bank pointer first, then stores every member in one block) | tried: box local, far-corner vector (best), inline ctor (pushed, not inlined), __forceinline ctor (both box temporaries materialised) | next: a primitive ctor that takes the bank pointer first and is inlined under `optimize("s")`
- 007f7ef0 _STL::__find_if<CThing**, unary_negate<CIsTriggerActivated> > DIFFER(430v432): retail keeps `first` in ebp (moved before `push esi ; push edi`) and the trigger local in edi; VC7.1 picks edi/ebp | tried: plain form, ternary GetInterface form, reference predicate argument | next: lane gap: register allocation
- 00a10a60 WordWrap_String::TokenizeW DIFFER(301v281): IsEastAsianChar / IsNonBeginningChar / IsNonEndingChar take the character in ax with no push (LTCG custom convention) | tried: reference body (1 grade) | next: lane gap: LTCG register-argument callees
- 00a10b80 WordWrap_String::TokenizeA DIFFER(476v450): the same ax-argument LTCG callees as TokenizeW | tried: reference body (1 grade) | next: lane gap: LTCG register-argument callees

## Skipped

None. No row covers two functions.

## Label corrections

- **Vtable-slot rows (catalog `vfunc_N` → real name):**
  - 00b84470: the catalog says `NHeroInformationScreens::CBase::CBase`. It is the CEngineSubPrimitiveRenderer constructor: 13 render lists with RenderPass stores, and it calls the CEngineComponent ctor 0x00B59710, whose manifest label is `CNavigationLayer`.
  - 00b17bd0: `CPSCAttractor::vfunc_14` is CPSCAttractor::InterpolateTo (slot 14, as CPSCSpline::InterpolateTo in FableWin.pdb).
  - 00b1bc80: `CPSCDecalRenderer::vfunc_20` is CPSCDecalRenderer::GetComponentCost.
  - 00b131e0: `CPSCEmitterGeneric::vfunc_18` is CPSCEmitterGeneric::DatabaseCopy.
  - 00b17410: `CPSCOrbit::vfunc_10` is CPSCOrbit::CollectUpdateParameters.
  - 00b094b0: `CPSCSingleSprite::vfunc_28` is CPSCSingleSprite::Initialise.
  - 00650e98: `CQuestInfoElementTick::vfunc_0` is CQuestInfoElementTick::DrawAt (Ego_r.pdb).
  - 007bcc40: `CTCShotDefinitionDragonFireball::vfunc_33` is InitMovement (Ego_r.pdb).
  - 007bee40: `CTCShotDefinitionThrow::vfunc_36` is CTCShotDefinitionThrow::UpdateOrientation.
  - 00714900: `CActionMoveTo::vfunc_1` is CActionMoveTo::Update.
  - 007153b0: `CActionPickUpGenericBox::vfunc_1` is CActionPickUpGenericBox::Update.
  - 00712900: `CActionPlayAnimation::vfunc_1` is CActionPlayAnimation::Update.
  - 007175d0: `CActionSummonerLightningAttackTarget::vfunc_1` is ...::Update.
  - 00714e00: `CActionTalkBase::vfunc_5` is CActionTalkBase::OnCleanup.
  - 00717710: `CActionTalkBase::vfunc_2` is CActionTalkBase::OnActivate.
  - 0055c6f0: `CMouseCursor::vfunc_1` is CMouseCursor::ProcessEvent.
  - 0053e9a0: `CScrollingViewport::vfunc_1` is CScrollingViewport::ProcessEvent.
  - 0054e6f0: `CScrollingViewport::vfunc_2` is CViewport::Draw. CScrollingViewport inherits it in slot 2.
  - 00545f00: `CSpellContainer::vfunc_142` is CSpellContainer::AddChildDynamically.
  - 00547380: `CSwappingStateComponent::vfunc_1` is CSwappingStateComponent::Update.
- **Other row labels:**
  - 00c69c60: the catalog says `NScript::CQ_GuildTrainingSkillScript::UpdateAnimation`, which is propagated. The body is libpk's software reverb, CReverbProcessor::ProcessInput (vtable 0x012B9594, no PDB/RTTI; descriptive name).
  - **STLport algorithm spellings:** the catalog's Dinkumware spellings are STLport:
    - `_Adjust_heap` / `_Push_heap` / `_Pop_heap` are `_STL::__adjust_heap` / `__push_heap` / `__pop_heap`;
    - `_Copy_if` 007b7140 is `_STL::copy_if`;
    - `_Find_if` 007f7ef0 is `_STL::__find_if`.
  - **Wrong element types in the catalog:**
    - 00a71220 holds pair<long,C3DMeshVolume>, not pair<long,CCountedPointer<CNavConnectorNode>>;
    - 00a33a70 holds pair<unsigned long,C3DMeshVolume>, not pair<unsigned long,CCountedPointer<CFaction>>;
    - 00bed880 holds pair<long,CCompositeSurfaceInfo>, not pair<unsigned long,CCountedPointer<CFaction>>.
- **Callee manifest labels (wrong, propagated):**
  - 0x00843700: CCreatureAction_PickUpGenericBox ctor (manifest: AddRealObjectToInventory).
  - 0x00693EF0: ~CCreatureActionBase (manifest: ~CPhysicsMeshInfo).
  - 0x0062E900: combat action base dtor (manifest: ~CTCLightningOrb).
  - 0x0073A6E0: GetCurrentMode (manifest: CountReferences).
  - 0x0073A8A0: HasMode.
  - 0x00547790: STLport `__find_if`.
  - 0x00A14440: C3DVector::Normalise (manifest: GetScaled).
  - 0x00692620: GetActionID (manifest: DrawGetEnvironment).
  - 0x00713630: CTCScriptedControl::CancelAllActions (manifest: CleanupAllActions; descriptive name).
  - 0x006CB960: SetNavigationInput (manifest: SetCoordAxis).
  - 0x006CBE30: SetThingToIgnore (manifest: AssignCreatureToTask).
  - 0x00C744E0: CDspGain::Reset (manifest: Init; descriptive name).
  - 0x00C74D30: CDspGain::Blend (manifest: Animation_InterpolateHelper; descriptive name).
- **Already landed under wrong names:**
  - 0x007BEDD0 is landed as CTCPhysicsRigidBody::GetFacingVector. It is the shot definition's target-position accessor: it reads this+0x51 and is called from the InitMovement rows.
  - 0x00C6A5D0 is landed as CEditControlCentre::GetCurrentMask. It is CReverbProcessor::Process, the per-block driver that calls 00c69c60 (descriptive name).
  - 0x00BEDD70 (landed `__adjust_heap`, the caller of 00bed880): its model is correctly pair<long,CCompositeSurfaceInfo>, but its catalog label and file name still say `pair<long,CCountedPointer<CNavConnectorNode>>`.
