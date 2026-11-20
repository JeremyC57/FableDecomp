// Behaviour test for 0x00DD4AD0 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main (links the REAL row object: the row
// body, the CIsThingAlive ctor / dtor compiled in their inline_depth(0) region, the TU's inline ~CScriptThing (its counted-pointer release
// and the ~CBaseClass call), the forced CTimer operator= / operator==, the expanded and called ~CTimer, the TU's implicit handle / movie
// special members (the movie's counted-pointer dtor is the out-of-line 0x006E7AB0, stubbed here with its retail release), the inline
// CScriptThing::MsgIsUsedBy through the cell door's implementation, and CWideScreenMagicPauseEntities all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the entity (0x20 bytes: Game +4, Me +8, ParentClass +0x14,
// PMasterData +0x18, ReleasedAndFollowing +0x1c); the parent (0x80 bytes: PrisonCutsceneFinished +0x6a); the master data
// (PrisonRaceNumber +0xe8, PrisonRaceWonByHero +0xec, PrisonKeyStolenByHero +0xee); the global script def (PrisonFreePrisonerMorality
// +0xde8); Me with a counted pointer-info record; the cell door with a counted record and an implementation whose slot 20 is MsgIsUsedBy;
// things with fake vtables (MsgIsHitBy 21, MsgIsTalkedToBy 27, MsgIsHitBySpecialAbilityFrom 41, MsgIsHitByAnySpecialAbilityFrom 42);
// handles with counted implementation records; pDSTGame's timer slots 87..90 (a ticking timer: every NewScriptFrame counts it down);
// Game's vtable slots 7, 8, 12, 70, 77, 156, 229, 235, 264, 361, 364, 365, 366, 370, 379, 477, 480, 601. A scripted world changes the
// cutscene flag, the talk / door-use / hit / ability / heal messages, the region load and the speech task at every NewScriptFrame;
// termination, StartScriptingEntity, MakePrisonerComment and the health check are scripted too.
// Reference model: Model() below, written from the quest's statements; every run is compared event by event, then the entity's flag,
// the parent's flag, the timer's state, and the balance of strings, things, CBaseObject parts, movies and reference counts.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); fflush(stdout); exit(3); }
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:?pDSTGame@@3PAVCGameScriptInterfaceBase@@A=_g_pDSTGame")
#pragma comment(linker, "/alternatename:??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ=@StubMoviePtrDtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@StubBaseCtor@8")
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
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=@StubBaseClassDtor@8")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@StubThingCopy@12")
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=@StubBusy@8")
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
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=@StubSpeak@32")
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=@StubGetScriptThing@12")
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
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A=_g_pGlobalScriptDef")
#pragma comment(linker, "/alternatename:?MakePrisonerComment@CQ_PrisonEscapeRescueMotherScript@NScript@@QAE_NABVCScriptThing@@ABVCCharString@@@Z=@StubComment@16")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@StubRelease@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@StubIsValid@8")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@StubTerminating@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@StubSetPredicate@12")
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CPrisonerAccomplice@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ")
extern "C" void __fastcall CallMain(void* self);

// ---- strings (one pointer, like retail's CCharString) ---------------------------------------------------------------------------
static int liveStrings;
class CCharString { public: CCharString(const char* text, long length); ~CCharString(); char* Text; };
static char* Dup(const char* s) { size_t n = strlen(s); char* p = (char*)malloc(n + 1); memcpy(p, s, n + 1); return p; }
CCharString::CCharString(const char* text, long length) { Text = Dup(text); ++liveStrings; if (length != -1) { printf("FAIL string length\n"); exit(4); } }
CCharString::~CCharString() { free(Text); Text = 0; --liveStrings; }

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
static const char *termScript, *startScript, *commentScript;   // '1' = true; past the end: true / true / false
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

// ---- world -----------------------------------------------------------------------------------------------------------------------
enum { FRAMES = 64 };
struct World {
    unsigned char pcf[FRAMES], talk[FRAMES], used[FRAMES], hit[FRAMES], ability[FRAMES], heal[FRAMES];
    int regionAt, speakFrames, raceNumber, timer0; bool key, won, raf0, doorNull;
};
static World world;
struct State { int frame, tpos, spos, cpos, busyUntil; bool valid; long timer; bool timerLive; };
static State gs, ms;
static int F(int f) { return f < FRAMES ? f : FRAMES - 1; }

static union { unsigned char b[0x20]; double align; } ent;
static union { unsigned char b[0x80]; double align; } parent;
static union { unsigned char b[0x100]; double align; } master;
static union { unsigned char b[0x1000]; double align; } def;
extern "C" { void* g_pGlobalScriptDef; void* g_pDSTGame; }

// things: { vptr, PImp.Data, PImp.Info }
struct Info { unsigned long RefCount; void* DeleteFunc; void* Data; };
struct Impl { void** vtbl; const char* name; };
struct Thing { void** vtbl; Impl* data; Info* info; };
static void* thingVtbl[80]; static void* doorImplVtbl[40];
static Impl meImpl, heroImpl, doorImpl;
static Info meInfo, doorInfo;
static Thing heroThing;
static Thing* Me() { return (Thing*)(ent.b + 8); }
static int liveThings, baseLive, movieLive;
static void* doorAddr;
static const char* Who(const void* t)
{ const Thing* th = (const Thing*)t; return t == Me() ? "Me" : t == &heroThing ? "Hero" : t == doorAddr ? "Door" : (th->data == &meImpl ? "MeCopy" : th->data ? th->data->name : "null"); }

// handles: { vptr, IntelligentPointer, PImp.Data, PImp.Info }
struct Handle { void* vtbl; void* ip; void* data; Info* info; };
static Info hMeInfo; static int meStbImpl;
enum { REF0 = 1000 };
static void* sehMeAddr;

extern "C" {
void __fastcall StubBaseCtor(void* self, void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseDtor(void*, void*) { --baseLive; }
void __fastcall StubBaseClassDtor(void* self, void*) { --liveThings; memset((char*)self + 4, 0xDD, 8); }   // only CScriptThing reaches ~CBaseClass here
void* __fastcall StubThingCopy(void* self, void*, const void* o)
{ Thing* t = (Thing*)self; const Thing* s = (const Thing*)o; t->vtbl = s->vtbl; t->data = s->data; t->info = s->info; if (t->info) ++t->info->RefCount; ++liveThings; return self; }
void __fastcall StubMoviePtrDtor(void* self, void*)
{   // CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer 0x006E7AB0
    void** p = (void**)self; Info* info = (Info*)p[1];
    if (info) { if (--info->RefCount == 0) { ((void (__fastcall*)(void*))info->DeleteFunc)(info->Data); delete info; } }
    p[0] = 0; p[1] = 0;
}
bool __fastcall StubComment(void* self, void*, const void* who, const CCharString* what)
{ if (self != parent.b) Bad("comment receiver"); if (who != Me()) Bad("comment thing"); bool r = NextBit(commentScript, gs.cpos, false); char s[64]; sprintf(s, "COMMENT:%s=%d", what->Text, r); Ev(s); return r; }
bool __fastcall StubIsValid(const void* self, void*) { if (self != sehMeAddr && sehMeAddr) Bad("IsValid receiver"); return ((const Handle*)self)->data != 0; }
void __fastcall StubRelease(void* self, void*)
{ Handle* h = (Handle*)self; if (self != sehMeAddr) Bad("Release receiver"); if (h->info) --h->info->RefCount; h->data = 0; h->info = 0; Ev("R"); }
static void CheckMe(const void* self) { if (self != sehMeAddr || ((const Handle*)self)->data != &meStbImpl) Bad("seh_me receiver"); }
void __fastcall StubSpeak(void* self, void*, const void* to, const char* text, int sel, bool a, bool b, bool c)
{ CheckMe(self); char s[96]; sprintf(s, "SPEAK:%s/%s/%d/%d%d%d", Who(to), text, sel, a, b, c); Ev(s); gs.busyUntil = gs.frame + world.speakFrames; }
bool __fastcall StubBusy(const void* self, void*) { CheckMe(self); bool r = gs.frame < gs.busyUntil; Ev(r ? "BUSY1" : "BUSY0"); return r; }
void* __fastcall StubGetScriptThing(const void* self, void*, Thing* ret)
{ CheckMe(self); ret->vtbl = thingVtbl; ret->data = &meImpl; ret->info = 0; ++liveThings; Ev("SCRIPTTHING"); return ret; }
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

// Me's messages (CScriptThing virtuals) and the door implementation's MsgIsUsedBy (slot 20)
static void CheckMeThing(const void* self) { if (self != Me()) Bad("message receiver"); }
static bool __fastcall T_HitBy(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.hit[F(gs.frame)] != 0; char s[64]; sprintf(s, "HIT:%s=%d", w->Text, r); Ev(s); return r; }
static bool __fastcall T_TalkedTo(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.talk[F(gs.frame)] != 0; char s[64]; sprintf(s, "TALK:%s=%d", w->Text, r); Ev(s); return r; }
static bool __fastcall T_AbilityFrom(const void* self, void*, int ab, const CCharString* w)
{ CheckMeThing(self); bool r = world.heal[F(gs.frame)] != 0; char s[64]; sprintf(s, "ABIL%d:%s=%d", ab, w->Text, r); Ev(s); return r; }
static bool __fastcall T_AnyAbilityFrom(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.ability[F(gs.frame)] != 0; char s[64]; sprintf(s, "ANYABIL:%s=%d", w->Text, r); Ev(s); return r; }
static bool __fastcall D_UsedBy(const void* self, void*, const CCharString* w)
{ if (self != &doorImpl) Bad("door impl receiver"); bool r = world.used[F(gs.frame)] != 0; char s[64]; sprintf(s, "USED:%s=%d", w->Text, r); Ev(s); return r; }

// ---- pDSTGame timers -------------------------------------------------------------------------------------------------------------
static void* dstVtbl[100]; static void* dstObj[1];
static long __fastcall D_Register(void* self, void*) { if (self != dstObj) Bad("timer receiver"); if (gs.timerLive) Bad("second timer"); gs.timerLive = true; gs.timer = world.timer0; Ev("TIMER"); return 5; }
static void __fastcall D_Deregister(void* self, void*, long i) { if (self != dstObj || i != 5) Bad("deregister"); gs.timerLive = false; Ev("DEREG"); }
static void __fastcall D_Set(void* self, void*, long i, long v) { if (self != dstObj || i != 5) Bad("set timer"); gs.timer = v; char s[24]; sprintf(s, "SET:%ld", v); Ev(s); }
static long __fastcall D_Get(const void* self, void*, long i) { if (self != dstObj || i != 5) Bad("get timer"); char s[24]; sprintf(s, "GT=%ld", gs.timer); Ev(s); return gs.timer; }

// ---- game interface ----------------------------------------------------------------------------------------------------------------
static void* gameVtbl[900]; static void* gameObj[1]; static bool badGame;
static void CheckGame(const void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*)
{ CheckGame(self); Ev("N"); ++gs.frame; if (gs.timer > 0) --gs.timer; parent.b[0x6a] = world.pcf[F(gs.frame)]; }
static bool __fastcall G_StartScripting(void* self, void*, const void* who, void* handle, int prio)
{
    CheckGame(self); char s[32];
    if (who != Me()) Bad("StartScriptingEntity thing");
    if (!sehMeAddr) sehMeAddr = handle; else if (handle != sehMeAddr) Bad("StartScriptingEntity handle");
    bool ok = NextBit(startScript, gs.spos, true);
    Handle* h = (Handle*)handle;
    if (ok) { ++hMeInfo.RefCount; if (h->info) --h->info->RefCount; h->data = &meStbImpl; h->info = &hMeInfo; }
    sprintf(s, "%c%d", ok ? 'S' : 's', prio); Ev(s); return ok;
}
static bool __fastcall G_RegionLoaded(void* self, void*, const CCharString* r)
{ CheckGame(self); bool b = gs.frame >= world.regionAt; char s[64]; sprintf(s, "REGION:%s=%d", r->Text, b); Ev(s); return b; }
static Thing* __fastcall G_GetHero(void* self, void*) { CheckGame(self); return &heroThing; }
static Thing* __fastcall G_Nearest(void* self, void*, Thing* ret, const void* near, const CCharString* name)
{
    CheckGame(self); char s[64]; sprintf(s, "NEAREST:%s/%s", Who(near), name->Text); Ev(s);
    ret->vtbl = thingVtbl; ret->data = world.doorNull ? 0 : &doorImpl; ret->info = world.doorNull ? 0 : &doorInfo;
    if (ret->info) ++ret->info->RefCount; ++liveThings; doorAddr = ret; return ret;
}
static void __fastcall G_Morality(const void* self, void*, float m) { CheckGame(self); char s[32]; sprintf(s, "MORALITY:%g", m); Ev(s); }
static void __fastcall G_FollowThing(void* self, void*, const void* t, const void* target, float d, bool b)
{ CheckGame(self); char s[64]; sprintf(s, "FOLLOW:%s-%s/%g/%d", Who(t), Who(target), d, b); Ev(s); }
static void __fastcall G_AllowFollow(void* self, void*, const void* t, bool b) { CheckGame(self); char s[48]; sprintf(s, "ALLOWFOLLOW:%s/%d", Who(t), b); Ev(s); }
static float healthNext; static const float* healthScript; static int healthPos, mHealthPos;
static float __fastcall G_GetHealth(void* self, void*, const void* thing)
{ CheckGame(self); healthNext = healthScript[healthPos++]; char s[48]; sprintf(s, "HEALTH:%s=%g", Who(thing), healthNext); Ev(s); return healthNext; }
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
    ((void**)movie)[2] = &movieImpl; ((void**)movie)[3] = info; ++movieLive;
}
static void __fastcall G_Pause(void* self, void*, bool b) { CheckGame(self); Ev(b ? "PAUSE1" : "PAUSE0"); }
static void __fastcall G_Persistent(void* self, void*, const void* t, bool b) { CheckGame(self); char s[48]; sprintf(s, "PERSIST:%s/%d", Who(t), b); Ev(s); }
static void __fastcall G_RegionFollow(void* self, void*, const void* a, const void* b, bool f)
{ CheckGame(self); char s[64]; sprintf(s, "REGIONFOLLOW:%s-%s/%d", Who(a), Who(b), f); Ev(s); }
static void __fastcall G_Faction(void* self, void*, const void* t, const CCharString* f) { CheckGame(self); char s[64]; sprintf(s, "FACTION:%s/%s", Who(t), f->Text); Ev(s); }

// ---- reference model -----------------------------------------------------------------------------------------------------------
static bool mInTalk;
static void MExit() { if (mInTalk) { Wv("PAUSE0"); Wv("~MOVIE"); } if (ms.timerLive) Wv("DEREG"); }
static bool MT() { bool t = NextBit(termScript, ms.tpos, true); Wv(t ? "T" : "t"); if (t) MExit(); return t; }
static bool mPcf;
static void MF() { Wv("N"); ++ms.frame; if (ms.timer > 0) --ms.timer; mPcf = world.pcf[F(ms.frame)] != 0; }
static bool MStart(int prio) { bool ok = NextBit(startScript, ms.spos, true); if (ok) ms.valid = true; char s[8]; sprintf(s, "%c%d", ok ? 'S' : 's', prio); Wv(s); return ok; }
static void MRelease() { if (ms.valid) { Wv("R"); ms.valid = false; } }
static long MGet() { char s[24]; sprintf(s, "GT=%ld", ms.timer); Wv(s); return ms.timer; }
static void MSet(long v) { ms.timer = v; char s[24]; sprintf(s, "SET:%ld", v); Wv(s); }
static bool MComment(const char* w) { bool r = NextBit(commentScript, ms.cpos, false); char s[64]; sprintf(s, "COMMENT:%s=%d", w, r); Wv(s); return r; }
static bool MHit()
{
    char s[64]; int f = F(ms.frame);
    bool hit = world.hit[f] != 0; sprintf(s, "HIT:SCRIPT_NAME_HERO=%d", hit); Wv(s);
    if (!hit) {
        bool any = world.ability[f] != 0; sprintf(s, "ANYABIL:SCRIPT_NAME_HERO=%d", any); Wv(s);
        if (any) { bool heal = world.heal[f] != 0; sprintf(s, "ABIL14:SCRIPT_NAME_HERO=%d", heal); Wv(s); hit = !heal; }
    }
    return hit;
}
static void MConv(const char* line)
{
    char s[96]; Wv("CONV:Me/00"); long i = ++convIndex; sprintf(s, "PERSON:%ld/Hero", i); Wv(s);
    sprintf(s, "LINE:%ld/%s/0/Me/Hero", i, line); Wv(s);
}
// one speech line: false = the thread ended (the exit events are written)
static bool MSpeak(const char* text)
{
    char s[96]; Wv("SCRIPTTHING");
    float h = healthScript[mHealthPos++]; sprintf(s, "HEALTH:MeCopy=%g", h); Wv(s);
    if (h > 0.0f) {
        sprintf(s, "SPEAK:Hero/%s/0/010", text); Wv(s); int busyUntil = ms.frame + world.speakFrames;
        for (;;) { bool b = ms.frame < busyUntil; Wv(b ? "BUSY1" : "BUSY0"); if (!b) break; MF(); if (MT()) return false; }
        if (MT()) return false;
    }
    return true;
}
struct Expect { bool raf; };
static Expect ex;
static void Model()
{
    char s[96];
    ex.raf = world.raf0;
    Wv("P"); MF(); if (MT()) return;
    Wv("TIMER"); ms.timerLive = true; ms.timer = world.timer0;
    mPcf = false;
    while (!mPcf) { MF(); if (MT()) return; }
    if (MT()) return;
    Wv("NEAREST:Me/CellDoor");
    while (!ex.raf) {
        MF(); if (MT()) return;
        while (!mPcf) { MF(); if (MT()) return; }
        if (MT()) return;
        bool talk = world.talk[F(ms.frame)] != 0; sprintf(s, "TALK:SCRIPT_NAME_HERO=%d", talk); Wv(s);
        if (talk) {
            if (MT()) return;
            MRelease();
            while (!MStart(4)) { MF(); if (MT()) return; }
            if (MT()) return;
            Wv("MOVIE:[]"); Wv("PAUSE1"); mInTalk = true;
            if (world.key) { if (MT()) return; if (!MSpeak("TEXT_QST_055_PRISON_PRISONER_KEY")) return; }
            else if (world.won) { if (MT()) return; if (!MSpeak("TEXT_QST_055_PRISON_PRISONER_HINT2")) return; }
            else if (world.raceNumber > 1) { if (MT()) return; if (!MSpeak("TEXT_QST_055_PRISON_PRISONER_HINT3")) return; }
            else {
                if (MT()) return;
                if (!MSpeak("TEXT_QST_055_PRISON_PRISONER_HINT4_01")) return;
                if (!MSpeak("TEXT_QST_055_PRISON_PRISONER_HINT4_02")) return;
            }
            MRelease(); Wv("PAUSE0"); Wv("~MOVIE"); mInTalk = false;
        }
        if (!world.doorNull) {
            bool used = world.used[F(ms.frame)] != 0; sprintf(s, "USED:SCRIPT_NAME_HERO=%d", used); Wv(s);
            if (used) {
                if (MT()) return;
                MConv("TEXT_QST_055_PRISON_PRISONER_KEY");
                Wv("PERSIST:Me/1"); Wv("CLEARINFO:Me"); Wv("MORALITY:2.5");
                ex.raf = true;
            }
        }
    }
    if (MT()) return;
    MRelease();
    Wv("ALLOWFOLLOW:Me/1"); Wv("FOLLOW:Me-Hero/3/1"); Wv("REGIONFOLLOW:Hero-Me/1"); Wv("PERSIST:Me/1"); Wv("FACTION:Me/FACTION_PRISONER");
    for (;;) {
        bool loaded = ms.frame >= world.regionAt; sprintf(s, "REGION:PrisonCourtyard=%d", loaded); Wv(s);
        if (loaded) break;
        MF(); if (MT()) return;
        if (MGet() == 0 && MComment("COURTYARD")) { if (MT()) return; MSet(30); }
        if (MHit()) { if (MT()) return; MConv("TEXT_QST_055_PRISON_PRISONER_ON_HIT"); }
    }
    if (MT()) return;
    Wv("REGIONFOLLOW:Hero-Me/0");
    for (;; MF()) {
        if (MT()) return;
        if (MGet() == 0 && MComment("GUARDS")) { if (MT()) return; MSet(30); }
        if (MHit()) { if (MT()) return; MConv("TEXT_QST_055_PRISON_PRISONER_ON_HIT"); }
    }
}

static int cov[16];
static void Run(const World& w, const char* term, const char* start, const char* comment, const float* health, int k)
{
    world = w; termScript = term; startScript = start; commentScript = comment; healthScript = health; healthPos = mHealthPos = 0;
    memset(&gs, 0, sizeof gs); memset(&ms, 0, sizeof ms); ngot = nwant = 0; got[0] = want[0] = 0; mInTalk = false;
    memset(parent.b, 0x5A, sizeof parent.b); parent.b[0x6a] = 1;
    memset(master.b, 0, sizeof master.b); memcpy(master.b + 0xe8, &w.raceNumber, 4); master.b[0xec] = w.won; master.b[0xee] = w.key;
    memset(ent.b, 0x11, sizeof ent.b); void* g = gameObj; memcpy(ent.b + 4, &g, 4); void* pp = parent.b; memcpy(ent.b + 0x14, &pp, 4);
    void* mp = master.b; memcpy(ent.b + 0x18, &mp, 4); ent.b[0x1c] = w.raf0;
    Thing* me = Me(); me->vtbl = thingVtbl; me->data = &meImpl; me->info = &meInfo;
    meInfo.RefCount = REF0; doorInfo.RefCount = REF0; hMeInfo.RefCount = REF0;
    liveThings = baseLive = liveStrings = movieLive = 0; badGame = false; sehMeAddr = 0; doorAddr = 0; memset(&ex, 0, sizeof ex);
    convIndex = 0; Model(); convIndex = 0;
    CallMain(ent.b);
    if (strcmp(got, want)) {
        ++failures;
        if (failures < 6) {
            int i = 0; while (got[i] && got[i] == want[i]) ++i;
            int a = i > 160 ? i - 160 : 0;
            printf("FAIL case %d events differ at %d\n  got  ...%.240s\n  want ...%.240s\n", k, i, got + a, want + a);
        }
        return;
    }
    check(!badGame, "Game receiver", k);
    check((ent.b[0x1c] != 0) == ex.raf, "ReleasedAndFollowing", k);
    check(!gs.timerLive, "TalkTimer deregistered on every exit", k);
    check(liveThings == 0, "every CScriptThing (predicate copy, door, script-thing temporaries) destroyed", k);
    check(baseLive == 0 && movieLive == 0, "every CBaseObject part and movie destroyed", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(meInfo.RefCount == REF0 && doorInfo.RefCount == REF0, "Me / door reference counts balanced", k);
    check(hMeInfo.RefCount == REF0, "handle references balanced", k);
    if (strstr(got, "SPEAK:Hero/TEXT_QST_055_PRISON_PRISONER_KEY")) ++cov[0];
    if (strstr(got, "HINT2")) ++cov[1];
    if (strstr(got, "HINT3")) ++cov[2];
    if (strstr(got, "HINT4_02")) ++cov[3];
    if (strstr(got, "MORALITY")) ++cov[4];
    if (strstr(got, "COMMENT:COURTYARD=1")) ++cov[5];
    if (strstr(got, "COMMENT:GUARDS=1")) ++cov[6];
    if (strstr(got, "ON_HIT")) ++cov[7];
    if (strstr(got, "REGIONFOLLOW:Hero-Me/0")) ++cov[8];
    if (strstr(got, "BUSY1 N T PAUSE0")) ++cov[9];
    if (strstr(got, "s4")) ++cov[10];
    if (strstr(got, "ABIL14:SCRIPT_NAME_HERO=1")) ++cov[11];
    if (strstr(got, "HEALTH:MeCopy=0") || strstr(got, "HEALTH:MeCopy=-1")) ++cov[12];
    if (strstr(got, "PAUSE1") && strstr(got, "TALK:SCRIPT_NAME_HERO=0 USED")) ++cov[13];
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int i = 0; i < 80; ++i) thingVtbl[i] = (void*)VirtualTrap;
    thingVtbl[21] = (void*)T_HitBy; thingVtbl[27] = (void*)T_TalkedTo; thingVtbl[41] = (void*)T_AbilityFrom; thingVtbl[42] = (void*)T_AnyAbilityFrom;
    for (int i = 0; i < 40; ++i) doorImplVtbl[i] = (void*)VirtualTrap;
    doorImplVtbl[20] = (void*)D_UsedBy; doorImpl.vtbl = doorImplVtbl; doorImpl.name = "door";
    meImpl.name = "me"; heroImpl.name = "hero";
    heroThing.vtbl = thingVtbl; heroThing.data = &heroImpl; heroThing.info = 0;
    g_pGlobalScriptDef = def.b; float morality = 2.5f; memcpy(def.b + 0xde8, &morality, 4);
    for (int i = 0; i < 100; ++i) dstVtbl[i] = (void*)VirtualTrap;
    dstVtbl[87] = (void*)D_Register; dstVtbl[88] = (void*)D_Deregister; dstVtbl[89] = (void*)D_Set; dstVtbl[90] = (void*)D_Get;
    dstObj[0] = dstVtbl; g_pDSTGame = dstObj;
    for (int i = 0; i < 900; ++i) gameVtbl[i] = (void*)VirtualTrap;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScripting; gameVtbl[12] = (void*)G_RegionLoaded; gameVtbl[70] = (void*)G_GetHero;
    gameVtbl[77] = (void*)G_Nearest; gameVtbl[156] = (void*)G_Morality; gameVtbl[229] = (void*)G_FollowThing; gameVtbl[235] = (void*)G_AllowFollow;
    gameVtbl[264] = (void*)G_GetHealth; gameVtbl[361] = (void*)G_ClearInfo; gameVtbl[364] = (void*)G_NewConv; gameVtbl[365] = (void*)G_AddPerson;
    gameVtbl[366] = (void*)G_AddLine; gameVtbl[370] = (void*)G_StartMovie; gameVtbl[379] = (void*)G_Pause; gameVtbl[477] = (void*)G_Persistent;
    gameVtbl[480] = (void*)G_RegionFollow; gameVtbl[601] = (void*)G_Faction;
    static float health[96]; int k = 0;
    static const char zeros[] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    // targeted: talk at 3 and 9, cutscene flag off at 6..7, door used at 11, region at 16, comments yes; a terminate at each of the first 110 tests
    for (int variant = 0; variant < 7; ++variant) {
        World w; memset(&w, 0, sizeof w);
        for (int f = 0; f < FRAMES; ++f) w.pcf[f] = f >= 2 && !(f == 6 || f == 7);
        w.talk[3] = 1; w.talk[9] = 1; w.used[11] = 1; w.used[5] = variant == 6; w.hit[17] = 1; w.ability[19] = 1; w.heal[20] = 1; w.ability[20] = 1;
        w.hit[24] = 1; w.regionAt = 16; w.speakFrames = variant == 3 ? 0 : 2; w.timer0 = variant & 1;
        w.key = variant == 0; w.won = variant == 1; w.raceNumber = variant == 2 ? 2 : 1; w.doorNull = variant == 5;
        for (int i = 0; i < 96; ++i) health[i] = (variant == 4 && i == 0) ? 0.0f : 10.0f;
        char ts[128];
        for (int t = 0; t < 110; ++t) { memcpy(ts, zeros, 120); ts[t] = '1'; ts[120] = 0; Run(w, ts, (t & 1) ? "1" : "0010", "1011", health, ++k); }
    }
    unsigned long seed = 29;
    for (int r = 0; r < 6000; ++r) {
#define RND(n) (seed = seed * 1103515245ul + 12345ul, (int)((seed >> 16) % (n)))
        World w; memset(&w, 0, sizeof w);
        int pcfAt = RND(4);
        for (int f = 0; f < FRAMES; ++f) { w.pcf[f] = f >= pcfAt && RND(8) != 0; w.talk[f] = RND(6) == 0; w.used[f] = RND(10) == 0;
            w.hit[f] = RND(9) == 0; w.ability[f] = RND(7) == 0; w.heal[f] = RND(2); }
        w.regionAt = RND(40); w.speakFrames = RND(4); w.raceNumber = RND(4); w.timer0 = RND(3);
        w.key = RND(4) == 0; w.won = RND(3) == 0; w.raf0 = RND(10) == 0; w.doorNull = RND(15) == 0;
        for (int i = 0; i < 96; ++i) health[i] = RND(4) ? 5.0f : (RND(2) ? 0.0f : -1.0f);
        static char ts[400], ss[16], cs[64];
        for (int i = 0; i < 399; ++i) ts[i] = RND(70) == 0 ? '1' : '0';
        ts[399] = 0;
        for (int i = 0; i < 15; ++i) ss[i] = RND(3) == 0 ? '0' : '1';
        ss[15] = 0;
        for (int i = 0; i < 63; ++i) cs[i] = RND(2) ? '1' : '0';
        cs[63] = 0;
        Run(w, ts, ss, cs, health, 1000 + r);
    }
    for (int i = 0; i < 14; ++i) printf("cov%d %d\n", i, cov[i]);
    if (failures) { printf("00dd4ad0_TEST FAIL %d\n", failures); return 1; }
    printf("00dd4ad0_TEST PASS (%d targeted, 6000 random)\n", k);
    return 0;
}