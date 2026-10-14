#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SItem { long m_0, m_4; }; struct CGroup { unsigned char* m_pFlags; SItem* m_first; SItem* m_last; unsigned long size() const { return m_last - m_first; } }; struct SEntry { CGroup* m_pGroup; long m_4; }; struct CVec { SEntry* m_first; SEntry* m_last; SEntry* m_end; unsigned long size() const { return m_last - m_first; } };
struct COwner { unsigned char _pad[0x1c]; CVec m_groups; void MarkGroups(const long* index) { for (unsigned long i = 0; i < m_groups.size(); ++i) { CGroup* g = m_groups.m_first[i].m_pGroup; if (g->size() != 0) g->m_pFlags[*index] = 1; } } };

int main() {
    unsigned char f1[4] = {0}, f2[4] = {0}; SItem items[2]; CGroup g1, g2; g1.m_pFlags = f1; g1.m_first = items; g1.m_last = items + 2; g2.m_pFlags = f2; g2.m_first = items; g2.m_last = items; SEntry e[2]; e[0].m_pGroup = &g1; e[1].m_pGroup = &g2; COwner o; o.m_groups.m_first = e; o.m_groups.m_last = e + 2; long idx = 3; o.MarkGroups(&idx); if (f1[3] != 1 || f2[3] != 0 || f1[0]) { std::printf("00a8ef50_TEST FAIL\n"); return 1; }
    std::printf("00a8ef50_TEST PASS\n"); return 0;
}