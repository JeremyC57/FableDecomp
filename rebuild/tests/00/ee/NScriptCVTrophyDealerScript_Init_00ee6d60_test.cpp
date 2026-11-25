// Behaviour test for 0x00EE6D60 NScript::CV_TrophyDealerScript::Init(): sets the active quest card's objective to
// "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_01" (retail 0x012F5208) in region "Witchwood2" with no extra text.
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
static char g_quest[] = "QUEST_TROPHY";
extern "C" void* __fastcall NF420_GetActiveQuestName(void*, int, CCharString* ret) { ret->PStringData = g_quest; ++g_strings; return ret; }
extern "C" void __fastcall NF420_SetQuestCardObjective(void*, int, const CCharString& quest, const CCharString& text, const CCharString& region, const CCharString& extra)
{ Log("quest", quest.PStringData); Log("text", text.PStringData); Log("region", region.PStringData); Log("extra", extra.PStringData); }
extern "C" void __fastcall NF420_Row(void* self, int edx);
#pragma comment(linker, "/alternatename:@NF420_Row@8=?Init@CV_TrophyDealerScript@NScript@@UAEXXZ")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    NF420InitVtbls(); g_gameVtbl[655] = (void*)NF420_GetActiveQuestName; g_gameVtbl[296] = (void*)NF420_SetQuestCardObjective;
    static unsigned char obj[0x48]; *(void**)(obj + 0x40) = g_game;
    NF420_Row(obj, 0);
    bool ok = !g_bad && strcmp(g_log, "quest[QUEST_TROPHY]text[TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_01]region[Witchwood2]extra[]") == 0 && g_strings == 0;
    if (ok) printf("00ee6d60_TEST PASS\n");
    else printf("FAIL bad=%d log=%s strings=%d\n", g_bad, g_log, g_strings);
    return ok ? 0 : 1;
}
