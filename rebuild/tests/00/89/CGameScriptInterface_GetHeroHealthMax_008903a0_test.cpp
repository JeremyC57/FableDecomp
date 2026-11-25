// Behaviour test for 0x008903A0 CGameScriptInterface::GetHeroHealthMax: the main player's controlled creature's value when it exists and its +0x91 bit 0 is
// clear, else 0.0f (retail's pooled __real@00000000, 0x0122DEDC).
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };
#include "engine/CPlayerManager.h"
#include <cstdio>
#include <cstring>

static unsigned char g_creature[0x200]; static CThingPlayerCreature* g_ret = 0; static int g_calls = 0;
static unsigned char g_player[0x40]; static unsigned char g_mgr[0x400]; static unsigned char g_gsi[0x400];
CPlayer& CPlayerManager::GetMainPlayer() { ++g_calls; return *(CPlayer*)g_player; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return this == (const CPlayer*)g_player ? g_ret : 0; }
int main()
{
    *(void**)(g_gsi + 0x14) = g_mgr;   // PlayerManager (+0x14)
    const CGameScriptInterface* gsi = (const CGameScriptInterface*)g_gsi;
    if (0xb0) *(float*)(g_creature + 0xb0) = 42.5f;
    g_ret = (CThingPlayerCreature*)g_creature; g_creature[0x91] = 0;
    float alive = gsi->CGameScriptInterface::GetHeroHealthMax();
    g_creature[0x91] = 1;
    float dead = gsi->CGameScriptInterface::GetHeroHealthMax();
    g_ret = 0;
    float none = gsi->CGameScriptInterface::GetHeroHealthMax();
    if (alive != 42.5f || dead != 0.0f || none != 0.0f || g_calls != 3) { std::printf("FAIL %f %f %f\n", alive, dead, none); return 1; }
    std::printf("CGameScriptInterface_008903a0_TEST PASS\n");
    return 0;
}
