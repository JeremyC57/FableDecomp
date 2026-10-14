#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static bool g_failed = false; static void* g_state = 0; static long g_value = 0;
struct CFisticuffsState_00e83840 { virtual bool HasFailed() { std::strcat(g_seq, "H"); return g_failed; } };
struct CCreature_00e83840 { virtual long GetFailValue() { std::strcat(g_seq, "G"); return 77; } virtual void OnMemberFailed(CFisticuffsState_00e83840* s, long v) { std::strcat(g_seq, "O"); g_state = s; g_value = v; } };
struct CFisticuffsMember { long m_0; CCreature_00e83840* m_pCreature; CFisticuffsState_00e83840 m_state; void OnPredicateFail() { if (m_state.HasFailed()) m_pCreature->OnMemberFailed(&m_state, m_pCreature->GetFailValue()); } };
int main() {
    CFisticuffsMember m; CCreature_00e83840 c; m.m_pCreature = &c; g_failed = false; m.OnPredicateFail(); if (std::strcmp(g_seq, "H")) { std::printf("00e83840_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_failed = true; m.OnPredicateFail(); if (std::strcmp(g_seq, "HGO") || g_state != &m.m_state || g_value != 77) { std::printf("00e83840_TEST FAIL\n"); return 1; }
    std::printf("00e83840_TEST PASS\n"); return 0;
}
