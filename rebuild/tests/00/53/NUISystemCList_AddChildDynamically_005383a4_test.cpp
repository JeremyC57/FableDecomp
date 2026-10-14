#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CComp { int x; }; struct SEntry { CComp* m_p; long m_i; }; struct CVec { SEntry* m_first; SEntry* m_last; SEntry* m_end; unsigned long size() const { return m_last - m_first; } CComp* Last() const { return m_first[size() - 1].m_p; } };
static CComp* g_pushed = 0; static CComp* g_added = 0; struct CPVec { CComp** a; CComp** b; CComp** c; void push_back(CComp* const& v) { g_pushed = v; std::strcat(g_seq, "P"); } };
struct CBase { unsigned char _pad[0xb0]; CVec m_scrolling; unsigned char _p[0xb4]; CPVec m_dynamic; void AddChildDynamically(CComp* c) { g_added = c; std::strcat(g_seq, "A"); } };
struct CList : CBase { void AddChildDynamically(CComp* c) { CBase::AddChildDynamically(c); m_dynamic.push_back(m_scrolling.Last()); } };

int main() {
    CList l; CComp c1, c2; SEntry e[2]; e[0].m_p = &c1; e[1].m_p = &c2; l.m_scrolling.m_first = e; l.m_scrolling.m_last = e + 2; l.AddChildDynamically(&c1); if (g_added != &c1 || g_pushed != &c2 || std::strcmp(g_seq, "AP")) { std::printf("005383a4_TEST FAIL\n"); return 1; }
    std::printf("005383a4_TEST PASS\n"); return 0;
}