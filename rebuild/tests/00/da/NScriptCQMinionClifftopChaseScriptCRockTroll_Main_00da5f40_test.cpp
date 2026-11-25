// Behaviour test for 0x00DA5F40 NScript::CQ_MinionClifftopChaseScript::CRockTroll::Main(): attaches the troll to the active quest, waits
// until the hero is within 20.0, then shows the quest info and adds a health bar with the icon "HUD_QUEST_ICON_ROCK_TROLL" (retail
// 0x012D75D4), keeping its id.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined; it is called through a
// __fastcall shim aliased to the row symbol). The game interface is a vtable fixture that logs what it is given, so the test asserts
// the literal; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
// ---- script fixtures: CCharString / CBaseClass under the row's spellings; the game interface and every CScriptThing are raw objects
// whose vtables trap except the slots a test fills (thiscall-compatible __fastcall functions, unused edx); a CScriptThing is
// {vptr, PImp.Data (an id), PImp.Info (a shared count block that never reaches zero)}.
class CCharString { public: void* PStringData; CCharString(); CCharString(const char* text, long length); ~CCharString(); };
class CBaseClass { public: virtual ~CBaseClass(); };
static char g_log[512]; static int g_bad = 0, g_strings = 0;
static void Log(const char* tag, const void* s) { strcat(g_log, tag); strcat(g_log, "["); strcat(g_log, s ? (const char*)s : "(null)"); strcat(g_log, "]"); }
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; if (length != -1) g_bad = 1; ++g_strings; }
CCharString::CCharString() { PStringData = 0; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
CBaseClass::~CBaseClass() { }
struct NF420Info { long RefCount; void* DeleteFunc; void* Data; };
static NF420Info g_info = { 1000, 0, 0 };
static void* g_gameVtbl[918]; static void* g_thingVtbl[79]; static void* g_game[1] = { g_gameVtbl };
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("game-interface / CScriptThing virtual"); }
static void NF420InitVtbls() { for (int i = 0; i < 918; ++i) g_gameVtbl[i] = (void*)NF420_VirtualTrap; for (int j = 0; j < 79; ++j) g_thingVtbl[j] = (void*)NF420_VirtualTrap; }
static void NF420MakeThing(void* p, long id) { void** t = (void**)p; t[0] = g_thingVtbl; t[1] = (void*)id; t[2] = &g_info; }
static long ThingId(const void* p) { return (long)((void* const*)p)[1]; }
// slot 72 GetThingWithScriptName(const CCharString&) const: a CScriptThing by hidden pointer, id 77, holding one reference
extern "C" void* __fastcall NF420_GetThingWithScriptName(void*, int, void* ret, const CCharString& name) { Log("thing", name.PStringData); NF420MakeThing(ret, 77); ++g_info.RefCount; return ret; }   // the returned thing owns one reference
// ---- end of script fixtures
static unsigned char g_obj[0x40]; static unsigned char g_hero[12]; static int g_frames = 0, g_dist = 0; static char g_quest[] = "QUEST_CLIFFTOP";
class CScriptThing;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{ if ((void*)&a != g_obj + 8 || (void*)&b != g_hero || distance != 20.0f) g_bad = 1; strcat(g_log, "near?;"); return ++g_dist == 2; }
extern "C" bool __fastcall NF420_IsActiveThreadTerminating(void*, int) { return false; }
extern "C" void __fastcall NF420_SetPredicate(void*, int, const void*) { strcat(g_log, "predicate;"); }
extern "C" void* __fastcall NF420_ThingCopy(void* self, int, const void* other) { memcpy(self, other, 12); return self; }
extern "C" void __fastcall NF420_ThingDtor(void*, int) { }
extern "C" void* __fastcall NF420_BaseObjectCtor(void* self, int) { return self; }
extern "C" void __fastcall NF420_BaseObjectDtor(void*, int) { }
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@NF420_IsActiveThreadTerminating@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@NF420_SetPredicate@12")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@NF420_ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@NF420_ThingDtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@NF420_BaseObjectCtor@8")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@NF420_BaseObjectDtor@8")
extern "C" void __fastcall NF420_NewScriptFrame(void*, int) { ++g_frames; strcat(g_log, "frame;"); }
extern "C" void* __fastcall NF420_GetActiveQuestName(void*, int, CCharString* ret) { ret->PStringData = g_quest; ++g_strings; return ret; }
extern "C" void __fastcall NF420_AttachToScript(void*, int, const void* thing, const CCharString& quest) { if (thing != g_obj + 8) g_bad = 1; Log("attach", quest.PStringData); }
extern "C" void* __fastcall NF420_GetHero(void*, int) { return g_hero; }
extern "C" void __fastcall NF420_DisplayQuestInfo(void*, int, bool show) { if (!show) g_bad = 1; strcat(g_log, "info;"); }
extern "C" long __fastcall NF420_AddQuestInfoBarHealth(void*, int, const void* thing, const void*, const CCharString& icon, float scale)
{ if (thing != g_obj + 8 || scale != 1.0f) g_bad = 1; Log("bar", icon.PStringData); return 42; }
extern "C" void __fastcall NF420_Row(void* self, int edx);
#pragma comment(linker, "/alternatename:@NF420_Row@8=?Main@CRockTroll@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
extern "C" void __cdecl NF420Trap0() { NF420TrapHit("?AskHeroQuestion@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDABVCCharString@@22_N3@Z"); }
#pragma comment(linker, "/alternatename:?AskHeroQuestion@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDABVCCharString@@22_N3@Z=_NF420Trap0")
extern "C" void __cdecl NF420Trap1() { NF420TrapHit("?ClearAllActions@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?ClearAllActions@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_NF420Trap1")
extern "C" void __cdecl NF420Trap2() { NF420TrapHit("?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_NF420Trap2")
extern "C" void __cdecl NF420Trap3() { NF420TrapHit("?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_NF420Trap3")
extern "C" void __cdecl NF420Trap4() { NF420TrapHit("?Converse@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@PBD@Z"); }
#pragma comment(linker, "/alternatename:?Converse@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@PBD@Z=_NF420Trap4")
extern "C" void __cdecl NF420Trap5() { NF420TrapHit("?DropGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?DropGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_NF420Trap5")
extern "C" void __cdecl NF420Trap6() { NF420TrapHit("?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z"); }
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_NF420Trap6")
extern "C" void __cdecl NF420Trap7() { NF420TrapHit("?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z"); }
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_NF420Trap7")
extern "C" void __cdecl NF420Trap8() { NF420TrapHit("?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z"); }
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_NF420Trap8")
extern "C" void __cdecl NF420Trap9() { NF420TrapHit("?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ"); }
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=_NF420Trap9")
extern "C" void __cdecl NF420Trap10() { NF420TrapHit("?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z"); }
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_NF420Trap10")
extern "C" void __cdecl NF420Trap11() { NF420TrapHit("?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_NF420Trap11")
extern "C" void __cdecl NF420Trap12() { NF420TrapHit("?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_NF420Trap12")
extern "C" void __cdecl NF420Trap13() { NF420TrapHit("?MoveToAndPickUpGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXAAVCScriptThing@@W4EScriptEntityMoveType@@_N@Z"); }
#pragma comment(linker, "/alternatename:?MoveToAndPickUpGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXAAVCScriptThing@@W4EScriptEntityMoveType@@_N@Z=_NF420Trap13")
extern "C" void __cdecl NF420Trap14() { NF420TrapHit("?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z"); }
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=_NF420Trap14")
extern "C" void __cdecl NF420Trap15() { NF420TrapHit("?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z"); }
#pragma comment(linker, "/alternatename:?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z=_NF420Trap15")
extern "C" void __cdecl NF420Trap16() { NF420TrapHit("?PerformExpression@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@ABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?PerformExpression@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@ABVCCharString@@@Z=_NF420Trap16")
extern "C" void __cdecl NF420Trap17() { NF420TrapHit("?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z"); }
#pragma comment(linker, "/alternatename:?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z=_NF420Trap17")
extern "C" void __cdecl NF420Trap18() { NF420TrapHit("?PlayCombatAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N11111@Z"); }
#pragma comment(linker, "/alternatename:?PlayCombatAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N11111@Z=_NF420Trap18")
extern "C" void __cdecl NF420Trap19() { NF420TrapHit("?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@J_N111111@Z"); }
#pragma comment(linker, "/alternatename:?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@J_N111111@Z=_NF420Trap19")
extern "C" void __cdecl NF420Trap20() { NF420TrapHit("?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z"); }
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_NF420Trap20")
extern "C" void __cdecl NF420Trap21() { NF420TrapHit("?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z"); }
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=_NF420Trap21")
extern "C" void __cdecl NF420Trap22() { NF420TrapHit("?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z"); }
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_NF420Trap22")
extern "C" void __cdecl NF420Trap23() { NF420TrapHit("?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z"); }
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_NF420Trap23")
extern "C" void __cdecl NF420Trap24() { NF420TrapHit("?UnsheatheWeapons@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?UnsheatheWeapons@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_NF420Trap24")
extern "C" void __cdecl NF420Trap25() { NF420TrapHit("?Validate@CBaseObject@@UBEXXZ"); }
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_NF420Trap25")
extern "C" void __cdecl NF420Trap26() { NF420TrapHit("?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ"); }
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_NF420Trap26")
extern "C" void __cdecl NF420Trap27() { NF420TrapHit("?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z"); }
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_NF420Trap27")
extern "C" void __cdecl NF420Trap28() { NF420TrapHit("?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z"); }
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_NF420Trap28")
extern "C" void __cdecl NF420Trap29() { NF420TrapHit("?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_NF420Trap29")
extern "C" void __cdecl NF420Trap30() { NF420TrapHit("?v00@CPredicateBase@NScript@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?v00@CPredicateBase@NScript@@UAEXXZ=_NF420Trap30")
int main()
{
    NF420InitVtbls(); g_gameVtbl[7] = (void*)NF420_NewScriptFrame; g_gameVtbl[655] = (void*)NF420_GetActiveQuestName; g_gameVtbl[465] = (void*)NF420_AttachToScript;
    g_gameVtbl[70] = (void*)NF420_GetHero; g_gameVtbl[321] = (void*)NF420_DisplayQuestInfo; g_gameVtbl[325] = (void*)NF420_AddQuestInfoBarHealth;
    *(void**)(g_obj + 4) = g_game; NF420MakeThing(g_obj + 8, 1); NF420MakeThing(g_hero, 2);
    NF420_Row(g_obj, 0);
    bool ok = !g_bad && strcmp(g_log, "predicate;frame;attach[QUEST_CLIFFTOP]near?;frame;near?;info;bar[HUD_QUEST_ICON_ROCK_TROLL]") == 0
        && *(long*)(g_obj + 0x1c) == 42 && g_strings == 0;
    if (ok) printf("00da5f40_TEST PASS\n");
    else printf("FAIL bad=%d log=%s strings=%d\n", g_bad, g_log, g_strings);
    return ok ? 0 : 1;
}
