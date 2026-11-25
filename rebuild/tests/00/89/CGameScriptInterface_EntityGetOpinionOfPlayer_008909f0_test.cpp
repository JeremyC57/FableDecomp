// Behaviour test for 0x008909F0 CGameScriptInterface::EntityGetOpinionOfPlayer: the opinion reaction manager's value for the thing,
// or 0.0 (retail's pooled 0.0f, 0x0122DEDC) when the script thing has no CThing.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal the row uses:
// a mutated literal (0.0f -> 0.5f) changes the result and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static void* g_thing = 0; static long g_opinion = -1; static void* g_mgr = 0;
extern "C" void* __fastcall NF420_GetPThing(void*, int) { return g_thing; }                                         // CScriptThing slot 11
extern "C" float __fastcall NF420_GetThingOpinionOfPlayer(void* self, int, const void* thing, long opinion) { if (self != g_mgr || thing != g_thing) exit(5); g_opinion = opinion; return 3.5f; }
#pragma comment(linker, "/alternatename:?GetThingOpinionOfPlayer@COpinionReactionManager@@QBEMABVCThing@@W4EOpinion@@@Z=@NF420_GetThingOpinionOfPlayer@16")
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("CScriptThing virtual"); }
extern "C" float __fastcall NF420_Row(void* self, int edx, const void* thing, long opinion);
#pragma comment(linker, "/alternatename:@NF420_Row@16=?EntityGetOpinionOfPlayer@CGameScriptInterface@@UBEMABVCScriptThing@@W4EOpinion@@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static void* tv[79]; for (int i = 0; i < 79; ++i) tv[i] = (void*)NF420_VirtualTrap; tv[11] = (void*)NF420_GetPThing;
    void* scriptThing[3] = { tv, 0, 0 };
    static unsigned char mgr[8], world[0x78], gsi[8]; g_mgr = mgr; *(void**)(world + 0x74) = mgr; *(void**)(gsi + 4) = world;
    float none = NF420_Row(gsi, 0, scriptThing, 2);
    static unsigned char thing[4]; g_thing = thing; float value = NF420_Row(gsi, 0, scriptThing, 2);
    bool ok = none == 0.0f && value == 3.5f && g_opinion == 2;
    if (ok) printf("008909f0_TEST PASS\n");
    else printf("FAIL none=%f value=%f\n", none, value);
    return ok ? 0 : 1;
}
