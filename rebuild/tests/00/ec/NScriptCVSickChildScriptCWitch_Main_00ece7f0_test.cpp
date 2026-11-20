// Behaviour test for 0x00ECE7F0 NScript::CV_SickChildScript::CWitch::Main() (links the REAL row object: the row body, the CIsThingAlive
// predicate built for SetPredicate, the expanded and called movie ctor / dtor COMDATs, the expanded ~CScriptThing of the GetScriptThing
// temporaries, the expanded CDefString conversions (GetString 0x00415D70 bound to a stub), the pause expansion and the virtual calls on
// ParentClass.seh_Witch all run as compiled). No class models: every external the row object names is bound with /alternatename -- the calls the
// row makes to extern "C" __fastcall stubs (thiscall-compatible, unused edx), every vtable slot and unreached COMDAT reference to a trap. Game
// (+0x04) slots 7, 8, 39, 70, 114, 125, 184, 264, 296, 361, 370, 379, 655, Me's slots 21 / 27 / 41 / 42 and seh_Witch's slots 12 / 13 / 26
// through fake vtables; things carry real counted-pointer info blocks {RefCount, DeleteFunc, Data} that the row releases itself. CWitch's
// IntroDone (+0x1C) / ThingsForPotionGot (+0x20), ParentClass (seh_Witch +0x58) and pGlobalScriptDef (IngredientObject +0x738, EndPotion
// +0x744) are raw buffers at the header offsets.
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
#pragma comment(linker, "/alternatename:?GetString@CDefString@@QBE?AVCCharString@@XZ=@DefGetString@12")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CIsThingAlive@NScript@@UAE@XZ=@FilterDtor@8")
#pragma comment(linker, "/alternatename:??BCCharString@@QBEPBDXZ=@StrToChars@8")
#pragma comment(linker, "/alternatename:??4CCharString@@QAEABV0@PBD@Z=@StrAssign@12")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@XZ=@StrDefault@8")
#pragma comment(linker, "/alternatename:?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A=_g_ScriptDefPtr")
#pragma comment(linker, "/alternatename:?PlayCutscene@CV_SickChildScript@NScript@@QAEXVCCharString@@0@Z=@PlayCutscene@16")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@ReleaseStub@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@IsValidStub@8")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@SetPred@12")
#pragma comment(linker, "/alternatename:_g_MovieVtbl=??_7CScriptGameResourceObjectMovieBase@@6B@")
#pragma comment(linker, "/alternatename:_g_STVtbl=??_7CScriptThing@@6B@")
#pragma comment(linker, "/alternatename:_g_FilterVtbl=??_7CIsThingAlive@NScript@@6B@")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:@CallRow@8=?Main@CWitch@CV_SickChildScript@NScript@@EAEXXZ")
extern "C" char g_MovieVtbl, g_STVtbl, g_FilterVtbl;
extern "C" void* g_ScriptDefPtr[2] = { 0, 0 };
extern "C" void __fastcall CallRow(void* self, void*);

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static int NextDigit(Script& sc) { char c = sc.s[sc.pos]; if (!c) return 0; ++sc.pos; return c - '0'; }
static Script sTerm, sStart, sTalk, sSpeak, sPerf, sHas, sAns, sHit, sAny, sHeal;
static const long kAns[3] = { -1, 1, 0 };

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 24000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static void Ws(const char* s) { while (*s) W(*s++); }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x40]; double a; } self;                 // CWitch (0x24)
static union { unsigned char b[0x80]; double a; } parent;               // CV_SickChildScript (seh_Witch +0x58)
static union { unsigned char b[0x760]; double a; } scriptDef;           // CScriptDef: IngredientObject +0x738, EndPotion +0x744
static void* gameVtbl[800]; static void* gameObj[1];
static void* meVtbl[80]; static void* sehVtbl[40];
static void* heroVtbl[1]; static void* hero[3];
struct Info { long RefCount; void* DeleteFunc; void* Data; };
static Info meInfo; static int liveInfos, deletes; static long nextData = 0x7000;
extern "C" void __fastcall DelFunc(void* data) { ++deletes; Ev('X'); (void)data; }
static Info* NewInfo() { Info* i = (Info*)malloc(sizeof(Info)); i->RefCount = 1; i->DeleteFunc = (void*)DelFunc; i->Data = (void*)(nextData += 0x10); ++liveInfos; return i; }
void __cdecl operator delete(void* p) { if (p != &meInfo) { free(p); --liveInfos; } }
static void FillThing(void** t) { Info* i = NewInfo(); t[0] = &g_STVtbl; t[1] = i->Data; t[2] = i; }
static void** Me() { return (void**)(self.b + 8); }
static void** Seh() { return (void**)(parent.b + 0x58); }
#define THINGS (*(long*)(self.b + 0x20))
#define INTRODONE (*(bool*)(self.b + 0x1C))

// strings: one text pointer, logged by a letter per literal
static int liveStrings;
static char Lit(const char* t)
{
    static const char* k[] = { "SCRIPT_NAME_HERO", "", "CS_SICK_POTION", "CS_SICK_WITCH", "NULL", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_YES",
        "TEXT_QST_B10_QUESTION_GIVE_MUSHROOM", "TEXT_QUEST_SICK_CHILD_OBJECTIVE_02", "TEXT_QUEST_SICK_CHILD_OBJECTIVE_03", "QUEST_SICK_CHILD",
        "OBJECT_MUSHROOM", "OBJECT_POTION", "TEXT_QST_B10_WITCH_ONHIT_10", "TEXT_QST_B10_WITCH_ONHIT_20", "TEXT_QST_B10_WITCH_ONHIT_30" };
    static const char l[] = "h0pwNnyqbdQiP123";
    if (!t) return '_';
    for (int i = 0; i < 16; ++i) if (!strcmp(t, k[i])) return l[i];
    return '?';
}
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; check(len == -1, "string length", 0); Ev('s'); Ev(Lit(text)); return s; }
extern "C" void* __fastcall StrDefault(const char** s, void*) { *s = 0; ++liveStrings; Ev('e'); return s; }
extern "C" void* __fastcall StrAssign(const char** s, void*, const char* text) { *s = text; Ev('='); Ev(Lit(text)); return s; }
extern "C" const char* __fastcall StrToChars(const char* const* s, void*) { Ev('^'); return *s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; Ev('~'); Ev(Lit(*s)); *s = 0; }
static void CalleeDestroys(const char* t) { --liveStrings; Ev('~'); Ev(Lit(t)); }
extern "C" void* __fastcall DefGetString(void* def, void*, const char** ret)
{
    check(def == scriptDef.b + 0x738 || def == scriptDef.b + 0x744, "GetString on IngredientObject / EndPotion", 0);
    *ret = def == scriptDef.b + 0x738 ? "OBJECT_MUSHROOM" : "OBJECT_POTION"; ++liveStrings; Ev('G'); Ev(Lit(*ret)); return ret;
}

// things / predicate
extern "C" void* __fastcall ThingCopy(void** t, void*, void* const* o) { t[0] = o[0]; t[1] = o[1]; t[2] = o[2]; if (t[2]) ++((Info*)t[2])->RefCount; Ev('k'); return t; }
extern "C" void __fastcall BaseClassDtor(void* t, void*) { (void)t; }
extern "C" void __fastcall SetPred(void* s, void*, void* const* f)
{
    check(s == self.b && f[0] == (void*)&g_FilterVtbl && f[1] == meVtbl && f[2] == Me()[1] && f[3] == (void*)&meInfo, "predicate: CIsThingAlive(Me)", 0);
    Ev('P');
}
extern "C" void __fastcall FilterDtor(void** f, void*) { if (f[3]) --((Info*)f[3])->RefCount; Ev('p'); }

// Me's own vtable: 21 MsgIsHitBy, 27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom
extern "C" bool __fastcall M_TalkedTo(void* t, void*, const char* const* who) { check(t == Me() && !strcmp(*who, "SCRIPT_NAME_HERO"), "MsgIsTalkedToBy args", 0); bool r = Next(sTalk); Ev(r ? 'A' : 'a'); return r; }
extern "C" bool __fastcall M_HitBy(void* t, void*, const char* const* who) { check(t == Me() && !strcmp(*who, "SCRIPT_NAME_HERO"), "MsgIsHitBy args", 0); bool r = Next(sHit); Ev(r ? 'B' : 'b'); return r; }
extern "C" bool __fastcall M_Any(void* t, void*, const char* const* who) { check(t == Me() && !strcmp(*who, "SCRIPT_NAME_HERO"), "special args", 0); bool r = Next(sAny); Ev(r ? 'D' : 'd'); return r; }
extern "C" bool __fastcall M_Heal(void* t, void*, long ab, const char* const* who) { check(t == Me() && ab == 14 && !strcmp(*who, "SCRIPT_NAME_HERO"), "heal-life args", 0); bool r = Next(sHeal); Ev(r ? 'H' : 'j'); return r; }

// movie {vptr, IntelligentPointer, PImp, Info}; seh_Witch (ParentClass +0x58) {vptr, IntelligentPointer, Data, Info}
extern "C" void* __fastcall BOCtor(void** o, void*) { o[1] = 0; Ev('o'); return o; }
extern "C" void __fastcall BODtor(void** o, void*) { (void)o; Ev('O'); }
extern "C" void __fastcall MovieCPDtor(void** cp, void*) { check(cp[0] == 0 && cp[1] == 0, "movie counted pointer empty", 0); Ev('c'); }
static void ReleaseInfo(Info* i) { if (i && --i->RefCount == 0) { ((void (__fastcall*)(void*))i->DeleteFunc)(i->Data); operator delete(i); } }
extern "C" bool __fastcall IsValidStub(void* const* h, void*) { check(h == Seh(), "IsValid on seh_Witch", 0); bool r = h[2] != 0; Ev('v'); Ev(r ? '1' : '0'); return r; }
extern "C" void __fastcall ReleaseStub(void** h, void*) { check(h == Seh() && h[3], "Release on a held seh_Witch", 0); Ev('-'); ReleaseInfo((Info*)h[3]); h[2] = h[3] = 0; }
extern "C" void* __fastcall S_GetScriptThing(void* const* h, void*, void** ret) { check(h == Seh(), "GetScriptThing on seh_Witch", 0); FillThing(ret); Ev('R'); return ret; }
extern "C" bool __fastcall S_IsPerf(void* h, void*) { check(h == Seh(), "performing on seh_Witch", 0); bool r = Next(sPerf); Ev(r ? 'Q' : 'q'); return r; }
static char TextLetter(const char* t, long* sel)
{
    static const char* keys[7] = { "TEXT_QST_B10_WITCH_NEED_MORE_CURE_10", "TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10", "TEXT_QST_B10_WITCH_WAITING_FOR_CURE",
        "TEXT_QST_B10_WITCH_GOT_CURE", "TEXT_QST_B10_WITCH_ONHIT_10", "TEXT_QST_B10_WITCH_ONHIT_20", "TEXT_QST_B10_WITCH_ONHIT_30" };
    static const char let[8] = "mgwc123"; static const long sels[7] = { 0, 0, 0, 2, 0, 0, 0 };
    if (t) for (int i = 0; i < 7; ++i) if (!strcmp(t, keys[i])) { *sel = sels[i]; return let[i]; }
    *sel = -1; return '?';
}
extern "C" void __fastcall S_Speak(void* h, void*, void* const* to, const char* text, long sel, int b1, int b2, int b3)
{
    long want_sel; char c = TextLetter(text, &want_sel);
    check(h == Seh() && to == (void* const*)hero && sel == want_sel && (b1 & 0xFF) == 0 && (b2 & 0xFF) == 1 && (b3 & 0xFF) == 0, "speak args", 0); Ev('K'); Ev(c);
}
extern "C" void __fastcall PlayCutscene(void* p, void*, const char* a, const char* b) { check(p == parent.b, "PlayCutscene on ParentClass", 0); Ev('C'); Ev(Lit(a)); Ev(Lit(b)); CalleeDestroys(a); CalleeDestroys(b); }

// Game
static bool T_(void* s) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall IsTerm(void* s, void*) { return T_(s); }
static void G(void* g, const char* what) { check(g == gameObj, what, 0); }
extern "C" void __fastcall G_NewScriptFrame(void* g, void*) { G(g, "frame this"); Ev('F'); }
extern "C" bool __fastcall G_Start(void* g, void*, void* const* thing, void** h, long prio)
{
    G(g, "start this"); check(thing == (void* const*)Me() && h == Seh() && prio == 4, "start scripting Me into seh_Witch at priority 4", 0);
    bool r = Next(sStart); Ev(r ? 'S' : 'n');
    if (r) { check(!h[2] && !h[3], "started into an empty handle", 0); Info* i = NewInfo(); h[2] = i->Data; h[3] = i; }
    return r;
}
extern "C" void* __fastcall G_GetHero(void* g, void*) { G(g, "hero this"); Ev('Y'); return hero; }
extern "C" bool __fastcall G_HasObject(void* g, void*, const char* const* obj, void* const* who)
{
    G(g, "possession this"); check(Lit(*obj) == 'i' && who == (void* const*)hero, "IsObjectInThingsPossession(IngredientObject, hero)", 0);
    bool r = Next(sHas); Ev('x'); Ev(r ? '1' : '0'); return r;
}
extern "C" void __fastcall G_TakeObject(void* g, void*, const char* const* obj) { G(g, "take this"); check(Lit(*obj) == 'i', "TakeObjectFromHero(IngredientObject)", 0); Ev('t'); }
extern "C" float __fastcall G_GetHealth(void* g, void*, void* const* t)
{
    G(g, "health this"); check(t[0] == &g_STVtbl, "health of the script thing", 0);
    bool alive = Next(sSpeak); Ev(alive ? 'U' : 'u'); return alive ? 2.0f : 0.0f;
}
extern "C" void __fastcall G_YesNo(void* g, void*, const char* const* q, const char* const* y, const char* const* n, const char* const* e, int f)
{
    G(g, "question this"); check(Lit(*q) == 'q' && Lit(*y) == 'y' && Lit(*n) == 'n' && Lit(*e) == '0' && (f & 0xFF) == 1, "GiveHeroYesNoQuestion args", 0); Ev('?');
}
extern "C" long __fastcall G_Answer(void* g, void*) { G(g, "answer this"); long a = kAns[NextDigit(sAns)]; Ev('#'); Ev((char)('1' + a)); return a; }
extern "C" void __fastcall G_Objective(void* g, void*, const char* const* q, const char* const* o, const char* const* a, const char* const* b)
{
    G(g, "objective this"); check(Lit(*q) == 'Q' && (Lit(*o) == 'b' || Lit(*o) == 'd') && Lit(*a) == '0' && Lit(*b) == '0', "SetQuestCardObjective args", 0); Ev('J'); Ev(Lit(*o));
}
extern "C" void __fastcall G_ClearInfo(void* g, void*, void* const* t) { G(g, "clear info this"); check(t == (void* const*)Me(), "ClearThingHasInformation(Me)", 0); Ev('I'); }
extern "C" void __fastcall G_StartMovie(void* g, void*, const char* const* name, void* const* movie)
{
    G(g, "movie this"); check(!strcmp(*name, "") && movie[0] == (void*)&g_MovieVtbl && !movie[2] && !movie[3], "movie sequence args", 0); Ev('m');
}
static int pauseDepth;
extern "C" void __fastcall G_PauseEntities(void* g, void*, int on) { G(g, "pause entities this"); pauseDepth += (on & 0xFF) ? 1 : -1; Ev((on & 0xFF) ? 'Z' : 'z'); }
extern "C" void* __fastcall G_QuestName(void* g, void*, const char** ret) { G(g, "quest name this"); *ret = "QUEST_SICK_CHILD"; ++liveStrings; Ev('E'); return ret; }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mStart, mTalk, mSpeak, mPerf, mHas, mAns, mHit, mAny, mHeal;
static long mThings, mTimesHit; static bool mIntro, mHeld;
static int cRel, cIntro, cMore, cPotion, cNo, cWait, cHit[4], cCure;
static char live[16][4]; static int nlive;
static void Push(const char* d) { strcpy(live[nlive++], d); }
static void Pop() { --nlive; Ws(live[nlive]); }
static bool T() { bool r = Next(mTerm); W(r ? 'T' : 't'); if (r) while (nlive) Pop(); return r; }
static void F() { W('F'); }
static bool Say(char text)
{
    W('R'); bool sp = Next(mSpeak); W(sp ? 'U' : 'u'); W('X');
    if (!sp) return false;
    Ws("YK"); W(text);
    for (;;) { bool pf = Next(mPerf); W(pf ? 'Q' : 'q'); if (!pf) break; F(); if (T()) return true; }
    return T();
}
static bool SayText(char lit)                                            // Speak(.., TextToSpeak, ..): the conversion runs after GetHero
{
    W('R'); bool sp = Next(mSpeak); W(sp ? 'U' : 'u'); W('X');
    if (!sp) return false;
    Ws("^YK"); W(lit);
    for (;;) { bool pf = Next(mPerf); W(pf ? 'Q' : 'q'); if (!pf) break; F(); if (T()) return true; }
    return T();
}
static void BlockOpen() { W('o'); Push("cO"); Ws("s0m~0"); W('Z'); Push("z"); }
static void BlockClose() { Pop(); Pop(); }
static bool Hit()
{
    Ws("sh"); bool a = Next(mHit); W(a ? 'B' : 'b');
    if (a) { Ws("~h"); return true; }
    Ws("sh"); bool b = Next(mAny); W(b ? 'D' : 'd');
    if (b) { Ws("sh"); bool c = Next(mHeal); W(c ? 'H' : 'j'); Ws("~h~h~h"); return !c; }
    Ws("~h~h"); return false;
}
static bool HitArm()                                                     // true = returned
{
    if (T()) return true;
    ++mTimesHit;
    W('e'); Push("~_");
    char lit;
    ++cHit[mTimesHit < 4 ? mTimesHit - 1 : 3];
    switch (mTimesHit) { case 1: lit = '1'; break; case 2: lit = '2'; break; case 3: lit = '3'; mTimesHit = 0; break; default: lit = '2'; mTimesHit = 0; break; }
    W('='); W(lit); live[nlive - 1][1] = lit;
    BlockOpen();
    if (SayText(lit)) return true;
    BlockClose(); Pop();
    return false;
}
static bool Has() { Ws("Gi"); W('Y'); bool r = Next(mHas); W('x'); W(r ? '1' : '0'); Ws("~i"); return r; }
static void Model()
{
    Ws("kPp" "F"); if (T()) return;
    W('v'); W(mHeld ? '1' : '0'); if (mHeld) { ++cRel; Ws("-X"); mHeld = false; }
    for (;;) { bool r = Next(mStart); W(r ? 'S' : 'n'); if (r) { mHeld = true; break; } F(); if (T()) return; }
    if (T()) return;
    mTimesHit = 0;
    while (mThings < 4) {
        F(); if (T()) return;
        Ws("sh"); bool talk = Next(mTalk); W(talk ? 'A' : 'a'); Ws("~h");
        bool leave = false;
        if (talk) {
            if (T()) return;
            BlockOpen();
            if (!mIntro) {
                if (T()) return;
                Ws("sNsw" "CwN" "~w~N");
                Ws("s0s0sb" "E" "Jb" "~Q~b~0~0");
                mIntro = true; ++cIntro;
            } else {
                if (T()) return;
                bool cond = Has() && mThings < 4;
                if (cond) {
                    if (T()) return;
                    Ws("s0snsysq" "?" "~q~y~n~0");
                    long ans;
                    for (;;) { ans = kAns[NextDigit(mAns)]; W('#'); W((char)('1' + ans)); if (ans >= 0) break; F(); if (T()) return; }
                    if (T()) return;
                    if (ans == 1) {
                        if (T()) return;
                        while (Has()) { F(); if (T()) return; Ws("Git~i"); ++mThings; }
                        if (T()) return;
                        if (mThings < 4) { if (T()) return; ++cMore; if (Say('m')) return; }
                        else {
                            if (T()) return;
                            Ws("GPsp" "CpP" "~p~P");
                            Ws("s0s0sd" "E" "Jd" "~Q~d~0~0");
                            W('I');
                            leave = true; ++cPotion;
                        }
                    } else { if (T()) return; ++cNo; if (Say('g')) return; }
                } else { if (T()) return; ++cWait; if (Say('w')) return; }
            }
            BlockClose();
            if (leave) break;
        }
        if (Hit()) { if (HitArm()) return; }
    }
    if (T()) return;
    for (;;) {
        if (T()) return;
        Ws("sh"); bool talk = Next(mTalk); W(talk ? 'A' : 'a'); Ws("~h");
        if (talk) { if (T()) return; ++cCure; BlockOpen(); if (Say('c')) return; BlockClose(); }
        if (Hit()) { if (HitArm()) return; }
        F();
    }
}

static void Run(const char* term, const char* st, const char* talk, const char* speak, const char* perf, const char* has, const char* ans,
                const char* hit, const char* any, const char* heal, long things0, int flags, int k)
{
    memset(self.b, 0xEE, sizeof self.b); memset(parent.b, 0xDD, sizeof parent.b);
    *(void**)(self.b + 0x00) = (void*)0x5E1F; *(void**)(self.b + 0x04) = gameObj;
    Me()[0] = meVtbl; Me()[1] = (void*)0x4D45; Me()[2] = &meInfo;
    *(void**)(self.b + 0x14) = parent.b; *(void**)(self.b + 0x18) = (void*)0x5DA7;
    INTRODONE = (flags & 1) != 0; THINGS = things0;
    Seh()[0] = sehVtbl; Seh()[1] = 0; Seh()[2] = 0; Seh()[3] = 0;
    if (flags & 2) { Info* i = NewInfo(); Seh()[2] = i->Data; Seh()[3] = i; }
    Script* ss[10] = { &sTerm, &sStart, &sTalk, &sSpeak, &sPerf, &sHas, &sAns, &sHit, &sAny, &sHeal };
    const char* tx[10] = { term, st, talk, speak, perf, has, ans, hit, any, heal };
    bool df[10] = { true, true, false, false, false, false, false, false, false, false };
    for (int i = 0; i < 10; ++i) { ss[i]->s = tx[i]; ss[i]->pos = 0; ss[i]->dflt = df[i]; }
    mTerm = sTerm; mStart = sStart; mTalk = sTalk; mSpeak = sSpeak; mPerf = sPerf; mHas = sHas; mAns = sAns; mHit = sHit; mAny = sAny; mHeal = sHeal;
    mThings = things0; mIntro = (flags & 1) != 0; mHeld = (flags & 2) != 0;
    ngot = nwant = 0; nlive = 0; liveStrings = deletes = pauseDepth = 0; liveInfos = (flags & 2) ? 1 : 0;
    meInfo.RefCount = 1; meInfo.DeleteFunc = (void*)VirtualTrap; meInfo.Data = Me()[1];
    Model();
    CallRow(self.b, 0);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) printf("  got  %.700s\n  want %.700s\n", got, want);
    }
    check(THINGS == mThings && INTRODONE == mIntro && self.b[0x1D] == 0xEE && self.b[0x24] == 0xEE, "CWitch fields", k);
    check(meInfo.RefCount == 1 && Me()[0] == meVtbl && *(void**)(self.b + 0x14) == parent.b, "Me / this untouched", k);
    check((Seh()[3] != 0) == mHeld, "seh_Witch held state", k);
    if (Seh()[3]) { ReleaseInfo((Info*)Seh()[3]); Seh()[3] = 0; }
    check(liveStrings == 0 && liveInfos == 0 && pauseDepth == 0, "everything released", k);
}

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; heroVtbl[0] = (void*)VirtualTrap; hero[0] = heroVtbl; hero[1] = (void*)0x4845; hero[2] = 0;
    g_ScriptDefPtr[0] = scriptDef.b;
    for (int i = 0; i < 800; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) meVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 40; ++i) sehVtbl[i] = (void*)VirtualTrap;
    meVtbl[21] = (void*)M_HitBy; meVtbl[27] = (void*)M_TalkedTo; meVtbl[41] = (void*)M_Heal; meVtbl[42] = (void*)M_Any;
    sehVtbl[12] = (void*)S_GetScriptThing; sehVtbl[13] = (void*)S_Speak; sehVtbl[26] = (void*)S_IsPerf;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_Start; gameVtbl[39] = (void*)G_Answer; gameVtbl[70] = (void*)G_GetHero;
    gameVtbl[114] = (void*)G_YesNo; gameVtbl[125] = (void*)G_TakeObject; gameVtbl[184] = (void*)G_HasObject; gameVtbl[264] = (void*)G_GetHealth;
    gameVtbl[296] = (void*)G_Objective; gameVtbl[361] = (void*)G_ClearInfo; gameVtbl[370] = (void*)G_StartMovie; gameVtbl[379] = (void*)G_PauseEntities;
    gameVtbl[655] = (void*)G_QuestName;
    int k = 0;
    //   term        start talk  speak perf  has    ans   hit   any  heal things flags
    Run("1",         "",   "",   "",   "",   "",    "",   "",   "",  "",  0, 0, ++k);
    Run("001",       "01", "",   "",   "",   "",    "",   "",   "",  "",  0, 2, ++k);
    Run("0000001",   "1",  "1",  "",   "",   "",    "",   "",   "",  "",  0, 0, ++k);
    Run("000000001", "1",  "01", "1",  "",   "0",   "",   "0",  "1", "0", 0, 1, ++k);
    Run("00000000000000000001","1","1","0","","1111110","01","","","", 0, 1, ++k);
    Run("0000000000000000000000000001","1","1001","11","1","10","2","111","","", 3, 1, ++k);
    unsigned long seed = 307;
    for (int r = 0; r < 20000; ++r) {
        static char b[10][48];
        int mod[10] = { 29, 3, 2, 2, 3, 2, 3, 3, 2, 2 };
        for (int j = 0; j < 10; ++j) {
            for (int i = 0; i < 47; ++i) { seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 16); b[j][i] = (j == 6) ? (char)('0' + v % 3) : ((v % mod[j] == 0) ? '1' : '0'); }
            b[j][47] = 0;
        }
        seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 8);
        Run(b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], (long)(v % 6), (int)((v >> 4) % 4), 1000 + r);
    }
    printf("model paths: seh_Witch release %d, intro %d, need more %d, potion %d, refused %d, waiting %d, hits 1/2/3/default %d/%d/%d/%d, got cure %d\n", cRel, cIntro, cMore, cPotion, cNo, cWait, cHit[0], cHit[1], cHit[2], cHit[3], cCure);
    if (failures) { printf("00ece7f0_TEST FAIL %d\n", failures); return 1; }
    printf("00ece7f0_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}