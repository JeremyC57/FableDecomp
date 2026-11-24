#include <cstdio>
struct SSimulationEntry { void* m_pSimulation; long m_tag; SSimulationEntry() { } };
static int g_erased = 0, g_inserted = 0; static SSimulationEntry* g_eraseFirst = 0; static SSimulationEntry* g_eraseLast = 0; static SSimulationEntry* g_insertWhere = 0; static unsigned long g_insertCount = 99;
struct CRetailVector_simulation { SSimulationEntry* m_pFirst; SSimulationEntry* m_pLast; SSimulationEntry* m_pEnd; unsigned long size() const { return m_pLast - m_pFirst; }
    SSimulationEntry* erase(SSimulationEntry* f, SSimulationEntry* l) { ++g_erased; g_eraseFirst = f; g_eraseLast = l; m_pLast = f; return f; }
    void insert(SSimulationEntry* w, unsigned long n, const SSimulationEntry&) { ++g_inserted; g_insertWhere = w; g_insertCount = n; }
    void resize(unsigned long count) { if (count < size()) erase(m_pFirst + count, m_pLast); else insert(m_pLast, count - size(), SSimulationEntry()); } };
struct CSimulationTable_00ac41d0 { long m_a; CRetailVector_simulation m_entries; void Resize(unsigned long count) { m_entries.resize(count); } };
int main() {
    SSimulationEntry buf[4]; CSimulationTable_00ac41d0 t; t.m_entries.m_pFirst = buf; t.m_entries.m_pLast = buf + 3; t.m_entries.m_pEnd = buf + 4;
    t.Resize(1); if (g_erased != 1 || g_eraseFirst != buf + 1 || g_eraseLast != buf + 3 || g_inserted) { std::printf("00ac41d0_TEST FAIL\n"); return 1; }
    t.m_entries.m_pLast = buf + 1; t.Resize(3); if (g_inserted != 1 || g_insertWhere != buf + 1 || g_insertCount != 2) { std::printf("00ac41d0_TEST FAIL\n"); return 1; }
    t.Resize(1); if (g_inserted != 2 || g_insertCount != 0) { std::printf("00ac41d0_TEST FAIL\n"); return 1; }
    std::printf("00ac41d0_TEST PASS\n"); return 0;
}