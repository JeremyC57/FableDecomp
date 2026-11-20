// Behaviour test for 0x00E17840 NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main (links the REAL row object, built with its
// row flag /d2inlS13: the row body, the CIsThingAlive ctor compiled in its inline_depth(0) region with the TU's implicit CScriptThing copy
// COMDAT, the row-local inline ~CIsThingAlive, the expanded by-value CScriptThing copy for SetIsPushableByHero, the TU's implicit handle /
// movie special members and the expanded CWideScreenMagicPauseEntities all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the entity (0x1c bytes: Game +4, Me +8, ParentClass +0x14);
// the parent (BalverineState +0x50); Me with a counted pointer-info record and a fake vtable (MsgIsHitBy 21, MsgIsTalkedToBy 27,
// MsgIsHitBySpecialAbilityFrom 41, MsgIsHitByAnySpecialAbilityFrom 42, SetFriendsWithEverythingFlag 67); handles with counted
// implementation records; Game's vtable slots 7, 8, 70, 264, 284, 360, 370, 379, 517, 526, 527, 844. A scripted world changes
// BalverineState, the talk / hit / ability / heal messages and the speech task at every NewScriptFrame; termination, StartScriptingEntity,
// IsQuestActive and the health check are scripted too.
// Reference model: Model() below, written from the quest's statements; every run is compared event by event, then the balance of
// strings, things, CBaseObject parts, movies and reference counts.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); fflush(stdout); exit(3); }
#pragma comment(linker, "/alternatename:??_GCBaseClass@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@StubThingDtor@8")
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
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@StubRelease@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@StubIsValid@8")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@StubTerminating@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@StubSetPredicate@12")
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CKG_Chief@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ")
extern "C" void __fastcall CallMain(void* self);

static int liveStrings;
class CCharString { public: CCharString(const char* text, long length); ~CCharString(); char* Text; };
static char* Dup(const char* s) { size_t n = strlen(s); char* p = (char*)malloc(n + 1); memcpy(p, s, n + 1); return p; }
CCharString::CCharString(const char* text, long length) { Text = Dup(text); ++liveStrings; if (length != -1) { printf("FAIL string length\n"); exit(4); } }
CCharString::~CCharString() { free(Text); Text = 0; --liveStrings; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
static void Bad(const char* what) { ++failures; if (failures < 40) printf("FAIL %s\n", what); }

enum { MAXLOG = 60000 };
static char got[MAXLOG + 64]; static int ngot;
static char want[MAXLOG + 64]; static int nwant;
static void Put(char* buf, int& n, const char* s) { size_t m = strlen(s); if (n + (int)m + 1 < MAXLOG) { memcpy(buf + n, s, m); n += (int)m; buf[n++] = ' '; buf[n] = 0; } }
static void Ev(const char* s) { Put(got, ngot, s); }
static void Wv(const char* s) { Put(want, nwant, s); }
static const char *termScript, *startScript, *questScript;   // '1' = true; past the end: true / true / false
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

enum { FRAMES = 64 };
struct World { unsigned char state[FRAMES], talk[FRAMES], hit[FRAMES], ability[FRAMES], heal[FRAMES]; int speakFrames; };
static World world;
struct State { int frame, tpos, spos, qpos, busyUntil; bool valid; };
static State gs, ms;
static int F(int f) { return f < FRAMES ? f : FRAMES - 1; }

static union { unsigned char b[0x1c]; double align; } ent;
static union { unsigned char b[0x64]; double align; } parent;

struct Info { unsigned long RefCount; void* DeleteFunc; void* Data; };
struct Impl { const char* name; };
struct Thing { void** vtbl; Impl* data; Info* info; };
static void* thingVtbl[80];
static Impl meImpl, heroImpl;
static Info meInfo;
static Thing heroThing;
static Thing* Me() { return (Thing*)(ent.b + 8); }
static int liveThings, baseLive, movieLive;
static const char* Who(const void* t)
{ const Thing* th = (const Thing*)t; return t == Me() ? "Me" : t == &heroThing ? "Hero" : (th->data == &meImpl ? "MeCopy" : th->data ? th->data->name : "null"); }

struct Handle { void* vtbl; void* ip; void* data; Info* info; };
static Info hMeInfo; static int meStbImpl;
enum { REF0 = 1000 };
static void* sehMeAddr;
static void SetState(int f) { long st = world.state[F(f)]; memcpy(parent.b + 0x50, &st, 4); }

extern "C" {
void __fastcall StubBaseCtor(void* self, void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseDtor(void*, void*) { --baseLive; }
void __fastcall StubThingDtor(void* self, void*) { Thing* t = (Thing*)self; if (t->info) --t->info->RefCount; --liveThings; memset(self, 0xDD, 12); }
bool __fastcall StubIsValid(const void* self, void*) { if (sehMeAddr && self != sehMeAddr) Bad("IsValid receiver"); return ((const Handle*)self)->data != 0; }
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
    ++liveThings;   // the predicate's copy (implicit copy COMDAT, no stub) is destroyed through ~CScriptThing
    Ev("P");
}
bool __fastcall StubTerminating(void* self, void*)
{ if (self != ent.b) Bad("terminate receiver"); bool t = NextBit(termScript, gs.tpos, true); Ev(t ? "T" : "t"); return t; }
}

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

static void* gameVtbl[900]; static void* gameObj[1]; static bool badGame;
static void CheckGame(const void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*) { CheckGame(self); Ev("N"); ++gs.frame; SetState(gs.frame); }
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
static Thing* __fastcall G_GetHero(void* self, void*) { CheckGame(self); return &heroThing; }
static float healthNext; static const float* healthScript; static int healthPos, mHealthPos;
static float __fastcall G_GetHealth(void* self, void*, const void* thing)
{ CheckGame(self); healthNext = healthScript[healthPos++]; char s[48]; sprintf(s, "HEALTH:%s=%g", Who(thing), healthNext); Ev(s); return healthNext; }
static bool __fastcall G_QuestActive(const void* self, void*, const CCharString* q)
{ CheckGame(self); bool r = NextBit(questScript, gs.qpos, false); char s[64]; sprintf(s, "QUEST:%s=%d", q->Text, r); Ev(s); return r; }
static void __fastcall G_HasInfo(void* self, void*, const void* t, bool a, bool b, bool c) { CheckGame(self); char s[48]; sprintf(s, "INFO:%s/%d%d%d", Who(t), a, b, c); Ev(s); }
static int movieImpl;
static void __fastcall MovieDelete(void* data) { if (data != &movieImpl) Bad("movie delete data"); --movieLive; Ev("~MOVIE"); }
static void __fastcall G_StartMovie(void* self, void*, const CCharString* name, void* movie)
{
    CheckGame(self); char s[48]; sprintf(s, "MOVIE:[%s]", name->Text); if (((void**)movie)[2] || ((void**)movie)[3]) Bad("movie handle not empty"); Ev(s);
    Info* info = new Info; info->RefCount = 1; info->DeleteFunc = (void*)MovieDelete; info->Data = &movieImpl;
    ((void**)movie)[2] = &movieImpl; ((void**)movie)[3] = info; ++movieLive;
}
static void __fastcall G_Pause(void* self, void*, bool b) { CheckGame(self); Ev(b ? "PAUSE1" : "PAUSE0"); }
static void __fastcall G_Killable(const void* self, void*, const void* t, bool a, bool b) { CheckGame(self); char s[48]; sprintf(s, "KILLABLE:%s/%d%d", Who(t), a, b); Ev(s); }
static void __fastcall G_Combo(const void* self, void*, const void* t, bool b) { CheckGame(self); char s[48]; sprintf(s, "COMBO:%s/%d", Who(t), b); Ev(s); }
static void __fastcall G_Stat(const void* self, void*, const void* t, bool b) { CheckGame(self); char s[48]; sprintf(s, "STAT:%s/%d", Who(t), b); Ev(s); }
static void __fastcall G_Pushable(void* self, void*, Thing t, bool b)
{   // by-value CScriptThing (the expanded inline copy): the callee destroys it
    CheckGame(self); char s[64];
    bool copyOk = t.data == &meImpl && t.info == &meInfo && meInfo.RefCount == REF0 + 1 && t.vtbl != 0;
    sprintf(s, "PUSHABLE:%s/%d", copyOk ? "MeCopy" : "?", b); if (t.info) --t.info->RefCount; Ev(s);
}

// ---- reference model -----------------------------------------------------------------------------------------------------------
static bool mInMovie;
static bool MT() { bool t = NextBit(termScript, ms.tpos, true); Wv(t ? "T" : "t"); if (t && mInMovie) { Wv("PAUSE0"); Wv("~MOVIE"); } return t; }
static void MF() { Wv("N"); ++ms.frame; }
static bool MStart(int prio) { bool ok = NextBit(startScript, ms.spos, true); if (ok) ms.valid = true; char s[8]; sprintf(s, "%c%d", ok ? 'S' : 's', prio); Wv(s); return ok; }
static void MRelease() { if (ms.valid) { Wv("R"); ms.valid = false; } }
static long MState() { return world.state[F(ms.frame)]; }
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
static void Model()
{
    char s[96];
    Wv("P"); MF(); if (MT()) return;
    Wv("COMBO:Me/0"); Wv("STAT:Me/0"); Wv("PUSHABLE:MeCopy/0"); Wv("KILLABLE:Me/01"); Wv("INFO:Me/000");
    for (;; MF()) {
        if (MT()) return;
        bool talk = world.talk[F(ms.frame)] != 0; sprintf(s, "TALK:SCRIPT_NAME_HERO=%d", talk); Wv(s);
        if (talk) {
            if (MT()) return;
            MRelease();
            while (!MStart(4)) { MF(); if (MT()) return; }
            if (MT()) return;
            Wv("MOVIE:[]"); Wv("PAUSE1"); mInMovie = true;
            long st = MState(); const char* line = 0;
            if (st == 2) line = "TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK";
            else if (st == 3) line = "TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK";
            else if (st == 4) line = "TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK";
            else if (st == 6) line = "TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK";
            else if (st == 7) {
                bool q = NextBit(questScript, ms.qpos, false); Wv(q ? "QUEST:Q_WhiteBalverineWW=1" : "QUEST:Q_WhiteBalverineWW=0");
                if (!q) line = "TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK";
            }
            if (line) { if (MT()) return; if (!MSpeak(line)) return; }
            Wv("PAUSE0"); Wv("~MOVIE"); mInMovie = false;
            MRelease();
        }
        int f = F(ms.frame);
        bool hit = world.hit[f] != 0; sprintf(s, "HIT:SCRIPT_NAME_HERO=%d", hit); Wv(s);
        if (!hit) {
            bool any = world.ability[f] != 0; sprintf(s, "ANYABIL:SCRIPT_NAME_HERO=%d", any); Wv(s);
            if (any) { bool heal = world.heal[f] != 0; sprintf(s, "ABIL14:SCRIPT_NAME_HERO=%d", heal); Wv(s); hit = !heal; }
        }
        if (hit && MState() != 7) {
            if (MT()) return;
            MF(); if (MT()) return;
            MRelease();
            while (!MStart(4)) { MF(); if (MT()) return; }
            if (MT()) return;
            Wv("MOVIE:[]"); Wv("PAUSE1"); mInMovie = true;
            if (!MSpeak("TEXT_QST_074_CHIEF_BEEN_ATTACKED")) return;
            Wv("PAUSE0"); Wv("~MOVIE"); mInMovie = false;
            MF(); if (MT()) return;
            Wv("FRIENDS1");
            MRelease();
        }
    }
}

static int cov[16];
static void Run(const World& w, const char* term, const char* start, const char* quest, const float* health, int k)
{
    world = w; termScript = term; startScript = start; questScript = quest; healthScript = health; healthPos = mHealthPos = 0;
    memset(&gs, 0, sizeof gs); memset(&ms, 0, sizeof ms); ngot = nwant = 0; got[0] = want[0] = 0; mInMovie = false;
    memset(parent.b, 0x5A, sizeof parent.b); SetState(0);
    memset(ent.b, 0x11, sizeof ent.b); void* g = gameObj; memcpy(ent.b + 4, &g, 4); void* pp = parent.b; memcpy(ent.b + 0x14, &pp, 4);
    Thing* me = Me(); me->vtbl = thingVtbl; me->data = &meImpl; me->info = &meInfo;
    meInfo.RefCount = REF0; hMeInfo.RefCount = REF0;
    liveThings = baseLive = liveStrings = movieLive = 0; badGame = false; sehMeAddr = 0;
    Model();
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
    check(liveThings == 0, "every CScriptThing (predicate copy, script-thing temporaries) destroyed", k);
    check(baseLive == 0 && movieLive == 0, "every CBaseObject part and movie destroyed", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(meInfo.RefCount == REF0, "Me's reference count balanced (predicate copy, by-value copy)", k);
    check(hMeInfo.RefCount == REF0, "handle references balanced", k);
    const char* pats[] = { "IN_FIRST_ATTACK", "AFTER_FIRST_ATTACK", "IN_SECOND_ATTACK", "IN_THIRD_ATTACK", "AFTER_THIRD_ATTACK", "QUEST:Q_WhiteBalverineWW=1",
        "BEEN_ATTACKED", "FRIENDS1", "s4", "BUSY1 N T PAUSE0", "ABIL14:SCRIPT_NAME_HERO=1", "HEALTH:MeCopy=0", "HIT:SCRIPT_NAME_HERO=1 N" };
    for (int i = 0; i < 13; ++i) if (strstr(got, pats[i])) ++cov[i];
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int i = 0; i < 80; ++i) thingVtbl[i] = (void*)VirtualTrap;
    thingVtbl[21] = (void*)T_HitBy; thingVtbl[27] = (void*)T_TalkedTo; thingVtbl[41] = (void*)T_AbilityFrom; thingVtbl[42] = (void*)T_AnyAbilityFrom;
    thingVtbl[67] = (void*)T_Friends;
    meImpl.name = "me"; heroImpl.name = "hero";
    heroThing.vtbl = thingVtbl; heroThing.data = &heroImpl; heroThing.info = 0;
    for (int i = 0; i < 900; ++i) gameVtbl[i] = (void*)VirtualTrap;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScripting; gameVtbl[70] = (void*)G_GetHero; gameVtbl[264] = (void*)G_GetHealth;
    gameVtbl[284] = (void*)G_QuestActive; gameVtbl[360] = (void*)G_HasInfo; gameVtbl[370] = (void*)G_StartMovie; gameVtbl[379] = (void*)G_Pause;
    gameVtbl[517] = (void*)G_Killable; gameVtbl[526] = (void*)G_Combo; gameVtbl[527] = (void*)G_Stat; gameVtbl[844] = (void*)G_Pushable;
    static float health[96]; int k = 0;
    static const char zeros[] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    static const unsigned char states[] = { 2, 3, 4, 6, 7, 7, 5, 0 };
    for (int variant = 0; variant < 8; ++variant) {
        World w; memset(&w, 0, sizeof w);
        for (int f = 0; f < FRAMES; ++f) w.state[f] = states[variant];
        w.talk[2] = 1; w.talk[7] = 1; w.hit[4] = 1; w.ability[9] = 1; w.heal[9] = variant & 1; w.hit[12] = variant == 6;
        w.speakFrames = variant == 3 ? 0 : 2;
        for (int i = 0; i < 96; ++i) health[i] = (variant == 7 && i == 0) ? 0.0f : 10.0f;
        char ts[128];
        for (int t = 0; t < 90; ++t) { memcpy(ts, zeros, 100); ts[t] = '1'; ts[100] = 0; Run(w, ts, (t & 1) ? "1" : "0010", variant == 5 ? "1" : "0", health, ++k); }
    }
    unsigned long seed = 37;
    for (int r = 0; r < 6000; ++r) {
#define RND(n) (seed = seed * 1103515245ul + 12345ul, (int)((seed >> 16) % (n)))
        World w; memset(&w, 0, sizeof w);
        static const unsigned char pick[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        for (int f = 0; f < FRAMES; ++f) { w.state[f] = pick[RND(8)]; w.talk[f] = RND(6) == 0; w.hit[f] = RND(7) == 0; w.ability[f] = RND(7) == 0; w.heal[f] = RND(2); }
        w.speakFrames = RND(4);
        for (int i = 0; i < 96; ++i) health[i] = RND(4) ? 5.0f : (RND(2) ? 0.0f : -1.0f);
        static char ts[400], ss[16], qs[64];
        for (int i = 0; i < 399; ++i) ts[i] = RND(60) == 0 ? '1' : '0';
        ts[399] = 0;
        for (int i = 0; i < 15; ++i) ss[i] = RND(3) == 0 ? '0' : '1';
        ss[15] = 0;
        for (int i = 0; i < 63; ++i) qs[i] = RND(2) ? '1' : '0';
        qs[63] = 0;
        Run(w, ts, ss, qs, health, 1000 + r);
    }
    for (int i = 0; i < 13; ++i) printf("cov%d %d\n", i, cov[i]);
    if (failures) { printf("00e17840_TEST FAIL %d\n", failures); return 1; }
    printf("00e17840_TEST PASS (%d targeted, 6000 random)\n", k);
    return 0;
}