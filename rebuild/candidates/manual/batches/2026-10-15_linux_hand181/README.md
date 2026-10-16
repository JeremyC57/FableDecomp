# Hand chunk 181 (C3DSkeletalMorph .. CAIStateGroup_DeedReactionRoot): 27 of 40 byte-exact

## What matched (27)
- C3DSkeletalMorph::PeekCombinedMorph 0x00A45260: lazy combined target, size re-read per iteration, `*(begin()+n)`, weight float local.
- C3DSkeletalMorphParameter ctor 0x00A454F0 (vtable 0x0129D490): `_Vector_base`+`_STLP_alloc_proxy` bone arrays fix the `lea` placement; inline memberwise target operator=.
- C2DVector::ToString 0x00A13CE0 (catalog said C3DVector): `CCharString("(") + FormatDouble(2,X) + ", " + FormatDouble(2,Y) + ")"`.
- CAC_Owner::GetRandomSpeech x3 (0x00E2E440 5 lines/+0x80, 0x00E362B0 10/+0x74, 0x00E53CB0 10/+0x78): `for(;;){ if (IsDone()) return none; pick; if (ok) break; yield; }` then IsDone re-checks (a while loop flips the layout).
- CAIBrain ctor 0x0088C160 (vtable 0x012780C4): `_Vector_base` model for both vectors fixed the argument-load order; parameter CDefPointer released inline.
- CAIStateGroupBase: AIAction_PerformAction 0x0097D8B0 (~CBrainOutput out of line here), AIAction_PumpCombatActions 0x0097D980 (implicit ~CBrainOutput inlined), SetUpPreCalcNav 0x0097F5B0 (unset route component in a dead slot; creature re-read into a named local before the setup call), AIAction_SpeakAISpeechToTarget 0x00981350 (speech-map local first; GetString as `s = ""; if (data) s = data->Chars;`).
- CAIStateGroupBehaviourBase::Initialise 0x00957940: no named locals so keys sit in the dead `name` slot; const and non-const lookups.
- CAIStateGroup_BanditKingCharge::Update 0x00961790: by-value CIntelligentPointer argument through an inline converting copy ctor from PTargetThing.
- CAIStateGroup_BanditKingLunge::IsRunnable 0x009614A0 (16.0f) / ThrowWeapons 0x00961550 (25.0f): distance helper args (hero, creature); ThrowWeapons needs `d = dx*dx; d += dy*dy;` and a named pointer local for the out-of-line operator=.
- CAIStateGroup_BanditSitRoundCampfire::Singsong 0x0093A7A0: ctor has 3 bools (4th zero is Perform's), creature local after the strings, int-width result.
- CAIStateGroup_BriarRoseWillUse::IsRunnable 0x00956CE0: `__forceinline` template CIntelligentPointer ctor (else emitted out of line).
- CAIStateGroup_Clean::OnActivate 0x00917520: building local before GetBoundingInfo; uninitialised `last` when the step loop never runs (retail behaviour).
- CAIStateGroup_CreatureAdvanceInCover IsRunnableLeaf 0x00978E60 / IsStillRunnable 0x00978F10, CreatureTakeCover::IsStillRunnable 0x00979E90: the two lookups share one key slot only as out-parameter GetInterface written in the body (not in an inline helper).
- CAIStateGroup_CreatureRangedCombat2::IsRunnable 0x0091EB60 and FireAtSpecificTargets::IsRunnable 0x0095E260: the NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon module is a temporary (novtable base ctor sets members, call through `[vtable]`).
- CAIStateGroup_CreatureUseCoverBase IsRunnable 0x00970AB0 (entry-returning FindEntry `sel` form puts the component in ecx), ClearCoverThing 0x00970B60, CleanupLeaf 0x00970C10 (inline `clear()` member).
- CAIStateGroup_DeedReactionRoot::OnActivate 0x00960070: derive from a real base (vptr + __int64 in a flat model pads the vptr).

## Parked for Fable 5.1 (13)
- 00a44cd0 C3DSkeletalMorphTarget::GetBoneScaling DIFFER(179v175): identity local collapses to one reused frame word and both returns copy through `mov ecx,eax`; begin() loaded into ebx then moved to ebp | tried: returned temporary, named local, __forceinline copy ctor, body-assigning ctor | next: lane gap: return-value copy register / scalar replacement
- 0097f690 CAIStateGroupBase::AIAction_MoveToPreCalcRoute DIFFER(226v226): callee-saved parameter registers assigned differently (ours ebx=anim, ebp=speed, edi=extra; retail ebx=speed, edi=anim, ebp=extra) and the result spills to the extra slot instead of the condition slot | tried: POD vector, user copy ctor (fixed the in-place argument build), float speed/extra | next: lane gap: register allocation
- 0097f780 CAIStateGroupBase::AIAction_MoveToPreCalcRouteProximity DIFFER(233v233): same parameter-register assignment and result-spill-slot difference as 0x0097F690 | tried: POD vector, user copy ctor | next: lane gap: register allocation
- 009577e0 CAIStateGroupBehaviourBase::IsRunnable DIFFER(181v181): `lea esi,[edi+14h]` scheduled before the first temporary's GetPItem call (retail after) | tried: SetPItem(GetPItem()), inline operator=, __forceinline SetTarget helper | next: lane gap: scheduling
- 0092bcb0 CAIStateGroup_AvoidRegionExit::IsRunnable DIFFER(207v207): esi/edi swap for the 0x105 component and its PThing address (retail reuses the creature's edi) | tried: creature local + two GetInterface, component IsThingAlive accessor, out-pointers declared together, no creature local | next: lane gap: register allocation
- 0094f090 CAIStateGroup_BalverineCornered::GetLandPos DIFFER(223v223): landing Z: `fld [pos.Z] ; fadd [tmp]` vs retail `fld [tmp] ; fadd [pos.Z]` (VC orders the operands by address) | tried: pos + dir*d, dir*d + pos, swapped operator+ operands, += form | next: lane gap: x87 operand order
- 0094f640 CAIStateGroup_BalverineCornered::Update DIFFER(224v236): ours merges both arms' PerformActionAndWait call (+ lea ecx) into one; retail keeps a call per arm and merges at the dtor | tried: plain arms, creature locals per arm (fixed regs), 0x118 action size (fixed the frame) | next: lane gap: tail merging of call arms
- 0094f730 CAIStateGroup_BalverineJumpAtFleeingEnemy::Update DIFFER(228v230): frame-rate/frame locals in their own slots (sub esp,124h) where retail overlaps them with the action temporaries (11Ch); retail keeps the dead second result in edi | tried: flat locals, block-scoped locals + creature locals (fixed the bl constant and creature regs), named action locals | next: lane gap: frame slot packing of block locals with temporaries
- 009612f0 CAIStateGroup_BanditKingCharge::IsRunnable DIFFER(174v174): esi/edi swap: hero in esi and edi pushed late (retail hero edi, esi pushed late) | tried: inline dist helper (both arg orders), pointer IS_THING_ALIVE helper, member distance helper | next: lane gap: register allocation
- 0095e790 CAIStateGroup_BoastingCrowdMember::IsRunnable DIFFER(209v209): esi/edi swap (hero vs interface-map register) | tried: GetInterface, group declared first (fixed the frame), HasTC+FindEntry, IS_THING_ALIVE<T>(T* const&) | next: lane gap: register allocation
- 00917a10 CAIStateGroup_Clean::IsRunnable DIFFER(184v184): early-exit def release stores the decremented count before `mov edx,esi ; test edx,edx` (ours after) | tried: one  | next: 
- 0091ea70 CAIStateGroup_CreatureRangedCombat2::IsAbleToFire DIFFER(240v240): ecx/edx/eax rotation: creature in ecx for the 0x34 test (retail eax), PBrain in edx (retail ecx), key address registers swapped | tried: early returns, nested single exits, inline GetCreature(), HasTC returning the masked word | next: lane gap: register allocation
- 009713e0 CAIStateGroup_CreatureUseCoverBase::DuckBehindCover DIFFER(190v190): ebx/ebp swapped (&PTargetThing vs speech id), eax/ecx/edx rotation at the def release and action args, `add ecx,38h` after the pushes vs ours `lea ecx,[eax+38h]` | tried: direct chain, creature local before GetPWorld, dialogue-ref local, combat-manager local, inline GetCreature() | next: lane gap: register allocation

## Skipped
- none

## Label corrections
- 0x00A13CE0 is C2DVector::ToString (formats X, Y only; BSim 0.84 to C3DVector::ToString, whose real body is 0x00A13DA0).
- 0x008FCF40 "CXMVPlaybackInfo" ctor = CAIStateGroupBase::Terminate; 0x008FCFC0 "CPlayer::GetCurrentMode" = CAIStateGroupBase::GetPWorld; 0x008FCF90 "CPixelShader::CountReferences" = GetPPlayerManager.
- 0x0062E900 "~CTCLightningOrb" = ~CCombatActionBase; 0x00693EF0 "~CPhysicsMeshInfo" = an animation-action base dtor (play-into-loop-out); 0x00661E80 "CText::GetAlignment" = a CThingCreatureBase flag getter (+0x15c bits 2/3).
- 0x00A14440 "GetScaled" = C3DVector::Normalise; 0x004C73D0 "DoSizeof" = CThing::GetPosition; 0x00449970 "Catch@0040d959" = GetMainPlayer; 0x00487DC0 "_Cons_val" = CPlayer::GetHeroThing; 0x0049D870 "GFGetPreMainMemoryUsed" = CWorld::GetWorldFrame.
- 0x0088B8F0 "CQuestManager::RemoveAvailableQuestCardType" = CAIBrain::SetBrainOutput; 0x00A44740 "CCastBattleCharge ctor" = the brain base ctor; 0x00462CB0 "CActiveFile_CopyAssign" / 0x00A46790 "CCombatWheel::operator_assign" = vector operator=.
- 0x006CB9C0 "CScriptThing::operator==" = navigation route setup (3 args); 0x007446A0 "_Cons_val" = a CTCPerceiveThing query; 0x008CCC70/0x008CCD30 both "CCombatAction_GenericResponseToDrain" = two balverine combat-action ctors (byte twins).
- 0x006C3AC0 / 0x00668D90 / 0x00A47060 "CCountedPointer<CIPhysicsFacesSegmentBase>::Reset" are Reset of other instantiations (morph target, brain output action, morph shared data).
- CAC_Owner label on 0x00E362B0 / 0x00E53CB0 is propagated from 0x00E2E440 (different script modules, different counts/offsets); true script classes not established.
