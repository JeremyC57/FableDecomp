# Hand chunk 234 (CTCPhysicsRigidBody .. CTCTavernGame): 22 of 40 byte-exact

Compiler: lane default VC7.1 RTM cl 13.10.3077 for every row (no QFE row). 22 matched, 16 parked, 2 skipped.

Models: most rows use a local model of `this`, never a pad-only stand-in. Each source's `// Layout:` comment
gives the retail offsets that differ from Ego_r.pdb: retail STLport vectors and maps are 12 bytes (the PDB has
16) and lists are 4 or 8 bytes. That puts CTCTavernGame 0x1C lower, CTCShop Alarm at 0x54 rather than 0x60,
CTCSpecialAbilities Creature at 0x18 rather than 0x20, and CThing PTCDriver/PhysicsTC 0x10 lower. Two headers
cannot model `this` at all. engine/CTCPhysicsStandard.h is a flattened pad model that types +0x0c as `long`
where retail has CTCPhysicsBase::Position C3DVector. engine/CTCShotDefinitionProjectile.h is flattened in the
same way and does not match the rows' real owners. engine/CTCTavernGame.h has the PDB offsets, but the landed
SetStateAfterTime (0x005E9B70) proves the 0x1C shift.

## What matched (22)
- **CTCPhysicsStandard** (labelled CTCPhysicsRigidBody) 008e5370 GetPossibleCollidedWithArea: the integer box the body can reach this frame (position grown by radius+1 and stretched along XY velocity). Named float locals minX/maxX/minY/maxY give retail's all-floats-then-ftol order; the box comes back through a `__forceinline` memberwise copy (no NRVO).
- **CTCQuestCompletionUI** 005e4740 Activate: opens the completion screen. HasTC has to return the raw masked `unsigned long` (`test ch,1`), because a `!= 0` bool gives `shr`. The out-param GetInterface keeps the key in the frame word, and a function-scope zero card pointer doubles as the zero of the type-3 call.
- **CTCRegionFollower** 006aebf0 IsCreatureAllowedInRegionAt: def pointers are CDefPointer locals and the creature def is read twice through an inline by-value getter. GetSubDef is a same-TU stand-in, and a `long&` count bump keeps the load after the store.
- **CTCShop** 00773c90 SetAlarm: starts or stops the looping alarm sample. The symbol string is an implicit conversion temporary homed in the dead `on` slot, and the bank comes through an inline getter, because a direct member read hoists the load. 00774b40 GetFreeSaleItemPlaces: an indexed loop over the re-read size returns its result vector through the copy ctor. Vector copy, push_back and _M_insert_overflow are same-TU stand-ins under `auto_inline(off)`, standing in for retail's LTCG non-escape.
- **CTCShotDefinitionBase** 007c2480 UpdateEffects: the movement test is `!(lengthSquared > eps*eps)`, which gives `fcompp ; test ah,5 ; jnp`. The driver pointer is zero-preloaded and filled under a raw-masked HasTC.
- **CEngineInternalPrimitive2DAnimatedMesh / ...2DMorphedAnimatedMesh** (labelled CTCShotDefinitionBase ctor) 00bd94e0, 00bd9710: ctors built through the inline (type, flags) base constructor, the chunk 211 sibling models.
- **CTCHeroStats** (labelled ~CTCShotDefinitionProjectile) 005814e5 ~CTCHeroStats: `#pragma optimize("s", on)`. The class is `__declspec(novtable)` so the vtable is never re-stored. The CDefPointer release goes through a local copy of the pointer, because releasing through the member reloads it.
- **CTCSpecialAbilities** 0072b160 SetupTurncoatSpell: nested tests with every exit on the shared `xor al,al`, and the lookup keys share one frame word. 0072bec0 ResumeAbilitiesForCutscene: four HasTC/GetTC statements, each re-reading ParentThing. 0072cd00 SetupDivineWrath and 0072ce20 SetupUnholyPower: the base owns the inline intelligent-pointer member (destroyed by ~CCombatActionBase), and the cast action is an argument temporary of the const& SetCurrentAction.
- **CTCStealth** 00747330 InitialiseSneakSpeed: the filter's base needs an inline *virtual* dtor to get the 0x0122FFCC vtable restore. Names and filter sit in a block that closes after the lookup. 00747660 GetVisibilityMultiplier: the out-param GetInterface keeps the component in eax, where HasTC+GetTC puts it in ecx. 00747890 GetNumberOfAttackersOverMaximum: a local vector stands in for a null argument, with an inline push_back and an InsertGrow stand-in.
- **CTCSummonSpell** 0079a6e0 ctor: modelled on the landed CTCForcePushPower ctor. Init-list members share the ebx zero, and `FadeInTimer = CurFadeInTimer = x` stores CurFadeInTimer first.
- **CTCTargetingAI** 007f69b0 FrameUpdate (labelled vfunc_9): an `if (!empty())` guard plus an inline `bool IS_THING_ALIVE` (nested ifs) fixes the block order. An open-coded `!thing || dead` moves the erase block. Uses the inline STLport list erase.
- **CTCTavernBar** 007ba3c0 GetRandomBarSlot: an uninitialised out pointer from the out-param GetInterface, with WorldRandomRange inline. An unused default CCharString declared first is needed for the frame.
- **CTCTavernGame** 005e9c50 CallFuncAfterYesNo and 005e9d60 CallFuncAfterOK: graded `--flags /GS` (`// Flags: /GS`). The empty names are implicit L"" temporaries, and the CGameEvent block local takes the /GS frame. 005eb3a0 GetTextWithNarrator: a zero-initialised out word for the talk component, with the voice group passed by value into the dead textId slot.

## Parked for Fable 5.1 (16)
- 007ee3d0 CTCVillageMember::OnSerialise DIFFER(244v244): ours hoists `lea` of &UID above the GetPItem null-test branch; retail recomputes it in each arm and reuses esi | tried: pointer-member / local-UID / getter forms | next: lane gap: scheduling (CSE hoist)
- 00800a60 CTCPreCalculatedNavigationRoute::OnSerialise DIFFER(297v301): same &UID hoist as 007ee3d0; &PThing is not CSE'd the same way | tried: as 007ee3d0 | next: solve 007ee3d0 first, same source shape
- 00787e12 CTCProjectileTrap::UpdateTrap DIFFER(280v279): the conversion temporary gets its own slot [ebp-0Ch] (sub esp,0Ch); retail overlaps it with the hero local (push ecx x2) | tried: block-scoped hero / named temp / inline getter | next: lane gap: frame packing
- 00762e10 CTCShapeManager::EditUpdateDisplacement DIFFER(326v311): retail caches the point vector's _M_start in ebx across the calls and stores points in place (fstp to the element, cached end); ours re-reads and copies through a temp | tried: stand-in vector ops / reference loop / pointer loop | next: lane gap: LTCG non-escape of the local vector
- 007729a0 CTCShop::CalcIsStallLike DIFFER(319v319): in the 0x21 list loop retail has the node in edi and &item in esi; ours has the reverse | tried: node pointer / item reference / STLport iterator / inline IS_THING_ALIVE | next: lane gap: register allocation
- 00775b40 CTCShop::FrameUpdate DIFFER(274v274): register swap with `this` in esi and the temporaries in edi in retail, reversed in ours | tried: named temps / getter forms | next: lane gap: register allocation
- 007be3e0 CTCShotDefinitionForcePushBall::InitAttachedEffects DIFFER(246v256): retail overlaps the PEffect conversion temp [ebp-0Ch] with the later emitter word and holds a zero in edi; ours uses immediates and a separate slot | tried: inline setter / block scopes / size TU (`optimize y off, s on`) | next: lane gap: frame packing + zero-register CSE
- 007be4e0 CTCShotDefinitionDrainLife::InitAttachedEffects DIFFER(246v256): byte twin of 007be3e0 | tried: same source | next: same as 007be3e0
- 007bbb3b CTCShotDefinitionProjectile::InitMovement DIFFER(268v268): retail gives the scattered-direction and CRightHandedSet temps their own slots (frame 58h); ours shares them (frame 40h), and the Z term is one step late | tried: named locals / block scopes | next: lane gap: frame packing
- 007bc8fc CTCShotDefinitionTearInGround::InitMovement DIFFER(308v308): `mov ecx,[edi+4]` (ParentThing) is loaded before the name-temp lea; retail loads it after | tried: implicit / named / getter forms | next: lane gap: scheduling
- 007eed20 CTCSmashableBase::Smash DIFFER(275v275): at the last store retail has the 0x4E component in eax and LastWorldFrameHit in edx; ours is reversed | tried: ternary / named component / FindEntry / getter | next: lane gap: register allocation
- 0072af30 CTCSpecialAbilities::IsAbleToUseAnyAbility DIFFER(256v256): the IsUsableBy argument is pushed before the weapon component moves into ecx; retail loads the component first and pushes through eax | tried: ternary / FindEntry / named entry / getter / out-param | next: lane gap: scheduling
- 0072caa0 CTCSpecialAbilities::SetupAssassinRush DIFFER(291v252): VC merges the 0xA9/0xA8 tests into `test ch,3` and keeps 8 in ebx, and the action blocks' set+dtor tails do not cross-jump | tried: nested / early-return / separate HasTC forms | next: per-test raw word reads in separate statements; else lane gap: tail merge
- 007471f0 CTCStealth::DrawPreparePrimitivesForRendering DIFFER(308v308): ours homes the ToWideString temp in the dead viewport slot (frame 40h); retail gives it its own word (frame 44h) | tried: block scopes / named wide string | next: lane gap: frame packing
- 00747c70 CTCStealth::UpdateDangerInfo DIFFER(303v303): retail keeps begin/end in eax/ecx, &DangerThings in ebp and the predicate in ebx; ours CSEs end into esi and swaps ebx/ebp | tried: inline STLport remove_if model (1 attempt) | next: out-of-line remove_if stand-in
- 007f68b0 CTCTargetingAI::GetPBestCreatureForInteractionType DIFFER(256v255): the 0x34 lookup key shares the perception pointer's word (retail uses the result word), and the box arithmetic is lea vs add | tried: declaration-order variants | next: lane gap: frame packing

## Skipped
- 005eaf46 CTCTavernGame::SetStateAfterYesNoAlt: a mid-function row. The real entry is 0x005EAF30 (SetStateAfterYesNo).
- 005eb08d CTCTavernGame::SetStateAfterOK: a mid-function row. The real entry is 0x005EB080.

## Label corrections
- 008e5370 CTCPhysicsRigidBody::GetPossibleCollidedWithArea -> CTCPhysicsStandard::GetPossibleCollidedWithArea (CTCPhysicsStandard vtable slot 98)
- 007ee3d0 CTCPreCalculatedNavigationRoute::OnSerialise -> CTCVillageMember::OnSerialise
- 00775b40 CTCShop::vfunc_9 -> CTCShop::FrameUpdate
- 00bd94e0 CTCShotDefinitionBase ctor -> CEngineInternalPrimitive2DAnimatedMesh::CEngineInternalPrimitive2DAnimatedMesh
- 00bd9710 CTCShotDefinitionBase ctor -> CEngineInternalPrimitive2DMorphedAnimatedMesh::CEngineInternalPrimitive2DMorphedAnimatedMesh
- 007be3e0 CTCShotDefinitionDrainLife::InitAttachedEffects -> CTCShotDefinitionForcePushBall::InitAttachedEffects (007be4e0 is the real DrainLife one)
- 005814e5 CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile -> CTCHeroStats::~CTCHeroStats
- 0072ce20 CTCSpecialAbilities::SetupDivineWrath -> CTCSpecialAbilities::SetupUnholyPower
- 007471f0 CTCStealth::vfunc_11 -> CTCStealth::DrawPreparePrimitivesForRendering
- 007f69b0 CTCTargetingAI::vfunc_9 -> CTCTargetingAI::FrameUpdate
- 005eaf46 / 005eb08d: mid-function rows (entries 0x005EAF30 / 0x005EB080)
