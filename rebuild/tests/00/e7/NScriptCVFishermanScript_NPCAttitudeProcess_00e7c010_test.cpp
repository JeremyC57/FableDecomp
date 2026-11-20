// Behaviour test for 0x00E7C010 NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing Me) (links the REAL row object: the row body, its
// expanded ~CScriptThing / CCountedPointer release, the null-safe Msg* wrappers, the CIsThingAlive ctor, the ScriptedThingBase ctor / dtor, the movie
// ctor / dtor COMDATs and the pause expansion all run as compiled). No class models: every external the row object names is bound with
// /alternatename -- the calls the row makes to extern "C" __fastcall stubs (thiscall-compatible, unused edx), every vtable slot and unreached COMDAT
// reference to a trap. Game (+0x40) slots 7, 8, 70, 72, 77, 108, 130, 264, 370, 373, 374, 376, 379, 411, 416, 472, 474, 475 and Me's implementation
// slots 21, 41, 42 through fake vtables; things carry real counted-pointer info blocks {RefCount, DeleteFunc, Data} that the row releases itself.
// Reference model: Model() below, written from the script's statements; every run is compared event by event (with the destruction order of every
// live object on each early return) plus the final state (relation counter, FishermanMustRunAway, every string / thing / handle released, Me's
// reference count after the callee-destroyed by-value argument).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@BODtor@8")
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:__purecall=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@BOCtor@8")
#pragma comment(linker, "/alternatename:?IsEqualTo@CScriptThing@@MBE_NABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsNull@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDead@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsAlive@CScriptThing@@UBE_NXZ=@ThingIsAlive@8")
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
#pragma comment(linker, "/alternatename:?GetAngleXY@CScriptThing@@UBEMXZ=@ThingAngle@8")
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
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=@BaseClassDtor@8")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CIsThingAlive@NScript@@UAE@XZ=@FilterDtor@8")
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
#pragma comment(linker, "/alternatename:?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PerformExpression@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Converse@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@PBD@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?AskHeroQuestion@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDABVCCharString@@22_N3@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=@Speak@32")
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=@GetScriptThing@12")
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z=@MoveToThing@36")
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@DistUnder@12")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@ReleaseStub@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@IsValidStub@8")
#pragma comment(linker, "/alternatename:?NPCAttitudes@@3PAJA=_g_RelationCounters")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@SetPred@12")
#pragma comment(linker, "/alternatename:_g_STVtbl=??_7CScriptThing@@6B@")
#pragma comment(linker, "/alternatename:_g_FilterVtbl=??_7CIsThingAlive@NScript@@6B@")
#pragma comment(linker, "/alternatename:_g_MovieVtbl=??_7CScriptGameResourceObjectMovieBase@@6B@")
extern "C" char g_STVtbl, g_FilterVtbl, g_MovieVtbl;
extern "C" void __fastcall CallRow(void* self, void*, void* vptr, void* data, void* info);   // thiscall `ret 0Ch`: the by-value Me
#pragma comment(linker, "/alternatename:@CallRow@20=?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z")

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static Script sTerm, sHit, sAny, sHeal, sLow, sStart, sDist, sAlive, sSpeak, sPerf;
// past the end: terminating / not hit / not hit / not heal / healthy / started / out of range / dead / dead / not performing

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 8000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static void Ws(const char* s) { while (*s) W(*s++); }

static void* meArgPtr; static void* meArg() { return meArgPtr; }
static long g_mCounter; static bool mRun;
// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x60]; double a; } self;                 // CV_FishermanScript (0x58)
extern "C" long g_RelationCounters[3] = { 0, 0, 0 };
static void* gameVtbl[600]; static void* gameObj[1];
static void* implVtbl[80]; static void* implObj[1];
static void* heroVtbl[1]; static void* hero[3];
struct Info { long RefCount; void* DeleteFunc; void* Data; };
static Info meInfo; static int liveInfos, deletes; static long nextData = 0x7000;
extern "C" void __fastcall DelFunc(void* data) { ++deletes; Ev('X'); (void)data; }
static Info* NewInfo() { Info* i = (Info*)malloc(sizeof(Info)); i->RefCount = 1; i->DeleteFunc = (void*)DelFunc; i->Data = (void*)(nextData += 0x10); ++liveInfos; return i; }
void __cdecl operator delete(void* p) { if (p != &meInfo) { free(p); --liveInfos; } }
static void FillThing(void** t) { Info* i = NewInfo(); t[0] = &g_STVtbl; t[1] = i->Data; t[2] = i; }
static void* meData; static bool nullMe;

// strings: one text pointer, logged by a letter per literal
static int liveStrings;
static char Lit(const char* t)
{
    if (!t) return '?';
    if (!strcmp(t, "SCRIPT_NAME_HERO")) return 'h'; if (!strcmp(t, "NPC_NPCStand")) return '1'; if (!strcmp(t, "NPC_HeroStand")) return '2';
    if (!strcmp(t, "NPC_Camera")) return '3'; if (!strcmp(t, "MK_FI_HERO")) return 'm'; if (!*t) return '0'; return '?';
}
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; check(len == -1, "string length", 0); Ev('s'); Ev(Lit(text)); return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; Ev('~'); Ev(Lit(*s)); *s = 0; }

// things / predicate
extern "C" void* __fastcall ThingCopy(void** t, void*, void* const* o) { if (!meArgPtr) meArgPtr = (void*)o; t[0] = o[0]; t[1] = o[1]; t[2] = o[2]; if (t[2]) ++((Info*)t[2])->RefCount; Ev('k'); return t; }
extern "C" void __fastcall BaseClassDtor(void* t, void*) { (void)t; }
extern "C" void __fastcall SetPred(void* s, void*, void* const* f)
{
    check(s == self.b && f[0] == (void*)&g_FilterVtbl && f[1] == (void*)&g_STVtbl && f[2] == meData && f[3] == (void*)&meInfo, "predicate: CIsThingAlive(Me)", 0);
    Ev('P');
}
extern "C" void __fastcall FilterDtor(void** f, void*) { if (f[3]) --((Info*)f[3])->RefCount; Ev('p'); }
static void* placement; static void* heroMarker; static void* camera;
extern "C" float __fastcall ThingAngle(void** t, void*) { check(t == placement || t == heroMarker, "angle of a stand point", 0); Ev('g'); return t == placement ? 1.25f : -2.5f; }
extern "C" bool __fastcall ThingIsAlive(void** t, void*) { check(t == camera, "alive test on the camera", 0); bool r = Next(sAlive); Ev(r ? 'L' : 'l'); return r; }
extern "C" bool __fastcall DistUnder(void* const* a, void* const* b, float d)
{
    check(d == 10.0f, "distance 10", 0);
    check((a == (void* const*)meArg() && b == (void* const*)placement) || (a == (void* const*)hero && b == (void* const*)heroMarker), "distance operands", 0);
    bool r = Next(sDist); Ev(r ? 'D' : 'd'); return r;
}

// Me's implementation: slots 21 MsgIsHitBy, 42 MsgIsHitByAnySpecialAbilityFrom, 41 MsgIsHitBySpecialAbilityFrom
extern "C" bool __fastcall I_HitBy(void* t, void*, const char* const* who) { check(t == implObj && !strcmp(*who, "SCRIPT_NAME_HERO"), "MsgIsHitBy args", 0); bool r = Next(sHit); Ev(r ? 'A' : 'a'); return r; }
extern "C" bool __fastcall I_AnySpecial(void* t, void*, const char* const* who) { check(t == implObj && !strcmp(*who, "SCRIPT_NAME_HERO"), "special args", 0); bool r = Next(sAny); Ev(r ? 'B' : 'b'); return r; }
extern "C" bool __fastcall I_Special(void* t, void*, long ab, const char* const* who) { check(t == implObj && ab == 14 && !strcmp(*who, "SCRIPT_NAME_HERO"), "heal-life args", 0); bool r = Next(sHeal); Ev(r ? 'C' : 'c'); return r; }

// ScriptedThingBase {vptr, IntelligentPointer, Data, Info}
static void* sehObj;
extern "C" void* __fastcall BOCtor(void** o, void*) { o[1] = 0; Ev('o'); return o; }
extern "C" void __fastcall BODtor(void** o, void*) { (void)o; Ev('O'); }
extern "C" bool __fastcall IsValidStub(void* const* h, void*) { bool r = h[2] != 0; Ev('v'); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall ReleaseStub(void** h, void*) { VirtualTrap(); (void)h; }   // never reached: the handle is fresh
extern "C" void* __fastcall GetScriptThing(void* const* h, void*, void** ret) { check(h == sehObj, "GetScriptThing on seh_me", 0); FillThing(ret); Ev('R'); return ret; }
extern "C" bool __fastcall IsPerf(void* h, void*) { check(h == sehObj, "performing on seh_me", 0); bool r = Next(sPerf); Ev(r ? 'Q' : 'q'); return r; }
extern "C" void __fastcall Speak(void* h, void*, void* const* to, const char* text, long sel, int b1, int b2, int b3)
{
    check(h == sehObj && to == (void* const*)hero && !strcmp(text, "TEXT_QST_023_IDLE_03_HATE") && sel == 0 && !b1 && b2 == 1 && !b3, "speak args", 0); Ev('K');
}
static void* mkThing;
extern "C" void __fastcall MoveToThing(void* h, void*, void* const* t, float d, long move, int b1, int b2, int b3, int b4)
{
    check(h == sehObj && t == mkThing && d == 3.0f && move == 1 && !b1 && !b2 && !b3 && b4 == 1, "move-to args", 0); Ev('V');
}

// Game

static bool T_(void* s) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall IsTerm(void* s, void*) { return T_(s); }
static void G(void* g, const char* what) { check(g == gameObj, what, 0); }
extern "C" void __fastcall G_NewScriptFrame(void* g, void*) { G(g, "frame this"); Ev('F'); }
extern "C" bool __fastcall G_Start(void* g, void*, void* const* thing, void** h, long prio)
{
    G(g, "start this"); check(thing == meArg() && prio == 4, "start scripting Me at priority 4", 0); sehObj = h;
    bool r = Next(sStart); Ev(r ? 'S' : 'n');
    if (r) { check(!h[2] && !h[3], "started into an empty handle", 0); Info* i = NewInfo(); h[2] = i->Data; h[3] = i; }
    return r;
}
extern "C" void* __fastcall G_GetHero(void* g, void*) { G(g, "hero this"); Ev('Y'); return hero; }
extern "C" void* __fastcall G_GetThing(void* g, void*, void** ret, const char* const* name) { G(g, "lookup this"); check(!strcmp(*name, "MK_FI_HERO"), "lookup name", 0); FillThing(ret); mkThing = ret; Ev('M'); return ret; }
extern "C" void* __fastcall G_GetNearest(void* g, void*, void** ret, void* const* from, const char* const* name)
{
    G(g, "nearest this"); check(from == meArg(), "nearest from Me", 0); char c = Lit(*name);
    FillThing(ret); if (c == '1') placement = ret; else if (c == '2') heroMarker = ret; else if (c == '3') camera = ret; else check(false, "nearest name", 0);
    Ev('G'); return ret;
}
extern "C" void __fastcall G_RemoveThing(void* g, void*, void* const* t, int b1, int b2) { G(g, "remove this"); check(t == meArg() && !b1 && !b2, "remove Me", 0); Ev('r'); }
extern "C" void __fastcall G_Sheathe(void* g, void*) { G(g, "sheathe this"); Ev('W'); }
static float health[2]; static int healthPos;
extern "C" float __fastcall G_GetHealth(void* g, void*, void* const* t)
{
    G(g, "health this");
    if (t == meArg()) { bool low = Next(sLow); Ev(low ? 'H' : 'j'); return low ? health[0] : health[1]; }
    bool alive = Next(sSpeak); Ev(alive ? 'U' : 'u'); return alive ? 0.5f : (healthPos ? -1.0f : 0.0f);
}
extern "C" void __fastcall G_StartMovie(void* g, void*, const char* const* name, void* const* movie)
{
    G(g, "movie this"); check(!strcmp(*name, "") && movie[0] == (void*)&g_MovieVtbl && !movie[2] && !movie[3], "movie sequence args", 0); Ev('m');
}
extern "C" bool __fastcall G_FadeOut(void* g, void*, float a, float b, unsigned long colour) { G(g, "fade this"); check(a == 1.5f && b == 1.5f && colour == 0xFF000000ul, "fade-out args", 0); Ev('J'); return true; }
extern "C" void __fastcall G_FadeIn(void* g, void*) { G(g, "fade-in this"); Ev('I'); }
extern "C" void __fastcall G_Pause(void* g, void*, float t) { G(g, "pause this"); check(t == 1.5f || t == 2.0f, "pause time", 0); Ev(t == 1.5f ? '5' : '6'); }
static int pauseDepth;
extern "C" void __fastcall G_PauseEntities(void* g, void*, bool on) { G(g, "pause entities this"); pauseDepth += on ? 1 : -1; Ev(on ? 'Z' : 'z'); }
extern "C" void __fastcall G_Camera(void* g, void*, void* const* t, const void* pos, const void* set, float tm, long a, long b)
{
    G(g, "camera this"); check(t == camera && !pos && !set && tm == -1.0f && a == 0 && b == -1, "camera point args", 0); Ev('c');
}
extern "C" void __fastcall G_CameraDefault(void* g, void*) { G(g, "camera default this"); Ev('d'); }
extern "C" void __fastcall G_Teleport(void* g, void*, void* const* a, void* const* b, int f) { G(g, "teleport this"); check(a == meArg() && (b == placement || b == heroMarker) && !f, "teleport args", 0); Ev('E'); Ev(b == placement ? '1' : '2'); }
extern "C" void __fastcall G_Facing(void* g, void*, void* const* a, float ang, int f) { G(g, "facing this"); check(a == meArg() && (ang == 1.25f || ang == -2.5f) && f == 1, "facing args", 0); Ev('f'); Ev(ang == 1.25f ? '1' : '2'); }
extern "C" void __fastcall G_FacingTowards(void* g, void*, void* const* a, void* const* b, int f) { G(g, "facing towards this"); check(a == meArg() && b == (void* const*)hero && !f, "facing-towards args", 0); Ev('b'); }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mHit, mAny, mHeal, mLow, mStart, mDist, mAlive, mSpeak, mPerf;
static char live[16][4]; static int nlive;                               // destruction events of the live locals, innermost last
static void Push(const char* d) { strcpy(live[nlive++], d); }
static void Pop() { --nlive; Ws(live[nlive]); }
static bool T() { bool r = Next(mTerm); W(r ? 'T' : 't'); if (r) while (nlive) Pop(); return r; }
static bool Cond()
{
    bool r; Ws("sh");
    if (nullMe) r = false; else { bool a = Next(mHit); W(a ? 'A' : 'a'); r = a; }
    if (r) { Ws("~h"); return true; }
    Ws("sh"); bool b = false; if (!nullMe) { b = Next(mAny); W(b ? 'B' : 'b'); }
    if (b) {
        Ws("sh"); bool c = false; if (!nullMe) { c = Next(mHeal); W(c ? 'C' : 'c'); }
        r = !c; Ws("~h~h~h"); return r;
    }
    Ws("~h~h"); return false;
}
static void Model()
{
    Ws("kPp" "F"); if (T()) return;
    for (;;) {
        if (T()) return;
        if (Cond()) {
            if (T()) return;
            bool low = Next(mLow); W(low ? 'H' : 'j');
            if (low && health[0] <= 1.0f || !low && health[1] <= 1.0f) {
                if (T()) return;
                g_mCounter = 2; mRun = true;
                W('o'); Push("O"); Ws("v0");
                bool held = false;
                for (;;) { bool r = Next(mStart); W(r ? 'S' : 'n'); if (r) { held = true; strcpy(live[nlive - 1], "XO"); break; } W('F'); if (T()) return; }
                if (T()) return;
                W('o'); Push("O"); Ws("s0m~0");                                // speech_movie
                W('Z'); Push("z");                                             // PauseMyEntitiesPlease
                W('W');
                Ws("s1G~1"); Push("X");                                        // NPCPlacementMarker
                bool d1 = Next(mDist); W(d1 ? 'D' : 'd');
                if (d1) {
                    if (T()) return;
                    Ws("E1gf1");
                    Ws("s2G~2"); Push("X");                                    // NPCHeroMarker
                    W('Y'); bool d2 = Next(mDist); W(d2 ? 'D' : 'd');
                    if (d2) { if (T()) return; Ws("E2gf2"); }
                    Ws("s3G~3"); Push("X");                                    // NPCCamera
                    bool al = Next(mAlive); W(al ? 'L' : 'l');
                    if (al) { if (T()) return; W('c'); }
                    Pop(); Pop();
                }
                Ws("Yb");
                W('R'); bool sp = Next(mSpeak); W(sp ? 'U' : 'u'); W('X');
                if (sp) {
                    Ws("YK");
                    for (;;) { bool pf = Next(mPerf); W(pf ? 'Q' : 'q'); if (!pf) break; W('F'); if (T()) return; }
                    if (T()) return;
                }
                Ws("sm" "M" "V" "X~m");
                Ws("5J6rd");
                Pop(); Pop(); Pop();                                           // ~NPCPlacementMarker, ~PauseMyEntitiesPlease, ~speech_movie
                W('I');
                Pop();                                                         // ~seh_me
                (void)held; return;
            }
        }
        W('F');
    }
}

static void CallRowWrap(bool nm) { CallRow(self.b, 0, &g_STVtbl, nm ? 0 : (void*)implObj, &meInfo); }

static void Run(const char* term, const char* hit, const char* any, const char* heal, const char* low, const char* st, const char* dist,
                const char* alive, const char* speak, const char* perf, float h0, float h1, bool nm, int k)
{
    memset(self.b, 0xEE, sizeof self.b); *(void**)(self.b + 0x40) = gameObj;
    g_RelationCounters[0] = 7; g_RelationCounters[1] = 8; g_RelationCounters[2] = 9;
    health[0] = h0; health[1] = h1; healthPos = (k & 1); nullMe = nm;
    Script* ss[10] = { &sTerm, &sHit, &sAny, &sHeal, &sLow, &sStart, &sDist, &sAlive, &sSpeak, &sPerf };
    const char* tx[10] = { term, hit, any, heal, low, st, dist, alive, speak, perf };
    bool df[10] = { true, false, false, false, false, true, false, false, false, false };
    for (int i = 0; i < 10; ++i) { ss[i]->s = tx[i]; ss[i]->pos = 0; ss[i]->dflt = df[i]; }
    mTerm = sTerm; mHit = sHit; mAny = sAny; mHeal = sHeal; mLow = sLow; mStart = sStart; mDist = sDist; mAlive = sAlive; mSpeak = sSpeak; mPerf = sPerf;
    ngot = nwant = 0; nlive = 0; liveStrings = liveInfos = deletes = pauseDepth = 0; sehObj = placement = heroMarker = camera = mkThing = 0;
    meInfo.RefCount = 2; meInfo.DeleteFunc = (void*)VirtualTrap; meData = nm ? 0 : (void*)implObj; meInfo.Data = meData;
    g_mCounter = 9; mRun = false;
    Model();
    // the row's by-value Me: the caller copies (one reference), the callee destroys it
    ++meInfo.RefCount;
    void* argStack[3]; (void)argStack;
    meArgPtr = 0;
    CallRowWrap(nm);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) printf("  got  %.400s\n  want %.400s\n", got, want);
    }
    check(g_RelationCounters[2] == g_mCounter && g_RelationCounters[0] == 7 && g_RelationCounters[1] == 8, "RelationCounters[2]", k);
    check(self.b[0x50] == (mRun ? 1 : 0xEE), "FishermanMustRunAway", k);
    check(self.b[0x4F] == 0xEE && self.b[0x51] == 0xEE, "no other store", k);
    check(meInfo.RefCount == 2, "Me's references (argument destroyed by the callee)", k);
    check(liveStrings == 0 && liveInfos == 0 && pauseDepth == 0, "everything released", k);
}

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; implObj[0] = implVtbl; heroVtbl[0] = (void*)VirtualTrap; hero[0] = heroVtbl; hero[1] = (void*)0x4845; hero[2] = 0;
    for (int i = 0; i < 600; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) implVtbl[i] = (void*)VirtualTrap;
    implVtbl[21] = (void*)I_HitBy; implVtbl[42] = (void*)I_AnySpecial; implVtbl[41] = (void*)I_Special;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_Start; gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThing;
    gameVtbl[77] = (void*)G_GetNearest; gameVtbl[108] = (void*)G_RemoveThing; gameVtbl[130] = (void*)G_Sheathe; gameVtbl[264] = (void*)G_GetHealth;
    gameVtbl[370] = (void*)G_StartMovie; gameVtbl[373] = (void*)G_FadeOut; gameVtbl[374] = (void*)G_FadeIn; gameVtbl[376] = (void*)G_Pause;
    gameVtbl[379] = (void*)G_PauseEntities; gameVtbl[411] = (void*)G_Camera; gameVtbl[416] = (void*)G_CameraDefault; gameVtbl[472] = (void*)G_Teleport;
    gameVtbl[474] = (void*)G_Facing; gameVtbl[475] = (void*)G_FacingTowards;
    int k = 0;
    for (int nm = 0; nm < 2; ++nm) {
        Run("1", "", "", "", "", "", "", "", "", "", 1.0f, 5.0f, nm, ++k);                         // terminating after the first frame
        Run("01", "", "", "", "", "", "", "", "", "", 1.0f, 5.0f, nm, ++k);                        // terminating at the loop head
        Run("0001", "0", "0", "", "", "", "", "", "", "", 1.0f, 5.0f, nm, ++k);                    // not hit, one frame
        Run("0001", "0", "1", "1", "", "", "", "", "", "", 1.0f, 5.0f, nm, ++k);                   // Heal Life only: not hit
    }
    Run("001", "1", "", "", "1", "", "", "", "", "", 1.0f, 5.0f, false, ++k);                          // hit, terminating
    Run("00001", "1", "", "", "0", "", "", "", "", "", 1.0f, 5.0f, false, ++k);                        // hit while healthy: keep watching
    Run("00001", "0", "1", "0", "0", "", "", "", "", "", 1.0f, 1.5f, false, ++k);                      // special hit, health 1.5
    Run("0001", "1", "", "", "1", "", "", "", "", "", 1.0f, 5.0f, false, ++k);                         // hit at 1: terminating before the scene
    Run("00001", "1", "", "", "1", "001", "", "", "", "", 0.25f, 5.0f, false, ++k);                    // waits to start, terminating
    Run("0000001", "1", "", "", "1", "1", "", "", "", "", 1.0f, 5.0f, false, ++k);                     // terminating after starting
    Run("00000000", "1", "", "", "1", "1", "0", "", "0", "", 1.0f, 5.0f, false, ++k);                  // no stand point, no speech
    Run("00000000", "0", "1", "0", "1", "1", "1", "", "0", "", 1.0f, 5.0f, false, ++k);                // stand point, terminating
    Run("000000000", "1", "", "", "1", "1", "11", "1", "1", "1101", 1.0f, 5.0f, false, ++k);           // everything
    Run("0000000000000000", "1", "", "", "1", "1", "10", "0", "1", "11", -3.0f, 5.0f, false, ++k);
    Run("00000000000000000000", "1", "", "", "1", "1", "11", "1", "1", "111", 1.0f, 5.0f, false, ++k);
    Run("0000000001", "1", "", "", "1", "1", "11", "1", "1", "", 1.0f, 5.0f, false, ++k);
    Run("00000000001", "1", "", "", "1", "1", "11", "1", "1", "", 1.0f, 5.0f, false, ++k);
    unsigned long seed = 97;
    for (int r = 0; r < 20000; ++r) {
        static char b[10][24];
        int mod[10] = { 11, 3, 2, 2, 3, 2, 2, 2, 2, 2 };
        for (int j = 0; j < 10; ++j) { for (int i = 0; i < 23; ++i) { seed = seed * 1103515245ul + 12345ul; b[j][i] = ((seed >> 16) % mod[j] == 0) ? '1' : '0'; } b[j][23] = 0; }
        seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 8);
        static const float hv[6] = { 1.0f, 0.5f, -2.0f, 1.0001f, 7.0f, 0.9999f };
        Run(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], hv[v % 6], hv[(v >> 3) % 6], ((v >> 6) % 7) == 0, 1000 + r);
    }
    if (failures) { printf("00e7c010_TEST FAIL %d\n", failures); return 1; }
    printf("00e7c010_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}