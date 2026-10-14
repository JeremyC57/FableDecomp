#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CGameflowAssistanceScript { void* m_pOwner; long m_state, m_timer, m_count; float m_rateA, m_rateB, m_rateC, m_rateD;
    CGameflowAssistanceScript(void* owner) { m_pOwner = owner; m_state = 0; m_timer = 0; m_count = 0; m_rateA = 0.12f; m_rateB = 0.03f; m_rateC = 0.2f; m_rateD = 0.01f; } };

int main() {
    int owner; CGameflowAssistanceScript s(&owner); if (s.m_pOwner != &owner || s.m_state || s.m_timer || s.m_count || s.m_rateA != 0.12f || s.m_rateB != 0.03f || s.m_rateC != 0.2f || s.m_rateD != 0.01f) { std::printf("00877160_TEST FAIL\n"); return 1; }
    std::printf("00877160_TEST PASS\n"); return 0;
}
