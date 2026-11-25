// Behaviour test for 0x00891720 CGameScriptInterface::ChangeHeroHealthBy: changes a living hero's health and, when the change is
// applied and negative (below retail's pooled 0.0f, 0x0122DEDC), reports the damage to the player gui.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal(s) the row uses:
// a mutated literal (-> 0.5f) changes the behaviour and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static char g_log[128]; static unsigned char g_hero[0xb8]; static unsigned char g_player[0x40]; static unsigned char g_gui[4];
extern "C" void* __fastcall NF420_GetMainPlayer(void*, int) { return g_player; }
extern "C" void* __fastcall NF420_GetPItem(void* self, int) { if (self != g_player + 0x34) exit(5); return g_hero; }
extern "C" void __fastcall NF420_ChangeHealth(void*, int, float amount, bool raw) { char b[32]; sprintf(b, "health%g,%d;", amount, raw); strcat(g_log, b); }   // hero slot 59
extern "C" void __fastcall NF420_AddDamageChange(void* self, int, float amount, bool flag) { char b[32]; sprintf(b, "damage%g,%d;", amount, flag); strcat(g_log, b); if (self != g_gui) exit(5); }
extern "C" void* NF420_PlayerGui[2] = { g_gui, 0 };
#pragma comment(linker, "/alternatename:?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ=@NF420_GetMainPlayer@8")
#pragma comment(linker, "/alternatename:?GetPItem@CBaseIntelligentPointer@@QBEPAVCThingCreatureBase@@XZ=@NF420_GetPItem@8")
#pragma comment(linker, "/alternatename:?AddDamageChange@CPlayerGui@@QAEXM_N@Z=@NF420_AddDamageChange@16")
#pragma comment(linker, "/alternatename:?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A=_NF420_PlayerGui")
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("hero virtual"); }
extern "C" void __fastcall NF420_Row(void* self, int edx, float amount, bool raw, bool apply);
#pragma comment(linker, "/alternatename:@NF420_Row@20=?ChangeHeroHealthBy@CGameScriptInterface@@UBEXM_N0@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
extern "C" void __cdecl NF420Trap0() { NF420TrapHit("??0CCreatureAction_StabbedToDeath@@QAE@AAVCThingCreatureBase@@_N@Z"); }
#pragma comment(linker, "/alternatename:??0CCreatureAction_StabbedToDeath@@QAE@AAVCThingCreatureBase@@_N@Z=_NF420Trap0")
extern "C" void __cdecl NF420Trap1() { NF420TrapHit("??1CCreatureActionBase@@UAE@XZ"); }
#pragma comment(linker, "/alternatename:??1CCreatureActionBase@@UAE@XZ=_NF420Trap1")
#pragma comment(linker, "/alternatename:?DELTA_NOTIONAL_ZERO@@3MB=_NF420TrapData")
extern "C" void __cdecl NF420Trap3() { NF420TrapHit("?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z"); }
#pragma comment(linker, "/alternatename:?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z=_NF420Trap3")
int main()
{
    static void* hv[60]; for (int i = 0; i < 60; ++i) hv[i] = (void*)NF420_VirtualTrap; hv[59] = (void*)NF420_ChangeHealth;
    *(void**)g_hero = hv; static unsigned char gsi[0x18]; *(void**)(gsi + 0x14) = (void*)0x1414;
    NF420_Row(gsi, 0, 0.25f, false, true);                               // a gain: no damage report
    bool ok = strcmp(g_log, "health0.25,0;") == 0;
    g_log[0] = 0; NF420_Row(gsi, 0, -0.25f, false, true);                // a loss: reported as damage 0.25
    ok = ok && strcmp(g_log, "health-0.25,0;damage0.25,1;") == 0;
    g_log[0] = 0; NF420_Row(gsi, 0, -0.25f, false, false);               // not applied: no report
    ok = ok && strcmp(g_log, "health-0.25,0;") == 0;
    if (ok) printf("CHANGE_HERO_HEALTH_BY_PASS\n");
    else printf("FAIL log=%s\n", g_log);
    return ok ? 0 : 1;
}
