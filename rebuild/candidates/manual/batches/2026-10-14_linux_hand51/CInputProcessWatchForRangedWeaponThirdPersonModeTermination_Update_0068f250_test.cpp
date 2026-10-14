#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CThing { unsigned char _pad[0x91]; unsigned char m_91_0 : 1; unsigned char m_91_1 : 7; }; static CThing g_thing; static CThing* g_hero = &g_thing; static long g_removed[4]; static long g_nRemoved = 0;
struct CPlayer { CThing* GetHeroCreature() { std::strcat(g_seq, "h"); return g_hero; } void RemoveMode(long mode) { std::strcat(g_seq, "R"); g_removed[g_nRemoved++] = mode; } };
static bool g_strafing = false; static bool IsThingToKeepStrafing(CThing*) { std::strcat(g_seq, "S"); return g_strafing; }
struct CObject { virtual void v000() { } unsigned char _pad04[0x14]; CPlayer* m_pPlayer; unsigned char _pad1c[0xc]; long m_countdown;
    virtual void Update() { CThing* hero = m_pPlayer->GetHeroCreature(); if (hero && !hero->m_91_0) { if (IsThingToKeepStrafing(hero)) { m_countdown = 5; return; } if (--m_countdown < 0) { m_pPlayer->RemoveMode(0x22); m_pPlayer->RemoveMode(0x2e); } } } };

int main() {
    CObject o; CPlayer p; o.m_pPlayer = &p; o.m_countdown = 0; std::memset(&g_thing, 0, sizeof g_thing); g_strafing = true; o.Update(); if (std::strcmp(g_seq, "hS") || o.m_countdown != 5) { std::printf("0068f250_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_strafing = false; o.m_countdown = 1; o.Update(); if (std::strcmp(g_seq, "hS") || o.m_countdown != 0) { std::printf("0068f250_TEST FAIL\n"); return 1; } g_seq[0] = 0; o.Update(); if (std::strcmp(g_seq, "hSRR") || o.m_countdown != -1 || g_removed[0] != 0x22 || g_removed[1] != 0x2e) { std::printf("0068f250_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_thing.m_91_0 = 1; o.Update(); if (std::strcmp(g_seq, "h")) { std::printf("0068f250_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_hero = 0; o.Update(); if (std::strcmp(g_seq, "h")) { std::printf("0068f250_TEST FAIL\n"); return 1; }
    std::printf("0068f250_TEST PASS\n"); return 0;
}
