#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CComp { int x; }; struct SEntry { CComp* m_p; long m_i; }; struct CVec { SEntry* m_first; SEntry* m_last; SEntry* m_end; unsigned long size() const { return m_last - m_first; } CComp* Last() const { return m_first[size() - 1].m_p; } };
static CComp* g_pushed = 0; static CComp* g_added = 0; struct CPVec { CComp** a; CComp** b; CComp** c; void push_back(CComp* const& v) { g_pushed = v; std::strcat(g_seq, "P"); } };
struct SRef { CComp* m_p; CComp* GetPComponent() const { return m_p; } }; static const SRef* g_ref = 0;
struct CBase { unsigned char _pad[0xb0]; CVec m_scrolling; unsigned char _p[0xb4]; CPVec m_dynamic; void AddChild(const SRef& r) { g_ref = &r; std::strcat(g_seq, "A"); } };
struct CList : CBase { void AddNonScrollingChild(const SRef& r) { CBase::AddChild(r); m_dynamic.push_back(r.GetPComponent()); } };

int main() {
    CList l; CComp c; SRef r; r.m_p = &c; l.AddNonScrollingChild(r); if (g_ref != &r || g_pushed != &c || std::strcmp(g_seq, "AP")) { std::printf("0053843b_TEST FAIL\n"); return 1; }
    std::printf("0053843b_TEST PASS\n"); return 0;
}
