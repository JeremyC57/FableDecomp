// Behaviour test for 0x00E25F70 NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main (links the REAL row object: the row body, the
// CIsThingAlive ctor compiled in its inline_depth(0) region with the TU's implicit CScriptThing copy COMDAT, the expanded by-value
// CScriptThing copy for SetIsPushableByHero, the TU's implicit handle / movie special members (ctor, dtor, operator=, with their counted
// pointer releases), CWideScreenMagicPauseEntities and the TU's own map<CCharString,handle> operator[] / lower_bound / ~map / _M_erase
// all run as compiled). StartMovieSequence binds the movie to a heap pointer-info record; its delete function marks the movie's end.
// Fixtures (native layout, every offset read by an instruction of the row): the entity (0x28 bytes: Game +4, Me +8, ParentClass +0x14,
// TalkCounter +0x1c, WavedOver +0x20, HitCount +0x24); the parent (0x4C bytes: GuardianSpokeToHero +0x48); the global script def
// (GSI_MazeCallsOverDistance +0xc68); Me with a counted pointer-info record (the copies' reference counts are checked); things with
// fake vtables (GetPos 6, MsgIsHitBy 21, MsgIsTalkedToBy 27, MsgIsHitBySpecialAbilityFrom 41, MsgIsHitByAnySpecialAbilityFrom 42,
// SetFriendsWithEverythingFlag 67); handles with counted implementation records; an STLport map header and nodes; Game's vtable slots
// 7, 8, 70, 72, 108, 264, 281, 288, 350, 360, 361, 364, 365, 366, 370, 379, 452, 475, 622, 655, 844. A scripted world changes the hero's
// distance, the hit / ability / heal / talk messages and Maze's speech task at every NewScriptFrame; termination, StartScriptingEntity
// results and the health check are scripted too.
// Reference model: Model() below, written from the quest's statements; every run is compared event by event, then the entity's
// counters and flags, the parent's flag, and the balance of strings, things, CBaseObject parts and reference counts.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); fflush(stdout); exit(3); }
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECBaseClass@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptThing@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCBaseClass@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptThing@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?AskHeroQuestion@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDABVCCharString@@22_N3@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActions@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Converse@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@PBD@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DecrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DropGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?IncrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsAlive@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsAwareOfHero@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDead@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsEqualTo@CScriptThing@@MBE_NABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsNull@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsOpenDoor@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsSneaking@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsSummonedCreature@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUnconscious@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUsable@CScriptThing@@UBE_NXZ=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?MsgIsTalkedToBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?PlayCombatAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N11111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@J_N111111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetActivationTriggerStatus@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetAsUsable@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetFriendsWithEverythingFlag@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetToKillOnLevelUnload@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UnsheatheWeapons@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UpdateThingAttachment@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
// callees
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@StubBaseCtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@ABV0@@Z=@StubBaseCopy@12")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:??4CBaseObject@@IAEAAV0@ABV0@@Z=@StubBaseAssign@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@StubThingDtor@8")
#pragma comment(linker, "/alternatename:??1CIsThingAlive@NScript@@UAE@XZ=@StubPredDtor@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@StubIsValid@8")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@StubRelease@8")
#pragma comment(linker, "/alternatename:?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z=@StubPlayAnimation@40")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=@StubSpeak@32")
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=@StubBusy@8")
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=@StubGetScriptThing@12")
#pragma comment(linker, "/alternatename:??0?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAE@XZ=@StubMapCtor@8")
#pragma comment(linker, "/alternatename:?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@2@@Z=@StubInsertUnique@20")
#pragma comment(linker, "/alternatename:?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@PAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@3@PAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@3@_N4@Z=@StubRunCutscene@24")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@StubDistance@12")
#pragma comment(linker, "/alternatename:?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A=_g_pGlobalScriptDef")
#pragma comment(linker, "/alternatename:?GDefaultToUsingPhysicsInCutsceneAnimations@NGlobalConsole@@3_NA=_g_Physics")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@StubSetPredicate@12")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@StubTerminating@8")
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ")
extern "C" void __fastcall CallMain(void* self);

// ---- strings (one pointer, like retail's CCharString) ---------------------------------------------------------------------------
static int liveStrings;
class CCharString { public:
    CCharString(const char* text, long length); CCharString(const CCharString& o); ~CCharString();
    bool operator<(const CCharString& o) const;
    char* Text; };
static char* Dup(const char* s) { size_t n = strlen(s); char* p = (char*)malloc(n + 1); memcpy(p, s, n + 1); return p; }
CCharString::CCharString(const char* text, long length) { Text = Dup(text); ++liveStrings; if (length != -1) { printf("FAIL string length\n"); exit(4); } }
CCharString::CCharString(const CCharString& o) { Text = Dup(o.Text); ++liveStrings; }
CCharString::~CCharString() { free(Text); Text = 0; --liveStrings; }
bool CCharString::operator<(const CCharString& o) const { return strcmp(Text, o.Text) < 0; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
static void Bad(const char* what) { ++failures; if (failures < 40) printf("FAIL %s\n", what); }

// ---- event logs ------------------------------------------------------------------------------------------------------------------
enum { MAXLOG = 60000 };
static char got[MAXLOG + 64]; static int ngot;
static char want[MAXLOG + 64]; static int nwant;
static void Put(char* buf, int& n, const char* s) { size_t m = strlen(s); if (n + (int)m + 1 < MAXLOG) { memcpy(buf + n, s, m); n += (int)m; buf[n++] = ' '; buf[n] = 0; } }
static void Ev(const char* s) { Put(got, ngot, s); }
static void Wv(const char* s) { Put(want, nwant, s); }
static const char* termScript;     // '1' terminating; past the end: terminating
static const char* startScript;    // '1' StartScriptingEntity(Me) succeeds; past the end: succeeds
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

// ---- world -----------------------------------------------------------------------------------------------------------------------
enum { FRAMES = 48 };
struct World {
    unsigned char hit[FRAMES], ability[FRAMES], heal[FRAMES], talk[FRAMES];
    int nearAt, speakFrames; bool alive;
};
static World world;
struct State { int frame; int tpos, spos; bool valid; int busyUntil; };
static State gs, ms;
static int F(int f) { return f < FRAMES ? f : FRAMES - 1; }

static union { unsigned char b[0x28]; double align; } ent;
static union { unsigned char b[0x4C]; double align; } parent;
static union { unsigned char b[0x1088]; double align; } def;
extern "C" { void* g_pGlobalScriptDef; bool g_Physics = true; }

// things: { vptr, PImp.Data, PImp.Info }; implementations: { pos, name }
struct Info { unsigned long RefCount; void* DeleteFunc; void* Data; };
struct Impl { float pos[3]; const char* name; };
struct Thing { void** vtbl; Impl* data; Info* info; };
static void* thingVtbl[80];
static Impl meImpl, heroImpl, exitImpl;
static Info meInfo;
static Thing heroThing;
static Thing* Me() { return (Thing*)(ent.b + 8); }
static int liveThings, baseLive, movieLive;
static const char* Who(const void* t)
{ const Thing* th = (const Thing*)t; return t == Me() ? "Me" : t == &heroThing ? "Hero" : (th->data == &meImpl ? "MeCopy" : th->data ? th->data->name : "null"); }

// handles: { vptr, IntelligentPointer, PImp.Data, PImp.Info }
struct Handle { void* vtbl; void* ip; void* data; Info* info; };
static Info hMeInfo, hHeroInfo; static int meStbImpl, heroStbImpl;
enum { REF0 = 1000 };
static void Share(Handle* h, void* data, Info* info) { if (info) ++info->RefCount; if (h->info) --h->info->RefCount; h->data = data; h->info = info; }
static void* sehMeAddr;

extern "C" {
void __fastcall StubBaseCtor(void* self, void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseCopy(void* self, void*, const void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseDtor(void*, void*) { --baseLive; }
void __fastcall StubThingDtor(void* self, void*) { Thing* t = (Thing*)self; if (t->info) --t->info->RefCount; --liveThings; memset(self, 0xDD, 12); }
void __fastcall StubPredDtor(void* self, void*)
{   // ~CIsThingAlive (out of line in retail, 0x00E241B0): destroys the predicate's Thing copy
    Thing* t = (Thing*)((char*)self + 4); if (t->data != &meImpl || t->info != &meInfo) Bad("predicate thing"); --t->info->RefCount; Ev("~PRED");
}
void* __fastcall StubBaseAssign(void* self, void*, const void*) { return self; }
bool __fastcall StubIsValid(const void* self, void*) { return ((const Handle*)self)->data != 0; }
void __fastcall StubRelease(void* self, void*)
{ Handle* h = (Handle*)self; if (self != sehMeAddr) Bad("Release receiver"); if (h->info) --h->info->RefCount; h->data = 0; h->info = 0; Ev("R"); }
static void CheckMe(const void* self) { if (self != sehMeAddr || ((const Handle*)self)->data != &meStbImpl) Bad("seh_me receiver"); }
void __fastcall StubPlayAnimation(void* self, void*, const CCharString* a, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7)
{ CheckMe(self); char s[96]; sprintf(s, "ANIM:%s/%d%d%d%d%d%d%d", a->Text, b1, b2, b3, b4, b5, b6, b7); Ev(s); }
void __fastcall StubSpeak(void* self, void*, const void* to, const char* text, int sel, bool a, bool b, bool c)
{ CheckMe(self); char s[96]; sprintf(s, "SPEAK:%s/%s/%d/%d%d%d", Who(to), text, sel, a, b, c); Ev(s); gs.busyUntil = gs.frame + world.speakFrames; }
bool __fastcall StubBusy(const void* self, void*) { CheckMe(self); bool r = gs.frame < gs.busyUntil; Ev(r ? "BUSY1" : "BUSY0"); return r; }
void* __fastcall StubGetScriptThing(const void* self, void*, Thing* ret)
{ CheckMe(self); ret->vtbl = thingVtbl; ret->data = &meImpl; ret->info = 0; ++liveThings; Ev("SCRIPTTHING"); return ret; }
// map: header node pointer +0, count +4; node { color, parent, left, right, key +0x10, value +0x14 }, 0x24 bytes
struct Node { int color; Node* parent; Node* left; Node* right; };
void __fastcall StubMapCtor(void* self, void*)
{ Node* h = (Node*)malloc(0x24); h->color = 0; h->parent = 0; h->left = h; h->right = h; ((Node**)self)[0] = h; ((int*)self)[1] = 0; Ev("MAP"); }
void** __fastcall StubInsertUnique(void* self, void*, void** ret, void* hint, const void* v)
{
    Node* h = ((Node**)self)[0]; const CCharString* key = (const CCharString*)v; const Handle* val = (const Handle*)((const char*)v + 4);
    Node* n = (Node*)malloc(0x24); n->color = 1; n->left = n->right = 0;
    new ((char*)n + 0x10) CCharString(*key);
    Handle* hv = (Handle*)((char*)n + 0x14); memcpy(hv, val, 16); hv->ip = 0; ++baseLive; if (hv->info) ++hv->info->RefCount;
    Node* root = h->parent;
    if (!root) { h->parent = n; n->parent = h; h->left = h->right = n; }
    else {
        Node* p = root;
        for (;;) {
            bool less = strcmp(key->Text, ((CCharString*)((char*)p + 0x10))->Text) < 0;
            Node*& c = less ? p->left : p->right;
            if (!c) { c = n; n->parent = p; break; }
            p = c;
        }
        if (h->left->left == n) h->left = n;
        if (h->right->right == n) h->right = n;
    }
    ++((int*)self)[1]; (void)hint; *ret = n; return ret;
}
static void CastOf(Node* n, char* out)
{
    if (!n) return; CastOf(n->left, out);
    const Handle* hv = (const Handle*)((char*)n + 0x14); char b[48];
    sprintf(b, "%s=%s,", ((CCharString*)((char*)n + 0x10))->Text, hv->data == &meStbImpl ? "me" : hv->data == &heroStbImpl ? "hero" : hv->data ? "?" : "null");
    strcat(out, b); CastOf(n->right, out);
}
void __fastcall StubRunCutscene(const CCharString* name, void* map, void* m1, void* m2, bool a, bool b)
{ char cast[128] = ""; char s[200]; CastOf(((Node**)map)[0]->parent, cast); sprintf(s, "CUT:%s[%s]%d%d%d%d", name->Text, cast, m1 != 0, m2 != 0, a, b); Ev(s); }
bool __fastcall StubDistance(const void* a, const void* b, float d)
{ bool r = gs.frame >= world.nearAt; char s[64]; sprintf(s, "D:%s-%s<%g=%d", Who(a), Who(b), d, r); Ev(s); return r; }
void __fastcall StubSetPredicate(void* self, void*, const void* pred)
{
    if (self != ent.b) Bad("SetPredicate receiver");
    const Thing* t = (const Thing*)((const char*)pred + 4);
    if (!*(void* const*)pred || t->data != &meImpl || t->info != &meInfo || meInfo.RefCount != REF0 + 1) Bad("predicate: a counted copy of Me");
    Ev("P");
}
bool __fastcall StubTerminating(void* self, void*)
{ if (self != ent.b) Bad("terminate receiver"); bool t = NextBit(termScript, gs.tpos, true); Ev(t ? "T" : "t"); return t; }
}

// Me's messages (CScriptThing virtuals)
static void CheckMeThing(const void* self) { if (self != Me()) Bad("message receiver"); }
static bool __fastcall T_HitBy(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.hit[F(gs.frame)] != 0; char s[64]; sprintf(s, "HIT:%s=%d", w->Text, r); Ev(s); return r; }
static bool __fastcall T_TalkedTo(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.talk[F(gs.frame)] != 0; char s[64]; sprintf(s, "TALK:%s=%d", w->Text, r); Ev(s); return r; }
static bool __fastcall T_AbilityFrom(const void* self, void*, int ab, const CCharString* w)
{ CheckMeThing(self); bool r = world.heal[F(gs.frame)] != 0; char s[64]; sprintf(s, "ABIL%d:%s=%d", ab, w->Text, r); Ev(s); return r; }
static bool __fastcall T_AnyAbilityFrom(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.ability[F(gs.frame)] != 0; char s[64]; sprintf(s, "ANYABIL:%s=%d", w->Text, r); Ev(s); return r; }
static void __fastcall T_Friends(void* self, void*, bool b) { CheckMeThing(self); Ev(b ? "FRIENDS1" : "FRIENDS0"); }
static const float* __fastcall T_GetPos(const void* self, void*)
{ const Thing* t = (const Thing*)self; char s[48]; sprintf(s, "GETPOS:%s", Who(self)); Ev(s); return t->data ? t->data->pos : 0; }

// ---- game interface ----------------------------------------------------------------------------------------------------------------
static void* gameVtbl[900]; static void* gameObj[1]; static bool badGame;
static void CheckGame(const void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*) { CheckGame(self); Ev("N"); ++gs.frame; }
static bool __fastcall G_StartScripting(void* self, void*, const void* who, void* handle, int prio)
{
    CheckGame(self); char s[32];
    if (who == &heroThing) { if (prio != 4) Bad("hero priority"); Share((Handle*)handle, &heroStbImpl, &hHeroInfo); Ev("H"); return true; }
    if (who != Me()) Bad("StartScriptingEntity thing");
    if (!sehMeAddr) sehMeAddr = handle; else if (handle != sehMeAddr) Bad("StartScriptingEntity handle");
    bool ok = NextBit(startScript, gs.spos, true);
    if (ok) Share((Handle*)handle, &meStbImpl, &hMeInfo);
    sprintf(s, "%c%d", ok ? 'S' : 's', prio); Ev(s); return ok;
}
static void __fastcall G_RemoveThing(void* self, void*, const void* thing, bool a, bool b)
{ CheckGame(self); char s[32]; sprintf(s, "REMOVE:%s/%d%d", Who(thing), a, b); Ev(s); }
static Thing* __fastcall G_GetThing(void* self, void*, Thing* ret, const CCharString* name)
{
    CheckGame(self); if (strcmp(name->Text, "M_MazeExit")) Bad("thing name");
    ret->vtbl = thingVtbl; ret->data = &exitImpl; ret->info = 0; ++liveThings;
    char s[64]; sprintf(s, "G:%s", name->Text); Ev(s); return ret;
}
static Thing* __fastcall G_GetHero(void* self, void*) { CheckGame(self); return &heroThing; }
static float healthNext;
static float __fastcall G_GetHealth(void* self, void*, const void* thing)
{ CheckGame(self); char s[48]; sprintf(s, "HEALTH:%s=%g", Who(thing), healthNext); Ev(s); return healthNext; }
static void __fastcall G_Deactivate(void* self, void*, const CCharString* q, unsigned long n) { CheckGame(self); char s[64]; sprintf(s, "DEACT:%s/%lu", q->Text, n); Ev(s); }
static void __fastcall G_Complete(void* self, void*, const CCharString* q, bool a, bool b, bool c) { CheckGame(self); char s[64]; sprintf(s, "COMPLETE:%s/%d%d%d", q->Text, a, b, c); Ev(s); }
static void __fastcall G_MiniMap(void* self, void*, const void* t, const CCharString* m) { CheckGame(self); char s[64]; sprintf(s, "MINIMAP:%s/%s", Who(t), m->Text); Ev(s); }
static void __fastcall G_HasInfo(void* self, void*, const void* t, bool a, bool b, bool c) { CheckGame(self); char s[48]; sprintf(s, "INFO:%s/%d%d%d", Who(t), a, b, c); Ev(s); }
static void __fastcall G_ClearInfo(void* self, void*, const void* t) { CheckGame(self); char s[48]; sprintf(s, "CLEARINFO:%s", Who(t)); Ev(s); }
static long convIndex;
static long __fastcall G_NewConv(void* self, void*, const void* t, bool a, bool b) { CheckGame(self); char s[48]; sprintf(s, "CONV:%s/%d%d", Who(t), a, b); Ev(s); return ++convIndex; }
static void __fastcall G_AddPerson(void* self, void*, long i, const void* t) { CheckGame(self); char s[48]; sprintf(s, "PERSON:%ld/%s", i, Who(t)); Ev(s); }
static void __fastcall G_AddLine(void* self, void*, long i, const CCharString* l, bool b, const void* a, const void* c)
{ CheckGame(self); char s[96]; sprintf(s, "LINE:%ld/%s/%d/%s/%s", i, l->Text, b, Who(a), Who(c)); Ev(s); }
static int movieImpl;
static void __fastcall MovieDelete(void* data) { if (data != &movieImpl) Bad("movie delete data"); --movieLive; Ev("~MOVIE"); }
static void __fastcall G_StartMovie(void* self, void*, const CCharString* name, void* movie)
{
    CheckGame(self); char s[48]; sprintf(s, "MOVIE:[%s]", name->Text); if (((void**)movie)[2] || ((void**)movie)[3]) Bad("movie handle not empty"); Ev(s);
    Info* info = new Info; info->RefCount = 1; info->DeleteFunc = (void*)MovieDelete; info->Data = &movieImpl;
    ((void**)movie)[2] = &movieImpl; ((void**)movie)[3] = info; ++movieLive;   // the movie's PImp now owns the sequence
}
static void __fastcall G_Pause(void* self, void*, bool b) { CheckGame(self); Ev(b ? "PAUSE1" : "PAUSE0"); }
static void __fastcall G_ClearEnemy(void* self, void*, const void* t) { CheckGame(self); char s[48]; sprintf(s, "CLEARENEMY:%s", Who(t)); Ev(s); }
static void __fastcall G_Facing(void* self, void*, const void* t, const void* target, bool b)
{ CheckGame(self); char s[64]; sprintf(s, "FACE:%s-%s/%d", Who(t), Who(target), b); Ev(s); }
static void __fastcall G_Opinion(void* self, void*, const void* t, const CCharString* o) { CheckGame(self); char s[64]; sprintf(s, "OPINION:%s/%s", Who(t), o->Text); Ev(s); }
static CCharString* __fastcall G_QuestName(void* self, void*, CCharString* ret) { CheckGame(self); new (ret) CCharString("Q_GuardianSister", -1); Ev("QUESTNAME"); return ret; }
static void __fastcall G_Pushable(void* self, void*, Thing t, bool b)
{   // by-value CScriptThing: the callee destroys it
    CheckGame(self); char s[64];
    bool copyOk = t.data == &meImpl && t.info == &meInfo && meInfo.RefCount == REF0 + 1 && t.vtbl != 0;
    sprintf(s, "PUSHABLE:%s/%d", copyOk ? "MeCopy" : "?", b); if (t.info) --t.info->RefCount; Ev(s);
}

// ---- reference model -----------------------------------------------------------------------------------------------------------
static bool MT() { bool t = NextBit(termScript, ms.tpos, true); Wv(t ? "T" : "t"); return t; }
static void MF() { Wv("N"); ++ms.frame; }
static bool MStart(int prio) { bool ok = NextBit(startScript, ms.spos, true); if (ok) ms.valid = true; char s[8]; sprintf(s, "%c%d", ok ? 'S' : 's', prio); Wv(s); return ok; }
static void MRelease() { if (ms.valid) { Wv("R"); ms.valid = false; } }
struct Expect { bool spoke, waved; long hits, talks; };
static Expect ex;
static const float* healthScript; static int healthPos, mHealthPos;
static void Model()
{
    char s[160];
    Wv("P"); Wv("~PRED"); MF(); if (MT()) return;
    MRelease();
    while (!MStart(3)) { MF(); if (MT()) return; }
    if (MT()) return;
    Wv("OPINION:Me/OPINION_SOURCE_MAZE_BS_PUB"); Wv("G:M_MazeExit"); Wv("GETPOS:exit");
    Wv("MINIMAP:Me/HUD_ORB_QUEST_CORE"); Wv("INFO:Me/010"); Wv("PUSHABLE:MeCopy/0");
    for (;; MF()) {
        if (MT()) return;
        if (!ex.spoke && !ex.waved) {
            bool r = ms.frame >= world.nearAt; sprintf(s, "D:Hero-Me<7=%d", r); Wv(s);
            if (r) {
                if (MT()) return;
                Wv("ANIM:ST_HELLO/0001100"); Wv("CONV:Me/00");
                long i = ++convIndex; sprintf(s, "PERSON:%ld/Hero", i); Wv(s);
                sprintf(s, "LINE:%ld/TEXT_QST_027_MAZE_CALL_HERO_OVER_10/0/Me/Hero", i); Wv(s);
                ex.waved = true;
            }
        }
        if (!ex.spoke) { if (MT()) return; Wv("FACE:Me-Hero/0"); }
        int f = F(ms.frame);
        bool hit = world.hit[f] != 0; sprintf(s, "HIT:SCRIPT_NAME_HERO=%d", hit); Wv(s);
        if (!hit) {
            bool any = world.ability[f] != 0; sprintf(s, "ANYABIL:SCRIPT_NAME_HERO=%d", any); Wv(s);
            if (any) { bool heal = world.heal[f] != 0; sprintf(s, "ABIL14:SCRIPT_NAME_HERO=%d", heal); Wv(s); hit = !heal; }
        }
        if (hit) {
            if (MT()) return;
            MRelease();
            while (!MStart(4)) { MF(); if (MT()) return; }
            if (MT()) return;
            Wv("MOVIE:[]"); Wv("PAUSE1"); Wv("SCRIPTTHING");
            float h = healthScript[mHealthPos++]; sprintf(s, "HEALTH:MeCopy=%g", h); Wv(s);
            if (h > 0.0f) {
                Wv("SPEAK:Hero/TEXT_QST_027_MAZE_ON_HIT_10/0/010"); int busyUntil = ms.frame + world.speakFrames;
                for (;;) { bool b = ms.frame < busyUntil; Wv(b ? "BUSY1" : "BUSY0"); if (!b) break; MF(); if (MT()) { Wv("PAUSE0"); Wv("~MOVIE"); return; } }
                if (MT()) { Wv("PAUSE0"); Wv("~MOVIE"); return; }
            }
            ++ex.hits; Wv("FRIENDS1"); Wv("CLEARENEMY:Me"); Wv("PAUSE0"); Wv("~MOVIE");
        }
        bool talk = world.talk[F(ms.frame)] != 0; sprintf(s, "TALK:SCRIPT_NAME_HERO=%d", talk); Wv(s);
        if (talk) {
            if (MT()) return;
            MRelease();
            while (!MStart(4)) { MF(); if (MT()) return; }
            if (MT()) return;
            Wv("H"); Wv("MAP"); Wv("MOVIE:[]"); Wv("PAUSE1"); Wv("CUT:CS_GUARDIANSISTER_BOWERSTONE[HERO=hero,ME=me,]0001"); Wv("PAUSE0"); Wv("~MOVIE");
            ex.spoke = true; Wv("CLEARINFO:Me"); Wv("QUESTNAME"); Wv("COMPLETE:Q_GuardianSister/000"); Wv("QUESTNAME"); Wv("DEACT:Q_GuardianSister/0");
            Wv("REMOVE:Me/01"); ++ex.talks;
        }
    }
}
static float __fastcall G_GetHealthScripted(void* self, void*, const void* thing)
{ healthNext = healthScript[healthPos++]; return G_GetHealth(self, 0, thing); }

static void Run(const World& w, const char* term, const char* start, const float* health, int k)
{
    world = w; termScript = term; startScript = start; healthScript = health; healthPos = mHealthPos = 0;
    memset(&gs, 0, sizeof gs); memset(&ms, 0, sizeof ms); ngot = nwant = 0; got[0] = want[0] = 0;
    memset(parent.b, 0, sizeof parent.b);
    memset(ent.b, 0x11, sizeof ent.b); void* g = gameObj; memcpy(ent.b + 4, &g, 4); void* pp = parent.b; memcpy(ent.b + 0x14, &pp, 4);
    long zero = 0; memcpy(ent.b + 0x1c, &zero, 4); ent.b[0x20] = 0; memcpy(ent.b + 0x24, &zero, 4);
    Thing* me = Me(); me->vtbl = thingVtbl; me->data = &meImpl; me->info = &meInfo;
    meInfo.RefCount = REF0; hMeInfo.RefCount = REF0; hHeroInfo.RefCount = REF0;
    liveThings = baseLive = liveStrings = movieLive = 0; badGame = false; sehMeAddr = 0; memset(&ex, 0, sizeof ex);
    convIndex = 0; Model(); convIndex = 0;
    CallMain(ent.b);
    if (strcmp(got, want)) {
        ++failures;
        if (failures < 6) {
            int i = 0; while (got[i] && got[i] == want[i]) ++i;
            int a = i > 120 ? i - 120 : 0;
            printf("FAIL case %d events differ at %d\n  got  ...%.200s\n  want ...%.200s\n", k, i, got + a, want + a);
        }
        return;
    }
    long hits, talks; memcpy(&hits, ent.b + 0x24, 4); memcpy(&talks, ent.b + 0x1c, 4);
    check(!badGame, "Game receiver", k);
    check((parent.b[0x48] != 0) == ex.spoke, "GuardianSpokeToHero", k);
    check((ent.b[0x20] != 0) == ex.waved, "WavedOver", k);
    check(hits == ex.hits, "HitCount", k);
    check(talks == 0, "TalkCounter untouched", k);
    check(liveThings == 0, "every CScriptThing temporary destroyed", k);
    check(baseLive == 0 && movieLive == 0, "every CBaseObject part and movie destroyed", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(meInfo.RefCount == REF0, "Me's reference count balanced (predicate copy, by-value copy)", k);
    check(hMeInfo.RefCount == REF0 && hHeroInfo.RefCount == REF0, "handle references balanced", k);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int i = 0; i < 80; ++i) thingVtbl[i] = (void*)VirtualTrap;
    thingVtbl[6] = (void*)T_GetPos; thingVtbl[21] = (void*)T_HitBy; thingVtbl[27] = (void*)T_TalkedTo; thingVtbl[41] = (void*)T_AbilityFrom;
    thingVtbl[42] = (void*)T_AnyAbilityFrom; thingVtbl[67] = (void*)T_Friends;
    meImpl.name = "me"; heroImpl.name = "hero"; exitImpl.name = "exit";
    heroThing.vtbl = thingVtbl; heroThing.data = &heroImpl; heroThing.info = 0;
    g_pGlobalScriptDef = def.b; long trigger = 7; memcpy(def.b + 0xc68, &trigger, 4); long exitd = 99; memcpy(def.b + 0xc6c, &exitd, 4);
    for (int i = 0; i < 900; ++i) gameVtbl[i] = (void*)VirtualTrap;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScripting; gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThing;
    gameVtbl[108] = (void*)G_RemoveThing; gameVtbl[264] = (void*)G_GetHealthScripted; gameVtbl[281] = (void*)G_Deactivate; gameVtbl[288] = (void*)G_Complete;
    gameVtbl[350] = (void*)G_MiniMap; gameVtbl[360] = (void*)G_HasInfo; gameVtbl[361] = (void*)G_ClearInfo; gameVtbl[364] = (void*)G_NewConv;
    gameVtbl[365] = (void*)G_AddPerson; gameVtbl[366] = (void*)G_AddLine; gameVtbl[370] = (void*)G_StartMovie; gameVtbl[379] = (void*)G_Pause;
    gameVtbl[452] = (void*)G_ClearEnemy; gameVtbl[475] = (void*)G_Facing; gameVtbl[622] = (void*)G_Opinion; gameVtbl[655] = (void*)G_QuestName;
    gameVtbl[844] = (void*)G_Pushable;
    static float health[64]; int k = 0;
    static const char zeros[] = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    // targeted: hit at 3, ability at 5 (heal at 6), talk at 8; a terminate at each of the first 80 tests
    for (int variant = 0; variant < 6; ++variant) {
        World w; memset(&w, 0, sizeof w);
        w.nearAt = variant == 1 ? 99 : 2; w.speakFrames = variant == 2 ? 0 : 2;
        w.hit[3] = 1; w.ability[5] = 1; w.ability[6] = 1; w.heal[6] = 1; w.talk[8] = variant != 3; w.talk[10] = variant == 4;
        for (int i = 0; i < 64; ++i) health[i] = (variant == 5 && i == 0) ? 0.0f : 10.0f;
        char ts[128];
        for (int t = 0; t < 80; ++t) { memcpy(ts, zeros, 100); ts[t] = '1'; ts[100] = 0; Run(w, ts, (t & 1) ? "1" : "0010", health, ++k); }
    }
    unsigned long seed = 23;
    for (int r = 0; r < 6000; ++r) {
#define RND(n) (seed = seed * 1103515245ul + 12345ul, (int)((seed >> 16) % (n)))
        World w; memset(&w, 0, sizeof w);
        w.nearAt = RND(12); w.speakFrames = RND(4);
        for (int f = 0; f < FRAMES; ++f) { w.hit[f] = RND(9) == 0; w.ability[f] = RND(7) == 0; w.heal[f] = RND(2); w.talk[f] = RND(12) == 0; }
        for (int i = 0; i < 64; ++i) health[i] = RND(4) ? 5.0f : (RND(2) ? 0.0f : -1.0f);
        static char ts[300], ss[16];
        for (int i = 0; i < 299; ++i) ts[i] = RND(60) == 0 ? '1' : '0';
        ts[299] = 0;
        for (int i = 0; i < 15; ++i) ss[i] = RND(3) == 0 ? '0' : '1';
        ss[15] = 0;
        Run(w, ts, ss, health, 1000 + r);
    }
    if (failures) { printf("00e25f70_TEST FAIL %d\n", failures); return 1; }
    printf("00e25f70_TEST PASS (%d targeted, 6000 random)\n", k);
    return 0;
}