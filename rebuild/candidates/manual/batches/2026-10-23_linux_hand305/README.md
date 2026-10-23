# Hand chunk 305 (CTCQuickAccessMenu .. CTCTavernGame): 7 of 40 byte-exact

## What matched (7)
- CTCSpecialAbilities::CancelAllAbilities (0x0072BA40): nine inline HasTC/GetTC CancelMove blocks. Each block is the plain
  `if (HasTC(t)) GetTC(t)->CancelMove()` form, with the lookup inlined through LowerBound.
- CTCSpecialAbilities::CancelAbilitiesForCutscene (0x0072BC60): the same block form. The catalog label "CancelAllAbilities"
  was propagated from 0x0072BA40.
- CTCQuickAccessMenu::GlobalUpdate (0x00625ED0, vtable slot 26):
  - Components.clear() is written as the STLport erase(begin, end) with a trivial pointer copy (memmove).
  - The inlined list and set clears match.
  - The front-end counted result is released through a DecRef/GetRefCount pair.
  - The 0xfc lookup uses the out-parameter GetInterface form.
- CTCRegionDisplay::SetCurrentAreaName, CCharString overload (0x00639BA0):
  - The CCharString overload builds a CWideString and calls the wide overload (0x00639A90).
  - The early-return test is on infoDisplay.
- CTCTavernGame::CreateGamePiece (0x005EA9B0):
  - The `T* p; GetInterface(type, &p);` out-parameter form; its miss path reads the never-written local, which shares the dead argument slot.
  - The owner is set with `P = CIntelligentPointer<CThing>(x)`.
- CTCTavernGame::DrawBetAndMoney (0x005EA400): the landed CTCBetting draw form, with these parts:
  - C2DVector temporaries of `def x screen size`.
  - A same-TU auto_inline(off) GetSubDef stand-in, which keeps the def in a register and reuses its frame word.
  - The money box drawn by an if/else on State == 5.
- CTCTavernBar::OnInitialActivate (0x007BA550): the 0x005EA9B0 model in a `for (;;)` loop, with these parts:
  - A loop-local `CThing* parent = ParentThing` (loaded before the CCharString argument is built).
  - An inline `GetDefinitionManager()` accessor, which gives the eax-to-ecx move.
  - The jug's interface lookups as an inlined STLport lower_bound (binary search) under the out-parameter form. Loading `last`
    before `first` gives retail's register assignment.

## Parked for Fable 5.1 (32)
- 00627400 CTCQuickAccessMenu::ConstructExpressionCategoryList DIFFER(737v742): fe/zero registers swapped (retail fe=ebx, zero=esi), so the else branch lacks xor ebx/esi and one release compares [eax],0 | tried: declaration orders, named fe local | next: hoist the zero constant into a named local
- 005ea650 CTCTavernGame::OnInitialActivate DIFFER(638v654): frame 0x14v0x10, GameMaster temp takes the name slot and the font temp moves to +0xc | tried: named font local (fixes frame, reads temp from frame) | next: font temporary inside the call expression with a block-scoped name
- 007af0f0 CTCRockTrollShield::OnHitAtPos DIFFER(533v533): facing dot-product pair order (Y then Z in retail) and the eax/edx swap it causes at GetSubDef | tried: nine source orders | next: an inline Dot() helper with the operands swapped
- 00773ee0 CTCShop::GetSaleItemsToPlace DIFFER(495v540): retail keeps this in ebp and tests nulls with test; ours puts the zero register in ebp and spills this | tried: the landed 0x774100 form | next: early-return restructuring so the returns do not merge
- 006e3890 CTCTalk::PlaySpeech DIFFER(531v529): VC threads the join test and moves the else block to the end; emote temps take different dead-argument slots | tried: if/else and goto forms | next: a bool join flag
- 007f5ae0 CTCTargetingBase::UpdateNearestInterestingThing DIFFER(544v544): callee-saved register rotation (creature/best/map) | tried: declaration orders | next: lane gap: register allocator
- 005eb180 CTCTavernGame::SetStateAfterInstructions DIFFER(536v536): vtable load through eax instead of edx in the 0x2f arm | tried: if/else-if chain, switch (advisor near-miss) | next: lane gap: register choice
- 0066a9e0 CTCSoundPlayer::PlayCollisionSound DIFFER(626v626): frame 0x1cv0x14, hidden CDefString returns get their own slots in retail, ours pack into dead argument slots | tried: named temps | next: block-scoped CDefString locals
- 0079b6f0 CTCSummonSpell::ResetMinionPos DIFFER(732v732): late ebp push, fchs schedule, lookup argument registers | tried: GetSubDef stand-in (739->732) | next: lane gap: scheduling
- 00731ea0 CTCSpotLight::OnAppearanceDraw DIFFER(553v575): retail uses edi=component this, esi=this-0xc, ebx as a zero register, and loads the colour bytes into cl/dl/bl/al | tried: CRGBColour B,G,R,A layout, movzx forms | next: a local base model with the draw callback at +0xc
- 007bc6fa CTCShotDefinitionProjectilePredictive::InitMovement DIFFER(514v514): one esi reload scheduled after two fstp st(0) pops (near-miss) | tried: named scale/offset/time variants, the QFE compiler | next: lane gap: x87 scheduling
- 007bca30 CTCShotDefinitionDrainLife::UpdateMovement DIFFER(539v528): fire-point subtraction keeps X on the x87 stack; the second Normalise's length-squared goes to [ebp-8] (retail [ebp-0xc]) | tried: component-local Normalise | next: C3DVector temporary with a user copy constructor
- 007bde31 CTCShotDefinitionFireballSpell::OnCollision DIFFER(474v555): dead-argument slot reuse, level in ebx, per-arm string slots (switch tail-merge) | tried: switch and if forms | next: block-scoped CCharString per arm
- 007bc15a CTCShotDefinitionMultiArrow::UpdateMovement DIFFER(571v560): sibling of 007bca30 (toTarget X forwarding plus the second Normalise length-squared slot) | tried: same as 007bca30 | next: same as 007bca30
- 007bd2d1 CTCShotDefinitionThrow::OnSubShotTrigger DIFFER(599v598): one extra frame slot, Create push order, [ecx+0xa4] address CSE | tried: GetDef stand-in (613->599), more than 10 forms | next: lane gap: slot packing
- 00803280 CTCTargetingPlayer::UpdateTargetThreatening DIFFER(548v551): frame 0xcv0x10 (creature spill/pointer slot sharing) plus one lookup register | tried: named creature local | next: block scope around the threatened pointer
- 007bf510 CTCShotDefinitionBase::OnCollision DIFFER(583v635): frame 0x34v0x18; retail homes bool/tag/key in dead param slots and keeps the thing in ebp | tried: named locals | next: temporaries inside the calls
- 007bf790 CTCShotDefinitionBase::Init DIFFER(752v762): component lookups via eax, 0x4d lookup tail-merge, constant register choice | tried: buildup/damage/accuracy store order (fixed) | next: one shared inline lookup for the two 0x4d calls
- 007bd845 CTCShotDefinitionDrainLife::InitMovement DIFFER(642v642): lenSq X*X order, one lea schedule, cross/velocity operand swaps, rand temp slot | tried: GetSubDef stand-in, operand orders, QFE compiler | next: lane gap: x87 operand order
- 007bd527 CTCShotDefinitionProjectilePredictive::OnSubShotTrigger DIFFER(672v675): [ebp-4] slot sharing (alive temp/shot) and the [ecx+0xa4] address CSE | tried: the 007bd2d1 forms | next: same as 007bd2d1
- 007bcf7f CTCShotDefinitionMultiArrow::InitMovement DIFFER(808v808): DrainLife residue (lenSq/velocity operand order) plus named-vector slot packing | tried: as 007bd845 | next: lane gap: x87 operand order
- 007be0f9 CTCShotDefinitionForcePushBall::OnCollision DIFFER(743v743): NEAR-MISS, fast-Normalise X uses `fld st(0); fmul st(3)` (retail `fld st(2); fmul st(1)`) | tried: X*=r, X=r*X, X=x*r, QFE compiler | next: lane gap: x87 operand order
- 00805600 CTCTargetingPlayer::FrameUpdate DIFFER(641v641): NEAR-MISS, ParentThing loaded into ecx (retail edx) before IsZTargetable | tried: named parent local, QFE compiler | next: lane gap: register choice
- 00639e40 CTCRegionDisplay::GlobalDraw DIFFER(958v930): text position kept in the frame by retail, dead-argument slot for GFAXToVX, draw tail-merge | tried: named position locals | next: a C2DVector temporary for the position
- 007993f0 CTCShopKeeper::WitnessedCrime DIFFER(698v698): NEAR-MISS, only the CDefPointer hidden-return slot (ours reuses the dead deed parameter slot, retail the key-0x23 word) | tried: function-scope def (0x18 frame), block-scoped def, block around the tail, const& temporary binding, inline helper returning the def | next: model the deed getter so `deed` stays live past the def construction
- 007f9960 CTCTargetingSpirit::UpdateTargetList DIFFER(626v551): frame 0x88v0x5c (pos/filter/facing temporaries unpacked); this in ebp v esi | tried: named filter locals (0xb0), filter temporaries in the call (0x88) | next: the box and sphere as call temporaries; the first facing call inside an inline helper
- 00802e00 CTCTargetingPlayer::SetPTargetedThing DIFFER(1002v990): thing parameter held in edi (retail re-reads its slot), this in esi (retail ebp), one extra frame word | tried: readable-action helper (1024), explicit blocks with a bool compare (1002) | next: IS_ALIVE inline on the intelligent pointer and the readable test as an inline bool helper
- 00803d60 CTCTargetingPlayer::UpdateProjectileTargeting DIFFER(679v699): first draft; frame 0xbcv0xa4, FOV getter on the camera base at +4, filter/LOS arguments unverified | tried: one draft | next: decode the Bind5 slots and the IsLineOfSight argument list exactly
- 007bfa90 CTCShotDefinitionBase::GetWeaponTrailGraphic DIFFER(929v926): frame 0x1cv0xc; retail keeps each def copy in esi and packs mask/key/iterator into two words | tried: FindTrail helper (977), block-scoped iterators (929), mask order (929), sequential mask (959) | next: copy the def pointer into a register local with manual AddRef/Release
- 00747da0 CTCStealth::FrameUpdate DIFFER(988v1010): retail rounds with fadd 0.5/fistp plus an integer compare of the float bits, which forces an ebp frame | tried: one full decoded draft | next: lane gap: hand-written x87 rounding helper
- 007280c0 CTCRandomAppearanceMorph::UpdateMorphSet DIFFER(807v802): the def temp copy/release register (retail edi, with the dec flags feeding jne) and loop temp scheduling | tried: raw inc/dec (805), GetDef() temp with four destructor spellings | next: the def temporary in its own block before the HasTC test
- 00727d00 CTCRandomAppearanceMorph::OnAppearanceDraw DIFFER(438v760): structural draft; the local set<CCharString>, the counted mesh exchange and the push-esp by-value argument are not modelled | tried: one draft | next: full decode of 0x00709A50's argument list

## Skipped
- 0072cf40 CTCSpecialAbilities::UseSpecialAbility: the catalog row holds this function, its jump table and about 20 small
  functions that follow it. Rule: int3 padding followed by another function body means SKIP.

## Label corrections
Each item gives the address, the catalog label, then the actual identity.
- Earlier passes:
  - 0x0079E500: CTCPhysicalShieldSpell::CancelForCutscene.
  - 0x009917E0: the speech-bank lookup.
  - 0x009C7A40: the text-entry CWideString getter.
  - 0x00686A70: CTCBase::GetWorld.
  - 0x0049D870: static CWorld::GetWorldFrame.
  - 0x00686D90: CTCBase::GetThingSearchTools.
  - 0x007F5810: CTCTargetingBase::SetPNearestInterestingThing.
  - 0x005EA950: CTCBetting::PlaySound.
  - 0x0066A920: the positional sound-event player.
  - 0x0066BD00: CTCSoundPlayer::PlayAttachedCriteria.
  - 0x0079AA90: CTCSummonSpell::GetValidSummonPos.
  - 0x00669580: GetSubDef (propagated).
  - 0x00669D30: the def effect lookup.
  - 0x004C0050: CTCGraphicAppearance::InsertAppearanceNode.
- 0x007BEDD0 "CTCPhysicsRigidBody::GetFacingVector": the folded GetTargetPosition.
- 0x00A14440 "GetScaled": C3DVector::Normalise.
- 0x0066F9B0 "SetFacingAnglesFromRHSet": the physics SetVelocity.
- 0x008685F0 "CVillageTaskCarryCrate::AssignCreatureToTask": CHitParameters::SetHitter.
- 0x008645E0 "CMainGameComponent::BeginInputLoading": the fastcall threat test.
- 0x007BEFF0, 0x00487DC0 and 0x00686D80 `_Cons_val`: GetPTargetThing, the player-character getter and a CTCBase getter.
- 0x00449970 "Catch@0040d959": CPlayerManager::GetMainPlayer.
- 0x00415FBC "GFGetPreMainMemoryUsed": the GetDefinitionManager thunk.
- 0x0076E5D0 "_Umove<EWeaponClass*>": the weapon-trail point getter.
- 0x0049D810 "DoSizeof": the world random-seed getter.
- 0x007BE92E and 0x007BE7EA "GetSubDef<CAbilityDef>": the shot sub-def getters.
- 0x00A22F80 "CMatrix3x3::MultiplyByQuaternionRotation": CRightHandedSet rotate-about-axis.
- 0x007C0FB0 "AddDependentThing": CTCShotDefinitionBase::AddHitThing.
- 0x0044C6B0 "GFGetPreMainMemoryUsed": CGameDefinitionManager::Get.
- 0x007446A0 `_Cons_val`: CTCPerceiveThing::GetPerceivedThing.
- 0x006868A0 "$E2": CTCTargetingBase::FrameUpdate.
- 0x00416359 "DrawGetEnvironment": the player-interface getter.
- 0x00445B80 "CanBeHitByMeleeStrike": CGamePlayerInterface::IsActive.
- 0x00487510 "DrawGetEnvironment": the CPlayer number getter.
- 0x0099AED0 "CCharString": CWideString().
- 0x004379D0 `_Cons_val`: the GUI target-text setter.
- 0x00686A80 and 0x00686D60 "GetDisplayEngine": CTCBase getters.
- 0x007EE540 "IS_THING_ALIVE": the village-member village getter (returns CTCVillage*).
- 0x0065914B "StartRead": the village criminal-known test.
- 0x006C5630 "GetFloorMaterial": the interface-0x51 deed-definition getter (CDefPointer by hidden return).
- 0x00A0BEB0 "DrawGetSeaLevel": the camera FOV getter.
- 0x00449960 "Catch@0040d959": the world level getter.
- 0x00485815 "GetSubDef<CAbilityDef>": the random-morph sub-def getter.
- 0x006CA950 "IsMapIncluded": the texture-morph list add.
- 0x00786580 "RemoveThingFromCoverList": CTCSkeletalMorph clear-by-tag.
- 0x004C7590 "DetachTarget": the thing dirty-flag setter.
- 0x006869D0 `_Cons_val`: the CTCBase game-data getter.
