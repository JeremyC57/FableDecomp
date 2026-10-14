#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SState { long m_pending; }; static long g_calls = 0; struct CComp { SState m_state; virtual SState* GetDeletionState() { ++g_calls; return &m_state; } }; struct CVec { CComp** m_first; CComp** m_last; CComp** m_end; unsigned long size() const { return m_last - m_first; } };
struct CList { unsigned char _pad[0x164]; CVec m_v; bool Any() { for (unsigned long i = 0; i != m_v.size(); ++i) { if (m_v.m_first[i]->GetDeletionState()->m_pending != 0) return true; } return false; } };

int main() {
    CList l; CComp c[3]; CComp* p[3]; for (int i = 0; i < 3; ++i) { c[i].m_state.m_pending = 0; p[i] = &c[i]; } l.m_v.m_first = p; l.m_v.m_last = p + 3; if (l.Any() || g_calls != 3) { std::printf("005362f2_TEST FAIL\n"); return 1; } c[1].m_state.m_pending = 1; g_calls = 0; if (!l.Any() || g_calls != 2) { std::printf("005362f2_TEST FAIL\n"); return 1; } l.m_v.m_last = p; if (l.Any()) { std::printf("005362f2_TEST FAIL\n"); return 1; }
    std::printf("005362f2_TEST PASS\n"); return 0;
}
