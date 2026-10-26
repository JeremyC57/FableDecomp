#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CThing { public: unsigned char _pad[0x91]; unsigned char m_91_0 : 1; unsigned char m_91_1 : 7; }; static CThing g_thing; static CThing* g_hero = &g_thing; static long g_removed[4]; static long g_nRemoved = 0;
class CPlayer { public: CThing* GetHeroCreature() { std::strcat(g_seq, "h"); return g_hero; } void RemoveMode(long mode) { std::strcat(g_seq, "R"); g_removed[g_nRemoved++] = mode; } };
static bool g_valid = true, g_held = false; static long g_input = 0; struct CTarget { bool IsValid() { std::strcat(g_seq, "V"); return g_valid; } };
struct CObject { virtual void v000() { } unsigned char _pad04[0x14]; CPlayer* m_pPlayer; unsigned char _pad1c[8]; CTarget* m_pTarget; unsigned char _pad28[0xc]; long m_lockTimer;
    bool IsButtonHeld(long, long) { std::strcat(g_seq, "Q"); return g_held; } void DeactivateZTarget(long input) { std::strcat(g_seq, "D"); g_input = input; }
    virtual void Update(long input) { m_pPlayer->GetHeroCreature(); if (m_lockTimer >= 0) m_lockTimer = m_lockTimer - 1; if (!m_pTarget->IsValid() && !IsButtonHeld(1, 2)) DeactivateZTarget(input); } };

int main() {
    CObject o; CPlayer p; CTarget t; o.m_pPlayer = &p; o.m_pTarget = &t; o.m_lockTimer = 1; g_valid = true; o.Update(7); if (std::strcmp(g_seq, "hV") || o.m_lockTimer != 0) { std::printf("00688c40_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; o.Update(7); if (std::strcmp(g_seq, "hV") || o.m_lockTimer != -1) { std::printf("00688c40_TEST FAIL\n"); return 1; } g_seq[0] = 0; o.Update(7); if (o.m_lockTimer != -1) { std::printf("00688c40_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_valid = false; g_held = true; o.Update(7); if (std::strcmp(g_seq, "hVQ")) { std::printf("00688c40_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_held = false; o.Update(8); if (std::strcmp(g_seq, "hVQD") || g_input != 8) { std::printf("00688c40_TEST FAIL\n"); return 1; }
    std::printf("00688c40_TEST PASS\n"); return 0;
}