// Behaviour test for 0x00D0B3E0 NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main (links the REAL row object: the row body,
// the CIsThingAlive ctor / dtor compiled in their inline_depth(0) region with the TU's implicit CScriptThing copy COMDAT, the expanded
// by-value CScriptThing copy for SetIsPushableByHero, the TU's CTimer members (ctor COMDAT, expanded and called dtor, forced operator= and
// GetTime) and the handle's implicit special members all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the entity (0x1C bytes: Game +4, Me +8, ParentClass +0x14); the
// parent (0x70 bytes: ItsAllOver +0x50, BanditKingFightStarted +0x52, BanditKingFightEnded +0x53, KingHealth +0x58, TwinBladeKilled +0x64,
// AngryBanditNeeded +0x68, BanditsNeededForCutscene +0x6c); Me with a counted pointer-info record (both copies' reference counts are checked)
// and fake vtable slots (GetDataString 3, MsgIsHitBy 21, MsgIsHitBySpecialAbilityFrom 41, MsgIsHitByAnySpecialAbilityFrom 42); handles
// with counted implementation records; pDSTGame's timer slots 87..90 (a ticking timer: every NewScriptFrame counts it down); Game's vtable
// slots 7, 8, 70, 72, 81, 108, 364, 365, 366, 451, 458, 460, 469, 475, 492, 512, 516, 529, 690, 844; a scripted rand().
// A scripted world changes the parent's flags, KingHealth, AngryBanditNeeded, the three distances, Maze's speech task and the hit messages
// at every NewScriptFrame; termination and StartScriptingEntity results are scripted too.
// Reference model: Model() below, written from the quest's statements; every run is compared event by event, then the parent's counter,
// the timer's final state, and the balance of strings, things, CBaseObject parts and reference counts.
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
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UnsheatheWeapons@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UpdateThingAttachment@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@StubBaseCtor@8")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@StubThingDtor@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@StubIsValid@8")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@StubRelease@8")
#pragma comment(linker, "/alternatename:?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z=@StubPlayAnimation@40")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=@StubClearCommands@8")
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=@StubBusy@8")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@StubUnder@12")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsOver@@YI_NABVCScriptThing@@0M@Z=@StubOver@12")
#pragma comment(linker, "/alternatename:?GFCharStringToInt@@YIJABVCCharString@@@Z=@StubToInt@4")
#pragma comment(linker, "/alternatename:?pDSTGame@@3PAVCGameScriptInterfaceBase@@A=_g_pDSTGame")
#pragma comment(linker, "/alternatename:?GDefaultToUsingPhysicsInCutsceneAnimations@NGlobalConsole@@3_NA=_g_Physics")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@StubSetPredicate@12")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@StubTerminating@8")
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ")
extern "C" void __fastcall CallMain(void* self);

// ---- strings ---------------------------------------------------------------------------------------------------------------------
static int liveStrings;
class CCharString { public:
    CCharString(const char* text, long length); ~CCharString();
    char* Text; };
static char* Dup(const char* s) { size_t n = strlen(s); char* p = (char*)malloc(n + 1); memcpy(p, s, n + 1); return p; }
CCharString::CCharString(const char* text, long length) { Text = Dup(text); ++liveStrings; if (length != -1) { printf("FAIL string length\n"); exit(4); } }
CCharString::~CCharString() { free(Text); Text = 0; --liveStrings; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
static void Bad(const char* what) { ++failures; if (failures < 40) printf("FAIL %s\n", what); }

// ---- event logs ------------------------------------------------------------------------------------------------------------------
enum { MAXLOG = 120000 };
static char got[MAXLOG + 64]; static int ngot;
static char want[MAXLOG + 64]; static int nwant;
static void Put(char* buf, int& n, const char* s) { size_t m = strlen(s); if (n + (int)m + 1 < MAXLOG) { memcpy(buf + n, s, m); n += (int)m; buf[n++] = ' '; buf[n] = 0; } }
static void Ev(const char* s) { Put(got, ngot, s); }
static void Wv(const char* s) { Put(want, nwant, s); }
static const char* termScript; static const char* startScript;
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

// ---- world -----------------------------------------------------------------------------------------------------------------------
enum { FRAMES = 64 };
struct World {
    int startedAt, endedAt, allOverAt, twinAt;
    unsigned char needed[FRAMES], under[FRAMES], over[FRAMES], kingFar[FRAMES], busy[FRAMES], hit[FRAMES], ability[FRAMES], heal[FRAMES];
    long health[FRAMES], angry[FRAMES];
    bool remove;
};
static World world;
static int F(int f) { return f < FRAMES ? f : FRAMES - 1; }
struct State { int frame, tpos, spos, rpos; bool valid; long timer; bool timerLive; long angryDec; };
static State gs, ms;
static const int* randScript; enum { NRAND = 400 };

static union { unsigned char b[0x1C]; double align; } ent;
static union { unsigned char b[0x70]; double align; } parent;
static void SetParent(int f)
{
    f = F(f); parent.b[0x50] = f >= world.allOverAt; parent.b[0x52] = f >= world.startedAt; parent.b[0x53] = f >= world.endedAt; parent.b[0x64] = f >= world.twinAt;
    parent.b[0x6c] = world.needed[f]; memcpy(parent.b + 0x58, &world.health[f], 4);
    long a = world.angry[f] - gs.angryDec; memcpy(parent.b + 0x68, &a, 4);
}
extern "C" { void* g_pDSTGame; bool g_Physics = true; }

struct Info { unsigned long RefCount; void* DeleteFunc; void* Data; };
struct Impl { const char* name; };
struct Thing { void** vtbl; Impl* data; Info* info; };
static void* thingVtbl[80];
static Impl meImpl = { "me" }, heroImpl = { "hero" }, villageImpl = { "village" }, kingImpl = { "king" };
static Info meInfo;
static Thing heroThing;
static Thing* Me() { return (Thing*)(ent.b + 8); }
static int liveThings, baseLive;
static const char* Who(const void* t)
{ const Thing* th = (const Thing*)t; return t == Me() ? "Me" : t == &heroThing ? "Hero" : th->data ? th->data->name : "null"; }

struct Handle { void* vtbl; void* ip; void* data; Info* info; };
static Info hMeInfo; static int meStbImpl;
enum { REF0 = 1000 };
static void* sehMeAddr;

extern "C" {
void __fastcall StubBaseCtor(void* self, void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseDtor(void*, void*) { --baseLive; }
void __fastcall StubThingDtor(void* self, void*) { Thing* t = (Thing*)self; if (t->info) --t->info->RefCount; if (t->data != &meImpl) --liveThings; memset(self, 0xDD, 12); }
bool __fastcall StubIsValid(const void* self, void*) { return ((const Handle*)self)->data != 0; }
void __fastcall StubRelease(void* self, void*)
{ Handle* h = (Handle*)self; if (self != sehMeAddr) Bad("Release receiver"); if (h->info) --h->info->RefCount; h->data = 0; h->info = 0; Ev("R"); }
static void CheckMe(const void* self) { if (self != sehMeAddr || ((const Handle*)self)->data != &meStbImpl) Bad("seh_me receiver"); }
void __fastcall StubPlayAnimation(void* self, void*, const CCharString* a, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7)
{ CheckMe(self); char s[96]; sprintf(s, "ANIM:%s/%d%d%d%d%d%d%d", a->Text, b1, b2, b3, b4, b5, b6, b7); Ev(s); }
void __fastcall StubClearCommands(void* self, void*) { CheckMe(self); Ev("CLEAR"); }
bool __fastcall StubBusy(const void* self, void*) { CheckMe(self); bool r = world.busy[F(gs.frame)] != 0; Ev(r ? "BUSY1" : "BUSY0"); return r; }
bool __fastcall StubUnder(const void* a, const void* b, float d)
{ bool r = world.under[F(gs.frame)] != 0; char s[64]; sprintf(s, "U:%s-%s<%g=%d", Who(a), Who(b), d, r); Ev(s); return r; }
bool __fastcall StubOver(const void* a, const void* b, float d)
{
    bool r = (a == &heroThing ? world.kingFar : world.over)[F(gs.frame)] != 0;
    char s[64]; sprintf(s, "O:%s-%s>%g=%d", Who(a), Who(b), d, r); Ev(s); return r;
}
long __fastcall StubToInt(const CCharString* s) { char b[48]; sprintf(b, "TOINT:%s", s->Text); Ev(b); return atol(s->Text); }
void __fastcall StubSetPredicate(void* self, void*, const void* pred)
{
    if (self != ent.b) Bad("SetPredicate receiver");
    const Thing* t = (const Thing*)((const char*)pred + 4);
    if (!*(void* const*)pred || t->data != &meImpl || t->info != &meInfo || meInfo.RefCount != REF0 + 1) Bad("predicate: a counted copy of Me");
    Ev("P");
}
bool __fastcall StubTerminating(void* self, void*)
{ if (self != ent.b) Bad("terminate receiver"); bool t = NextBit(termScript, gs.tpos, true); Ev(t ? "T" : "t"); return t; }
int __cdecl rand(void) { int r = randScript[gs.rpos++ % NRAND]; char s[16]; sprintf(s, "RND%d", r); Ev(s); return r; }
}

// Me's virtuals
static CCharString* __fastcall T_DataString(const void* self, void*, CCharString* ret)
{ if (self != Me()) Bad("GetDataString receiver"); new (ret) CCharString(world.remove ? "1" : "0", -1); return ret; }
static bool __fastcall T_HitBy(const void* self, void*, const CCharString* w)
{ bool r = world.hit[F(gs.frame)] != 0; char s[64]; sprintf(s, "HIT:%s/%s=%d", Who(self), w->Text, r); Ev(s); return r; }
static bool __fastcall T_AbilityFrom(const void* self, void*, int ab, const CCharString* w)
{ bool r = world.heal[F(gs.frame)] != 0; char s[64]; sprintf(s, "ABIL%d:%s/%s=%d", ab, Who(self), w->Text, r); Ev(s); return r; }
static bool __fastcall T_AnyAbilityFrom(const void* self, void*, const CCharString* w)
{ bool r = world.ability[F(gs.frame)] != 0; char s[64]; sprintf(s, "ANYABIL:%s/%s=%d", Who(self), w->Text, r); Ev(s); return r; }

// ---- pDSTGame timers -------------------------------------------------------------------------------------------------------------
static void* dstVtbl[100]; static void* dstObj[1];
static long __fastcall D_Register(void* self, void*) { if (self != dstObj) Bad("timer receiver"); if (gs.timerLive) Bad("second timer"); gs.timerLive = true; gs.timer = 99; Ev("TIMER"); return 5; }
static void __fastcall D_Deregister(void* self, void*, long i) { if (self != dstObj || i != 5) Bad("deregister"); gs.timerLive = false; Ev("DEREG"); }
static void __fastcall D_Set(void* self, void*, long i, long v) { if (self != dstObj || i != 5) Bad("set timer"); gs.timer = v; char s[24]; sprintf(s, "SET:%ld", v); Ev(s); }
static long __fastcall D_Get(const void* self, void*, long i) { if (self != dstObj || i != 5) Bad("get timer"); char s[24]; sprintf(s, "GT=%ld", gs.timer); Ev(s); return gs.timer; }

// ---- game interface ----------------------------------------------------------------------------------------------------------------
static void* gameVtbl[900]; static void* gameObj[1]; static bool badGame;
static void CheckGame(const void* self) { if (self != (void*)gameObj) badGame = true; }
static void AbsorbAngry()   // carry the row's own decrements of AngryBanditNeeded across the world's refresh
{ long cur; memcpy(&cur, parent.b + 0x68, 4); gs.angryDec += (world.angry[F(gs.frame)] - gs.angryDec) - cur; }
static void __fastcall G_NewScriptFrame(void* self, void*) { CheckGame(self); Ev("N"); AbsorbAngry(); ++gs.frame; if (gs.timer > 0) --gs.timer; SetParent(gs.frame); }
static bool __fastcall G_StartScripting(void* self, void*, const void* who, void* handle, int prio)
{
    CheckGame(self); if (who != Me() || prio != 4) Bad("StartScriptingEntity arguments");
    if (!sehMeAddr) sehMeAddr = handle; else if (handle != sehMeAddr) Bad("StartScriptingEntity handle");
    bool ok = NextBit(startScript, gs.spos, true);
    if (ok) { Handle* h = (Handle*)handle; ++hMeInfo.RefCount; if (h->info) --h->info->RefCount; h->data = &meStbImpl; h->info = &hMeInfo; }
    Ev(ok ? "S" : "s"); return ok;
}
static void __fastcall G_RemoveThing(void* self, void*, const void* t, bool a, bool b) { CheckGame(self); char s[32]; sprintf(s, "REMOVE:%s/%d%d", Who(t), a, b); Ev(s); }
static Thing* __fastcall G_GetThing(void* self, void*, Thing* ret, const CCharString* name)
{ CheckGame(self); if (strcmp(name->Text, "BanditKing")) Bad("thing name"); ret->vtbl = thingVtbl; ret->data = &kingImpl; ret->info = 0; ++liveThings; Ev("G:BanditKing"); return ret; }
static Thing* __fastcall G_Nearest(void* self, void*, Thing* ret, const void* t, const CCharString* def)
{ CheckGame(self); ret->vtbl = thingVtbl; ret->data = &villageImpl; ret->info = 0; ++liveThings; char s[64]; sprintf(s, "NEAREST:%s/%s", Who(t), def->Text); Ev(s); return ret; }
static Thing* __fastcall G_GetHero(void* self, void*) { CheckGame(self); return &heroThing; }
static long convIndex;
static long __fastcall G_NewConv(void* self, void*, const void* t, bool a, bool b) { CheckGame(self); char s[48]; sprintf(s, "CONV:%s/%d%d", Who(t), a, b); Ev(s); return ++convIndex; }
static void __fastcall G_AddPerson(void* self, void*, long i, const void* t) { CheckGame(self); char s[48]; sprintf(s, "PERSON:%ld/%s", i, Who(t)); Ev(s); }
static void __fastcall G_AddLine(void* self, void*, long i, const CCharString* l, bool b, const void* a, const void* c)
{ CheckGame(self); char s[96]; sprintf(s, "LINE:%ld/%s/%d/%s/%s", i, l->Text, b, Who(a), Who(c)); Ev(s); }
#define G_BOOL(fn, tag) static void __fastcall fn(void* self, void*, const void* t, bool b) { CheckGame(self); char s[48]; sprintf(s, tag ":%s/%d", Who(t), b); Ev(s); }
G_BOOL(G_Targetable, "TARGETABLE") G_BOOL(G_Damageable, "DAMAGEABLE") G_BOOL(G_Engage, "ENGAGE") G_BOOL(G_MoveAct, "MOVEACT")
static void __fastcall G_Attach(void* self, void*, const void* t, const void* v) { CheckGame(self); char s[48]; sprintf(s, "ATTACH:%s/%s", Who(t), Who(v)); Ev(s); }
static void __fastcall G_CutBeh(void* self, void*, const void* t, int b) { CheckGame(self); char s[48]; sprintf(s, "CUTBEH:%s/%d", Who(t), b); Ev(s); }
static void __fastcall G_Attack(void* self, void*, const void* t, const void* o, bool a, bool b) { CheckGame(self); char s[48]; sprintf(s, "ATTACK:%s-%s/%d%d", Who(t), Who(o), a, b); Ev(s); }
static void __fastcall G_Facing(void* self, void*, const void* t, const void* o, bool b) { CheckGame(self); char s[48]; sprintf(s, "FACE:%s-%s/%d", Who(t), Who(o), b); Ev(s); }
static unsigned long __fastcall G_Sound(void* self, void*, const void* t, const CCharString* n) { CheckGame(self); char s[64]; sprintf(s, "SND:%s/%s", Who(t), n->Text); Ev(s); return 1; }
static void __fastcall G_GiveEnemy(void* self, void*, const void* t, const void* o) { CheckGame(self); char s[48]; sprintf(s, "ENEMY:%s-%s", Who(t), Who(o)); Ev(s); }
static void __fastcall G_Pushable(void* self, void*, Thing t, bool b)
{
    CheckGame(self); char s[64];
    bool copyOk = t.data == &meImpl && t.info == &meInfo && meInfo.RefCount == REF0 + 1 && t.vtbl != 0;
    sprintf(s, "PUSHABLE:%s/%d", copyOk ? "MeCopy" : "?", b); if (t.info) --t.info->RefCount; Ev(s);
}

// ---- reference model -------------------------------------------------------------------------------------------------------------
static bool MT() { bool t = NextBit(termScript, ms.tpos, true); Wv(t ? "T" : "t"); if (t && ms.timerLive) Wv("DEREG"); return t; }
static void MF() { Wv("N"); ++ms.frame; if (ms.timer > 0) --ms.timer; }
static bool MStart() { bool ok = NextBit(startScript, ms.spos, true); if (ok) ms.valid = true; Wv(ok ? "S" : "s"); return ok; }
static void MRelease() { if (ms.valid) { Wv("R"); ms.valid = false; } }
static int MRand() { int r = randScript[ms.rpos++ % NRAND]; char s[16]; sprintf(s, "RND%d", r); Wv(s); return r; }
static long MGet() { char s[24]; sprintf(s, "GT=%ld", ms.timer); Wv(s); return ms.timer; }
static void MSet(long v) { ms.timer = v; char s[24]; sprintf(s, "SET:%ld", v); Wv(s); }
static void MAnim(const char* n) { char s[64]; sprintf(s, "ANIM:%s/0001100", n); Wv(s); }
static void MSnd(const char* n) { char s[64]; sprintf(s, "SND:Me/%s", n); Wv(s); }
static long Angry() { return world.angry[F(ms.frame)] - ms.angryDec; }
static bool MCutWait()   // false: terminated
{
    if (!world.needed[F(ms.frame)]) return true;
    if (MT()) return false;
    Wv("MOVEACT:Me/1"); MRelease();
    while (world.needed[F(ms.frame)]) { MF(); if (MT()) return false; }
    if (MT()) return false;
    MRelease();
    while (!MStart()) { MF(); if (MT()) return false; }
    if (MT()) return false;
    Wv("MOVEACT:Me/0");
    return true;
}
static bool MBoos() { if (MRand() % 4 == 0) { if (MT()) return false; MSnd(MRand() % 2 == 0 ? "SND_CROWDBOASTBOOS_01" : "SND_CROWDBOASTBOOS_02"); } return true; }
static void Model()
{
    char s[96];
    Wv("P"); MF(); if (MT()) return;
    Wv("TARGETABLE:Me/0"); Wv("DAMAGEABLE:Me/0"); Wv("PUSHABLE:MeCopy/0"); Wv("ENGAGE:Me/0");
    while (!(F(ms.frame) >= world.startedAt)) { MF(); if (MT()) return; }
    if (MT()) return;
    Wv("MOVEACT:Me/0"); Wv("NEAREST:Me/VILLAGE_BANDIT_CAMP_BOSS"); Wv("ATTACH:Me/village");
    MRelease();
    while (!MStart()) { MF(); if (MT()) return; }
    if (MT()) return;
    Wv("CLEAR");
    long last = world.health[F(ms.frame)];
    Wv("G:BanditKing"); Wv("TIMER"); ms.timerLive = true; ms.timer = 99; MSet(0);
    bool paused = false;
    for (;; MF()) {
        if (MT()) return;
        if (!MCutWait()) return;
        if (!paused && F(ms.frame) >= world.allOverAt) { if (MT()) return; Wv("CUTBEH:Me/1"); paused = true; }
        bool u = world.under[F(ms.frame)] != 0; sprintf(s, "U:Me-Hero<3.5=%d", u); Wv(s);
        if (u) { if (MT()) return; Wv("ATTACK:Me-Hero/01"); continue; }
        bool o = world.over[F(ms.frame)] != 0; sprintf(s, "O:Me-Hero>6.5=%d", o); Wv(s);
        if (!o || MGet() > 0) continue;
        if (MT()) return;
        bool b = world.busy[F(ms.frame)] != 0; Wv(b ? "BUSY1" : "BUSY0");
        if (!b) {
            if (MT()) return;
            long h = world.health[F(ms.frame)];
            if (h < last) {
                if (MT()) return;
                last = h; Wv("FACE:Me-Hero/1");
                if (MRand() % 2 == 0) {
                    if (MT()) return;
                    int a = MRand() % 3; MAnim(a == 0 ? "SPECIAL_BOAST" : a == 1 ? "ST_THREATEN" : "SCRIPT_SHOUT");
                    if (!MBoos()) return;
                } else { if (MT()) return; MSet(2); }
            } else {
                bool far = world.kingFar[F(ms.frame)] != 0; sprintf(s, "O:Hero-king>8=%d", far); Wv(s);
                if (far) {
                    if (MT()) return;
                    Wv("FACE:Me-Hero/1");
                    if (MRand() % 2 == 0) {
                        if (MT()) return;
                        MAnim(MRand() % 2 == 0 ? "SPECIAL_BOAST" : "ST_WAVE_SPECIAL_01");
                        if (MRand() % 8 == 0) {
                            if (MT()) return;
                            Wv("CONV:Me/00"); long i = ++convIndex; sprintf(s, "PERSON:%ld/Hero", i); Wv(s);
                            sprintf(s, "LINE:%ld/TEXT_QST_009_BOSSFIGHT_TAUNTING/0/Me/Hero", i); Wv(s);
                        }
                        if (!MBoos()) return;
                    } else { if (MT()) return; MSet(2); }
                } else {
                    if (MT()) return;
                    Wv("FACE:Me-Hero/1");
                    if (MRand() % 2 == 0) {
                        if (MT()) return;
                        MAnim(MRand() % 2 == 0 ? "COCKY" : "ST_UNPERTURBED_GESTURE");
                        MSnd(MRand() % 2 == 0 ? "SND_CROWDLARGEBOASTREACT_01" : "SND_CROWDLARGEBOASTREACT_02");
                    } else { if (MT()) return; MSet(2); }
                }
            }
        }
        if (F(ms.frame) >= world.endedAt) break;
    }
    if (MT()) return;
    sprintf(s, "TOINT:%s", world.remove ? "1" : "0"); Wv(s);
    if (world.remove) { if (MT()) return; Wv("REMOVE:Me/01"); }
    Wv("TARGETABLE:Me/1"); Wv("DAMAGEABLE:Me/1"); Wv("MOVEACT:Me/1");
    if (MT()) return;
    for (;;) {
        if (!MCutWait()) return;
        if (!(F(ms.frame) >= world.twinAt)) {
            if (MT()) return;
            if (MGet() <= 0) {
                if (MT()) return;
                if (MRand() % 2 == 0) {
                    if (MT()) return;
                    int a = MRand() % 3; MAnim(a == 0 ? "SPECIAL_BOAST" : a == 1 ? "ST_THREATEN" : "SCRIPT_SHOUT");
                    if (!MBoos()) return;
                } else { if (MT()) return; MSet(2); }
            }
        }
        bool brk = Angry() > 0;
        if (!brk) {
            int f = F(ms.frame); bool hit = world.hit[f] != 0; sprintf(s, "HIT:Me/SCRIPT_NAME_HERO=%d", hit); Wv(s); brk = hit;
            if (!brk) {
                bool any = world.ability[f] != 0; sprintf(s, "ANYABIL:Me/SCRIPT_NAME_HERO=%d", any); Wv(s);
                if (any) { bool heal = world.heal[f] != 0; sprintf(s, "ABIL14:Me/SCRIPT_NAME_HERO=%d", heal); Wv(s); brk = !heal; }
            }
        }
        if (brk) break;
        MF(); if (MT()) return;
    }
    if (MT()) return;
    Wv("ENEMY:Me-Hero"); Wv("ENGAGE:Me/1"); MRelease();
    if (Angry() != 0) { if (MT()) return; ++ms.angryDec; }
    for (;;) { MF(); if (MT()) return; }
}

static void Run(const World& w, const char* term, const char* start, const int* rnd, int k)
{
    world = w; termScript = term; startScript = start; randScript = rnd;
    memset(&gs, 0, sizeof gs); memset(&ms, 0, sizeof ms); ngot = nwant = 0; got[0] = want[0] = 0;
    memset(parent.b, 0, sizeof parent.b); SetParent(0);
    memset(ent.b, 0x11, sizeof ent.b); void* g = gameObj; memcpy(ent.b + 4, &g, 4); void* pp = parent.b; memcpy(ent.b + 0x14, &pp, 4);
    Thing* me = Me(); me->vtbl = thingVtbl; me->data = &meImpl; me->info = &meInfo;
    meInfo.RefCount = REF0; hMeInfo.RefCount = REF0;
    liveThings = baseLive = liveStrings = 0; badGame = false; sehMeAddr = 0;
    convIndex = 0; Model(); convIndex = 0;
    CallMain(ent.b);
    if (strcmp(got, want)) {
        ++failures;
        if (failures < 6) {
            int i = 0; while (got[i] && got[i] == want[i]) ++i;
            int a = i > 150 ? i - 150 : 0;
            printf("FAIL case %d events differ at %d\n  got  ...%.240s\n  want ...%.240s\n", k, i, got + a, want + a);
        }
        return;
    }
    long angry; memcpy(&angry, parent.b + 0x68, 4);
    check(!badGame, "Game receiver", k);
    AbsorbAngry();
    check(gs.angryDec == ms.angryDec && angry == world.angry[F(gs.frame)] - gs.angryDec, "AngryBanditNeeded decremented once when needed", k);
    check(!gs.timerLive, "ActionTimer deregistered on every exit", k);
    check(liveThings == 0, "every CScriptThing temporary destroyed (village, King)", k);
    check(baseLive == 0, "every CBaseObject part destroyed", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(meInfo.RefCount == REF0, "Me's reference count balanced (predicate copy, by-value copy)", k);
    check(hMeInfo.RefCount == REF0, "handle references balanced", k);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int i = 0; i < 80; ++i) thingVtbl[i] = (void*)VirtualTrap;
    thingVtbl[3] = (void*)T_DataString; thingVtbl[21] = (void*)T_HitBy; thingVtbl[41] = (void*)T_AbilityFrom; thingVtbl[42] = (void*)T_AnyAbilityFrom;
    heroThing.vtbl = thingVtbl; heroThing.data = &heroImpl; heroThing.info = 0;
    for (int i = 0; i < 100; ++i) dstVtbl[i] = (void*)VirtualTrap;
    dstVtbl[87] = (void*)D_Register; dstVtbl[88] = (void*)D_Deregister; dstVtbl[89] = (void*)D_Set; dstVtbl[90] = (void*)D_Get;
    dstObj[0] = dstVtbl; g_pDSTGame = dstObj;
    for (int i = 0; i < 900; ++i) gameVtbl[i] = (void*)VirtualTrap;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScripting; gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThing;
    gameVtbl[81] = (void*)G_Nearest; gameVtbl[108] = (void*)G_RemoveThing; gameVtbl[364] = (void*)G_NewConv; gameVtbl[365] = (void*)G_AddPerson;
    gameVtbl[366] = (void*)G_AddLine; gameVtbl[451] = (void*)G_GiveEnemy; gameVtbl[458] = (void*)G_Engage; gameVtbl[460] = (void*)G_Attack;
    gameVtbl[469] = (void*)G_Targetable; gameVtbl[475] = (void*)G_Facing; gameVtbl[492] = (void*)G_Attach; gameVtbl[512] = (void*)G_CutBeh;
    gameVtbl[516] = (void*)G_Damageable; gameVtbl[529] = (void*)G_MoveAct; gameVtbl[690] = (void*)G_Sound; gameVtbl[844] = (void*)G_Pushable;
    static int rnd[NRAND]; unsigned long seed = 77;
#define RND(n) (seed = seed * 1103515245ul + 12345ul, (int)((seed >> 16) % (n)))
    for (int i = 0; i < NRAND; ++i) rnd[i] = RND(32768);
    int k = 0;
    static const char zeros[] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    for (int variant = 0; variant < 6; ++variant) {
        World w; memset(&w, 0, sizeof w);
        w.startedAt = 2; w.endedAt = 20; w.allOverAt = variant == 1 ? 5 : 99; w.twinAt = variant == 2 ? 26 : 99; w.remove = variant == 3;
        for (int f = 0; f < FRAMES; ++f) {
            w.needed[f] = (f == 7 || f == 8 || f == 24 || f == 25); w.under[f] = (f % 5) == 0; w.over[f] = (f % 3) != 0; w.kingFar[f] = (f % 4) < 2;
            w.busy[f] = (f % 7) == 3; w.health[f] = 100 - 5 * (f / 3); w.angry[f] = (variant != 4 && f >= 30) ? 2 : 0;
            w.hit[f] = variant == 4 && f == 33; w.ability[f] = variant == 5 && f >= 31; w.heal[f] = variant == 5 && f == 31;
        }
        char ts[128];
        for (int t = 0; t < 110; ++t) { memcpy(ts, zeros, 120); ts[t] = '1'; ts[120] = 0; Run(w, ts, (t & 1) ? "1" : "0010", rnd, ++k); }
    }
    for (int r = 0; r < 6000; ++r) {
        World w; memset(&w, 0, sizeof w);
        w.startedAt = RND(6); w.endedAt = w.startedAt + RND(30); w.allOverAt = RND(40); w.twinAt = RND(60); w.remove = RND(3) == 0;
        int angryAt = RND(64);
        for (int f = 0; f < FRAMES; ++f) {
            w.needed[f] = RND(10) == 0; w.under[f] = RND(4) == 0; w.over[f] = RND(3) != 0; w.kingFar[f] = RND(2); w.busy[f] = RND(4) == 0;
            w.health[f] = f ? w.health[f - 1] - (RND(3) == 0 ? 7 : 0) : 100; w.angry[f] = f >= angryAt ? RND(3) : 0;
            w.hit[f] = RND(15) == 0; w.ability[f] = RND(12) == 0; w.heal[f] = RND(2);
        }
        for (int i = 0; i < NRAND; ++i) rnd[i] = RND(32768);
        static char ts[400], ss[24];
        for (int i = 0; i < 399; ++i) ts[i] = RND(90) == 0 ? '1' : '0';
        ts[399] = 0;
        for (int i = 0; i < 23; ++i) ss[i] = RND(3) == 0 ? '0' : '1';
        ss[23] = 0;
        Run(w, ts, ss, rnd, 1000 + r);
    }
    if (failures) { printf("00d0b3e0_TEST FAIL %d\n", failures); return 1; }
    printf("00d0b3e0_TEST PASS (%d targeted, 6000 random)\n", k);
    return 0;
}