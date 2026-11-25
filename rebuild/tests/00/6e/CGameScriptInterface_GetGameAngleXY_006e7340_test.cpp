// Behaviour test for 0x006E7340 CGameScriptInterface::GetGameAngleXY: the camera object's angle when the world's holder yields one,
// else 0.0 (retail's pooled 0.0f, 0x0122DEDC).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal the row uses:
// a mutated literal (0.0f -> 0.5f) changes the result and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static bool g_has = false; static void* g_objVtbl[26]; static void* g_obj[1] = { g_objVtbl };
extern "C" bool __fastcall NF420_TryGet(void*, int, void** out) { if (g_has) *out = g_obj; return g_has; }       // holder slot 10
extern "C" float __fastcall NF420_GetAngle(void*, int) { return 1.25f; }                                          // object slot 25
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("fixture virtual"); }
extern "C" float __fastcall NF420_Row(void* self, int edx);
#pragma comment(linker, "/alternatename:@NF420_Row@8=?GetGameAngleXY@CGameScriptInterface@@UBEMXZ")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static void* hv[11]; for (int i = 0; i < 11; ++i) hv[i] = (void*)NF420_VirtualTrap; hv[10] = (void*)NF420_TryGet;
    for (int j = 0; j < 26; ++j) g_objVtbl[j] = (void*)NF420_VirtualTrap; g_objVtbl[25] = (void*)NF420_GetAngle;
    static void* holder[1] = { hv }; static unsigned char world[0x38]; *(void**)(world + 0x34) = holder;
    static unsigned char gsi[8]; *(void**)(gsi + 4) = world;
    float none = NF420_Row(gsi, 0);
    g_has = true; float angle = NF420_Row(gsi, 0);
    bool ok = none == 0.0f && angle == 1.25f;
    if (ok) printf("CGameScriptInterface_006e7340_TEST PASS\n");
    else printf("FAIL none=%f angle=%f\n", none, angle);
    return ok ? 0 : 1;
}
