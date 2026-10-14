#include <cstdio>
struct SSimulationSlot_00bb6420 { long m_a; void* m_pBuffer; bool m_inUse; unsigned char _pad[0x2b]; bool IsInUse() { return m_inUse; } };
struct CEngineParticleSimulationRenderData { unsigned char _pad[4]; SSimulationSlot_00bb6420 m_slots[2];
    bool IsBusy() { for (long i = 0; i < 2; ++i) { if (m_slots[i].m_pBuffer && m_slots[i].IsInUse()) return true; } return false; } };
int main() {
    CEngineParticleSimulationRenderData d; d.m_slots[0].m_pBuffer = 0; d.m_slots[1].m_pBuffer = 0; d.m_slots[0].m_inUse = true; d.m_slots[1].m_inUse = true;
    if (d.IsBusy()) { std::printf("00bb6420_TEST FAIL\n"); return 1; }
    d.m_slots[1].m_pBuffer = &d; if (!d.IsBusy()) { std::printf("00bb6420_TEST FAIL\n"); return 1; }
    d.m_slots[1].m_inUse = false; if (d.IsBusy()) { std::printf("00bb6420_TEST FAIL\n"); return 1; }
    d.m_slots[0].m_pBuffer = &d; if (!d.IsBusy()) { std::printf("00bb6420_TEST FAIL\n"); return 1; }
    std::printf("00bb6420_TEST PASS\n"); return 0;
}