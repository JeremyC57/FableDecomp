// Behaviour test for 0x00DFB100 CTraderToRescue::OnPredicateFail(): when the trader was killed (by anyone), fails the active quest with
// the message L"TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED" (retail 0x012DEC88).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); Me's CScriptThing
// vtable and the game interface are fixtures, and CWideString's constructor is defined under the row's spelling and records the wide
// text, so the test asserts the literal; a mutated literal fails.
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
class CWideString { public: void* PStringData; CWideString(const unsigned short* text); ~CWideString(); };
class CTraderToRescue { public: void OnPredicateFail(); };
static char g_wide[64]; static char g_quest[] = "QUEST_B11"; static bool g_killed = true; static int g_wides = 0;
CWideString::CWideString(const unsigned short* text) { PStringData = (void*)text; int i = 0; for (; text[i] && i < 63; ++i) g_wide[i] = (char)text[i]; g_wide[i] = 0; ++g_wides; }
CWideString::~CWideString() { --g_wides; }
static unsigned char g_obj[0x20];
extern "C" bool __fastcall NF420_MsgIsKilledBy(void* self, int, const CCharString& who) { if (self != g_obj + 8) g_bad = 1; Log("killedby", who.PStringData); return g_killed; }
extern "C" void* __fastcall NF420_GetActiveQuestName(void*, int, CCharString* ret) { ret->PStringData = g_quest; ++g_strings; return ret; }
extern "C" void __fastcall NF420_SetQuestAsFailed(void*, int, const CCharString& quest, bool a, const CWideString& message, bool b)
{ if (!a || !b || message.PStringData == 0) g_bad = 1; Log("failed", quest.PStringData); Log("message", g_wide); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    NF420InitVtbls(); g_thingVtbl[18] = (void*)NF420_MsgIsKilledBy; g_gameVtbl[655] = (void*)NF420_GetActiveQuestName; g_gameVtbl[289] = (void*)NF420_SetQuestAsFailed;
    *(void**)(g_obj + 4) = g_game; NF420MakeThing(g_obj + 8, 1);
    ((CTraderToRescue*)g_obj)->OnPredicateFail();
    bool ok = !g_bad && strcmp(g_log, "killedby[]failed[QUEST_B11]message[TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED]") == 0 && g_strings == 0 && g_wides == 0;
    g_killed = false; g_log[0] = 0; ((CTraderToRescue*)g_obj)->OnPredicateFail();
    ok = ok && strcmp(g_log, "killedby[]") == 0;
    if (ok) printf("00dfb100_TEST PASS\n");
    else printf("FAIL bad=%d log=%s strings=%d\n", g_bad, g_log, g_strings);
    return ok ? 0 : 1;
}
