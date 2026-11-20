// Behaviour test for 0x00E97070 NScript::CV_HeroDollsScript::CBS_Teacher::Main() (links the REAL row object: the row body, the expanded seh_me /
// seh_KidIntro / movie ctors, the ScriptedThingBase and movie dtor COMDATs, the expanded KidIntro assignments (CCountedPointer ShareData /
// ReleaseData), the expanded and called ~CScriptThing of the GetNearestWithDefName / GetScriptThing temporaries and the pause expansion all run as
// compiled). No class models: every external the row object names is bound with /alternatename -- the calls the row makes to extern "C"
// __fastcall stubs (thiscall-compatible, unused edx), every vtable slot and unreached COMDAT reference to a trap. Game (+0x04) slots 7, 8, 39,
// 70, 81, 114, 121, 264, 281, 288, 295, 296, 360, 361, 370, 376, 379, 386, 394, 655, 737, KidIntro's slots 1 / 75 and the hero's slot 36 through
// fake vtables; things carry real counted-pointer info blocks {RefCount, DeleteFunc, Data} that the row releases itself. ParentClass (+0x14) and
// PMasterData (+0x18) are raw buffers at the header offsets (+0x48 / +0x49, +0xF4); frames can end the doll script from outside.
// Reference model: Model() below, written from the script's statements; every run is compared event by event (with the destruction order of the
// live locals on each early return) plus the final state of every field the row writes and the release of every string and thing.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:__purecall=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@BODtor@8")
#pragma comment(linker, "/alternatename:??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ=@MovieCPDtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@BOCtor@8")
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
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=@BaseClassDtor@8")
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
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=@ClearCmd@8")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=@Follow@20")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@ReleaseStub@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@IsValidStub@8")
#pragma comment(linker, "/alternatename:??9CCharString@@QBE_NPBD@Z=@StrNe@12")
#pragma comment(linker, "/alternatename:??8?$CBasicString@D@@QBE_NPBD@Z=@StrEq@12")
#pragma comment(linker, "/alternatename:?RemoveHeroDolls@CV_HeroDollsScript@NScript@@QAEXXZ=@RemoveDolls@8")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@Dist@12")
#pragma comment(linker, "/alternatename:?HeroHasAllDolls@CV_HeroDollsScript@NScript@@QAE_NXZ=@HasAll@8")
#pragma comment(linker, "/alternatename:?HeroHasAnyDolls@CV_HeroDollsScript@NScript@@QAE_NXZ=@HasAny@8")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:_g_MovieVtbl=??_7CScriptGameResourceObjectMovieBase@@6B@")
#pragma comment(linker, "/alternatename:@CallRow@8=?Main@CBS_Teacher@CV_HeroDollsScript@NScript@@EAEXXZ")
extern "C" char g_MovieVtbl;
extern "C" void __fastcall CallRow(void* self, void*);

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static int NextDigit(Script& sc) { char c = sc.s[sc.pos]; if (!c) return 1; ++sc.pos; return c - '0'; }   // answers: digit - 1
enum { NS = 15 };
static Script sTerm, sAlive, sAny, sAll, sStart, sDist, sAns, sSpeak, sPerf, sName, sCtl, sCut, sTav, sInv, sEnd;
// past the end: terminating / dead / no dolls / not all / started / near / answered no / dead / idle / named "" / controlled / no cutscene /
// no tavern game / nothing received / not ended

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 20000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static void Ws(const char* s) { while (*s) W(*s++); }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x40]; double a; } self;                 // CBS_Teacher (0x28)
static union { unsigned char b[0x60]; double a; } parent;               // CV_HeroDollsScript (ParentClass)
static union { unsigned char b[0x120]; double a; } master;              // CQ_SunnyvaleMasterData
static void* gameVtbl[800]; static void* gameObj[1];
static void* meVtbl[80]; static void* thVtbl[80];
static void* heroVtbl[40]; static void* hero[3];
struct Info { long RefCount; void* DeleteFunc; void* Data; };
static Info meInfo; static int liveInfos, deletes; static long nextData = 0x7000;
extern "C" void __fastcall DelFunc(void* data) { ++deletes; Ev('X'); (void)data; }
static Info* NewInfo() { Info* i = (Info*)malloc(sizeof(Info)); i->RefCount = 1; i->DeleteFunc = (void*)DelFunc; i->Data = (void*)(nextData += 0x10); ++liveInfos; return i; }
void __cdecl operator delete(void* p) { if (p != &meInfo) { free(p); --liveInfos; } }
static void FillThing(void** t) { Info* i = NewInfo(); t[0] = thVtbl; t[1] = i->Data; t[2] = i; }
static void ReleaseInfo(Info* i) { if (i && --i->RefCount == 0) { ((void (__fastcall*)(void*))i->DeleteFunc)(i->Data); operator delete(i); } }
static void** Me() { return (void**)(self.b + 8); }
static void** Kid() { return (void**)(self.b + 0x1C); }
#define ENDED (*(bool*)(parent.b + 0x48))
#define KIDDONE (*(bool*)(parent.b + 0x49))
#define USING (*(bool*)(master.b + 0xF4))

// strings: one text pointer, logged by a letter per literal
static int liveStrings;
static char Lit(const char* t)
{
    static const char* k[] = { "CREATURE_BS_VILLAGER_BOY", "", "TEXT_QST_083_TEACHER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO",
        "OBJECT_DOLL_JACK_OF_BLADES_01", "OBJECT_QUEST_CARD_HERO_DOLLS", "TEXT_QUEST_HERO_DOLLS_OBJECTIVE_01", "QUEST_HERO_DOLLS" };
    static const char l[] = "b0wynjco9";
    if (!t) return '?';
    for (int i = 0; i < 9; ++i) if (!strcmp(t, k[i])) return l[i];
    return '?';
}
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; check(len == -1, "string length", 0); Ev('s'); Ev(Lit(text)); return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; Ev('~'); Ev(Lit(*s)); *s = 0; }
static const char* kidName = "KID";
extern "C" bool __fastcall StrNe(const char* const* s, void*, const char* text)
{
    check(s == &kidName && !strcmp(text, ""), "KidIntro.GetName() != \"\"", 0); bool r = Next(sName); Ev('!'); Ev(r ? '1' : '0'); return r;
}
extern "C" bool __fastcall StrEq(const char* const* s, void*, const char* text) { return !StrNe(s, 0, text); }   // for mutants: the same scripted name
extern "C" void __fastcall BaseClassDtor(void* t, void*) { (void)t; }

// things: KidIntro's slots 1 GetName / 75 IsAlive, the hero's slot 36 MsgReceivedInventoryItem
static char Who(void* const* t) { return t == (void* const*)Me() ? 'M' : t == (void* const*)Kid() ? 'K' : t == (void* const*)hero ? 'h' : '?'; }
extern "C" const char* const* __fastcall K_GetName(void* t, void*) { check(t == Kid(), "GetName on KidIntro", 0); Ev('V'); return &kidName; }
extern "C" bool __fastcall K_IsAlive(void* t, void*) { check(t == Kid(), "IsAlive on KidIntro", 0); bool r = Next(sAlive); Ev('L'); Ev(r ? '1' : '0'); return r; }
extern "C" bool __fastcall H_Received(void* t, void*) { check(t == hero, "MsgReceivedInventoryItem on the hero", 0); bool r = Next(sInv); Ev('i'); Ev(r ? '1' : '0'); return r; }

// ScriptedThingBase locals {vptr, IntelligentPointer, Data, Info}; movie {vptr, IntelligentPointer, PImp, Info}
static void* sehMe; static void* sehKid;
static char Seh(void* h) { return h && h == sehMe ? 'R' : h && h == sehKid ? 'r' : '?'; }
extern "C" void* __fastcall BOCtor(void** o, void*) { if (!sehMe) sehMe = o; o[1] = 0; Ev('o'); return o; }   // the first CBaseObject built is seh_me
extern "C" void __fastcall BODtor(void** o, void*) { (void)o; Ev('O'); }
extern "C" void __fastcall MovieCPDtor(void** cp, void*) { check(cp[0] == 0 && cp[1] == 0, "movie counted pointer empty", 0); Ev('c'); }
extern "C" bool __fastcall IsValidStub(void* const* h, void*) { bool r = h[2] != 0; Ev('v'); Ev(Seh((void*)h)); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall ReleaseStub(void** h, void*) { check(h[3] != 0, "Release on a held handle", 0); Ev('-'); ReleaseInfo((Info*)h[3]); h[2] = h[3] = 0; }
extern "C" void* __fastcall GetScriptThing(void* const* h, void*, void** ret) { FillThing(ret); Ev(Seh((void*)h)); return ret; }
extern "C" bool __fastcall IsPerf(void* h, void*) { bool r = Next(sPerf); Ev(Seh(h)); Ev(r ? 'Q' : 'q'); return r; }
extern "C" void __fastcall ClearCmd(void* h, void*) { Ev('E'); Ev(Seh(h)); }
extern "C" void __fastcall Follow(void* h, void*, void* const* t, float d, int run)
{
    check(t == (void* const*)hero && d == 1.0f && (run & 0xFF) == 1, "FollowThing(hero, 1.0f, true)", 0); Ev('W'); Ev(Seh(h));
}
static char TextLetter(const char* t)
{
    static const char* keys[4] = { "TEXT_QST_083_TEACHER_INTRODUCTION", "TEXT_QST_083_TEACHER_DO_SWAP", "TEXT_QST_083_TEACHER_DONT_SWAP", "TEXT_QST_083_DOLLKID_INTRODUCTION" };
    static const char let[4] = { 'i', 's', 'x', 'k' };
    for (int i = 0; i < 4; ++i) if (!strcmp(t, keys[i])) return let[i];
    return '?';
}
extern "C" void __fastcall Speak(void* h, void*, void* const* to, const char* text, long sel, int b1, int b2, int b3)
{
    check(to == (void* const*)hero && sel == 0 && (b1 & 0xFF) == 0 && (b2 & 0xFF) == 1 && (b3 & 0xFF) == 0, "speak args", 0); Ev('K'); Ev(Seh(h)); Ev(TextLetter(text));
}

// CV_HeroDollsScript
extern "C" bool __fastcall HasAny(void* p, void*) { check(p == parent.b, "HeroHasAnyDolls on ParentClass", 0); bool r = Next(sAny); Ev('H'); Ev(r ? '1' : '0'); return r; }
extern "C" bool __fastcall HasAll(void* p, void*) { check(p == parent.b, "HeroHasAllDolls on ParentClass", 0); bool r = Next(sAll); Ev('A'); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall RemoveDolls(void* p, void*) { check(p == parent.b, "RemoveHeroDolls on ParentClass", 0); Ev('r'); }

extern "C" bool __fastcall Dist(void* const* a, void* const* b, float d)
{
    check(a == (void* const*)hero && (d == 20.0f || d == 5.0f), "distance from the hero", 0); bool r = Next(sDist);
    Ev('D'); Ev(Who(b)); Ev(d == 20.0f ? '2' : '5'); Ev(r ? '1' : '0'); return r;
}

// Game
static bool T_(void* s) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall IsTerm(void* s, void*) { return T_(s); }
static void G(void* g, const char* what) { check(g == gameObj, what, 0); }
extern "C" void __fastcall G_NewScriptFrame(void* g, void*) { G(g, "frame this"); Ev('F'); if (Next(sEnd)) ENDED = true; }
extern "C" bool __fastcall G_Start(void* g, void*, void* const* thing, void** h, long prio)
{
    G(g, "start this"); char w = Who(thing); check((w == 'M' || w == 'K') && prio == 4, "start scripting Me / KidIntro at priority 4", 0);
    if (w == 'M') check(h == sehMe, "Me started into seh_me", 0); else sehKid = h;
    bool r = Next(sStart); Ev(r ? 'S' : 'n'); Ev(w);
    if (r) { check(!h[2] && !h[3], "started into an empty handle", 0); Info* i = NewInfo(); h[2] = i->Data; h[3] = i; }
    return r;
}
extern "C" void* __fastcall G_GetHero(void* g, void*) { G(g, "hero this"); Ev('Y'); return hero; }
extern "C" void* __fastcall G_Nearest(void* g, void*, void** ret, void* const* from, const char* const* name)
{
    G(g, "nearest this"); check(!strcmp(*name, "CREATURE_BS_VILLAGER_BOY"), "nearest CREATURE_BS_VILLAGER_BOY", 0);
    FillThing(ret); Ev('N'); Ev(Who(from)); return ret;
}
extern "C" long __fastcall G_Answer(void* g, void*) { G(g, "answer this"); int d = NextDigit(sAns); Ev('a'); Ev((char)('0' + d)); return d - 1; }
extern "C" void __fastcall G_YesNo(void* g, void*, const char* const* q, const char* const* y, const char* const* n, const char* const* e, int f)
{
    G(g, "yes/no this"); check(Lit(*q) == 'w' && Lit(*y) == 'y' && Lit(*n) == 'n' && Lit(*e) == '0' && (f & 0xFF) == 1, "GiveHeroYesNoQuestion args", 0); Ev('G');
}
extern "C" void __fastcall G_GiveObject(void* g, void*, const char* const* o, long n, int f) { G(g, "give object this"); check(Lit(*o) == 'j' && n == -1 && !(f & 0xFF), "GiveHeroObject args", 0); Ev('g'); }
extern "C" float __fastcall G_GetHealth(void* g, void*, void* const* t)
{
    G(g, "health this"); check(t[0] == thVtbl, "health of the script thing", 0);
    bool alive = Next(sSpeak); Ev(alive ? 'U' : 'u'); return alive ? 2.0f : 0.0f;
}
extern "C" void __fastcall G_Deactivate(void* g, void*, const char* const* q, unsigned long n) { G(g, "deactivate this"); check(Lit(*q) == '9' && n == 0, "DeactivateQuestLater args", 0); Ev('d'); }
extern "C" void __fastcall G_QuestCompleted(void* g, void*, const char* const* q, int a, int b, int c) { G(g, "completed this"); check(Lit(*q) == '9' && !(a & 0xFF) && !(b & 0xFF) && !(c & 0xFF), "SetQuestAsCompleted args", 0); Ev('e'); }
extern "C" void __fastcall G_GiveCard(void* g, void*, const char* const* card, const char* const* q, int f) { G(g, "card this"); check(Lit(*card) == 'c' && Lit(*q) == '9' && !(f & 0xFF), "GiveHeroQuestCardDirectly args", 0); Ev('C'); }
extern "C" void __fastcall G_Objective(void* g, void*, const char* const* q, const char* const* o, const char* const* a, const char* const* b)
{
    G(g, "objective this"); check(Lit(*q) == '9' && Lit(*o) == 'o' && Lit(*a) == '0' && Lit(*b) == '0', "SetQuestCardObjective args", 0); Ev('J');
}
extern "C" void __fastcall G_SetInfo(void* g, void*, void* const* t, int a, int b, int c) { G(g, "set info this"); check(!(a & 0xFF) && (b & 0xFF) == 1 && !(c & 0xFF), "SetThingHasInformation(x, false, true, false)", 0); Ev('I'); Ev(Who(t)); }
extern "C" void __fastcall G_ClearInfo(void* g, void*, void* const* t) { G(g, "clear info this"); Ev('i'); Ev('-'); Ev(Who(t)); }
extern "C" void __fastcall G_StartMovie(void* g, void*, const char* const* name, void* const* movie)
{
    G(g, "movie this"); check(!strcmp(*name, "") && movie[0] == (void*)&g_MovieVtbl && !movie[2] && !movie[3], "movie sequence args", 0); Ev('m');
}
extern "C" void __fastcall G_Pause(void* g, void*, float d) { G(g, "pause this"); check(d == 0.5f, "Pause(0.5f)", 0); Ev('P'); }
static int pauseDepth;
extern "C" void __fastcall G_PauseEntities(void* g, void*, int on) { G(g, "pause entities this"); pauseDepth += (on & 0xFF) ? 1 : -1; Ev((on & 0xFF) ? 'Z' : 'z'); }
extern "C" bool __fastcall G_Cutscene(void* g, void*) { G(g, "cutscene this"); bool r = Next(sCut); Ev('%'); Ev(r ? '1' : '0'); return r; }
extern "C" bool __fastcall G_Controlled(void* g, void*) { G(g, "controlled this"); bool r = Next(sCtl); Ev('#'); Ev(r ? '1' : '0'); return r; }
extern "C" bool __fastcall G_Tavern(void* g, void*) { G(g, "tavern this"); bool r = Next(sTav); Ev('&'); Ev(r ? '1' : '0'); return r; }
extern "C" void* __fastcall G_QuestName(void* g, void*, const char** ret) { G(g, "quest name this"); *ret = "QUEST_HERO_DOLLS"; ++liveStrings; Ev('q'); return ret; }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mAlive, mAny, mAll, mStart, mDist, mAns, mSpeak, mPerf, mName, mCtl, mCut, mTav, mInv, mEnd;
struct State { bool ended, kidDone, using_; } m;
static int cYes, cNo, cKid, cKidName, cKidCard, cRecv, cRelMe, cRelKid, cOutside;
static char live[16][4]; static int nlive;                               // destruction events of the live locals, innermost last
static void Push(const char* d) { strcpy(live[nlive++], d); }
static void Pop() { --nlive; Ws(live[nlive]); }
static bool T() { bool r = Next(mTerm); W(r ? 'T' : 't'); if (r) while (nlive) Pop(); return r; }
static void F() { W('F'); if (Next(mEnd)) { if (!m.ended) ++cOutside; m.ended = true; } }
static bool B(Script& s, char tag) { bool r = Next(s); W(tag); W(r ? '1' : '0'); return r; }
static bool Say(char who, char text)                                     // the speak-and-wait macro; true = returned
{
    W(who); bool sp = Next(mSpeak); W(sp ? 'U' : 'u'); W('X');
    if (!sp) return false;
    Ws("YK"); W(who); W(text);
    for (;;) { bool pf = Next(mPerf); W(who); W(pf ? 'Q' : 'q'); if (!pf) break; F(); if (T()) return true; }
    return T();
}
static bool Start(char w) { bool r = Next(mStart); W(r ? 'S' : 'n'); W(w); if (r) strcpy(live[nlive - 1], "XO"); return r; }
static bool Dist(char w, char d) { W('Y'); bool r = Next(mDist); W('D'); W(w); W(d); W(r ? '1' : '0'); return r; }
static void ReleaseIfValid(char who) { W('v'); W(who); bool held = !strcmp(live[nlive - 1], "XO"); W(held ? '1' : '0'); if (held) { if (who == 'R') ++cRelMe; else ++cRelKid; Ws("-X"); strcpy(live[nlive - 1], "O"); } }
static void Assign() { Ws("X~b"); }                                      // operator= releases KidIntro's old thing (its last owner), then ~"CREATURE.."
static void Model()
{
    W('o'); Push("O");                                                   // seh_me
    if (B(mAlive, 'L')) { if (T()) return; Ws("i-K"); }
    bool DoesHeroHaveAnyDolls = B(mAny, 'H');
    W('P');
    while (!m.ended) {
        F(); if (T()) return;
        if (B(mAll, 'A')) {
            if (T()) return;
            m.using_ = true;
            Ws("IM");
            Start('M');
            while (!Dist('M', '2')) { F(); if (T()) return; }
            if (T()) return;
            Ws("YWR");
            while (!Dist('M', '5')) { F(); if (T()) return; }
            if (T()) return;
            Ws("ER");
            W('o'); Push("cO"); Ws("s0m~0"); W('Z'); Push("z");           // speech_movie, PauseMyEntitiesPlease
            if (Say('R', 'i')) return;
            Ws("s0snsysw" "G" "~w~y~n~0");
            long answer;
            for (;;) { int d = NextDigit(mAns); W('a'); W((char)('0' + d)); answer = d - 1; if (answer >= 0) break; F(); if (T()) return; }
            if (T()) return;
            if (answer == 1) {
                if (T()) return;
                if (Say('R', 's')) return;
                ++cYes; W('r'); Ws("sjg~j");
                m.ended = true;
            } else {
                if (T()) return;
                if (Say('R', 'x')) return;
                ++cNo;
                Pop(); Pop();
                break;
            }
            Pop(); Pop();
        } else if (!m.kidDone && DoesHeroHaveAnyDolls) {
            if (T()) return;
            W('o'); Push("O");                                           // seh_KidIntro
            ++cKid; Ws("sbYNh"); Assign();
            W('V'); if (B(mName, '!')) { if (T()) return; ++cKidName; Ws("sbNK"); Assign(); }
            Ws("IK");
            Start('K');
            Ws("YWr");
            F(); if (T()) return;
            while (!Dist('K', '5')) { F(); if (T()) return; }
            if (T()) return;
            for (;;) {
                bool c = !B(mCtl, '#'); if (!c) c = B(mCut, '%'); if (!c) c = B(mTav, '&');
                if (!c) break;
                F(); if (T()) return;
            }
            if (T()) return;
            Ws("Er");
            W('o'); Push("cO"); Ws("s0m~0"); W('Z'); Push("z");
            if (Say('r', 'k')) return;
            Pop(); Pop();
            Ws("i-K");
            ReleaseIfValid('r');
            Ws("sc" "q" "C" "~9~c");
            Ws("s0s0so" "q" "J" "~9~o~0~0");
            m.kidDone = true; ++cKidCard;
            Pop();
        } else {
            if (T()) return;
            W('Y'); if (B(mInv, 'i')) { if (T()) return; if (B(mAny, 'H')) { if (T()) return; DoesHeroHaveAnyDolls = true; ++cRecv; } }
        }
    }
    if (T()) return;
    Ws("i-M");
    ReleaseIfValid('R');
    m.using_ = false;
    Ws("q" "e" "~9");
    Ws("q" "d" "~9");
    Pop();
}

static void Run(const char* const* in, int flags, int k)
{
    memset(self.b, 0xEE, sizeof self.b); memset(parent.b, 0xDD, sizeof parent.b); memset(master.b, 0xCC, sizeof master.b);
    *(void**)(self.b + 0x00) = (void*)0x5E1F; *(void**)(self.b + 0x04) = gameObj;
    Me()[0] = meVtbl; Me()[1] = (void*)0x4D45; Me()[2] = &meInfo;
    *(void**)(self.b + 0x14) = parent.b; *(void**)(self.b + 0x18) = master.b;
    liveStrings = liveInfos = deletes = pauseDepth = 0;
    Info* ki = NewInfo(); Kid()[0] = thVtbl; Kid()[1] = ki->Data; Kid()[2] = ki;
    ENDED = (flags & 1) != 0; KIDDONE = (flags & 2) != 0; USING = (flags & 4) != 0;
    Script* ss[NS] = { &sTerm, &sAlive, &sAny, &sAll, &sStart, &sDist, &sAns, &sSpeak, &sPerf, &sName, &sCtl, &sCut, &sTav, &sInv, &sEnd };
    Script* ms[NS] = { &mTerm, &mAlive, &mAny, &mAll, &mStart, &mDist, &mAns, &mSpeak, &mPerf, &mName, &mCtl, &mCut, &mTav, &mInv, &mEnd };
    static const bool df[NS] = { true, false, false, false, true, true, false, false, false, false, true, false, false, false, false };
    for (int i = 0; i < NS; ++i) { ss[i]->s = in[i]; ss[i]->pos = 0; ss[i]->dflt = df[i]; *ms[i] = *ss[i]; }
    m.ended = ENDED; m.kidDone = KIDDONE; m.using_ = USING;
    ngot = nwant = 0; nlive = 0; sehMe = sehKid = 0;
    meInfo.RefCount = 1; meInfo.DeleteFunc = (void*)VirtualTrap; meInfo.Data = Me()[1];
    Model();
    CallRow(self.b, 0);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) printf("  got  %.900s\n  want %.900s\n", got, want);
    }
    check(ENDED == m.ended && KIDDONE == m.kidDone && USING == m.using_, "ParentClass / PMasterData flags", k);
    check(parent.b[0x47] == 0xDD && parent.b[0x4A] == 0xDD && master.b[0xF3] == 0xCC && master.b[0xF5] == 0xCC, "no other parent / master store", k);
    check(meInfo.RefCount == 1 && Me()[0] == meVtbl && Me()[1] == (void*)0x4D45 && Me()[2] == &meInfo, "Me untouched", k);
    check(*(void**)(self.b + 0x14) == parent.b && *(void**)(self.b + 0x18) == master.b && self.b[0x28] == 0xEE && Kid()[0] == thVtbl, "no other store to this", k);
    Info* kn = (Info*)Kid()[2]; check(kn && kn->RefCount == 1 && Kid()[1] == kn->Data, "KidIntro owns its thing once", k);
    ReleaseInfo(kn); Kid()[2] = 0;
    check(liveStrings == 0 && liveInfos == 0 && pauseDepth == 0, "everything released", k);
}
static void Run1(const char* term, const char* alive, const char* any, const char* all, const char* start, const char* dist, const char* ans,
                 const char* speak, const char* perf, const char* name, const char* ctl, const char* cut, const char* tav, const char* inv,
                 const char* end, int flags, int k)
{
    const char* in[NS] = { term, alive, any, all, start, dist, ans, speak, perf, name, ctl, cut, tav, inv, end };
    Run(in, flags, k);
}

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; hero[0] = heroVtbl; hero[1] = (void*)0x4845; hero[2] = 0;
    for (int i = 0; i < 800; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) meVtbl[i] = thVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 40; ++i) heroVtbl[i] = (void*)VirtualTrap;
    thVtbl[1] = (void*)K_GetName; thVtbl[75] = (void*)K_IsAlive; heroVtbl[36] = (void*)H_Received;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_Start; gameVtbl[39] = (void*)G_Answer; gameVtbl[70] = (void*)G_GetHero;
    gameVtbl[81] = (void*)G_Nearest; gameVtbl[114] = (void*)G_YesNo; gameVtbl[121] = (void*)G_GiveObject; gameVtbl[264] = (void*)G_GetHealth;
    gameVtbl[281] = (void*)G_Deactivate; gameVtbl[288] = (void*)G_QuestCompleted; gameVtbl[295] = (void*)G_GiveCard; gameVtbl[296] = (void*)G_Objective;
    gameVtbl[360] = (void*)G_SetInfo; gameVtbl[361] = (void*)G_ClearInfo; gameVtbl[370] = (void*)G_StartMovie; gameVtbl[376] = (void*)G_Pause;
    gameVtbl[379] = (void*)G_PauseEntities; gameVtbl[386] = (void*)G_Cutscene; gameVtbl[394] = (void*)G_Controlled; gameVtbl[655] = (void*)G_QuestName;
    gameVtbl[737] = (void*)G_Tavern;
    int k = 0;
    //    term      alive any  all  start dist ans  speak perf name ctl  cut  tav  inv  end  flags
    Run1("1",       "1", "",   "",  "",  "",  "",  "",   "",  "",  "",  "",  "",  "",  "",  0, ++k);   // KidIntro alive, terminating
    Run1("",        "0", "",   "",  "",  "",  "",  "",   "",  "",  "",  "",  "",  "",  "",  1, ++k);   // script already ended
    Run1("01",      "0", "0",  "0", "",  "",  "",  "",   "",  "",  "",  "",  "",  "",  "",  0, ++k);   // no dolls: the receive test, terminating
    Run1("0000000000000000000","0","1","1","1","0001","0003","11","101","","","","","","",0, ++k);   // all dolls: swap accepted (answer 2 = yes)
    Run1("000000000000000000","1","1","1","0","1","2","1",  "",  "",  "",  "",  "",  "",  "",  4, ++k);   // swap refused (answer 1 = no), seh_me not held
    Run1("0000000000000000000000001","0","1","01","11","011","","111","1","1","011","01","01","","",0, ++k);   // the doll kid intro, then terminating
    Run1("00000000000000000000000000","0","10","00","1","1","","0","","0","","","","1","01",2, ++k);   // kid already done: received a doll, ended from outside
    Run1("000000000001","1","0","000","","","","","","","","","","011","",0, ++k);   // received items, no dolls, terminating
    unsigned long seed = 977;
    for (int r = 0; r < 20000; ++r) {
        static char b[NS][48]; const char* in[NS];
        static const int mod[NS] = { 29, 2, 2, 3, 4, 3, 1, 3, 3, 2, 4, 4, 4, 3, 9 };
        for (int j = 0; j < NS; ++j) {
            for (int i = 0; i < 47; ++i) {
                seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 16);
                b[j][i] = (j == 6) ? (char)('0' + v % 4) : (j == 10 ? ((v % mod[j] == 0) ? '0' : '1') : ((v % mod[j] == 0) ? '1' : '0'));
            }
            b[j][47] = 0; in[j] = b[j];
        }
        seed = seed * 1103515245ul + 12345ul;
        Run(in, (int)((seed >> 12) % 8), 1000 + r);
    }
    printf("model paths: swap %d, refused %d, kid arm %d (second lookup %d, card %d), doll received %d, seh_me release %d, seh_KidIntro release %d, ended from outside %d\n", cYes, cNo, cKid, cKidName, cKidCard, cRecv, cRelMe, cRelKid, cOutside);
    if (failures) { printf("00e97070_TEST FAIL %d\n", failures); return 1; }
    printf("00e97070_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}