// Behaviour test for 0x006A98F0 CThingPlayerCreature::SerialiseGameState: after the creature base serialises, outside load modes the
// max health comes from the def and a health still at 0.0 (retail's pooled 0.0f, 0x0122DEDC) becomes the max; the thing is drawable.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal(s) the row uses:
// a mutated literal (-> 0.5f) changes the behaviour and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static int g_base = 0; static bool g_baseResult = true;
extern "C" bool __fastcall NF420_BaseSerialise(void*, int, void*) { ++g_base; return g_baseResult; }
#pragma comment(linker, "/alternatename:?SerialiseGameState@CThingCreatureBase@@UAE_NAAVCPersistContext@@@Z=@NF420_BaseSerialise@12")
extern "C" bool __fastcall NF420_Row(void* self, int edx, void* context);
#pragma comment(linker, "/alternatename:@NF420_Row@12=?SerialiseGameState@CThingPlayerCreature@@UAE_NAAVCPersistContext@@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static unsigned char def[0x54]; *(float*)(def + 0x50) = 80.0f;
    static unsigned char thing[0xe4]; *(void**)(thing + 0xe0) = def;
    static unsigned char ctx[0x1c]; *(long*)(ctx + 0x18) = 0;
    *(float*)(thing + 0xb4) = 0.0f; bool r1 = NF420_Row(thing, 0, ctx);
    bool ok = r1 && *(float*)(thing + 0xb0) == 80.0f && *(float*)(thing + 0xb4) == 80.0f && (thing[0x92] & 2);
    *(float*)(thing + 0xb4) = 0.5f; NF420_Row(thing, 0, ctx);
    ok = ok && *(float*)(thing + 0xb4) == 0.5f;                          // a set health is kept
    *(long*)(ctx + 0x18) = 1; *(float*)(thing + 0xb4) = 0.0f; NF420_Row(thing, 0, ctx);
    ok = ok && *(float*)(thing + 0xb4) == 0.0f;                          // load mode: untouched
    g_baseResult = false; bool r4 = NF420_Row(thing, 0, ctx);
    ok = ok && !r4 && g_base == 4;
    if (ok) printf("006a98f0_TEST PASS\n");
    else printf("FAIL health=%f max=%f\n", *(float*)(thing + 0xb4), *(float*)(thing + 0xb0));
    return ok ? 0 : 1;
}
