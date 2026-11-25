// Behaviour test for 0x00753BA0 CTCLook::ImplementHeadSettings: inside the animation range, head angles both 0.0 (retail's pooled 0.0f,
// 0x0122DEDC) remove the vertical / horizontal head anims; otherwise both head anims are updated.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal(s) the row uses:
// a mutated literal (-> 0.5f) changes the behaviour and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static char g_log[128]; static unsigned char g_anim[4];
extern "C" void __fastcall NF420_InitialiseHeadAnims(void*, int) { strcat(g_log, "init;"); }
extern "C" bool __fastcall NF420_IsInsideAnimationRange(void*, int) { return true; }
extern "C" void* __fastcall NF420_GetAnimation(void*, int) { return g_anim; }
extern "C" void __fastcall NF420_RemoveCombinationAnimsOfType(void* self, int, long type) { char b[16]; sprintf(b, "remove%ld;", type); strcat(g_log, b); if (self != g_anim) exit(5); }
extern "C" void __fastcall NF420_UpdateHeadCreatureAnim(void*, int, const void*, long anim, void* animation) { char b[16]; sprintf(b, "update%ld;", anim); strcat(g_log, b); if (animation != g_anim) exit(5); }
#pragma comment(linker, "/alternatename:?InitialiseHeadAnims@CTCLook@@AAEXXZ=@NF420_InitialiseHeadAnims@8")
#pragma comment(linker, "/alternatename:?IsInsideAnimationRange@CTCLook@@QBE_NXZ=@NF420_IsInsideAnimationRange@8")
#pragma comment(linker, "/alternatename:?GetAnimation@CTCAnimationComplex@@QAEAAVCAnimateThing@@XZ=@NF420_GetAnimation@8")
#pragma comment(linker, "/alternatename:?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z=@NF420_RemoveCombinationAnimsOfType@12")
#pragma comment(linker, "/alternatename:?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z=@NF420_UpdateHeadCreatureAnim@20")
extern "C" void __fastcall NF420_Row(void* self, int edx);
#pragma comment(linker, "/alternatename:@NF420_Row@8=?ImplementHeadSettings@CTCLook@@AAEXXZ")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
#pragma comment(linker, "/alternatename:?g_CCharString_013bace4@@3VCCharString@@A=_NF420TrapData")
#pragma comment(linker, "/alternatename:?g_CCharString_013bace8@@3VCCharString@@A=_NF420TrapData")
int main()
{
    static unsigned char parent[0x50]; static unsigned char look[0xd0]; *(void**)(look + 4) = parent; look[0xcc] = 1;
    NF420_Row(look, 0);                                                   // angles 0, 0
    bool ok = strcmp(g_log, "init;remove12;remove11;") == 0 && look[0xcc] == 0;
    g_log[0] = 0; *(float*)(look + 0x2c) = 0.5f; *(float*)(look + 0x30) = 0.5f; NF420_Row(look, 0);
    ok = ok && strcmp(g_log, "update12;update11;") == 0;
    if (ok) printf("00753ba0_TEST PASS\n");
    else printf("FAIL log=%s\n", g_log);
    return ok ? 0 : 1;
}
