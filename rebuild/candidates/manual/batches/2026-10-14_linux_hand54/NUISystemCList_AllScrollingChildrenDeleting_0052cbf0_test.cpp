#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_calls = 0; struct CComp { bool m_deleting; virtual bool IsDeleting() { ++g_calls; return m_deleting; } }; struct SEntry { CComp* m_p; long m_i; }; struct CVec { SEntry* m_first; SEntry* m_last; SEntry* m_end; unsigned long size() const { return m_last - m_first; } };
struct CList { unsigned char _pad[0xb0]; CVec m_v; bool All() { for (unsigned long i = 0; i < m_v.size(); ++i) { if (!m_v.m_first[i].m_p->IsDeleting()) return false; } return true; } };

int main() {
    CList l; CComp c[3]; SEntry e[3]; for (int i = 0; i < 3; ++i) { c[i].m_deleting = true; e[i].m_p = &c[i]; } l.m_v.m_first = e; l.m_v.m_last = e + 3; if (!l.All() || g_calls != 3) { std::printf("0052cbf0_TEST FAIL\n"); return 1; } c[1].m_deleting = false; g_calls = 0; if (l.All() || g_calls != 2) { std::printf("0052cbf0_TEST FAIL\n"); return 1; } l.m_v.m_last = e; if (!l.All()) { std::printf("0052cbf0_TEST FAIL\n"); return 1; }
    std::printf("0052cbf0_TEST PASS\n"); return 0;
}
