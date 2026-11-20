// Behaviour test for 0x00EC4A00 NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main (links the REAL row object: the row body,
// the expanded CSpawnedFunc1 ctors, the implicit CScriptThing copy COMDAT it calls for Par1 / the second argument, the STLport map
// operator[] / insert / ~map COMDATs, the ScriptedThingBase ctor / dtor / copy COMDATs and the CWideScreenMagicPauseEntities expansion all
// run as compiled). No class models: every external the row object names is bound with /alternatename -- the calls the row makes to
// extern "C" __fastcall stubs (thiscall-compatible, unused edx), every vtable slot and unreached COMDAT reference to a trap. Game (+4)
// slots 7, 8, 70, 370, 379, 451, 498, 585 through a fake vtable. Raw native-layout fixtures: the entity (Game +4, Me {vptr, Data, Info}
// +8, ParentClass +0x14), the parent script (PlayedExhumeCutScene +0x54, AddedItemsToRockTroll +0x55), the script def (CDefStrings
// +0x730 / +0x734), counted-pointer info blocks {RefCount, DeleteFunc, Data}, STLport tree nodes {color, parent, left, right, key,
// ScriptedThingBase} linked by the insert_unique stub.
// Reference model: Model() below, written from the script's statements; every run is compared event by event plus the final state (flags,
// the two spawned threads' fields -- vtable, Func, PClass, Par1 = Me with one reference each --, every string, handle reference, node and
// movie released, Me's reference count).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:??_GCBaseClass@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@BODtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@BOCtor@8")
#pragma comment(linker, "/alternatename:??MCCharString@@QBE_NABV0@@Z=@StrLess@12")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@ABV0@@Z=@BOCopy@12")
#pragma comment(linker, "/alternatename:?IsEqualTo@CScriptThing@@MBE_NABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsNull@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDead@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsAlive@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetScriptCounter@CScriptThing@@UAEJXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DecrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IncrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UpdateThingAttachment@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetToKillOnLevelUnload@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetActivationTriggerStatus@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetActivationTriggerStatus@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetFriendsWithEverythingFlag@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetAsUsable@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsSummonedCreature@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsOpenDoor@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUsable@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUnconscious@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKicked@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOnCutSceneAnimEvent@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOnMoralityChanged@CScriptThing@@UBE_NAAJ@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedSuccessfulFlourish@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedFlourish@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgGetThingsKilled@CScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgTalkedToAnyone@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgAttackedWithBareHandsWithoutHittingAnything@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgAttackedWithMeleeWeaponWithoutHittingAnything@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithBareHands@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithBareHands@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithRangedWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithRangedWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithMeleeWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithMeleeWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOpenedChest@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyNonAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnyNonAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnyAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnySpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedSpecialAbility@CScriptThing@@UBE_NW4EHeroAbility@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgReceivedInventoryItem@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgReceivedMoney@CScriptThing@@UBE_NAAJ@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoKnockedMeOut@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKnockedOutBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHowLongWasExpressionPerformed@CScriptThing@@UBEJXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoExpressedToMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgExpressionPerformedTo@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoTalkedToMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsTalkedToBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoHitMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithProjectileWeapon@CScriptThing@@UBE_NABVCCharString@@AAM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithWeapon@CScriptThing@@UBE_NABVCCharString@@0@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithDecapitate@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithFlourish@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsUsedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoKilledMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKilledBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsTriggeredBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsAwareOfHero@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsSneaking@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentScriptPriority@CScriptThing@@UBE?AW4EScriptAIPriority@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentStateGroupType@CScriptThing@@UBE?AW4EAIStateGroupType@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPThingUniqueID@CScriptThing@@UBE_KXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPThing@CScriptThing@@UBEPAVCThing@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetAngleXY@CScriptThing@@UBEMXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetHomeMapName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentMapName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetHomePos@CScriptThing@@UBE?AVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPos@CScriptThing@@UBEABVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetName@CScriptThing@@UBEABVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptThing@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@ThingDtor@8")
#pragma comment(linker, "/alternatename:??1CSuspendableProcessMicrothread@@MAE@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=@IsPerf@8")
#pragma comment(linker, "/alternatename:?UnsheatheWeapons@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DropGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToAndPickUpGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXAAVCScriptThing@@W4EScriptEntityMoveType@@_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActions@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@J_N111111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayCombatAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N11111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z=@PlayAnim@40")
#pragma comment(linker, "/alternatename:?PerformExpression@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Converse@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@PBD@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?AskHeroQuestion@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDABVCCharString@@22_N3@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@2@@Z=@InsertUnique@20")
#pragma comment(linker, "/alternatename:?OnInterrupted@CSpawnedFuncBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?OnPredicateFail@CSpawnedFuncBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Restart@CSuspendableProcessMicrothread@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?RunSuspendableProcess@CSuspendableProcessMicrothread@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_G?$CSpawnedFunc1@VCV_RockTrollFirstEncounterScript@NScript@@VCScriptThing@@@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z=@SFBaseCtor@16")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@ABV0@@Z=@StrCopy@12")
#pragma comment(linker, "/alternatename:?GDefaultToUsingPhysicsInCutsceneAnimations@NGlobalConsole@@3_NA=_g_GDefault")
#pragma comment(linker, "/alternatename:?WatchForRockTrollHit@CV_RockTrollFirstEncounterScript@NScript@@QAEXVCScriptThing@@@Z=_WatchHitStub")
#pragma comment(linker, "/alternatename:?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@PAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@3@PAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@3@_N4@Z=@RunCutscene@24")
#pragma comment(linker, "/alternatename:??4CBaseObject@@IAEAAV0@ABV0@@Z=@BOAssign@12")
#pragma comment(linker, "/alternatename:??0?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAE@XZ=@MapCtor@8")
#pragma comment(linker, "/alternatename:?GetString@CDefString@@QBE?AVCCharString@@XZ=@DefStrConv@12")
#pragma comment(linker, "/alternatename:?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A=_g_ScriptDefHandle")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@ReleaseStub@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@IsValidStub@8")
#pragma comment(linker, "/alternatename:?AddSpawnedFunction@CScriptBase@@QAEXPAVCSpawnedFuncBase@@ABVCCharString@@@Z=@AddSpawned@16")
#pragma comment(linker, "/alternatename:?WatchForRockTrollKilled@CV_RockTrollFirstEncounterScript@NScript@@QAEXVCScriptThing@@@Z=_WatchKilledStub")
#pragma comment(linker, "/alternatename:??H@YI?AVCCharString@@ABV0@0@Z=@Concat@12")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:_g_STVtbl=??_7CScriptThing@@6B@")
#pragma comment(linker, "/alternatename:_g_STBVtbl=??_7CScriptGameResourceObjectScriptedThingBase@@6B@")
#pragma comment(linker, "/alternatename:_g_MovieVtbl=??_7CScriptGameResourceObjectMovieBase@@6B@")
#pragma comment(linker, "/alternatename:_g_SF1Vtbl=??_7?$CSpawnedFunc1@VCV_RockTrollFirstEncounterScript@NScript@@VCScriptThing@@@@6B@")
extern "C" char g_STVtbl, g_STBVtbl, g_SF1Vtbl, g_MovieVtbl;
extern "C" void __fastcall CallMain(void* self);   // the row (private virtual, thiscall)
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CRTFE_RockTroll@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ")

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static Script sTerm, sStartT, sStartH, sPerf, sNew;      // past the end: terminating / started / started / not performing / allocated

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 8000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static void Ws(const char* s) { while (*s) W(*s++); }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x20]; double a; } self;                 // CRTFE_RockTroll (0x1C)
static union { unsigned char b[0x60]; double a; } parent;               // CV_RockTrollFirstEncounterScript (0x58)
static union { unsigned char b[0x740]; double a; } def;                 // CScriptDef (CDefStrings +0x730 / +0x734)
extern "C" void* g_ScriptDefHandle = def.b;
extern "C" unsigned char g_GDefault = 0;
static void* gameVtbl[600]; static void* gameObj[1];
static void* meVtbl[1]; static void* heroVtbl[1];
struct Info { long RefCount; void* DeleteFunc; void* Data; };
static Info meInfo, heroInfo;
static void* hero[3];
static const char* reward1 = "Reward1Text"; static const char* reward2 = "Reward2Text";
extern "C" void __cdecl WatchKilledStub() { VirtualTrap(); }
extern "C" void __cdecl WatchHitStub() { VirtualTrap(); }

// strings: one text pointer; concatenations live in a pool
static int liveStrings; static char pool[64][64]; static int npool;
static char C0(const char* t) { return t && *t ? *t : '0'; }
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; check(len == -1, "string length", 0); Ev('s'); Ev(C0(text)); return s; }
extern "C" void* __fastcall StrCopy(const char** s, void*, const char* const* o) { *s = *o; ++liveStrings; Ev('k'); Ev(C0(*o)); return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; Ev('~'); Ev(C0(*s)); *s = 0; }
extern "C" bool __fastcall StrLess(const char* const* a, void*, const char* const* b) { return strcmp(*a, *b) < 0; }
extern "C" void* __fastcall Concat(const char** ret, const char* const* a, const char* const* b)
{
    char* p = pool[npool++ & 63]; strcpy(p, *a); strcat(p, *b); *ret = p; ++liveStrings; Ev('c'); return ret;
}
extern "C" void* __fastcall DefStrConv(const unsigned char* d, void*, const char** ret)
{
    check(d == def.b + 0x730 || d == def.b + 0x734, "def string member", 0);
    *ret = d == def.b + 0x730 ? reward1 : reward2; ++liveStrings; Ev('r'); return ret;
}

// counted-pointer handles: Info blocks from malloc, freed by the row (free) or by our Release
static int liveHandles, deletes; static long nextData = 0x7000;
extern "C" void __fastcall DelFunc(void* data) { ++deletes; --liveHandles; Ev('X'); (void)data; }   // the row frees the block after this
static Info* NewInfo() { Info* i = (Info*)malloc(sizeof(Info)); i->RefCount = 1; i->DeleteFunc = (void*)DelFunc; i->Data = (void*)(nextData += 0x10); ++liveHandles; return i; }

// things {vptr, Data, Info}: the row destroys its argument temporaries through ~CScriptThing
extern "C" void __fastcall ThingDtor(void** t, void*)
{
    check(t[0] == (void*)&g_STVtbl && t[2] == (void*)&meInfo && t[1] == meInfo.Data, "destroyed temporary is a copy of Me", 0);
    --meInfo.RefCount; Ev('x');
}

// ScriptedThingBase {vptr, IntelligentPointer, Data, Info}
extern "C" void* __fastcall BOCtor(void** o, void*) { o[1] = 0; Ev('o'); return o; }
extern "C" void* __fastcall BOCopy(void** o, void*, void* const* src) { o[1] = src[1]; Ev('p'); return o; }
extern "C" void __fastcall BODtor(void** o, void*) { Ev('O'); (void)o; }
static int validCalls;
extern "C" bool __fastcall IsValidStub(void* const* h, void*) { bool r = h[2] != 0; Ev('v'); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall ReleaseStub(void** h, void*)
{
    Info* i = (Info*)h[3]; Ev('L');
    if (i && --i->RefCount == 0) { Ev('X'); ++deletes; free(i); --liveHandles; }
    h[2] = h[3] = 0;
}
// the row's ScriptedThingBase::operator= COMDAT (retail 0x008ABD10's bytes) calls CBaseObject::operator= and shares PImp inline
extern "C" void* __fastcall BOAssign(void** o, void*, void* const* src) { o[1] = src[1]; Ev('a'); return o; }
static void* trollSeh; static void* heroSeh; static int perfCalls; static int animCalls;
extern "C" void __fastcall PlayAnim(void** h, void*, const char* const* name, int b1, int b2, int b3, int b4, int b5, int b6, int b7)
{
    check(h == trollSeh, "animation on seh_Troll", 0);
    check(!b1 && !b2 && !b3 && b4 == 1 && b5 == (int)g_GDefault && !b6 && !b7, "animation flags", 0);
    check(strcmp(*name, animCalls == 0 ? "SPECIAL_BOAST" : "SPECIAL_IDLE") == 0, "animation name", animCalls);
    ++animCalls; Ev('P');
}
extern "C" bool __fastcall IsPerf(void* h, void*) { check(h == trollSeh, "performing on seh_Troll", 0); bool r = Next(sPerf); Ev(r ? 'K' : 'k'); return r; }

// STLport map: header {color, parent, left, right}, nodes + key + ScriptedThingBase (0x24). The row's ~map frees them with the CRT free
// (STLport malloc allocator, `_free`), so node release is checked through the pair destructor events (O~T O~H), not a counter.
struct Node { long color; Node* parent; Node* left; Node* right; const char* key; void* h[4]; };
static int liveNodes; static void* mapObj;
static void* nodeBlocks[16]; static int nnodeBlocks;
static void TrackNode(void* p) { nodeBlocks[nnodeBlocks++ & 15] = p; }

extern "C" void* __fastcall MapCtor(void** m, void*)
{
    Node* hd = (Node*)malloc(sizeof(Node)); memset(hd, 0, sizeof(Node)); hd->left = hd->right = hd; ++liveNodes; TrackNode(hd);
    m[0] = hd; m[1] = 0; mapObj = m; Ev('M'); return m;
}
void __cdecl operator delete(void* p)          // tree nodes (map dtor) and counted-pointer info blocks (handle release)
{
    for (int i = 0; i < 16; ++i) if (nodeBlocks[i] == p) { nodeBlocks[i] = 0; --liveNodes; free(p); return; }
    free(p);
}
extern "C" void* __fastcall InsertUnique(void** m, void*, Node** ret, Node* hint, const unsigned char* v)
{
    check(m == mapObj, "insert map", 0); (void)hint;
    Node* hd = (Node*)m[0]; Node* n = (Node*)malloc(sizeof(Node)); memset(n, 0, sizeof(Node)); ++liveNodes; TrackNode(n);
    n->key = *(const char* const*)v; ++liveStrings;
    memcpy(n->h, v + 4, 16); if (n->h[3]) ++((Info*)n->h[3])->RefCount;
    check(n->h[0] == (void*)&g_STBVtbl, "inserted value is a ScriptedThingBase", 0);
    if (!hd->parent) { hd->parent = n; n->parent = hd; hd->left = hd->right = n; }
    else {
        Node* x = hd->parent;
        for (;;) {
            if (strcmp(n->key, x->key) < 0) { if (!x->left) { x->left = n; break; } x = x->left; }
            else { if (!x->right) { x->right = n; break; } x = x->right; }
        }
        n->parent = x;
    }
    m[1] = (void*)((long)m[1] + 1); Ev('U'); Ev(C0(n->key)); *ret = n; return ret;
}
static const char* nodeHeroData; static void* nodeHero; static void* nodeTroll;

// movie: the row's ctor / dtor COMDATs (retail 0x006E7B60 / 0x006E7B80's bytes) run; their CBaseObject parts log 'o' / 'O'
static void* movieObj; static int liveMovies;
// spawned threads
static unsigned char playedInit; static unsigned char spawnBuf[2][0x48]; static int nspawn; static bool nullSpawn[2]; static void* spawned[2]; static int nadded;
static const char* sfName[2]; static unsigned long sfId[2];
void* __cdecl operator new(unsigned int n)
{
    check(n == 0x48, "new size", 0); int i = nspawn++; bool ok = Next(sNew); Ev('N');
    if (!ok || i > 1) { nullSpawn[i & 1] = true; return 0; }
    memset(spawnBuf[i], 0xCD, 0x48); return spawnBuf[i];
}
extern "C" void* __fastcall SFBaseCtor(unsigned char* o, void*, const char* const* name, unsigned long id)
{
    int i = o == spawnBuf[0] ? 0 : o == spawnBuf[1] ? 1 : -1; check(i >= 0, "spawn base this", 0);
    if (i >= 0) { sfName[i] = *name; sfId[i] = id; }
    Ev('B'); return o;
}
extern "C" void __fastcall AddSpawned(void* p, void*, void* f, const char* const* name)
{
    check(p == parent.b, "AddSpawnedFunction on ParentClass", 0); check(strcmp(*name, "") == 0, "spawn group name", 0);
    check(parent.b[0x54] == playedInit, "PlayedExhumeCutScene unchanged at each spawn", 0);   // the store follows the second spawn
    spawned[nadded & 1] = f; ++nadded; Ev('A');
}

// Game
static void* heroSeh_Data() { return heroSeh ? ((void**)heroSeh)[2] : 0; }
static void* trollSeh_Data() { return trollSeh ? ((void**)trollSeh)[2] : 0; }
static bool T_(void* s) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall IsTerm(void* s, void*) { return T_(s); }
extern "C" void __fastcall G_NewScriptFrame(void* g, void*) { check(g == gameObj, "frame this", 0); Ev('F'); }
extern "C" bool __fastcall G_StartScriptingEntity(void* g, void*, void* const* thing, void** h, long prio)
{
    check(g == gameObj && prio == 4, "start scripting args", 0);
    bool troll = thing == (void* const*)(self.b + 8); check(troll || thing == (void* const*)hero, "start scripting thing", 0);
    if (troll) trollSeh = h; else heroSeh = h;
    bool r = Next(troll ? sStartT : sStartH); Ev(troll ? 'S' : 'H'); Ev(r ? '1' : '0');
    if (r) { check(h[2] == 0 && h[3] == 0, "started into an empty handle", 0); h[3] = NewInfo(); h[2] = ((Info*)h[3])->Data; }
    return r;
}
extern "C" void* __fastcall G_GetHero(void* g, void*) { check(g == gameObj, "hero this", 0); Ev('h'); return hero; }
extern "C" void __fastcall G_StartMovieSequence(void* g, void*, const char* const* name, void* movie)
{
    void* const* mv = (void* const*)movie;
    check(g == gameObj && strcmp(*name, "") == 0 && mv[0] == (void*)&g_MovieVtbl && !mv[2] && !mv[3], "movie sequence args", 0); movieObj = movie; Ev('Q');
}
static int pauseDepth;
extern "C" void __fastcall G_Pause(void* g, void*, bool on) { check(g == gameObj, "pause this", 0); pauseDepth += on ? 1 : -1; Ev(on ? 'Z' : 'z'); }
extern "C" void __fastcall G_GiveTarget(void* g, void*, void* const* a, void* const* b)
{
    check(g == gameObj && a == (void* const*)(self.b + 8) && b == (void* const*)hero, "best enemy target args", 0); Ev('G');
}
extern "C" void __fastcall G_LookAt(void* g, void*, void* const* a, void* const* b)
{
    check(g == gameObj && a == (void* const*)(self.b + 8) && b == (void* const*)hero, "look-at args", 0); Ev('E');
}
extern "C" void __fastcall G_AddItem(void* g, void*, void* const* thing, const char* const* item)
{
    check(g == gameObj && thing == (void* const*)(self.b + 8), "add item thing", 0); Ev('I'); Ev(*item == reward1 ? '1' : *item == reward2 ? '2' : '?');
}
extern "C" void __fastcall RunCutscene(const char* const* name, void* map, void* p1, void* p2, int b1, int b2)
{
    check(strcmp(*name, "CS_ROCKTROLL_EXHUME") == 0 && map == mapObj && !p1 && !p2 && !b1 && b2 == 1, "cutscene args", 0);
    check(pauseDepth == 1, "cutscene under the paused entities", 0);
    Node* hd = *(Node**)map; Node* r = hd->parent;
    bool ok = r && r->key && strcmp(r->key, "HERO") == 0 && r->right && strcmp(r->right->key, "TROLL") == 0;
    check(ok && r->h[2] == heroSeh_Data() && r->right->h[2] == trollSeh_Data(), "cutscene map HERO / TROLL", 0);
    Ev('R');
}

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mStartT, mStartH, mPerf, mNew; static bool mAdded, mPlayed; static long mMeRefs; static bool trollHeld, heroHeld;
static bool T() { bool r = Next(mTerm); W(r ? 'T' : 't'); return r; }
static void SehDtor(bool& held) { if (held) { W('X'); held = false; } W('O'); }
static void Spawn(const char* fn)
{
    bool ok = Next(mNew); W('N');
    if (ok) { W('s'); W('W'); W('s'); W('P'); W('c'); W('B'); W('x'); ++mMeRefs; }
    Ws("s0A~0");
    if (ok) Ws("~P~P~W");
    (void)fn;
}
static void Model()
{
    Spawn("WatchForRockTrollKilled");
    W('o'); Ws("v0");                                                     // seh_Troll; if(seh_Troll.IsValid()) Release();
    for (;;) { bool r = Next(mStartT); W('S'); W(r ? '1' : '0'); if (r) { trollHeld = true; break; } W('F'); if (T()) { SehDtor(trollHeld); return; } }
    if (T()) { SehDtor(trollHeld); return; }
    if (!mAdded) {
        if (T()) { SehDtor(trollHeld); return; }
        Ws("rI1~R"); Ws("rI2~R"); mAdded = true;
    }
    if (!mPlayed) {
        if (T()) { SehDtor(trollHeld); return; }
        W('o'); Ws("v0");                                                 // seh_Hero
        for (;;) { W('h'); bool r = Next(mStartH); W('H'); W(r ? '1' : '0'); if (r) { heroHeld = true; break; } W('F'); if (T()) { SehDtor(heroHeld); SehDtor(trollHeld); return; } }
        if (T()) { SehDtor(heroHeld); SehDtor(trollHeld); return; }
        W('M');                                                           // csEntMap
        Ws("sH" "o" "kH" "p" "UH" "O~H" "O" "a" "~H");                    // csEntMap["HERO"]=seh_Hero;
        Ws("sT" "o" "kT" "p" "UT" "O~T" "O" "a" "~T");                    // csEntMap["TROLL"]=seh_Troll;
        Ws("o" "s0Q~0");                                                  // speech_movie; StartMovieSequence("", speech_movie)
        W('Z');                                                           // PauseMyEntitiesPlease
        Ws("sCR~C");                                                      // RunCutsceneMacro
        Spawn("WatchForRockTrollHit");
        mPlayed = true;
        W('z'); W('O');                                                   // ~PauseMyEntitiesPlease, ~speech_movie
        Ws("O~T" "O~H");                                                  // ~csEntMap: nodes (TROLL first: _M_erase walks the right subtree first)
        SehDtor(heroHeld);
    }
    Ws("sSP~S" "sSP~S");                                                  // PlayAnimation SPECIAL_BOAST / SPECIAL_IDLE
    for (;;) { bool r = Next(mPerf); W(r ? 'K' : 'k'); if (!r) break; W('F'); if (T()) { SehDtor(trollHeld); return; } }
    if (T()) { SehDtor(trollHeld); return; }
    W('v'); W(trollHeld ? '1' : '0');
    if (trollHeld) { W('L'); W('X'); trollHeld = false; }
    Ws("hE" "hG");
    for (;;) { W('F'); if (T()) { SehDtor(trollHeld); return; } }
}

static void Run(const char* term, const char* st, const char* sh, const char* perf, const char* nw, bool added, bool played, bool phys, int k)
{
    memset(self.b, 0xEE, sizeof self.b); memset(parent.b, 0xEE, sizeof parent.b);
    *(void**)(self.b + 4) = gameObj; *(void**)(self.b + 8) = meVtbl; meInfo.RefCount = 5; meInfo.Data = (void*)0x4D45;
    *(void**)(self.b + 0xC) = meInfo.Data; *(void**)(self.b + 0x10) = &meInfo; *(void**)(self.b + 0x14) = parent.b;
    parent.b[0x54] = played; parent.b[0x55] = added; playedInit = played; g_GDefault = phys;
    sTerm.s = term; sStartT.s = st; sStartH.s = sh; sPerf.s = perf; sNew.s = nw;
    sTerm.pos = sStartT.pos = sStartH.pos = sPerf.pos = sNew.pos = 0;
    sTerm.dflt = true; sStartT.dflt = true; sStartH.dflt = true; sPerf.dflt = false; sNew.dflt = true;
    mTerm = sTerm; mStartT = sStartT; mStartH = sStartH; mPerf = sPerf; mNew = sNew;
    ngot = nwant = 0; nspawn = nadded = 0; nullSpawn[0] = nullSpawn[1] = false; spawned[0] = spawned[1] = 0;
    liveStrings = liveHandles = liveNodes = liveMovies = deletes = pauseDepth = animCalls = 0; trollSeh = heroSeh = 0; mapObj = 0;
    mAdded = added; mPlayed = played; mMeRefs = 5; trollHeld = heroHeld = false;
    Model();
    CallMain(self.b);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) printf("  got  %.400s\n  want %.400s\n", got, want);
    }
    check(parent.b[0x55] == (unsigned char)mAdded, "AddedItemsToRockTroll", k);
    check(parent.b[0x54] == (unsigned char)mPlayed, "PlayedExhumeCutScene", k);
    check(parent.b[0x56] == 0xEE && parent.b[0x53] == 0xEE, "no other parent store", k);
    check(meInfo.RefCount == mMeRefs, "Me's references (one per spawned thread)", k);
    check(liveStrings == 0 && liveHandles == 0 && liveMovies == 0 && pauseDepth == 0, "everything released", k);
    if (failures && failures < 4 && (liveStrings || liveHandles || liveNodes || liveMovies || pauseDepth)) printf("  live s%d h%d n%d m%d p%d\n", liveStrings, liveHandles, liveNodes, liveMovies, pauseDepth);
    for (int i = 0; i < nadded; ++i) {
        unsigned char* f = (unsigned char*)spawned[i];
        if (!f) { check(nullSpawn[i], "null spawn only when new failed", k); continue; }
        check(f == spawnBuf[i], "spawned object is the new block", k);
        check(*(void**)f == (void*)&g_SF1Vtbl, "spawn vtable", k);
        check(*(void**)(f + 0x34) == (void*)(i == 0 ? WatchKilledStub : WatchHitStub), "spawn Func", k);
        check(*(void**)(f + 0x38) == (void*)parent.b, "spawn PClass", k);
        check(*(void**)(f + 0x3C) == (void*)&g_STVtbl && *(void**)(f + 0x40) == meInfo.Data && *(void**)(f + 0x44) == (void*)&meInfo, "spawn Par1 = Me", k);
        check(strcmp(sfName[i], i == 0 ? "ParentClass.WatchForRockTrollKilled" : "ParentClass.WatchForRockTrollHit") == 0 && sfId[i] == 0, "spawn name / id", k);
    }
}

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; meVtbl[0] = heroVtbl[0] = (void*)VirtualTrap;
    hero[0] = heroVtbl; hero[1] = (void*)0x4845; hero[2] = &heroInfo; heroInfo.RefCount = 9;
    for (int i = 0; i < 600; ++i) gameVtbl[i] = (void*)VirtualTrap;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScriptingEntity; gameVtbl[70] = (void*)G_GetHero;
    gameVtbl[370] = (void*)G_StartMovieSequence; gameVtbl[379] = (void*)G_Pause; gameVtbl[451] = (void*)G_GiveTarget;
    gameVtbl[498] = (void*)G_LookAt; gameVtbl[585] = (void*)G_AddItem;
    int k = 0;
    for (int f = 0; f < 4; ++f) {
        bool added = (f & 1) != 0, played = (f & 2) != 0;
        Run("1", "1", "", "", "", added, played, false, ++k);                                   // started, terminating at once
        Run("1", "0", "", "", "", added, played, true, ++k);                                    // not started, terminating
        Run("01", "001", "", "", "", added, played, false, ++k);                                // retries, terminating
        Run("0001", "001", "", "", "0", added, played, false, ++k);                             // first spawn allocation fails
        Run("01", "1", "", "", "", added, played, false, ++k);                                  // terminating after the loop
        Run("001", "1", "", "", "", added, played, false, ++k);
        Run("0001", "1", "01", "", "", added, played, true, ++k);
        Run("00001", "1", "001", "", "", added, played, false, ++k);
        Run("000001", "1", "1", "", "", added, played, false, ++k);
        Run("0000001", "1", "1", "", "10", added, played, true, ++k);                           // second spawn allocation fails
        Run("00000001", "1", "1", "1101", "", added, played, false, ++k);
        Run("000000001", "1", "1", "11", "", added, played, false, ++k);
        Run("0000000000001", "1", "1", "1", "", added, played, true, ++k);
        Run("00000000000000001", "0101", "001", "111", "", added, played, false, ++k);
    }
    unsigned long seed = 71;
    for (int r = 0; r < 20000; ++r) {
        static char ts[64], st[16], sh[16], pf[16], nw[4];
        for (int i = 0; i < 63; ++i) { seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 13 == 0) ? '1' : '0'; }
        for (int i = 0; i < 15; ++i) { seed = seed * 1103515245ul + 12345ul; st[i] = ((seed >> 16) % 3 == 0) ? '1' : '0'; }
        for (int i = 0; i < 15; ++i) { seed = seed * 1103515245ul + 12345ul; sh[i] = ((seed >> 16) % 3 == 0) ? '1' : '0'; }
        for (int i = 0; i < 15; ++i) { seed = seed * 1103515245ul + 12345ul; pf[i] = ((seed >> 16) % 2 == 0) ? '1' : '0'; }
        for (int i = 0; i < 3; ++i) { seed = seed * 1103515245ul + 12345ul; nw[i] = ((seed >> 16) % 5 == 0) ? '0' : '1'; }
        ts[63] = st[15] = sh[15] = pf[15] = nw[3] = 0;
        seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 8);
        Run(ts, st, sh, pf, nw, (v & 1) != 0, (v & 2) != 0, (v & 4) != 0, 1000 + r);
    }
    if (failures) { printf("00ec4a00_TEST FAIL %d\n", failures); return 1; }
    printf("00ec4a00_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}