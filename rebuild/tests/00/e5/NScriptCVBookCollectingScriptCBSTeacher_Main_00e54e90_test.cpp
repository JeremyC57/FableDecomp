// Behaviour test for 0x00E54E90 NScript::CV_BookCollectingScript::CBS_Teacher::Main() (links the REAL row object: the row body, the expanded
// and called movie ctor / dtor COMDATs, the expanded ~CScriptThing of the GetScriptThing temporaries, the pause expansion and the seh_me member's
// virtual calls all run as compiled). No class models: every external the row object names is bound with /alternatename -- the calls the row makes
// to extern "C" __fastcall stubs (thiscall-compatible, unused edx), every vtable slot and unreached COMDAT reference to a trap. Game (+0x04) slots
// 7, 8, 70, 184, 264, 288, 295, 296, 360, 361, 370, 379, 477, 647, 655, 719, Me's slot 27 and seh_me's slots 12 / 13 / 26 through fake vtables;
// things carry real counted-pointer info blocks {RefCount, DeleteFunc, Data} that the row releases itself. ParentClass (+0x14) and PMasterData
// (+0x18) are raw buffers at the header offsets; frames script the hero-dolls flag and the donated-book count.
// Reference model: Model() below, written from the script's statements; every run is compared event by event (with the destruction order of the
// live locals on each early return) plus the final state of every field the row writes and the release of every string and thing.
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
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@BODtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@BOCtor@8")
#pragma comment(linker, "/alternatename:??1CBaseClass@@UAE@XZ=@BaseClassDtor@8")
#pragma comment(linker, "/alternatename:??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ=@MovieCPDtor@8")
#pragma comment(linker, "/alternatename:?IsAlive@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetAngleXY@CScriptThing@@UBEMXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?LookForBook@CBS_Teacher@CV_BookCollectingScript@NScript@@AAEXXZ=@LookForBook@8")
#pragma comment(linker, "/alternatename:?AskForBook@CBS_Teacher@CV_BookCollectingScript@NScript@@AAE_NJVCCharString@@@Z=@AskForBook@16")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@ReleaseStub@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@IsValidStub@8")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:?AddGossip@CV_BookCollectingScript@NScript@@QAEXVCCharString@@000@Z=@AddGossip@24")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:_g_MovieVtbl=??_7CScriptGameResourceObjectMovieBase@@6B@")
#pragma comment(linker, "/alternatename:_g_STVtbl=??_7CScriptThing@@6B@")
#pragma comment(linker, "/alternatename:@CallRow@8=?Main@CBS_Teacher@CV_BookCollectingScript@NScript@@EAEXXZ")
extern "C" char g_MovieVtbl, g_STVtbl;
extern "C" void __fastcall CallRow(void* self, void*);

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static int NextDigit(Script& sc) { char c = sc.s[sc.pos]; if (!c) return 0; ++sc.pos; return c - '0'; }
static Script sTerm, sStart, sTalk, sSpeak, sPerf, sHas, sGive, sDolls, sBooks, sTod;
static const long kTod[7] = { 1200, 845, 846, 1614, 1615, 1616, 844 };

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 20000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static void Ws(const char* s) { while (*s) W(*s++); }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x60]; double a; } self;                 // CBS_Teacher (0x44)
static union { unsigned char b[0xD0]; double a; } parent;               // CV_BookCollectingScript (0xC8)
static union { unsigned char b[0x120]; double a; } master;              // CQ_SunnyvaleMasterData
static void* gameVtbl[800]; static void* gameObj[1];
static void* meVtbl[80]; static void* sehVtbl[40];
static void* heroVtbl[1]; static void* hero[3];
static const char* bookDef[5] = { "BOOK_A", "BOOK_B", "BOOK_C", "BOOK_D", "BOOK_E" };   // vector<CCharString> elements (PStringData)
struct Info { long RefCount; void* DeleteFunc; void* Data; };
static Info meInfo; static int liveInfos, deletes; static long nextData = 0x7000;
extern "C" void __fastcall DelFunc(void* data) { ++deletes; Ev('X'); (void)data; }
static Info* NewInfo() { Info* i = (Info*)malloc(sizeof(Info)); i->RefCount = 1; i->DeleteFunc = (void*)DelFunc; i->Data = (void*)(nextData += 0x10); ++liveInfos; return i; }
void __cdecl operator delete(void* p) { if (p != &meInfo) { free(p); --liveInfos; } }
static void FillThing(void** t) { Info* i = NewInfo(); t[0] = &g_STVtbl; t[1] = i->Data; t[2] = i; }
static void** Me() { return (void**)(self.b + 8); }
static void** Seh() { return (void**)(self.b + 0x34); }
#define P_LONG(o) (*(long*)(parent.b + (o)))
#define P_BOOL(o) (*(bool*)(parent.b + (o)))
#define DOLLS (*(bool*)(master.b + 0xF4))
#define LADYGREY (*(bool*)(master.b + 0xD6))

// strings: one text pointer, logged by a letter per literal
static int liveStrings;
static char Lit(const char* t)
{
    static const char* k[] = { "SCRIPT_NAME_HERO", "", "Book Collection - Need Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_NEED", "VILLAGE_BOWERSTONE_SLUMS",
        "FACTION_VILLAGERS", "Book Collection - Given Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_GIVEN", "OBJECT_QUEST_CARD_BOOK_COLLECTION",
        "TEXT_QUEST_BOOK_COLLECTION_OBJECTIVE_01", "TEXT_QST_B16_BOOK_REFUSE_AGAIN", "QUEST_BOOK_COLLECTION" };
    static const char l[] = "h0NnvfGgcorQ";
    if (!t) return '?';
    for (int i = 0; i < 12; ++i) if (!strcmp(t, k[i])) return l[i];
    return '?';
}
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; check(len == -1, "string length", 0); Ev('s'); Ev(Lit(text)); return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; Ev('~'); Ev(Lit(*s)); *s = 0; }
static void CalleeDestroys(const char* t) { --liveStrings; Ev('~'); Ev(Lit(t)); }

extern "C" void __fastcall BaseClassDtor(void* t, void*) { (void)t; }
extern "C" bool __fastcall M_TalkedTo(void* t, void*, const char* const* who) { check(t == Me() && !strcmp(*who, "SCRIPT_NAME_HERO"), "MsgIsTalkedToBy args", 0); bool r = Next(sTalk); Ev(r ? 'A' : 'a'); return r; }

// movie {vptr, IntelligentPointer, PImp, Info}; seh_me member {vptr, IntelligentPointer, Data, Info}
extern "C" void* __fastcall BOCtor(void** o, void*) { o[1] = 0; Ev('o'); return o; }
extern "C" void __fastcall BODtor(void** o, void*) { (void)o; Ev('O'); }
extern "C" void __fastcall MovieCPDtor(void** cp, void*) { check(cp[0] == 0 && cp[1] == 0, "movie counted pointer empty", 0); Ev('c'); }
static void ReleaseInfo(Info* i) { if (i && --i->RefCount == 0) { ((void (__fastcall*)(void*))i->DeleteFunc)(i->Data); operator delete(i); } }
extern "C" bool __fastcall IsValidStub(void* const* h, void*) { check(h == Seh(), "IsValid on seh_me", 0); bool r = h[2] != 0; Ev('v'); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall ReleaseStub(void** h, void*) { check(h == Seh() && h[3], "Release on a held seh_me", 0); Ev('-'); ReleaseInfo((Info*)h[3]); h[2] = h[3] = 0; }
extern "C" void* __fastcall S_GetScriptThing(void* const* h, void*, void** ret) { check(h == Seh(), "GetScriptThing on seh_me", 0); FillThing(ret); Ev('R'); return ret; }
extern "C" bool __fastcall S_IsPerf(void* h, void*) { check(h == Seh(), "performing on seh_me", 0); bool r = Next(sPerf); Ev(r ? 'Q' : 'q'); return r; }
static char TextLetter(const char* t, long* sel)
{
    static const char* keys[7] = { "TEXT_QST_B16_INTRO_B", "TEXT_QST_B16_INTRO", "TEXT_QST_B16_BOOK_REQUEST_AGAIN", "TEXT_QST_B16_BOOK_LOST",
        "TEXT_QST_B16_BOOK_LOOK", "TEXT_QST_B16_BOOK_WRONG_TIME", "TEXT_QST_B16_COMPLETE" };
    static const char let[8] = "bialkwe"; static const long sels[7] = { 0, 0, 2, 0, 2, 2, 2 };
    for (int i = 0; i < 7; ++i) if (!strcmp(t, keys[i])) { *sel = sels[i]; return let[i]; }
    *sel = -1; return '?';
}
extern "C" void __fastcall S_Speak(void* h, void*, void* const* to, const char* text, long sel, int b1, int b2, int b3)
{
    long want_sel; char c = TextLetter(text, &want_sel);
    check(h == Seh() && to == (void* const*)hero && sel == want_sel && (b1 & 0xFF) == 0 && (b2 & 0xFF) == 1 && (b3 & 0xFF) == 0, "speak args", 0); Ev('K'); Ev(c);
}

// the CV_BookCollectingScript / CBS_Teacher helpers
extern "C" void __fastcall AddGossip(void* p, void*, const char* a, const char* b, const char* c, const char* d)
{
    check(p == parent.b, "AddGossip on ParentClass", 0); Ev('A'); CalleeDestroys(a); CalleeDestroys(b); CalleeDestroys(c); CalleeDestroys(d);
}
static long lookIndex;
extern "C" void __fastcall LookForBook(void* s, void*) { check(s == self.b, "LookForBook on this", 0); Ev('L'); P_LONG(0x8C) = lookIndex; }
extern "C" bool __fastcall AskForBook(void* s, void*, long book, const char* refusal)
{
    check(s == self.b && book == P_LONG(0x8C), "AskForBook(this, LastBookRequested)", 0); bool r = Next(sGive); Ev('k'); Ev(r ? '1' : '0');
    check(!strcmp(refusal, "TEXT_QST_B16_BOOK_REFUSE_AGAIN"), "refusal text", 0); CalleeDestroys(refusal);
    if (r) P_LONG(0x84) += 1;                                            // a handed-over book is donated
    return r;
}

// Game
static bool T_(void* s) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall IsTerm(void* s, void*) { return T_(s); }
static void G(void* g, const char* what) { check(g == gameObj, what, 0); }
static void Frame(bool& dolls, long& books, Script& d, Script& b) { dolls = Next(d); if (Next(b)) books += 1; }
extern "C" void __fastcall G_NewScriptFrame(void* g, void*) { G(g, "frame this"); Ev('F'); long bk = P_LONG(0x84); bool dl; Frame(dl, bk, sDolls, sBooks); DOLLS = dl; P_LONG(0x84) = bk; }
extern "C" bool __fastcall G_Start(void* g, void*, void* const* thing, void** h, long prio)
{
    G(g, "start this"); check(thing == (void* const*)Me() && h == Seh() && prio == 4, "start scripting Me into seh_me at priority 4", 0);
    bool r = Next(sStart); Ev(r ? 'S' : 'n');
    if (r) { check(!h[2] && !h[3], "started into an empty handle", 0); Info* i = NewInfo(); h[2] = i->Data; h[3] = i; }
    return r;
}
extern "C" void* __fastcall G_GetHero(void* g, void*) { G(g, "hero this"); Ev('Y'); return hero; }
extern "C" bool __fastcall G_HasObject(void* g, void*, const char* const* obj, void* const* who)
{
    G(g, "possession this"); check(obj == &bookDef[P_LONG(0x8C)] && who == (void* const*)hero, "IsObjectInThingsPossession(BookDef[LastBookRequested], hero)", 0);
    bool r = Next(sHas); Ev('x'); Ev(r ? '1' : '0'); return r;
}
extern "C" float __fastcall G_GetHealth(void* g, void*, void* const* t)
{
    G(g, "health this"); check(t[0] == &g_STVtbl, "health of the script thing", 0);
    bool alive = Next(sSpeak); Ev(alive ? 'U' : 'u'); return alive ? 2.0f : 0.0f;
}
extern "C" void __fastcall G_QuestCompleted(void* g, void*, const char* const* q, int a, int b, int c) { G(g, "completed this"); check(Lit(*q) == 'Q' && !(a & 0xFF) && !(b & 0xFF) && !(c & 0xFF), "SetQuestAsCompleted args", 0); Ev('E'); }
extern "C" void __fastcall G_GiveCard(void* g, void*, const char* const* card, const char* const* q, int f) { G(g, "card this"); check(Lit(*card) == 'c' && Lit(*q) == 'Q' && !(f & 0xFF), "GiveHeroQuestCardDirectly args", 0); Ev('C'); }
extern "C" void __fastcall G_Objective(void* g, void*, const char* const* q, const char* const* o, const char* const* a, const char* const* b)
{
    G(g, "objective this"); check(Lit(*q) == 'Q' && Lit(*o) == 'o' && Lit(*a) == '0' && Lit(*b) == '0', "SetQuestCardObjective args", 0); Ev('j');
}
extern "C" void __fastcall G_SetInfo(void* g, void*, void* const* t, int a, int b, int c) { G(g, "set info this"); check(t == (void* const*)Me() && !(a & 0xFF) && !(b & 0xFF) && (c & 0xFF) == 1, "SetThingHasInformation(Me, false, false, true)", 0); Ev('i'); }
extern "C" void __fastcall G_ClearInfo(void* g, void*, void* const* t) { G(g, "clear info this"); check(t == (void* const*)Me(), "ClearThingHasInformation(Me)", 0); Ev('c'); Ev('I'); }
extern "C" void __fastcall G_StartMovie(void* g, void*, const char* const* name, void* const* movie)
{
    G(g, "movie this"); check(!strcmp(*name, "") && movie[0] == (void*)&g_MovieVtbl && !movie[2] && !movie[3], "movie sequence args", 0); Ev('m');
}
static int pauseDepth;
extern "C" void __fastcall G_PauseEntities(void* g, void*, int on) { G(g, "pause entities this"); pauseDepth += (on & 0xFF) ? 1 : -1; Ev((on & 0xFF) ? 'Z' : 'z'); }
extern "C" void __fastcall G_Persistent(void* g, void*, void* const* t, int on) { G(g, "persistent this"); check(t == (void* const*)Me() && (on & 0xFF) == 1, "SetThingPersistent(Me, true)", 0); Ev('1'); }
extern "C" long __fastcall G_GetTimeOfDay(void* g, void*) { G(g, "time this"); Ev('d'); return kTod[NextDigit(sTod)]; }
extern "C" void* __fastcall G_QuestName(void* g, void*, const char** ret) { G(g, "quest name this"); *ret = "QUEST_BOOK_COLLECTION"; ++liveStrings; Ev('q'); return ret; }
extern "C" void __fastcall G_RemoveRumour(void* g, void*, const char* const* cat) { G(g, "rumour this"); check(Lit(*cat) == 'N', "RemoveRumourCategory(Need Books)", 0); Ev('r'); }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mStart, mTalk, mSpeak, mPerf, mHas, mGive, mDolls, mBooks, mTod;
struct State { long books, gossip, last; bool info, dolls, lady, intro, held; } m;
static long accepted;
static char live[16][4]; static int nlive;
static void Push(const char* d) { strcpy(live[nlive++], d); }
static void Pop() { --nlive; Ws(live[nlive]); }
static bool T() { bool r = Next(mTerm); W(r ? 'T' : 't'); if (r) while (nlive) Pop(); return r; }
static void F() { W('F'); Frame(m.dolls, m.books, mDolls, mBooks); }
static long Tod() { W('d'); return kTod[NextDigit(mTod)]; }
static bool Say(char text)
{
    W('R'); bool sp = Next(mSpeak); W(sp ? 'U' : 'u'); W('X');
    if (!sp) return false;
    Ws("YK"); W(text);
    for (;;) { bool pf = Next(mPerf); W(pf ? 'Q' : 'q'); if (!pf) break; F(); if (T()) return true; }
    return T();
}
static void Release() { W('-'); W('X'); m.held = false; }
static bool MSeh()
{
    W('v'); W(m.held ? '1' : '0'); if (m.held) Release();
    for (;;) { bool r = Next(mStart); W(r ? 'S' : 'n'); if (r) { m.held = true; break; } F(); if (T()) return true; }
    return T();
}
static bool TalkBlockOpen() { W('o'); Push("cO"); Ws("s0m~0"); W('Z'); Push("z"); return false; }
static void TalkBlockClose() { W('v'); W(m.held ? '1' : '0'); if (m.held) Release(); Pop(); Pop(); }
static void Model()
{
    W('1');
    if (m.books < 4 && m.gossip == 0) { if (T()) return; Ws("sfsvsnsN" "A~N~n~v~f"); m.gossip = 1; }
    else if (m.books > 3 && m.gossip == 1) { if (T()) return; Ws("sNr~N"); m.gossip = 2; }
    else if (m.books > 9 && m.gossip < 3) { if (T()) return; Ws("sfsvsgsG" "A~G~g~v~f"); m.gossip = 3; }
    while (m.books < accepted) {
        F(); if (T()) return;
        bool c1;
        if (m.info) { c1 = Tod() > 1615 || Tod() < 845; } else c1 = false;
        if (c1) { if (T()) return; Ws("cI"); m.info = false; }
        else if (!m.info && Tod() > 845 && Tod() < 1615) { if (T()) return; W('i'); m.info = true; }
        if (m.dolls) { if (T()) return; while (m.dolls) { F(); if (T()) return; } if (T()) return; m.info = false; }
        Ws("sh"); bool talk = Next(mTalk); W(talk ? 'A' : 'a'); Ws("~h");
        if (talk) {
            if (T()) return;
            TalkBlockOpen();
            if (MSeh()) return;
            if (!m.intro) {
                if (T()) return;
                if (m.lady) { if (T()) return; if (Say('b')) return; } else { if (T()) return; if (Say('i')) return; }
                Ws("sc" "q" "C" "~Q~c");
                Ws("s0s0so" "q" "j" "~Q~o~0~0");
                m.intro = true;
            } else if (Tod() >= 845 && Tod() <= 1615) {
                if (m.last >= 0) {
                    if (T()) return; if (Say('a')) return;
                    W('Y'); bool has = Next(mHas); W('x'); W(has ? '1' : '0');
                    if (has) {
                        if (T()) return;
                        Ws("sr"); bool given = Next(mGive); W('k'); W(given ? '1' : '0'); Ws("~r"); if (given) m.books += 1;
                        if (!given) { if (T()) return; W('L'); m.last = lookIndex; }
                        else { if (T()) return; m.last = -1; }
                    } else { if (T()) return; if (Say('l')) return; W('L'); m.last = lookIndex; }
                } else { if (T()) return; if (Say('k')) return; W('L'); m.last = lookIndex; }
            } else { if (T()) return; if (Say('w')) return; }
            TalkBlockClose();
        }
    }
    if (T()) return;
    Ws("cI");
    Ws("q" "E" "~Q");
    if (T()) return;
    for (;;) {
        if (m.dolls) { if (T()) return; while (m.dolls) { F(); if (T()) return; } if (T()) return; }
        else {
            Ws("sh"); bool talk = Next(mTalk); W(talk ? 'A' : 'a'); Ws("~h");
            if (talk) {
                if (T()) return;
                TalkBlockOpen();
                if (MSeh()) return;
                if (Say('e')) return;
                TalkBlockClose();
            }
        }
        F(); if (T()) return;
    }
}

static void Run(const char* term, const char* st, const char* talk, const char* speak, const char* perf, const char* has, const char* give,
                const char* dolls, const char* books, const char* tod, long books0, long gossip0, long last0, long acc, int flags, int k)
{
    memset(self.b, 0xEE, sizeof self.b); memset(parent.b, 0xDD, sizeof parent.b); memset(master.b, 0xCC, sizeof master.b);
    *(void**)(self.b + 0x00) = (void*)0x5E1F; *(void**)(self.b + 0x04) = gameObj;
    Me()[0] = meVtbl; Me()[1] = (void*)0x4D45; Me()[2] = &meInfo;
    *(void**)(self.b + 0x14) = parent.b; *(void**)(self.b + 0x18) = master.b;
    *(long*)(self.b + 0x28) = acc;
    Seh()[0] = sehVtbl; Seh()[1] = 0; Seh()[2] = 0; Seh()[3] = 0;
    if (flags & 16) { Info* i = NewInfo(); --liveInfos; Seh()[2] = i->Data; Seh()[3] = i; }   // held from an earlier run of the thread
    P_BOOL(0x78) = (flags & 1) != 0; P_LONG(0x7C) = gossip0; P_BOOL(0x80) = (flags & 2) != 0; P_LONG(0x84) = books0; P_LONG(0x8C) = last0;
    *(const char***)(parent.b + 0x94) = bookDef; *(const char***)(parent.b + 0x98) = bookDef + 5; *(const char***)(parent.b + 0x9C) = bookDef + 5;
    DOLLS = (flags & 4) != 0; LADYGREY = (flags & 8) != 0;
    lookIndex = (k % 5);
    Script* ss[10] = { &sTerm, &sStart, &sTalk, &sSpeak, &sPerf, &sHas, &sGive, &sDolls, &sBooks, &sTod };
    const char* tx[10] = { term, st, talk, speak, perf, has, give, dolls, books, tod };
    bool df[10] = { true, true, false, false, false, false, false, false, false, false };
    for (int i = 0; i < 10; ++i) { ss[i]->s = tx[i]; ss[i]->pos = 0; ss[i]->dflt = df[i]; }
    mTerm = sTerm; mStart = sStart; mTalk = sTalk; mSpeak = sSpeak; mPerf = sPerf; mHas = sHas; mGive = sGive; mDolls = sDolls; mBooks = sBooks; mTod = sTod;
    m.books = books0; m.gossip = gossip0; m.last = last0; m.info = (flags & 2) != 0; m.dolls = (flags & 4) != 0; m.lady = (flags & 8) != 0;
    m.intro = (flags & 1) != 0; m.held = (flags & 16) != 0; accepted = acc;
    ngot = nwant = 0; nlive = 0; liveStrings = liveInfos = deletes = pauseDepth = 0;
    if (flags & 16) liveInfos = 1;
    meInfo.RefCount = 1; meInfo.DeleteFunc = (void*)VirtualTrap; meInfo.Data = Me()[1];
    Model();
    CallRow(self.b, 0);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) printf("  got  %.700s\n  want %.700s\n", got, want);
    }
    check(P_LONG(0x7C) == m.gossip && P_LONG(0x84) == m.books && P_LONG(0x8C) == m.last && P_BOOL(0x80) == m.info && P_BOOL(0x78) == m.intro, "parent fields", k);
    check(parent.b[0x79] == 0xDD && parent.b[0x7A] == 0xDD && parent.b[0x88] == 0xDD && parent.b[0x90] == 0xDD, "no other parent store", k);
    check(meInfo.RefCount == 1 && Me()[0] == meVtbl && *(long*)(self.b + 0x28) == acc && self.b[0x44] == 0xEE, "Me / this untouched", k);
    check((Seh()[3] != 0) == m.held, "seh_me held state", k);
    if (Seh()[3]) { ReleaseInfo((Info*)Seh()[3]); Seh()[3] = 0; }
    check(liveStrings == 0 && liveInfos == 0 && pauseDepth == 0, "everything released", k);
}

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; heroVtbl[0] = (void*)VirtualTrap; hero[0] = heroVtbl; hero[1] = (void*)0x4845; hero[2] = 0;
    for (int i = 0; i < 800; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) meVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 40; ++i) sehVtbl[i] = (void*)VirtualTrap;
    meVtbl[27] = (void*)M_TalkedTo;
    sehVtbl[12] = (void*)S_GetScriptThing; sehVtbl[13] = (void*)S_Speak; sehVtbl[26] = (void*)S_IsPerf;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_Start; gameVtbl[70] = (void*)G_GetHero; gameVtbl[184] = (void*)G_HasObject;
    gameVtbl[264] = (void*)G_GetHealth; gameVtbl[288] = (void*)G_QuestCompleted; gameVtbl[295] = (void*)G_GiveCard; gameVtbl[296] = (void*)G_Objective;
    gameVtbl[360] = (void*)G_SetInfo; gameVtbl[361] = (void*)G_ClearInfo; gameVtbl[370] = (void*)G_StartMovie; gameVtbl[379] = (void*)G_PauseEntities;
    gameVtbl[477] = (void*)G_Persistent; gameVtbl[647] = (void*)G_GetTimeOfDay; gameVtbl[655] = (void*)G_QuestName; gameVtbl[719] = (void*)G_RemoveRumour;
    int k = 0;
    //   term       start talk  speak perf  has   give  dolls books tod    books0 goss last acc flags
    Run("1",        "",   "",   "",   "",   "",   "",   "",   "",   "",    0, 0, -1, 10, 0, ++k);
    Run("01",       "",   "",   "",   "",   "",   "",   "",   "",   "",    5, 1, -1, 10, 0, ++k);
    Run("01",       "",   "",   "",   "",   "",   "",   "",   "",   "",    10, 2, -1, 10, 0, ++k);
    Run("001",      "",   "0",  "",   "",   "",   "",   "",   "",   "0",   0, 3, -1, 2, 2, ++k);
    Run("00000001", "1",  "1",  "11", "1",  "",   "",   "",   "",   "00",  0, 3, -1, 2, 0, ++k);
    Run("000000000001","1","1", "1",  "",   "1",  "1",  "",   "",   "0",   1, 3, 2, 2, 1, ++k);
    Run("0000000000000001","1","01","1","","0","",  "001", "1",  "6566", 1, 3, -1, 2, 1 | 2 | 16, ++k);
    Run("000000000000000000001","1","11","111","1","","","","","00", 0, 3, -1, 1, 8 | 16, ++k);
    unsigned long seed = 211;
    for (int r = 0; r < 20000; ++r) {
        static char b[10][48];
        int mod[10] = { 23, 3, 2, 2, 3, 2, 2, 3, 4, 1 };
        for (int j = 0; j < 10; ++j) {
            for (int i = 0; i < 47; ++i) { seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 16); b[j][i] = (j == 9) ? (char)('0' + v % 7) : ((v % mod[j] == 0) ? '1' : '0'); }
            b[j][47] = 0;
        }
        seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 8);
        Run(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], (long)(v % 12), (long)((v >> 4) % 4), (long)((v >> 6) % 6) - 1, (long)((v >> 9) % 12), (int)((v >> 13) % 32), 1000 + r);
    }
    if (failures) { printf("00e54e90_TEST FAIL %d\n", failures); return 1; }
    printf("00e54e90_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}