// Behaviour test for 0x00E9E420 NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main() (links the REAL row object: the row body, the
// expanded CTimer / CScriptThing / CIsThingAlive / sehMe / seh_hero / movie / pause bodies, the ScriptedThingBase and movie dtor COMDATs, the
// cast map's ctor / operator[] (lower_bound walk) / dtor (_M_erase) COMDATs and the ScriptedThingBase assignment all run as compiled). No class
// models: every external the row object names is bound with /alternatename -- the calls to extern "C" __fastcall stubs (thiscall-compatible,
// unused edx), everything else to a trap. Game (+0x04) slots 7, 8, 70, 72, 74, 264, 352, 361, 364..366, 370, 371, 379, 475, 622, 626 and
// pDSTGame slots 87..90 (a SEPARATE object) through fake vtables; things carry real counted-pointer info blocks {RefCount, DeleteFunc, Data}
// that the row releases itself, and their implementations a vtable whose slot 75 is IsAlive. The map's insert_unique is a stub that links a
// real node into the row's own tree (the row's _M_erase frees it). ParentClass (+0x14) and the script def are raw buffers at the header
// offsets (+0x48..+0x60; +0x69C, +0x6A8, +0x6AC, +0x6C4, +0x6C8, +0x6D4); AnimTimer is the member at +0x1C.
// Reference model: Model() below, written from the script's statements; every run is compared event by event (with the destruction order of
// the live locals on each early return) plus the final state of every ParentClass field and of every string, thing, handle and timer.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:__purecall=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CBaseClass@@QAE@XZ=@BaseClassCtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@ABV0@@Z=@BOCopy@12")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@BOCtor@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@ABV0@@Z=@StrCopy@12")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=@BaseClassDtor@8")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@BODtor@8")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:??1CIsThingAlive@NScript@@UAE@XZ=@FilterDtor@8")
#pragma comment(linker, "/alternatename:??4CBaseObject@@IAEAAV0@ABV0@@Z=@BOAssign@12")
#pragma comment(linker, "/alternatename:??MCCharString@@QBE_NABV0@@Z=@StrLess@12")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptThing@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptThing@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?AskHeroQuestion@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDABVCCharString@@22_N3@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActions@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=@ClearCmd@8")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Converse@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@PBD@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DecrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DropGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GDefaultToUsingPhysicsInCutsceneAnimations@NGlobalConsole@@3_NA=_g_GDefault")
#pragma comment(linker, "/alternatename:?GetActivationTriggerStatus@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetAngleXY@CScriptThing@@UBEMXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentMapName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentScriptPriority@CScriptThing@@UBE?AW4EScriptAIPriority@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentStateGroupType@CScriptThing@@UBE?AW4EAIStateGroupType@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetHomeMapName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetHomePos@CScriptThing@@UBE?AVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetName@CScriptThing@@UBEABVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPThing@CScriptThing@@UBEPAVCThing@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPThingUniqueID@CScriptThing@@UBE_KXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPos@CScriptThing@@UBEABVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetScriptCounter@CScriptThing@@UAEJXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=@GetScriptThing@12")
#pragma comment(linker, "/alternatename:?IncrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:?IsAwareOfHero@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDead@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@Dist@12")
#pragma comment(linker, "/alternatename:?IsEqualTo@CScriptThing@@MBE_NABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsNull@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsOpenDoor@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=@IsPerf@8")
#pragma comment(linker, "/alternatename:?IsSneaking@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsSummonedCreature@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUnconscious@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUsable@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@IsValidStub@8")
#pragma comment(linker, "/alternatename:?MoveToAndPickUpGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXAAVCScriptThing@@W4EScriptEntityMoveType@@_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgAttackedWithBareHandsWithoutHittingAnything@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgAttackedWithMeleeWeaponWithoutHittingAnything@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgExpressionPerformedTo@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgGetThingsKilled@CScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithBareHands@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithMeleeWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithRangedWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithBareHands@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithMeleeWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithRangedWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHowLongWasExpressionPerformed@CScriptThing@@UBEJXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyNonAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithDecapitate@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithFlourish@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithProjectileWeapon@CScriptThing@@UBE_NABVCCharString@@AAM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithWeapon@CScriptThing@@UBE_NABVCCharString@@0@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKicked@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKilledBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKnockedOutBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsTalkedToBy@CScriptThing@@UBE_NABVCCharString@@@Z=@TalkedTo@12")
#pragma comment(linker, "/alternatename:?MsgIsTriggeredBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsUsedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOnCutSceneAnimEvent@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOnMoralityChanged@CScriptThing@@UBE_NAAJ@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOpenedChest@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnyAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnyNonAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnySpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedFlourish@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedSpecialAbility@CScriptThing@@UBE_NW4EHeroAbility@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedSuccessfulFlourish@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgReceivedInventoryItem@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgReceivedMoney@CScriptThing@@UBE_NAAJ@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgTalkedToAnyone@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoExpressedToMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoHitMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoKilledMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoKnockedMeOut@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoTalkedToMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PerformExpression@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z=@PlayAnim@40")
#pragma comment(linker, "/alternatename:?PlayCombatAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N11111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@J_N111111@Z=@PlayLoop@44")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@ReleaseStub@8")
#pragma comment(linker, "/alternatename:?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@PAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@3@PAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@3@_N4@Z=@RunCut@24")
#pragma comment(linker, "/alternatename:?SetActivationTriggerStatus@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetAsUsable@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetFriendsWithEverythingFlag@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@SetPred@12")
#pragma comment(linker, "/alternatename:?SetToKillOnLevelUnload@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=@Speak@32")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UnsheatheWeapons@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UpdateThingAttachment@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@2@@Z=@InsUnique@20")
#pragma comment(linker, "/alternatename:?pDSTGame@@3PAVCGameScriptInterfaceBase@@A=_g_pDSTGame")
#pragma comment(linker, "/alternatename:?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A=_g_ScriptDefHandle")
#pragma comment(linker, "/alternatename:_g_STVtbl=??_7CScriptThing@@6B@")
#pragma comment(linker, "/alternatename:_g_MovieVtbl=??_7CScriptGameResourceObjectMovieBase@@6B@")
#pragma comment(linker, "/alternatename:_g_SehVtbl=??_7CScriptGameResourceObjectScriptedThingBase@@6B@")
#pragma comment(linker, "/alternatename:_g_FilterVtbl=??_7CIsThingAlive@NScript@@6B@")
#pragma comment(linker, "/alternatename:@CallRow@8=?Main@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ")
extern "C" char g_STVtbl, g_MovieVtbl, g_SehVtbl, g_FilterVtbl;
extern "C" void __fastcall CallRow(void* self, void*);

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; char dflt; };
static char NextC(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c; }
static bool Next(Script& sc) { return NextC(sc) == '1'; }
enum { NS = 9 };
static Script sTerm, sAlive, sStart, sPerf, sSpeak, sTimer, sRand, sDist, sTalk;
// past the end: terminating / dead / started / idle / dead / timer running / rand 0 / far / not talked to

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 30000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static void Ws(const char* s) { while (*s) W(*s++); }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x40]; double a; } self;                 // CBriarRose (0x20)
static union { unsigned char b[0x80]; double a; } parent;               // CV_IntroductionToTrophiesScript (0x6C)
static union { unsigned char b[0x700]; double a; } def;                 // CScriptDef (fields to +0x6D8)
extern "C" void* g_ScriptDefHandle = def.b;                             // CDefPointer<const CScriptDef> {Object}
extern "C" bool g_GDefault = false;
static void* gameVtbl[700]; static void* gameObj[1];
static void* dstVtbl[100]; static void* dstObj[1];
extern "C" void* g_pDSTGame = dstObj;
static void* meVtbl[80]; static void* implVtbl[80];
struct Impl { void** vt; char who; };
static Impl implMe = { implVtbl, 'm' }, implLook = { implVtbl, 'l' }, implHero = { implVtbl, 'h' }, implVil = { implVtbl, 'v' }, implSeh = { implVtbl, 's' };
struct Info { long RefCount; void* DeleteFunc; void* Data; };
static Info meInfo, heroInfo; static int liveInfos;
extern "C" void __fastcall DelFunc(void* data) { Ev('X'); (void)data; }
static Info* NewInfo(void* data) { Info* i = (Info*)malloc(sizeof(Info)); i->RefCount = 1; i->DeleteFunc = (void*)DelFunc; i->Data = data; ++liveInfos; return i; }
void __cdecl operator delete(void* p) { if (p == &meInfo || p == &heroInfo) VirtualTrap(); free(p); --liveInfos; }
static void* hero[3];
static void FillThing(void** t, Impl* impl) { t[0] = &g_STVtbl; t[1] = impl; t[2] = NewInfo(impl); }
static void ReleaseInfo(Info* i) { if (i && --i->RefCount == 0) { ((void (__fastcall*)(void*))i->DeleteFunc)(i->Data); operator delete(i); } }
static void** Me() { return (void**)(self.b + 8); }
#define P_INTRO   (parent.b[0x48])
#define P_TROPHY  (parent.b[0x49])
#define P_ATTACKED (parent.b[0x4A])
#define P_APPR1   (parent.b[0x4B])
#define P_APPR2   (parent.b[0x4C])
#define P_ATTSPEECH (parent.b[0x4D])
#define P_HITS    (*(long*)(parent.b + 0x50))
#define P_HITWARN (parent.b[0x54])
#define P_TAVERN  (parent.b[0x55])
#define P_OPCUR   (*(long*)(parent.b + 0x58))
#define P_OPREQ   (*(long*)(parent.b + 0x5C))
#define P_CUTSCENE (parent.b[0x60])
#define D_DEED    (*(long*)(def.b + 0x69C))
#define D_NEAR    (*(float*)(def.b + 0x6A8))
#define D_FAR     (*(float*)(def.b + 0x6AC))
#define D_ANIM    (*(long*)(def.b + 0x6C4))
#define D_ANIMVAR (*(long*)(def.b + 0x6C8))
#define D_HITMAX  (*(long*)(def.b + 0x6D4))

// strings: one text pointer, logged by a letter per literal
static int liveStrings;
static char Lit(const char* t)
{
    static const char* k[] = { "M_TrophiesLookAtMe", "OPINION_SOURCE_BRIAR_ROSE_LOUD", "OPINION_SOURCE_BRIAR_ROSE_MEDIUM", "OPINION_SOURCE_BRIAR_ROSE_QUIET", "",
        "ST_IDLE", "ST_IDLE_SUBTLE", "SCRIPT_TALK_IDLE_01", "SCRIPT_TALK_IDLE_02", "SCRIPT_TALK_IDLE_03", "SCRIPT_LISTEN_BASE_IN", "SCRIPT_LISTEN_BASE_LOOP",
        "SCRIPT_LISTEN_BASE_OUT", "SCRIPT_FOLD_ARMS_IN", "SCRIPT_FOLD_ARMS_LOOP", "SCRIPT_FOLD_ARMS_OUT", "TrophiesIntroWatcher",
        "TEXT_QST_038_BRIAR_APPROACH_01", "TEXT_QST_038_BRIAR_APPROACH_02", "SCRIPT_NAME_HERO", "ROSE", "HERO", "CS_TROPHIES_INTRO", "CS_TROPHIES_TROPHY" };
    static const char l[] = "Mlmq0123456789ABVxyhrHit";
    if (!t) return '?';
    for (int i = 0; i < 24; ++i) if (!strcmp(t, k[i])) return l[i];
    return '?';
}
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; check(len == -1, "string length", 0); Ev('s'); Ev(Lit(text)); return s; }
extern "C" void* __fastcall StrCopy(const char** s, void*, const char* const* o) { *s = *o; ++liveStrings; Ev('c'); Ev(Lit(*o)); return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; Ev('~'); Ev(Lit(*s)); *s = 0; }
extern "C" bool __fastcall StrLess(const char* const* a, void*, const char* const* b) { return strcmp(*a, *b) < 0; }
extern "C" void __fastcall BaseClassCtor(void*, void*) { }
extern "C" void __fastcall BaseClassDtor(void*, void*) { }

// things: copies share the info block
static int liveThings;
extern "C" void* __fastcall ThingCopy(void** t, void*, void* const* o) { t[0] = o[0]; t[1] = o[1]; t[2] = o[2]; if (t[2]) ++((Info*)t[2])->RefCount; ++liveThings; Ev('y'); return t; }
extern "C" bool __fastcall Impl_IsAlive(Impl* i, void*) { check(i->vt == implVtbl, "IsAlive on a thing implementation", 0); bool r = Next(sAlive); Ev('L'); Ev(i->who); Ev(r ? '1' : '0'); return r; }
static char Who(void* const* t) { return t == (void* const*)Me() ? 'M' : t == (void* const*)hero ? 'h' : (t && t[1]) ? ((Impl*)t[1])->who : '?'; }

// predicate {vptr, Thing}
static void* filterAddr;
extern "C" void __fastcall SetPred(void* ent, void*, void** f)
{
    check(ent == self.b && f[0] == (void*)&g_FilterVtbl && f[2] == &implMe && f[3] == &meInfo, "predicate: CIsThingAlive(Me)", 0);
    filterAddr = f; Ev('e');
}
extern "C" void __fastcall FilterDtor(void** f, void*) { check(f == filterAddr, "predicate destroyed", 0); ReleaseInfo((Info*)f[3]); --liveThings; Ev('x'); }

// ScriptedThingBase {vptr, IntelligentPointer, Data, Info}; movie {vptr, IntelligentPointer, PImp, Info}
static void* sehMe; static void* sehHero; static int bo;
static char Seh(void* h) { return h && h == sehMe ? 'R' : h && h == sehHero ? 'r' : '?'; }
extern "C" void* __fastcall BOCtor(void** o, void*) { if (!sehMe) sehMe = o; o[1] = 0; ++bo; Ev('o'); return o; }
extern "C" void* __fastcall BOCopy(void** o, void*, void* const* src) { (void)src; o[1] = 0; ++bo; Ev('B'); return o; }
extern "C" void* __fastcall BOAssign(void** o, void*, void* const* src) { (void)src; Ev('a'); return o; }
extern "C" void __fastcall BODtor(void** o, void*) { (void)o; --bo; Ev('O'); }
extern "C" bool __fastcall IsValidStub(void* const* h, void*) { bool r = h[2] != 0; Ev('v'); Ev(Seh((void*)h)); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall ReleaseStub(void** h, void*) { check(h[3] != 0, "Release on a held handle", 0); Ev('-'); ReleaseInfo((Info*)h[3]); h[2] = h[3] = 0; }
extern "C" void* __fastcall GetScriptThing(void* const* h, void*, void** ret) { check(h == sehMe, "GetScriptThing on sehMe", 0); FillThing(ret, &implSeh); ++liveThings; Ev(Seh((void*)h)); return ret; }
extern "C" bool __fastcall IsPerf(void* h, void*) { bool r = Next(sPerf); Ev(Seh(h)); Ev(r ? 'Q' : 'q'); return r; }
extern "C" void __fastcall ClearCmd(void* h, void*) { Ev('E'); Ev(Seh(h)); }
static char TextLetter(const char* t)
{
    static const char* keys[4] = { "TEXT_QST_038_BRIAR_WARNING", "TEXT_QST_038_BRIAR_SNIFFY", "TEXT_QST_038_BRIAR_BLATHER", "TEXT_QST_038_BRIAR_ATTACK" };
    static const char let[4] = { 'w', 's', 'b', 'a' };
    for (int i = 0; i < 4; ++i) if (!strcmp(t, keys[i])) return let[i];
    return '?';
}
extern "C" void __fastcall Speak(void* h, void*, void* const* to, const char* text, long sel, int b1, int b2, int b3)
{
    char L = TextLetter(text);
    check(h == sehMe && to == (void* const*)hero && sel == (L == 'a' ? 0 : 2) && (b1 & 0xFF) == 0 && (b2 & 0xFF) == 1 && (b3 & 0xFF) == 0, "speak args", 0);
    Ev('K'); Ev(L);
}
static bool AnimOut(char a) { return a == '8' || a == 'B'; }
extern "C" void __fastcall PlayAnim(void* h, void*, const char* const* a, int b1, int b2, int b3, int b4, int b5, int b6, int b7)
{
    char L = Lit(*a);
    check(h == sehMe && !(b1 & 0xFF) && !(b2 & 0xFF) && (b3 & 0xFF) == 1 && (b4 & 0xFF) == (AnimOut(L) ? 1 : 0) && (b5 & 0xFF) == (g_GDefault ? 1 : 0)
          && !(b6 & 0xFF) && !(b7 & 0xFF), "PlayAnimation args", 0);
    Ev('P'); Ev(L);
}
extern "C" void __fastcall PlayLoop(void* h, void*, const char* const* a, long n, int b1, int b2, int b3, int b4, int b5, int b6, int b7)
{
    check(h == sehMe && n == 1 && !(b1 & 0xFF) && !(b2 & 0xFF) && (b3 & 0xFF) == 1 && !(b4 & 0xFF) && (b5 & 0xFF) == (g_GDefault ? 1 : 0)
          && !(b6 & 0xFF) && !(b7 & 0xFF), "PlayLoopingAnimation args", 0);
    Ev('p'); Ev(Lit(*a));
}
extern "C" bool __fastcall Dist(void* const* a, void* const* b, float d)
{
    check(a == (void* const*)Me() && b == (void* const*)hero && (d == D_FAR || d == D_NEAR), "distance Me - hero", 0);
    bool r = Next(sDist); Ev('D'); Ev(d == D_FAR ? 'f' : 'n'); Ev(r ? '1' : '0'); return r;
}
extern "C" bool __fastcall TalkedTo(void* const* t, void*, const char* const* who)
{
    check(t == (void* const*)Me() && !strcmp(*who, "SCRIPT_NAME_HERO"), "Me.MsgIsTalkedToBy(SCRIPT_NAME_HERO)", 0);
    bool r = Next(sTalk); Ev('!'); Ev(r ? '1' : '0'); return r;
}
extern "C" int __cdecl rand() { char c = NextC(sRand); int v = (c - '0') + 8 * 1237; Ev('r'); Ev(c); return v; }

// the cast map: {header, node count, compare}; nodes {color, parent, left, right, key, value[4]}
struct Node { long color; Node* parent; Node* left; Node* right; const char* key; void* val[4]; };
static int nodes;
extern "C" void** __fastcall InsUnique(void** tree, void*, void** ret, Node* hint, void** pair)
{
    Node* h = (Node*)tree[0]; check(hint == h || hint != 0, "insert hint", 0);
    Node* n = (Node*)malloc(sizeof(Node)); ++nodes;
    n->key = (const char*)pair[0]; ++liveStrings; Ev('I'); Ev(Lit(n->key));
    n->val[0] = pair[1]; n->val[1] = 0; n->val[2] = pair[3]; n->val[3] = pair[4]; ++bo; if (n->val[3]) ++((Info*)n->val[3])->RefCount;
    n->color = 0; n->left = n->right = 0;
    if (!h->parent) { h->parent = n; n->parent = h; h->left = h->right = n; }
    else {
        Node* p = h->parent;
        for (;;) {
            if (strcmp(n->key, p->key) < 0) { if (!p->left) { p->left = n; break; } p = p->left; }
            else { if (!p->right) { p->right = n; break; } p = p->right; }
        }
        n->parent = p;
        if (h->left->left == n) h->left = n;
        if (h->right->right == n) h->right = n;
    }
    ++*(long*)(tree + 1);
    ret[0] = n; return ret;
}
static void* cutMap; static char cutName;
extern "C" void __fastcall RunCut(const char* const* name, void** map, void* p1, void* p2, int b1, int b2)
{
    check(!p1 && !p2 && !(b1 & 0xFF) && (b2 & 0xFF) == 1 && P_CUTSCENE == 1, "RunCutsceneMacro(name, mymap, 0, 0, false, true) with CutsceneStarted", 0);
    Node* h = (Node*)map[0]; long count = *(long*)(map + 1);
    bool ok = count == 2 && h->parent && h->left && h->right;
    if (ok) {
        Node* hn = h->left; Node* rn = h->right;
        ok = !strcmp(hn->key, "HERO") && !strcmp(rn->key, "ROSE") && rn->val[0] == (void*)&g_SehVtbl && hn->val[0] == (void*)&g_SehVtbl
             && rn->val[2] == ((void**)sehMe)[2] && rn->val[3] == ((void**)sehMe)[3] && hn->val[2] == ((void**)sehHero)[2] && hn->val[3] == ((void**)sehHero)[3];
    }
    check(ok, "cast map {HERO: seh_hero, ROSE: sehMe}", 0);
    Ev('W'); Ev(Lit(*name));
}

// pDSTGame timers: AnimTimer is the member (index 0x40), the two locals are registered in order
static int liveTimers; static long nextTimer, timerVal[3];
static char TimerL(long i) { return i == 0x40 ? 'c' : i == 0x51 ? 'a' : i == 0x52 ? 'b' : '?'; }
static long __fastcall D_Register(void* t, void*) { check(t == dstObj, "timer via pDSTGame", 0); ++liveTimers; long i = nextTimer++; Ev('['); Ev(TimerL(i)); return i; }
static void __fastcall D_Deregister(void* t, void*, long i) { check(t == dstObj, "DeregisterTimer via pDSTGame", 0); --liveTimers; Ev(']'); Ev(TimerL(i)); }
static void __fastcall D_Set(void* t, void*, long i, long v) { check(t == dstObj, "SetTimer via pDSTGame", 0); char L = TimerL(i); if (L != '?') timerVal[L == 'c' ? 2 : L - 'a'] = v; Ev('='); Ev(L); }
static long TimerValue(char c) { return c == '0' ? 0 : c == '2' ? -3 : 7; }
static long __fastcall D_Get(void* t, void*, long i) { check(t == dstObj, "GetTimer via pDSTGame", 0); char c = NextC(sTimer); Ev('?'); Ev(TimerL(i)); Ev(c); return TimerValue(c); }

// Game
static void G(void* g) { check(g == gameObj, "Game receiver", 0); }
static void* lookAddr;
static bool __fastcall IsTerm_(void* s) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall IsTerm(void* s, void*) { return IsTerm_(s); }
static void __fastcall G_Frame(void* g, void*) { G(g); Ev('F'); }
static bool __fastcall G_Start(void* g, void*, void* const* thing, void** h, long prio)
{
    G(g); char w = Who(thing); check((w == 'M' || w == 'h') && prio == 4, "start scripting Me / the hero at priority 4", 0);
    if (w == 'M') check(h == sehMe, "Me started into sehMe", 0); else sehHero = h;
    bool r = Next(sStart); Ev(r ? 'S' : 'n'); Ev(w);
    if (r) { check(!h[2] && !h[3], "started into an empty handle", 0); Info* i = NewInfo(h); h[2] = (void*)&implSeh; h[3] = i; }
    return r;
}
static void* __fastcall G_GetHero(void* g, void*) { G(g); Ev('Y'); return hero; }
static void* __fastcall G_GetThing(void* g, void*, void** ret, const char* const* n)
{
    G(g); check(Lit(*n) == 'M', "GetThingWithScriptName(M_TrophiesLookAtMe)", 0); FillThing(ret, &implLook); ++liveThings; lookAddr = ret; Ev('G'); return ret;
}
static void* __fastcall G_GetRandom(void* g, void*, void** ret, const char* const* n)
{
    G(g); check(Lit(*n) == 'V', "GetRandomThingWithScriptName(TrophiesIntroWatcher)", 0); FillThing(ret, &implVil); ++liveThings; Ev('V'); return ret;
}
static float __fastcall G_Health(void* g, void*, void* const* t) { G(g); check(t[1] == &implSeh, "health of sehMe's script thing", 0); bool a = Next(sSpeak); Ev(a ? 'U' : 'u'); return a ? 2.0f : 0.0f; }
static void __fastcall G_Facing(void* g, void*, void* const* a, void* const* b, int f) { G(g); check(a == (void* const*)Me() && b == lookAddr && !(f & 0xFF), "EntitySetFacingAngleTowardsThing(Me, LookAtMe, false)", 0); Ev('A'); }
static void __fastcall G_OpinionSource(void* g, void*, void* const* t, const char* const* s) { G(g); check(t == (void* const*)Me(), "opinion source on Me", 0); Ev('H'); Ev(Lit(*s)); }
static void __fastcall G_StartMovie(void* g, void*, const char* const* name, void* const* movie)
{
    G(g); check(!strcmp(*name, "") && movie[0] == (void*)&g_MovieVtbl && !movie[2] && !movie[3], "movie sequence args", 0); Ev('m');
}
static int pauseDepth;
static void __fastcall G_Pause(void* g, void*, int on) { G(g); pauseDepth += (on & 0xFF) ? 1 : -1; Ev((on & 0xFF) ? 'Z' : 'z'); }
static void* convVil;
static long __fastcall G_NewConv(void* g, void*, void* const* t, int a, int b) { G(g); check(Who(t) == 'v' && !(a & 0xFF) && !(b & 0xFF), "AddNewConversation(villager, false, false)", 0); convVil = (void*)t; Ev('C'); return 0x33; }
static void __fastcall G_AddPerson(void* g, void*, long c, void* const* t) { G(g); check(c == 0x33 && t == (void* const*)hero, "AddPersonToConversation(index, hero)", 0); Ev('J'); }
static void __fastcall G_AddLine(void* g, void*, long c, const char* const* s, int f, void* const* a, void* const* b)
{
    G(g); check(c == 0x33 && !(f & 0xFF) && a == convVil && b == (void* const*)hero, "AddLineToConversation(index, text, false, villager, hero)", 0); Ev('j'); Ev(Lit(*s));
}
static void __fastcall G_FixCam(void* g, void*, int on) { G(g); Ev('f'); Ev((on & 0xFF) ? '1' : '0'); }
static void __fastcall G_ClearInfo(void* g, void*, void* const* t) { G(g); check(t == (void* const*)Me(), "ClearThingHasInformation(Me)", 0); Ev('i'); }
static void __fastcall G_MiniMap(void* g, void*, void* const* t) { G(g); check(t == (void* const*)Me(), "MiniMapRemoveMarker(Me)", 0); Ev('k'); }
static void __fastcall G_Deed(void* g, void*, void* const* t, long d) { G(g); check(t == (void* const*)Me() && d == 56, "EntityPostOpinionDeedToAll(Me, SHOW_TROPHY_GOOD)", 0); Ev('d'); }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mAlive, mStart, mPerf, mSpeak, mTimer, mRand, mDist, mTalk;
struct State { unsigned char intro, trophy, attacked, appr1, appr2, attspeech, hitwarn, tavern, cutscene; long hits, opcur, opreq; } m;
static long mAnimTimer, mDeedTimer;
static bool mHeld;                                                       // sehMe holds a thing
static int cOpinion, cWarn, cAnim[8], cAppr1, cAppr2, cConv, cSniffy, cIntro, cTrophy, cBlather, cBlather2, cAttack, cDeed, cRel;
static char live[24][4]; static int nlive;                               // destruction events of the live locals, innermost last
static void Push(const char* d) { strcpy(live[nlive++], d); }
static void Pop() { --nlive; Ws(live[nlive]); }
static void SetSehLive() { for (int i = nlive - 1; i >= 0; --i) if (live[i][0] == 'X' || live[i][0] == 'O') { if (!strcmp(live[i], "XO") || !strcmp(live[i], "O")) { strcpy(live[i], mHeld ? "XO" : "O"); return; } } }
static int sehSlot;
static void SehLive() { strcpy(live[sehSlot], mHeld ? "XO" : "O"); }
static bool T() { bool r = Next(mTerm); W(r ? 'T' : 't'); if (r) while (nlive) Pop(); return r; }
static void F() { W('F'); }
static bool B(Script& s, const char* tag) { bool r = Next(s); Ws(tag); W(r ? '1' : '0'); return r; }
static bool Alive(char who) { bool r = Next(mAlive); W('L'); W(who); W(r ? '1' : '0'); return r; }
static void Str(char l) { W('s'); W(l); }
static void UnStr(char l) { W('~'); W(l); }
static bool PerfLoop() { for (;;) { bool pf = Next(mPerf); W('R'); W(pf ? 'Q' : 'q'); if (!pf) return false; F(); if (T()) return true; } }
static bool Say(char text)                                               // health test + speak-and-wait; true = returned
{
    W('R'); bool sp = Next(mSpeak); W(sp ? 'U' : 'u'); W('X');
    if (!sp) return false;
    Ws("YK"); W(text);
    if (PerfLoop()) return true;
    return T();
}
static void ReleaseIfValid() { Ws("vR"); W(mHeld ? '1' : '0'); if (mHeld) { ++cRel; Ws("-X"); mHeld = false; SehLive(); } }
static bool StartMe() { for (;;) { bool r = Next(mStart); W(r ? 'S' : 'n'); W('M'); if (r) { mHeld = true; SehLive(); return false; } F(); if (T()) return true; } }
static long Timer(char which) { char c = NextC(mTimer); W('?'); W(which); W(c); return TimerValue(c); }
static void Movie() { W('o'); Push("O"); Str('0'); W('m'); UnStr('0'); W('Z'); Push("z"); }
static bool mHeroHeld;
static void Cutscene(char name, bool trophy)                                          // seh_hero, mymap["ROSE"]=sehMe, mymap["HERO"]=seh_hero, the cut scene
{
    W('o');                                                              // seh_hero
    W('Y'); mHeroHeld = Next(mStart); W(mHeroHeld ? 'S' : 'n'); W('h');
    // mymap["ROSE"]=sehMe: key temporary, operator[] inserts value_type(k, V()) (V() then the pair's copies), the pair and V() die,
    // then the handle assignment (CBaseObject::operator=), then the key temporary dies
    Str('r'); W('o'); Ws("crB"); Ws("Ir"); Ws("O~r"); W('O'); W('a'); UnStr('r');
    Str('H'); W('o'); Ws("cHB"); Ws("IH"); Ws("O~H"); W('O'); W('a'); UnStr('H');
    Ws("f1"); Str(name); W('W'); W(name); UnStr(name); Ws("f0");
    if (trophy) Ws("ik");                                                // ClearThingHasInformation(Me), MiniMapRemoveMarker(Me)
    // block end: mymap (_M_erase: ROSE node then HERO node: value then key), then seh_hero
    Ws("O~r"); Ws("O~H");
    if (mHeroHeld) W('X'); W('O');
}
static void Model()
{
    nlive = 0;
    W('['); W('a'); Push("]a");                                          // deedTimer
    W('['); W('b'); Push("]b");                                          // standStillTimer (waypoint has no events)
    Ws("=c"); mAnimTimer = 0;
    Str('M'); W('G'); UnStr('M'); Push("X");                             // LookAtMe
    if (Alive('l')) { if (T()) return; W('A'); }
    Ws("=a"); mDeedTimer = D_DEED;
    Ws("yex");                                                           // SetPredicate(CIsThingAlive(Me))
    W('o'); Push("O"); sehSlot = nlive - 1; mHeld = false;               // sehMe
    ReleaseIfValid();
    if (StartMe()) return;
    if (T()) return;
    for (;; F()) {
        if (T()) return;
        if (m.opcur != m.opreq) {
            if (T()) return;
            static const char src[3] = { 'l', 'm', 'q' };
            if (m.opreq >= 0 && m.opreq <= 2) {
                if (T()) return;
                Str(src[m.opreq]); W('H'); W(src[m.opreq]); UnStr(src[m.opreq]);
                m.opcur = m.opreq; ++cOpinion;
            }
        }
        if (m.hits > D_HITMAX && m.attspeech && !m.hitwarn) {
            if (T()) return;
            ReleaseIfValid();
            if (StartMe()) return;
            if (T()) return;
            Movie();
            if (Say('w')) return;
            m.hitwarn = 1; ++cWarn;
            Pop(); Pop();
        }
        bool animNow = Timer('c') == 0;
        if (animNow) { Ws("vR"); W(mHeld ? '1' : '0'); animNow = mHeld; }
        if (animNow) {
            if (T()) return;
            Ws("ER");
            if (Alive('l')) { if (T()) return; W('A'); }
            if (PerfLoop()) return;
            if (T()) return;
            char rc = NextC(mRand); W('r'); W(rc); int idx = rc - '0';
            if (T()) return;
            ++cAnim[idx];
            static const char* const play[8] = { "P1", "P2", "P3", "P4", "P5", "", "P6p7P8", "P9pAPB" };
            for (const char* p = play[idx]; *p; p += 2) { Str(p[1]); W(p[0]); W(p[1]); UnStr(p[1]); }
            char rc2 = NextC(mRand); W('r'); W(rc2); Ws("=c"); mAnimTimer = D_ANIM + ((rc2 - '0') + 8 * 1237) % D_ANIMVAR;
        }
        W('Y'); bool heroAlive = Alive('h');
        if (heroAlive && m.tavern) {
            if (T()) return;
            for (int pass = 0; pass < 2; ++pass) {
                unsigned char& done = pass ? m.appr2 : m.appr1;
                if (done) continue;
                W('Y'); bool near = Next(mDist); W('D'); W(pass ? 'n' : 'f'); W(near ? '1' : '0');
                if (!near) continue;
                if (T()) return;
                Str('V'); W('V'); UnStr('V'); Push("X");                     // villager
                if (Alive('v')) {
                    if (T()) return;
                    W('C'); Ws("YJ"); Str(pass ? 'y' : 'x'); W('Y'); W('j');   // VC builds the CCharString argument before the GetHero() call
                    W(pass ? 'y' : 'x'); UnStr(pass ? 'y' : 'x'); ++cConv;
                }
                Pop();
                done = 1; if (pass) ++cAppr2; else ++cAppr1;
            }
        }
        Str('h'); bool talked = Next(mTalk); W('!'); W(talked ? '1' : '0'); UnStr('h');
        if (talked) {
            if (T()) return;
            Ws("ER");
            Movie();
            if (PerfLoop()) return;
            if (T()) return;
            if (m.attacked && m.attspeech) {
                if (T()) return;
                ReleaseIfValid();
                if (StartMe()) return;
                if (T()) return;
                if (Say('s')) return;
                ++cSniffy;
            } else if (!m.intro) {
                if (T()) return;
                m.cutscene = 1;
                ReleaseIfValid();
                if (StartMe()) return;
                if (T()) return;
                Cutscene('i', false);
                m.cutscene = 0; m.intro = 1; ++cIntro;
            } else if (!m.trophy) {
                if (T()) return;
                m.cutscene = 1;
                ReleaseIfValid();
                if (StartMe()) return;
                if (T()) return;
                Cutscene('t', true);
                m.cutscene = 0; m.trophy = 1; ++cTrophy;
            } else {
                if (m.trophy == 1) ++cBlather; else ++cBlather2;         // the fitted read: both outcomes the same terminate check
                if (T()) return;
                ReleaseIfValid();
                if (StartMe()) return;
                if (T()) return;
                if (Say('b')) return;
            }
            Pop(); Pop();
        } else if (m.attacked) {
            if (T()) return;
            if (!m.attspeech) {
                if (T()) return;
                W('i');
                ReleaseIfValid();
                if (StartMe()) return;
                if (T()) return;
                Movie();
                if (Say('a')) return;
                m.attspeech = 1; ++cAttack;
                Pop(); Pop();
            }
        }
        if (Timer('a') <= 0) {
            if (T()) return;
            W('d'); Ws("=a"); mDeedTimer = D_DEED; ++cDeed;
        }
    }
}

struct Init { unsigned char intro, trophy, attacked, appr1, appr2, attspeech, hitwarn, tavern, cutscene; long hits, opcur, opreq; bool gdef; };
static void Run(const char* const* in, const Init& st, int k)
{
    memset(self.b, 0xEE, sizeof self.b); memset(parent.b, 0xDD, sizeof parent.b);
    *(void**)(self.b + 0x00) = (void*)0x5E1F; *(void**)(self.b + 0x04) = gameObj;
    Me()[0] = meVtbl; Me()[1] = &implMe; Me()[2] = &meInfo;
    *(void**)(self.b + 0x14) = parent.b; *(void**)(self.b + 0x18) = (void*)0x4D41; *(long*)(self.b + 0x1C) = 0x40;
    P_INTRO = st.intro; P_TROPHY = st.trophy; P_ATTACKED = st.attacked; P_APPR1 = st.appr1; P_APPR2 = st.appr2; P_ATTSPEECH = st.attspeech;
    P_HITWARN = st.hitwarn; P_TAVERN = st.tavern; P_CUTSCENE = st.cutscene; P_HITS = st.hits; P_OPCUR = st.opcur; P_OPREQ = st.opreq;
    g_GDefault = st.gdef;
    liveStrings = liveInfos = liveTimers = pauseDepth = bo = 0; nextTimer = 0x51; timerVal[0] = timerVal[1] = timerVal[2] = 0x7777;
    Script* ss[NS] = { &sTerm, &sAlive, &sStart, &sPerf, &sSpeak, &sTimer, &sRand, &sDist, &sTalk };
    Script* ms[NS] = { &mTerm, &mAlive, &mStart, &mPerf, &mSpeak, &mTimer, &mRand, &mDist, &mTalk };
    static const char df[NS] = { '1', '0', '1', '0', '0', '1', '0', '0', '0' };
    for (int i = 0; i < NS; ++i) { ss[i]->s = in[i]; ss[i]->pos = 0; ss[i]->dflt = df[i]; *ms[i] = *ss[i]; }
    m.intro = st.intro; m.trophy = st.trophy; m.attacked = st.attacked; m.appr1 = st.appr1; m.appr2 = st.appr2; m.attspeech = st.attspeech;
    m.hitwarn = st.hitwarn; m.tavern = st.tavern; m.cutscene = st.cutscene; m.hits = st.hits; m.opcur = st.opcur; m.opreq = st.opreq;
    mAnimTimer = mDeedTimer = 0x7777;
    ngot = nwant = 0; sehMe = sehHero = 0; filterAddr = 0;
    meInfo.RefCount = 1; meInfo.DeleteFunc = (void*)VirtualTrap; meInfo.Data = &implMe;
    heroInfo.RefCount = 1; heroInfo.DeleteFunc = (void*)VirtualTrap; heroInfo.Data = &implHero;
    Model();
    CallRow(self.b, 0);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) { int d = 0; while (d < ngot && d < nwant && got[d] == want[d]) ++d; int s0 = d > 120 ? d - 120 : 0;
            printf("  diverge at %d\n  got  ..%.240s\n  want ..%.240s\n", d, got + s0, want + s0); }
    }
    check(P_INTRO == m.intro && P_TROPHY == m.trophy && P_ATTACKED == m.attacked && P_APPR1 == m.appr1 && P_APPR2 == m.appr2 && P_ATTSPEECH == m.attspeech
          && P_HITWARN == m.hitwarn && P_TAVERN == m.tavern && P_CUTSCENE == m.cutscene && P_HITS == m.hits && P_OPCUR == m.opcur && P_OPREQ == m.opreq,
          "ParentClass fields", k);
    check(parent.b[0x47] == 0xDD && parent.b[0x4E] == 0xDD && parent.b[0x4F] == 0xDD && parent.b[0x56] == 0xDD && parent.b[0x57] == 0xDD
          && parent.b[0x61] == 0xDD && parent.b[0x64] == 0xDD, "no other parent store", k);
    check(timerVal[2] == mAnimTimer && timerVal[0] == mDeedTimer, "AnimTimer / deedTimer values", k);
    check(meInfo.RefCount == 1 && heroInfo.RefCount == 1 && Me()[0] == meVtbl && Me()[1] == &implMe && Me()[2] == &meInfo, "Me / hero untouched", k);
    check(*(void**)(self.b + 0x14) == parent.b && *(void**)(self.b + 0x18) == (void*)0x4D41 && *(long*)(self.b + 0x1C) == 0x40 && self.b[0x20] == 0xEE,
          "no other store to this", k);
    check(liveStrings == 0 && liveInfos == 0 && liveTimers == 0 && pauseDepth == 0 && bo == 0, "everything released", k);
}
static void Run1(const char* term, const char* alive, const char* start, const char* perf, const char* speak, const char* timer, const char* rnd,
                 const char* dist, const char* talk, Init st, int k)
{
    const char* in[NS] = { term, alive, start, perf, speak, timer, rnd, dist, talk };
    Run(in, st, k);
}

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; dstObj[0] = dstVtbl; hero[0] = &g_STVtbl; hero[1] = &implHero; hero[2] = &heroInfo;
    for (int i = 0; i < 700; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 100; ++i) dstVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) meVtbl[i] = implVtbl[i] = (void*)VirtualTrap;
    meVtbl[27] = (void*)TalkedTo; implVtbl[75] = (void*)Impl_IsAlive;
    dstVtbl[87] = (void*)D_Register; dstVtbl[88] = (void*)D_Deregister; dstVtbl[89] = (void*)D_Set; dstVtbl[90] = (void*)D_Get;
    gameVtbl[7] = (void*)G_Frame; gameVtbl[8] = (void*)G_Start; gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThing;
    gameVtbl[74] = (void*)G_GetRandom; gameVtbl[264] = (void*)G_Health; gameVtbl[352] = (void*)G_MiniMap; gameVtbl[361] = (void*)G_ClearInfo;
    gameVtbl[364] = (void*)G_NewConv; gameVtbl[365] = (void*)G_AddPerson; gameVtbl[366] = (void*)G_AddLine; gameVtbl[370] = (void*)G_StartMovie;
    gameVtbl[371] = (void*)G_FixCam; gameVtbl[379] = (void*)G_Pause; gameVtbl[475] = (void*)G_Facing; gameVtbl[622] = (void*)G_OpinionSource;
    gameVtbl[626] = (void*)G_Deed;
    D_DEED = 900; D_NEAR = 4.0f; D_FAR = 9.5f; D_ANIM = 300; D_ANIMVAR = 7; D_HITMAX = 2;
    int k = 0;
    //  intro trophy attacked appr1 appr2 attspeech hitwarn tavern cutscene hits opcur opreq gdef
    Init s0 = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, false };
    //    term                      alive   start perf  speak timer  rand  dist talk
    Run1("1",                       "0",    "",   "",   "",   "",    "",   "",  "",  s0, ++k);                      // terminating at the look-at
    Run1("001",                     "1",    "01", "",   "",   "1",   "",   "",  "",  s0, ++k);                      // facing, one start retry
    Init s1 = s0; s1.opcur = 0; s1.opreq = 2;
    Run1("00001",                   "0",    "",   "",   "",   "1",   "",   "",  "",  s1, ++k);                      // opinion QUIET
    Init s2 = s0; s2.hits = 5; s2.attspeech = 1;
    Run1("0000000001",              "0",    "",   "1",  "1",  "11",  "",   "",  "",  s2, ++k);                      // hit warning
    Run1("0000000000000000001",     "01",   "",   "1",  "",   "0110","66", "",  "",  s0, ++k);                      // anim 6 (listen), deed post
    Init s3 = s0; s3.tavern = 1; s3.appr1 = 0; s3.appr2 = 0;
    Run1("00000000001",             "011",  "",   "",   "",   "11",  "",   "11","",  s3, ++k);                      // both approach messages
    Run1("0000000000000000000000001","00",  "11", "0",  "",   "11",  "",   "",  "1", s0, ++k);                      // talk: intro cut scene
    Init s4 = s0; s4.intro = 1;
    Run1("0000000000000000000000001","00",  "10", "0",  "",   "11",  "",   "",  "1", s4, ++k);                      // talk: trophy cut scene, hero not started
    Init s5 = s0; s5.intro = 1; s5.trophy = 1;
    Run1("0000000000000000000001",  "00",   "1",  "010","1",  "11",  "",   "",  "1", s5, ++k);                      // talk: blather
    Init s6 = s5; s6.trophy = 2;
    Run1("0000000000000000000001",  "00",   "1",  "010","1",  "11",  "",   "",  "1", s6, ++k);                      // blather, non-canonical flag byte 2
    Init s7 = s0; s7.attacked = 1;
    Run1("0000000000000000000001",  "00",   "1",  "0",  "1",  "11",  "",   "",  "",  s7, ++k);                      // attack speech
    Init s8 = s7; s8.attspeech = 1; s8.intro = 1;
    Run1("0000000000000000000001",  "00",   "1",  "0",  "1",  "11",  "",   "",  "1", s8, ++k);                      // talk: sniffy
    unsigned long seed = 1931;
    for (int r = 0; r < 20000; ++r) {
        static char b[NS][96]; const char* in[NS];
        static const int mod[NS] = { 37, 2, 4, 3, 3, 3, 8, 3, 3 };
        for (int j = 0; j < NS; ++j) {
            for (int i = 0; i < 95; ++i) {
                seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 16);
                char c;
                if (j == 5) c = (char)('0' + v % 3);                         // timer: 0 / 7 / -3
                else if (j == 6) c = (char)('0' + v % 8);                    // rand
                else if (j == 2) c = (v % mod[j] == 0) ? '0' : '1';          // start: mostly started
                else c = (v % mod[j] == 0) ? '1' : '0';
                b[j][i] = c;
            }
            b[j][95] = 0; in[j] = b[j];
        }
        seed = seed * 1103515245ul + 12345ul; unsigned f = (unsigned)(seed >> 8);
        Init st;
        st.intro = f & 1; st.trophy = (f >> 1) & 1 ? ((f >> 20) % 5 == 0 ? 2 : 1) : 0; st.attacked = (f >> 2) & 1; st.appr1 = (f >> 3) & 1; st.appr2 = (f >> 4) & 1;
        st.attspeech = (f >> 5) & 1; st.hitwarn = (f >> 6) & 1; st.tavern = (f >> 7) & 1; st.cutscene = (f >> 8) & 1; st.hits = (f >> 9) % 5;
        st.opcur = (f >> 12) % 4; st.opreq = (f >> 14) % 4; st.gdef = ((f >> 16) & 1) != 0;
        Run(in, st, 1000 + r);
    }
    printf("model paths: opinion %d, warning %d, anims %d/%d/%d/%d/%d/%d/%d/%d, approach %d/%d (conversation %d), sniffy %d, intro %d, trophy %d, blather %d (flag byte 2: %d), attack %d, deed %d, sehMe release %d\n",
           cOpinion, cWarn, cAnim[0], cAnim[1], cAnim[2], cAnim[3], cAnim[4], cAnim[5], cAnim[6], cAnim[7], cAppr1, cAppr2, cConv, cSniffy, cIntro, cTrophy,
           cBlather, cBlather2, cAttack, cDeed, cRel);
    if (failures) { printf("00e9e420_TEST FAIL %d\n", failures); return 1; }
    printf("00e9e420_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}