// Behaviour test for 0x00890400 CGameScriptInterface::GetHealth: the entity from vtable slot 11 -> its Health when alive (+0x91 bit 0 clear) and in
// combat (+0x6c bit 1), else 0.0f (retail's pooled __real@00000000, 0x0122DEDC).
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#include "engine/CGameScriptInterface.h"
#include <cstdio>
struct CHealthEntityView {
    unsigned char _pad_0x00[0x6c];
    unsigned char CombatFlags;
    unsigned char _pad_0x6d[0x24];
    unsigned char StateFlags;
    unsigned char _pad_0x92[0x22];
    float Health;
};
float __stdcall CGameScriptInterface_GetHealth(CGameScriptInterface* self);
static CHealthEntityView g_entity; static CHealthEntityView* g_ret = 0; static const void* g_self = 0;
static CHealthEntityView* __fastcall GetEntity(CGameScriptInterface* self) { g_self = self; return g_ret; }
static void* g_vtbl[16]; static unsigned char g_gsi[0x400];
int main()
{
    g_vtbl[11] = (void*)&GetEntity; *(void**)g_gsi = g_vtbl;
    CGameScriptInterface* self = (CGameScriptInterface*)g_gsi;
    g_entity.Health = 12.5f; g_entity.StateFlags = 0; g_entity.CombatFlags = 2; g_ret = &g_entity;
    float a = CGameScriptInterface_GetHealth(self);
    g_entity.CombatFlags = 0; float b = CGameScriptInterface_GetHealth(self);
    g_entity.CombatFlags = 2; g_entity.StateFlags = 1; float c = CGameScriptInterface_GetHealth(self);
    g_ret = 0; float d = CGameScriptInterface_GetHealth(self);
    if (a != 12.5f || b != 0.0f || c != 0.0f || d != 0.0f || g_self != self) { std::printf("FAIL %f %f %f %f\n", a, b, c, d); return 1; }
    std::printf("CGameScriptInterface_00890400_TEST PASS\n");
    return 0;
}
