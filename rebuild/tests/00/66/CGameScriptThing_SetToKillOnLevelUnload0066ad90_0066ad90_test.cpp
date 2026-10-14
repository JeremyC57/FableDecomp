#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = ""; static long g_arg[4];
struct CCharString { const char* m_p; CCharString() : m_p("") { } CCharString(const CCharString& o) : m_p(o.m_p) { std::strcat(g_seq, "c"); } CCharString(const char* t, long) : m_p(t) { std::strcat(g_seq, "e"); } CCharString& operator=(const CCharString& o) { m_p = o.m_p; std::strcat(g_seq, "="); return *this; } };
struct CTCGraphicAppearance_Retail { int x; };
struct CThing_Retail { unsigned char m_type; CTCGraphicAppearance_Retail* m_pAppearance; unsigned long m_flags; CCharString m_dataString; bool m_dead;
    void SetToKillOnLevelUnload(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; } void OnLevelUnloadKill_008368f0(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; } void KillOnLevelUnload_008368d0(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; }
    long GetCurrentStateGroupType() { std::strcat(g_seq, "S"); return 77; } void SetDataString(const CCharString& t) { m_dataString = t; } };
static CThing_Retail g_thing; static CThing_Retail* g_item = &g_thing;
struct CIntelligentPointer_Retail { virtual void v00() { } void* m_pItem; CThing_Retail* GetPItem() { std::strcat(g_seq, "g"); return g_item; } CThing_Retail* GetPItem() const { std::strcat(g_seq, "g"); return g_item; } };
static void reset() { std::memset(&g_thing, 0, sizeof g_thing); g_seq[0] = 0; g_item = &g_thing; }
static void* g_pGameState_013b8394 = 0;
struct CGameScriptThing { unsigned char _pad[0xc]; CIntelligentPointer_Retail m_thing; void SetToKill(long kill) { if (!g_pGameState_013b8394) return; if (m_thing.GetPItem()) { if (!m_thing.GetPItem()->m_dead) m_thing.GetPItem()->OnLevelUnloadKill_008368f0(kill); } } };

int main() {
    CGameScriptThing s; g_pGameState_013b8394 = &s; reset(); s.SetToKill(3); if (std::strcmp(g_seq, "gggK") || g_arg[0] != 3) { std::printf("0066ad90_TEST FAIL\n"); return 1; } reset(); g_thing.m_dead = true; s.SetToKill(4); if (std::strcmp(g_seq, "gg")) { std::printf("0066ad90_TEST FAIL\n"); return 1; } reset(); g_item = 0; s.SetToKill(5); if (std::strcmp(g_seq, "g")) { std::printf("0066ad90_TEST FAIL\n"); return 1; } g_pGameState_013b8394 = 0; reset(); s.SetToKill(6); if (g_seq[0]) { std::printf("0066ad90_TEST FAIL\n"); return 1; }
    std::printf("0066ad90_TEST PASS\n"); return 0;
}