// Behaviour test for 0x00E72630 NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main() (links the REAL row object: the row body, its expanded
// seh_me / movie ctors, the expanded seh_me release at the first exit, the ScriptedThingBase dtor and movie dtor COMDATs and the pause expansion all
// run as compiled). No class models: every external the row object names is bound with /alternatename -- the calls the row makes to extern "C"
// __fastcall stubs (thiscall-compatible, unused edx), every vtable slot and unreached COMDAT reference to a trap. Game (+0x04) slots 7, 8, 70, 81, 264,
// 370, 379, 477, 516, 517, 526, 567 and Me's slot 27 through fake vtables; things carry real counted-pointer info blocks {RefCount, DeleteFunc, Data}
// that the row releases itself. Reference model: Model() below, written from the script's statements (the WhatSuitIsNext state machine with its
// fall-through cases); every run is compared event by event (with the destruction order of every live object on each early return) plus the final
// state (Me untouched, no store to this, every string / thing / handle released, the pause balanced).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:__purecall=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsEqualTo@CScriptThing@@MBE_NABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsNull@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDead@CScriptThing@@UBE_NXZ=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@BODtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@BOCtor@8")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@ThingDtor@8")
#pragma comment(linker, "/alternatename:??1CIsThingAlive@NScript@@UAE@XZ=@FilterDtor@8")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ=@MovieCPDtor@8")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=@IsPerf@8")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=@Speak@32")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=@GetScriptThing@12")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@ReleaseStub@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@IsValidStub@8")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@SetPred@12")
#pragma comment(linker, "/alternatename:?FadeOutFaceAndOpenDoor@CV_DemonDoorsScript@NScript@@QAEXABVCScriptThing@@ABVCCharString@@@Z=@FadeOutFace@16")
#pragma comment(linker, "/alternatename:?DressedAsBandit@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@AAE_NXZ=@DressBandit@8")
#pragma comment(linker, "/alternatename:?DressedAsEvilMage@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@AAE_NXZ=@DressMage@8")
#pragma comment(linker, "/alternatename:?DressedAsGoodKnight@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@AAE_NXZ=@DressKnight@8")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:_g_FilterVtbl=??_7CIsThingAlive@NScript@@6B@")
#pragma comment(linker, "/alternatename:_g_MovieVtbl=??_7CScriptGameResourceObjectMovieBase@@6B@")
#pragma comment(linker, "/alternatename:@CallRow@8=?Main@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@EAEXXZ")
extern "C" char g_FilterVtbl, g_MovieVtbl;
extern "C" void __fastcall CallRow(void* self, void*);

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
enum { NS = 9 };
static Script sTerm, sStart, sTalk, sKnight, sMage, sBandit, sSpeak, sPerf, sValid;
// past the end: terminating / started / not talked to / not dressed x3 / dead / not performing / (valid unused)

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 12000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static void Ws(const char* s) { while (*s) W(*s++); }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x40]; double a; } self;                 // CDF_ThreeSuits (0x1C)
static union { unsigned char b[0x60]; double a; } parent;               // CV_DemonDoorsScript (ParentClass)
static void* gameVtbl[600]; static void* gameObj[1];
static void* meVtbl[80];
static void* heroVtbl[1]; static void* hero[3];
struct Info { long RefCount; void* DeleteFunc; void* Data; };
static Info meInfo; static int liveInfos, deletes; static long nextData = 0x7000;
extern "C" void __fastcall DelFunc(void* data) { ++deletes; Ev('X'); (void)data; }
static Info* NewInfo() { Info* i = (Info*)malloc(sizeof(Info)); i->RefCount = 1; i->DeleteFunc = (void*)DelFunc; i->Data = (void*)(nextData += 0x10); ++liveInfos; return i; }
void __cdecl operator delete(void* p) { if (p != &meInfo) { free(p); --liveInfos; } }
static void FillThing(void** t) { Info* i = NewInfo(); t[0] = meVtbl; t[1] = i->Data; t[2] = i; }
static void** Me() { return (void**)(self.b + 8); }

// strings: one text pointer, logged by a letter per literal
static int liveStrings;
static char Lit(const char* t)
{
    if (!t) return '?';
    if (!strcmp(t, "SCRIPT_NAME_HERO")) return 'h'; if (!strcmp(t, "REGION_EXIT_POINT")) return 'e'; if (!strcmp(t, "DD_ThreeSuits")) return 'd';
    if (!*t) return '0'; return '?';
}
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; check(len == -1, "string length", 0); Ev('s'); Ev(Lit(text)); return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; Ev('~'); Ev(Lit(*s)); *s = 0; }

// things / predicate
static void Release(Info* i) { if (i && --i->RefCount == 0) { ((void (__fastcall*)(void*))i->DeleteFunc)(i->Data); operator delete(i); } }
extern "C" void* __fastcall ThingCopy(void** t, void*, void* const* o) { t[0] = o[0]; t[1] = o[1]; t[2] = o[2]; if (t[2]) ++((Info*)t[2])->RefCount; Ev('k'); return t; }
extern "C" void __fastcall ThingDtor(void** t, void*) { check(t[0] == meVtbl, "~CScriptThing on a thing", 0); Release((Info*)t[2]); t[2] = 0; }
extern "C" void __fastcall SetPred(void* s, void*, void* const* f)
{
    check(s == self.b && f[0] == (void*)&g_FilterVtbl && f[1] == meVtbl && f[2] == Me()[1] && f[3] == (void*)&meInfo, "predicate: CIsThingAlive(Me)", 0);
    Ev('P');
}
extern "C" void __fastcall FilterDtor(void** f, void*) { if (f[3]) --((Info*)f[3])->RefCount; Ev('p'); }
static void* regionExit;

// Me's own vtable: slot 27 MsgIsTalkedToBy
extern "C" bool __fastcall M_TalkedTo(void* t, void*, const char* const* who) { check(t == Me() && !strcmp(*who, "SCRIPT_NAME_HERO"), "MsgIsTalkedToBy args", 0); bool r = Next(sTalk); Ev(r ? 'A' : 'a'); return r; }

// ScriptedThingBase {vptr, IntelligentPointer, Data, Info}; movie {vptr, IntelligentPointer, PImp, Info}
static void* sehObj;
extern "C" void* __fastcall BOCtor(void** o, void*) { o[1] = 0; Ev('o'); return o; }
extern "C" void __fastcall BODtor(void** o, void*) { (void)o; Ev('O'); }
extern "C" void __fastcall MovieCPDtor(void** cp, void*) { check(cp[0] == 0 && cp[1] == 0, "movie counted pointer empty", 0); Ev('c'); }
extern "C" bool __fastcall IsValidStub(void* const* h, void*) { bool r = h[2] != 0; Ev('v'); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall ReleaseStub(void** h, void*) { VirtualTrap(); (void)h; }   // never reached: the handle is fresh
extern "C" void* __fastcall GetScriptThing(void* const* h, void*, void** ret) { check(h == sehObj, "GetScriptThing on seh_me", 0); FillThing(ret); Ev('R'); return ret; }
extern "C" bool __fastcall IsPerf(void* h, void*) { check(h == sehObj, "performing on seh_me", 0); bool r = Next(sPerf); Ev(r ? 'Q' : 'q'); return r; }
static char TextLetter(const char* t)
{
    static const char* keys[8] = { "TEXT_QST_088_THREE_SUITS_INTRO", "TEXT_QST_088_THREE_SUITS_SEEN_KNIGHT", "TEXT_QST_088_THREE_SUITS_WANT_KNIGHT",
        "TEXT_QST_088_THREE_SUITS_SEEN_MAGE", "TEXT_QST_088_THREE_SUITS_WANT_MAGE", "TEXT_QST_088_THREE_SUITS_SEEN_BANDIT",
        "TEXT_QST_088_THREE_SUITS_WANT_BANDIT", "TEXT_QST_088_THREE_SUITS_OPENING" };
    static const char let[8] = { 'i', 'k', 'K', 'm', 'M', 'b', 'B', 'n' };
    for (int i = 0; i < 8; ++i) if (!strcmp(t, keys[i])) return let[i];
    return '?';
}
extern "C" void __fastcall Speak(void* h, void*, void* const* to, const char* text, long sel, int b1, int b2, int b3)
{
    check(h == sehObj && to == (void* const*)hero && sel == 0 && (b1 & 0xFF) == 0 && (b2 & 0xFF) == 1 && (b3 & 0xFF) == 0, "speak args", 0); Ev('K'); Ev(TextLetter(text));
}
extern "C" bool __fastcall DressKnight(void* s, void*) { check(s == self.b, "knight test on this", 0); bool r = Next(sKnight); Ev('J'); Ev(r ? '1' : '0'); return r; }
extern "C" bool __fastcall DressMage(void* s, void*) { check(s == self.b, "mage test on this", 0); bool r = Next(sMage); Ev('L'); Ev(r ? '1' : '0'); return r; }
extern "C" bool __fastcall DressBandit(void* s, void*) { check(s == self.b, "bandit test on this", 0); bool r = Next(sBandit); Ev('I'); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall FadeOutFace(void* p, void*, void* const* face, const char* const* door)
{
    check(p == parent.b && face == (void* const*)Me() && !strcmp(*door, "DD_ThreeSuits"), "FadeOutFaceAndOpenDoor args", 0); Ev('D');
}

// Game
static bool T_(void* s) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall IsTerm(void* s, void*) { return T_(s); }
static void G(void* g, const char* what) { check(g == gameObj, what, 0); }
extern "C" void __fastcall G_NewScriptFrame(void* g, void*) { G(g, "frame this"); Ev('F'); }
extern "C" bool __fastcall G_Start(void* g, void*, void* const* thing, void** h, long prio)
{
    G(g, "start this"); check(thing == (void* const*)Me() && prio == 4, "start scripting Me at priority 4", 0); sehObj = h;
    bool r = Next(sStart); Ev(r ? 'S' : 'n');
    if (r) { check(!h[2] && !h[3], "started into an empty handle", 0); Info* i = NewInfo(); h[2] = i->Data; h[3] = i; }
    return r;
}
extern "C" void* __fastcall G_GetHero(void* g, void*) { G(g, "hero this"); Ev('Y'); return hero; }
extern "C" void* __fastcall G_GetNearestDef(void* g, void*, void** ret, void* const* from, const char* const* name)
{
    G(g, "nearest this"); check(from == (void* const*)Me() && !strcmp(*name, "REGION_EXIT_POINT"), "nearest REGION_EXIT_POINT from Me", 0);
    FillThing(ret); regionExit = ret; Ev('G'); return ret;
}
extern "C" void __fastcall G_SetRegionExit(void* g, void*, void* const* t, int on) { G(g, "region exit this"); check(t == regionExit, "region exit thing", 0); Ev('x'); Ev((on & 0xFF) ? '1' : '0'); }
extern "C" void __fastcall G_Persistent(void* g, void*, void* const* t, int on) { G(g, "persistent this"); check(t == (void* const*)Me() && (on & 0xFF) == 1, "SetThingPersistent(Me, true)", 0); Ev('1'); }
extern "C" void __fastcall G_Damageable(void* g, void*, void* const* t, int on) { G(g, "damageable this"); check(t == (void* const*)Me() && (on & 0xFF) == 0, "EntitySetAsDamageable(Me, false)", 0); Ev('2'); }
extern "C" void __fastcall G_Combo(void* g, void*, void* const* t, int on) { G(g, "combo this"); check(t == (void* const*)Me() && (on & 0xFF) == 0, "EntitySetAsToAddToComboMultiplierWhenHit(Me, false)", 0); Ev('3'); }
extern "C" void __fastcall G_Killable(void* g, void*, void* const* t, int a, int b) { G(g, "killable this"); check(t == (void* const*)Me() && (a & 0xFF) == 0 && (b & 0xFF) == 0, "EntitySetAsKillable(Me, false, false)", 0); Ev('4'); }
static int healthMode;
extern "C" float __fastcall G_GetHealth(void* g, void*, void* const* t)
{
    G(g, "health this"); check(t[0] == meVtbl, "health of the script thing", 0);
    bool alive = Next(sSpeak); Ev(alive ? 'U' : 'u'); return alive ? (healthMode ? 0.001f : 3.0f) : (healthMode ? -1.0f : 0.0f);
}
extern "C" void __fastcall G_StartMovie(void* g, void*, const char* const* name, void* const* movie)
{
    G(g, "movie this"); check(!strcmp(*name, "") && movie[0] == (void*)&g_MovieVtbl && !movie[2] && !movie[3], "movie sequence args", 0); Ev('m');
}
static int pauseDepth;
extern "C" void __fastcall G_PauseEntities(void* g, void*, int on) { G(g, "pause entities this"); pauseDepth += (on & 0xFF) ? 1 : -1; Ev((on & 0xFF) ? 'Z' : 'z'); }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mStart, mTalk, mKnight, mMage, mBandit, mSpeak, mPerf, mValid;
static char live[16][4]; static int nlive;                               // destruction events of the live locals, innermost last
static void Push(const char* d) { strcpy(live[nlive++], d); }
static void Pop() { --nlive; Ws(live[nlive]); }
static bool T() { bool r = Next(mTerm); W(r ? 'T' : 't'); if (r) while (nlive) Pop(); return r; }
static int mState;                                                       // WhatSuitIsNext
static bool Say(char text)                                               // the speak-and-wait macro; true = returned
{
    W('R'); bool sp = Next(mSpeak); W(sp ? 'U' : 'u'); W('X');
    if (!sp) return false;
    Ws("YK"); W(text);
    for (;;) { bool pf = Next(mPerf); W(pf ? 'Q' : 'q'); if (!pf) break; W('F'); if (T()) return true; }
    return T();
}
static bool Dressed(char tag, Script& s) { bool r = Next(s); W(tag); W(r ? '1' : '0'); return r; }
static void Model()
{
    W('o'); Push("O");                                                   // seh_me
    Ws("kPp" "F"); if (T()) return;
    Ws("v0");
    for (;;) { bool r = Next(mStart); W(r ? 'S' : 'n'); if (r) { strcpy(live[nlive - 1], "XO"); break; } W('F'); if (T()) return; }
    if (T()) return;
    Ws("1234");
    Ws("seG~e"); Push("X");                                              // region_exit
    Ws("x0");
    mState = 0;
    for (;;) {
        if (T()) return;
        Ws("sh"); bool talk = Next(mTalk); W(talk ? 'A' : 'a'); Ws("~h");
        if (talk) {
            if (T()) return;
            W('o'); Push("cO"); Ws("s0m~0");                             // speech_movie
            W('Z'); Push("z");                                           // PauseMyEntitiesPlease
            bool ret = false;
            switch (mState) {
            case 0:
                if (Say('i')) return;
                mState = 1;
            case 1:
                if (Dressed('J', mKnight)) { if (T()) return; if (Say('k')) return; mState = 2; }
                else { if (T()) return; if (Say('K')) return; break; }
            case 2:
                if (Dressed('L', mMage)) { if (T()) return; if (Say('m')) return; mState = 3; }
                else { if (T()) return; if (Say('M')) return; break; }
            case 3:
                if (Dressed('I', mBandit)) {
                    if (T()) return; if (Say('b')) return; if (Say('n')) return;
                    Ws("sdD~d"); Ws("x1"); ret = true;
                } else { if (T()) return; if (Say('B')) return; }
            }
            if (ret) { while (nlive) Pop(); return; }
            Pop(); Pop();                                                // ~PauseMyEntitiesPlease, ~speech_movie
        }
        W('F');
    }
}

static void Run(const char* term, const char* st, const char* talk, const char* kn, const char* mg, const char* bd, const char* speak,
                const char* perf, int hm, int k)
{
    memset(self.b, 0xEE, sizeof self.b);
    *(void**)(self.b + 0x00) = (void*)0x5E1F;                            // the entity vtable (unused by Main)
    *(void**)(self.b + 0x04) = gameObj;
    Me()[0] = meVtbl; Me()[1] = (void*)0x4D45; Me()[2] = &meInfo;
    *(void**)(self.b + 0x14) = parent.b; *(void**)(self.b + 0x18) = (void*)0x5DA7;
    healthMode = hm;
    Script* ss[8] = { &sTerm, &sStart, &sTalk, &sKnight, &sMage, &sBandit, &sSpeak, &sPerf };
    const char* tx[8] = { term, st, talk, kn, mg, bd, speak, perf };
    bool df[8] = { true, true, false, false, false, false, false, false };
    for (int i = 0; i < 8; ++i) { ss[i]->s = tx[i]; ss[i]->pos = 0; ss[i]->dflt = df[i]; }
    mTerm = sTerm; mStart = sStart; mTalk = sTalk; mKnight = sKnight; mMage = sMage; mBandit = sBandit; mSpeak = sSpeak; mPerf = sPerf;
    ngot = nwant = 0; nlive = 0; liveStrings = liveInfos = deletes = pauseDepth = 0; sehObj = regionExit = 0;
    meInfo.RefCount = 1; meInfo.DeleteFunc = (void*)VirtualTrap; meInfo.Data = Me()[1];
    Model();
    CallRow(self.b, 0);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) printf("  got  %.600s\n  want %.600s\n", got, want);
    }
    check(meInfo.RefCount == 1 && Me()[0] == meVtbl && Me()[1] == (void*)0x4D45 && Me()[2] == &meInfo, "Me untouched", k);
    check(*(void**)(self.b + 0x14) == parent.b && *(void**)(self.b + 0x18) == (void*)0x5DA7 && self.b[0x1C] == 0xEE, "no store to this", k);
    check(liveStrings == 0 && liveInfos == 0 && pauseDepth == 0, "everything released", k);
}

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; heroVtbl[0] = (void*)VirtualTrap; hero[0] = heroVtbl; hero[1] = (void*)0x4845; hero[2] = 0;
    for (int i = 0; i < 600; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) meVtbl[i] = (void*)VirtualTrap;
    meVtbl[27] = (void*)M_TalkedTo;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_Start; gameVtbl[70] = (void*)G_GetHero; gameVtbl[81] = (void*)G_GetNearestDef;
    gameVtbl[264] = (void*)G_GetHealth; gameVtbl[370] = (void*)G_StartMovie; gameVtbl[379] = (void*)G_PauseEntities;
    gameVtbl[477] = (void*)G_Persistent; gameVtbl[516] = (void*)G_Damageable; gameVtbl[517] = (void*)G_Killable; gameVtbl[526] = (void*)G_Combo;
    gameVtbl[567] = (void*)G_SetRegionExit;
    int k = 0;
    //   term                    start  talk    knight mage   bandit speak          perf
    Run("1",                    "",    "",     "",    "",    "",    "",            "",     0, ++k);   // terminating after the first frame
    Run("01",                   "001", "",     "",    "",    "",    "",            "",     0, ++k);   // waiting to start, terminating
    Run("0001",                 "1",   "",     "",    "",    "",    "",            "",     0, ++k);   // terminating after starting
    Run("00001",                "1",   "0",    "",    "",    "",    "",            "",     0, ++k);   // not talked to, terminating at the head
    Run("000001",               "1",   "1",    "",    "",    "",    "0",           "",     0, ++k);   // talked to, intro silent, then the knight test
    Run("0000000001",           "1",   "1",    "0",   "",    "",    "11",          "1",    0, ++k);   // intro said, wants the knight
    Run("000000000000000000001","1",   "11",   "11",  "1",   "1",   "11111111",    "",     0, ++k);   // the whole sequence in two talks
    Run("000000000000000000000","1",   "1",    "1",   "1",   "1",   "11111",       "1010", 1, ++k);   // all at once, door opens
    Run("0000000000000000000000000001","1", "0101", "01", "01", "01", "1111111111", "", 0, ++k);
    Run("00000000000000000000000000000000000001","1","1111","1","0101","01","00000000","",1, ++k);
    unsigned long seed = 131;
    for (int r = 0; r < 20000; ++r) {
        static char b[8][40];
        int mod[8] = { 17, 3, 2, 2, 2, 2, 2, 3 };
        for (int j = 0; j < 8; ++j) { for (int i = 0; i < 39; ++i) { seed = seed * 1103515245ul + 12345ul; b[j][i] = ((seed >> 16) % mod[j] == 0) ? '1' : '0'; } b[j][39] = 0; }
        seed = seed * 1103515245ul + 12345ul;
        Run(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], (int)((seed >> 9) & 1), 1000 + r);
    }
    if (failures) { printf("00e72630_TEST FAIL %d\n", failures); return 1; }
    printf("00e72630_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}