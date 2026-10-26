#include <cstdio>
struct CSimulation_00b80b40 { int m_n; CSimulation_00b80b40() : m_n(0) { } virtual void Run() { ++m_n; } };
struct SSimulationEntry { CSimulation_00b80b40* m_pSimulation; long m_tag; };
class CEnginePrimitiveSimulationInfo { public: unsigned char _pad[8]; SSimulationEntry* m_pFirst; SSimulationEntry* m_pLast;
    void RunSimulation() { for (SSimulationEntry* entry = m_pFirst; entry != m_pLast; ++entry) { if (entry->m_pSimulation) entry->m_pSimulation->Run(); } } };
int main() {
    CSimulation_00b80b40 a, b; SSimulationEntry es[3] = { { &a, 0 }, { 0, 0 }, { &b, 0 } }; CEnginePrimitiveSimulationInfo info; info.m_pFirst = es; info.m_pLast = es + 3;
    info.RunSimulation(); if (a.m_n != 1 || b.m_n != 1) { std::printf("00b80b40_TEST FAIL\n"); return 1; }
    info.m_pLast = es; info.RunSimulation(); if (a.m_n != 1) { std::printf("00b80b40_TEST FAIL\n"); return 1; }
    std::printf("00b80b40_TEST PASS\n"); return 0;
}