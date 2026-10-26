#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static bool g_melee = true, g_play = false; class CThing { public: unsigned char _pad[0x91]; unsigned char m_91_0 : 1; unsigned char m_91_1 : 7; };
struct CShotDef { bool CanBeHitByMeleeStrike() { std::strcat(g_seq, "M"); return g_melee; } }; static bool IsPlayHitResponse(CThing*) { std::strcat(g_seq, "P"); return g_play; }
class CHitParameters { public: unsigned char _pad[0x20]; bool m_playGetHitResponse; bool m_overrideGetHitResponse; unsigned char _pad22[0x26]; CShotDef* m_pShotDef;
    bool IsPlayGetHitResponse(CThing* target) { if (m_overrideGetHitResponse) return m_playGetHitResponse; if (m_pShotDef && !m_pShotDef->CanBeHitByMeleeStrike()) return false; if (!target || target->m_91_0 || IsPlayHitResponse(target)) return true; return false; } };

int main() {
    CHitParameters p; std::memset(&p, 0, sizeof p); CThing t; std::memset(&t, 0, sizeof t); CShotDef s;
    p.m_overrideGetHitResponse = true; p.m_playGetHitResponse = true; if (!p.IsPlayGetHitResponse(&t) || g_seq[0]) { std::printf("008685a0_TEST FAIL\n"); return 1; } p.m_playGetHitResponse = false; if (p.IsPlayGetHitResponse(&t)) { std::printf("008685a0_TEST FAIL\n"); return 1; }
    p.m_overrideGetHitResponse = false; p.m_pShotDef = &s; g_melee = false; if (p.IsPlayGetHitResponse(&t) || std::strcmp(g_seq, "M")) { std::printf("008685a0_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_melee = true; g_play = false; if (p.IsPlayGetHitResponse(&t) || std::strcmp(g_seq, "MP")) { std::printf("008685a0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_play = true; if (!p.IsPlayGetHitResponse(&t) || std::strcmp(g_seq, "MP")) { std::printf("008685a0_TEST FAIL\n"); return 1; } g_seq[0] = 0; t.m_91_0 = 1; g_play = false; if (!p.IsPlayGetHitResponse(&t) || std::strcmp(g_seq, "M")) { std::printf("008685a0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; p.m_pShotDef = 0; if (!p.IsPlayGetHitResponse(0) || g_seq[0]) { std::printf("008685a0_TEST FAIL\n"); return 1; }
    std::printf("008685a0_TEST PASS\n"); return 0;
}