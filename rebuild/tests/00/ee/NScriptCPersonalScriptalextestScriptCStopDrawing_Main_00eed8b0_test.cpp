// Behaviour test for 0x00EED8B0 NScript::CPersonalScript_alextestScript::CStopDrawing::Main(): each frame, when the hero
// ("SCRIPT_NAME_HERO", retail 0x0125D1C8) talks to it, hits it, or hits it with any special ability but ability 0xE, the entity
// stops being drawn.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined; it is called through a
// __fastcall shim aliased to the row symbol). Me's CScriptThing vtable and the game interface are fixtures that log the names they are
// given, so the test asserts the literal at all four uses; a mutated literal fails.
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
static unsigned char g_obj[0x40]; static int g_term = 0, g_frames = 0;
extern "C" bool __fastcall NF420_IsActiveThreadTerminating(void*, int) { return ++g_term == 4; }
extern "C" void __fastcall NF420_SetPredicate(void*, int, const void*) { strcat(g_log, "predicate;"); }
extern "C" void* __fastcall NF420_ThingCopy(void* self, int, const void* other) { memcpy(self, other, 12); return self; }
extern "C" void __fastcall NF420_FilterDtor(void*, int) { }
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@NF420_IsActiveThreadTerminating@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@NF420_SetPredicate@12")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@NF420_ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CIsThingAlive@NScript@@UAE@XZ=@NF420_FilterDtor@8")
extern "C" void __fastcall NF420_NewScriptFrame(void*, int) { ++g_frames; strcat(g_log, "frame;"); }
extern "C" void __fastcall NF420_SetAsDrawable(void*, int, const void* thing, bool drawable) { if (thing != g_obj + 8 || drawable) g_bad = 1; strcat(g_log, "hide;"); }
extern "C" bool __fastcall NF420_MsgIsTalkedToBy(void* self, int, const CCharString& who) { if (self != g_obj + 8) g_bad = 1; Log("talked", who.PStringData); return false; }
extern "C" bool __fastcall NF420_MsgIsHitBy(void*, int, const CCharString& who) { Log("hit", who.PStringData); return false; }
extern "C" bool __fastcall NF420_MsgIsHitByAnySpecialAbilityFrom(void*, int, const CCharString& who) { Log("anyability", who.PStringData); return true; }
extern "C" bool __fastcall NF420_MsgIsHitBySpecialAbilityFrom(void*, int, long ability, const CCharString& who) { if (ability != 0xe) g_bad = 1; Log("ability", who.PStringData); return false; }
extern "C" void __fastcall NF420_Row(void* self, int edx);
#pragma comment(linker, "/alternatename:@NF420_Row@8=?Main@CStopDrawing@CPersonalScript_alextestScript@NScript@@EAEXXZ")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
extern "C" void __cdecl NF420Trap0() { NF420TrapHit("??RCIsThingAlive@NScript@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_NF420Trap0")
extern "C" void __cdecl NF420Trap1() { NF420TrapHit("?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ"); }
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_NF420Trap1")
int main()
{
    NF420InitVtbls(); g_gameVtbl[7] = (void*)NF420_NewScriptFrame; g_gameVtbl[511] = (void*)NF420_SetAsDrawable;
    g_thingVtbl[27] = (void*)NF420_MsgIsTalkedToBy; g_thingVtbl[21] = (void*)NF420_MsgIsHitBy;
    g_thingVtbl[42] = (void*)NF420_MsgIsHitByAnySpecialAbilityFrom; g_thingVtbl[41] = (void*)NF420_MsgIsHitBySpecialAbilityFrom;
    *(void**)(g_obj + 4) = g_game; NF420MakeThing(g_obj + 8, 1);
    NF420_Row(g_obj, 0);
    bool ok = !g_bad && strcmp(g_log, "predicate;frame;talked[SCRIPT_NAME_HERO]hit[SCRIPT_NAME_HERO]anyability[SCRIPT_NAME_HERO]ability[SCRIPT_NAME_HERO]hide;frame;") == 0
        && g_term == 4 && g_strings == 0;
    if (ok) printf("00eed8b0_TEST PASS\n");
    else printf("FAIL bad=%d log=%s term=%d strings=%d\n", g_bad, g_log, g_term, g_strings);
    return ok ? 0 : 1;
}
