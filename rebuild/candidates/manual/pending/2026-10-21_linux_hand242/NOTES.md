# Hand chunk 242 (CChangingStateComponent .. CCreatureAction_DragonRoar): 33 of 40 byte-exact

Compiler: lane default RTM cl 13.10.3077 for every row (no QFE toolset needed). All 33 src/ files grade
MATCH/RELOCATION_MATCH in gall.sh; all 33 tests print PASS under tall.sh (g++) and tall.py (VC7.1).

## What matched (33)
- NUISystem::CChangingStateComponent::UpdateStateChange 0x0052CD20 -- state-change bookkeeping and child walk; inline STLport list front/pop_front, first try.
- NUISystem::CComponent::Draw 0x00530260 -- draws the component and its children per layer; `this == child->GetParent()` operand order and two bool locals for the layer flags.
- NUISystem::CComponent::Initialise 0x005339B0 -- resets position/zoom/colour; inline C2DVector::Clear keeps the repeated PPositionParent compare; `X = Y = 1.0f` stores Y first.
- NUISystem::CComponentContainer::Initialise 0x005461D0 / NUISystem::CTextContainer::Initialise 0x00546D90 -- stacks children vertically; the loop is a guarded do-while with the operator[] key declared inside the guard, so the key's zero store sinks into the preheader.
- NUISystem::CClickable::ProcessEvent 0x0055AD60 -- mouse-event switch; jump table, case blocks in retail order; `this` is the event-receiver base at +4.
- CCreatureActionBase copy ctor 0x0062EBC0 -- explicit member-wise copy; C3DVector needs a user copy ctor (no lea copies).
- CCreatureActionBase ctor 0x00693C70 -- STLport `_Vector_base` + `_STLP_alloc_proxy` vector model fixes the operator new push placement; inline map ctor.
- CCombatWheel::ReassignAttacker 0x006E80F0 -- implicit operator= keeps the dead Z copy; GetLengthSquared X+Y+Z (Y emitted first).
- CCombatManager::IsDefenderDueForAttack 0x006E97F0 -- compiled-out trace as named name+message locals passed to an inline no-op; a __forceinline predicate puts the refusal block after the loop.
- CCombatManager::PerformSequenceReset 0x006EA2C0 -- same-TU find + GetSubDef stand-ins; the found node kept and `->value` formed at use; __forceinline world random.
- CCombatManager::IsTargetOverriddenByAttemptedFacingDirection 0x006EA5F0 -- `float angle = 60.0f; if (found) angle = ...` keeps 60.0f in a frame slot; float compared with the double cos.
- CCombatManager::RegisterAttackedCombatant 0x006ED270 -- find + GetDef stand-ins; named insert-result pair read after the value temporary dies.
- CCombatManager::RegisterCombatant 0x006EDC50 -- inline by-value name helper returning a named local (build/copy/destroy).
- std::vector<CCombatWheel::SSlot>::operator= 0x006F3C20 -- STLport assignment; same-TU stand-ins for __uninitialized_copy / __copy / _Destroy put every tag temporary in the dead argument slot (4-byte frame) and duplicate the _M_finish tail like retail.
- CCombatAction_ControlledStrafeJump ctor 0x0085C190 -- RollMode/facings assigned in the body (an init list sinks the byte store).
- CCombatAction_GenericResponseToDrain::Initialise 0x008676A0 -- turns away from the drainer and adds CModeDrained; owner through the intelligent pointer's inline `operator CThing*()` (fetched before the target), physics reference via an inline GetPhysics() before the member-wise CRightHandedSet.
- CCreatureAction_CastWillEnflameSpell::Initialise 0x008BA220 / CCreatureAction_CastWillDrainLifeSpell::PerformAction 0x008BA7F0 -- the Cast*Spell model plus a same-TU CVectorMap::LowerBound stand-in: only then does the GetInterface key share its frame word with the name temporary and the by-value operator= parameter.
- CCreatureAction_CastWillLightningSpell(owner, state, param, target) 0x008BCA00 -- landed 3-arg overload plus a by-value inline SetPTarget fed an explicit CIntelligentPointer temporary; first try.
- CCombatAction_StrikeResponseMazeDoForcePush::FinishAction 0x008C2810 -- Maze's hit counter / force push; GetSubDef + LowerBound stand-ins, an inline CThing::GetSubDef wrapper (thing fetched before the push), C3DVector offset whose dead Z keeps its word.
- CCreatureAction_CastBattleCharge::Initialise 0x008CD5A0 -- landed CastAssassinRush model; __forceinline GetPhysics() + reference keeps physics in ecx.
- NCreatureMode::CModeScorpionKingVulnerable::Recover 0x008DF3A0 -- tail plunge or hit-location swap; each component from a CTCBed-style accessor `T* c = 0; if (HasTC) c = GetTC(ternary)` (puts the zero in ecx).
- CCreatureAction_DragonRoar::PerformAction 0x008F5490 -- barrier ahead of the dragon's mouth; in-place operator+= with a user C3DVector copy ctor (else the Y add keeps t.Y on the x87 stack); scoped dummy-name string.
- CCombatSequence_NonInterruptableBoxStrafe::OnCreate 0x009064B0 -- WhisperAdvance model; reference getters + by-reference ctors rotate the AddAction lea registers.
- CCombatSequenceJacksCombo::OnCreate 0x0090A1F0 -- landed OnCreate family; three InterruptableMidAttack actions.
- CConsole::NextCommandGuess 0x009EB0D0 -- inline IsEqualNoCase on a by-value entry copy; implicit CCharString->CConsoleCommandLine conversion.
- CConsole::SetHelpBarString 0x009EB2C0 -- __forceinline operator==(char); nested ifs with one trailing "" assignment.
- CConsole::RunCommandLine 0x009EB430 -- the lower-cased lookup goes through an inline FindInput(key) wrapper; a direct `map.find(name.ToLower())` hoists the map lea above the ToLower call.
- CConsole::RunTextCommand 0x009EC710 -- implicit ";" conversion in the loop condition; inline CConsoleCommandLine::Reset.
- CChecksumCacheFile::Save 0x00A60AA0 -- local CDiskFileWin32/CFileDataOutputStream (SaveBinary model); first try.
- CString::GetBuffer 0x00C0C4B0 (catalogued CCharString::SetLength) -- MFC-style buffer; memcpy intrinsic, __stdcall static Release.
- NScript::CV_ChickenKickingScript::CChickenSign::Main 0x00E64DF0 -- relabels the sign per sector; TextKeys[i] through `*(begin() + n)` (start loaded before the index); nested GetThingWithScriptName argument.

## Parked for Fable 5.1 (7)
- 0052c920 CChangingStateComponent::ProcessChangeState DIFFER(393v395): colour-delta channels: retail `mov al,t.R ; sub al,c.R` / `push ebx ; mov bl,t.A`, stores R,G,A,B; ours loads both operands into registers (no ebx), stores R,G,B,A | tried: operator- via 4-arg ctor, member-wise d.R=..., init-list ctor order, GetR() getters + EEEE ctor, named channel locals (the push 1/push 0 linear flag is fixed: explicit if/else of two calls) | next: lane gap: byte-register allocation
- 00c41e40 CCombatant::SetCombatParameters DIFFER(368v368): float arms: retail `mov eax,[flags] ; fstp member ; or eax,bit ; mov [flags],eax`, ours `fstp member` before the flags load | tried: flags-first, flags copied to a local, `const T&` value, SetDirty helper | next: separate non-template float overload, else lane gap: x87 store scheduling
- 00746ec0 CTCStealth::GetPositionToMoveToForStabOn DIFFER(328v326): after the inline fast normalise retail `fstp r ; fxch ; fmul [r]`, ours `fst r ; fmul st(2)` | tried: five normalise/length orderings | next: lane gap: x87 stack allocation
- 00909a00 CCombatSequence_WillForcePushLongRange::IsValid DIFFER(298v336): retail keeps the dead normalise + dot + `fcomp ; fnstsw` (both arms return true); ours folds it away | tried: every if/return form, named bools, LowerBound stand-in | next: an arm that differs in source and folds only after inlining (e.g. `return IsSomething()` of an inline constant-true predicate)
- 00858400 CCombatActionBase::ProcessPossibleProneThingsStrikeEvent DIFFER(400v400): `mov [esp+0x40],ebp` vs retail `[esp+0x44]`; tag `lea edx,[esp+0x4c]` vs retail `[esp+0x44]`/`[esp+0x50]` | tried: named pointer, `&thing` temporary, _Construct layer, by-value push_back param (fixes the forwarded store), pointer local at entry, LowerBound and _M_insert_overflow stand-ins | next: lane gap: dead-argument-slot choice for temporaries
- 008c2ad0 CCombatAction_WillHealInto::FinishAction DIFFER(381v370): `mov eax,[def] ; cmp eax,ebx ; mov esi,eax` vs retail `mov esi,[def] ; cmp esi,ebx` | tried: named/Get()/operator T*/const pointer, if-init, hand release (TeleportIn form), GetSubDef stand-in (block-scoped effectType already fixed the slot sharing) | next: lane gap: register preference
- 00d91320 CCheckTheSilverKeyChest::Main DIFFER(316v335): retail branches straight out of the unassigned-string strcmp arm, keeps the block-compare arm out of line after the epilogue with its bool stored to [esp+0xf] (frame 0x18 vs 0x14), matched body inline; ours merges the arms (sete) and places the matched body after the loop | tried: named bool in operator==, non-null-first, ternary, split `if`, hand-split goto (SetButton1OverrideText form), named bool in Main | next: hand-split goto with the block-compare result in a function-scope named bool

## Skipped
None.

## Label corrections
- 0x008DF3A0 "CCreatureAction_BriarRose::Execute" (forge-autoname4) is NCreatureMode::CModeScorpionKingVulnerable::Recover (only caller 0x008E0D08 in that class's FrameUpdate; `this` matches the Ego_r layout).
- 0x006F3C20 "CCombatWheel::CopyCombatWheel" is std::vector<SSlot>::operator=.
- 0x00746EC0 "CCombatAction_StabThing::CalculateStabVector" is CTCStealth::GetPositionToMoveToForStabOn.
- 0x00C0C4B0 "CCharString::SetLength" is the MFC-style CString::GetBuffer.
- 0x00546D90 "CComponentContainer::Initialise" is NUISystem::CTextContainer::Initialise (propagated from the identical 0x005461D0).
- 0x008C38B0 and 0x008C3930 "CCombatAction_Guildmaster_HealFollowersInto" are the CCombatAction_WillForcePushInto and CCombatAction_WillHealRelease ctors (vtables 0x012804FC / 0x01280644 RTTI).
- 0x008BFD60 / 0x008BFCA0 / 0x008BFC40 "CCombatAction_GenericResponseToDrain" are the CCreatureAction_StrafeBackward / DiagonalStrafeBR / DiagonalStrafeBL ctors.
- 0x006925A0 "CTCInventoryBase::GetCanItemBeQuickAssigned" is CCombatActionBase::HasPerformedAction (byte +0x62 getter); 0x00856F00 "CTCHeroExperience::GetRoughExperienceLevel" is CCombatActionBase::IsThingInStruckThingList; 0x00856E50 is CCombatActionBase::GetPActiveStrikeEvent.
- 0x007AED10 "CWorld::DrawGetEnvironment" is CTCBoss::GetCurrentPhase (folded getter); 0x007FD3D0 "CAIStateGroupBuyFromSeller::BoughtSomething" is the barrier driver's lifetime setter (folded); 0x0070D510 "_Dest_val<...CMusicManager...>" is the CAnimateThing queue clear (folded).
- 0x00A14440 "C3DVector::GetScaled" is C3DVector::Normalise; 0x0049D810 "CVertexBufferWin32::DoSizeof" is CWorld::GetPRandomSeed; 0x006E9700 "CCombatWheel_ValidateSequencePosition" is CCombatManager::IsCreatureOnRotatingCombatRing; 0x006F4A00 "C3DMeshMaterial" is the CCombatantInfo ctor; 0x0099F570 "CCharString::AppendData" is operator+(const CCharString&, const CCharString&); 0x006ED030 "CCombatManager_UpdateDefenderState" is UpdateAttackingCombatantsList; 0x008B0B10 "std::vector::push_back" is CCombatSequenceBase::AddAction; 0x0099A380 "CBaseIntelligentPointer" is CBaseObject's ctor; 0x0052CD20's callee "CList::AllScrollingChildrenDeleting" is propagated.
